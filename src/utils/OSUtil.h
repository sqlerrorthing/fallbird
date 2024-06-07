//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_OSUTIL_H
#define STEALER_OSUTIL_H

#include "../Includes.h"
#include <windows.h>
#include <intrin.h>
#include <array>

class OSUtil {
public:
    static std::string getOsInfo();
    static std::string getMemoryInfo();
    static std::string getCPUInfo();
};


#endif //STEALER_OSUTIL_H
