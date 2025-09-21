#! /usr/bin/env zx

import fs from "fs";

import {generateApealSourcePkgConfig} from './scripts/js/apeal_pkg_config.mjs';


const apealTryDir = '.';
const apealDir = path.resolve(apealTryDir);

const pkgConfigContents = generateApealSourcePkgConfig(apealDir);

//const pkgConfigPath = "/opt/lib/pkgconfig";

const pkgConfigName = "apeal.pc"

fs.writeFileSync(pkgConfigPath + "/" + pkgConfigName, pkgConfigContents);

