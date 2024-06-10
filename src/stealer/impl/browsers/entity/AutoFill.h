//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_AUTOFILL_H
#define STEALER_AUTOFILL_H

#include "../../../../Includes.h"
#include "Entity.h"

struct AutoFill : public Entity {
    std::string name;
    std::string value;

    std::string toString() override {
        std::stringstream ss;
        return "Name: " + this->name + "\n" +
               "Value: " + this->value;

        return ss.str();
    }
};

#endif //STEALER_AUTOFILL_H
