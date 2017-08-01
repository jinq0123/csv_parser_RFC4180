#include <cassert>
#include "csv_parser/csv_parser.h"

int main() {
    CCsvParser parser;
    assert(0 == parser.GetRowCount());
    return 0;
}
