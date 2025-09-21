// @ts-check

import {generateMake, generateNinja} from './buildfile_generate.mjs';

/**
 * 
 * @param {import ("./gengraphics.d.ts").GenerateGraphicsParams} params
 */

export async function generate(params) {

    let targetType = params.target.split('/').pop();

    switch(targetType){
        case 'Makefile': await generateMake(params); break;
        case 'build.ninja': await generateNinja(params); break;
        default: break;
    }
}
