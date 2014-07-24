#include <iostream>
#include <vector>

#include "csv_parser.h"


// CSV 解析状态 -- 取决于 ENCLOSURE_CHAR(") 的个数
typedef enum CSV_ENCLOSURE_STATUS_ENUM
{
    CSV_ENCLOSURE_NONE,
    CSV_ENCLOSURE_ENTER,
    CSV_ENCLOSURE_EXIT
}CSV_ENCLOSURE_STATUS_ENUM;


// 当前 fieldCache 缓存的内容压入 record 并清空 fieldCache
inline void CCsvParser::FieldHandler(std::vector<std::string>& record)
{
    record.push_back(fieldCache);
    fieldCache.clear();
}


// 打开 csv 文件
bool CCsvParser::Init(std::string filename)
{
    fin.open(filename, std::ios::in);
    return fin.is_open();
}

CCsvParser::~CCsvParser()
{
//    if (fin)
//    {
//        fin.close();
//    }
}

int CCsvParser::StateAction(char ch, int quote_status, std::vector<std::string>& record)
{
    // CSV_ENCLOSURE_NONE " CSV_ENCLOSURE_ENTER " CSV_ENCLOSURE_EXIT
    if ( (DELIMITER_CHAR == ch) && (CSV_ENCLOSURE_ENTER != quote_status) )
    {
        FieldHandler(record);
        return CSV_ENCLOSURE_NONE;
    }
    
    if (ENCLOSURE_CHAR == ch)
    {
        if (CSV_ENCLOSURE_EXIT == quote_status)
        {
            fieldCache.push_back(ch); 
            return CSV_ENCLOSURE_ENTER;
        }
        else
        {
            // 状态转移到下一个状态码
            // CSV_ENCLOSURE_NONE -> CSV_ENCLOSURE_ENTER
            // CSV_ENCLOSURE_ENTER -> CSV_ENCLOSURE_EXIT
            return quote_status + 1;
        }
    }

    fieldCache.push_back(ch); 
    return quote_status % CSV_ENCLOSURE_EXIT;  // if CSV_ENCLOSURE_EXIT, return CSV_ENCLOSURE_NONE
}

bool CCsvParser::ParseRecord(std::vector<std::string>& record)
{
    int quote_status = CSV_ENCLOSURE_NONE;

    while(std::getline(fin, lineData))
    {
        // Parse line to record
        for (size_t i =0; i < lineData.size(); i++)
        {
            quote_status = StateAction(lineData[i], quote_status, record);
        }

        // Handle last field
        if (CSV_ENCLOSURE_ENTER != quote_status)  // Record finished
        {
            FieldHandler(record);
            return true; 
        }
        // Record Cintinued, push <CR><LF>
        fieldCache.push_back(CR_CHAR);
        fieldCache.push_back(LF_CHAR);        
    }
    return false;  // File End
}

// return   CSV 中 record 个数，不包含 header。
// 如果解析失败，返回 -1
// CSV Record 不能超过 int 最大数
int CCsvParser::Parse(void (*RecordHandler)(std::vector<std::string>&, int))
{
    if ( !ParseRecord(header) )
    {
        return -1;
    }

    int recordCount = 0;
    while( ParseRecord(recordCache) )
    {
        recordCount++;
        RecordHandler(recordCache, recordCount);
        recordCache.clear();
    }

    return recordCount;
}