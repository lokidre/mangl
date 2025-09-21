// @ts-check

import fs from 'fs';
import path from 'path/posix';

import {generateHeader} from './generate_header.mjs';
import {params_, project_, setup_} from './gengradle_generate.mjs';
import {createAndWriteFile, linkFile} from './lib_fs.mjs';
import {content_, nl, outclear, outn, outnn} from './lib_io.mjs';
import {createProjectTools} from './gengradle_tools.mjs';

export function createGradleSetup() {
    createSettingsGradle();
    createRootBuildGradle();
    createModuleBuildGradle();
    generateProGuardRules();

    // link to google-services.json if necessary
    if (fs.existsSync('google-services.json')) linkFile('.', `${params_.output}/app`, 'google-services.json');

    if (project_.crashlytics || project_.ads) {
        if (!fs.existsSync('google-services.json')) throw new Error('Ads or Crashlytics require google-services.json');
    }

    createProjectTools();
}

function createSettingsGradle() {
    outclear(generateHeader('gradle'));

    let mavenDeps = '';

    if (project_.ads) {

        // For mintegral ads
        mavenDeps = `
        maven {
              url = uri("https://dl-maven-android.mintegral.com/repository/mbridge_android_sdk_oversea")
        }
        maven {
              url = uri("https://artifact.bytedance.com/repository/pangle/")
        }        
`;            
    }

    outn(`
pluginManagement {
    repositories {
        google {
            content {
                includeGroupByRegex("com\\\\.android.*")
                includeGroupByRegex("com\\\\.google.*")
                includeGroupByRegex("androidx.*")
            }
        }
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
        ${mavenDeps}
    }
}
rootProject.name = "${project_.name}"
include(":app")

`);

    createAndWriteFile(`${params_.output}/settings.gradle.kts`, content_);
}

/**
 * @param {import ("./gengradle.d.ts").GradleDependencyOrPlugin} dep
 * @returns {boolean}
 */
function checkDependency(dep) {
    if (dep.ads && !project_.ads) return false;
    if (dep.firebase && !project_.firebase) return false;
    if (dep.crashlytics && !project_.crashlytics) return false;
    if (dep.multiplayer && !project_.multiplayer) return false;
    return true;
}

function createRootBuildGradle() {
    outclear(generateHeader('gradle'));

    outn('// Top-level build file where you can add configuration options common to all sub-projects/modules.');
    outn('plugins {');

    for (let plugin of setup_.root.plugins) {
        if (!checkDependency(plugin)) continue;

        outn(`    id ("${plugin.id}") version "${plugin.version}" apply false`);
    }

    outn('}');

    createAndWriteFile(`${params_.output}/build.gradle.kts`, content_);
}

