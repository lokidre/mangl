#! /usr/bin/env zx

import {config} from '../../../.build-config.mjs';

const {generate} = await import(config.mangl_scripts_dir + '/genpacket/index.mjs');


generate({
    desc: 'flat_object_desc.yaml',
    output: 'generated/flat_object_pack_gen.hpp',
    config: config,
});

generate({
    desc: 'flat_object_desc.yaml',
    output: 'generated/flat_object_pack_gen_yaml.hpp',
    config: config,
    format: 'yaml',
});

generate({
    desc: 'flat_object_desc.yaml',
    output: 'generated/flat_object_pack_gen_json.hpp',
    config: config,
    format: 'json',
});

