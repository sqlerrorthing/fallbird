//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_FILESUTIL_H
#define STEALER_FILESUTIL_H

#include <iostream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class FilesUtil {
public:
    static std::vector<fs::path> scanDirectory(const fs::path &dir);
};


#endif //STEALER_FILESUTIL_H
