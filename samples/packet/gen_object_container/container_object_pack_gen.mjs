#! /usr/bin/env zx

import {config} from '../../../.build-config.mjs';

const {generate} = await import(config.mangl_scripts_dir + '/genpacket/index.mjs');

generate({
    desc: 'container_object_desc.yaml',
    output: 'generated/container_object_pack_gen_packet.hpp',
    config: config,
    format: 'packet',
});

generate({
    desc: 'container_object_desc.yaml',
    output: 'generated/container_object_pack_gen_yaml.hpp',
    config: config,
    format: 'yaml',
});

generate({
    desc: 'container_object_desc.yaml',
    output: 'generated/container_object_pack_gen_json.hpp',
    config: config,
    format: 'json',
});
