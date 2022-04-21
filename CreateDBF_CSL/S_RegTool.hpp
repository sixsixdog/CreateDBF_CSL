#ifndef __S_REG_TOOL_H__
#define __S_REG_TOOL_H__
#include <regex>

using namespace std;

smatch rmatch(string str, string r)
{
	smatch result;
	regex regstr(r,regex::icase);
	auto matched = regex_match(str, result, regstr);
	return result;
}

string rreplace(string partern, string substr,string replace)
{
	return regex_replace(partern, regex(substr, regex::icase), replace);
}



#endif // !__S_REG_TOOL_H__
