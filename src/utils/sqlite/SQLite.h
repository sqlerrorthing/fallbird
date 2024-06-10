//
// Created by .1qxz on 10.06.2024.
//

#ifndef STEALER_SQLITE_H
#define STEALER_SQLITE_H

#include "../../Includes.h"
#include <codecvt>

#define ulong unsigned long long
#define uint unsigned int
#define byte unsigned char

struct RecordHeaderField {
    long size;
    long type;
};

struct TableEntry {
    std::vector<std::string> content;
};

struct SqliteMasterEntry {
    std::string itemName;
    long long rootNum;
    std::string sqlStatement;
};

class SQLite {
public:
    explicit SQLite(const fs::path &db_path);

    std::string getValue(const int &rowNum, const int &field);
    size_t getRowCount();
    bool readTable(const std::string &tableName);

private:
    ulong _dbEncoding;
    std::vector<byte> _fileBytes;
    ulong _pageSize;
    std::vector<byte> _sqlDataTypeSize = { 0, 1, 2, 3, 4, 6, 8, 8, 0, 0 };
    std::vector<std::string> _fieldNames;
    std::vector<SqliteMasterEntry> _masterTableEntries;
    std::vector<TableEntry> _tableEntries;

    ulong convertToULong(const int &startIndex, const int &size);
    int gvl(const int &startIdx);
    int cvl(const int &startIdx, int &endIdx);

    bool readTableFromOffset(const ulong &offset);
    void readMasterTable(ulong &offset);

    static bool isOdd(const long &value);
};


#endif //STEALER_SQLITE_H
