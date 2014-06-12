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


int CCsvParser::RecordStatus(int preStatus, char ch)
{
	if ( 0 == m_transferMap.count(preStatus) )
	{
		return CSV_RECORD_STATUS_ERROR_BUTT;
	}
	return m_transferMap[preStatus](ch);
}

bool isTextData(int status)
{
    return (CSV_RECORD_STATUS_ESCAPED_FIELD == status) || (CSV_RECORD_STATUS_NON_ESCAPED_FIELD == status);
}

bool IsInnerFieldEnd(int status)
{
    return (CSV_RECORD_STATUS_FIELD_END == status);
}

bool IsLastFieldEnd(int status)
{
    return (CSV_RECORD_STATUS_ESCAPED_SUB == status)
		|| (CSV_RECORD_STATUS_NON_ESCAPED_FIELD == status)
		|| (CSV_RECORD_STATUS_FIELD_END == status);
}

void FieldEnd(std::vector<std::string>& record, std::string& field)
{
    // std::cout << field << "  ||  ";
    record.push_back(field);
    field.clear();
}

bool CCsvParser::open(std::string filename)
{
    fin.open(filename, std::ios::in);
    return fin.is_open();
}

int CCsvParser::parser(void (*RecordHandler)(std::vector<std::string>&, int))
{
    int recordCount = 0; // count without header
    std::string lineData;
    std::vector<std::string> record;
    std::string field;
	int curStatus = CSV_RECORD_STATUS_INIT;

    while(std::getline(fin, lineData))
    {
        //recordCount++;
        // parser line to record
        for (size_t i =0; i < lineData.size(); i++)
        {
            curStatus = RecordStatus(curStatus, lineData[i]);
            if (isTextData(curStatus))
            {
                field.push_back(lineData[i]);
            }
            else if (IsInnerFieldEnd(curStatus))
            {
                FieldEnd(record, field);
            }
        }
        // line end, deal with last field
		if ( IsLastFieldEnd(curStatus) )
		{
			recordCount++;
			FieldEnd(record, field);
			RecordHandler(record, recordCount);
			record.clear();
			curStatus = CSV_RECORD_STATUS_INIT;
		}
		else
		{
			field.push_back(16);
			field.push_back(10);
		}
    }
	if (curStatus != CSV_RECORD_STATUS_INIT)
	{
		std::cout << "CSV format error!" << std::endl;
	}
    fin.close();  // TODO
    return recordCount;
}