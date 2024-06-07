//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_BASE64UTIL_H
#define STEALER_BASE64UTIL_H

#include "../Includes.h"

class Base64Util {
public:
    static std::vector<BYTE> b64decode(std::string &input);
};


#endif //STEALER_BASE64UTIL_H
