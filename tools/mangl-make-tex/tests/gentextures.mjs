#! /usr/bin/env zx

import sharp from 'sharp';

import {config} from '../../../.build-config.mjs';

let resDir = `${config.mangl_samples_resources_dir}/maketex`;
let texturesDir = `${resDir}/textures`;
let renderDir = `${resDir}/render`;
let renderFontsDir = `${resDir}/render`;
let fontsDir = '${config.mangl_samples_resources_dir}/fonts';
let srcGenDir = './generated';


await createSquares(250);



function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

function getRandomColor() {
    const r = getRandomInt(0, 255);
    const g = getRandomInt(0, 255);
    const b = getRandomInt(0, 255);
    return `rgb(${r}, ${g}, ${b})`;
};

function pad(num, size) {
    num = num.toString();
    while (num.length < size) num = "0" + num;
    return num;
}

async function createSquares(imageCount) {
    for (let i = 0; i < imageCount; i++) {
        const randomWidth = getRandomInt(3, 10);
        const randomHeight = getRandomInt(300, 800);

        const outputImagePath = path.join(renderDir, `square_random_vertical_${pad(i + 1, 3)}.png`);

        const color = getRandomColor();

        const image = sharp({
            create: {
                width: randomWidth,
                height: randomHeight,
                channels: 3,
                background: color,
            },
        });

        const text = pad(i + 1, 3);

        const fontSize = Math.max(20, Math.floor(randomWidth / 8));

        await image.composite([
                {
                    input: Buffer.from(`
            <svg width="${randomWidth}" height="${randomHeight}">
              <text x="50%" y="50%" font-size="${fontSize}" text-anchor="middle" alignment-baseline="middle" fill="white">${text}</text>
            </svg>
          `),
                    blend: 'over',
                },
            ])
            .toFile(outputImagePath);

        //console.log(`Created: ${outputImagePath}`);
    }
}

