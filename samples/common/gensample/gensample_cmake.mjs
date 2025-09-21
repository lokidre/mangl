// @ts-check

// @ts-ignore: node packages
import fs from 'fs';


/**
 * 
 * @param {import ("./gensample.d.ts").GenSampleContext } context
 */
export async function genSampleCmake(context) {
    console.log(`Generating cmake projects...`);

    let project = context.project;

    let targetDir = '_build_cmake';
    fs.mkdirSync(targetDir, {recursive: true});

    let cmake = `cmake_minimum_required(VERSION 3.20)\n`;
    cmake += '\n';

    let projectName = project.name;
    if (!projectName.includes('sample_'))
        projectName = 'sample_' + projectName;
    if (!projectName.includes('mangl_'))
        projectName = 'mangl_' + projectName;

    cmake += `project(${projectName})\n`;
    cmake += '\n';

    cmake += `set(CONFIG_PROJECT_NO_CONFIG_H ON)\n`;

    if (project.options) {
        let options = project.options;
        if (options.multiplayer)  cmake += `set(CONFIG_PROJECT_MULTIPLAYER ON)\n`;
        if (options.console)  cmake += `set(CONFIG_PROJECT_CONSOLE ON)\n`;
        if (options.bluetooth)  cmake += `set(CONFIG_PROJECT_BLUETOOTH ON)\n`;
        cmake += '\n';
    }



    cmake += `add_compile_definitions(MANGL_DEBUG_STATS_FPS=1)\n`;
    cmake += '\n';


    if (project.compile_definitions) {
        for (let def of project.compile_definitions) {
            cmake += `add_compile_definitions(${def})\n`;
        }
        cmake += '\n';
    }

    if (project.set) {
        for (let def of project.set) {
            cmake += `set(${def})\n`;
        }
        cmake += '\n';
    }


    if (project.include_directories) {
        for (let val of project.include_directories) {
            cmake += `include_directories(${val})\n`;
        }
        cmake += '\n';
    }

    
    cmake += `include(${context.manglSamplesCommonDir}/tests.cmake)\n`;
    cmake += '\n';

    if (project.cmake) {
        cmake += `include(${context.projectDir}/${project.cmake})\n`;
        cmake += '\n';
    }


    for (let target of project.targets) {
        cmake += `addTest(${target.name} ${context.projectDir}/${target.sources.join(' ')})\n`;
    }

    fs.writeFileSync(`${targetDir}/CMakeLists.txt`, cmake);
}

