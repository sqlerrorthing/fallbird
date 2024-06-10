//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_BROWSERMODULE_H
#define STEALER_BROWSERMODULE_H

#include "../../../Includes.h"
#include <thread>
#include "../../../utils/chromuim/ChromiumUtil.h"
#include "entity/SharedEntity.h"

class BrowserModule {
public:
    virtual void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) = 0;
};

#endif //STEALER_BROWSERMODULE_H
