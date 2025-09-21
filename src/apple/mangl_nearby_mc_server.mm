#import "mangl_nearby_mc_server.h"

#include "mangl_apple_nearby.h"
#include "mangl_imacos_util.h"


//#if MANGL_MULTIPLAYER
#if defined(MANGL_MULTIPLAYER) && !defined(MANGL_IXWEBSOCKET)



// #define MANGL_LOCAL_LOG 1
// #include "../core/mangl_local_log.h"


#if APEAL_OS_IPHONE
#import "../ios/mangl_main_vc.h"
#import "../ios/mangl_main_vc+except.h"
#else
#import "../macos/mangl_main_view.h"
#import "../macos/mangl_main_view_controller.h"
#endif


#include "mangl_imacos_util.h"

#include "../core/mangl_framework.h"
#include "../core/mangl_util.h"


using namespace mangl;

// MARK: MNearbyServerMC
@interface ManglNearbyServerMC() {
    net::AppleNearbyServer* nearbyServer_;
    net::EventQueue* inQueue_;
    net::DiscoveryInfo discoveryInfo_;

    bool isAdvertising_;
}

@property (nonatomic) MCSession* session;
@property (nonatomic) MCNearbyServiceAdvertiser* advertiser;

@end

@implementation ManglNearbyServerMC

// MARK: Server initialization
+ (instancetype)instance {
    static ManglNearbyServerMC *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[ManglNearbyServerMC alloc] init];
    });

    return sharedInstance;
}

- (instancetype)init {
    self = [super init];

    nearbyServer_ = nullptr;
    inQueue_ = nullptr;
    
    _session = nil;
    
    _advertiser = nil;
    isAdvertising_ = false;

    return self;
}

- (void)start:(net::AppleNearbyServer*)server di:(net::DiscoveryInfoA)di {
    
    // TODO: move to location permissions separately
    [ManglMainViewController.instance requestLocationPermissions];
    
    nearbyServer_ = server;
    inQueue_ = &nearbyServer_->getEventsQueue();
    discoveryInfo_ = di;
}

-(void)shutdown {
    if (_session) {
        [_session disconnect];
        _session = nil;
    }

    if (_advertiser) {
        [_advertiser stopAdvertisingPeer];
        _advertiser = nil;
    }
    
    nearbyServer_ = nullptr;
    inQueue_ = nullptr;
}



- (void)startAdvertising {
    MDebugFunc();
    
    const auto& di = discoveryInfo_;
    
    auto playerName = toNSString(di.alias);
    auto serviceName = [self formatBonjourServiceName:di.proto];

    auto localPeerId = [self getLocalPeerID:playerName];

    auto diDict = [self discoveryInfoToDict:di];

    if (!_session) {
        _session = [[MCSession alloc] initWithPeer:localPeerId
                                  securityIdentity:nil
                              encryptionPreference:MCEncryptionOptional];
        _session.delegate = self;
    }

    if (!_advertiser) {
        _advertiser = [[MCNearbyServiceAdvertiser alloc] initWithPeer:localPeerId
                                                        discoveryInfo:diDict
                                                          serviceType:serviceName];
        _advertiser.delegate = self;
    }

    [_advertiser startAdvertisingPeer];
    isAdvertising_ = true;
}

- (void)stopAdvertising
{
    isAdvertising_ = false;
    
    if (_advertiser) {
        [_advertiser stopAdvertisingPeer];
        // self.mcNearbyServiceAdvertiser = nil ;
    }
}

- (bool)isAdvertising {
    return isAdvertising_;
}

- (void)sendData:(PeerKey)key data:(DataParamA)data unreliable:(bool)unreliable
{
    auto peer = [self findPeerByKey:key];

    if (!peer) {
        MDebugTrap();
        return;
    }

    NSError* error{};
    auto nsdata = [NSData dataWithBytes:data.data() length:data.size()];
    auto mode = unreliable ? MCSessionSendDataUnreliable : MCSessionSendDataReliable;
    auto result = [_session sendData:nsdata toPeers:@[peer] withMode:mode error:&error];

    if (!result)
        MDebugFunc(":Error: %s", error);

}

-(void)disconnect:(PeerKey)peerKey
{
    // Don't know how to disconnect particular peer
//    if (_session)
//        [_session disconnect];
}


//
// MARK: MCNearbyServiceAdvertiserDelegate
//
- (void)advertiser:(MCNearbyServiceAdvertiser *)advertiser didNotStartAdvertisingPeer:(NSError *)error
{
    MDebugFunc("%s", error.description);
    [ManglMainViewController showError:error withTitle:@"Connection Error"];
}

