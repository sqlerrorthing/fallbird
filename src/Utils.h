//
// Created by .1qxz on 06.06.2024.
//

#ifndef STEALER_UTILS_H
#define STEALER_UTILS_H
#include "Includes.h"
#include <random>
#include <algorithm>

namespace fs = std::filesystem;

class Utils {
public:
    static std::string generateString(size_t length);
    static fs::path getTemp();
    static fs::path getUserPath();
    static fs::path getRoamingPath();
    static fs::path getLocalPath();

    static std::vector<std::string> splitString(const std::string& str, char delimiter);

    static std::string readFile(const fs::path &file);
private:
    static bool copyFile(const fs::path &from, const fs::path &to);
};


#endif //STEALER_UTILS_H
