//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_HISTORY_H
#define STEALER_HISTORY_H

#include "../../../../Includes.h"
#include "Entity.h"

struct History : public Entity {
    std::string url = "Unknown";
    std::string title = "Unknown";

    std::string toString() override {
        return  "Title: " + this->title + "\n" +
                "Url: " + this->url;
    }
};

#endif //STEALER_HISTORY_H
