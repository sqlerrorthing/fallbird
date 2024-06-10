//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_FIREFOXBROWSERMODULE_H
#define STEALER_FIREFOXBROWSERMODULE_H

#include "../BrowserModule.h"

class FirefoxBrowserModule : public BrowserModule {
public:
    void setProfile(const std::string &_profile)
    {
        this->profile = _profile;
    }

    fs::path withProfile(const fs::path &browser_root)
    {
        return browser_root / this->getProfile();
    }

protected:
    std::string getProfile() {return this->profile;}

private:
    std::string profile;
};

#endif //STEALER_FIREFOXBROWSERMODULE_H
