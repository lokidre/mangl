// @ts-check

import { clearSource, getSource, out, out_ind, nl, indent, unindent, beginBlock, endBlock } from './genpacket_out.mjs';

import { updateGeneratedObject } from './genpack_generated_objects.mjs';
import { getDefaultValue, getPropElementType, isPropContainer, isPropPod } from './genpacket_inc.mjs';


export function packObject(obj)
{
    clearSource();
    indent();

    for (let property of obj.properties) {
        packPacketProperty(property, obj);
    }

    unindent();

    updateGeneratedObject(obj.name, {packSource: getSource()});
}


function packPacketProperty(prop, obj) {

    let objName = obj.name;

    let propName = prop.name;
    if (!propName) throw new Error(`property.name field is missing: ${objName}`);

    let propId = prop.id;
    if (!propId) throw new Error(`id field is missing: ${objName}.${propName}`);

    let propRef = `obj.${propName}`;

    let valueType = getPropElementType(prop); //prop.elemType ?? prop.valueType ?? prop.underlying;
    let defaultValue = getDefaultValue(prop);
    let propType = prop.type;


    if (isPropPod(prop)) {
        //out(`if (${propRef} != ${defaultValue ?? '0'})`);
        if (defaultValue)
            out(`if (!Packet::isPropDefault(${propRef}, ${defaultValue}))`);
        else
            out(`if (!Packet::isPropEmpty(${propRef}))`);
        out_ind(`packer->pack(${propId}, ${propRef});`);
        return;
    }

    if ("enum" === propType) {
        let underlying = valueType ?? 'int';
        out(`if (static_cast<${underlying}>(${propRef}) != ${defaultValue ?? '0'})`);
        out_ind(`packer->pack(${propId}, static_cast<${underlying}>(${propRef}));`);
        return;
    }


    if ('string' === propType) {
        if (defaultValue)
            out(`if (${propRef} != "${defaultValue}")`);
        else
            out(`if (!${propRef}.empty())`);
        out_ind(`packer->pack(${propId}, ${propRef});`);
        return;
    }


    if ('object' === propType) {
        beginBlock(`if (!isEmptyObject(${propRef}))`);
        out(`packer->beginObject(${propId});`);
        out(`packObject(${propRef}, packer);`);
        out(`packer->endObject(${propId});`);
        endBlock();
        nl();
        return;
    }

    if (isPropContainer(prop)) {
        beginBlock(`if (!${propRef}.empty())`);

        if (undefined == valueType) {
            if ('map' === prop.type) {
                out(`packer->packMap(${propId}, ${propRef});`);
            } else {
                out(`packer->packArray(${propId}, ${propRef});`);
            }
        } else {
            out(`Index size = std::ssize(${propRef});`);
            
            if ('map' === prop.type) {
                out(`packer->packMapBegin(${propId}, size);`);

                beginBlock(`for (auto& [key, val]: ${propRef})`);
                out(`packer->packKey(key);`);
                out(`packer->beginElement();`);
                out(`packObject(val, packer);`);
                out(`packer->endElement();`);
                endBlock();
            } else {
                out(`packer->packArrayBegin<Index>(${propId}, size);`)

                beginBlock(`for (auto& val: ${propRef})`);
                out(`packer->beginElement();`);
                out(`packObject(val, packer);`);
                out(`packer->endElement();`);
                endBlock();
            }
        }

        endBlock();

        return;
    }

    //console.log(prop);

    // if ('map' === propType) {
    //     beginBlock(`if (!${propRef}.empty())`);

    //     if(undefined == valueType){
    //         out(`packer->packMap(${propId}, ${propRef});`);
    //     } else {
    //         out(`Index size = std::ssize(${propRef});`);
    //         out(`packer->packMapBegin(${propId}, size);`);

    //         beginBlock(`for (auto& [key, val]: ${propRef})`);
    //         out(`packer->packKey(key);`);
    //         out(`packer->beginElement();`);
    //         out(`packObject(val, packer);`);
    //         out(`packer->endElement();`);
    //         endBlock();
    //     }  

    //     endBlock();
    //     return;
    // }

    throw new Error(`Invalid property.type: ${objName}.${propName}.${prop.type}`);
}


