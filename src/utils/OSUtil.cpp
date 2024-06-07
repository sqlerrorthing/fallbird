//
// Created by .1qxz on 08.06.2024.
//

#include <cstring>
#include "OSUtil.h"

std::string OSUtil::getOsInfo() {
    std::stringstream ss;

    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (GetVersionEx(&osvi)) {
        ss << "Windows " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion;
    } else {
        ss << "Unknown";
    }

    return ss.str();
}

std::string OSUtil::getMemoryInfo() {
    std::stringstream ss;

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if (GlobalMemoryStatusEx(&statex)) {
        std::uintmax_t used = (statex.ullTotalPhys - statex.ullAvailPhys) / (1024 * 1024);
        std::uintmax_t total = statex.ullTotalPhys / (1024 * 1024);

        ss << "used " << used << "MB of total ";
        ss << total << "MB";
    } else {
        ss << "Unknown";
    }

    return ss.str();
}

std::string OSUtil::getCPUInfo() {
    std::array<int, 4> integerBuffer = {};
    constexpr size_t sizeofIntegerBuffer = sizeof(int) * integerBuffer.size();

    std::array<char, 64> charBuffer = {};

    constexpr std::array<int, 3> functionIds = {
            static_cast<int>(0x8000'0002),
            static_cast<int>(0x8000'0003),
            static_cast<int>(0x8000'0004)
    };

    std::string cpu;

    for (int id : functionIds)
    {
        __cpuid(integerBuffer.data(), id);
        std::memcpy(charBuffer.data(), integerBuffer.data(), sizeofIntegerBuffer);
        cpu += std::string(charBuffer.data());
    }

    return cpu;
}

std::string OSUtil::getClipboardText() {
    if (! OpenClipboard(nullptr))
        return "";

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr)
        return "";

    char * pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr)
        return "";

    std::string text(pszText);

    GlobalUnlock(hData);

    CloseClipboard();

    return text;
}
