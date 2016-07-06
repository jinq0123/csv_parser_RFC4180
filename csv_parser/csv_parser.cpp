#include "csv_parser.h"

#include <fstream>

const CCsvParser::Row CCsvParser::emptyRow;
const std::string CCsvParser::emptyField;

// CSV ����״̬ -- ȡ���� ENCLOSURE_CHAR(") �ĸ���
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
}

void CCsvParser::Parse(const std::string& file_path)
{
    // ������binary�򿪣���ֹWindows���滻"\r\n".
    Parse(std::ifstream(file_path, std::ifstream::binary));
}

void IgnoreUtf8BOM(std::string& line)
{
    if ("\xEF\xBB\xBF" == line.substr(0, 3))
        line = line.substr(3);
}

void CCsvParser::Parse(std::istream& is)
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
}

// ��ɵ�ǰ�ֶ�
inline void CCsvParser::PushField()
{
    rowCache.push_back(fieldCache);
    fieldCache.clear();
}

// ����ʱѹ��һ�м�¼
void CCsvParser::FinishRow()
{
    // ������"\r\n"���У����һ���ֶο�����'\r'
    if (!fieldCache.empty() && '\r' == *fieldCache.rbegin())
        fieldCache.resize(fieldCache.size() - 1);

    PushField();
    table.push_back(rowCache);
    rowCache.clear();
}

// ѹ��һ���ַ��������¸�״̬
int CCsvParser::PushChar(char ch)
{
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
        // CSV_ENCLOSURE_NONE -> CSV_ENCLOSURE_ENTER -> CSV_ENCLOSURE_EXIT
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
    // Parse line to row.
    for (size_t i = 0; i < line.size(); i++)
        quoteStatus = PushChar(line[i]);

    // Row finish.
    if (CSV_ENCLOSURE_ENTER != quoteStatus)
    {
        FinishRow();
        return;
    }

    // '\n' in field. Push '\n'.
    fieldCache.push_back('\n');
}
