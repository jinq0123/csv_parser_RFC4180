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
	test("12,34,56", 0, 3, "");
	test("12,34,56", 1, 2, "");
	test("\"ab\nc\"", 0, 0, "ab\nc");
	test("\"ab\r\nc\"", 0, 0, "ab\r\nc");
	test("a\nb", 0, 0, "a");
	test("a\nb", 1, 0, "b");
	test("a\r\nb", 0, 0, "a");
	test("a\r\nb", 1, 0, "b");
	const std::string BOM("\xEF\xBB\xBF");
	test(BOM + "a\nb", 0, 0, "a");
	test(BOM + "a\nb", 1, 0, "b");
	test("a\n" + BOM + "b", 1, 0, BOM + "b");
	test("a\ra\nb", 0, 0, "a\ra");
	test("a\ra\nb", 1, 0, "b");

	test(R"(a"b)", 0, 0, "");  // 字段内有双引号，试图连接下行
	test(R"(a"
b)", 0, 0, "");  // 字段内有双引号，试图连接下行

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

	test(R"(Los Angeles,34°03′N,118°15′W
New York City,40°42′46″N,74°00′21″W
Paris,48°51′24″N,2°21′03″E)",
       2, 2, R"(2°21′03″E)");
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
