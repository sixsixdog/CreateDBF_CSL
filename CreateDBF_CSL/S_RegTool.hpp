#ifndef __S_REG_TOOL_H__
#define __S_REG_TOOL_H__
#include <regex>

using namespace std;

//��ȫƥ���ַ�����smatch���ؽ��
inline bool rmatch(string &str,smatch &matched, string r)
{
	regex regstr(r,regex::icase);
	return regex_match(str, matched, regstr);
}
//��ȫƥ���ַ�����ֻ�����Ƿ�ƥ�䲻���ؽ��
inline bool rmatch(string& str, string r)
{
	regex regstr(r, regex::icase);
	return regex_match(str, regstr);
}
//�ڸ����ַ����в���ƥ����ַ�������smatch���ؽ��
inline bool rsearch(string &str,smatch &matched, string r)
{
	regex regstr(r,regex::icase);
	return regex_search(str, matched, regstr);
}
//�ڸ����ַ����в���ƥ����ַ�����ֻ�����Ƿ�ƥ�䲻���ؽ��
inline bool rsearch(string& str, string r)
{
	regex regstr(r, regex::icase);
	return regex_search(str, regstr);
}
//�滻�ַ���
inline string rreplace(string partern, string substr,string replace)
{
	return regex_replace(partern, regex(substr, regex::icase), replace);
}



#endif // !__S_REG_TOOL_H__
