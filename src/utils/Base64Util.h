//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_BASE64UTIL_H
#define STEALER_BASE64UTIL_H

#include "../Includes.h"
#include <mutex>

class Base64Util {
public:
    static std::vector<BYTE> b64decode(const std::string &input);
    static std::string b64encode(const std::vector<unsigned char>& bytes);
    static std::string b64encodeKey(const std::vector<unsigned char>& bytes, const std::string& key);
    static std::mutex mtx;
};


#endif //STEALER_BASE64UTIL_H
