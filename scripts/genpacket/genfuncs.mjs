// @ts-check

import { clearSource, getSource, out, nl, indent, unindent, appendSource } from './genpacket_out.mjs';
import {getGeneratedObjects, updateGeneratedObject} from './genpack_generated_objects.mjs';


function buildYaml(obj){
    clearSource();
    indent();

    nl();

    out(`if (isEmptyYaml(name)) {`)
    out(`    yaml->map();`)
    out(`} else {`);
    out(`    yaml->obj(name);`);
    out(`}`);

    for (let property of obj.properties) {
        if(undefined === property.type){
            if(property.object !== undefined ){
                property.type = "object";
            }
            else{
                property.type = "pod";
            }
        }
        if(property.packetType){
            if(typeof property.sourceType === "object"){
                let arr = Object.keys(property.sourceType)
                out(`yaml->flowSeq("${property.name}"`)
                for(let elem of arr){
                    out(`,obj.${property.name}.${elem}`)
                }
                out(`);`)
            }else{
                //out(`yaml->field("${property.name}",((${property.packetType})obj.${property.name}));`)
                if(property.defaultSkip){
                    if(property.sourceType){
                        out(`if(obj.${property.name} != ${property.defaultValue}){`)
                        out(`    yaml->field("${property.name}",(${property.packetType})obj.${property.name});`)
                        out(`}`)
                    }else{
                        out(`if(obj.${property.name} != ${property.defaultValue}){`)
                        out(`    yaml->field("${property.name}",obj.${property.name});`)
                        out(`}`)
                    }
                //continue;
                }
                else{
                    out(`yaml->field("${property.name}",((${property.packetType})obj.${property.name}));`)
                }
            }
        }else{
        switch(property.type){
            case "pod": {
                let defaultValue = property.defaultValue ?? '0';
                out(`if (obj.${property.name} != ${defaultValue})`);
                out(`    yaml->field("${property.name}", obj.${property.name});`);
                break;
            }

            case "string": {
                let defaultValue = property.defaultValue ?? "";
                out(`if (obj.${property.name} != \"${defaultValue}\")`);
                out(`    yaml->field("${property.name}", obj.${property.name});`);
                break;
            }

            case "array":
            case "vector":
                    {
                if(undefined === property.elemType){
                    out(`yaml->flowSeq("${property.name}", obj.${property.name});`);
                }  else {
                        out(`yaml->object("${property.name}");`)
                        out(`for(auto& val: obj.${property.name}){`)
                        //out(`    yaml->map();`)
                        out(`    packObject(val, {}, yaml);`);

                        // out(`    for(auto& elem: val){`)
                        // out(`        packObject(elem, {}, yaml);`)
                        // out(`    }`)
                        //out(`    yaml->pop();`)
                        out(`}`)
                        out(`yaml->pop();`)
                }
                
                // else if("vector" === property.elemType){
                //     if(property.subElemType){
                //         out(`yaml->object("${property.name}");`)
                //         out(`for(auto& val:obj.${property.name}){`)
                //         out(`     yaml->map();`)
                //         out(`    for(auto& elem: val){`)
                //         out(`        packObject(elem, {}, yaml);`)
                //         out(`    }`)
                //         out(`    yaml->pop();`)
                //         out(`}`)
                //         out(`yaml->pop();`)
                //     }
                // }
                // else{
                //     out(`yaml->object("${property.name}");`)
                //     out(`for(auto& val:obj.${property.name}){`)
                //     out(`    packObject(val, {}, yaml);`)
                //     out(`}`)
                //     out(`yaml->pop();`)
                // } 
                break;   
            }
            case "map":{
                out(`yaml->object("${property.name}");`)
                if(property.elemType){
                    out(`for (auto& [key, value] : obj.${property.name}) {`)
                    //out(`    packObject(value, std::to_string(key), yaml);`)
                    out(`    packObject(value, key, yaml);`)
                    out(`}`)
                }
                else{
                    //out(`yaml->map();`)
                    out(`for (auto& [key, value]: obj.${property.name}) {`)
                    out(`    yaml->field(key, value);`)
                    //out(`    yaml->fieldIntegral(key, value);`)
                    out(`}`)
                    //out(`yaml->pop();`)
                } 
                out(`yaml->pop();`) 
                break;
            }
            case "object":{
                out(`packObject(obj.${property.name}, "${property.name}", yaml);`)
                break;    
            }
            default:{
                throw new Error('Invalid type of property: ' + property.type);  
            }
        }
    }
    }
    out(`yaml->pop();`)
    nl();

    unindent();

    updateGeneratedObject(obj.name, {packSource: getSource()});

    // let genObject = generatedObjects.get(obj.name);

    // if (!genObject) {
    //     genObject = {}
    // }

    // genObject.packSource = source;

    // generatedObjects.set(obj.name, genObject);

}

