#import "mangl_nearby_mc_base.h"

#include "../core/mangl_network.h"
#include "../core/mangl_util.h"
#include "mangl_imacos_util.h"


#if defined(MANGL_MULTIPLAYER) && !defined(MANGL_IXWEBSOCKET)

// MARK: MNearbyBaseMC
@interface ManglNearbyBaseMC()
@end


using namespace mangl;

@implementation ManglNearbyBaseMC

- (instancetype)init {
    self = [super init];

    _peersByKey = NSMutableDictionary.dictionary;
    _keysByPeer = NSMutableDictionary.dictionary;

    return self;
}

-(MCPeerID*)getLocalPeerID:(NSString*)displayName {
    if (!_localPeerID)
        _localPeerID = [self createLocalPeerID:displayName];
    return _localPeerID;
}


- (MCPeerID*)createLocalPeerID:(NSString*)displayName {
    MCPeerID* peerId = [[MCPeerID alloc] initWithDisplayName:displayName];
    
//    static auto peerNameKey = @"mangl_local_peer_name_key";
//    static auto peerDataKey = @"mangl_local_peer_data_key";
//    
//    auto defaults = NSUserDefaults.standardUserDefaults;
//    auto storedDisplayName = [defaults stringForKey:peerNameKey];
//
//    MCPeerID *peerID = nil;
//    
//    // TODO: test storing with
//    // archivedDataWithRootObject:requiringSecureCoding:
//
//    if ([storedDisplayName isEqualToString:displayName]) {
//        NSData *peerIDData = [defaults dataForKey:peerDataKey];
//        peerID = [NSKeyedUnarchiver unarchiveObjectWithData:peerIDData];
//    }
//
//    if (!peerID) {
//        peerID = [[MCPeerID alloc] initWithDisplayName:displayName];
//
//        NSData *peerIdData = [NSKeyedArchiver archivedDataWithRootObject:_localPeerID];
//        [defaults setObject:peerIdData forKey:peerDataKey];
//        [defaults setObject:displayName forKey:peerNameKey];
//        [defaults synchronize];
//    }

    return peerId;
}


- (PeerKey)getPeerKey:(MCPeerID *)peerID {
    NSNumber* obj = [_keysByPeer objectForKey:peerID];

    if (!obj)
        return 0;

    return obj.intValue;
}

- (MCPeerID *)findPeerByKey:(PeerKey)key {
    
    id obj = [_peersByKey objectForKey:@(key)];

    if (!obj) return nullptr;

    auto peer = objc_cast<MCPeerID>(obj);

    MDebugTrapAssert(peer);

    return peer;
}



- (PeerKey)addPeer:(MCPeerID *)peerID {
    auto pk = M::generateId<PeerKey>();

    _peersByKey[@(pk)] = peerID;
    _keysByPeer[peerID] = @(pk);

    return pk;
}

- (void)removePeer:(MCPeerID *)peerID {
    if (auto pk = [self getPeerKey:peerID]; pk) {
        [_peersByKey removeObjectForKey:@(pk)];
    }

    [_keysByPeer removeObjectForKey:peerID];
}



- (mmp::MpPeer)onNewPeerDiscovered:(MCPeerID *)peerID {
    auto key = [self addPeer:peerID];

    mmp::MpPeer peerInfo;
    peerInfo.key = key;
    peerInfo.playerId = toString(peerID.displayName);

//    peerInfo.player.id = peerInfo.player.alias = peerInfo.player.name = toString(peerID.displayName);

    return peerInfo;
}


-(net::DiscoveryInfo)discoveryInfoFromDict:(nullable NSDictionary<NSString*, NSString*>*)diDict {
    net::DiscoveryInfo di;

    if (!diDict)
        return di;

    String sdict;

    for (NSString *key in diDict) {
        NSString *value = diDict[key];
        sdict += apl::fmt("%s:%s;", key, value);
    }

    di.parse(sdict);

    return di;
}

- (NSMutableDictionary*)discoveryInfoToDict:(net::DiscoveryInfoA)di {
    auto sdi = di.build();
    auto list = apl::parseStdVectorTrim<StrView>(sdi, ';');
    auto diDict = [[NSMutableDictionary alloc] init];

    for (auto& item: list) {
        auto [key, val] = apl::str_split_trim(item, ':');
        if (key.empty())
            continue;
        auto nsKey = toNSString(key);
        if (!nsKey)
            continue;
        if (val.empty())
            continue;
        
        auto nsVal = toNSString(val);
        if (!nsVal)
            continue;
        
        [diDict setValue:nsVal forKey:nsKey];
        //if (!key.empty())
        //    diDict[toNSString(key)] = toNSString(value);
    }
    return diDict;
}

- (NSString *)formatBonjourPlayerName:(StrView)playerName {
    auto name = toNSString(playerName);

    if (name.length > 63) {
        name = [name substringToIndex:63];
    }

    return name;
}


- (NSString *)formatBonjourServiceName:(StrView)protoName {
    String serviceName;

    for (auto c: protoName) {
        if (serviceName.size() >= 15)
            break;
        if ('_' == c)
            serviceName += '-';
        else
            serviceName += c;
    }

    MDebugTrapAssert(!serviceName.empty());

    return toNSString(serviceName);
}


@end


#endif
