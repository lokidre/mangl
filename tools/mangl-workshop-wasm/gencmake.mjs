#! /usr/bin/env zx


const contents = fs.readFileSync('../../.build-config.json');

const {config} = JSON.parse(contents);

const {generate} = await import(path.join(config.apeal_scripts_dir, 'genproj', 'index.mjs'));


let params = {
  buildConfig : '../../.build-config.yaml',
  sources : 'sources.yaml',
  targetCmakeSources : 'sources.cmake',
  targetCmakeConfig : './.build-config.cmake',
}

generate(params);

