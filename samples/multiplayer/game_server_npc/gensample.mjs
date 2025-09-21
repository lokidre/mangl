#! /usr/bin/env zx

import {genSample} from '../../common/gensample.mjs';

genSample({
    name: 'game_server_npc',
    multiplayer: true,
    console: true,
    cmake: "../shared/gencmake_shared_sources.cmake"
});
