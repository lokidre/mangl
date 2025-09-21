import {ipcRenderer} from 'electron';

import * as ID from './worker_ids';
import * as GD from './gd';

export function requestLoadConfig() {
    ipcRenderer.send(ID.RequestLoadConfig, {});
}

ipcRenderer.on(ID.RespondLoadConfig, (event, data) => {
    GD.pubsub_.dispatch(ID.RequestLoadConfig, data);
});


export function requestLoadProject() {
    ipcRenderer.send(ID.RequestLoadProject, {});
}

ipcRenderer.on(ID.RespondLoadProject, (event, data) => GD.pubsub_.dispatch(ID.RequestLoadProject, data));
