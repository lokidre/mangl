/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

namespace mangl::mmp {

enum class MessageType: char {
    Invalid = 0,
    Action = 'A',
    Command = 'C',
    Event = 'E',
};

enum class MessageCategory: char {
    Invalid = 0,
    System          = 's',
    Match           = 'm',
    Game            = 'g',
    Player          = 'p',
    Board           = 'b',
    Piece           = 'f',
};


enum class MessageErrorCat: char {
    NoError         = 0,
    Generic         = 'g',
    Critical        = 'c',
    VersionObsolete = 'o',
    VersionMismatch = 'v',
};



enum class MessageOrder: char {
    Invalid = 0,

    //
    // System orders
    //
    Hello   = 'H',  // First message the client needs to send to the host
    Welcome = 'W',  // The first message sent to connected client
    Alert   = 'A',  // The alert to be shown to the user
    Message = 'M',  // The status message

    Ping    = '>',  // Connection ping
    Pong    = '<',  // Connection pong

    Error   = 'E',

    Disconnected    = 'D',
    Remaining       = '.',


    //
    // Match Orders    
    //
    MatchCreate  = 'C',
    MatchCreated = 'C',
    MatchFind    = 'F',
    MatchFound   = 'F',
    MatchAdd     = 'A',  // Add player
    MatchAdded   = 'A',  // Add player
    MatchKick    = 'X',
    MatchKicked  = 'X',
    MatchQuit    = 'Q',
    MatchUpdate  = 'U',
    MatchJoin    = 'J',
    MatchJoined  = 'J',

    MatchStatus  = 'T',

    MatchLaunch    = 'L',
    MatchReady     = 'R',
    MatchCountdown = 'D',
    MatchBegin     = 'B',
    MatchPause     = 'P',
    MatchPaused    = 'P',
    MatchResume    = 'Z',
    MatchResumed   = 'Z',
    MatchEnd       = 'E',



    //
    // Game orders
    //
    GameState = 'S',  // Total game state
    GameField = 'F',  // Game field information
    GameElement = 'E',  // Information about game element or elements
    GameCharacter = 'C',  // Information about game character or characters
    GameMove = 'M',
    GameResign = 'R',

    // 
    // 
    //GameReady = 'R',
    //GameCountdown = '#',
    //GameStart = 'G',

    //GamePause =     'P',
    //GameResume =    'Z',

    //QuitGame =      'Q',

    //Disconnected    = 'D',
    //Remaining       = '.',

    ////
    //// Board orders
    ////
    //BoardUpdate = 'U',

    ////
    //// Piece orders
    ////
    //PieceUpdate = 'U',


};


}  // namespace mangl::mmp

