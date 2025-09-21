#! /usr/bin/env zx

const {config} = await import('../../.build-config.mjs');

const {generate} = await import(config.apeal_scripts_dir + "/genproj/index.mjs");


let params = {
    buildConfig: '../../.build-config.yaml',
    sources: 'sources.yaml',
    targetCmakeSources: 'sources.cmake',
}

generate(params);

