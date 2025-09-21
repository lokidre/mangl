// @ts-check

import { packObject } from "./genpacket_packet_pack.mjs";
import { unpackObject } from "./genpacket_packet_unpack.mjs";

import { clearSource, getSource, out, out_ind, nl, appendSource, beginBlock, endBlock } from './genpacket_out.mjs';
import { getGeneratedObjects } from './genpack_generated_objects.mjs';
import { createUtilHeader } from "./genpacket_util.mjs";


export function createHeaderFilePacket(params) {

    let packet = params.packetDescObject;

    let mainObject = packet.root;
    let mainName = mainObject.name;

    for (let object of packet.objects) {
        packObject(object);
        unpackObject(object);
    }

    clearSource();


    let cppStringView = 'std::string_view';
    let cppString = 'std::string';



    out(`inline void pack(const ${mainName}& obj, mangl::PacketPacker* packer);`);
    out(`inline void unpack(mangl::PacketLoader& loader, ${mainName}* obj);`);
    nl(2);

    if (params.generateClear) {
        out(`inline void clear(${mainName}* obj);`);
        nl(2);
    }

    if (params.generatePackString) {
        beginBlock(`inline auto packStringPacket(const ${mainName}& obj)`);
        out(`mangl::PacketPacker packer;`);
        out(`pack(obj, &packer);`);
        out(`${cppString} packet{packer.makeStdStringView()};`);
        out(`return packet;`);
        endBlock();
        nl();

        beginBlock(`inline auto unpackStringPacket(${cppStringView} packet, ${mainName}* obj)`);
        out(`mangl::PacketLoader loader;`);
        out(`loader.load(packet);`);
        out(`unpack(loader, obj);`);
        endBlock();
        nl();
    }

    if (params.generatePackFile) {
        beginBlock(`inline auto packFilePacket(${cppStringView} filename, const ${mainName}& obj)`);
        out(`auto packet = packStringPacket(obj);`);
        out(`apl::saveFile(filename, packet);`);
        out(`return packet;`);
        endBlock();
        nl();

        beginBlock(`inline auto unpackFilePacket(${cppStringView} filename, ${mainName}* obj)`);
        out(`auto str = apl::loadFile(filename);`);
        out(`unpackStringPacket(str, obj);`);
        endBlock();
        nl();
    }



    nl(2);

    out(`namespace ${params.implNamespace} {`);
    nl();

    // Commonly used types and nemspaces
    out(`using namespace mangl;`);
    nl();

    out(`using ::apl::Index;`);
    out(`using std::ssize;`);
    nl();


    let generatedObjects = getGeneratedObjects();


    createUtilHeader(params);

    for (let [objectName, objectGen] of generatedObjects) {
        out(`inline void packObject(const ${objectName}& obj, mangl::PacketPacker* packer);`);
        out(`inline void unpackObject(mangl::PacketLoader& loader, ${objectName}* obj);`);
    }
    nl();

    for (let [objectName, objectGen] of generatedObjects) {
        beginBlock(`inline void packObject(const ${objectName}& obj, mangl::PacketPacker* packer)`);
        appendSource(objectGen.packSource);
        endBlock();
        nl();
    }
    nl();

    for (let [objectName, objectGen] of generatedObjects) {
        beginBlock(`inline void unpackObject(mangl::PacketLoader& loader, ${objectName}* obj)`);
        appendSource(objectGen.unpackSource);
        endBlock();
        nl();
    }
    nl();

    out(`}  // namespace ${params.implNamespace}`);
    nl(2);


    beginBlock(`inline void unpack(mangl::PacketLoader& loader, ${mainName}* obj)`);
    out(`${params.implNamespace}::clearObject(obj);`);
    out(`${params.implNamespace}::unpackObject(loader, obj);`);
    endBlock();

    beginBlock(`inline void pack(const ${mainName}& obj, mangl::PacketPacker* packer)`);
    out(`packer->begin();`);
    out(`${params.implNamespace}::packObject(obj, packer);`);
    out(`packer->end();`);
    endBlock();


    if (params.generateClear) {
        beginBlock(`inline void clear(${mainName}* obj)`);
        out(`${params.implNamespace}::clearObject(obj);`);
        endBlock();
        nl(2);
    }



    //
    // Generate comparison operators just for debugging
    //
    if (params.generateObjectCompareOperator) {
        nl();
        for (let object of packet.objects) {
            if (object.haveComparison)
                continue;
            out(`inline bool operator == (const ${object.name}& a, const ${object.name}& b) noexcept;`);
            out(`inline bool operator != (const ${object.name}& a, const ${object.name}& b) noexcept;`);
        }

        nl();
        for (let object of packet.objects) {
            if (object.haveComparison)
                continue;

            beginBlock(`inline bool operator == (const ${object.name}& a, const ${object.name}& b) noexcept`);
            for (let property of object.properties) {
                out(`if (a.${property.name} != b.${property.name}) return false;`);
            }
            out(`return true;`);
            endBlock();
            nl();

            beginBlock(`inline bool operator != (const ${object.name}& a, const ${object.name}& b) noexcept`);
            out(`return !(a == b);`);
            endBlock();
            nl();
        }
        nl();
    }

    return getSource();
}

