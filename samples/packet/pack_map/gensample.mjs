#! /usr/bin/env zx

import {genSample} from '../../common/gensample.mjs';

let project = {
    name: 'pack_map',
    options: {
        console: true,
    },
    compile_definitions: ["MANGL_NO_MAIN=1"],
};

genSample(project);

