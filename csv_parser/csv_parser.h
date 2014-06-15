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
const char DELIMITER_CHAR = 0x2c;      // COMMA ,
const char ENCLOSURE_CHAR = 0x22;      // DQUOTE "
const char LF_CHAR = 0x0A;
const char CR_CHAR = 0x0D;


class CCsvParser
{
public:
    //CCsvParser();
    //~CCsvParser();

    bool Init(std::string filename);
    int parser(void (*RecordHandler)(std::vector<std::string>&, int));
private:
    std::ifstream fin;
};
#endif
