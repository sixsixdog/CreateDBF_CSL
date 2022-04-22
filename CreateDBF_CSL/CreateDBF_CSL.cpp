
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

//创建字段
void createField(vector<string>& info, vector<fieldDefinition>& fields);
//获取字段信息
vector<string> getInfo(string& str);
//字段信息 VARCHAR2(12)
vector<string> getIntInfo(string& str);
//字段信息 VARCHAR2(12,2)
vector<string> getOtherInfo(string& str);

//重载输出流
ostream &operator<<(ostream& o, vector<string> f)
{
    o << f[0] << "\t" << f[1] << "\t" << f[2] << "\t" << f[3];
    return o;
}

//重载输出流
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

    //载入csv文件
    ifstream excel(path, ios::in);
    if (excel&&excel.is_open())

    {
        try
        {
            string add_str;
            bool first = true;
            while (excel)
            {
                //读取一行
                getline(excel, add_str);
                //首个数据要去除头标记
                if (first)
                {
                    add_str = add_str.substr(3);
                    first = false;
                }

                //若不为空
                if (strcmp(add_str.c_str(),"")!=0)
                {
                    //剔除引号
                    add_str = rreplace(add_str, "\"", "");
                    //获取字段信息
                    auto ret = getInfo(add_str);
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
   //根据给定生成dbf文件
   DBF dbf(out,fields);
}

void createField(vector<string> &info, vector<fieldDefinition> &fields)
{
    //初始化空定义
    fieldDefinition newField = {};
    //字段名
    strncpy(newField.cFieldName, info[0].c_str(), 11);

    //整数长度
    newField.uLength = atoi(info[2].c_str());

    //小数长度
    newField.uNumberOfDecimalPlaces = atoi(info[3].c_str());

    //根据类型设置dbf类型
    if (rmatch(info[1],"Text"))
    {
        newField.cFieldType = 'C';
    }
    else if (rmatch(info[1], "Float"))
    {
        newField.cFieldType = 'N';
        if (newField.uLength > 18)
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
    //输出
    cout << newField << endl;
    //追加进数组
    fields.push_back(newField);
}

//获取字段信息
vector<string> getInfo(string &str)
{
    //初始化空数组
    auto ret = vector<string>();
    //获取整数类型信息
    ret = getIntInfo(str);
    //若得到信息为空
    if(ret.size()==0)
    {
        //获取小数类型信息
        ret = getOtherInfo(str);
    }
    //返回所得
    return ret;
}

vector<string> getIntInfo(string &str)
{
    auto ret = vector<string>();

    //正则表达式匹配结果
    smatch matched = smatch();
    //搜寻
    auto r = rsearch(str, matched, R"((.*),(\w*)2*\((\d*)\))");
    //若匹配失败 返回空数组
    if(!r)
		return ret;
    //获得的信息整理为数组
    for (auto i = 1; i < matched.size(); i++)
    {
		ret.push_back(matched[i]);
    }
    //整型所以小数赋0
    ret.push_back("0");
    return ret;
}

//获取小数字段信息
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
