#! /usr/bin/env zx

import {genRes} from "./genres.mjs"

export async function generate(params){
    await genRes(params);
}

