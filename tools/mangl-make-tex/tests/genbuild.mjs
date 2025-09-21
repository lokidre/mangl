#! /usr/bin/env zx

import {config} from '../../../.build-config.mjs';

const {generate} = await import(config.mangl_scripts_dir + '/gengraphics/index.mjs');

await generate({
    config: config,
    desc: 'make-tex-samples.yaml',
    include: '../../../.build-config.make',
    target: 'Makefile',
    nohpp: true,
    texmake: "../cmake-build-debug/mangl-make-tex",
    texparams: [
      "--bleeding=1",
      "--bleeding-debug=1",
      "--method=dfs",
   ]
});

/*
await generate({
    config: config,
    desc: 'make-tex-samples.yaml',
    include: '../../../.build-config.ninja',
    target: 'build.ninja',
    nohpp: true,
});
*/
