//
// Created by .1qxz on 10.06.2024.
//

#include "SQLite.h"

static std::string ulongToString(ulong value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

SQLite::SQLite(const fs::path &db_path) {
    this->_fileBytes = FilesUtil::readAllBytes(db_path);
    this->_pageSize = convertToULong(16, 2);
    this->_dbEncoding = convertToULong(56, 4);

    auto value = static_cast<ulong>(100L);
    this->readMasterTable(value);
}

std::string SQLite::getValue(const int &rowNum, const int &field) {
    try
    {
        if(rowNum >= this->getRowCount())
            return "";

        return field >= this->_tableEntries[rowNum].content.size() ? "" : _tableEntries[rowNum].content[field];
    }
    catch (...)
    {
        return "";
    }
}

size_t SQLite::getRowCount() {
    return this->_tableEntries.size();
}

bool SQLite::readTableFromOffset(const ulong &offset) {
    try
    {
        switch(this->_fileBytes[offset])
        {
            case 13: {
                auto num1 = (uint)(convertToULong((int)offset + 3, 2) - 1UL);
                auto num2 = 0;

                if(!_tableEntries.empty())
                {
                    num2 = _tableEntries.size();
                    _tableEntries.resize(_tableEntries.size() + static_cast<int>(num1) + 1);
                }
                else
                    _tableEntries.resize(static_cast<int>(num1) + 1);

                for(uint index1 = 0; (int)index1 <= (int)num1; ++index1)
                {
                    auto num3 = this->convertToULong((int)offset + 8 + (int)index1 * 2, 2);
                    if ((long)offset != 100L)
                        num3 += offset;

                    auto endIdx1 = gvl((int)num3);
                    cvl((int)num3, endIdx1);
                    auto endIdx2 = gvl((int)((long)num3 + (endIdx1 - (long)num3) + 1L));
                    cvl((int)((long)num3 + (endIdx1 - (long)num3) + 1L), endIdx2);
                    auto num4 = num3 + (ulong)(endIdx2 - (long)num3 + 1L);
                    auto endIdx3 = gvl((int)num4);
                    auto endIdx4 = endIdx3;
                    auto num5 = cvl((int)num4, endIdx3);
                    std::vector<RecordHeaderField> array;
                    auto num6 = (long)num4 - endIdx3 + 1L;
                    auto index2 = 0;

                    while(num6 < num5)
                    {
                        array.resize(index2 + 1);
                        auto startIdx = endIdx4 + 1;
                        endIdx4 = gvl(startIdx);
                        array[index2].type = cvl(startIdx, endIdx4);
                        array[index2].size = array[index2].type <= 9L ? _sqlDataTypeSize[array[index2].type] :
                                             !isOdd(array[index2].type) ? (array[index2].type - 12L) / 2L :
                                             (array[index2].type - 13L) / 2L;
                        num6 = num6 + (endIdx4 - startIdx) + 1L;
                        ++index2;
                    }

                    if(array.empty())
                        continue;

                    _tableEntries[num2 + static_cast<int>(index1)].content.resize(array.size());
                    auto num7 = 0;

                    for(auto index3 = 0; index3 <= array.size() - 1; ++index3)
                    {
                        if(array[index3].type > 9L)
                        {
                            if(!isOdd(array[index3].type))
                            {
                                std::wstring_convert<std::codecvt_utf16<wchar_t>> converter;
                                switch ((long)_dbEncoding)
                                {
                                    case 1L:
                                    {
                                        _tableEntries[num2 + static_cast<int>(index1)].content[index3] = std::string(reinterpret_cast<const char*>(&_fileBytes[num4 + num5 + num7]), array[index3].size);
                                        break;
                                    }
                                    case 2L:
                                    {
                                        std::wstring tempStr = std::wstring(reinterpret_cast<const wchar_t*>(&_fileBytes[num4 + num5 + num7]), array[index3].size / sizeof(wchar_t));
                                        _tableEntries[num2 + static_cast<int>(index1)].content[index3] = std::string(tempStr.begin(), tempStr.end());
                                        break;
                                    }
                                    case 3L:
                                    {
                                        std::wstring tempStr(reinterpret_cast<const wchar_t*>(&_fileBytes[num4 + num5 + num7]), array[index3].size / sizeof(wchar_t));

                                        for (wchar_t& ch : tempStr) {
                                            ch = ((ch & 0xFF00) >> 8) | ((ch & 0x00FF) << 8);
                                        }

                                        _tableEntries[num2 + static_cast<int>(index1)].content[index3] = std::string(tempStr.begin(), tempStr.end());
                                        break;
                                    }
                                }
                            }
                            else
                                _tableEntries[num2 + static_cast<int>(index1)].content[index3] = std::string(reinterpret_cast<const char*>(&_fileBytes[num4 + num5 + num7]), array[index3].size);
                        }
                        else
                            _tableEntries[num2 + static_cast<int>(index1)].content[index3] = ulongToString(
                                    this->convertToULong(static_cast<int>(num4 + num5 + num7),
                                                         static_cast<int>(array[index3].size)));

                        num7 += (int)array[index3].size;
                    }
                }

                break;
            }
            case 5: {
                auto num1 = (uint)(this->convertToULong((int)((long)offset + 3L), 2) - 1UL);
                for(uint index = 0; (int)index <= (int)num1; ++index)
                {
                    auto num2 = (uint)convertToULong((int)offset + 12 + (int)index * 2, 2);
                    this->readTableFromOffset((this->convertToULong((int)((long)offset + num2), 4) - 1UL) * _pageSize);
                }

                this->readTableFromOffset((convertToULong((int)((long)offset + 8L), 4) - 1UL) * _pageSize);
                break;
            }
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

void SQLite::readMasterTable(ulong &offset) {
    while(true)
    {
        switch (_fileBytes[offset])
        {
            case 5: {
                auto num1 = (uint) (convertToULong((int) offset + 3, 2) - 1UL);
                for (auto index = 0; index <= (int) num1; ++index) {
                    auto num2 = (uint) convertToULong((int) offset + 12 + index * 2, 2);
                    if (offset == 100L)
                    {
                        auto temp = convertToULong(static_cast<int>(offset) + 12 + index * 2, 2);
                        this->readMasterTable(temp);
                    }
                    else
                    {
                        auto tempOffset = convertToULong(static_cast<int>(offset) + 8, 4) - 1L * static_cast<long>(_pageSize);
                        this->readMasterTable(tempOffset);
                    }
                }

                offset = ((long) convertToULong((int) offset + 8, 4) - 1L) * (long) _pageSize;
                continue;
            }
            case 13: {
                auto num3 = convertToULong((int)offset + 3, 2) - 1UL;
                auto num4 = 0;

                if (!_masterTableEntries.empty())
                {
                    num4 = _masterTableEntries.size();
                    _masterTableEntries.resize(_masterTableEntries.size() + static_cast<size_t>(num3) + 1);
                }
                else
                {
                    _masterTableEntries.resize(static_cast<size_t>(num3) + 1);
                }

                for (ulong index1 = 0; index1 <= num3; ++index1)
                {
                    auto num2 = convertToULong((int)offset + 8 + (int)index1 * 2, 2);
                    if (offset != 100L) num2 += (ulong)offset;
                    auto endIdx1 = gvl((int)num2);
                    cvl((int)num2, endIdx1);
                    auto endIdx2 = gvl((int)((long)num2 + (endIdx1 - (long)num2) + 1L));
                    cvl((int)((long)num2 + (endIdx1 - (long)num2) + 1L), endIdx2);
                    auto num5 = num2 + (ulong)(endIdx2 - (long)num2 + 1L);
                    auto endIdx3 = gvl((int)num5);
                    auto endIdx4 = endIdx3;
                    auto num6 = cvl((int)num5, endIdx3);
                    auto numArray = new long[5];

                    for (auto index2 = 0; index2 <= 4; ++index2)
                    {
                        auto startIdx = endIdx4 + 1;
                        endIdx4 = gvl(startIdx);
                        numArray[index2] = cvl(startIdx, endIdx4);
                        numArray[index2] = numArray[index2] <= 9L ? _sqlDataTypeSize[numArray[index2]] :
                                           !isOdd(numArray[index2]) ? (numArray[index2] - 12L) / 2L :
                                           (numArray[index2] - 13L) / 2L;
                    }

                    if ((long)_dbEncoding == 1L || (long)_dbEncoding == 2L)
                    {
                        switch((long)_dbEncoding)
                        {
                            case 1L: {
                                std::wstring temp(reinterpret_cast<const wchar_t*>(_fileBytes.data() + static_cast<int>(num5 + num6 + numArray[0])),
                                                               static_cast<int>(numArray[1]) / sizeof(wchar_t));
                                _masterTableEntries[num4 + (int) index1].itemName = std::string(temp.begin(), temp.end());
                                break;
                            }
                            case 2L: {
                                std::wstring temp(reinterpret_cast<const wchar_t*>(_fileBytes.data() + static_cast<int>(num5 + num6 + numArray[0])),
                                             static_cast<int>(numArray[1]) / sizeof(wchar_t));
                                _masterTableEntries[num4 + (int) index1].itemName = std::string(temp.begin(), temp.end());
                                break;
                            }
                            case 3L: {
                                std::wstring temp(reinterpret_cast<const wchar_t*>(_fileBytes.data() + static_cast<int>(num5 + num6 + numArray[0])),
                                             static_cast<int>(numArray[1]) / sizeof(wchar_t));
                                _masterTableEntries[num4 + (int) index1].itemName = std::string(temp.begin(), temp.end());
                                break;
                            }
                        }
                    }

                    _masterTableEntries[num4 + static_cast<int>(index1)].rootNum = static_cast<long>(
                            convertToULong(static_cast<int>(num5 + num6 + numArray[0] + numArray[1] + numArray[2]),
                                           static_cast<int>(numArray[3])));

                    switch (static_cast<long>(_dbEncoding))
                    {
                        case 1: {
                            _masterTableEntries[num4 + static_cast<int>(index1)].sqlStatement =
                                    std::string(reinterpret_cast<const char*>(_fileBytes.data() + static_cast<int>(num5 + num6 +
                                                                                                                   numArray[0] + numArray[1] +
                                                                                                                   numArray[2] + numArray[3])),
                                                static_cast<int>(numArray[4]));
                            break;
                        }
                        case 2: {
                            std::wstring temp(reinterpret_cast<const wchar_t*>(_fileBytes.data() + static_cast<int>(num5 + num6 +
                                                                                                               numArray[0] + numArray[1] +
                                                                                                               numArray[2] + numArray[3])),
                                         static_cast<int>(numArray[4]) / sizeof(wchar_t));
                            _masterTableEntries[num4 + static_cast<int>(index1)].sqlStatement = std::string(temp.begin(), temp.end());

                            break;
                        }
                        case 3: {
                            std::wstring temp(reinterpret_cast<const wchar_t*>(_fileBytes.data() + static_cast<int>(num5 + num6 +
                                                                                                               numArray[0] + numArray[1] +
                                                                                                               numArray[2] + numArray[3])),
                                         static_cast<int>(numArray[4]) / sizeof(wchar_t));
                            _masterTableEntries[num4 + static_cast<int>(index1)].sqlStatement = std::string(temp.begin(), temp.end());

                            break;
                        }
                    }
                }

                break;
            }
        }

        break;
    }
}

bool SQLite::readTable(const std::string &tableName) {
    auto index1 = -1;
    for (auto index2 = 0; index2 < _masterTableEntries.size(); ++index2)
    {
        std::string t(_masterTableEntries[index2].itemName);
        if (_masterTableEntries[index2].itemName == tableName)
        {
            index1 = static_cast<int>(index2);
            break;
        }
    }

    if (index1 == -1)
        return false;

    std::string& sqlStatement = _masterTableEntries[index1].sqlStatement;
    size_t startPos = sqlStatement.find_first_of('(') + 1;
    if (startPos == std::string::npos)
        return false;

    std::vector<std::string> strArray;
    size_t endPos = sqlStatement.find_last_of(')');
    if (endPos == std::string::npos)
        return false;

    std::string fields = sqlStatement.substr(startPos, endPos - startPos);
    std::istringstream iss(fields);
    std::string field;

    while (std::getline(iss, field, ','))
    {
        field = field.substr(field.find_first_not_of(" \t"));
        size_t length = field.find(' ');
        if (length != std::string::npos)
            field = field.substr(0, length);

        if (field.compare(0, 6, "UNIQUE") != 0)
        {
            _fieldNames.push_back(field);
        }
    }

    return readTableFromOffset(static_cast<ulong>((_masterTableEntries[index1].rootNum - 1L) * _pageSize));
}

ulong SQLite::convertToULong(const int &startIndex, const int &size) {
    try
    {
        if ((size > 8) || (size == 0))
            return 0;

        ulong num = 0;
        for (int index = 0; index < size; ++index)
        {
            num = (num << 8) | static_cast<ulong>(_fileBytes[startIndex + index]);
        }
        return num;
    }
    catch(...)
    {
        return 0;
    }
}

int SQLite::gvl(const int &startIdx) {
    try
    {
        if (startIdx > _fileBytes.size())
            return 0;

        for (int index = startIdx; index <= startIdx + 8; ++index)
        {
            if (index > _fileBytes.size() - 1)
                return 0;
            if ((_fileBytes[index] & 128) != 128)
                return index;
        }

        return startIdx + 8;
    }
    catch(...)
    {
        return 0;
    }
}

int SQLite::cvl(const int &startIdx, int &endIdx) {
    try
    {
        ++endIdx;
        std::vector<byte> numArray(8);
        int num1 = endIdx - startIdx;
        bool flag = false;
        if ((num1 == 0) || (num1 > 9))
            return 0;
        switch (num1)
        {
            case 1:
                numArray[0] = static_cast<byte>(_fileBytes[startIdx] & 127);
                return *reinterpret_cast<long*>(&numArray[0]);
            case 9:
                flag = true;
                break;
        }

        int num2 = 1;
        int num3 = 7;
        int index1 = 0;
        if (flag)
        {
            numArray[0] = _fileBytes[endIdx - 1];
            --endIdx;
            index1 = 1;
        }

        int index2 = endIdx - 1;
        while (index2 >= startIdx)
        {
            if (index2 - 1 >= startIdx)
            {
                numArray[index1] = static_cast<byte>(((_fileBytes[index2] >> (num2 - 1)) & (255 >> num2)) |
                                                     (_fileBytes[index2 - 1] << num3));
                ++num2;
                ++index1;
                --num3;
            }
            else if (!flag)
            {
                numArray[index1] = static_cast<byte>((_fileBytes[index2] >> (num2 - 1)) & (255 >> num2));
            }

            index2 += -1;
        }

        return *reinterpret_cast<long*>(&numArray[0]);
    }
    catch(...)
    {
        return 0;
    }
}

bool SQLite::isOdd(const long &value) {
    return (value & 1) == 1;
}




