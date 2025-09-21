#! /usr/bin/env zx

import {genSample} from '../../common/gensample.mjs';

await genSample({
    name: 'nearby_ping',
    multiplayer: true,
    bluetooth: true,
});
