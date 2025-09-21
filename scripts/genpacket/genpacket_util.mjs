// @ts-check

import { getSource, out, nl, beginBlock, endBlock } from './genpacket_out.mjs';

import * as inc from './genpacket_inc.mjs';


export function createUtilHeader(params) {

    let packet = params.packetDescObject;

    let mainObject = packet.root;
    let mainName = mainObject.name;

    let objsSet = [];
    let funcSuffix = `_${params.fileType}`;

    for (let obj of packet.objects) {
        let objName = obj.name;
        out(`inline void clearObject(${objName}* obj);`);
        out(`inline bool isEmptyObject(const ${objName}& obj);`);

        objsSet.push(objName);
    }
    nl();


    // Declare all the pending arrays
    let arraysSet = [];

    for (let obj of packet.objects) {
        for (let prop of obj.properties) {
            if (prop.type !== 'array')
                continue;

            let elementType = inc.getPropElementType(prop);
            if (inc.isPropTypePod(elementType))
                continue;

            //let propName = prop.name;
            if (arraysSet.includes(elementType))
                continue;
            arraysSet.push(elementType);


            // out(`template <class ArrayT>`);
            // out(`inline void clearArray_${elementType}(ArrayT* obj);`);

            // out(`template <class ArrayT>`);
            // out(`inline bool isEmptyArray_${elementType}(const ArrayT& obj);`);


//            out(`template <class ArrayT>`);
            out(`inline void clearArray(${elementType}* begin, const ${elementType}* end);`);
            out(`inline bool isEmptyArray(const ${elementType}* begin, const ${elementType}* end);`);
        }
    }

    if (arraysSet.length !== 0) {
        nl();
        arraysSet = [];

        beginBlock(`inline void clearArray(std::integral auto* begin, const std::integral auto* end)`);
        out(`for (auto it = begin; it != end; ++it) *it = 0;`);
        endBlock();

        beginBlock(`inline void clearArray(std::floating_point auto* begin, const std::floating_point auto* end)`);
        out(`for (auto it = begin; it != end; ++it) *it = 0;`);
        endBlock();


        beginBlock(`inline bool isEmptyArray(const std::integral auto* begin, const std::integral auto* end)`);
        out(`for (auto it = begin; it != end; ++it) if (*it != 0) return false;`);
        out(`return true;`);
        endBlock();

        beginBlock(`inline bool isEmptyArray(const std::floating_point auto* begin, const std::floating_point auto* end)`);
        out(`for (auto it = begin; it != end; ++it) if (*it != 0) return false;`);
        out(`return true;`);
        endBlock();

        nl();
    }

    for (let obj of packet.objects) {
        //let objName = obj.name;

        for (let prop of obj.properties) {
            if (prop.type !== 'array')
                continue;

            let elementType = inc.getPropElementType(prop);// ?? 'pod';

            if (inc.isPropTypePod(elementType))
                continue;

            //let propName = prop.name;
            if (arraysSet.includes(elementType))
                continue;
            arraysSet.push(elementType);


            beginBlock(`inline void clearArray(${elementType}* begin, const ${elementType}* end)`);
            beginBlock(`for (auto it = begin; it != end; ++it)`);
            out(`auto& el = *it;`);

            for(;;) {
                if (inc.isPropTypePod(elementType) || 'enum' === elementType) {
                    out(`el = ${inc.getUnderlyingDefaultValue(prop) ?? '{}'}`);
                    break;
                }

                if ('string' === elementType) {
                    let defaultValue = inc.getUnderlyingDefaultValue(prop);
                    if (defaultValue) {
                        out(`el = ${defaultValue};`);
                    } else {
                        out(`el.clear();`);
                    }
                    break;
                }


                if (inc.isPropTypeDynamicContainer(elementType)) {
                    out(`el.clear();`);
                    break;
                }

                if ('array' === elementType) {
                    out(`clearArray(el.data(), el.data() + el.size());`);
                    break;
                }

                out(`clearObject(&el);`);
                break;


                //throw new Error(`Invalid property.valueType: ${objName}.${propName}.${elementType}`);
            }

            endBlock();
            endBlock();
            nl();

            // out(`template <class ArrayT>`);
            // beginBlock(`inline bool isEmptyArray_${elementType}(const ArrayT& obj)`);
            // beginBlock(`for (auto& el: obj)`);

            beginBlock(`inline bool isEmptyArray(const ${elementType}* begin, const ${elementType}* end)`);
            beginBlock(`for (auto it = begin; it != end; ++it)`);
            out(`auto& el = *it;`);


            for(;;) {

                if (inc.isPropTypePod(elementType) || 'enum' === elementType) {
                    out(`if (el != ${inc.getUnderlyingDefaultValue(prop) ?? '{}'}) return false;`);
                    break;
                }

                if ('string' === elementType) {
                    let defaultValue = inc.getUnderlyingDefaultValue(prop);
                    if (defaultValue) {
                        out(`if (el != ${defaultValue}) return false;`);
                    } else {
                        out(`if (!el.empty()) return false;`);
                    }
                    break;
                }


                if (inc.isPropTypeDynamicContainer(elementType)) {
                    out(`if (!el.empty()) return false;`);
                    break;
                }

                if ('array' === elementType) {
                    out(`if (!isArrayEmpty(el.data(), el.data() + el.size())) return false;`);
                    break;
                }

                //if ('object' === elementType) {
                    out(`if (!isEmptyObject(el)) return false;`);
                    break;
                //}


                //throw new Error(`Invalid property.valueType: ${objName}.${propName}.${elementType}`);
            }

            endBlock();

            out(`return true;`);
            endBlock();
            nl();
        }

    }
    nl();



    //
    // Generate definitions
    //
    for (let obj of packet.objects) {
        let objName = obj.name;
        let objPtr = `${objName}*`;

        beginBlock(`inline void clearObject(${objPtr} obj)`);

        for (let prop of obj.properties) {
            let propName = prop.name;
            let propRef = `obj->${propName}`;
            let propAddr = `&(obj->${propName})`;

            if (inc.isPropPod(prop)) {
                let defaultValue = prop.defaultValue; //inc.getDefaultValue(prop); // ?? '{}';
                if (defaultValue) {
                    out(`Packet::clearProp(${propAddr}, ${defaultValue});`);
                } else {
                    out(`Packet::clearProp(${propAddr});`);
                }
                //out(`${propRef} = ${defaultValue};`);
                continue;
            }

            if ('string' === prop.type) {
                if (prop.defaultValue) {
                    out(`${propRef} = "${prop.defaultValue}";`);
                } else {
                    out(`${propRef}.clear();`);
                }
                continue;
            }

            if ('enum' === prop.type) {
                let defaultValue = inc.getDefaultValue(prop) ?? '{}';
                out(`${propRef} = ${defaultValue};`);
                continue;
            }



            if ('array' === prop.type) {
                //out(`clearArray_${inc.getPropElementType(prop)}(${propAddr});`);
                out(`clearArray(${propRef}.data(), ${propRef}.data() + ${propRef}.size());`);
                continue;
            }

            if (inc.isPropTypeDynamicContainer(prop.type)) {
                out(`${propRef}.clear();`);
                continue;
            }

            if ('object' === prop.type) {
                out(`clearObject(${propAddr});`);
                continue;
            }

            throw new Error(`Invalid property.type: ${objName}.${propName}.${prop.type}`);
        }
        endBlock();


        beginBlock(`inline bool isEmptyObject(const ${objName}& obj)`);
        // console.log(obj);

        for (let prop of obj.properties) {
            let propName = prop.name;
            let propRef = `obj.${propName}`;

            if (inc.isPropPod(prop)) {
                let defaultValue = inc.getDefaultValue(prop);
                if (defaultValue) {
                    out(`if (!Packet::isPropDefault(${propRef}, ${defaultValue})) return false;`);
                } else {
                    out(`if (!Packet::isPropEmpty(${propRef})) return false;`);
                }
                //out(`if (${propRef} != ${inc.getDefaultValue(prop) ?? '0'}) return false;`);
                continue;
            }

                // console.log(prop);
            

            if ('enum' === prop.type) {
                out(`if (int(${propRef}) != int(${inc.getDefaultValue(prop) ?? '0'})) return false;`);
                continue;
            }



            if ('string' === prop.type) {
                if (prop.defaultValue) {
                    out(`if (${propRef} != "${inc.getDefaultValue(prop)}") return false;`);
                } else {
                    out(`if (!${propRef}.empty()) return false;`);
                }
                continue;
            }

            if (inc.isPropDynamicContainer(prop)) {
                out(`if (!${propRef}.empty()) return false;`);
                continue;
            }

            if ('array' === prop.type) {
                out(`if (!isEmptyArray(${propRef}.data(), ${propRef}.data() + ${propRef}.size())) return false;`);
                continue;
            }

            if ('object' === prop.type) {
                out(`isEmptyObject(${propRef});`);
                continue;
            }



            throw new Error(`Invalid property.type: ${objName}.${propName}.${prop.type}`);
        }

        out(`return true;`);
        endBlock();
        nl();
    }
    nl();



    return getSource();
}