function loadObject(object){
    clearSource();
    indent();
    nl();

    out(`for (auto& objNode: node){`);

    for(let property of object.properties) {
        const propertyType = property.type ?? 'pod';
        switch (propertyType) {
            case 'pod':
            case 'string':
                    out(`    if ("${property.name}" == objNode.key) {`)
                    out(`        objNode.get(&(obj->${property.name}));`)
                    out(`        continue;`)
                    out(`    }`)
                    break;
    
    
            case 'object': {
                out(`    if("${property.name}" == objNode.key){`)
                out(`        unpackObject(objNode, &obj->${property.name});`)
                out(`        continue;`)
                out(`    }`)
                break;
            }
    
            case 'vector': 
            case 'array':
            {
                out(`    if("${property.name}" == objNode.key){`);

                if(property.elemType){
                    if( "vector" === property.type){
                        out(`        obj->${property.name}.resize(objNode.nodes.size());`)
                    }

                    out(`        for (int i = 0; i < obj->${property.name}.size(); i++) {`)
                    out(`             unpackObject(objNode.nodes[i], &obj->${property.name}[i]);`)
                    out(`        }`)
                }
                else{
                    out(`        for(auto& arrNode: objNode){`)
                    out(`            arrNode.get(&obj->${property.name}.emplace_back());`)
                    out(`        }`)
                }
                out(`        continue;`)

                out(`    }`)
                break;
            }
    
            case 'map': {
                out(`    if("${property.name}" == objNode.key){`)
                out(`        for(auto& mapNode: objNode){`)
                //out(`            for(auto& mapEl: mapNode){`)
                out(`           decltype(obj->${property.name})::key_type key;`)
                out(`           decltype(obj->${property.name})::value_type::second_type value;`)
                out(`           mapNode.getKey(&key);`)
                out(`           //key = apl::parseInt(mapNode.key);`)
                if(property.elemType){
                out(`           unpackObject(mapNode, &value);`)
                }
                else{
                out(`           mapNode.get(&value);`)
                }
                out(`           obj->${property.name}.emplace(key, std::move(value));`)
                //out(`            }`)
                out(`        }`)
                out(`        continue;`)
                out(`    }`)
                break;
            }
            default:
                throw new Error('Invalid type of property: ', propertyType);
        
        }

    }
    
    out(`}`)
    nl();
    unindent();

    updateGeneratedObject(object.name, {unpackSource: getSource()});

    // let genObject = generatedObjects.get(object.name);

    // if (!genObject) {
    //     genObject = {}
    // }

    // genObject.unpackSource = source;

    // generatedObjects.set(object.name, genObject);
}

    function buildJson(obj){
        clearSource();
        indent();
        nl();

        out(`if (isEmptyJson(name)) {`)
        out(`    builder->object();`)
        out(`} else {`);
        out(`    builder->object(name);`);
        out(`}`);
        for (let property of obj.properties) {
            if(undefined === property.type){
                if(property.object !== undefined ){
                    property.type = "object";
                }
                else{
                    property.type = "pod";
                }
            }
            if(property.packetType){
                if(typeof property.sourceType === "object"){
                    let arr = Object.keys(property.sourceType)
                    out(`builder->flowSeq("${property.name}"`)
                    for(let elem of arr){
                        out(`,obj.${property.name}.${elem}`)
                    }
                    out(`);`)
                }else{
                    //out(`yaml->field("${property.name}",((${property.packetType})obj.${property.name}));`)
                    if(property.defaultSkip){
                        if(property.sourceType){
                            out(`if(obj.${property.name} != ${property.defaultValue}){`)
                            out(`    builder->field("${property.name}",(${property.packetType})obj.${property.name});`)
                            out(`}`)
                        }else{
                            out(`if(obj.${property.name} != ${property.defaultValue}){`)
                            out(`    builder->field("${property.name}",obj.${property.name});`)
                            out(`}`)
                        }
                    //continue;
                    }
                    else{
                        out(`builder->field("${property.name}",((${property.packetType})obj.${property.name}));`)
                    }
                }
            }else{
            switch(property.type){
                case "pod":{
                    let defaultValue = property.defaultValue ?? '0';
                    out(`    if (obj.${property.name} != ${defaultValue})`);
                    out(`        builder->field("${property.name}", obj.${property.name});`);
                    break;
                }

                case "string":{
                    let defaultValue = property.defaultValue ?? '';
                    out(`    if (obj.${property.name} != \"${defaultValue}\")`);
                    out(`        builder->field("${property.name}", obj.${property.name});`);
                    break;
                }

                case "array":
                case "vector":
                    {
                    if(undefined === property.elemType){
                        out(`builder->flowSeq("${property.name}",obj.${property.name});`)    
                    }
                    else{
                        out(`builder->array("${property.name}");`)
                        out(`for(auto& val:obj.${property.name}){`)
                        out(`    packObject(val, {}, builder);`)
                        out(`}`)
                        out(`builder->pop();`)
                    } 
                    break;   
                }
                case "map":{
                    out(`builder->object("${property.name}");`)
                    if(property.elemType){
                        out(`for (auto& [key, value] : obj.${property.name}) {`)
                        //out(`    packObject(value, std::to_string(key), yaml);`)
                        out(`    packObject(value, key, builder);`)
                        out(`}`)
                    }
                    else{
                        //out(`yaml->map();`)
                        out(`for (const auto& [key,value]: obj.${property.name}) {`)
                        out(`    builder->field(key, value);`)
                        //out(`    builder->fieldIntegral(key, value);`)
                        out(`}`)
                        //out(`yaml->pop();`)
                    } 
                    out(`builder->pop();`) 
                    break;
                }
                case "object":{
                    out(`packObject(obj.${property.name}, "${property.name}", builder);`)
                    break;    
                }
                default:{
                    throw new Error('Invalid type of property: ', property.type);  
                }
            }
        }
        }
        out(`builder->pop();`)
        nl();
    
        unindent();

        updateGeneratedObject(obj.name, {packSource: getSource()});

        // let genObject = generatedObjects.get(obj.name);
    
        // if (!genObject) {
        //     genObject = {}
        // }
    
        // genObject.packSource = source;
    
        // generatedObjects.set(obj.name, genObject);
    
    }



