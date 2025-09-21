// @ts-check

/**  
 * @param {any} config
 * @returns {Map}
*/
export function loadProjectConfigVars(config) {
    let prefix = "CONFIG_";
    let vars = new Map();

    for (let [key, value] of Object.entries(config)) {
        let varName = `${prefix}${key}`.toUpperCase();
        vars.set(varName, value);
    }

    //console.log(vars);

    return vars;
}

