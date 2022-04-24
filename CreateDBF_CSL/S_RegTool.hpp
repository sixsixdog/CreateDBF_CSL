#ifndef __S_REG_TOOL_H__
#define __S_REG_TOOL_H__
#include <regex>

using namespace std;

//完全匹配字符串以smatch返回结果
inline bool rmatch(string &str,smatch &matched, string r)
{
	regex regstr(r,regex::icase);
	return regex_match(str, matched, regstr);
}
//完全匹配字符串，只返回是否匹配不返回结果
inline bool rmatch(string& str, string r)
{
	regex regstr(r, regex::icase);
	return regex_match(str, regstr);
}
//在给定字符串中查找匹配的字符串，以smatch返回结果
inline bool rsearch(string &str,smatch &matched, string r)
{
	regex regstr(r,regex::icase);
	return regex_search(str, matched, regstr);
}
//在给定字符串中查找匹配的字符串，只返回是否匹配不返回结果
inline bool rsearch(string& str, string r)
{
	regex regstr(r, regex::icase);
	return regex_search(str, regstr);
}
//替换字符串
inline string rreplace(string partern, string substr,string replace)
{
	return regex_replace(partern, regex(substr, regex::icase), replace);
}



#endif // !__S_REG_TOOL_H__
