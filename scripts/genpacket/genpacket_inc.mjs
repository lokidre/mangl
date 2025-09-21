// @ts-check


/** @param {import ("./genpacket.d.ts").Property} prop  */
export function getPropType(prop) {
    return prop.type ?? 'pod';
}

/** @param {import ("./genpacket.d.ts").PropertyType} type  */
export function isPropTypeDynamicContainer(type) {
    return 'vector' === type || 'map' === type || 'set' === type;
}

/** @param {import ("./genpacket.d.ts").PropertyType} type  */
export function isPropTypeContainer(type) {
    return isPropTypeDynamicContainer(type) || 'array' === type;
}


/** @param {import ("./genpacket.d.ts").PropertyType} type  */
export function isPropTypePod(type) {
    if (type === undefined)
        return true;
    if ('char' === type || 'int' === type)
        return true;
    return false;
}

/** @param {import ("./genpacket.d.ts").Property} prop  */
export function isPropPod(prop) {
    return isPropTypePod(prop.type);
}

/** @param {import ("./genpacket.d.ts").Property} prop  */
export function isPropContainer(prop) {
    return isPropTypeContainer(prop.type);
}

/** @param {import ("./genpacket.d.ts").Property} prop  */
export function isPropDynamicContainer(prop) {
    return isPropTypeDynamicContainer(prop.type);
}


/** 
 * @param {import ("./genpacket.d.ts").Property} prop  
 * @param {import ("./genpacket.d.ts").PropertyType} propType
 * */
export function getDefaultValueType(prop, propType) {

    if (isPropTypePod(propType))
        return prop.defaultValue ?? `0`;

    if ('enum' == propType)
        return prop.defaultValue;

    if ('string' == propType)
        return prop.defaultValue;

    // switch(propType) {

    // case 'string': 
    // case 'enum': return prop.defaultValue ?? `{}`;

    // default:
    //     break;
    // }
    return undefined;
}

/** @param {import ("./genpacket.d.ts").Property} prop  */
export function getDefaultValue(prop) {
    return getDefaultValueType(prop, getPropType(prop));
}

/** @param {import ("./genpacket.d.ts").Property} prop  */
export function getUnderlyingDefaultValue(prop) {
    let valueType = getPropElementType(prop);
    return getDefaultValueType(prop, valueType);
}

/** @param {import ("./genpacket.d.ts").Property} prop  */
export function getPropElementType(prop) {
    return prop.valueType ?? prop.elemType ?? prop.underlying;
}

/** @param {import ("./genpacket.d.ts").Property} prop  */
export function isElementObject(prop) {
    let elType = getPropElementType(prop);
    if (elType === 'object')
        return true;
    if ('pod' === elType || 'map' === elType || 'vector' === elType || 'set' === elType)
        return false;
    return true;
}
