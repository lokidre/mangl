#! /usr/bin/env zx

const {config} = await import('../../../.build-config.mjs');

import {generate} from '../../../scripts/genfunc/index.mjs';

generate({
  yamlFile : './project_files_state.yaml',
  outputFile : '../src/generated/project_files_state.hpp',
  config: config,
  fileType: 'yaml',
});
