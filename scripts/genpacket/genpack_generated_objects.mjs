// @ts-check

var generatedObjects_ = new Map();

export function clearGeneratedObjects() {
    generatedObjects_.clear();
}

export function getGeneratedObjects() {
    return generatedObjects_;
}


export function updateGeneratedObject(name, props) {

    let genObject = generatedObjects_.get(name);

    if (!genObject) {
        genObject = {};
    }

    Object.keys(props).forEach(function (key) {
        genObject[key] = props[key];
    });

    generatedObjects_.set(name, genObject);
}
