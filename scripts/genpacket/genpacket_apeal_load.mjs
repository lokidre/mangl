// @ts-check

import { clearSource, getSource, out, nl, indent, unindent, beginBlock, endBlock } from './genpacket_out.mjs';
import { updateGeneratedObject} from './genpack_generated_objects.mjs';
import { getPropElementType, isPropPod, isPropTypePod } from './genpacket_inc.mjs';


export function generateApealPacketLoad(obj, setup) {
    clearSource();

    indent();
    nl();

    beginBlock(`for (auto& objNode: node)`);

    for(let property of obj.properties) {
        beginBlock(`if ("${property.name}" == objNode.key)`);
        unpackProperty(property, obj, setup);
        out(`continue;`);
        endBlock();
        nl();
    }
    
    endBlock(`for nodes`);

    nl();
    unindent();

    updateGeneratedObject(obj.name, {unpackSource: getSource()});
}


function unpackProperty(prop, obj, setup) {
    let objName = obj.name;

    let propName = prop.name;
    let propRef = `obj->${propName}`;
    let propAddr = `&obj->${propName}`;

    if (isPropPod(prop) || 'string' === prop.type) {
        out(`objNode.get(${propAddr});`);
        return;
    }

    if ('enum' === prop.type) {
        let underlying = getPropElementType(prop) ?? 'int';

        // if ('int' === underlying) {
        //     out(`${propRef} = static_cast<${prop.object}>(objNode.getInt());`);
        //     return;
        // }

        // if ('char' === underlying) {
        //     out(`${propRef} = static_cast<${prop.object}>(objNode.getChar());`);
        //     return;
        // }

        //beginBlock();
        out(`${underlying} val;`);
        out(`objNode.get(&val);`);
        out(`${propRef} = static_cast<${prop.object}>(val);`);
        //endBlock();
        return;
    }

    if ('object' === prop.type) {
        out(`unpackObject(objNode, ${propAddr});`);
        return;
    }

    if ('vector' === prop.type || 'array' === prop.type) {
        out(`auto size = ssize(objNode.nodes);`);

        if ('array' != prop.type) {
            out(`${propRef}.resize(size);`);
        } else {
            out(`auto arraySize = ssize(${propRef});`);
            out(`if (arraySize < size) size = arraySize;`);
        }

        beginBlock(`for (Index i = 0; i < size; ++i)`);

        let valueType = getPropElementType(prop);

        if (isPropTypePod(valueType) || 'string' === valueType || 'enum' === valueType) {
            out(`objNode.nodes[i].get(${propAddr}[i]);`);
        } else {
            out(`unpackObject(objNode.nodes[i], ${propAddr}[i]);`);
        }

        endBlock();
        return;
    }

    if ('set' === prop.type) {
        out(`auto size = ssize(objNode.nodes);`);
        beginBlock(`for (Index i = 0; i < size; ++i)`);
        out(`decltype(${propRef})::value_type val{};`);

        let valueType = getPropElementType(prop);
        if (isPropTypePod(valueType) || 'string' === valueType || 'enum' === valueType) {
            out(`objNode.nodes[i].get(&val);`);
        } else {
            out(`unpackObject(objNode.nodes[i], &val);`);
        }

        out(`${propRef}.emplace(std::move(val));`)

        endBlock();
        return;
    }

    if ('map' === prop.type) {
        beginBlock(`for(auto& mapNode: objNode)`);

        out(`decltype(${propRef})::key_type key{};`);
        out(`decltype(${propRef})::value_type::second_type val{};`);
        nl();

        beginBlock(`for(auto& valNode: mapNode)`);

        out(`if ("key" == valNode.key) valNode.get(&key);`)
        
        beginBlock(`if ("val" == valNode.key)`);

        let valueType = getPropElementType(prop);

        if (isPropTypePod(valueType) || 'string' === valueType || 'enum' === valueType) {
            out(`valNode.get(&val);`);
        } else {
            out(`unpackObject(valNode, &val);`);
        }
        endBlock();

        endBlock();

        out(`${propRef}.emplace(key, std::move(val));`)

        endBlock(`for nodes`);
        return;
    }

    throw new Error(`Invalid type of property: ${objName}.${propName}.${prop.type}`);
}

