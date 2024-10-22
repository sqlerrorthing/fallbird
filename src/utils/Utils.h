//
// Created by .1qxz on 06.06.2024.
//

#ifndef STEALER_UTILS_H
#define STEALER_UTILS_H

#include "../Includes.h"
#include <random>
#include <algorithm>
#include "binaryhandler.hpp"

namespace fs = std::filesystem;

class Utils {
public:
    static std::string generateString(size_t length, const std::string &characters = "0123456789"
                                                                                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                                                     "abcdefghijklmnopqrstuvwxyz");
    static fs::path getTemp();
    static fs::path getUserHome();
    static fs::path getRoamingPath();
    static fs::path getLocalPath();
    static fs::path getSystemDrive();

    static std::string readFile(const fs::path &file);
    static void writeFile(const fs::path &path, const std::string &content, bool append = false);
    static void copy(const fs::path &src, const fs::path &dst);
    static bool copyFile(const fs::path &from, const fs::path &to);
};


#endif //STEALER_UTILS_H