- (void)advertiser:(MCNearbyServiceAdvertiser*)advertiser didReceiveInvitationFromPeer:(MCPeerID *)peerID
       withContext:(NSData*)context
 invitationHandler:(void (^)(BOOL accept, MCSession* session))invitationHandler
{
    MDebugFunc("%s, data:%d", peerID.displayName, context.length);
    
    using namespace net;

    auto pk = [self getPeerKey:peerID];

    bool shouldAsk = !pk;

    DiscoveryInfo di;
    if (!di.parseData({context.bytes, context.length}))
        return;

    if (!pk) {
        pk = [self addPeer:peerID];
    }

    bool response = false;
    if (shouldAsk)
        if (nearbyServer_ && nearbyServer_->onPeerShouldAsk)
            nearbyServer_->onPeerShouldAsk(&response, pk, fromNSString(peerID.displayName));

    shouldAsk = response;

    if (shouldAsk) {
        NSString* title = [NSString stringWithFormat:@"Connection from %@", peerID.displayName];
        NSString* message = @"Do you accept the connection request?";

#if APEAL_OS_IPHONE
        auto alert = [UIAlertController alertControllerWithTitle:title
                                                         message:message
                                                  preferredStyle:UIAlertControllerStyleAlert];

        auto rejectAction = [UIAlertAction actionWithTitle:@"Reject"
                                                     style:UIAlertActionStyleDefault
                                                   handler:^(UIAlertAction *) {
            invitationHandler(false, nil);
        }];
        [alert addAction:rejectAction];
        
        auto acceptAction = [UIAlertAction actionWithTitle:@"Accept"
                                                     style:UIAlertActionStyleDefault
                                                   handler:^(UIAlertAction *) {
            auto& queue = self->nearbyServer_->getEventsQueue();
            queue.pushFound(pk, di.build());
            queue.pushState(pk, PeerState::Accepted);
            invitationHandler(true, self.session);
        }];
        [alert addAction:acceptAction];
        
        [ManglMainViewController.instance presentViewController:alert animated:YES completion:nil];
#else
        auto alert = [[NSAlert alloc] init];
        [alert setShowsHelp:NO];
        [alert setAlertStyle:NSAlertStyleInformational];
        [alert setMessageText:title];
        [alert setInformativeText: message];
        [alert addButtonWithTitle: @"Accept"];
        [alert addButtonWithTitle: @"Decline"];

        auto response = [alert runModal];
        switch (response) {
        case NSAlertFirstButtonReturn:
            nearby_->onPeerAccepted(peerInfo.key);
            invitationHandler(true, self.session);
            break;
        case NSAlertSecondButtonReturn:
            invitationHandler(false, nil);
            break;
        }
#endif
    } else {
        auto& eq = self->nearbyServer_->getEventsQueue();
        eq.pushFound(pk, di.build());
        eq.pushState(pk, PeerState::Accepted);
        invitationHandler(true, self.session);
    }
}


//
// MARK: MCSessionDelegate (Server)
//
- (void)session:(MCSession *)session 
 didReceiveData:(NSData *)data
       fromPeer:(MCPeerID *)peerID
{
    //MDebugFunc(": %d",  peerID.displayName);

    auto pk = [self getPeerKey:peerID];
    
    if (pk && nearbyServer_)
        nearbyServer_->getEventsQueue().pushData(pk, {data.bytes, data.length});
}

- (void)session:(MCSession *)session  
           peer:(nonnull MCPeerID *)peerID
 didChangeState:(MCSessionState)state
{
    auto pk = [self getPeerKey:peerID];
    PeerState peerState{};

    switch (state) {
    case MCSessionStateNotConnected: peerState = PeerState::Disconnected;  break;
    case MCSessionStateConnecting:   peerState = PeerState::Connecting;    break;
    case MCSessionStateConnected:    peerState = PeerState::Connected;     break;
    }

    MDebugFunc(": %d: %s, %s", pk, M::getPeerStateString(peerState), toString(peerID.displayName));

    if (pk && nearbyServer_)
        nearbyServer_->getEventsQueue().pushState(pk, peerState);
}

- (void)session:(nonnull MCSession *)session didFinishReceivingResourceWithName:(nonnull NSString *)resourceName
       fromPeer:(nonnull MCPeerID *)peerID
          atURL:(nullable NSURL *)localURL
      withError:(nullable NSError *)error
{
    MDebugFunc();
}


- (void)session:(nonnull MCSession *)session didReceiveStream:(nonnull NSInputStream *)stream
       withName:(nonnull NSString *)streamName
       fromPeer:(nonnull MCPeerID *)peerID
{
    MDebugFunc();
}


- (void)session:(nonnull MCSession *)session didStartReceivingResourceWithName:(nonnull NSString *)resourceName
       fromPeer:(nonnull MCPeerID *)peerID
   withProgress:(nonnull NSProgress *)progress
{
    MDebugFunc();
}


@end


// MARK: C++ MNetworkNearbyServer and MNetworkNearbyClient
namespace mangl::net {

void AppleNearbyServer::doStart() {
    [ManglNearbyServerMC.instance start:this di:startParams_.di];
}


void AppleNearbyServer::doStartAdvertising() {
    [ManglNearbyServerMC.instance startAdvertising];
}

void AppleNearbyServer::doStopAdvertising() {
    [ManglNearbyServerMC.instance stopAdvertising];
}

bool AppleNearbyServer::doIsAdvertising() {
    return [ManglNearbyServerMC.instance isAdvertising];
}


void AppleNearbyServer::doSend(NetworkDataSendParamsA param) {
    [ManglNearbyServerMC.instance sendData:param.pk data:param.data unreliable:param.unreliable];
}

void AppleNearbyServer::doShutdown() {
    [ManglNearbyServerMC.instance shutdown];
}

void AppleNearbyServer::doDisconnect(PeerKey pk) {
    [ManglNearbyServerMC.instance disconnect:pk];
}

}  // namespace mangl


#endif

