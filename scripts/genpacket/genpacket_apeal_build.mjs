// @ts-check

import { clearSource, getSource, out, out_ind, nl, indent, unindent, beginBlock, endBlock } from './genpacket_out.mjs';
import { updateGeneratedObject} from './genpack_generated_objects.mjs';
import { getPropType, getDefaultValue, getPropElementType, isPropPod, isPropContainer } from './genpacket_inc.mjs';


export function generateApealPacketBuild(obj, setup) {
    clearSource();

    indent();

    nl();

    out(`if (isEmpty${setup.cppSuffix}(name)) {`);
    out_ind(`builder->map();`)
    out(`} else {`);
    out_ind(`builder->obj(name);`);
    out(`}`);

    for (let prop of obj.properties) {
        generatePropertyBuild(prop, obj, setup);
    }

    out(`builder->pop();  // ${obj.name}`, 0, 1);

    unindent();

    updateGeneratedObject(obj.name, {packSource: getSource()});
}


function generatePropertyBuild(prop, obj, setup) {

    let objRef = obj.name;

    let propName = prop.name;
    let propRef = `obj.${propName}`;
    let propAddr = `&obj.${propName}`;
    let propType = getPropType(prop);
    let valueType = prop.elemType ?? prop.valueType;


    if (isPropPod(prop)) {
        let defaultValue = prop.defaultValue;
        if (defaultValue)
            out(`if (!Packet::isPropDefault(${propRef}, ${defaultValue}))`);
        else
            out(`if (!Packet::isPropEmpty(${propRef}))`);

        out_ind(`builder->field("${propName}", ${propRef});`);
        return;
    }

    if ('string' === propType) {
        if (prop.defaultValue)
            out(`if (${propRef} != "${getDefaultValue(prop)}")`);
        else
            out(`if (!${propRef}.empty())`);
        out_ind(`builder->field("${propName}", ${propRef});`);
        return;
    }


    if ("enum" === propType) {
        let underlying = prop.underlying ?? 'int';
        out(`if (static_cast<${underlying}>(${propRef}) != ${getDefaultValue(prop) ?? '0'})`);
        out_ind(`builder->field("${propName}", static_cast<${underlying}>(${propRef}));`);
        return;
    }


    if (isPropContainer(prop)) {

        if ('array' === prop.type) {
            beginBlock(`if (!isEmptyArray(${propRef}.data(), ${propRef}.data() + ${propRef}.size()))`);
        } else {
            beginBlock(`if (!${propRef}.empty())`);
        }

        if ('map' === prop.type) {
            out(`builder->array("${propName}");`);

            beginBlock(`for (auto& [key, val]: ${propRef})`);
            out(`builder->map();`);
            out(`builder->field("key", key);`);
                if (valueType) {
                out(`packObject(val, "val", builder);`);
            } else {
                out(`builder->field("val", val);`);
            }
            out(`builder->pop();`);
            endBlock();
            out(`builder->pop();  // ${propName}`);
   
        } else {
            if (undefined === valueType) {
                out(`builder->flowSeqVec("${propName}", ${propRef});`);
            }  else {
                out(`builder->array("${propName}");`)
                beginBlock(`for (auto& val: ${propRef})`);
                out(`packObject(val, {}, builder);`);
                endBlock();
                out(`builder->pop();  // ${propName}`);
                nl();
            }
        }

        endBlock();
        nl();
        return;
    }

    if ("object" === propType) {
        out(`if (!isEmptyObject(${propRef}))`);
        out_ind(`packObject(${propRef}, "${propName}", builder);`);
        return;
    }

    throw new Error(`Invalid type of property: ${propType}`);
}


