#! /usr/bin/env zx

import {generate} from '../../../scripts/genproj/genproj_result.mjs'

let params = {
  buildConfig : '../../../build_config.yaml',
  sources : './project.yaml',
  targetCmakeSources : 'project.cmake',
  targetCmakeConfig : './.build_project_config.cmake',
}

generate(params);

