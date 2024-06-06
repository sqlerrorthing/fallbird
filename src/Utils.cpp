//
// Created by .1qxz on 06.06.2024.
//

#include "Utils.h"

std::string Utils::generateString(size_t length) {
    const std::string characters =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string random_string;
    random_string.shrink_to_fit();

    for (size_t i = 0; i < length; ++i) {
        random_string += characters[distribution(generator)];
    }

    return random_string;
}

std::filesystem::path Utils::getTemp() {
    return fs::temp_directory_path();
}

std::filesystem::path Utils::getLocalPath() {
    return getUserPath() / "AppData" / "Roaming";
}

std::filesystem::path Utils::getRoamingPath() {
    return getUserPath() / "AppData" / "Roaming";
}

std::filesystem::path Utils::getUserPath() {
    return std::filesystem::path(std::getenv("USERPROFILE"));
}
