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
    std::uniform_int_distribution<std::string::size_type> distribution(0, characters.size() - 1);

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
    return getUserHome() / "AppData" / "Local";
}

fs::path Utils::getRoamingPath() {
    return getUserHome() / "AppData" / "Roaming";
}

fs::path Utils::getUserHome() {
    return {std::getenv("USERPROFILE")};
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

void Utils::writeFile(const fs::path &path, const std::string &content) {
    fs::create_directories(path.parent_path());
    std::ofstream file(path, std::ios::out | std::ios::binary);

    if (file.is_open()) {
        file << "\xEF\xBB\xBF";
        file << content;
        file.close();
    }
}

void Utils::copy(const fs::path &src, const fs::path &dst) {
    try {
        if (fs::exists(src) && fs::exists(dst) && fs::is_directory(dst)) {
            fs::path destination = dst / src.filename();
            if (fs::is_directory(src)) {
                fs::copy(src, destination, fs::copy_options::recursive);
            } else if (fs::is_regular_file(src)) {
                fs::copy(src, destination);
            } else {
                #if DEV
                    std::cerr << "Source path is neither a file nor a directory." << std::endl;
                #endif
            }
        } else {
            #if DEV
                std::cerr << "Source or destination path does not exist or destination is not a directory." << std::endl;
            #endif
        }
    } catch (fs::filesystem_error& e) {
        #if DEV
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        #endif
    }
}
