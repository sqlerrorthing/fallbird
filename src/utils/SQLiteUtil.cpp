//
// Created by .1qxz on 10.06.2024.
//

#include "SQLiteUtil.h"

void SQLiteUtil::connectAndExecute(const char *sql, const fs::path &db_path,
                                   const std::function<void(int&, sqlite3_stmt*)>& callback) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open(db_path.string().c_str(), &db);
    if (rc) {
        return;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return;
    }

    callback(rc, stmt);
    if (rc != SQLITE_DONE) {
#ifdef DEV
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
#endif
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void SQLiteUtil::connectAndRead(const char *sql, const fs::path &db_path,
                                const std::function<void(sqlite3_stmt *)> &callback) {
    SQLiteUtil::connectAndExecute(sql, db_path, [&callback, &db_path](int &rc, sqlite3_stmt *stmt) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            try {
                callback(stmt);
            }
            catch (...) {
#ifdef DEV
                std::cerr << "Error when trying to get something in the database " << db_path.string() << std::endl;
#endif
            }
        }
    });
}

std::string SQLiteUtil::readString(sqlite3_stmt *&stmt, int col) {
    const unsigned char* str = sqlite3_column_text(stmt, col);
    if(str == nullptr)
        return "";

    return reinterpret_cast<const char*>(str);
}

std::vector<BYTE> SQLiteUtil::readBytes(sqlite3_stmt *&stmt, int col) {
    std::vector<BYTE> bytes;
    const void* value = sqlite3_column_blob(stmt, col);
    int value_size = sqlite3_column_bytes(stmt, col);

    if(value == nullptr || value_size == 0)
        return {};

    bytes.assign(static_cast<const BYTE*>(value), static_cast<const BYTE*>(value) + value_size);
    return bytes;
}

long long SQLiteUtil::readLong64(sqlite3_stmt *&stmt, int col) {
    return sqlite3_column_int64(stmt, col);
}

int SQLiteUtil::readInt(sqlite3_stmt *&stmt, int col) {
    return sqlite3_column_int(stmt, col);
}
