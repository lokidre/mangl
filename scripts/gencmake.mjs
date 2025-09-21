#! /usr/bin/env zx

const {config} = await import('../.build-config.mjs');
const {generate} = await import(`${config.apeal_scripts_dir}/genproj/index.mjs`);

generate({
    buildConfig: '../.build-config.yaml',
    sources: 'mangl_sources.yaml',
    targetCmakeSources: './cmake/mangl_sources.cmake',
});

generate({
    buildConfig: '../.build-config.yaml',
    sources: 'mangl_sources_console.yaml',
    targetCmakeSources: './cmake/mangl_sources_console.cmake',
});
