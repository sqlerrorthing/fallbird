//
// Created by .1qxz on 10.06.2024.
//

#ifndef STEALER_BOOKMARK_H
#define STEALER_BOOKMARK_H

#include "Entity.h"
#include "../../../../Includes.h"

struct Bookmark : public Entity {
    std::string name;
    std::string url;

    std::string toString() override {
        return "Name: " + name + "\n" +
               "Url: " + url;
    }
};

#endif //STEALER_BOOKMARK_H
