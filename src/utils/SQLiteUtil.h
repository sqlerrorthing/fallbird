//
// Created by .1qxz on 10.06.2024.
//

#ifndef STEALER_SQLUTIL_H
#define STEALER_SQLUTIL_H

#include <functional>
#include "../Includes.h"
#include "sqlite3.h"


class SQLiteUtil {
public:
    static void connectAndRead(const char* sql, const fs::path &db_path,
                               const std::function<void(sqlite3_stmt*)>& callback);

    static void connectAndExecute(const char* sql, const fs::path &db_path,
                                  const std::function<void(int&, sqlite3_stmt*)>& callback);

    static std::string readString(sqlite3_stmt *&stmt, int col);
};


#endif //STEALER_SQLUTIL_H
