// @ts-check

// @ts-ignore: Node modules
import fs from 'fs';

// @ts-ignore: Node modules
import path from 'path/posix';


/**
 * 
 * @param {import ("./gensample.d.ts").GenSampleContext } context
 */
export async function genSampleXcode(context) {
    console.log(`Generating xcodegen projects...`);


    for (let target of context.project.targets) {
        await genSampleXcodeTarget(context, target);
    }

}


/**
 * 
 * @param {import ("./gensample.d.ts").GenSampleContext } context
 * @param {import ("./gensample.d.ts").SampleProjectTarget } target
 */
async function genSampleXcodeTarget(context, target) {
    let targetDir = `_build_ios_${target.name}`;

    fs.mkdirSync(targetDir, {recursive: true});


    await genSampleXcodeInfoPlist(context, target, targetDir);
    await genSampleXcodeSourcesYaml(context, target, targetDir);
    await genSampleXcodeEntitlements(context, target, targetDir);
    await genSampleXcodeGenproj(context, target, targetDir);

    // Run genproj
    context.genproj.generate({
        projectConfig: `${targetDir}/genproj.yaml`,
        sources: [
            path.join(targetDir, 'sources.yaml'), 
            `${context.config.mangl_dir}/samples/common/common_sources.yaml`,
            `${context.config.mangl_scripts_dir}/mangl_sources.yaml`,
            `${context.config.cognit_scripts_dir}/cognit_sources.yaml`,
            `${context.config.apeal_scripts_dir}/apeal_sources.yaml`,
        ],
        buildConfig: `${context.manglRootDir}/.build-config.yaml`,
        targetDir: targetDir,
        targetXcodeGen: "project.yml",
    })
    

}

/**
 * 
 * @param {import ("./gensample.d.ts").GenSampleContext } context
 * @param {import ("./gensample.d.ts").SampleProjectTarget } target
 * @param {string} targetDir
 */
export async function genSampleXcodeSourcesYaml(context, target, targetDir) {
    let object = {
        sources: [
            {dir: "..", files: target.sources},
            {
                dir: `${context.manglSamplesCommonDir}/src`,
                source_group: 'SamplesCommon', 
                include_directories: context.manglSamplesCommonDir, 
                //files: ['test_app.cpp', 'test_app.h']
            },
            // {
            //     dir: `${context.manglSamplesCommonDir}/textures`,
            //     bundle_group: 'SamplesTextures', 
            //     //files: ['tex_common.png', 'tex_common.yaml', 'tex_common.hpp']
            // },
        ]
    }

    let contents = await context.yaml.stringify(object);

    fs.writeFileSync(`${targetDir}/sources.yaml`, contents);

}

/**
 * 
 * @param {import ("./gensample.d.ts").GenSampleContext } context
 * @param {import ("./gensample.d.ts").SampleProjectTarget } target
 * @param {string} targetDir
 */
export async function genSampleXcodeInfoPlist(context, target, targetDir) {
    let project = context.project;

    // Add multiplayer options
    let multiplayerInfoPlist = '';
    if (project.options) {
        let options = project.options;
        if (options.multiplayer) {
            const multiplayerProtocolName = 'msample';
            multiplayerInfoPlist += `

            <key>NSBluetoothAlwaysUsageDescription</key>
            <string>Bluetooth is required for local multiplayer mode.</string>
            <key>NSBonjourServices</key>
            <array>
                <string>_${multiplayerProtocolName}-proto._tcp</string>
                <string>_${multiplayerProtocolName}-proto._udp</string>
            </array>
            
            `;
    

        }
    }

    let content = `<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleDevelopmentRegion</key>
    <string>en</string>
    <key>CFBundleDisplayName</key>
    <string>${target.name}</string>
    <key>CFBundleExecutable</key>
    <string>$(EXECUTABLE_NAME)</string>
    <key>CFBundleIdentifier</key>
    <string>$(PRODUCT_BUNDLE_IDENTIFIER)</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundleName</key>
    <string>$(PRODUCT_NAME)</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleShortVersionString</key>
    <string>$(MARKETING_VERSION)</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>CFBundleVersion</key>
    <string>$(CURRENT_PROJECT_VERSION)</string>
    <key>LSRequiresIPhoneOS</key>
    <true/>
	<key>UILaunchStoryboardName</key>
	<string>launch_screen.storyboard</string>
    <key>UIPrerenderedIcon</key>
    <true/>
    <key>UIRequiresFullScreen</key>
    <true/>
    <key>UIStatusBarHidden</key>
    <true/>
    <key>UISupportedInterfaceOrientations</key>
    <array>
        <string>UIInterfaceOrientationPortrait</string>
        <string>UIInterfaceOrientationPortraitUpsideDown</string>
    </array>
    <key>UIViewControllerBasedStatusBarAppearance</key>
    <true/>
    ${multiplayerInfoPlist}
</dict>
</plist>
`;

    fs.writeFileSync(`${targetDir}/info.plist`, content);

}

