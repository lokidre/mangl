import {app, dialog, ipcMain} from 'electron';

import {ipcRenderer} from 'electron';
import * as ID from '../ipc/worker_ids';


import Store, {Schema} from 'electron-store';
import yaml from 'js-yaml';


import fs from "fs";
import path from "path/posix";

import { Project } from '../setup/project';
import { PackageDescConfig, YamlPackage } from '../lib/yamlPackageClasses';
import { sendRenderer } from '../ipc/worker';
import { Dialog } from '@mui/material';


const lastProjectLocationKey = 'lastProjectLocation';

  

export function showProjectOpenDialog() {
    let store = new Store();



    // let defaultDir = app.getPath("recent");
    // console.log(defaultDir);


    let defaultDir = store.get(lastProjectLocationKey);

    if (!defaultDir)
        defaultDir = app.getPath("home");

    dialog.showOpenDialog({
        title: 'Open Project',
        message: 'Open project containing mangl_config.yaml',
        defaultPath: defaultDir as string,
        //properties: ['openFile'],
        properties: ['openDirectory'],
    }).then((value: Electron.OpenDialogReturnValue) => {
        if (value.canceled)
            return;
        
        let projectDir = value.filePaths[0];
        store.set(lastProjectLocationKey, projectDir);

        loadProject(projectDir);
    
    }).catch((e) => {
        dialog.showErrorBox("Error!", e.message);
    });

    return;

    let projectDirs = dialog.showOpenDialogSync({
        title: 'Open Project',
        message: 'Open project containing mangl_config.yaml',
        defaultPath: defaultDir as string,
        //properties: ['openFile'],
        properties: ['openDirectory'],
    });

    if (!projectDirs)
        return;

    let projectDir = projectDirs[0];

    store.set(lastProjectLocationKey, projectDir);

    loadProject(projectDir);



        // .then((value: Electron.OpenDialogReturnValue) => {})
        // .catch((reason) => {});

}

export function saveProject(project: Project) {
}

export function loadProject(projectDir: string) {
    let project = new Project();

    project.projectDir = projectDir;
    project.configFilePath = path.join(projectDir, "mangl_config.yaml");
    project.configKeyPath = path.join(projectDir, "mangl_config.key");
    project.configDatPath = path.join(projectDir, "mangl_config.data");

    try {

        if (fs.existsSync(project.configFilePath)) {
            //throw new Error(`Can't find file: ${project.configFilePath}`);

            let configContents = fs.readFileSync(project.configFilePath, 'utf-8');

            project.configObject = yaml.load(configContents) as YamlPackage;
        } 

        if (project.configObject === undefined)
            project.configObject = new YamlPackage();


        if (!project.configObject.config)
            project.configObject.config = new PackageDescConfig();

        if (project.configObject.config.sections === undefined) {
            project.configObject.config.sections = [];
        }



        if (!fs.existsSync(project.configKeyPath)) 
            throw new Error(`Can't find file: ${project.configKeyPath}`);

        project.configKeyData = fs.readFileSync(project.configKeyPath, 'utf-8');


        sendRenderer(ID.OnProjectLoadedId, project);



    } catch(e) {
        dialog.showErrorBox("Error", e.message);
    }
}
