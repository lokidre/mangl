#! /usr/bin/env zx

import {generate} from '../../../scripts/genproj/index.mjs'

let params = {
  buildConfig : '../../../build_config.yaml',
  sources : '../../common/common_sources.yaml',
  targetCmakeSources : './sources.cmake',
  targetCmakeConfig : './.build_project_config.cmake',
}

generate(params);

