/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"

namespace mangl {

// Hiding SQLLite
struct StorageEngineSQLiteContext;

class DatabaseSQLite {
    std::unique_ptr<StorageEngineSQLiteContext> ctx_;

public:
    //using KeyValCont = std::unordered_map<String, String>;
    using KeyValCont = std::map<String, String>;

    DatabaseSQLite();
    ~DatabaseSQLite();

    void open(StrView location, StrView table);
    void close(StrView table);

    [[nodiscard]] bool isOpen(StrView table) const;

    void writeString(StrView table, StrView key, StrView val);
    String readString(StrView table, StrView key);

    KeyValCont readAll(StrView table);
};



class StorageEngineSQLite: public apl::NonCopyable {
    String table_;
    std::unique_ptr<DatabaseSQLite> db_;

public:
    using KeyValCont = DatabaseSQLite::KeyValCont;

    explicit StorageEngineSQLite(StrView table): table_(table) {
        db_ = std::make_unique<DatabaseSQLite>();
    }

    void open(StrView location) { db_->open(location, table_); }
    void close(){  db_->close(table_); }

    [[nodiscard]] bool isOpen() const { return db_->isOpen(table_); }

    void writeString(StrView key, StrView val) {
        db_->writeString(table_, key, val);
    }

    String readString(StrView key) {
        return db_->readString(table_, key);
    }

    KeyValCont readAll() { return db_->readAll(table_); }
};


class StorageEngineSQLiteSettings final: public StorageEngineSQLite {
public:
    StorageEngineSQLiteSettings(): StorageEngineSQLite("settings"){}
};

class StorageEngineSQLiteStats final: public StorageEngineSQLite {
public:
    StorageEngineSQLiteStats(): StorageEngineSQLite("stats"){}
};

}  // namespace mangl
