import {ipcMain, ipcRenderer} from 'electron';

import * as ID from './worker_ids';

let mainWindow_: Electron.BrowserWindow;

export function sendRenderer<T>(eventId: string, parm?: T) {
    mainWindow_.webContents.send(eventId, parm);
}

export function initWorker(win: Electron.BrowserWindow) {
    mainWindow_ = win;
}

ipcMain.on(ID.RequestLoadConfig, (event, arg) => {
    // SerialPort.list().then((ports) => {
    //     let availPorts = ports.filter((p) => p.manufacturer);

    //     sendRenderer(ID.RespondPortsListId, {ports: availPorts, count: availPorts.length});
    // });
});


ipcMain.on(ID.RequestLoadProject, async (event, arg) => {
});

