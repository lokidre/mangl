#! /usr/bin/env zx

let buildDir = "_build_wasm_makefile";

fs.mkdirSync(buildDir, {recursive: true});

cd(buildDir);

await $`emcmake cmake .. -G "Unix Makefiles"`


