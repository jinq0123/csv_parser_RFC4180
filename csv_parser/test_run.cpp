#include <iostream>
#include <string>
#include <vector>

#include "csv_parser.h"

void RecordHandler(std::vector<std::string>& record, int sequence)
{
    //if ( 0 == (sequence % 10000) )
    {
        std::cout << "Records " << sequence << ": " << std::endl;
		for (size_t i = 0; i < record.size(); i++)
		{
			std::cout << record[i] << " || ";
		}
		std::cout << std::endl << std::endl;
    }
}

int main()
{
	CCsvParser parser;
	if (parser.Init("test_data.csv"))
	{
		std::cout << "Record Number: " << parser.Parse(RecordHandler) << std::endl;
	}
	// check output
	std::string ss;
	std::cin >> ss;
	return 0;
}