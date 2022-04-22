#ifndef __S_REG_TOOL_H__
#define __S_REG_TOOL_H__
#include <regex>

using namespace std;

bool rmatch(string &str,smatch &matched, string r)
{
	regex regstr(r,regex::icase);
	return regex_match(str, matched, regstr);
}
bool rmatch(string& str, string r)
{
	regex regstr(r, regex::icase);
	return regex_match(str, regstr);
}

bool rsearch(string &str,smatch &matched, string r)
{
	regex regstr(r,regex::icase);
	return regex_search(str, matched, regstr);
}

bool rsearch(string& str, string r)
{
	regex regstr(r, regex::icase);
	return regex_search(str, regstr);
}

string rreplace(string partern, string substr,string replace)
{
	return regex_replace(partern, regex(substr, regex::icase), replace);
}



#endif // !__S_REG_TOOL_H__
