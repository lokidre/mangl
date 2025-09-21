#! /usr/bin/env zx

import {genSample} from '../../common/gensample.mjs';

let project = {
    name: 'pack_pod_object',
    compile_definitions: ["MANGL_NO_MAIN=1"],
    set: ["CONFIG_TARGET_CONSOLE ON"]
};

genSample(project);

