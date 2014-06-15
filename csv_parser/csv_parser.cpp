#include <iostream>
#include <vector>

#include "csv_parser.h"

typedef enum CSV_PARSER_STATUS_ENUM
{
    CSV_RECORD_SWITCH,
    CSV_ITEM_SWITCH,
    CSV_ENCLOSURE_NONE,
    CSV_ENCLOSURE_ENTER,
    CSV_ENCLOSURE_INNER,
    CSV_ENCLOSURE_PAUSE,
    CSV_PARSER_STATUS_ERROR_BUTT
}CSV_PARSER_STATUS_ENUM;
typedef int (*TRANS_FUNC)(char);
typedef void (*INNER_ACTION_FUNC)(char ch, std::string& item, std::vector<std::string>& record);

// status transfer
inline int TransItemSwitch(char ch)
{
	switch ( ch )
	{
	case ENCLOSURE_CHAR:
		return CSV_ENCLOSURE_ENTER;
	case DELIMITER_CHAR:
		return CSV_ITEM_SWITCH;
	default:
		return CSV_ENCLOSURE_NONE;
	}
}
inline int TransRecordSwitch(char ch)
{
	return TransItemSwitch(ch);
}
inline int transEnclosureNone(char ch)
{
	return TransItemSwitch(ch);
}
inline int transEnclosureEnter(char ch)
{
	switch ( ch )
	{
	case ENCLOSURE_CHAR:
		return CSV_ENCLOSURE_PAUSE;
	default:
		return CSV_ENCLOSURE_INNER;
	}
}
inline int transEnclosureInner(char ch)
{
	return transEnclosureEnter(ch);
}
inline int transEnclosurePause(char ch)
{
	switch ( ch )
	{
	case ENCLOSURE_CHAR:
		return CSV_ENCLOSURE_INNER;
	case DELIMITER_CHAR:
		return CSV_ITEM_SWITCH;
	default:
		return CSV_ENCLOSURE_NONE;
	}
}

TRANS_FUNC transFunc[] = 
{
	TransRecordSwitch,
	TransItemSwitch,
	transEnclosureNone,
	transEnclosureEnter,
	transEnclosureInner,
	transEnclosurePause
};

inline void UdateStatus(int& status, char ch)
{
	if ( (status < 0) || (status >= CSV_PARSER_STATUS_ERROR_BUTT) )
	{
		status = CSV_PARSER_STATUS_ERROR_BUTT;
	}
	else
	{
		status = transFunc[status](ch);
	}
}

inline void ActionItemSwitch(char ch, std::string& item, std::vector<std::string>& record)
{
	record.push_back(item);
    item.clear();
}
inline void ActionEnclosureNone(char ch, std::string& item, std::vector<std::string>& record)
{
	item.push_back(ch);
}
inline void ActionEnclosureInner(char ch, std::string& item, std::vector<std::string>& record)
{
	item.push_back(ch);
}
inline void ActionEnclosureEnter(char ch, std::string& item, std::vector<std::string>& record)
{
	// Do nothing
}
inline void ActionEnclosurePause(char ch, std::string& item, std::vector<std::string>& record)
{
	// Do nothing
}

INNER_ACTION_FUNC innerActionFunc[] =
{
	0,  // unreachable
	ActionItemSwitch,
	ActionEnclosureNone,
	ActionEnclosureEnter,
	ActionEnclosureInner,
	ActionEnclosurePause
};

inline void InnerStatusAction(int status, char ch, std::string& item, std::vector<std::string>& record)
{
	if ( (status > 0) && (status < CSV_PARSER_STATUS_ERROR_BUTT) )
	{
		innerActionFunc[status](ch, item, record);
	}
	// else // Do nothing.
}

// return: true -- record end, false -- record continue
inline bool HandleLineEnd(int& status, std::string& item, std::vector<std::string>& record)
{
	if ( (CSV_RECORD_SWITCH == status))
	{
		// Empty line
	}
	else if ( (CSV_ENCLOSURE_ENTER == status) || (CSV_ENCLOSURE_INNER == status) )
	{
		status = CSV_ENCLOSURE_INNER;  // Update status
		item.push_back(LF_CHAR);
		item.push_back(CR_CHAR);
	}
	else
	{
		status = CSV_RECORD_SWITCH;  // Update status
		ActionItemSwitch(0, item, record);
	}
	return (CSV_RECORD_SWITCH == status);
}

bool CCsvParser::Init(std::string filename)
{
    fin.open(filename, std::ios::in);
    return fin.is_open();
}

int CCsvParser::parser(void (*RecordHandler)(std::vector<std::string>&, int))
{
    int recordCount = 0; // count without header
    std::string lineData;
    std::vector<std::string> record;
    std::string item;
	int curStatus = CSV_RECORD_SWITCH;

    while(std::getline(fin, lineData))
    {
        // Inner line data
        for (size_t i =0; i < lineData.size(); i++)
        {
            UdateStatus(curStatus, lineData[i]);
			InnerStatusAction(curStatus, lineData[i], item, record);
        }
        // line end, LF CR
		bool isRecordEnd = HandleLineEnd(curStatus, item, record);
		if ( isRecordEnd )
		{
			recordCount++;
			RecordHandler(record, recordCount);
			record.clear();
		}
    }
	if (curStatus != CSV_RECORD_SWITCH)
	{
		std::cout << "CSV format error!" << std::endl;
	}
    fin.close();  // TODO
    return recordCount;
}