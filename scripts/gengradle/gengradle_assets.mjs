// @ts-check

import fs from 'fs';
import path from 'path/posix';


import {params_, project_, vars_} from './gengradle_generate.mjs';
import { readFilesRecursive } from './lib_fs.mjs';
import { processValueBody } from './gengradle_vars.mjs';


export function processAppAssets() {
    let assetsDir = `${params_.output}/app/src/main/assets`;


    fs.rmSync(assetsDir, { recursive: true, force: true });
    fs.mkdirSync(assetsDir, {recursive: true});

    if (project_.assets) {
        for (let asset of project_.assets) {
            let assetDir = processValueBody(asset, vars_);
            

            let srcFiles = readFilesRecursive(assetDir);

            for (let srcFile of srcFiles) {
                let srcName = path.basename(srcFile);
                let dstFile = path.join(assetsDir, srcName);

                fs.symlinkSync(srcFile, dstFile);
            }

            // console.log(srcFiles);
        }
    }

}
