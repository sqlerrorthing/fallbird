//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_FILESUTIL_H
#define STEALER_FILESUTIL_H

#include <iostream>
#include <vector>
#include <filesystem>
#include <functional>
#include "Utils.h"

namespace fs = std::filesystem;

class FilesUtil {
public:
    static std::vector<fs::path> scanDirectory(const fs::path &dir, const std::function<bool(fs::path)> &callback = nullptr);
    static fs::path copyTemporary(const fs::path &src);
};


#endif //STEALER_FILESUTIL_H
