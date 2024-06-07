//
// Created by .1qxz on 07.06.2024.
//

#include "StringUtil.h"

std::string StringUtil::replace(const std::string& str, const std::string& from, const std::string& to) {
    std::string out(str);
    size_t start_pos = out.find(from);
    if (start_pos != std::string::npos) {
        out.replace(start_pos, from.length(), to);
    }

    return out;
}

std::vector<std::string> StringUtil::splitString(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

std::string StringUtil::toUpperCase(const std::string &str) {
    std::string result = str;
    std::locale loc;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::toupper(result[i], loc);
    }
    return result;
}
