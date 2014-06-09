#ifndef _CSV_PARSER_H_
#define _CSV_PARSER_H_
#include <string>
#include <fstream>

const char DELIMITER = ',';  // COMMA
const char QUOTE = '"';      // ESCAPE

class CCsvParser
{
public:
	bool open(std::string filename);
	int parser(void (*RecordHandler)(std::vector<std::string>&, int));
private:
	std::ifstream fin;
};
#endif