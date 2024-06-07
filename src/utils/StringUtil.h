//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_STRINGUTIL_H
#define STEALER_STRINGUTIL_H
#include "../Includes.h"

class StringUtil {
public:
    static std::string replace(const std::string& str, const std::string& from, const std::string& to);
    static std::vector<std::string> splitString(const std::string& str, char delimiter);
    static std::string toUpperCase(const std::string& str);
};


#endif //STEALER_STRINGUTIL_H
