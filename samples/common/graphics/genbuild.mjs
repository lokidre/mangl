#! /usr/bin/env zx

const {config} = await import(`../../../.build-config.mjs`);
const {generate} = await import(`${config.mangl_scripts_dir}/gengraphics/index.mjs`);

await generate({
    config: config,
    desc: 'samples-graphics.yaml',
    include: '../../../.build-config.make',
    target: 'Makefile',
});

await generate({
    config: config,
    desc: 'samples-graphics.yaml',
    include: '../../../.build-config.ninja',
    target: 'build.ninja',
});
