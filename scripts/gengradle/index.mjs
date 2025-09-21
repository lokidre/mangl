// @ts-check

import {generateGradle} from './gengradle_generate.mjs';


/**
 * 
 * @param {import ("./gengradle.d.ts").GenerateGradleParams} params
 */

export async function generate(params) {

    try {
        await generateGradle(params);
    } catch(e) {
        console.error(e.message);

        // @ts-ignore - process global object
        process.exit(1);
    }

}

