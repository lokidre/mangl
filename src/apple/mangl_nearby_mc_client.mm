#import "mangl_nearby_mc_client.h"

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


// MARK: MNearbyClientMC
@interface ManglNearbyClientMC() {
    net::AppleNearbyClient* nearbyClient_;

    net::DiscoveryInfo discoveryInfo_;
    bool isDiscovering_;
    
    PeerKey peerKey_;
}
@property (nonatomic) MCSession* session;
@property (nonatomic) MCNearbyServiceBrowser* browser;
@end


@implementation ManglNearbyClientMC

+ (instancetype)instance {
    static ManglNearbyClientMC* sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[ManglNearbyClientMC alloc] init];
    });

    return sharedInstance;
}

-(instancetype)init {
    self = [super init];
    
    nearbyClient_ = nullptr;

    _session = nil;
    
    _browser = nil;
    discoveryInfo_ = {};
    isDiscovering_ = false;
    
    peerKey_ = 0;

    return self;
}

- (void)startDiscovery:(net::AppleNearbyClient*)client di:(net::DiscoveryInfoA)di {
    MDebugFunc();
    
    // TODO: move to location permissions separately
    [ManglMainViewController.instance requestLocationPermissions];

    
    discoveryInfo_ = di;
    nearbyClient_ = client;

    auto playerName = [self formatBonjourPlayerName:di.alias];
    auto serviceName = [self formatBonjourServiceName:di.proto];

    auto localPeerId = [self getLocalPeerID:playerName];

    //auto diDict = [self discoveryInfoToDict:di];

    if (!_browser) {
        _browser = [[MCNearbyServiceBrowser alloc] initWithPeer:localPeerId
                                                    serviceType:serviceName];
        _browser.delegate = self;
    }

    if (!_session) {
        _session = [[MCSession alloc] initWithPeer:localPeerId
                                  securityIdentity:nil
                              encryptionPreference:MCEncryptionOptional];
        _session.delegate = self;
    }

    [_browser startBrowsingForPeers];
    
    isDiscovering_ = true;
}


- (void)stopDiscovery {
    isDiscovering_ = false;
    
    if (_browser) {
        [_browser stopBrowsingForPeers];
        _browser = nil;
    }
}

- (bool)isDiscovering {
    return isDiscovering_;
}

- (void)connect:(net::AppleNearbyClient*)client
            url:(StrView)url
  discoveryInfo:(net::DiscoveryInfoA)di
        timeout:(mangl::Timestamp)timeout
{
    nearbyClient_ = client;
    
    if (auto peerId = [self findPeerByKey:peerKey_]) {
        auto diData = discoveryInfo_.build();
        auto context = [NSData dataWithBytes:diData.data() length:diData.length()];

        [_browser invitePeer:peerId
                   toSession:_session
                 withContext:context
                     timeout:timeout];
    }
}

- (void)disconnect {
    if (_session) {
        [_session disconnect];
        _session = nil;
    }
}

- (void)sendData:(DataParamA)data unreliable:(bool)unreliable
{
    if (!_session || !peerKey_ || !nearbyClient_) {
        MDebugTrap();
        return;
    }

    // I don't know if it's a good idea to have only one peer
    auto peer = [self findPeerByKey:peerKey_];

    if (!peer) {
        MDebugTrap();
        return;
    }

    NSError *error{};
    auto nsdata = [NSData dataWithBytes:data.data() length:data.size()];
    auto mode = unreliable ? MCSessionSendDataUnreliable : MCSessionSendDataReliable;
    auto result = [_session sendData:nsdata 
                             toPeers:@[peer]
                            withMode:mode
                               error:&error];

    if (!result)
        MDebugFunc(":Error: %s", error);
}

-(void)shutdown {
    [self stopDiscovery];
    
    nearbyClient_ = nullptr;
    peerKey_ = 0;
    
    if (_session) {
        [_session disconnect];
        _session = nil;
    }

    if (_browser) {
        [_browser stopBrowsingForPeers];
        _browser = nil;
    }
}


//
// MARK: MCNearbyServiceBrowserDelegate
//
- (void)browser:(MCNearbyServiceBrowser *)browser didNotStartBrowsingForPeers:(NSError *)error
{
    MDebugFunc("%s", error.localizedDescription);
    [ManglMainViewController showAlert:error.localizedDescription withTitle:@"Connection Error"];
}

