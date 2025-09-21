#! /usr/bin/env zx

import path from 'path/posix';

const {config} = await import('../../.build-config.mjs');

const {generate} = await import(path.join(config.apeal_scripts_dir, 'genproj', 'index.mjs'));


let params = {
    buildConfig: '../../.build-config.yaml',
    sources: 'sources.yaml',
    targetCmakeSources: 'sources.cmake',
}

generate(params);

