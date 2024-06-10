//
// Created by .1qxz on 10.06.2024.
//

#ifndef STEALER_ENTITY_H
#define STEALER_ENTITY_H

#include "../../../../Includes.h"

struct Entity {
    virtual std::string toString() = 0;

    void write(std::stringstream &ss, bool newLine = true)
    {
        ss << toString();
        ss << "\n";

        if(newLine)
            ss << "\n";
    }
};

#endif //STEALER_ENTITY_H
