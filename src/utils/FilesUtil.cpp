//
// Created by .1qxz on 08.06.2024.
//

#include "FilesUtil.h"

std::vector<fs::path> FilesUtil::scanDirectory(const fs::path &dir) {
    std::vector<fs::path> paths;
    try
    {
        for (const auto& entry : fs::directory_iterator(dir)) {
            const fs::path& item_path = entry.path();
            paths.push_back(item_path);
        }
    }
    catch (const fs::filesystem_error& ignored) {}

    return paths;
}
