#! /usr/bin/env zx

import {genSample} from '../../common/gensample.mjs';

genSample({
    name: 'match_host',
    multiplayer: true,
    cmake: "../shared/gencmake_shared_sources.cmake"
});

