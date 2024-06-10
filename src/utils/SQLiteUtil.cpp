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
#if DEV
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
#endif
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void SQLiteUtil::connectAndRead(const char *sql, const fs::path &db_path,
                                const std::function<void(sqlite3_stmt *)> &callback) {
    SQLiteUtil::connectAndExecute(sql, db_path, [&callback](int &rc, sqlite3_stmt *stmt) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            callback(stmt);
        }
    });
}
