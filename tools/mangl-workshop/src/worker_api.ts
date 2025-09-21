import {ipcRenderer} from 'electron'

// import * as Lora from './lib/lora'
import * as ID from './worker_ids'
import * as GD from './gd'

// export interface LoraPacket {
//     text: string;
// }

export function requestPortsList() {
    ipcRenderer.send(ID.RequestPortsListId, {})
}

ipcRenderer.on(ID.RespondPortsListId, (event, data) =>
    GD.pubsub_.dispatch(ID.RespondPortsListId, data)
)

// export function requestLoraConnect(config: Lora.Config) {
//     ipcRenderer.send(ID.RequestLoraSerialConnect, {config: config})
// }

// export function requestLoraDisconnect(config: Lora.Config) {
//     ipcRenderer.send(ID.RequestLoraSerialDisconnect, {config: config})
// }

ipcRenderer.on(ID.EventLoraSerialConnect, (event, data) =>
    GD.pubsub_.dispatch(ID.EventLoraSerialConnect, data)
)

ipcRenderer.on(ID.EventLoraReceivePacket, (event, data) =>
    GD.pubsub_.dispatch(ID.EventLoraReceivePacket, data)
)

ipcRenderer.on(ID.EventLoraSendText, (event, data) =>
    GD.pubsub_.dispatch(ID.EventLoraSendText, data)
)

export function requestLoraSend(text: string) {
    ipcRenderer.send(ID.RequestLoraSendText, text)
}
