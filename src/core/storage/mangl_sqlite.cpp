/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_sqlite.h"

#include "../../3rdparty/sqlite/sqlite3.h"

#include "../mangl_common.h"
#include "../mangl_environment.h"

#include <apeal/file.hpp>
#include <apeal/uuid.hpp>


namespace mangl {

static auto sqliteCreateTableTemplate = R"(
CREATE TABLE IF NOT EXISTS ${table} (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE,
    value TEXT
);

CREATE UNIQUE INDEX IF NOT EXISTS ${table}_name_index ON ${table}(name);
)";

//INSERT OR REPLACE INTO %s (`name`, `value`) VALUES ('%q', '%q');

static auto sqliteInsertValueTemplate = R"(
INSERT INTO ${table} (`name`, `value`) VALUES (?1, ?2)
    ON CONFLICT(`name`) DO UPDATE SET `value`=?2 ;
)";

static auto sqliteSelectValueTemplate = R"(
SELECT value FROM ${table} WHERE name = ? LIMIT 1;
)";

static auto sqliteSelectAllTemplate = R"(
SELECT name, value FROM ${table};
)";


static String formatSql(StrView tableV, StrView s) {
    String sql{s};
    String var = "${table}";
    String table{tableV};

    for (;;) {
        auto pos = sql.find(var);
        if (pos == sql.npos)
            break;

        sql.replace(pos, var.size(), table);
    }

    return sql;
}

struct StorageEngineSQLiteContext {
    //using KVContainer = std::unordered_map<String, String>;
    using KVContainer = std::map<String, String>;

    sqlite3 *db{};
    String dbfile;

    struct TableInfo {
        String insertSql, selectSql;
        sqlite3_stmt* insertStmt{};
        sqlite3_stmt* selectStmt{};

        String selectAllSql;
    };

    StrValCont<TableInfo> tables;

    bool isOpen(StrView table) {
        if (!db)
            return false;
        return tables.find(table) != tables.end();
    }

    void openDbFile(StrView locationView) {

        String location{locationView};

        auto [parent, name] = apl::splitFilePath(location);
        if (!parent.empty() && parent != "." && parent != "..") {
            apl::AFileSystem::mkdirIfNotExistsParents(parent);
        }

        auto flags = SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX;
        int result = sqlite3_open_v2(location.c_str(), &db, flags, nullptr);
        if (SQLITE_OK != result) {
            if (!apl::fileExists(location))
                mthrowError("SQLITE:cannot connect to database: %s", location);

            // Try to move the damaged file to temporary directory
            apl::FileSystem fs;
            auto tmpFile = apl::makeFilePathExt(Env::tempDir, apl::generateUuidBase58(), "sqlite");

            try{
                fs.rename(location, tmpFile);
            }
            catch(...) {
                // If renaming to temporary directory fails, then rename to the same dir
                tmpFile = location + "." + apl::generateUuidBase58();
                fs.rename(location, tmpFile);
            }

            // Check if the file is still there, delete it
            fs.rmIfExists(location);

            result = sqlite3_open_v2(location.c_str(), &db, flags, nullptr);
            apl::verify(SQLITE_OK == result, "SQLITE:cannot connect to database: %s", location);
        }
        dbfile = location;
    }

    void open(StrView locationView, StrView table)
    {
        if (!db) {
            openDbFile(locationView);
        }

        if (tables.find(table) == tables.end()) {

//            auto formatSql = [&table](auto s) {
//                String sql = s;
//                String var = "${table}";
//                for (;;) {
//                    if (auto pos = sql.find(var); pos != String::npos)
//                        sql.replace(pos, var.size(), table);
//                    else
//                        break;
//                }
//                return sql;
//            };
//
//            auto stmt = formatSql(sqliteCreateTableTemplate);

            auto stmt = formatSql(table, sqliteCreateTableTemplate);

            char *errmsg{};
            auto result = sqlite3_exec(db, stmt.c_str(), nullptr, nullptr, &errmsg);
            if (result != SQLITE_OK) {
                close();
                if (SQLITE_NOTADB == result) {
                    apl::FileSystem::rm(locationView);
                }
                openDbFile(locationView);
                result = sqlite3_exec(db, stmt.c_str(), nullptr, nullptr, &errmsg);
            }

            apl::verify(!result, "SQLite:create_table: %s", errmsg);

            auto &ti = tables.emplace(table, TableInfo()).first->second;

            const char *end;
            int flags = SQLITE_PREPARE_PERSISTENT;

            ti.insertSql = formatSql(table, sqliteInsertValueTemplate);
            result = sqlite3_prepare_v3(db, ti.insertSql.c_str(), -1, flags, &ti.insertStmt, &end);
            apl::verify(result == SQLITE_OK, "SQLite:prep_insert: %s", sqlite3_errmsg(db));


            ti.selectSql = formatSql(table, sqliteSelectValueTemplate);
            result = sqlite3_prepare_v3(db, ti.selectSql.c_str(), -1, flags, &ti.selectStmt, &end);
            apl::verify(result == SQLITE_OK, "SQLite:prep_select: %s", sqlite3_errmsg(db));

            ti.selectAllSql = formatSql(table, sqliteSelectAllTemplate);
        }

    }

