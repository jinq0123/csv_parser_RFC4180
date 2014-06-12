#include <iostream>
#include <vector>

#include "csv_parser.h"

int TransferInit(char ch)
{
    switch ( ch )
    {
    case ESCAPE:
        return CSV_RECORD_STATUS_ESCAPED_PRE;
    case DELIMITER:
        return CSV_RECORD_STATUS_FIELD_END;
    default:  // TEXTDATA
        return CSV_RECORD_STATUS_NON_ESCAPED_FIELD;
     }
}

int TransferNonEscapedField(char ch)
{
    switch ( ch )
    {
    case DELIMITER:
        return CSV_RECORD_STATUS_FIELD_END;
    case ESCAPE:
        return CSV_RECORD_STATUS_ESCAPED_PRE;
    default:
        return CSV_RECORD_STATUS_NON_ESCAPED_FIELD;
    }
}

int TransferEscapedField(char ch)
{
    switch ( ch )
    {
    case ESCAPE:
        return CSV_RECORD_STATUS_ESCAPED_SUB;
    default:
        return CSV_RECORD_STATUS_ESCAPED_FIELD;
    }
}

int TransferEscapedPre(char ch)
{
    switch ( ch )
    {
    case ESCAPE:
        return CSV_RECORD_STATUS_ESCAPED_SUB;
    default:
        return CSV_RECORD_STATUS_ESCAPED_FIELD;
    }
}

int TransferEscapedSub(char ch)
{
    switch ( ch )
    {
    case DELIMITER:
        return CSV_RECORD_STATUS_FIELD_END;
    case ESCAPE:
        return CSV_RECORD_STATUS_ESCAPED_FIELD;
    default:
        return CSV_RECORD_STATUS_NON_ESCAPED_FIELD;
    }
}

int TransferFieldEnd(char ch)
{
    switch ( ch )
    {
    case ESCAPE:
        return CSV_RECORD_STATUS_ESCAPED_PRE;
    case DELIMITER:
        return CSV_RECORD_STATUS_FIELD_END;
    default:
        return CSV_RECORD_STATUS_NON_ESCAPED_FIELD;
     }
}


TransferFuncMap::value_type init_value[] = 
{
	TransferFuncMap::value_type(CSV_RECORD_STATUS_INIT, TransferInit),
	TransferFuncMap::value_type(CSV_RECORD_STATUS_NON_ESCAPED_FIELD, TransferNonEscapedField),
	TransferFuncMap::value_type(CSV_RECORD_STATUS_ESCAPED_PRE, TransferEscapedPre),
	TransferFuncMap::value_type(CSV_RECORD_STATUS_ESCAPED_SUB, TransferEscapedSub),
	TransferFuncMap::value_type(CSV_RECORD_STATUS_ESCAPED_FIELD, TransferEscapedField),
	TransferFuncMap::value_type(CSV_RECORD_STATUS_FIELD_END, TransferFieldEnd)
};
TransferFuncMap CCsvParser::m_transferMap(init_value, init_value + FUNC_NUM);


int RecordStatus(int preStatus, char ch)
{
	return 0;
}

bool isTextData(int status)
{
    return (CSV_RECORD_STATUS_ESCAPED_FIELD == status) || (CSV_RECORD_STATUS_NON_ESCAPED_FIELD == status);
}

bool isFieldEnd(int status)
{
    return (CSV_RECORD_STATUS_FIELD_END == status);
}

void FieldEnd(std::vector<std::string>& record, std::string& field)
{
    std::cout << field << "  ||  ";
    record.push_back(field);
    field.clear();
}

bool LastFieldEnd(int status, std::vector<std::string>& record, std::string& field)
{
    // error check
    if ( (CSV_RECORD_STATUS_ESCAPED_SUB != status) && (CSV_RECORD_STATUS_NON_ESCAPED_FIELD != status) )
    {
        return false;
    }
    FieldEnd(record, field);
    return true;
}

bool CCsvParser::open(std::string filename)
{
    fin.open(filename, std::ios::in);
    return fin.is_open();
}

int CCsvParser::parser(void (*RecordHandler)(std::vector<std::string>&, int))
{
    std::string header;
    std::getline(fin, header);
    // TODO: deal with header

    int recordCount = 0; // count without header
    std::string lineData;
    std::vector<std::string> record;
    std::string field;
    bool newField = true;
    while(std::getline(fin, lineData))
    {
        recordCount++;
        int curStatus = CSV_RECORD_STATUS_FIELD_END;
        record.clear();
        field.clear();
        // parser line to record
        // std::cout << lineData << std::endl;
        for (size_t i =0; i < lineData.size(); i++)
        {
            curStatus = RecordStatus(curStatus, lineData[i]);
            if (isTextData(curStatus))
            {
                field.push_back(lineData[i]);
            }
            else if (isFieldEnd(curStatus))
            {
                FieldEnd(record, field);
            }
        }
        // line end, deal with last field
        LastFieldEnd(curStatus, record, field);
        RecordHandler(record, recordCount);
    }
    fin.close();  // TODO
    return recordCount;
}
