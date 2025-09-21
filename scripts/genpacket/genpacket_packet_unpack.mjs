// @ts-check

import { clearSource, getSource, out, out_ind, nl, indent, unindent, beginBlock, endBlock } from './genpacket_out.mjs';
import { updateGeneratedObject } from './genpack_generated_objects.mjs';
import { isPropPod, getPropElementType } from './genpacket_inc.mjs';

export function unpackObject(object)
{
    clearSource();
    indent();

    for (let prop of object.properties){
        if(prop.defaultValue){
            out(`bool hasDefault${prop.name} = false;`);
        }
    }

    beginBlock(`while (auto field = loader.getId())`);

    beginBlock(`if (PacketFieldType::ElementBegin == field.type)`);
    out(`loader.unpackObject();`);
    out(`field = loader.getId();`);
    endBlock();

    beginBlock(`if (PacketFieldType::ObjectEnd == field.type || PacketFieldType::ElementEnd == field.type)`);
    out(`loader.unpackObject();`);
    out(`return;`);
    endBlock();


    out(`switch(field.id) {`);

    for (let prop of object.properties) {
        out(`case ${prop.id}:`);
        indent();
        generatePropertyUnpack(prop, object);
        out(`break;`);
        unindent();
    }


    out(`default:`);
    out_ind(`loader.skip(field);`);
    out_ind(`break;`);
    out(`}  // switch (property)`);
    

    endBlock('for all fields');

    for(let property of object.properties){
        if(property.defaultValue){
            beginBlock(`if (!hasDefault${property.name})`);
            if ("string" == property.type) {
                out_ind(`obj->${property.name} = \"${property.defaultValue}\";`);
            } else {
                out_ind(`obj->${property.name} = ${property.defaultValue};`);
            }
            endBlock();
        }
    }

    
    unindent();

    updateGeneratedObject(object.name, {unpackSource: getSource()});
}



function generatePropertyUnpack(prop, obj) {

    let objName = obj.name;

    let propName = prop.name;
    if (!propName) throw new Error(`property.name field is missing: ${objName}`);


    //let valueType = property.elemType ?? property.valueType;
    // let propType = property.type ?? 'pod';
    let propRef = `obj->${propName}`;
    let propAddr = `&obj->${propName}`;

    let valueType = getPropElementType(prop);


    if (isPropPod(prop) || 'string' === prop.type) {
        out(`loader.unpack(${propAddr});`);

        if (prop.defaultValue)
            out(`hasDefault${propName} = true;`);
        return;
    }

    if ('enum' === prop.type) {
        let underlying = valueType ?? 'int';
        // beginBlock();
        // out(`${underlying} val;`);
        // out(`loader.unpack(&val);`);
        // out(`${propRef} = static_cast<${prop.object}>(val);`);
        // endBlock();

        out(`${propRef} = static_cast<${prop.object}>(loader.unpackValue<${underlying}>());`);

        if (prop.defaultValue)
            out(`hasDefault${propName} = true;`);
        return;
    }


    if ('object' === prop.type) {
        out(`loader.unpackObject();`);
        beginBlock(`if (PacketFieldType::ObjectBegin == field.type)`);
        out(`unpackObject(loader, ${propAddr});`);
        endBlock();
        return;
    }

    if ('vector' === prop.type) {
        if (undefined === prop.valueType) {
            out(`loader.unpackArray(&${propRef});`);
        } else {
            beginBlock();
            out(`auto size = loader.arraySize();`)
            out(`${propRef}.resize(size);`);
            beginBlock(`for (auto& val: ${propRef})`)
            out(`unpackObject(loader, &val);`)
            endBlock();
            endBlock();
        }
        return;
    }

    if ('set' === prop.type) {
        if (undefined === prop.valueType) {
            out(`loader.unpackSet(&${propRef});`);
        } else {
            beginBlock();
            out(`auto size = loader.arraySize();`)
            beginBlock(`for (Index i = 0; i < size; ++size)`)
            out(`decltype(${propRef})::value_type val{};`);
            out(`unpackObject(loader, &val);`)
            out(`${propRef}.emplace(std::move(val));`);
            endBlock();
            endBlock();
        }
        return;
    }


    if ('array' === prop.type) {
        if (undefined === valueType) {
            out(`loader.unpackStdArray(${propAddr});`);
        } else {
            beginBlock();

            out(`auto size = loader.arraySize();`);
            out(`auto containerSize = ssize(${propRef});`);

            out(`Index i = 0;`);

            out(`for (; i < containerSize; ++i) {`)
            out_ind(`unpackObject(loader, &${propRef}[i]);`)
            out(`}`);

            beginBlock(`if (i < size)`);
            out(`${valueType} temp;`);
            out(`for (; i < size; ++i) {`)
            out_ind(`unpackObject(loader, &temp);`);
            out(`}`);
            endBlock();

            endBlock();
        }

        return;
    }



    if ('map' === prop.type) {
        if(undefined == valueType) {
            out(`loader.unpackMap(${propAddr});`);
        } else {
            beginBlock();
            out(`auto size = loader.mapSize();`);
            nl()

            out(`decltype(${propRef})::key_type key;`);
            out(`decltype(${propRef})::mapped_type value;`);
            nl();

            beginBlock(`for (int i = 0; i < size; i++)`);
            out(`loader.unpack(&key);`);
            out(`unpackObject(loader, &value);`);
            out(`${propRef}.emplace(key, value);`);
            endBlock();

            endBlock();
        }    
        return;
    }

       
    throw new Error(`Invalid property type: ${objName}.${propName}.${prop.type}`);

}



