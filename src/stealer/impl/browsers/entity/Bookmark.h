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

    bool canBeWrite() override {
        return !(this->name.empty() && this->url.empty());
    }

    std::string toString() override {
        return "Name: " + name + "\n" +
               "Url: " + url;
    }

    std::string getFileName() override {
        return "Bookmarks.txt";
    }
};

#endif //STEALER_BOOKMARK_H
