#! /usr/bin/env zx

import {genSample} from '../../common/gensample.mjs';

let project = {
    name: 'gen_flat_object',
    options: {
        console: true,
    },
    compile_definitions: ["MANGL_NO_MAIN=1"],
};

genSample(project);

