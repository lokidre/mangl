// @ts-check

/**
 * 
 * @param {string | Array | number | object} value 
 * @param {*} vars 
 * @param {boolean} [nothrow=false]
 * @returns 
 */
export function processValueBody(value, vars, nothrow) {

    if (Array.isArray(value)) {
        let values = [];
        for (let v of value) {
            values.push(processValueBody(v, vars, nothrow));
        }
        return values;
    }

    if (typeof value === 'number') return value.toString();


    if (typeof value === 'object') {
        let object = {};
        for (let v in value) {
            object[v] = processValueBody(value[v], vars, nothrow);
        }
        return object;
    }

    let regex = new RegExp(/\$\{[a-zA-Z0-9_.-]*\}/gi);
    let matches = value.match(regex, value);

    if (!matches) return value;

    let replaced = false;


    for (let match of matches) {
        if (match.length <= 3) continue;

        let varName = match.substring(2, match.length - 1);
        let varValue = vars.get(varName.toUpperCase());

        if (varValue === undefined) {
            if (nothrow)
                return undefined;
            throw new Error(`Invalid variable: ${varName}`);
        }

        replaced = true;

        if (Array.isArray(varValue)) {
            value = varValue;
        } else {
            value = value.replace(match, varValue);
        }
    }


    if (replaced) return processValueBody(value, vars);

    return value;
}
