#! /usr/bin/env zx

import {config} from '../../../.build-config.mjs';

const {generate} = await import(config.mangl_scripts_dir + '/genpacket/index.mjs');

generate({
    desc: 'sample_game_packet_desc.yaml',
    output: '../shared/sample_game_packet_gen.hpp',
    config: config,
    generateClear: true,
});

generate({
    desc: 'sample_game_packet_desc.yaml',
    output: '../shared/sample_game_packet_gen_yaml.hpp',
    config: config,
    format: 'yaml',
});

generate({
    desc: 'sample_game_packet_desc.yaml',
    output: '../shared/sample_game_packet_gen_json.hpp',
    config: config,
    format: 'json',
});
