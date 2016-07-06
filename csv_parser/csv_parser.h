/**
 * csv_parser.h
 *
 * @author Jackon Yang <jiekunyang@gmail.com>
 * @author Jin Qing (http://blog.csdn.net/jq0123)
 */

#ifndef _CSV_PARSER_H_
#define _CSV_PARSER_H_

#include <iosfwd>  // for istream
#include <string>
#include <vector>

class CCsvParser
{
public:
    CCsvParser() {}
    virtual ~CCsvParser() {};

public:
    void SetDelimiterChar(char delimiter) { delimiterChar = delimiter; }
    void SetEnclosureChar(char enclosure) { enclosureChar = enclosure; }

public:
    void Parse(const std::string& file_path);
    void Parse(std::istream& is);

public:
    size_t GetRowCount() const { return table.size(); }
    size_t GetColCount() const { return GetRow(0).size(); }

    using Row = std::vector<std::string>;
    // ����һ�м�¼��������0��ʼ����¼ͷҲ����¼���ء����������򷵻ؿ��С�
    const Row& GetRow(size_t row_index) const
    {
        if (row_index < table.size())
            return table[row_index];
        return emptyRow;
    }

private:
    int PushChar(char ch);
    void ParseLine(const std::string& line);
    inline void PushField();
    inline void PushRow();

private:
    char delimiterChar = ',';  // COMMA ,
    char enclosureChar = '"';  // DQUOTE "

private:
    using Table = std::vector<Row>;
    Table table;

private:
    static const Row emptyRow;

private:
    int quoteStatus = 0;
    Row rowCache;
    std::string fieldCache;
};  // class CCsvParser

#endif
