#! /usr/bin/env zx

import {genSample} from '../../common/gensample.mjs';

genSample({
    name: 'multiplayer_server',
    multiplayer: true,
    console: true,
    cmake: "../shared/gencmake_shared_sources.cmake"
});
