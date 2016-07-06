#include "csv_parser.h"

#include <fstream>

const CCsvParser::Row CCsvParser::emptyRow;

// CSV 解析状态 -- 取决于 ENCLOSURE_CHAR(") 的个数
enum
{
    CSV_ENCLOSURE_NONE,
    CSV_ENCLOSURE_ENTER,
    CSV_ENCLOSURE_EXIT
};

void CCsvParser::Parse(const std::string& file_path)
{
    Parse(std::ifstream(file_path));
}

void CCsvParser::Parse(std::istream& is)
{
    quoteStatus = CSV_ENCLOSURE_NONE;
    std::string line;
    while (std::getline(is, line))
        ParseLine(line);
}

// 当前 fieldCache 缓存的内容压入 record 并清空 fieldCache
inline void CCsvParser::PushField()
{
    rowCache.push_back(fieldCache);
    fieldCache.clear();
}

// 压入一行记录
inline void CCsvParser::PushRow()
{
    table.push_back(rowCache);
    rowCache.clear();
}

// 压入一个字符，返回下个状态
int CCsvParser::PushChar(char ch)
{
    // CSV_ENCLOSURE_NONE " CSV_ENCLOSURE_ENTER " CSV_ENCLOSURE_EXIT
    if ((delimiterChar == ch) && (CSV_ENCLOSURE_ENTER != quoteStatus))
    {
        PushField();
        return CSV_ENCLOSURE_NONE;
    }

    if (enclosureChar == ch)
    {
        if (CSV_ENCLOSURE_EXIT == quoteStatus)
        {
            fieldCache.push_back(ch); 
            return CSV_ENCLOSURE_ENTER;
        }
        // 状态转移到下一个状态码
        // CSV_ENCLOSURE_NONE -> CSV_ENCLOSURE_ENTER
        // CSV_ENCLOSURE_ENTER -> CSV_ENCLOSURE_EXIT
        return quoteStatus + 1;
    }

    fieldCache.push_back(ch); 
    if (CSV_ENCLOSURE_EXIT == quoteStatus)
        return CSV_ENCLOSURE_NONE;
    return quoteStatus;
}

// 解析一行字符
void CCsvParser::ParseLine(const std::string& line)
{
    // Parse line to record
    for (size_t i = 0; i < line.size(); i++)
        quoteStatus = PushChar(line[i]);

    // Handle last field
    if (CSV_ENCLOSURE_ENTER != quoteStatus)  // Record finished
    {
        PushField();
        PushRow();
        return;
    }

    // Record continue. Push '\n'.
    fieldCache.push_back('\n');
}
