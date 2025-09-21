#! /usr/bin/env zx

import fs from 'fs';

export async function genRes(params) {
    let yamlFile = params.yamlFile;
    let outputFile = params.outputFile;

    const YAML = await import(params.apealJsDir + '/yaml.mjs');
    const doc = YAML.parse(fs.readFileSync(yamlFile, 'utf8'));
    const resources = doc.resources;

    let genDate = new Date();
    let dateString = genDate.toISOString().split('T')[0];

    out(`// *** Automatically Generated *** DO NOT EDIT ***`);
    out(`// * Generated Date: ${dateString}`);
    out(`#pragma once`);
    nl();
    out(`#include <mangl/resources.h>`);
    out(`#include <mangl/audio.h>`);
    nl();
    out(`namespace ${resources.namespace} {`);
    nl();

    if(resources.textures){
        let enumName = 'TexturesRes';
        out(`enum class ${enumName}: int {`);
        genEnum(resources.textures);
        out(`};`);
        nl();
        out(`constexpr bool operator !(${enumName} e) noexcept { return e == static_cast<${enumName}>(0); }`);
        nl();
    }

    if(resources.music){
        let enumName = 'MusicRes';
        out(`enum class ${enumName}: int {`);
        genEnum(resources.music);
        out(`};`);
        nl();
        out(`constexpr bool operator !(${enumName} e) noexcept { return e == static_cast<${enumName}>(0); }`);
        nl();
    }

    if(resources.sfx){
        let enumName = 'SoundsRes';
        out(`enum class ${enumName}: int {`);
        genEnum(resources.sfx);
        out(`};`);
        nl();
        out(`constexpr bool operator !(${enumName} e) noexcept { return e == static_cast<${enumName}>(0); }`);
        nl();
    }

    if(resources.music){
        out(`inline void loadMusic() {`);
        loadMusic(resources.music);
        out(`}`);
        nl();
    }

    if(resources.sfx){
        out(`inline void loadSfx() {`);
        loadSfx(resources.sfx);
        out(`}`);
        nl();
    }
    out(`}  // namespace ${resources.namespace}`);

    fs.writeFileSync(outputFile, source);
}

let source = '';
let indentLevel = 0;
let indentSpaces = '';

function out(s) {
    source += `${indentSpaces}${s}\n`;
}

function nl(count) {
    if (count === undefined) count = 1;
    for (let i = 0; i < count; ++i) source += '\n';
}

function indent(count) {
    if (count === undefined) count = 1;
    indentLevel += count;
    updateIndent();
}

function unindent(count) {
    if (count === undefined) count = 1;
    indentLevel -= count;
    updateIndent();
}

function updateIndent() {
    indentSpaces = '';
    for (let i = 0; i < indentLevel; ++i) {
        indentSpaces += '    ';
    }
}

function genEnum(object) {
    indent();
    out('MInvalid = 0,');
    out('MBegin = 1,');
    out('MFirst = MBegin,');
    nl();

    let first = true;
    let last = 'MFirst';
    for (let el of object) {
        if (!el.id)
            console.error("Id is missing for", object);

        let value = el.id;
        if (first) {
            out(`${value} = MFirst,`);
            first = false;
        } else {
            out(`${value},`);
        }

        if (el.alias) {
            out(`${el.alias} = ${value},`);
        }
        last = value;
    }
    nl();
    out(`MLast = ${last},`);
    out(`MEnd,`);
    //out('Count_,');
    unindent();
}

function loadMusic(object) {
    indent();
    out(`using namespace mangl;`);
    out(`using enum MusicRes;`);
    nl();
    // out(`auto res = Resources::instance();`);
    // nl();
    for (let element of object) {
        let id = undefined;
        let file = undefined;

        for (let [key, value] of Object.entries(element)) {
            switch (key) {
            case 'id': id = value; break;
            case 'file': file = value; break;
            case 'alias': break;
            default: throw Error(`Invalid music key: ${key}:${value}`);
            }
        }

        if (!id) throw Error('Music id is missing');
        if (!file) throw Error('Music file is missing');

        out(`Music::load(${id}, Res::makeBundleMusicPath("${file}"));`);

        // if ('id' === key) {
        //     id = value;
        // } else if (value) {
        //     //out(`Music::load(MusicRes::${id}, Resources::makeBundlePathExt("${value}", res->musicExt));`);
        // }

    }
    unindent();
}

function loadSfx(object) {
    indent();
    out(`using namespace mangl;`);
    out(`using enum SoundsRes;`);
    nl();
    // out(`auto res = Resources::instance();`);
    // nl();
    for (let element of object) {
        let id = undefined;
        let file = undefined;

        for (let [key, value] of Object.entries(element)) {
            switch (key) {
            case 'id': id = value; break;
            case 'file': file = value; break;
            default: throw Error(`Invalid music key: ${key}:${value}`);
            }
        }

        if (!id) throw Error('Music id is missing');
        if (!file) throw Error('Music file is missing');

        out(`Sfx::load(${id}, Res::makeBundleSoundPath("${file}"));`);
    }
    unindent();
}
