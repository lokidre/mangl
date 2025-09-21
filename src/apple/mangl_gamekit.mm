#import "mangl_gamekit.h"

#if APEAL_OS_IPHONE
#import "../ios/mangl_main_vc.h"
#else
#import "../macos/mangl_main_vc.h"
#endif

#include "mangl_imacos_util.h"

#include "../core/mangl_framework.h"
#include "../core/mangl_util.h"



#if MANGL_MULTIPLAYER

using namespace mangl;

@interface ManglGameKitMatchmaker() {
    net::GameKitClient* client_;
    net::GameKitServer* server_;
    net::GameKitMatchmaker* matchmaker_;
}

@property (nonatomic) NSMutableDictionary* gkPlayers;
@property (nonatomic) GKMatch* gkMatch;

@end


@implementation ManglGameKitMatchmaker

+ (instancetype)instance {
    static ManglGameKitMatchmaker* sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
      sharedInstance = [[ManglGameKitMatchmaker alloc] init];
    });

    return sharedInstance;
}

- (instancetype)init {
    self = [super init];

    client_ = nullptr;
    server_ = nullptr;
    matchmaker_ = nullptr;
    
    _gkPlayers = [[NSMutableDictionary alloc] init];

    return self;
}

- (void)shutdown {
//    bool skipShutdown = false;
    
////    if (gameKit_) {
//    if (gameKit == gkMatchmaker_ || gameKit->isMatchmaker) {
//    //if (gameKit->isMatchmaker) {
//        skipShutdown = true;
//        gkMatchmaker_ = nullptr;
//    }
////    }
    
    if (_gkMatch) {
        [_gkMatch disconnect];
        _gkMatch = nil;
    }
    
    [_gkPlayers removeAllObjects];

    client_ = nullptr;
    server_ = nullptr;
    matchmaker_ = nullptr;
}



//
// MARK: Helper funcions
//
- (mmp::MpPlayer)fromGKPlayer:(GKPlayer *)player {
    mmp::MpPlayer p{};
    p.id = fromNSString(player.gamePlayerID);
    p.alias = fromNSString(player.alias);
    p.name = fromNSString(player.displayName);
    return p;
}

- (PeerKey)findPlayerKey:(NSString *)playerId {
    for (NSNumber* key in _gkPlayers) {
        GKPlayer* player = [_gkPlayers objectForKey:key];
        if ([playerId isEqualToString:player.gamePlayerID])
            return key.intValue;
    }

    return 0;
}

- (PeerKey)onNewPlayer:(GKPlayer *)player {
    // Generate new uuid
    auto key = net::NetworkBaseCommon::generatePeerKey();

    _gkPlayers[@(key)] = player;

    mmp::MpPeer peerInfo;
    peerInfo.key = key;
    
    auto mp = [self fromGKPlayer:player];
    peerInfo.playerId = mp.id;
    peerInfo.playerAlias = mp.alias;
    peerInfo.playerName = mp.name;

    return peerInfo.key;
}

- (GKPlayer *)playerByKey:(PeerKey)key {
    return (GKPlayer *)[_gkPlayers objectForKey:@(key)];
}

- (void)startMatchmakingGui:(net::GameKitMatchmaker*)mm
                     params:(mmp::MpGameParamsA)params
{
    matchmaker_ = mm;
    
    auto request = [[GKMatchRequest alloc] init];
    request.minPlayers = params.minPlayers == 0 ? 2 : params.minPlayers;
    request.defaultNumberOfPlayers = params.defPlayers == 0 ? 2 : params.defPlayers;
    request.maxPlayers = params.maxPlayers == 0 ? 2 : params.maxPlayers;
    request.playerGroup = 1; //params.group;
    request.recipientResponseHandler = ^(GKPlayer* player, GKInviteeResponse response) {
        MDebugFunc();
        if (response == GKInviteeResponseAccepted) {
            [GKMatchmaker.sharedMatchmaker finishMatchmakingForMatch:self.gkMatch];
        }
    };

    auto mmvc = [[GKMatchmakerViewController alloc] initWithMatchRequest:request];
    mmvc.matchmakerDelegate = self;

#if APEAL_OS_IPHONE
    [ManglMainViewController.instance presentViewController:mmvc animated:YES completion:nil];
#else
    [ManglMainViewController.instance presentViewControllerAsModalWindow:mmvc];
#endif
}

-(void)stopMatchmaking {
    matchmaker_ = nullptr;
}

//
// Server side functionality
//
- (void)onConnect:(net::GameKitClient*)client {
    client_ = client;
}

- (void)startServer:(net::GameKitServer*)server {
    server_ = server;
}

- (void)serverDisconnect:(PeerKey)pk {
    
}

