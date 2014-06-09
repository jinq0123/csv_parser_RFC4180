#include <iostream>
#include <string>
#include <vector>

#include "csv_parser.h"

void RecordHandler(std::vector<std::string>& record, int sequence)
{
    //if ( 0 == (sequence % 10000) )
    {
        std::cout << std::endl << "Records: " << sequence << std::endl;
    }
    record.clear();
}

int main()
{
	CCsvParser parser;
	if (parser.open("test_data.csv"))
	{
		std::cout << parser.parser(RecordHandler) << std::endl;
	}
	// check output
	std::string ss;
	std::cin >> ss;
	return 0;
}