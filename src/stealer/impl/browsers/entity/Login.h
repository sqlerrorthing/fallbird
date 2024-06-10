//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_LOGIN_H
#define STEALER_LOGIN_H

#include "../../../../Includes.h"
#include "Entity.h"

struct Login : public Entity {
    std::string origin;
    std::string username;
    std::string password;

    std::string toString() override {
        return  "Url: " + this->origin + "\n" +
                "  Username: " + this->username + "\n" +
                "  Password: " + this->password;
    }
};

#endif //STEALER_LOGIN_H
