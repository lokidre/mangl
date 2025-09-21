#! /usr/bin/env zx

import {config} from '../../../.build-config.mjs';

const {generate} = await import(config.mangl_scripts_dir + '/genpacket/index.mjs');

generate({
    desc: 'pod_object_desc.yaml',
    output: 'generated/pod_object_pack_gen_packet.hpp',
    config: config,
    format: 'packet',
});

generate({
    desc: 'pod_object_desc.yaml',
    output: 'generated/pod_object_pack_gen_yaml.hpp',
    config: config,
    format: 'yaml',
});

generate({
    desc: 'pod_object_desc.yaml',
    output: 'generated/pod_object_pack_gen_json.hpp',
    config: config,
    format: 'json',
});