/**
 * 
 * @param {import ("./gensample.d.ts").GenSampleContext } context
 * @param {import ("./gensample.d.ts").SampleProjectTarget } target
 * @param {string} targetDir
 */
export async function genSampleXcodeEntitlements(context, target, targetDir) {
    let content = `<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>aps-environment</key>
    <string>development</string>
</dict>
</plist>  
`;

    fs.writeFileSync(`${targetDir}/sample.entitlements`, content);

}

/**
 * 
 * @param {import ("./gensample.d.ts").GenSampleContext } context
 * @param {import ("./gensample.d.ts").SampleProjectTarget } target
 * @param {string} targetDir
 */
export async function genSampleXcodeGenproj(context, target, targetDir) {

    //console.log("development_team: ", process.env.MANGL_SAMPLES_DEVELOPMENT_TEAM);
    let project = context.project;

    let compile_definitions = ["MANGL_NO_CONFIG_H=1"];

    compile_definitions.push('MANGL_DEBUG_STATS_FPS=1');

    if (project.compile_definitions !== undefined) {
        for (let def of project.compile_definitions) {
            compile_definitions.push(def);
        }
    }


    let object = {
        project: {
            name: target.name,
            display_name: target.name,
            version: "0.0.1",
            build: 1,
            platform: 'iOS',
            deployment_target: "14.0",
            targeted_device_family: "1",
            compile_definitions: compile_definitions,
            // other_cxxflags: ["-fcxx-modules", "-fmodules"],
            // warning_cflags: ["-Wno-undef", "-Wno-ambiguous-macro", "-Wno-deprecated-declarations"],
            // other_ldflags: ["-all_load", "-ObjC"],
            app_launch_storyboard: `${context.manglSamplesCommonDir}/ios/launch_screen.storyboard`,
        },

        xcode: {
            product_bundle_identifier: "com.example.mangl.-",
            infoplist_file: "info.plist",
            code_sign_entitlements: "sample.entitlements",

            // @ts-ignore: process
            development_team: process.env.MANGL_SAMPLES_DEVELOPMENT_TEAM,
          
            // shared_frameworks: [
            //   'AudioToolbox', 'AVFoundation', 'CoreAudio', 
            //   'OpenAL',
            //   'CoreGraphics', 'CoreMedia', 'MediaPlayer', 
            //   'Metal', 'MetalKit',
            //   'MessageUI', 
            //   'QuartzCore', 
            //   'SystemConfiguration', 
            //   'UIKit'
            // ],
          
            // shared_libraries: ['libsqlite3', 'libxml2', 'libz'],
        },
    };

    if (project.options !== undefined) {
        let options = project.options;
        if (options.multiplayer) {
            object.project["multiplayer"] = 1;
            object.project.compile_definitions.push("MANGL_MULTIPLAYER=1");
            object.project.compile_definitions.push("MANGL_BLUETOOTH=1");
        }
    }


    let contents = await context.yaml.stringify(object);

    fs.writeFileSync(`${targetDir}/genproj.yaml`, contents);

}
