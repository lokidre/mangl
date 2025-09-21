import {ipcRenderer} from 'electron';

import * as ID from './worker_ids';
import * as GD from './gd';
import { Project } from '../setup/project';

export function requestFileData(data: string) {
    // console.log('worket_api.ts  requestFileData')
    // console.log('workerApi'+ data)
    ipcRenderer.send(ID.RequestFileDataId, data);
}

ipcRenderer.on(ID.RespondFileDataId, (event, data) => {
    // console.log('wotrker_api.ts, RespondFileDataId', data)

    GD.pubsub_.dispatch(ID.RespondFileDataId, data);
});






ipcRenderer.on(ID.OnProjectLoadedId, (event, data) => {    
    GD.pubsub_.dispatch(ID.OnProjectLoadedId, data);
});


export function requestProjectOpenDialog() {
    ipcRenderer.send(ID.RequestProjectOpenDialogId, {});
}

export function requestProjectSave(project: Project) {
    ipcRenderer.send(ID.RequestProjectSaveId, project);
}
