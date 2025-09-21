/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "packet_dump.h"

#include <apeal/console.hpp>
#include <apeal/fs.hpp>

#include <mangl/packet.h>

using namespace mangl;
using namespace apl;
using namespace std;


void getArr(PacketLoader* loader);
void getVector(PacketLoader* loader);

void getKey(PacketFieldType type, PacketLoader* loader) {
    switch (type) {
    case PacketFieldType::UInt: {
        auto valU = loader->getInt();
        cout << "key"
             << " | " << valU << endl;
        break;
    }
    case PacketFieldType::Float: {
        auto valU = loader->getFloat();
        cout << "key"
             << " | " << valU << endl;
        break;
    }
    case PacketFieldType::Double: {
        auto valU = loader->getDouble();
        cout << "key"
             << " | " << valU << endl;
        break;
    }
    case PacketFieldType::String: {
        auto valU = loader->getString();
        cout << "key"
             << " | " << valU << endl;
        break;
    }
    default: break;
    }
}

void getValue(PacketFieldType type, PacketLoader* loader) {
    switch (type) {
    case PacketFieldType::UInt: {
        auto valU = loader->getInt();
        cout << "value"
             << " | " << valU << endl;
        break;
    }
    case PacketFieldType::Float: {
        auto valU = loader->getFloat();
        cout << "value"
             << " | " << valU << endl;
        break;
    }
    case PacketFieldType::Double: {
        auto valU = loader->getDouble();
        cout << "value"
             << " | " << valU << endl;
        break;
    }
    case PacketFieldType::Bool: {
        auto valU = loader->getBool();
        cout << "value"
             << " | " << valU << endl;
        break;
    }
    case PacketFieldType::String: {
        auto valU = loader->getString();
        cout << "value"
             << " | " << valU << endl;
        break;
    }
    case PacketFieldType::ObjectBegin: {
        cout << "ObjectBegin"
             << " | ";
        loader->getUnpackObj();
        cout << "Object Begin" << endl;
        break;
    }
    case PacketFieldType::ObjectEnd: {
        cout << "ObjectEnd"
             << " | ";
        loader->getSkipObj();
        cout << "Object End" << endl;
        break;
    }
    case PacketFieldType::ElementBegin: {
        cout << "ElementBegin"
             << " | ";
        loader->getUnpackObj();
        cout << "Element Begin" << endl;
        break;
    }
    case PacketFieldType::ElementEnd: {
        cout << "ElementEnd"
             << " | ";
        loader->getSkipObj();
        cout << "Element End" << endl;
        break;
    }
    case PacketFieldType::Array: {
        getArr(loader);
        break;
    }
    case PacketFieldType::Binary: {
        getVector(loader);
        break;
    }
    default: break;
    }
}

void getArr(PacketLoader* loader){
    auto size = loader->unpackArrayBegin();
    for(int i = 0; i<size; i++){
        auto valType = loader->getType();
        getValue(valType, loader);
    }
}

void getVector(PacketLoader* loader){
    auto size = loader->binarySize();
    for(int i = 0; i<size; i++){
        auto valType = loader->getType();
        getValue(valType, loader);
    }
}

void getMap(PacketLoader* loader) {
    auto size = loader->getUnpackMap();
    for (int i = 0; i < size; i++) {
        auto keyType = loader->getType();
        getKey(keyType, loader);
        auto valType = loader->getType();
        getValue(valType, loader);
    }
}


void packetDump(apl::StrView filename) {
    using namespace apl;
    using namespace std;
    using namespace mangl;

    //    auto data = loadFile(filename);
    //
    //    mangl::PacketLoader packetLoader;
    //
    //    packetLoader.load(data.data(), data.size());

    PacketLoader loader;
    auto packetData = apl::loadFile(filename);
    loader.load(packetData.data(), packetData.size());

    Console con;

    cout << " " << endl;
    cout << "ID"
         << " | ";
    cout << "Offset"
         << " | ";
    cout << "Type"
         << " | ";
    cout << "Value"
         << " | " << endl;
    cout << " " << endl;

    while (auto field = loader.getId()) {

        cout << field.id << " | ";
        cout << loader.getCurrentOffset() << " | ";

        switch (field.type) {
        case PacketFieldType::Invalid:
            cout << "Invalid"
                 << " | ";
            cout << "Invalid" << endl;
            break;
        case PacketFieldType::Int: {
            cout << "Int"
                 << " | ";
            auto val = loader.getInt();
            cout << val << endl;
            break;
        }
        case PacketFieldType::UInt: {
            cout << "UInt"
                 << " | ";
            auto val = loader.getInt();
            cout << val << endl;
            break;
        }
        case PacketFieldType::Float: {
            cout << "Float" << endl;
            auto val = loader.getFloat();
            cout << val << " | ";
            break;
        }
        case PacketFieldType::Double: {
            cout << "Double"
                 << " | ";
            auto val = loader.getDouble();
            cout << val << endl;
            break;
        }
        case PacketFieldType::Bool: {
            cout << "Bool"
                 << " | ";
            auto val = loader.getBool();
            cout << val << endl;
            break;
        }
        case PacketFieldType::String: {
            cout << "String"
                 << " | ";
            auto val = loader.getString();
            cout << val << endl;
            break;
        }
        case PacketFieldType::Binary: {
            cout << "Binary"
                 << " | ";
            cout << "Binary Begin" << endl;
            getVector(&loader);
            break;
        }
        case PacketFieldType::Map: {
            cout << "Map"
                 << " | ";
            cout << "Map Begin" << endl;
            getMap(&loader);
            break;
        }
        case PacketFieldType::Array: {
            cout << "Array"
                 << " | ";
            getArr(&loader);

            cout << "Array Begin" << endl;
            break;
        }
        case PacketFieldType::ObjectBegin: {
            cout << "ObjectBegin"
                 << " | ";
            loader.getUnpackObj();
            cout << "Object Begin" << endl;
            break;
        }
        case PacketFieldType::ObjectEnd: {
            cout << "ObjectEnd"
                 << " | ";
            loader.getSkipObj();
            cout << "Object End" << endl;
            break;
        }
        case PacketFieldType::ElementBegin: {
            cout << "ElementBegin"
                 << " | ";
            loader.getUnpackObj();
            cout << "Element Begin" << endl;
            break;
        }
        case PacketFieldType::ElementEnd: {
            cout << "ElementEnd"
                 << " | ";
            loader.getSkipObj();
            cout << "Element End" << endl;
            break;
        }
        default: break;
        }
    }
}
