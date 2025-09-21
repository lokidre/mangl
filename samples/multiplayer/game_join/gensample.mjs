#! /usr/bin/env zx

import {genSample} from '../../common/gensample.mjs';

genSample({
    name: 'game_join',
    multiplayer: true,
    cmake: "../shared/gencmake_shared_sources.cmake"
});
