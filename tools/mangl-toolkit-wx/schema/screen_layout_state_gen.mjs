#! /usr/bin/env zx

const {config} = await import('../../../.build-config.mjs');

import {generate} from '../../../scripts/genfunc/index.mjs';

generate({
  yamlFile : './screen_layout_state.yaml',
  outputFile : '../src/generated/screen_layout_state.hpp',
  config: config,
  fileType: 'yaml',
});
