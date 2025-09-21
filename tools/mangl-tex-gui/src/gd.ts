import {PubSub} from './lib/pubsub';

import * as ID from './worker_ids';

export const pubsub_ = new PubSub();

//
// Status bar
//
export interface StatusBarParams {
    text?: string;
    color?: string;
    type?: string;
}

export function emitStatusBar(status: StatusBarParams) {
    pubsub_.dispatch(ID.SetStatusBarMessage, status);
}
