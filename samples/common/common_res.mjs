#! /usr/bin/env zx

import {config} from '../../.build-config.mjs';

const {generate} = await import(config.mangl_scripts_dir + '/genenum/index.mjs');


generate({
    apealJsDir: config.apeal_scripts_dir + "/js",
    yamlFile: "common_res.yaml",
    outputFile: "generated/common_res.hpp",
});