- (void)browser:(MCNearbyServiceBrowser *)browser
      foundPeer:(MCPeerID *)peerID
withDiscoveryInfo:(nullable NSDictionary<NSString*, NSString*>*)diDict
{
    MDebugFunc("%s", peerID.displayName);

    auto peerInfo = [self onNewPeerDiscovered:peerID];
    peerKey_ = peerInfo.key;

    auto remoteInfo = [self discoveryInfoFromDict:diDict];
    if (nearbyClient_)
        nearbyClient_->getEventsQueue().pushFound(peerInfo.key, remoteInfo.build());

    auto diData = discoveryInfo_.build();
    auto context = [NSData dataWithBytes:diData.data() length:diData.length()];

    [_browser invitePeer:peerID toSession:_session withContext:context timeout:60];
}

- (void)browser:(MCNearbyServiceBrowser *)browser lostPeer:(MCPeerID *)peerID
{
    MDebugFunc("%s", peerID.displayName);

    if (auto pk = [self getPeerKey:peerID]) {
        if (nearbyClient_)
            nearbyClient_->getEventsQueue().pushState(pk, PeerState::Lost);
        [self removePeer:peerID];
    }
}



//
// MARK: MCSessionDelegate (Client)
//
- (void)session:(MCSession *)session
           peer:(nonnull MCPeerID *)peerID
 didChangeState:(MCSessionState)state
{
    auto pk = [self getPeerKey:peerID];
    PeerState ps{};

    switch (state) {
    case MCSessionStateNotConnected: ps = PeerState::Disconnected; break;
    case MCSessionStateConnecting:   ps = PeerState::Connecting; break;
    case MCSessionStateConnected:    ps = PeerState::Connected; break;
    }

    MDebugFunc(": %d - %c",  pk, M::getPeerStateString(ps));

    if (pk && nearbyClient_)
        nearbyClient_->getEventsQueue().pushState(pk, ps);
}

- (void)session:(MCSession *)session
 didReceiveData:(NSData *)data
       fromPeer:(MCPeerID *)peerID
{
    //MDebugFunc(": %d",  peerID.displayName);

    auto pk = [self getPeerKey:peerID];

    if (pk && nearbyClient_)
        nearbyClient_->getEventsQueue().pushData(pk, {data.bytes, data.length});
}

- (void)session:(nonnull MCSession*)session didFinishReceivingResourceWithName:(nonnull NSString*)resourceName
       fromPeer:(nonnull MCPeerID*)peerID
          atURL:(nullable NSURL*)localURL
      withError:(nullable NSError*)error
{
    MDebugFunc();
}


- (void)session:(nonnull MCSession*)session didReceiveStream:(nonnull NSInputStream*)stream
       withName:(nonnull NSString*)streamName
       fromPeer:(nonnull MCPeerID*)peerID
{
    MDebugFunc();
}


- (void)session:(nonnull MCSession*)session didStartReceivingResourceWithName:(nonnull NSString*)resourceName
       fromPeer:(nonnull MCPeerID*)peerID
   withProgress:(nonnull NSProgress*)progress
{
    MDebugFunc();
}
//- (void)connect:(mangl::PeerKey)key timeout:(double)timeout {
//}

@end


// MARK: C++ MNetworkNearbyServer and MNetworkNearbyClient
namespace mangl::net {


void AppleNearbyClient::doStartDiscovery(DiscoveryInfoA di) {
    [ManglNearbyClientMC.instance startDiscovery:this di:di];
}

void AppleNearbyClient::doStopDiscovery() {
    [ManglNearbyClientMC.instance stopDiscovery];
}

bool AppleNearbyClient::doIsDiscovering() {
    return [ManglNearbyClientMC.instance isDiscovering];
}


void AppleNearbyClient::doConnect() {
    [ManglNearbyClientMC.instance connect:this
                                      url:connectParams_.url
                            discoveryInfo:connectParams_.di
                                  timeout:connectParams_.timeout];
}

void AppleNearbyClient::doDisconnect() {
    [ManglNearbyClientMC.instance disconnect];
}

void AppleNearbyClient::doSend(NetworkDataSendParamsA params) {
    [ManglNearbyClientMC.instance sendData:params.data
                                unreliable:params.unreliable];
}

void AppleNearbyClient::doShutdown() {
    [ManglNearbyClientMC.instance shutdown];
}


}  // namespace mangl

#endif
