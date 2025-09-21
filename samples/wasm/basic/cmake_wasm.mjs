#! /usr/bin/env zx

let buildDir = "_build_wasm";

fs.mkdirSync(buildDir, {recursive: true});

cd(buildDir);

await $`emcmake cmake .. -G Ninja`

// await $`emcmake cmake .. -G "Unix Makefiles"`

