/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_setup.h"

#if MANGL_BLUETOOTH_DBUS

#include <sdbus-c++/sdbus-c++.h>



namespace mangl::net::impl {

//static auto ServiceName = "org.mangl.bitmpserver";
//auto objectPath = "/org/bluez/object";

class Communicator: public sdbus::AdaptorInterfaces<sdbus::ObjectManager_adaptor> {
public:
    Communicator(sdbus::IConnection& connection, sdbus::ObjectPath objectPath)
        : AdaptorInterfaces(connection, std::move(objectPath)) {
    }
};

constexpr inline auto InterfaceDbusProperties = "org.freedesktop.DBus.Properties";
constexpr inline auto InterfaceDbusObjectManager = "org.freedesktop.DBus.ObjectManager";
constexpr inline auto InterfaceBluez = "org.bluez";
constexpr inline auto InterfaceAdapter = "org.bluez.Adapter1";
constexpr inline auto InterfaceGattManager = "org.bluez.GattManager1";
constexpr inline auto InterfaceAdvertisingManager = "org.bluez.LEAdvertisingManager1";


struct BtDbusContext {

    String bluezDevice = "/org/bluez/hci0";


    String destination = "org.bluez.mpsample";
    String appPath = "/org/bluez/mpsample";
    String advPath = "/org/bluez/mpsample/adv";


    std::unique_ptr<sdbus::IConnection> connection;
    std::unique_ptr<sdbus::IProxy> bluezProxy;
    std::unique_ptr<sdbus::IProxy> bluezRootProxy;
    std::unique_ptr<sdbus::IProxy> bluezGattProxy;

    void setProperty(const std::string& name, bool value) {
        bluezProxy->setProperty(name).onInterface(InterfaceAdapter).toValue(value);
    }

    void setProperty(StringA name, StringA value) {
        bluezProxy->setProperty(name).onInterface(InterfaceAdapter).toValue(value);
    }

    std::string getProperty(const std::string& name) {
        return bluezProxy->getProperty(name).onInterface(InterfaceAdapter);
    }

    bool getBoolProperty(const std::string& name) {
        return bluezProxy->getProperty(name).onInterface(InterfaceAdapter);
    }

    bool getBluetoothStatus() {
        sdbus::Variant variant;

        bluezProxy->callMethod("Get")
            .onInterface(InterfaceDbusProperties)
            .withArguments(InterfaceAdapter, "Powered")
            .storeResultsTo(variant);

        return variant.get<bool>();
    }

    void createBusConnection() {
        connection = sdbus::createSystemBusConnection();

        //objectManagerProxy = sdbus::createProxy(*connection.get(), destination, appPath);

        std::map<sdbus::ObjectPath, std::map<String, std::map<String, sdbus::Variant>>> objectsInterfacesAndProperties;

        bluezRootProxy = sdbus::createProxy(*connection.get(), InterfaceBluez, "/");
        bluezRootProxy->callMethod("GetManagedObjects").onInterface(InterfaceDbusObjectManager).storeResultsTo(objectsInterfacesAndProperties);

        std::vector<String> adapters;

        for (auto& [objectKey, objectValue] : objectsInterfacesAndProperties) {
            bool adapterFound = false;
            String adapterName;

            for (auto& [key, value] : objectValue) {
                if (InterfaceAdapter == key) {
                    adapterFound = true;
                    adapterName = key;
                }
            }

            if (adapterFound)
                adapters.push_back(objectKey);
        }

        MDebugPrint("Adapters found:");
        for (auto& adapter: adapters) {
            MDebugPrint("   %s", adapter);
        }

        if (adapters.empty()) {
            throw std::runtime_error("Adapter not found");
        }

        bluezDevice = adapters.front();


        bluezProxy = sdbus::createProxy(*connection.get(), InterfaceBluez, bluezDevice);
        //bluezProxy = sdbus::createProxy(*connection.get(), BluezService, "/");

        bluezGattProxy = sdbus::createProxy(*connection.get(), InterfaceGattManager, bluezDevice);
        bluezGattProxy->finishRegistration();

        int a = 0;
    }

};

using BtDictionary = std::map<std::string,std::map<std::string, sdbus::Variant>>;

}  // namespace mangl

#endif // If bluetooth is enabled

