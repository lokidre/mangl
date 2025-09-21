#! /usr/bin/env zx

import {generate} from '../../scripts/genproj/index.mjs';

let params = {
  buildConfig : '../../.build-config.yaml',
  sources : 'sources.yaml',
  targetCmakeSources : 'sources.cmake',
  targetCmakeConfig : '.build-config.cmake',
}

generate(params);

