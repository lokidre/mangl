#! /usr/bin/env zx

import path from 'path/posix';

import {config} from '../../../.build-config.mjs';

const {generate} = await import(config.apeal_scripts_dir + '/genproj/index.mjs');


generate({
    projectConfig: 'ios_min.yaml',
    sources: [
        './sources.yaml', 
        `${config.mangl_scripts_dir}/mangl_sources.yaml`,
        `${config.cognit_scripts_dir}/cognit_sources.yaml`,
        `${config.apeal_scripts_dir}/apeal_sources.yaml`,
    ],
    buildConfig: '../../../.build-config.yaml',
    targetDir: "_build",
    targetXcodeGen: "project.yml",
})

// Run xcodegen from _build directory