function createModuleBuildGradle() {
    outclear(generateHeader('gradle'));

    outnn(`@file:Suppress("UnstableApiUsage")`);

    outn('plugins {');
    for (let plugin of setup_.module.plugins) {
        if (!checkDependency(plugin)) continue;

        outn(`    id ("${plugin.id}")`);
    }
    outnn('}');

    let cmakeBuildType = project_.buildType === 'debug' ? 'Debug' : 'Release';
    // let cmakeBuildType = "Debug";
    // let cmakeStlLib = "c++_shared";
    let cmakeStlLib = 'c++_static';
    let ndkAbiFilters = '';

    if (project_.abiFilters) {
        let archs = project_.abiFilters.map((v) => `"${v}"`).join(',');
        ndkAbiFilters = `abiFilters.addAll(arrayOf(${archs}))`;
    } else if (project_.debug) {
        // ndkAbiFilters = `abiFilters.addAll(arrayOf("armeabi-v7a", "arm64-v8a", "x86", "x86_64"))`;
        ndkAbiFilters = `
                // abiFilters.addAll(arrayOf("armeabi-v7a"))
                // abiFilters.addAll(arrayOf("arm64-v8a"))
                // abiFilters.addAll(arrayOf("x86"))
                // abiFilters.addAll(arrayOf("x86_64"))
`;
    }

    //     outn(`
    // kotlin {
    //      jvmToolchain(11)
    // }
    // `);

    outn(`
android {
    namespace = "${project_.namespace}"
    compileSdk = ${setup_.compileSdk}
    ndkVersion = "${setup_.ndkVersion}"
    buildToolsVersion = "${setup_.buildToolsVersion}"
`);

    let signingConfig = '';

    if (project_.release) {
        if (project_.signingStoreFile) {
            let storeFilePath = path.resolve(project_.signingStoreFile);

            outn(`    signingConfigs {`);
            outn(`        create("release") {`);
            outn(`            storeFile = file("${storeFilePath}")`);
            if (project_.signingKeyPassword) outn(`            storePassword = "${project_.signingStorePassword}"`);
            if (project_.signingKeyAlias) outn(`            keyAlias = "${project_.signingKeyAlias}"`);
            if (project_.signingKeyPassword) outn(`            keyPassword = "${project_.signingKeyPassword}"`);
            outn(`        }`);
            outn(`    }`);
        }

        signingConfig = `signingConfig = signingConfigs.getByName("release")`;
    }

    let crashlyticsReleaseOpts = '';
    if (project_.crashlytics) {
        crashlyticsReleaseOpts = `

            firebaseCrashlytics {
                nativeSymbolUploadEnabled = true
                mappingFileUploadEnabled = true
            }        
`;
    }

    outn(`
    sourceSets {
        getByName("main") {
            // assets.srcDirs("src/main/assets")
            assets.setSrcDirs(listOf("src/main/assets"))
        }
    }

    defaultConfig {
        applicationId = "${project_.appId}"
        minSdk = ${project_.minSdk ?? setup_.minSdk}
        targetSdk = ${project_.targetSdk ?? setup_.targetSdk}
        versionCode = ${project_.versionCode}
        versionName = "${project_.versionName}"

        externalNativeBuild {
            cmake {
                arguments.add("-DCMAKE_ANDROID_ASSETS_DIRECTORIES=\${file("src/main/assets")}")
                arguments.add("-DANDROID_STL=${cmakeStlLib}")
                arguments.add("-DCMAKE_BUILD_TYPE=${cmakeBuildType}")
            }
        }
    }

    buildTypes {

        getByName("release") {
            isDebuggable = false
            isJniDebuggable = false
            isMinifyEnabled = false
            isShrinkResources = false
            // isMinifyEnabled = true
            // isShrinkResources = true

            proguardFiles(getDefaultProguardFile("proguard-android-optimize.txt"), "proguard-rules.pro")
            ${signingConfig}
            ${crashlyticsReleaseOpts}

            ndk {
                debugSymbolLevel = "FULL"
            }            
        }

        getByName("debug") {
            isDebuggable = true
            isJniDebuggable = true
            isMinifyEnabled = false
            isShrinkResources = false

            ndk {
                ${ndkAbiFilters}
            }
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "${setup_.cmakeVersion}"
        }
    }

    buildFeatures {
        prefab = true
    }

    dependenciesInfo {
        includeInBundle = true
        includeInApk = true
    }

    kotlinOptions {
        jvmTarget = "17"
        tasks.register("testClasses")
    }
`);

    outn(`}`); // android section

    // outn(`
    // tasks.named("mergeAssets") {
    //     outputs.upToDateWhen { false }
    // }
    // `);

//     outn(`
// afterEvaluate {
//     tasks.matching { task ->
//         task.name.equals("mergeReleaseAssets", ignoreCase = true)
//     }.configureEach {
//         outputs.upToDateWhen { false }
//     }
// }
// `);

    outn(`dependencies {`);

    generateGradleDependencies(setup_.dependencies, 'Essential Packages');

    outn(`}`);

    createAndWriteFile(`${params_.output}/app/build.gradle.kts`, content_);
}

/**
 * @param {import ("./gengradle.d.ts").GradleSetupDependency[]} deps
 * @param {string} comments
 */
function generateGradleDependencies(deps, comments) {
    if (!deps) return;

    for (let dep of deps) {
        if (!checkDependency(dep)) continue;

        if (dep.platform) {
            outn(`   implementation(platform("${dep.name}:${dep.version}"))`);
        } else {
            outn(`   implementation("${dep.name}:${dep.version}")`);
        }
    }
}

function generateProGuardRules() {
    outclear(generateHeader('cmake'));

    outn(`# For more details, see`);
    outn(`#   http://developer.android.com/guide/developing/tools/proguard.html`);

    // if (project_.debug) {
    outn(`-keepattributes LineNumberTable,SourceFile`);
    // }

    outn(`-renamesourcefileattribute SourceFile`);

    outn(`-keep class mangl.** { *; }`);
    outn(`-keep class com.google.android.gms.** { *; }`);
    outn(`-keep class androidx.games.** {*;}`);
    outn(`-keep public class * extends java.lang.Exception`);
    outn(`-dontwarn android.media.LoudnessCodecController$OnLoudnessCodecUpdateListener`);
    outn(`-dontwarn android.media.LoudnessCodecController`);

    // Ads
    outn(`-keep class com.bytedance.sdk.** { *; }`);


    createAndWriteFile(`${params_.output}/app/proguard-rules.pro`, content_);
}
