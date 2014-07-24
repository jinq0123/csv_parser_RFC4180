/**
 * csv_parser.h
 *
 * @author Jackon Yang <jiekunyang@gmail.com>
 */

#ifndef _CSV_PARSER_H_
#define _CSV_PARSER_H_
#include <string>
#include <fstream>
#include <map>

/**
 * @li DEC is how it would be represented in decimal form (base 10)
 * @li HEX is how it would be represented in hexadecimal format (base 16)
 *
 * @li    DEC   HEX     Character Name
 * @li    10    0x0A    LF (NL line feed, new line)
 * @li    13    0x0D    CR (carriage return)
 * @li    32    0x20    space
 * @li    34    0x22    double quote  "
 * @li    44    0x2C    comma  ,
*/

class CCsvParser
{
public:
    bool Init(std::string filename);
    int Parse(void (*RecordHandler)(std::vector<std::string>&, int));
    CCsvParser(char delemiter=0x2c, char enclosure=0x22, char LF=0x0A, char CR=0x0D)
        :DELIMITER_CHAR(delemiter), ENCLOSURE_CHAR(enclosure), LF_CHAR(LF), CR_CHAR(CR)
    {}
    ~CCsvParser();
private:
    bool ParseRecord(std::vector<std::string>& record);
    int StateAction(char ch, int quote_status, std::vector<std::string>& record);
    void FieldHandler(std::vector<std::string>& record);

    std::vector<std::string> header;

    std::ifstream fin;
    const char DELIMITER_CHAR;      // COMMA ,
    const char ENCLOSURE_CHAR;      // DQUOTE "
    const char LF_CHAR;             // <LF>
    const char CR_CHAR;             // <CR>

    // 避免在循环内重复声明、定义变量
    std::string lineData;
    std::vector<std::string> recordCache;
    std::string fieldCache;
};
#endif