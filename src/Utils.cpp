//
// Created by .1qxz on 06.06.2024.
//

#include <fstream>
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

fs::path Utils::getTemp() {
    return fs::temp_directory_path();
}

fs::path Utils::getLocalPath() {
    return getUserPath() / "AppData" / "Local";
}

fs::path Utils::getRoamingPath() {
    return getUserPath() / "AppData" / "Roaming";
}

fs::path Utils::getUserPath() {
    return fs::path(std::getenv("USERPROFILE"));
}

std::string Utils::readFile(const fs::path &source_path) {
    if(!exists(source_path))
        return "";

    if(is_directory(source_path))
    {
        #if DEV
            std::cout << "Attempt to read file " << source_path.string() << " but its directory!";
        #endif
    }

    fs::path temp_file_path = getTemp() / generateString(20);

    if(!copyFile(source_path, temp_file_path))
        return "";

    std::ifstream file(temp_file_path, std::ios::in | std::ios::binary);
    if (!file) {
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    fs::remove(temp_file_path);

    return content;
}

bool Utils::copyFile(const fs::path &from, const fs::path &to) {
    std::ifstream  src(from, std::ios::binary);
    std::ofstream  dst(to,   std::ios::binary);

    dst << src.rdbuf();

    return exists(to);
}

std::vector<std::string> Utils::splitString(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}