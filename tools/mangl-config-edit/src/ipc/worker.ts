import {ipcMain} from 'electron';

import * as ID from './worker_ids';

import {serverYmlDataToClient} from '../lib/modifyData';


import {showProjectOpenDialog } from '../main/project_tools';

let mainWindow_: Electron.BrowserWindow;

export function sendRenderer<T>(eventId: string, parm?: T) {
    // console.log('worker.ts, sendRenderer ',)
    // console.log('renderer ', eventId, parm)
    mainWindow_.webContents.send(eventId, parm);
}

export function initWorker(win: Electron.BrowserWindow) {
    mainWindow_ = win;
}

ipcMain.on(ID.RequestFileDataId, (event, data: string) => {
    // console.log('worker.ts, ipcMain.on(ID.RequestFileDataId', data)
    // let fileData = fs.readFileSync(path as string, { encoding: 'utf8', flag: 'r' })
    let res = serverYmlDataToClient(data);

    sendRenderer(ID.RespondFileDataId, res);
});

ipcMain.on(ID.RequestProjectOpenDialogId, (event: any, data: any) => {
    showProjectOpenDialog();
});
