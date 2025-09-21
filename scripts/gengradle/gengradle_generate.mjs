// @ts-check

//import 'zx/globals';

import fs from "fs";

import { copyStaticFiles } from "./lib_fs.mjs";
import { loadProjectConfigVars } from "./project_vars.mjs";
import { createGradleSetup } from "./gengradle_gradle.mjs";
import { createAndroidManifestxml } from "./gengradle_manifest.mjs";
import { createMainActivityJava, createMainApplicationJava, createManglConfigJava, linkManglSources } from "./gengradle_java.mjs";
import { createAndroidLayout, createAndroidNetworkSecurityConfig, createAndroidResValues, createLaunchAssets } from "./gengradle_res.mjs";
import { createCMakeLists, createManglConfigHeader } from "./gengradle_cmake.mjs";
import { processAppAssets } from "./gengradle_assets.mjs";


/** @type {import ("./gengradle.d.ts").GradleSetup} */
export var setup_;

/** @type {import ("./gengradle.d.ts").AndroidProject} */
export var project_;

/** @type {import ("./gengradle.d.ts").GenerateGradleParams} */
export var params_;

/** @type {Map} */
export var vars_ = new Map();


/**
 * @param {import ("./gengradle.d.ts").GenerateGradleParams} params
 */
export async function generateGradle(params) {
    params_ = params;

    let config = params_.config;

    // Loading all the configurations
    // @ts-ignore: __dirname
    let setupDoc = await loadYaml(config, `${config.mangl_scripts_dir}/gengradle/gengradle_setup.yaml`);

    setup_ = setupDoc.project;

    let projectDoc = await loadYaml(config, params.project);
    project_ = projectDoc.project;

    checkProject();

    params_.output = `_build_${project_.name}`;
    let outdir = params_.output;


    vars_ = loadProjectConfigVars(params_.config);


    createGradleSetup();
    

    createAndroidManifestxml();


    createCMakeLists();
    createManglConfigHeader();


    createManglConfigJava();
    createMainActivityJava();
    createMainApplicationJava();

    linkManglSources();


    createAndroidResValues();
    createAndroidLayout();
    createAndroidNetworkSecurityConfig();
    createLaunchAssets();
    processAppAssets();

    copyStaticFiles(`${config.mangl_scripts_dir}/gengradle/static`, outdir);

}



/**
 * 
 * @param {import ("./gengradle.d.ts").ProjectConfig} config
 * @param {string} yamlFile
 */
async function loadYaml(config, yamlFile) {

    const YAML = await import(`${config.apeal_scripts_dir}/js/yaml.mjs`);

    const doc = YAML.parse(fs.readFileSync(yamlFile, 'utf8'));
    return doc;
}


function checkProject() {
    verifyProjectField(project_.appId, "project.appId is missing!");
    verifyProjectField(project_.name, "project.name is missing!");
    verifyProjectField(project_.appTag, "project.appTag is missing!");

    if (!project_.buildType) project_.buildType = "debug";
    project_.debug = project_.buildType === "debug";
    project_.release = !project_.debug;

    if (project_.ads) project_.firebase = true;
    if (project_.crashlytics) project_.firebase = true;
}

function verifyProjectField(field, message) {
    if (!field) {
        console.error(message);
        // @ts-ignore: node built in classes
        process.exit(1);
    }
}


