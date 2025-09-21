#! /usr/bin/env zx

import {config} from '../../../.build-config.mjs';

const {generate} = await import(`${config.mangl_scripts_dir}/genpacket/index.mjs`);

let params = {
    config: config,
    format: 'yaml',
    generatePackFile: true,
    // generateClear: true,
}

generate({ ...params, ... {
    desc: 'model_view_state.yaml',
    output: '../src/generated/model_view_state_pack.hpp',
}});

generate({ ...params, ... {
    desc: 'project_files_state.yaml',
    output: '../src/generated/project_files_state_pack.hpp',
}});

generate({ ...params, ... {
    desc: 'screen_layout_state.yaml',
    output: '../src/generated/screen_layout_state_pack.hpp',
}});

generate({ ...params, ... {
    desc: 'window_layout_state.yaml',
    output: '../src/generated/window_layout_state_pack.hpp',
}});

