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
    if(!exists(src))
        return "";

    if(is_directory(src))
        return "";

    fs::path temp_file_path = Utils::getTemp() / Utils::generateString(20);
    if(!Utils::copyFile(src, temp_file_path))
        return "";

    return temp_file_path;
}

std::vector<unsigned char> FilesUtil::readBytesFromFile(const fs::path &filePath) {
    std::vector<unsigned char> bytes;

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    bytes.reserve(fileSize);

    bytes.insert(bytes.begin(),
                 std::istreambuf_iterator<char>(file),
                 std::istreambuf_iterator<char>());

    file.close();

    return bytes;
}
