// import {SerialPort} from 'serialport'
import {ipcMain, ipcRenderer} from 'electron'

// import * as Lora from './lib/lora'

import * as ID from './worker_ids'

let mainWindow_: Electron.BrowserWindow

export function sendRenderer<T>(eventId: string, parm?: T) {
    mainWindow_.webContents.send(eventId, parm)
}

export function initWorker(win: Electron.BrowserWindow) {
    mainWindow_ = win
    //console.log("worker init:");

    //console.log("list: ", SerialPort.list());

    //electron.app.whenReady().then(() => {

    // let listenersCount = ipcMain.listenerCount(ID.ipcPortsListId);
    // console.log("listeners:", listenersCount);

    // });
}

ipcMain.on(ID.RequestPortsListId, (event, arg) => {
    // SerialPort.list().then((ports) => {
    //     let availPorts = ports.filter((p) => p.manufacturer)

    //     sendRenderer(ID.RespondPortsListId, {ports: availPorts, count: availPorts.length})
    // })
})

function onLoraData(
    // reason: Lora.LoraCallbackReason,
    // data: Buffer,
    // address: number,
    // rssi: number,
    // snr: number
) {
    // if (reason === 'data') {
    //     let payload = ''
    //     for (let ch of data) {
    //         if (ch >= 0x20 && ch < 0xfe) {
    //             payload += String.fromCharCode(ch)
    //         }
    //     }

    //     //console.debug("Payload:", payload);

    //     sendRenderer<ID.LoraReceivePacket>(ID.EventLoraReceivePacket, {
    //         payload: payload,
    //         address: address,
    //         rssi: rssi,
    //         snr: snr,
    //     })
    // }
}

ipcMain.on(ID.RequestLoraSerialConnect, async (event, arg) => {
    // await Lora.release()

    // //console.debug(arg.config)
    // //arg.config.debugAt = true;

    // try {
    //     let result = await Lora.init(arg.config as Lora.Config, onLoraData)
    //     await Lora.startListening()

    //     // await Lora.open(arg.config as Lora.Config)
    //     // let result = await Lora.at("AT", {expect: /.*OK.*/g})
    //     // console.log("Result:", result)

    //     sendRenderer<ID.LoraConnectionEvent>(ID.EventLoraSerialConnect, {
    //         status: true,
    //         response: result,
    //     })
    // } catch (err) {
    //     sendRenderer<ID.LoraConnectionEvent>(ID.EventLoraSerialConnect, {status: false, error: err})
    // }
})

ipcMain.on(ID.RequestLoraSerialDisconnect, async (event, arg) => {
    // await Lora.release()
    // sendRenderer<ID.LoraConnectionEvent>(ID.EventLoraSerialConnect, {status: false})
})

ipcMain.on(ID.RequestLoraSendText, async (event, arg) => {
    // try {
    //     await Lora.send(arg)

    //     sendRenderer<ID.LoraConnectionEvent>(ID.EventLoraSendText, {status: true})
    // } catch (err) {
    //     sendRenderer<ID.LoraConnectionEvent>(ID.EventLoraSendText, {status: false, error: err})
    // }
})
