// @ts-check

// @ts-ignore: node import error
import path from 'path/posix';

import {generateHeader} from './generate_header.mjs';
import {params_, project_, setup_} from './gengradle_generate.mjs';
import {content_, nl, outclear, outn, outnn} from './lib_io.mjs';
import {createAndWriteFile} from './lib_fs.mjs';

export function createCMakeLists() {
    outclear(generateHeader('cmake'));

    outnn(`cmake_minimum_required(VERSION ${setup_.cmakeVersion})`);
    outnn(`project(main)`);

    let buildConfigCmake = path.resolve(params_.cmake.config);
    outnn(`include(${buildConfigCmake})`);

    // nl();
    // outn(`set(CMAKE_ANDROID_ASSETS_DIRECTORIES \${CMAKE_SOURCE_DIR}/src/main/assets)`);

    if (project_.multiplayer) outn(`set(CONFIG_PROJECT_MULTIPLAYER ON)`);
    if (project_.bluetooth) outn(`set(CONFIG_PROJECT_BLUETOOTH ON)`);
    nl();

    outnn('include(${CONFIG_MANGL_CMAKE_DIR}/project_header.cmake)');

    let cmake = params_.cmake;

    if (cmake.compile_definitions) {
        if (Array.isArray(cmake.compile_definitions)) {
            for (let def of cmake.compile_definitions) {
                outn(`add_compile_definitions(${def})`);
            }
        } else {
            outn(`add_compile_definitions(${cmake.compile_definitions})`);
        }
        nl();
    }

    if (params_.cmake.include_directories) {
        if (Array.isArray(params_.cmake.include_directories)) {
            for (let def of params_.cmake.include_directories) {
                outn(`include_directories(${def})`);
            }
        } else {
            outn(`include_directories(${params_.cmake.include_directories})`);
        }
        nl();
    }

    /** type {string[]} */
    let cmakeSources = [];

    if (params_.cmake.sources) {
        if (Array.isArray(params_.cmake.sources)) {
            for (let source of params_.cmake.sources) cmakeSources.push(source);
        } else {
            cmakeSources.push(cmake.sources);
        }
        nl();
    }

    for (let source of cmakeSources) {
        if (!source || typeof source !== 'string') continue;
        let ext = source.split('.').pop();
        source = path.resolve(source);
        if (ext === 'cmake') outn(`include(${source})`);
        else outn(`list(APPEND main_SRC ${source})`);
    }

    nl();

    outn('include(${CONFIG_APEAL_CMAKE_DIR}/apeal_sources.cmake)');
    outn('include(${CONFIG_COGNIT_CMAKE_DIR}/cognit_sources.cmake)');
    outn('include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources.cmake)');
    outn('include(${CONFIG_MANGL_CMAKE_DIR}/current_mangl_config_h.cmake)');
    outn('include(${CONFIG_MANGL_CMAKE_DIR}/project_target.cmake)');
    nl();

    if (params_.cmake.cmakelists) {
        for (let line of params_.cmake.cmakelists) outn(line);
        nl();
    }

    createAndWriteFile(`${params_.output}/app/src/main/cpp/CMakeLists.txt`, content_);
}

export function createManglConfigHeader() {
    outclear(generateHeader('cpp'));

    outnn(`#pragma once`);
    outn(`#define MANGL_APP_ID       "${project_.appId}"`);
    // outn(`#define MANGL_APP_COMPANY  "${project_.manglAppCompany}"`)
    outn(`#define MANGL_APP_TITLE    "${project_.name}"`);
    outn(`#define MANGL_APP_TAG      "${project_.appTag}"`);

    nl();

    if (project_.compileDefinitions) {
        for (let define of project_.compileDefinitions) {
            let assignIndex = define.indexOf('=');
            if (assignIndex <= 0) outn(`#define ${define}`);
            else {
                let name = define.substring(0, assignIndex);
                let value = define.substring(assignIndex + 1);
                outn(`#define ${name} ${value}`);
            }

            // let comps = define.split('=');
            // outn(`#define ${comps.join(' ')}`);
        }
        nl();
    }

    if (project_.ads) outn(`#define MANGL_ADS 1`);
    if (project_.admob) outn(`#define MANGL_ADS_ADMOB 1`);
    //if (project_.admobAppId)  outn(`#define MANGL_ADS_ADMOB_APPD_ID 1`);
    if (project_.admobBannerId) {
        outn(`#define MANGL_ADS_BANNER 1`);
        outn(`#define MANGL_ADS_BANNER_ADMOB 1`);
        outn(`#define MANGL_ADS_BANNER_ADMOB_UNIT_ID "${project_.admobBannerId}"`);
        outn(`// Test Banner Id`);
        outn(`//#define MANGL_ADS_BANNER_ADMOB_UNIT_ID "ca-app-pub-3940256099942544/9214589741"`);
    }

    if (project_.admobInterstitialId) {
        outn(`#define MANGL_ADS_INTERSTITIAL 1`);
        outn(`#define MANGL_ADS_INTERSTITIAL_ADMOB 1`);
        outn(`#define MANGL_ADS_INTERSTITIAL_ADMOB_UNIT_ID "${project_.admobInterstitialId}"`);
        outn(`// Test Interstitial Id`);
        outn(`//#define MANGL_ADS_INTERSTITIAL_ADMOB_UNIT_ID "ca-app-pub-3940256099942544/1033173712"`);
    }

    if (project_.admobAppOpenId) {
        outn(`#define MANGL_ADS_APPOPEN 1`);
        outn(`#define MANGL_ADS_APPOPEN_ADMOB 1`);
        outn(`#define MANGL_ADS_APPOPEN_ADMOB_UNIT_ID "${project_.admobAppOpenId}"`);
        outn(`// Test AppOpen Id`);
        outn(`//#define MANGL_ADS_APPOPEN_ADMOB_UNIT_ID "ca-app-pub-3940256099942544/9257395921"`);
    }

    if (project_.admobRewardedId) {
        outn(`#define MANGL_ADS_REWARDED 1`);
        outn(`#define MANGL_ADS_REWARDED_ADMOB 1`);
        outn(`#define MANGL_ADS_REWARDED_ADMOB_UNIT_ID "${project_.admobRewardedId}"`);
        outn(`// Test Rewarded Id`);
        outn(`//#define MANGL_ADS_REWARDED_ADMOB_UNIT_ID "ca-app-pub-3940256099942544/5224354917"`);
    }

    createAndWriteFile(`${params_.output}/app/src/main/cpp/mangl_config.h`, content_);
}
