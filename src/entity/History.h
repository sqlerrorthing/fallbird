//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_HISTORY_H
#define STEALER_HISTORY_H

#include "../Includes.h"
#include "Entity.h"

struct History : public Entity {
    std::string url = "Unknown";
    std::string title = "Unknown";

    bool canBeWrite() override {
        return !(url.empty() && title.empty());
    }

    std::string toString() override {
        Counter::increaseHistory();

        return  "Title: " + this->title + "\n" +
                "Url: " + this->url;
    }

    std::string getFileName() override {
        return "History.txt";
    }
};

#endif //STEALER_HISTORY_H
