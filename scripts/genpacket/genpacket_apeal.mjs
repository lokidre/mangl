// @ts-check

import { clearSource, getSource, out, nl, appendSource, out_ind, beginBlock, endBlock } from './genpacket_out.mjs';
import { getGeneratedObjects } from './genpack_generated_objects.mjs';

import { generateApealPacketBuild } from './genpacket_apeal_build.mjs';
import { generateApealPacketLoad } from './genpacket_apeal_load.mjs';
import { createUtilHeader } from "./genpacket_util.mjs";


export function generateApealPackerSources(params) {

    clearSource();

    let fileType = params.fileType;
    let packet = params.packetDescObject;

    let builderSetup = {};

    switch (fileType) {
    case 'yaml':
        builderSetup.cppSuffix = 'Yaml';
        builderSetup.cppLoader = `::apl::${builderSetup.cppSuffix}`;
        break;
    case 'json':
        builderSetup.cppSuffix = 'Json';
        builderSetup.cppLoader = `::apl::${builderSetup.cppSuffix}Loader`;
        break;
    default:
        break;
    }

    let cppSuffix = builderSetup.cppSuffix;

    builderSetup.cppPacket = `apl::${cppSuffix}`;
    builderSetup.cppNode = `apl::${cppSuffix}Node`;
    builderSetup.cppBuilder = `apl::${cppSuffix}Builder`;
    builderSetup.cppParser = `apl::${cppSuffix}Parser`;
    builderSetup.cppStringViewType = 'std::string_view';
    builderSetup.cppStringType = 'std::string';




    for (let object of packet.objects) {
        generateApealPacketBuild(object, builderSetup);
        generateApealPacketLoad(object, builderSetup);
    }


    let mainObject = packet.root;
    let mainName = mainObject.name;

    clearSource();


    let setup = builderSetup;
    let cppPacket = setup.cppPacket;
    let cppBuilder = setup.cppBuilder;
    let cppParser = setup.cppParser;
    let cppNode = setup.cppNode;
    let cppStringView = setup.cppStringViewType;

    out(`inline void pack(const ${mainName}& obj, ${cppBuilder}* builder);`);
    out(`inline void unpack(const ${cppPacket}& root, ${mainName}* obj);`);
    nl();
   
    if (params.generatePackString) {
        beginBlock(`inline auto packString${cppSuffix}(const ${mainName}& obj)`);
        out(`${cppBuilder} builder;`);
        out(`pack(obj, &builder);`);
        out(`return builder.str();`);
        endBlock();
        nl();

        beginBlock(`inline auto unpackString${cppSuffix}(${cppStringView} str, ${mainName}* obj)`);
        out(`${cppPacket} packet;`);
        out(`${cppParser} parser;`);
        out(`parser.parse(str, &packet);`);
        out(`unpack(packet, obj);`);
        endBlock();
        nl();


        beginBlock(`inline auto unpackData${cppSuffix}(apl::DataParam<>::Arg data, ${mainName}* obj)`);
        out(`unpackString${cppSuffix}({reinterpret_cast<const char *>(data.data()), data.size()}, obj);`);
        endBlock();
        nl();
    }

    if (params.generatePackFile) {
        beginBlock(`inline auto packFile${cppSuffix}(${cppStringView} filename, const ${mainName}& obj)`);
        out(`auto str = packString${cppSuffix}(obj);`);
        out(`apl::saveFile(filename, str);`);
        endBlock();
        nl();

        beginBlock(`inline auto unpackFile${cppSuffix}(${cppStringView} filename, ${mainName}* obj)`);
        out(`auto str = apl::loadFile(filename);`);
        out(`unpackString${cppSuffix}(str, obj);`);
        endBlock();
        nl();

        beginBlock(`inline auto unpackFileIfExists${cppSuffix}(${cppStringView} filename, ${mainName}* obj)`);
        out(`if (!apl::fileExists(filename)) return;`);
        out(`auto str = apl::loadFile(filename);`);
        out(`unpackString${cppSuffix}(str, obj);`);
        endBlock();
        nl();

    }


    nl(2);

    out(`namespace ${params.implNamespace} {`);

    nl();

    out(`using namespace mangl;`);
    nl();

    out(`using ::apl::Index;`);
    out(`using std::ssize;`);
    nl();

    createUtilHeader(params);


    out(`template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>`);
    out(`bool isEmpty${setup.cppSuffix}(T name) { return false; }`, 0, 1);

    out(`inline bool isEmpty${setup.cppSuffix}(${cppStringView} name) { return name.empty(); }`, 0, 1);


    for (let [objectName, objectGen] of getGeneratedObjects()) {
        out(`inline void packObject(const ${objectName}& obj, ${cppStringView} name, ${cppBuilder}* builder);`);
        out(`inline void unpackObject(const ${cppNode}& node, ${objectName}* obj);`);
        nl();
    }
    nl();


    for (let [objName, objGen] of getGeneratedObjects()) {

        beginBlock(`inline void packObject(const ${objName}& obj, ${cppStringView} name, ${cppBuilder}* builder)`);
        appendSource(objGen.packSource);
        endBlock();
        nl();

        // out(`inline void packObject(const ${objectName}& obj, std::integral auto name, ${cppBuilder}* builder) {`);
        // appendSource(objectGen.packSource);
        // out('}');

        beginBlock(`inline void unpackObject(const ${cppNode}& node, ${objName}* obj)`);
        appendSource(objGen.unpackSource);
        endBlock();
        nl();
    }


    out(`}  // namespace ${params.implNamespace}`);
    nl(2);



    beginBlock(`inline void pack(const ${mainName}& obj, ${cppBuilder}* builder)`);
    out(`builder->begin();`);
    out(`${params.implNamespace}::packObject(obj, "${mainName}", builder);`);
    out(`builder->end();`);
    endBlock();
    nl();

    beginBlock(`inline void unpack(const ${setup.cppPacket}& root, ${mainName}* obj)`);
    out(`${params.implNamespace}::clearObject(obj);`);
    beginBlock(`for (auto& docNode: root.front())`);
    out(`${params.implNamespace}::unpackObject(docNode, obj);`);
    endBlock();
    endBlock();
    nl();


    return getSource();

}

