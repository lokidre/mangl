#! /usr/bin/env zx

//import {generate} from '../../../../apeal/scripts/genproj/index.mjs'


import path from 'path/posix'

import {config} from '../../../.build-config.mjs';

const {generate} = await import(path.join(config.apeal_scripts_dir, 'genproj', 'index.mjs'));



let params = {
  buildConfig : '../../../.build-config.yaml',
  sources :'sources.yaml',
  targetCmakeSources : './sources.cmake',
  //targetCmakeConfig : './.build_project_config.cmake',
}

generate(params);

