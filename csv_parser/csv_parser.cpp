#include "csv_parser.h"

#include <fstream>

const CCsvParser::Row CCsvParser::emptyRow;

// CSV ����״̬ -- ȡ���� ENCLOSURE_CHAR(") �ĸ���
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

// ��ǰ fieldCache ���������ѹ�� record ����� fieldCache
inline void CCsvParser::PushField()
{
    rowCache.push_back(fieldCache);
    fieldCache.clear();
}

// ѹ��һ�м�¼
inline void CCsvParser::PushRow()
{
    table.push_back(rowCache);
    rowCache.clear();
}

// ѹ��һ���ַ��������¸�״̬
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
        // ״̬ת�Ƶ���һ��״̬��
        // CSV_ENCLOSURE_NONE -> CSV_ENCLOSURE_ENTER
        // CSV_ENCLOSURE_ENTER -> CSV_ENCLOSURE_EXIT
        return quoteStatus + 1;
    }

    fieldCache.push_back(ch); 
    if (CSV_ENCLOSURE_EXIT == quoteStatus)
        return CSV_ENCLOSURE_NONE;
    return quoteStatus;
}

// ����һ���ַ�
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
