#! /usr/bin/env zx

try {

    let output = await $`xcodebuild -showBuildSettings`;
    
    let lines = output.stdout.split('\n');

    //console.log(lines[0])

    let DWARF_DSYM_FOLDER_PATH, DWARF_DSYM_FILE_NAME;
    let PRODUCT_NAME, EXECUTABLE_PATH, TARGET_BUILD_DIR;
    let UNLOCALIZED_RESOURCES_FOLDER_PATH;


    for (let line of lines) {
        let index = line.indexOf('=');
        if (index < 0)
            continue;
        let name = line.substring(0, index);
        name = name.trim();

        let value = line.substring(index + 1);
        value = value.trim();

        switch (name) {
        case 'DWARF_DSYM_FOLDER_PATH': DWARF_DSYM_FOLDER_PATH = value; break;
        case 'DWARF_DSYM_FILE_NAME': DWARF_DSYM_FILE_NAME = value; break;
        case 'PRODUCT_NAME': PRODUCT_NAME = value; break;
        case 'EXECUTABLE_PATH': EXECUTABLE_PATH = value; break;
        case 'TARGET_BUILD_DIR': TARGET_BUILD_DIR = value; break;
        case 'UNLOCALIZED_RESOURCES_FOLDER_PATH': UNLOCALIZED_RESOURCES_FOLDER_PATH = value; break;
        default: break;
        }

        console.log(name, value);

        //console.log(index, name)


    }

    // DWARF_DSYM_FOLDER_PATH = DWARF_DSYM_FOLDER_PATH.replaceAll("Debug", "AppStore");
    // TARGET_BUILD_DIR = TARGET_BUILD_DIR.replaceAll("Debug", "AppStore");

    // let args = [
    //     `${DWARF_DSYM_FOLDER_PATH}/${DWARF_DSYM_FILE_NAME}`,
    //     `${DWARF_DSYM_FOLDER_PATH}/${DWARF_DSYM_FILE_NAME}/Contents/Resources/DWARF/${PRODUCT_NAME}`,
    //     `${DWARF_DSYM_FOLDER_PATH}/${DWARF_DSYM_FILE_NAME}/Contents/Info.plist`,
    //     `${TARGET_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/GoogleService-Info.plist`,
    //     `${TARGET_BUILD_DIR}/${EXECUTABLE_PATH}`,
    // ];

    // // let args = [
    // //     `-gsp`, `../GoogleService-Info.plist`,
    // //     `--`,
    // //     `${DWARF_DSYM_FOLDER_PATH}/${DWARF_DSYM_FILE_NAME}`,
    // //     `${DWARF_DSYM_FOLDER_PATH}/${DWARF_DSYM_FILE_NAME}/Contents/Resources/DWARF/${PRODUCT_NAME}`,
    // //     `${DWARF_DSYM_FOLDER_PATH}/${DWARF_DSYM_FILE_NAME}/Contents/Info.plist`,
    // //     `${TARGET_BUILD_DIR}/${EXECUTABLE_PATH}`,
    // // ];


    // //await $`Pods/FirebaseCrashlytics/run --debug ${args}`;

    // console.log(args)

    //console.log(output.stdout);

} catch (p) {
    console.log(`Error:${p.exitCode}:${p.stderr}${p.stdout}`);
}
