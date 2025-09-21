#! /usr/bin/env zx

import {genSample} from '../../common/gensample.mjs';

genSample({
    name: 'sdl',
//    compile_definitions: ["MANGL_DEBUG_STATS_FPS=1"],
    set: ["CONFIG_MANGL_SDL YES"],
});