    void close() {
        if (db) {
            sqlite3_close(db);
            db = nullptr;
        }
    }

    void close(StrView table)
    {
        tables.erase(table);

        if (tables.empty())
            close();
    }


    void writeString(StrView table, StrView key, StrView value)
    {
        auto it = tables.find(table);

        apl::verify(it != tables.end(), "SQLite:Table not open: %s", table);

        auto &ti = it->second;

        auto result = sqlite3_bind_text(ti.insertStmt, 1, key.data(), (int)key.size(), nullptr);
        apl::verify(result == SQLITE_OK, "SQLite:writeString:bind_1: %s", sqlite3_errmsg(db));

        result = sqlite3_bind_text(ti.insertStmt, 2, value.data(), (int)value.size(), nullptr);
        apl::verify(result == SQLITE_OK, "SQLite:writeString:bind_2: %s", sqlite3_errmsg(db));

        result = sqlite3_step(ti.insertStmt);
        apl::verify(result == SQLITE_DONE, "SQLite:writeString:step: %s", sqlite3_errmsg(db));

        sqlite3_reset(ti.insertStmt);
    }

    String readString(StrView table, StrView key)
    {
        String value;

        auto it = tables.find(table);
        apl::verify(it != tables.end(), "Table not open: %s", table);
        auto &ti = it->second;

        auto result = sqlite3_bind_text(ti.selectStmt, 1, key.data(), (int)key.size(), nullptr);
        apl::verify(SQLITE_OK == result, "SQLite:readString:bind: %s", sqlite3_errmsg(db));

        for (;;) {
            result = sqlite3_step(ti.selectStmt);

            if (SQLITE_ROW == result) {
                auto size = sqlite3_column_bytes(ti.selectStmt, 0);
                if (!size)  // if NULL
                    break;
                auto data = (const char *)sqlite3_column_text(ti.selectStmt, 0);
                value.assign(data,size);
                break;
            }
            if (SQLITE_DONE == result)
                break;
            mthrowError("SQLite:readString: %s", sqlite3_errmsg(db));
        }

        sqlite3_reset(ti.selectStmt);

        return value;
    }

    KVContainer readAll(StrView table)
    {
        auto it = tables.find(table);
        apl::verify(it != tables.end(), "Table not open: %s", table);
        auto &ti = it->second;

        sqlite3_stmt *stmt;
        const char *tail;

        auto result = sqlite3_prepare_v2(db, ti.selectAllSql.c_str(), -1, &stmt, &tail);
        apl::verify(SQLITE_OK == result, "SQLite:readAll: %s", sqlite3_errmsg(db));

        KVContainer kv;
        String key, value;

        for (;;) {
            result = sqlite3_step(stmt);

            if (SQLITE_ROW == result) {

                auto size = sqlite3_column_bytes(stmt, 0);
                if (!size) {
                    MDebugTrap(); // shouldn't be happening
                    continue;
                }
                auto data = (const char *)sqlite3_column_text(stmt, 0);
                key.assign(data,size);

                size = sqlite3_column_bytes(stmt, 1);
                if (size) {
                    data = (const char *)sqlite3_column_text(stmt, 1);
                    value.assign(data,size);
                } else {
                    value.clear();
                }

                kv[key] = value;
                continue;
            }
            if (SQLITE_DONE == result)
                break;
            mthrowError("SQLite:readAll: %s", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
        return kv;
    }

};



DatabaseSQLite::DatabaseSQLite() {
    ctx_ = std::make_unique<StorageEngineSQLiteContext>();
}

DatabaseSQLite::~DatabaseSQLite() {
    ctx_->close();
    ctx_.reset();
}

bool DatabaseSQLite::isOpen(StrView table) const {
    return ctx_->isOpen(table);
}


void DatabaseSQLite::open(StrView location, StrView table) {
    ctx_->open(location, table);
}

void DatabaseSQLite::close(StrView table) {
    ctx_->close(table);
}

void DatabaseSQLite::writeString(StrView table, StrView key, StrView val) {
    ctx_->writeString(table, key, val);
}

String DatabaseSQLite::readString(StrView table, StrView key) {
    return ctx_->readString(table, key);
}

auto DatabaseSQLite::readAll(StrView table) -> KeyValCont {
    return ctx_->readAll(table);
}


}  // namespace mangl
