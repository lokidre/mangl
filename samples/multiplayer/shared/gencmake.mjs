#! /usr/bin/env zx

import {config} from '../../../.build-config.mjs';

const {generate} = await import(config.apeal_scripts_dir + '/genproj/index.mjs');

generate({
    buildConfig : '../../../.build-config.yaml',
    sources : './gencmake_shared_sources.yaml',
    targetCmakeSources : './gencmake_shared_sources.cmake',
});
