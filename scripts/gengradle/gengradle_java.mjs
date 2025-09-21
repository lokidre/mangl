// @ts-check

import fs from "fs";

import { generateHeader } from "./generate_header.mjs";
import { params_, project_ } from "./gengradle_generate.mjs";
import { createAndWriteFile, linkDirContents } from "./lib_fs.mjs";
import { content_, outclear, outn } from "./lib_io.mjs";


function makeProjectNamespacePath() {
    let namespaceComps = project_.namespace.split(".");
    let javaNamespacePath = namespaceComps.join("/");

    return javaNamespacePath;
}

export function createManglConfigJava() {
    outclear(generateHeader("kotlin"));

    let javaDebug = project_.debug ? "true" : "false";

        outn(`
package config;

object ManglConfig: mangl.ManglProjectConfig() {
    const val DEBUG: Boolean = ${javaDebug}
    @JvmStatic
    fun onInit() {}
}
`);

    createAndWriteFile(`${params_.output}/app/src/main/java/config/ManglConfig.kt`, content_)
}


export function createMainActivityJava(){
    outclear(generateHeader("kotlin"));

        outn(`
package ${project_.namespace};

import android.os.Bundle;

class MainActivity: mangl.ManglMainActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }
}
`
        );

    let javaNamespacePath = makeProjectNamespacePath();

    createAndWriteFile(`${params_.output}/app/src/main/java/${javaNamespacePath}/MainActivity.kt`, content_)
}

export function createMainApplicationJava() {
    outclear(generateHeader("kotlin"));

        outn(`
package ${project_.namespace};

class MainApplication: mangl.ManglMainApplication() {
}

`
        );

    let javaNamespacePath = makeProjectNamespacePath();

    createAndWriteFile(`${params_.output}/app/src/main/java/${javaNamespacePath}/MainApplication.kt`, content_);
}


export function linkManglSources() {
    let destJavaDir = `${params_.output}/app/src/main/java/mangl`;
    let srcJavaDir = `${params_.config.mangl_src_dir}/android/java`;

    if (fs.existsSync(destJavaDir)) {
        // Clean the directory
        let entries = fs.readdirSync(destJavaDir);

        for (let file of entries) {
            fs.unlinkSync(destJavaDir + "/" + file);
        }
    
    } else {
        fs.mkdirSync(destJavaDir, {recursive: true});
    }

    linkDirContents(`${srcJavaDir}/mangl_core`, destJavaDir);

    let adsSuffix = project_.ads ? "" : "_dummy";
    linkDirContents(`${srcJavaDir}/mangl_ads${adsSuffix}`, destJavaDir);

    let btSuffix = project_.bluetooth ? "" : "_dummy";
    linkDirContents(`${srcJavaDir}/mangl_ble${btSuffix}`, destJavaDir);

}
