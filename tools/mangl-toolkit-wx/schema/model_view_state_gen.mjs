#! /usr/bin/env zx

const {config} = await import('../../../.build-config.mjs');

import {generate} from '../../../scripts/genfunc/index.mjs';

generate({
  yamlFile : './model_view_state.yaml',
  outputFile : '../src/generated/model_view_state.hpp',
  config: config,
  fileType: 'yaml',
});
