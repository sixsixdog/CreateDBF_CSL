
#include "CDBF_OPS.hpp"
#include <iostream>
#include <iomanip> 
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "windows.h"
#include "S_RegTool.hpp"


using namespace std;

void createField(vector<string>& info, vector<fieldDefinition>& fields);
vector<string> getInfo(string& str);
vector<string> getIntInfo(string& str);
vector<string> getOtherInfo(string& str);

ostream &operator<<(ostream& o, vector<string> f)
{
    o << f.at(0) << "\t" << f.at(1) << "\t" << f.at(2) << "\t" << f.at(3);
    return o;
}

ostream &operator<<(ostream& o, fieldDefinition f)
{
	o << setw(20) << setiosflags(ios::left) << "[" + string(f.cFieldName) + "]" << "[" << char(f.cFieldType) << "]\t[" << int(f.uLength) << "]\t[" << int(f.uNumberOfDecimalPlaces) << "]";
	return o;
}

int main(int argc, char* argv[])
{
    auto path = argv[1];
    //auto path = "C:/Users/MinGW/Desktop/DBF建表.csv";
    auto out = argv[2];
    //auto out = "C:/Users/MinGW/Desktop/DBF建表.dbf";

    auto output = "";
    auto fields = vector<fieldDefinition>();

    ifstream excel(path, ios::in);
    if (excel&&excel.is_open())

    {
        try
        {
            string add_str = "";
            bool first = true;
            while (excel)
            {
                getline(excel, add_str);
                //首个去除头标记
                if (first)
                {
                    add_str = add_str.substr(3);
                    first = false;
                }

                if (strcmp(add_str.c_str(),"")!=0)
                {
                    //剔除引号
                    add_str = rreplace(add_str, "\"", "");
                    //获取字段信息
                    auto ret = getInfo(add_str);
                    //输出
                    //cout << ret << endl;
                    //根据字段信息创建字段
                    createField(ret, fields);
                }
            }
        }
        catch(exception& e)
		{
			cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << ":" << e.what() << endl;
        }
    }
   DBF dbf(out,fields);
}

void createField(vector<string> &info, vector<fieldDefinition> &fields)
{
    fieldDefinition newField = {};
    strncpy(newField.cFieldName, info[0].c_str(), 11);

    auto lenth = atoi(info[2].c_str());


    newField.uLength = atoi(info[2].c_str());

    newField.uNumberOfDecimalPlaces = atoi(info[3].c_str());

    if (rmatch(info[1],"Text"))
    {
        newField.cFieldType = 'C';
    }
    else if (rmatch(info[1], "Float"))
    {
        newField.cFieldType = 'N';
        if (lenth > 18)
            newField.uLength = 18;
    }
    else if (rmatch(info[1], "Numeric"))
    {
        newField.cFieldType = 'N';
    }else if(rmatch(info[1],"Number"))
    {
        newField.cFieldType = 'N';
    }
	else
    {
        newField.cFieldType = 'C';
    }

    //cout << info << endl;
    cout << newField << endl;
    fields.push_back(newField);
}

vector<string> getInfo(string &str)
{
    auto ret = vector<string>();
    ret = getIntInfo(str);
    if(ret.size()==0)
    {
        ret = getOtherInfo(str);
    }

    return ret;
}

vector<string> getIntInfo(string &str)
{
    auto ret = vector<string>();

    //正则表达式匹配
    smatch matched = smatch();
    auto r = rsearch(str, matched, R"((.*),(\w*)2*\((\d*)\))");
    if(!r)
		return ret;
    for (auto i = 1; i < matched.size(); i++)
    {
		ret.push_back(matched[i]);
    }
    ret.push_back("0");
    return ret;
}

//getOtherInfo
vector<string> getOtherInfo(string &str)
{
	auto ret = vector<string>();
	//正则表达式匹配
	smatch matched = smatch();
	rsearch(str, matched, R"((.*),(\w*)2*\((\d*),(\d*)\))");
	for (auto i = 1; i < matched.size(); i++)
	{
		ret.push_back(matched[i]);
	}
	return ret;
}
