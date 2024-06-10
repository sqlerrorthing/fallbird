//
// Created by .1qxz on 10.06.2024.
//

#ifndef STEALER_FPASSWORDS_H
#define STEALER_FPASSWORDS_H


#include "../FirefoxBrowserModule.h"

class FPasswords : public FirefoxBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
};


#endif //STEALER_FPASSWORDS_H
