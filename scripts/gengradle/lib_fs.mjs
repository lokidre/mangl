// @ts-check

import fs from 'fs';
import path from 'path/posix';

import {printStat} from './lib_stats.mjs';

/**
 * @param {string} srcdir
 * @param {string} dstdir
 * */
export function copyStaticFiles(srcdir, dstdir) {
    fs.mkdirSync(dstdir, {recursive: true});

    let entries = fs.readdirSync(srcdir, {
        withFileTypes: true,
        //recursive: true,
    });

    //console.log(entries);

    for (let entry of entries) {
        if (entry.isDirectory()) {
            copyStaticFiles(srcdir + '/' + entry.name, dstdir + '/' + entry.name);
            continue;
        }

        let dstpath = dstdir + '/' + entry.name;

        // fs.copyFileSync(entry.path + '/' + entry.name, dstpath);
        fs.copyFileSync(entry.parentPath + '/' + entry.name, dstpath);
    }
}

/**
 * @param {string} fileName
 * @param {string} content
 */
export function createAndWriteFile(fileName, content) {
    let dirname = path.dirname(fileName); // extract directory name from path
    //let filename = path.basename(fileName); // extract file name from path

    //console.log(dirname)

    fs.mkdirSync(dirname, {recursive: true});

    // if (!fs.existsSync(dirname))
    //     throw new Error(`Directory doesn't exist: ${dirname}`);

    fs.writeFileSync(fileName, content);

    printStat(fileName);
}

/**
 * @param {string} src
 * @param {string} dest
 * @param {string} filename
 */
export function linkFile(src, dest, filename) {
    //let dirname = path.dirname(dest); // extract directory name from path
    let dirname = dest;
    let destpath = dest + '/' + filename;

    fs.mkdirSync(dirname, {recursive: true});
    if (fs.existsSync(destpath)) fs.unlinkSync(destpath);
    fs.linkSync(src + '/' + filename, destpath);

    printStat(destpath);
}

/**
 * @param {string} srcdir
 * @param {string} destdir
 * @param {string} dirname
 */
function symlinkDir(srcdir, destdir, dirname) {
    let basedir = path.dirname(destdir); // extract directory name from path

    fs.mkdirSync(destdir, {recursive: true});

    let destpath = destdir + '/' + dirname;
    if (fs.existsSync(destpath)) fs.unlinkSync(destpath);

    fs.symlinkSync(srcdir + '/' + dirname, destpath, 'dir');
}

/**
 * @param {string} src
 * @param {string} dest
 * @param {string} folderName
 */
export function copyFiles(src, dest, folderName) {
    fs.mkdirSync(dest + '/' + folderName, {recursive: true});

    // , (error) => {
    //     if(error){
    //         console.log("Error Found:", error);
    //     }
    //     else{
    //         console.log("Folder created successfully:",dest + `/${folderName}` )
    //     }
    // });

    let entries = fs.readdirSync(src + `/${folderName}`);

    for (let file in entries) {
        console.log(src + `/${folderName}/` + file);
        //fs.readFileSync("example_file.txt", "utf8")

        fs.copyFileSync(src + `/${folderName}/` + file, dest + `/${folderName}/` + file, fs.constants.COPYFILE_EXCL);

        // , (err) => {
        //     if (err) {
        //       console.log("Error Found:", err);
        //     }
        //     else{
        //         console.log("File copied successfully: ", file)
        //     }
        // })
    }
}

/**
 * @param {string} srcdir
 * @param {string} dstdir
 */
export function linkDirContents(srcdir, dstdir) {
    let entries = fs.readdirSync(srcdir);

    for (let file of entries) {
        //fs.linkSync(`${srcdir}/${file}`, `${dstdir}/${file}`);
        fs.symlinkSync(`${srcdir}/${file}`, `${dstdir}/${file}`);
    }
}

/**
 * @param {string} dir
 * @param {string[]} fileList
 */
export function readFilesRecursive(dir, fileList = []) {
    const files = fs.readdirSync(dir);
    files.forEach((file) => {
        const filePath = path.join(dir, file);
        const stat = fs.statSync(filePath);
        if (stat.isDirectory()) {
            readFilesRecursive(filePath, fileList);
        } else {
            fileList.push(filePath);
        }
    });
    return fileList;
}
