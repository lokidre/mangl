// @ts-check

import fs from 'fs';

import { clearSource, getSource, out, nl, appendSource, outn } from './genpacket_out.mjs';

import {createUtilHeader} from './genpacket_util.mjs';
import {createHeaderFilePacket} from './genpacket_packet.mjs';
import {generateApealPackerSources} from './genpacket_apeal.mjs';
import { clearGeneratedObjects} from './genpack_generated_objects.mjs';
import { preprocessPacketDescription } from './genpacket_desc.mjs';


export async function generate(params) {

    let yamlParserModule;

    if (params.yamlParser) {
        yamlParserModule = params.yamlParser;
    } else {
        let apealJsDir = params.apealJsDir ?? `${params.config.apeal_scripts_dir}/js`;
        yamlParserModule = `${apealJsDir}/yaml.mjs`;
    }

    params.generatePackString = true;
    
    if (params.generatePackFile)
        params.generatePackString = true;

    let descFile = params.desc ?? params.yamlFile;


    const YAML = await import(yamlParserModule);
    const doc = YAML.parse(fs.readFileSync(descFile, 'utf8'));

    params.packetDescObject = doc.packet;
    let packet = params.packetDescObject;


    // 
    // Preprocess process packet description
    preprocessPacketDescription(params);


    // Cleanup
    clearGeneratedObjects();

    let format = params.format ?? params.fileType;
    if (format === undefined) format = 'packet';
    params.fileType = format;


    let sourceContents;
    let requiredIncludes = [];


    let mainObject = packet.root;
    let mainName = mainObject.name;

    params.implNamespace = `impl_${mainName}_${params.fileType}`;


    switch(format){
    // case 'util':
    //     sourceContents = createUtilHeader(params);
    //     break;
    case 'packet':
        requiredIncludes.push('<apeal/data_param.hpp>');
        requiredIncludes.push('<apeal/index.hpp>');
        requiredIncludes.push('<mangl/packet.h>');
        sourceContents = createHeaderFilePacket(params);
        break;

    case "yaml":
    case "json":
        requiredIncludes.push('<apeal/data_param.hpp>');
        requiredIncludes.push('<apeal/index.hpp>');
        requiredIncludes.push(`<apeal/parse/${format}.hpp>`);
        
        requiredIncludes.push('<mangl/packet.h>');

        requiredIncludes.push('<string>');
        requiredIncludes.push('<string_view>');

        sourceContents = generateApealPackerSources(params);
        break;

    default:
        throw new Error(`Wrong packet format type: ${params.fileType}`);
    }

    if (params.generatePackFile) {
        requiredIncludes.push(`<apeal/file.hpp>`);
    }




    let genDate = new Date();
    let dateString = genDate.toISOString().split('T')[0];

    clearSource();

    out(` /*** Automatically Generated *** DO NOT EDIT ***`);
    out(` * Generated Date: ${dateString}`);
    out(` */`);
    out('#pragma once');
    nl();
    

    if (packet.header) {
        for (let item of packet.header) {
            if (item.include) {
                let inc = item.include;
                if (inc[0] !== '<' && inc[0] !== '\"')
                    inc = `\"${inc}\"`;
                out(`#include ${inc}`);
            }
        }
        nl();
    }

    for (let file of requiredIncludes) {
        out(`#include ${file}`);
    }
    nl();


    if (packet.root?.namespace) {
        outn(`namespace ${packet.root.namespace} {`);
    }

    appendSource(sourceContents);

    if (packet.root?.namespace) {
        outn(`}  // namespace ${packet.root.namespace}`);
    }


    let outputFile = params.output ?? params.outputFile;
    console.log(`Writing:`, outputFile);
    fs.writeFileSync(outputFile, getSource());

}

