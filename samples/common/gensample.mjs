// @ts-check

import fs from 'fs';
import path from 'path/posix';
import os from 'os';


import {genSampleCmake} from './gensample/gensample_cmake.mjs';
import {genSampleAndroid} from './gensample/gensample_android.mjs';
import { genSampleXcode } from './gensample/gensample_xcodegen.mjs';


/**
 * 
 * @param {import ("./gensample/gensample.d.ts").SampleProject } project 
 */

export async function genSample(project) {
    //console.log(`Loading config...`);
    let context = await loadManglConfig(project);

    if (!context)
        return;

    processProject(context);

    await genSampleCmake(context);

    await genSampleAndroid(context);

    if (os.type() === 'Darwin') {
        await genSampleXcode(context);
    }

}

/**
 * 
 * @param {import ("./gensample/gensample.d.ts").GenSampleContext } context
 */
function processProject(context) {
    let project = context.project;

    if (project.options === undefined)
        project.options = {};

    if (project.console) project.options.console = true;
    if (project.multiplayer) project.options.multiplayer = true;
    if (project.bluetooth) project.options.bluetooth = true;


    if (project.targets === undefined) {
        project.targets = [{
            name: `mangl_sample_${project.name}`, 
            sources: [`sample_${project.name}.cpp`]
        }];
    }

    for (let target of project.targets) {
        if (target.sources === undefined) {
            if (target.source !== undefined)
                target.sources = [target.source];
        }
    }
}



/**
 * 
 * @param {import ("./gensample/gensample.d.ts").SampleProject } project 
 * @returns {Promise<import ("./gensample/gensample.d.ts").GenSampleContext | undefined>}
 */
async function loadManglConfig(project) {
    let upLevelCount = 0;
    let projectDir = path.resolve(__dirname);
    let currentLevel = path.resolve(__dirname);

    let manglConfigFile = '';

    for (;;) {
        //console.log(currentLevel);
        manglConfigFile = path.join(currentLevel, '.build-config.json');

        if (fs.existsSync(manglConfigFile))
            break;

        if (currentLevel === '' || currentLevel === '/' || currentLevel === '.') {
            console.error('.build-config.json is missing. Run configure.mjs')
            return undefined;
        }


        currentLevel = path.dirname(currentLevel);
        upLevelCount++;
    }

    let manglRootDir = path.resolve(currentLevel);

    /** type {import ("./gensample/gensample.d.ts").SampleProject} */
    let context = {
        project: project,
        config: require(manglConfigFile).config,
        projectDir: projectDir,
        manglRootDir: manglRootDir,
        manglSamplesDir: `${manglRootDir}/samples`,
        manglSamplesCommonDir: `${manglRootDir}/samples/common`,
    };

    context.yaml = await import(context.config.apeal_scripts_dir + "/js/yaml.mjs");
    context.genproj = await import(context.config.apeal_scripts_dir + "/genproj/index.mjs");

    // @ts-ignore
    return context;
}


