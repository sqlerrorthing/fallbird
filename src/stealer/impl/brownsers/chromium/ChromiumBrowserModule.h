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

    void setProfile(const std::string &_profile)
    {
        this->profile = _profile;
    }

protected:
    std::vector<BYTE> getMasterKey() {return this->master_key;}
    std::string getProfile() {return this->profile;}

private:
    std::vector<BYTE> master_key;
    std::string profile = "Default";
};


#endif //STEALER_CHROMIUMBROWSERMODULE_H
