#include <iostream>
#include <string>
#include <vector>

#include "csv_parser.h"

int main()
{
	CCsvParser parser;
	parser.Parse("test_data.csv");
	std::cout << "Row count: " << parser.GetRowCount() << std::endl;
	std::cout << "Col count: " << parser.GetColCount() << std::endl;
	for (size_t i = 0; i < parser.GetRowCount(); i++)
	{
		const std::vector<std::string>& row = parser.GetRow(i);
		for (size_t j = 0; j < row.size(); j++)
			std::cout << row[j] << " || ";
		std::cout << std::endl;
	}

	// check output
	std::string ss;
	std::cin >> ss;
	return 0;
}
