//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_BROWSERMODULE_H
#define STEALER_BROWSERMODULE_H

#include "../../../Includes.h"
#include <thread>
#include "../../../utils/chromuim/ChromiumUtil.h"

class BrowserModule {
public:
    virtual void execute(const fs::path &root, const std::string &name, const fs::path &path) = 0;
};

#endif //STEALER_BROWSERMODULE_H