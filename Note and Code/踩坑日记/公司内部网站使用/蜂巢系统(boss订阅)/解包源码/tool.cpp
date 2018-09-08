#include "tool.h"

using namespace std;

string strtool::trim(const string& str)
{
	string::size_type pos = str.find_first_not_of(' ');
	if (pos == string::npos)
	{
		return str;

	}
	string::size_type pos2 = str.find_last_not_of(' ');
	if (pos2 != string::npos)
	{
		return str.substr(pos, pos2 - pos + 1);

	}
	return str.substr(pos);
}

int strtool::split(const string& str, vector<string>& ret_, const string& sep)
{
	if (str.empty())
	{
		return 0;
	}

	string tmp;
	string::size_type pos_begin = str.find_first_not_of(sep);
	string::size_type comma_pos = 0;

	while (pos_begin != string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
	return 0;
}

string strtool::replace(const string& str, const string& src, const string& dest)
{
	string ret;

	string::size_type pos_begin = 0;
	string::size_type pos = str.find(src);
	while (pos != string::npos)
	{
		cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";
		ret.append(str.data() + pos_begin, pos - pos_begin);
		ret += dest;
		pos_begin = pos + 1;
		pos = str.find(src, pos_begin);
	}
	if (pos_begin < str.length())
	{
		ret.append(str.begin() + pos_begin, str.end());

	}
	return ret;
}

int strtool::StringToInt(const string& strInput)
{
	int iOut = 0;

	stringstream stream(strInput);
	stream>> iOut;

	return iOut;
}

/**
 * @brief 将指定格式输入的时间(2018-01-08 12:20:23)转换成时间戳
 * @param strTime
 * @return 
 */
long long timetool::GetTimeStamp(const string& strTime)
{
    static map<string, long long> mapTime;
    if (mapTime.find(strTime) != mapTime.end())
    {
        return mapTime[strTime];
    }

    int iYear, iMon, iDay, iHour, iMin, iSec;
    sscanf(strTime.c_str(), "%d-%d-%d %d:%d:%d", &iYear, &iMon, &iDay, &iHour, &iMin, &iSec);

    struct tm stTm;
    memset(&stTm, 0, sizeof(stTm));
    strptime(strTime.c_str(), "%F %H:%M:%S", &stTm);
    stTm.tm_isdst = 0;  //这个不为0，mktime出来的时间会比time出来的少一个小时 

#if 0
    stTm.tm_year = iYear-1900;
    stTm.tm_mon  = iMon-1;
    stTm.tm_mday = iDay;
    stTm.tm_hour = iHour;
    stTm.tm_min  = iMin;
    stTm.tm_sec  = iSec;
//    printf("输入: year[%d], month[%d], day[%d], hour[%d], minute[%d], sec[%d]\n", 
 //           1900+stTm.tm_year, 1+stTm.tm_mon, stTm.tm_mday                            
  //        , stTm.tm_hour, stTm.tm_min, stTm.tm_sec);

    if ((iMon <0 || iMon > 12) || (iDay <0 || iDay > 31) || (iHour < 0 || iHour >= 24) || (iMin < 0 || iMin >= 60) || (iSec < 0 || iSec >= 60))
    {
        return -1;
    }
    //return mktime(&stTm);
#endif

    return mapTime[strTime]=mktime(&stTm);
}

