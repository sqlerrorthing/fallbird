//
// Created by .1qxz on 10.06.2024.
//

#include "FBookmarks.h"

void FBookmarks::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->withProfile(browser_root) / "places.sqlite";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<std::unique_ptr<Entity>> bookmarks;

    SQLiteUtil::connectAndRead(R"(
        SELECT moz_bookmarks.title, moz_places.url
        FROM moz_bookmarks
        JOIN moz_places ON moz_bookmarks.fk = moz_places.id
        WHERE moz_bookmarks.type = 1 AND moz_bookmarks.title IS NOT NULL
    )", copied_db, [&bookmarks](sqlite3_stmt *stmt) {
        auto bookmarkRecord = std::make_unique<Bookmark>();

        bookmarkRecord->name = SQLiteUtil::readString(stmt, 0);
        bookmarkRecord->url = SQLiteUtil::readString(stmt, 1);

        bookmarks.push_back(std::move(bookmarkRecord));
    });

    fs::remove(copied_db);

    Entity::writeSelf(root, bookmarks);
}
