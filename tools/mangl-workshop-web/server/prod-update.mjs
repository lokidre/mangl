#! /usr/bin/env zx

//import 'zx/globals';

import fs from "fs";

let deployRoot = '/srv/southgate/server';
//let deployTarget = `${deployRoot}/build`;
let backupsDir = `${deployRoot}/backup`;
let deployName = 'southgate_server';

try {
    let date = new Date();
    let timestamp = makeTimestamp(date);

    fs.rmSync('build', {recursive: true, force: true})
    fs.rmSync('node_modules', {recursive: true, force: true})

    await $`yarn`;
    await $`yarn build`;

    fs.mkdirSync(backupsDir, {recursive: true});

    //await $`7z a ${backupsDir}/${deployName}-${timestamp}.7z build node_modules`;

    fs.rmSync(`${deployRoot}/build`, {force: true, recursive: true});
    fs.rmSync(`${deployRoot}/node_modules`, {force: true, recursive: true});

    await $`sudo pm2 stop ${deployName}`;

    fs.renameSync('build', `${deployRoot}/build`);
    fs.renameSync('node_modules', `${deployRoot}/node_modules`);

    await $`sudo pm2 start ${deployName}`;
    await $`sudo pm2 status`;

} catch(err) {
    console.error(err);
    process.exit(1);
}


function makeTimestamp(date) {
    let year = date.getFullYear();
    let mo = date.getMonth();
    let dy = date.getDate();

    let hr = date.getHours();
    let mn = date.getMinutes();
    let sc = date.getSeconds();


    //let timestamp = new Date().toISOString();

    let timestamp = `${year}${pad(mo, 2)}${pad(dy, 2)}${pad(hr, 2)}${pad(mn, 2)}${pad(sc, 2)}`;

    return timestamp;

}

function moveFile(src, dst) {
    
    fs.rename(oldPath, newPath, function (err) {
        if (err) {
            if (err.code === 'EXDEV') {
                copy();
            } else {
                callback(err);
            }
            return;
        }
        callback();
    });

    function copy() {
        var readStream = fs.createReadStream(oldPath);
        var writeStream = fs.createWriteStream(newPath);

        readStream.on('error', callback);
        writeStream.on('error', callback);

        readStream.on('close', function () {
            fs.unlink(oldPath, callback);
        });

        readStream.pipe(writeStream);
    }    
}

function pad(num, size) {
    num = num.toString();
    while (num.length < size) num = "0" + num;
    return num;
}
