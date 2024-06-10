//
// Created by .1qxz on 08.06.2024.
//

#include "FilesUtil.h"

std::vector<fs::path> FilesUtil::scanDirectory(const fs::path &dir, const std::function<bool(fs::path)> &callback) {
    std::vector<fs::path> paths;
    try
    {
        for (const auto& entry : fs::directory_iterator(dir)) {
            const fs::path& item_path = entry.path();

            if(callback && !callback(item_path))
                continue;

            paths.push_back(item_path);
        }
    }
    catch (const fs::filesystem_error& ignored) {}

    return paths;
}

fs::path FilesUtil::copyTemporary(const fs::path &src) {
    if(is_directory(src))
        return "";

    fs::path temp_file_path = Utils::getTemp() / Utils::generateString(20);
    if(!Utils::copyFile(src, temp_file_path))
        return "";

    return temp_file_path;
}
