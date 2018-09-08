#ifndef _TOOL_H_
#define _TOOL_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <map>

using namespace std;

namespace strtool
{
	string trim(const string& str);

	int split(const string& str, vector<string>& ret_, const string& sep = ",");

	string replace(const string& str, const string& src, const string& dest);

	int StringToInt(const string& strInput);

	template <typename Type>
	string IntToString(Type tmp)
	{
		string strOut;

		stringstream stream;
		stream<< tmp;
		strOut = stream.str();

		return strOut;
	}
}

namespace timetool
{
    long long GetTimeStamp(const string& strTime);
}

#endif