- (void)sendData:(net::NetworkDataSendParamsA)params {
    auto nsdata = [NSData dataWithBytes:params.data.data() length:params.data.size()];
    auto mode = params.unreliable ? GKMatchSendDataUnreliable : GKMatchSendDataReliable;
    NSError* error{};
    
    if (params.pk) {
        auto player = [self playerByKey:params.pk];

        MDebugTrapAssert(player);
        if (!player)
            return;

        if (_gkMatch)
            [_gkMatch sendData:nsdata toPlayers:@[player] dataMode:mode error:&error];
    } else {
        if (_gkMatch)
            [_gkMatch sendDataToAllPlayers:nsdata withDataMode:mode error:&error];
    }
}


//
// MARK: GKMatchmakerViewControllerDelegate
//
- (void)matchmakerViewController:(GKMatchmakerViewController *)viewController
                   didFindMatch:(GKMatch *)match
{
    MDebugFunc();

    _gkMatch = match;
    _gkMatch.delegate = self;


    dispatch_async(dispatch_get_main_queue(), ^{
        
#if APEAL_OS_IPHONE
        [ManglMainViewController.instance dismissViewControllerAnimated:YES completion:nil];
#else
        [ManglMainViewController.instance dismissViewController:viewController];
#endif
        
        
        if (match.expectedPlayerCount == 0) {
            std::vector<mmp::MpPeer> peers;
            mmp::MpPeer localPeer{};
            auto localPlayer = GKLocalPlayer.localPlayer;
            localPeer.key = [self onNewPlayer:localPlayer];
            localPeer.playerId = fromNSString(localPlayer.gamePlayerID);
            localPeer.playerAlias = fromNSString(localPlayer.alias);
            localPeer.playerName = fromNSString(localPlayer.displayName);
            
            MDebugFunc("local peer key: %d (%s)", localPeer.key, localPeer.playerName);
            
            for (GKPlayer* player in match.players) {
                auto& peer = peers.emplace_back();
                peer.key = [self onNewPlayer:player];
                peer.playerId = fromNSString(player.gamePlayerID);
                peer.playerAlias = fromNSString(player.alias);
                peer.playerName = fromNSString(player.displayName);
                
                MDebugFunc("remove peer key: %d (%s)", peer.key, peer.playerName);
                
                //sortedPeers[peer.player.id] = peer;
                //if (peer.player.id == localPlayer.id)
                //    localPeer = peer;
            }
            
            if (self->matchmaker_)
                self->matchmaker_->processMatchmakingComplete(peers, localPeer);
            
            // We need to send "connected" signal just in case
        }
        
    });
    
}

- (void)matchmakerViewController:(GKMatchmakerViewController *)viewController
            didFindHostedPlayers:(NSArray<GKPlayer *> *)players
{
    MDebugFunc();

    dispatch_async(dispatch_get_main_queue(), ^{
        
#if APEAL_OS_IPHONE
        [ManglMainViewController.instance dismissViewControllerAnimated:YES completion:nil];
#else
        [ManglMainViewController.instance dismissViewController:viewController];
#endif
    });
}

- (void)matchmakerViewController:(GKMatchmakerViewController *)viewController
                didFailWithError:(NSError *)error {
    MDebugFunc("%s", error);

    dispatch_async(dispatch_get_main_queue(), ^{
#if APEAL_OS_IPHONE
        [ManglMainViewController.instance dismissViewControllerAnimated:YES completion:nil];
#else
        [ManglMainViewController.instance dismissViewController:viewController];
#endif
        
        [ManglMainViewController showError:error withTitle:@"Matchmaking Error"];
    });
}

- (void)matchmakerViewControllerWasCancelled:(GKMatchmakerViewController *)viewController {
    MDebugFunc();

    dispatch_async(dispatch_get_main_queue(), ^{
        
#if APEAL_OS_IPHONE
        [ManglMainViewController.instance dismissViewControllerAnimated:YES completion:nil];
#else
        [ManglMainViewController.instance dismissViewController:viewController];
#endif
        
    });
}



//
// MARK: GKMatchDelegate
//
- (void)match:(GKMatch *)match didReceiveData:(NSData *)data
                                 forRecipient:(GKPlayer *)recipient
                             fromRemotePlayer:(GKPlayer *)player
{
    auto senderKey = [self findPlayerKey:player.gamePlayerID];
    auto receiverKey = [self findPlayerKey:recipient.gamePlayerID];

//    MDebugFunc("Received data from: %d (%s)", senderKey, fromNSString(player.displayName));

    if (senderKey) {
        if (client_)
            client_->getEventsQueue().pushData(receiverKey, {data.bytes, data.length});
        if (server_)
            server_->getEventsQueue().pushData(senderKey, {data.bytes, data.length});
    }
}

