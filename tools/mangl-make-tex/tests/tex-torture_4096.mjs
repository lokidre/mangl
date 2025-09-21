#! /usr/bin/env zx

// ts-check

import {config} from '../../../.build-config.mjs'


let resDir = `${config.mangl_samples_resources_dir}/maketex`;
let texturesDir = `${resDir}/textures`
let renderDir = `${resDir}/render`
let renderFontsDir = `${resDir}/render`
let fontsDir = "${config.mangl_samples_resources_dir}/fonts"
let srcGenDir = "./generated"

let maketex = '../cmake-build-release/mangl-make-tex'


async function runTest(params) {
    let beginTime = new Date();
    
//    await echo`${params.join(' ')}`
    await $`${maketex} ${params}`
    
    let endTime = new Date();

    let diff = endTime - beginTime;

    let ms = diff % 1000;
    let ss = Math.floor(diff / 1000) % 60;
    let mm = Math.floor(diff / 1000 / 60) % 60;
    let hh = Math.floor(diff / 1000 / 60 / 60);
    
    let diffTime = `${hh}:${mm}:${ss}:${ms}ms`;

    console.log(`${params[1]}: ${diffTime}`);
}

//
// Methods: dfs, brute, rectpack
//

await runTest([
    '--input=tex_sample_desc_4096.yaml',
    '--method=dfs',
    '--bleeding=1',
    '--bleeding-debug=1',
    `--define=ROOT=${resDir}`,
    '-o',
    `${texturesDir}/tex_sample_dfs_4096.yaml`,
    `${texturesDir}/tex_sample_dfs_4096.png`,
])



await runTest([
    '--input=tex_sample_desc_4096.yaml',
    '--method=brute',
    '--bleeding=1',
    '--bleeding-debug=1',
    `--define=ROOT=${resDir}`,
    '-o',
    `${texturesDir}/tex_sample_brute_4096.yaml`,
    `${texturesDir}/tex_sample_brute_4096.png`,
])


await runTest([
    '--input=tex_sample_desc_4096.yaml',
    '--method=rectpack',
    '--bleeding=1',
    '--bleeding-debug=1',
    `--define=ROOT=${resDir}`,
    '-o',
    `${texturesDir}/tex_sample_rectpack_4096.yaml`,
    `${texturesDir}/tex_sample_rectpack_4096.png`,
])
