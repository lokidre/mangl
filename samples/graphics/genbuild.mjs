#! /usr/bin/env zx

import {config} from '/.build-config.mjs';

const {generate} = await import(config.mangl_scripts_dir + '/gengraphics/index.mjs');

await generate({
    config: config,
    desc: 'chessdash-graphics.yaml',
    include: '../.build-config.make',
    target: 'Makefile',
});

await generate({
    config: config,
    desc: 'chessdash-graphics.yaml',
    include: '../.build-config.ninja',
    target: 'build.ninja',
});
