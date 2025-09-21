import {PubSub} from './lib/pubsub'

import * as ID from './worker_ids'

export const pubsub_ = new PubSub()

//
// Status bar
//
//const statusBarId = 'statusBarId'
export interface StatusBarParams {
    text?: string
    color?: string
    type?: string
}

export function emitStatusBar(status: StatusBarParams) {
    pubsub_.dispatch(ID.SetStatusBarMessage, status)
}

// export function addStatusBarListener(listener: (status: StatusBarParams) => void) {
//     pubsub_.addListener(statusBarId, listener)
// }

// export function removeStatusBarListener(listener: (status: StatusBarParams) => void) {
//     pubsub_.removeListener(statusBarId, listener)
// }