- (void)match:(GKMatch *)match didReceiveData:(NSData*)data
                             fromRemotePlayer:(GKPlayer*)player {
    //MDebugFunc();

    auto pk = [self findPlayerKey:player.gamePlayerID];

    if (pk) {
        if (client_)
            client_->getEventsQueue().pushData(pk, {data.bytes, data.length});
        if (server_)
            server_->getEventsQueue().pushData(pk, {data.bytes, data.length});
    }
}

- (void)match:(GKMatch*)match player:(GKPlayer *)player
            didChangeConnectionState:(GKPlayerConnectionState)connectionState
{

    auto pk = [self findPlayerKey:player.gamePlayerID];

    PeerState peerState{};

    switch (connectionState) {
    case GKPlayerStateDisconnected: peerState = PeerState::Disconnected; break;
    case GKPlayerStateUnknown:      peerState = PeerState::Disconnected; break;
    case GKPlayerStateConnected:    peerState = PeerState::Connected;    break;
    }
    
    MDebugFunc("%s, %c", player.gamePlayerID, (char)peerState);

    if (pk) { 
        if (client_)
            client_->getEventsQueue().pushState(pk, peerState);
        if (server_)
            server_->getEventsQueue().pushState(pk, peerState);
    }
}

- (void)match:(GKMatch*)match didFailWithError:(NSError *)error {
    dispatch_async(dispatch_get_main_queue(), ^{
        [ManglMainViewController showError:error withTitle:@"Match Error"];
    });
    //online_->onMatchError((int)error.code, toString(error.localizedDescription));
}

- (BOOL)match:(GKMatch*)match shouldReinviteDisconnectedPlayer:(GKPlayer *)player
{
    auto key = [self findPlayerKey:player.gamePlayerID];

    if (!key)
        return NO;

    // TODO: option return online_->onShouldReinviteDisconnectedPlayer(key);
    return YES;
}

//
// MARK: GKInviteEventListener
//
- (void)player:(GKPlayer *)player didAcceptInvite:(GKInvite *)invite {
    MDebugFunc();

    dispatch_async(dispatch_get_main_queue(), ^{
        MultiplayerInvite inv;

        inv.attributes = invite.playerAttributes;
        inv.group = invite.playerGroup;
        inv.player = [self fromGKPlayer:invite.sender];

        Multiplayer::instance()->inviteReceivedSignal(&inv);
        
        auto mmvc = [[GKMatchmakerViewController alloc] initWithInvite:invite];
        mmvc.matchmakerDelegate = self;
        
#if APEAL_OS_IPHONE
        [ManglMainViewController.instance presentViewController:mmvc animated:YES completion:nil];
#else
        [ManglMainViewController.instance presentViewControllerAsModalWindow:mmvc];
#endif
    });
}

-(void)player:(GKPlayer *)player didRequestMatchWithRecipients:(NSArray<GKPlayer*>*)recipientPlayers {
    MDebugFunc();
    dispatch_async(dispatch_get_main_queue(), ^{
        [ManglMainViewController showAlert:@"Invite Sent"];
    });
}


@end


namespace mangl::net {

//
// MARK: MNetworkOnline
//

//
// Matchmaking
//
void GameKitMatchmaker::doStartMatchmakingGui(mmp::MpGameParams params) {
    [ManglGameKitMatchmaker.instance startMatchmakingGui:this params:params];
}

void GameKitMatchmaker::doStopMatchmakingGui() {
    [ManglGameKitMatchmaker.instance stopMatchmaking];
}



//
// Client
//
void GameKitClient::doSend(NetworkDataSendParamsA params) {
    [ManglGameKitMatchmaker.instance sendData:params];
}

void GameKitClient::doConnect() {
    [ManglGameKitMatchmaker.instance onConnect:this];
}

void GameKitClient::doDisconnect() {
//    [ManglGameKitMatchmaker.instance onConnect:this];
}

void GameKitClient::doShutdown() {
    [ManglGameKitMatchmaker.instance shutdown];
}



//
// Server side
//
void GameKitServer::doSend(NetworkDataSendParamsA params) {
    [ManglGameKitMatchmaker.instance sendData:params];
}

void GameKitServer::doStartAdvertising(){
    [ManglGameKitMatchmaker.instance startServer:this];
}

void GameKitServer::doDisconnect(PeerKey pk) {
    [ManglGameKitMatchmaker.instance serverDisconnect:pk];
}

void GameKitServer::doShutdown(){
    [ManglGameKitMatchmaker.instance shutdown];
}


} // namespace mangl
#endif
