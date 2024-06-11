//
// Created by .1qxz on 11.06.2024.
//

#ifndef STEALER_TOX_H
#define STEALER_TOX_H


#include "../../../StealerModule.h"

class Tox : public StealerModule {
public:
    void execute(fs::path &root) override;
};


#endif //STEALER_TOX_H