function loadJsonObject(object){
    clearSource();
    indent();
    nl();

    out(`for (const auto& objNode: node.nodes) {`);

    for(let property of object.properties) {
        const propertyType = property.type ?? 'pod';
        switch (propertyType) {
        //if((property.array === undefined) && (property.map === undefined) && (property.object === undefined)){
        //     if(property.sourceType){
        //         if(typeof property.sourceType === "object"){
        //             let arr = Object.keys(property.sourceType)
        //             out(`    if("${property.name}" == objNode.key){`)
        //             let i =0;
        //             for ( let elem of arr ){
        //                 out(`        objNode.nodes[${i}].get(&obj->${property.name}.${elem});`)
        //                 i++;
        //             }
        //             out(`        continue;`)
        //             out(`    }`)
        //         }
        //         else{
        //             out(`    if("${property.name}" == objNode.key){`)
        //             out(`        ${property.packetType} value;`)
        //             out(`        objNode.get(&value);`)
        //             out(`        obj->${property.name} = (${property.sourceType})value;`)
        //             out(`        continue;`)
        //             out(`    }`)
        //         }
        //     }else{
        //         out(`    if ("${property.name}" == objNode.key) {`)
        //         out(`        objNode.get(&(obj->${property.name}));`)
        //         out(`        continue;`)
        //         out(`    }`)
        //     }

        //     continue;
        // }

        case 'pod':
        case 'string':
                out(`    if ("${property.name}" == objNode.key) {`)
                out(`        objNode.get(&(obj->${property.name}));`)
                out(`        continue;`)
                out(`    }`)
                break;


        case 'object': {
            out(`    if("${property.name}" == objNode.key){`)
            out(`        unpackObject(objNode, &obj->${property.name});`)
            out(`        continue;`)
            out(`    }`)
            break;
        }

        case 'vector': 
        case 'array':
        {
            out(`    if("${property.name}" == objNode.key){`)           
            if(property.elemType){
                if( "vector" === property.type){
                    out(`        obj->${property.name}.resize(objNode.nodes.size());`)
                }
                out(`        for (apl::Index i = 0; i < std::ssize(obj->${property.name}); ++i) {`)
                out(`             unpackObject(objNode.nodes[i], &obj->${property.name}[i]);`)
                out(`        }`)
            }
            else{
                out(`        for(auto& arrNode: objNode){`)
                out(`            arrNode.get(&obj->${property.name}.emplace_back());`)
                out(`        }`)
            }
            out(`        continue;`)
            out(`    }`)
            break;
        }

        case 'map': {
            out(`    if("${property.name}" == objNode.key){`)
            out(`        for(auto& mapNode: objNode){`)
            //out(`            for(auto& mapEl: mapNode){`)
            out(`           decltype(obj->${property.name})::key_type key;`)
            out(`           decltype(obj->${property.name})::value_type::second_type value;`)
            out(`           mapNode.getKey(&key);`)
            out(`           //key = apl::parseInt(mapNode.key);`)
            if(property.elemType){
            out(`           unpackObject(mapNode, &value);`)
            }
            else{
            out(`           mapNode.get(&value);`)
            }
            out(`           obj->${property.name}.emplace(key, std::move(value));`)
            //out(`            }`)
            out(`        }`)
            out(`        continue;`)
            out(`    }`)
            break;
        }
        default:
            throw new Error('Invalid type of property: ', propertyType);
        }
    }

    
    out(`}`)


    nl();
    unindent();

    updateGeneratedObject(object.name, {unpackSource: getSource()});

    // let genObject = generatedObjects.get(object.name);

    // if (!genObject) {
    //     genObject = {}
    // }

    // genObject.unpackSource = source;

    // generatedObjects.set(object.name, genObject);
}

