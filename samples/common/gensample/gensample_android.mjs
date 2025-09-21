// @ts-check

import fs from 'fs';
import { project_ } from '../../../scripts/gengradle/gengradle_generate.mjs';


/**
 * 
 * @param {import ("./gensample.d.ts").GenSampleContext } context
 */
export async function genSampleAndroid(context) {
    console.log(`Generating android projects...`);

    for (let target of context.project.targets) {
        await genSampleAndroidTarget(context, target);
    }


}


/**
 * 
 * @param {import ("./gensample.d.ts").GenSampleContext } context
 * @param {import ("./gensample.d.ts").SampleProjectTarget } target
 */
async function genSampleAndroidTarget(context, target) {
    let targetDir = `_build_android_${target.name}`;

    fs.mkdirSync(targetDir, {recursive: true});

    let project = context.project;

    let contents = '';

    contents = `
project:
  name: ${target.name}
  displayName: ${target.name}
  appId: org.manglsample.${target.name}
  appTag: ${target.name}
  namespace: org.manglsample.${target.name}
  versionCode: 1
  versionName: 0.1

  kotlin: true

  buildType: debug

  multiplayer: ${project.options?.multiplayer ? 1 : 0}
  bluetooth: ${project.options?.bluetooth ? 1 : 0}

  abiFilters: [arm64-v8a]
  # abiFilters: [x86_64]

  appIconDir: ${context.manglSamplesCommonDir}/android/icon

`;

    if (project.ads) contents += `  ads: true\n`;
    if (project.admob) contents += `  admob: true\n`;
    if (project.admobAppId) contents += `  admobAppId: ${project.admobAppId}\n`;
    if (project.admobBannerId) contents += `  admobBannerId: ${project.admobBannerId}\n`;
    if (project.admobInterstitialId) contents += `  admobInterstitialId: ${project.admobInterstitialId}\n`;


    //contents = await context.yaml.stringify(gengradleProject);

    fs.writeFileSync(`${targetDir}/${target.name}.yaml`, contents);


    // let targetSources = [];
    // if (target.sources) {
    //     targetSources
    // }

    contents = `#! /usr/bin/env zx

import {config} from "${context.manglRootDir}/.build-config.mjs";

const {generate} = await import(config.mangl_scripts_dir + "/gengradle/index.mjs");

generate({
    config: config,
    project: "${target.name}.yaml",
    cmake: {
        config: "${context.manglRootDir}/.build-config.cmake",
        include_directories: [
            "${context.manglSamplesCommonDir}",
        ],
        sources: [
            "${context.manglSamplesCommonDir}/common_sources.cmake",
            ${target.sources.map((v) => `"${context.projectDir}/${v}"`).join(',')},
        ],
        compile_definitions: [
          "MANGL_DEBUG_STATS_FPS=1",
        ],
    }
});

`   
    ;

    // cmakelists: [
    //     "add_executable(${target.name} ${context.projectDir}/${target.sources.join(' ')} \${main_SRC})"
    // ]

    //"include(${context.manglSamplesCommonDir}/tests.cmake)",
    // "addTest(${target.name} ${context.projectDir}/${target.sources.join(' ')})",
    
    //sources: "../sources.cmake",


    let gradleScripFile = `${targetDir}/gengradle.mjs`;
    fs.writeFileSync(gradleScripFile, contents);
    fs.chmodSync(gradleScripFile, 0o755);


    //
    // Google services file
    //
    //fs.copyFileSync(`${context.manglSamplesCommonDir}/android/google-services.json`, `${targetDir}/google-services.json`);

}

