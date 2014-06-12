#ifndef _CSV_PARSER_H_
#define _CSV_PARSER_H_
#include <string>
#include <fstream>
#include <map>

const char DELIMITER = ',';  // COMMA
const char ESCAPE = '"';      // QUOTE

typedef enum CSV_RECORD_STATUS
{
    CSV_RECORD_STATUS_INIT = 1001,
    CSV_RECORD_STATUS_ESCAPED_PRE,
    CSV_RECORD_STATUS_ESCAPED_SUB,
    CSV_RECORD_STATUS_ESCAPED_FIELD,
    CSV_RECORD_STATUS_NON_ESCAPED_FIELD,
    CSV_RECORD_STATUS_FIELD_END,
    CSV_RECORD_STATUS_ERROR_BUTT  // no transfer func
}CSV_RECORD_STATUS;

const int FUNC_NUM = 6;
typedef int (*TransferFunc)(char);
typedef std::map<CSV_RECORD_STATUS, TransferFunc> TransferFuncMap;

class CCsvParser
{
public:
	bool open(std::string filename);
	int parser(void (*RecordHandler)(std::vector<std::string>&, int));
private:
	std::ifstream fin;
	static TransferFuncMap m_transferMap;
};
#endif