// function buildJson(object){
//     source = '';
//     indent();
//     nl();
//     out(`builder->object("${object.name}");`)
//     for (let property of object.properties) {
//         switch(property.type){
//             case "array":{
//                 out(`builder->array("${property.name}");`)
//                 out(`for (auto it : obj.${property.name}) {`)
//                 out(`    builder->arrayValue(it);`)
//                 out(`}`)
//                 out(`builder->close();`)
//                 break;
//             }
//             case "map":{
//                 out(`builder->array("${property.name}");`)
//                 out(`for (auto& [key, value] : obj.badges) {`)
//                 out(`    builder->value(key,value);`)
//                 out(`}`)
//                 out(`builder->close();`)
//                 break;
//             }
//             case "object":{
//                 out(`packObject(obj.${property.name},builder);`)
//                 break;
//             }
//             default:{
//                 out(`builder->value("${property.name}",obj.${property.name});`)
//                 break;    
//             }
//         }
//     }
//     out(`builder->close();`)
//     nl();
//     unindent();
//     let genObject = generatedObjects.get(object.name);

//     if (!genObject) {
//         genObject = {}
//     }

//     genObject.packSource = source;

//     generatedObjects.set(object.name, genObject);
// }

export function createHeaderFile(params){

    let outputFile = params.outputFile;
    let fileType = params.fileType;

    let stringViewType = 'std::string_view';
    let stringType = 'std::string';

    let packet = params.packetDescObject;


    for (let object of packet.objects) {
        if ("yaml" === fileType) {
            buildYaml(object);
            loadObject(object);
        } else if ("json" === fileType) {
            buildJson(object);
            loadJsonObject(object);
        }
    }

    for (let file of packet.header) {
        out(`#include "${file.include}"`);
    }

    nl();


    let mainObject = packet.root;
    let generatedObjects = getGeneratedObjects();

    if (!mainObject) {
        for (let [objectName, objectGen] of generatedObjects) {
            mainObject = {
                name: objectName,
                object: objectName,
            }
            break;
        }
    }



    let implNamespace = `impl_${mainObject.name}_${fileType}`;


    if("json"===fileType){
        // out(`#include <apeal/parse/json.hpp>`)
        // out(`#include <string>`)
        // out(`#include <string_view>`)

        // nl();

        // if (packet.root?.namespace) {
        //     out(`namespace ${packet.root.namespace} {`);
        // }

        nl();

        out(`inline void pack(const ${mainObject.object}& obj, apl::JsonBuilder* yaml);`);
        out(`inline void unpack(const apl::Json& yaml, ${mainObject.object}* obj);`)

        nl(2);


        out(`namespace ${implNamespace} {`);
        nl(2);

        out(`template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>`)
        out(`bool isEmptyJson(T name ) {`)
        out(`    return false;`)
        out(`}`)
        nl();
        out(`inline bool isEmptyJson(${stringViewType} name) {`)
        out(`    return name.empty();`)
        out(`}`)
    }
    if("yaml"===fileType){
        // out(`#include <apeal/parse/yaml.hpp>`)
        // out(`#include <string>`)
        // out(`#include <string_view>`)

        // if (packet.root?.namespace) {
        //     out(`namespace ${packet.root.namespace} {`);
        // }

        nl();

        out(`inline void pack(const ${mainObject.object}& obj, apl::YamlBuilder* yaml);`);
        out(`inline void unpack(const apl::Yaml& yaml, ${mainObject.object}* obj);`)


        nl(2);

        out(`namespace ${implNamespace} {`);
        nl(2);


        out(`template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>`)
        out(`bool isEmptyYaml(T name) {`)
        out(`    return false;`)
        out(`}`)
        nl();
        out(`inline bool isEmptyYaml(${stringViewType} name) {`)
        out(`    return name.empty();`)
        out(`}`)

    }
    //out(`using namespace apl;`)
    for (let [objectName, objectGen] of generatedObjects) {
        if("json"===fileType){
            //out(`inline void packObject(const ${objectName}& obj, apl::JsonBuilder* builder);`);
            out(`inline void packObject(const ${objectName}& obj, ${stringViewType} name, apl::JsonBuilder* builder);`);
            out(`inline void packObject(const ${objectName}& obj, std::integral auto name, apl::JsonBuilder* builder);`);
            out(`inline void unpackObject(const apl::Json& node, ${objectName}* obj);`);
        }
        else if("yaml"===fileType){
            out(`inline void packObject(const ${objectName}& obj, ${stringViewType} name, apl::YamlBuilder* yaml);`);
            out(`inline void packObject(const ${objectName}& obj, std::integral auto name, apl::YamlBuilder* yaml);`);
            out(`inline void unpackObject(const apl::YamlNode& node, ${objectName}* obj);`);
        }
    }

    nl(2);



    // for(let [objectName, objectGen] of generatedObjects){
    //     if("json"===fileType){
    //         out(`inline void pack(const ${objectName}& obj, apl::JsonBuilder *builder){`)   
    //         out(`    builder->begin();`);
    //         out(`    packObject(obj,builder);`)
    //         out(`    builder->end();`)
    //         out(`}`)
    //     }
    //     else if("yaml"===fileType){
    //         out(`inline void pack(const ${objectName}& obj, apl::YamlBuilder *yaml){`)
    //         out(`    packObject(obj,yaml);`)
    //         out(`}`)
    //     }
    // }

    let cppBuilder = fileType === 'json' ? 'apl::JsonBuilder' : 'apl::YamlBuilder';
    let cppNode = fileType === 'json' ? 'apl::Json' : 'apl::YamlNode';

    for (let [objectName, objectGen] of generatedObjects) {

        out(`inline void packObject(const ${objectName}& obj, ${stringViewType} name, ${cppBuilder}* builder) {`);
        appendSource(objectGen.packSource);
        out('}');

        out(`inline void packObject(const ${objectName}& obj, std::integral auto name, ${cppBuilder}* builder) {`);
        appendSource(objectGen.packSource);
        out('}');

        out(`inline void unpackObject(const ${cppNode}Json& node, ${objectName}* obj){`)
        appendSource(objectGen.unpackSource);
        out(`}`);



        // if( "json" === fileType){
        //     out(`inline void packObject(const ${objectName}& obj, ${stringViewType} name, apl::JsonBuilder* builder) {`);
        //     source += objectGen.packSource;
        //     out('}');

        //     out(`inline void packObject(const ${objectName}& obj, std::integral auto name, apl::JsonBuilder* builder) {`);
        //     source += objectGen.packSource;
        //     out('}');

        //     out(`inline void unpackObject(const apl::Json& node, ${objectName}* obj){`)
        //     source += objectGen.unpackSource;
        //     out(`}`)


        // }
        // else if("yaml"==fileType){
        //     out(`inline void packObject(const ${objectName}& obj, ${stringViewType} name, apl::YamlBuilder* yaml) {`);
        //     source += objectGen.packSource;
        //     out('}');

        //     out(`inline void packObject(const ${objectName}& obj, std::integral auto name, apl::YamlBuilder* yaml) {`);
        //     source += objectGen.packSource;
        //     out('}');

        //     out(`inline void unpackObject(const apl::YamlNode& node, ${objectName}* obj){`)
        //     source += objectGen.unpackSource;
        //     out(`}`)

        // }
    }
    
    nl();

    if ( "yaml" === fileType ){

        out(`}  // namespace ${implNamespace}`);
        nl(2);


        out(`inline ${stringType} packString(const ${mainObject.object}& obj){`)
        out(`    apl::YamlBuilder builder;`)
        out(`    pack(obj, &builder);`)
        out(`    return builder.str();`)
        out(`}`)
        nl();
        out(`inline void packFile(const ${mainObject.object}& obj, ${stringViewType} fileName){`)
        out(`    auto str = packString(obj);`);
        out(`    apl::saveFile(fileName, str);`);
        out(`}`)
        nl();
        out(`inline void unpack(const apl::Yaml& yaml, ${mainObject.object}* obj){`)
        out(`    for (auto& doc: yaml.documents) {`)
        out(`         for (auto& docNode: doc) {`)
        out(`             ${implNamespace}::unpackObject(docNode, obj );`)
        out(`         }`)
        out(`   }`)
        out(`}`)
        nl();
        out(`inline void unpackYaml(${stringViewType} text, ${mainObject.object}* obj){ `)
        out(`    auto yaml = apl::parseYaml(text);`)
        out(`    unpack(yaml, obj);`)
        out(`}`)
        nl();

        // out(`inline void unpackYamlFile(${stringViewType} fileName, ${mainObject.object}* obj){`)
        // out(`    auto yaml = apl::loadYaml(fileName);`)
        // out(`    unpack(yaml,obj);`)
        // out(`}`)
        // nl();
        // out(`inline void unpackYamlFileIfExists(${stringViewType} filename, ${mainObject.object}* obj){`)
        // out(`    if(apl::FileSystem::exists(filename)){`)
        // out(`       unpackFile(filename,obj);`)
        // out(`   }`)
        // out(`}`)

        nl(2);
        
        out(`inline void pack(const ${mainObject.object}& obj, apl::YamlBuilder* builder){`)
        out(`    builder->begin();`)
        out(`    ${implNamespace}::packObject(obj, "${mainObject.name}", builder);`)
        out(`    builder->end();`)
        out(`}`)
        nl();

    } else if ("json" === fileType) {

        out(`}  // namespace ${implNamespace}`);
        nl(2);

       
        out(`inline void unpack(const apl::Json& json, ${mainObject.object}* obj){`);
        out(`    for (auto& docNode: json.nodes) {`)
        out(`        ${implNamespace}::unpackObject(docNode, obj );`)
        out(`    }`)
        out(`}`)
        nl();
        out(`inline void unpackJson(${stringViewType} text, ${mainObject.object}* obj){`)
        out(`    auto json = apl::parseJson(text);`)
        out(`    unpack(json, obj);`)
        out(`}`)
        nl();

        out(`inline void pack(const ${mainObject.object}& obj, apl::JsonBuilder* builder){`)
        out(`    builder->begin();`)
        out(`    ${implNamespace}::packObject(obj, "${mainObject.name}", builder);`)
        out(`    builder->end();`)
        out(`}`)
        nl();

    }


    return getSource();
}

