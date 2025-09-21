import { map } from './common/map.mjs';
import { nullTag } from './common/null.mjs';
import { seq } from './common/seq.mjs';
import { string } from './common/string.mjs';
import { boolTag } from './core/bool.mjs';
import { float, floatExp, floatNaN } from './core/float.mjs';
import { int, intHex, intOct } from './core/int.mjs';
import { schema } from './core/schema.mjs';
import { schema as schema$1 } from './json/schema.mjs';
import { binary } from './yaml-1.1/binary.mjs';
import { omap } from './yaml-1.1/omap.mjs';
import { pairs } from './yaml-1.1/pairs.mjs';
import { schema as schema$2 } from './yaml-1.1/schema.mjs';
import { set } from './yaml-1.1/set.mjs';
import { timestamp, floatTime, intTime } from './yaml-1.1/timestamp.mjs';

const schemas = new Map([
    ['core', schema],
    ['failsafe', [map, seq, string]],
    ['json', schema$1],
    ['yaml11', schema$2],
    ['yaml-1.1', schema$2]
]);
const tagsByName = {
    binary,
    bool: boolTag,
    float,
    floatExp,
    floatNaN,
    floatTime,
    int,
    intHex,
    intOct,
    intTime,
    map,
    null: nullTag,
    omap,
    pairs,
    seq,
    set,
    timestamp
};
const coreKnownTags = {
    'tag:yaml.org,2002:binary': binary,
    'tag:yaml.org,2002:omap': omap,
    'tag:yaml.org,2002:pairs': pairs,
    'tag:yaml.org,2002:set': set,
    'tag:yaml.org,2002:timestamp': timestamp
};
function getTags(customTags, schemaName) {
    let tags = schemas.get(schemaName);
    if (!tags) {
        if (Array.isArray(customTags))
            tags = [];
        else {
            const keys = Array.from(schemas.keys())
                .filter(key => key !== 'yaml11')
                .map(key => JSON.stringify(key))
                .join(', ');
            throw new Error(`Unknown schema "${schemaName}"; use one of ${keys} or define customTags array`);
        }
    }
    if (Array.isArray(customTags)) {
        for (const tag of customTags)
            tags = tags.concat(tag);
    }
    else if (typeof customTags === 'function') {
        tags = customTags(tags.slice());
    }
    return tags.map(tag => {
        if (typeof tag !== 'string')
            return tag;
        const tagObj = tagsByName[tag];
        if (tagObj)
            return tagObj;
        const keys = Object.keys(tagsByName)
            .map(key => JSON.stringify(key))
            .join(', ');
        throw new Error(`Unknown custom tag "${tag}"; use one of ${keys}`);
    });
}

export { coreKnownTags, getTags };
