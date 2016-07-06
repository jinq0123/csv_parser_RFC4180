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
    void Reset();
    void Parse(const std::string& file_path);
    void Parse(std::istream& is);

public:
    size_t GetRowCount() const { return table.size(); }
    size_t GetColCount() const { return GetRow(0).size(); }

    using Row = std::vector<std::string>;
    // 返回一行记录。索引从0开始。记录头也按记录返回。索引过界则返回空行。
    const Row& GetRow(size_t row_index) const
    {
        if (row_index < table.size())
            return table[row_index];
        return emptyRow;
    }
    // 返回指定格子内容。索引从0开始。索引过界则返回空。
    const std::string& GetGrid(size_t row_index, size_t col_index) const
    {
        const Row& row = GetRow(row_index);
        if (col_index < row.size())
            return row[col_index];
        return emptyField;
    }

private:
    int PushChar(char ch);
    void ParseLine(const std::string& line);
    inline void PushField();
    void FinishRow();

private:
    char delimiterChar = ',';  // COMMA ,
    char enclosureChar = '"';  // DQUOTE "

private:
    using Table = std::vector<Row>;
    Table table;

private:
    static const Row emptyRow;
    static const std::string emptyField;

private:
    int quoteStatus = 0;
    Row rowCache;
    std::string fieldCache;
};  // class CCsvParser

#endif
