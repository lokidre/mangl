export const RequestPortsListId = 'ipcRequestPortsListId'
export const RespondPortsListId = 'ipcPortsListId'

export interface LoraConnectionEvent {
    error?: Error
    response?: string
    status: boolean
}

export const RequestLoraSerialConnect = 'RequestLoraSerialConnect'
export const RequestLoraSerialDisconnect = 'RequestLoraSerialDisconnect'
export const EventLoraSerialConnect = 'RespondLoraSerialConnect'

export const RequestLoraSendText = 'RequestLoraSendTextId'
export const EventLoraSendText = 'RespondLoraSendTextId'

export interface LoraConnectionEvent {
    status: boolean // true connected
    path?: string // connection device path
}

export const EventLoraReceivePacket = 'EventLoraReceivePacketId'

export interface LoraReceivePacket {
    payload: string
    address: number
    rssi: number
    snr: number
}

export const SetStatusBarMessage = 'SetStatusBarMessage'
