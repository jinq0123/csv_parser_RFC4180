#include "csv_parser.h"

#include <fstream>

const CCsvParser::Row CCsvParser::emptyRow;
const std::string CCsvParser::emptyField;

// CSV 解析状态 -- 取决于 ENCLOSURE_CHAR(") 的个数
enum
{
    CSV_ENCLOSURE_NONE,
    CSV_ENCLOSURE_ENTER,
    CSV_ENCLOSURE_EXIT
};

void CCsvParser::Reset()
{
    table.clear();
    quoteStatus = CSV_ENCLOSURE_NONE;
    rowCache.clear();
    fieldCache.clear();
    parseError.clear();
}

bool CCsvParser::Parse(const std::string& file_path)
{
    // Todo: return open error.
    // 总是以binary打开，防止Windows下替换"\r\n".
    return Parse(std::ifstream(file_path, std::ifstream::binary));
}

void IgnoreUtf8BOM(std::string& line)
{
    if ("\xEF\xBB\xBF" == line.substr(0, 3))
        line = line.substr(3);
}

bool CCsvParser::Parse(std::istream& is)
{
    Reset();
    std::string line;
    bool first_line = true;
    while (std::getline(is, line))
    {
        if (first_line)
        {
            first_line = false;
            IgnoreUtf8BOM(line);
        }
        ParseLine(line);
    }
    // Todo: return read error
    return parseError.empty();
}

// 当前 fieldCache 缓存的内容压入 record 并清空 fieldCache
inline void CCsvParser::PushField()
{
    rowCache.push_back(fieldCache);
    fieldCache.clear();
}

// 结束时压入一行记录
void CCsvParser::FinishRow()
{
    // 允许以"\r\n"分行，最后一个字段可能有'\r'
    if (!fieldCache.empty() && '\r' == *fieldCache.rbegin())
        fieldCache.resize(fieldCache.size() - 1);

    PushField();
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
    if (CSV_ENCLOSURE_ENTER != quoteStatus)  // Row finished
    {
        FinishRow();
        return;
    }

    // '\n' in field. Push '\n'.
    fieldCache.push_back('\n');
}
