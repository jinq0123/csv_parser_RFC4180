#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "csv_parser.h"

void test(const std::string& data,
	size_t row, size_t col,
	const std::string expect)
{
	CCsvParser parser;
	std::istringstream iss(data);
	parser.Parse(iss);
	std::string result = parser.GetGrid(row, col);
	assert(expect == result);
}

void run_tests()
{
	test("12,34,56", 0, 2, "56");
	test("\"ab\nc\"", 0, 0, "ab\nc");
	test("\"ab\r\nc\"", 0, 0, "ab\r\nc");
	test("a\nb", 0, 0, "a");
	test("a\nb", 1, 0, "b");
	// test("a\r\nb", 0, 0, "a");
	// test("a\r\nb", 1, 0, "b");
	test(R"(abc)", 0, 0, "abc");

	// test from https://en.wikipedia.org/wiki/Comma-separated_values
	test(R"(1997,Ford,E350)", 0, 2, "E350");
	test(R"("1997","Ford","E350")", 0, 2, "E350");
	test(R"(1997,Ford,E350,"Super, luxurious truck")",
	                 0, 3, "Super, luxurious truck");
	test(R"(1997,Ford,E350,"Super, ""luxurious"" truck")",
	               0, 3, R"(Super, "luxurious" truck)");

	test(R"(1997,Ford,E350,"Go get one now
they are going fast")",
	               0, 3, R"(Go get one now
they are going fast)");

	test(R"(1997, Ford, E350)", 0, 1, " Ford");
	// ? test(R"(1997, "Ford" ,E350)", 0, 1, R"( "Ford" )");
	test(R"(1997,Ford,E350," Super luxurious truck ")",
	                 0, 3, " Super luxurious truck ");

	test(R"(Los Angeles,34°„03°‰N,118°„15°‰W
New York City,40°„42°‰46°ÂN,74°„00°‰21°ÂW
Paris,48°„51°‰24°ÂN,2°„21°‰03°ÂE)",
       2, 2, R"(2°„21°‰03°ÂE)");
}

int main()
{
	run_tests();

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

	std::cin.get();
	return 0;
}
