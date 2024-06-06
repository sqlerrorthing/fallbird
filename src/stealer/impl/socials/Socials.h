//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_SOCIALS_H
#define STEALER_SOCIALS_H

#include "../../StealerImpl.h"

class Socials : public StealerImpl {
public:
    Socials();

    void execute(fs::path& root) override;

private:
    std::list<StealerImpl*> modules;
};

#endif //STEALER_SOCIALS_H
