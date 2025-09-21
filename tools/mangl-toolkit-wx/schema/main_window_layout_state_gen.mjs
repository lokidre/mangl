#! /usr/bin/env zx

const {config} = await import('../../../.build-config.mjs');

import {generate} from '../../../scripts/genfunc/index.mjs';

generate({
  yamlFile : './main_window_layout_state.yaml',
  outputFile : '../src/generated/main_window_layout_state.hpp',
  config: config,
  fileType: 'yaml',
});
