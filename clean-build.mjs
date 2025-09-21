#! /usr/bin/env zx

// @ts-check

import {config} from "./.build-config.mjs";

const {cleanBuild} = await import(`${config.apeal_scripts_dir}/clean/index.mjs`);

await cleanBuild();

