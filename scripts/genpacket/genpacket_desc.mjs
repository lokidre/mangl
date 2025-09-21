// @ts-check

export function preprocessPacketDescription(params) {

    let packet = params.packetDescObject;

    if (packet.generate) {
        let generate = packet.generate;
        if (generate.string)
            params.generatePackString = true;
        if (generate.file)
            params.generatePackString = params.generatePackFile = true;
    }
    
    if (!packet.root) {
        packet.root = {};
    }

    if (!packet.root.name) {
        let firstObj = packet.objects[0];
        packet.root.name = firstObj.name;
    }

    for (let obj of packet.objects) {
        if (!obj.name) 
            throw new Error(`object.name field is missing`);


        let currentId = 1;
        let maxId = 1;

        if (obj.fields) {
            let fields = [];

            for (let field of obj.fields) {
                fields.push({name: field});
            }

            if (!obj.properties) {
                obj.properties = [];
            }

            fields.push.apply(fields, obj.properties);

            obj.properties = fields;
        }

        for (let prop of obj.properties) {
            if (!prop.name) 
                throw new Error(`property.name field is missing: ${obj.name}`);
        
            // if (!property.type)
            //     property.type = 'pod';

            if (prop.id) {
                if (prop.id >= maxId) {
                    currentId = prop.id + 1;
                    maxId = currentId;
                }
            } else {  // User didn't provide property id
                prop.id = currentId;
                currentId++;
                maxId++;
            }

            if (!prop.type && prop.object)
                prop.type = 'object';

            if (prop.type === 'enum') {
                if (!prop.object && prop.enum)
                    prop.object = prop.enum

                assertObjectProp(obj, prop.object, "'enum' or 'object' fields are missing");
            }


        }
    }

}

function assertObjectProp(obj, condition, message) {
    if (!condition)
        throw new Error(`${obj.name}: ${message}`);
}
