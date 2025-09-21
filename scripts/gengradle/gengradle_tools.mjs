//@ts-check

import fs from 'fs';

import { createAndWriteFile } from "./lib_fs.mjs";
import {params_, project_, vars_} from './gengradle_generate.mjs';
import { content_, outclear, outn } from './lib_io.mjs';


export function createProjectTools() {

    if (project_.crashlytics)
        createCrashlyticsUpload();

    createBuildRelease();
}

function createBuildRelease() {
    outclear();

    if (process.platform === 'win32') {
        outn(`gradlew clean bundleRelease`);
        outn(`gradlew clean bundleRelease`);
        
        let bin = `${params_.output}/build-release.bat`;
        createAndWriteFile(bin, content_);
    } else {
        outn('#! /bin/bash');
        outn(`./gradlew bundleRelease`);
        // outn(`./gradlew clean bundleRelease`);

        let bin = `${params_.output}/build-release.sh`;
        createAndWriteFile(bin, content_);
        fs.chmodSync(bin, 0o755);
    }

}


function createCrashlyticsUpload() {
    // Create crashlytics upload scripts
    outclear();
    let buildDir = 'app/build';

    if (process.platform === 'win32') {
        outn(`firebase crashlytics:symbols:upload --app=${project_.crashlyticsAppId} ${buildDir}`);
        let bin = `${params_.output}/crashlytics-upload.bat`;
        createAndWriteFile(bin, content_);
    } else {
        outn('#! /bin/bash');
        outn(`firebase crashlytics:symbols:upload --app=${project_.crashlyticsAppId} ${buildDir}`);
        let bin = `${params_.output}/crashlytics-upload.sh`;
        createAndWriteFile(bin, content_);
        fs.chmodSync(bin, 0o755);
    }
}
