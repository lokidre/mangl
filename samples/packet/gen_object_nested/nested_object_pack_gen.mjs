#! /usr/bin/env zx

import {config} from '../../../.build-config.mjs';

const {generate} = await import(config.mangl_scripts_dir + '/genpacket/index.mjs');

generate({
    desc: 'nested_object_desc.yaml',
    output: 'generated/nested_object_pack_gen.hpp',
    config: config,
});

generate({
    desc: 'nested_object_desc.yaml',
    output: 'generated/nested_object_pack_gen_yaml.hpp',
    config: config,
    format: 'yaml',
});

generate({
    desc: 'nested_object_desc.yaml',
    output: 'generated/nested_object_pack_gen_json.hpp',
    config: config,
    format: 'json',
});


