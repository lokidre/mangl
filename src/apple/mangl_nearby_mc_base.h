/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_types.h"

#if MANGL_MULTIPLAYER

//defined(MANGL_MULTIPLAYER) && !defined(MANGL_IXWEBSOCKET)

#include "../core/mangl_multiplayer.h"
#include "../core/mangl_network.h"

@import MultipeerConnectivity;

@interface ManglNearbyBaseMC: NSObject
@property (nonatomic) MCPeerID* _Nullable localPeerID;
@property (nonatomic) NSMutableDictionary* _Nullable peersByKey;
@property (nonatomic) NSMutableDictionary* _Nullable keysByPeer;


- (NSString *_Nonnull)formatBonjourPlayerName:(mangl::StrView)playerName;
- (NSString *_Nonnull)formatBonjourServiceName:(mangl::StrView)protoName;


- (MCPeerID*_Nonnull)getLocalPeerID:(NSString*_Nonnull)displayName;

- (mangl::PeerKey)getPeerKey:(MCPeerID* _Nonnull)peerID;
- (MCPeerID *_Nullable)findPeerByKey:(mangl::PeerKey)key;
- (mangl::PeerKey)addPeer:(MCPeerID*_Nonnull)peerID;
- (void)removePeer:(MCPeerID *_Nonnull)peerID;

- (mangl::mmp::MpPeer)onNewPeerDiscovered:(MCPeerID *_Nonnull)peerID;



- (mangl::net::DiscoveryInfo)discoveryInfoFromDict:(nullable NSDictionary<NSString*, NSString*>*)diDict;
- (NSMutableDictionary *_Nonnull)discoveryInfoToDict:(mangl::net::DiscoveryInfoA)di;


@end

#endif
