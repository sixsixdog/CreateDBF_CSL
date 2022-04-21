// CreateDBF_CSL.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "CDBF_OPS.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "windows.h"
#include "S_RegTool.hpp"


#define split splitType1

using namespace std;
vector<string> splitType1(string str, string sp);
void createField(vector<string>& info, vector<fieldDefinition>& fields);
void string_replace(string& s1, const string& s2, const string& s3);

ostream &operator<<(ostream& o, vector<string> f)
{
    o << f.at(0) << f.at(1) << "\t," << f.at(2) << "\t," << f.at(3);
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
        string add_str="";
        bool first = true;
        while (excel)
        {  
            streamsize len = {};
            getline(excel, add_str);
            //首个去除头标记
            if (first)
            {
                add_str = add_str.substr(3);
                first = false;
            }
            if (add_str != "")
            {
                add_str = rreplace(add_str, "\"", "");

                auto sp = split(add_str, ",");
                createField(sp, fields);
            }
        }
    }
   DBF dbf(out,fields);
}

vector<string> splitType1(string str, string sp)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    auto arr = vector<string>();
    char* substr = "";
    substr = strtok(const_cast<char*>(str.c_str()), sp.data());
    arr.push_back(substr);

    substr = strtok(NULL, "\0");
    auto temp = string(substr);
    string out;
    if (strstr(substr, "VARCHAR2") > 0 || strstr(substr, "VARCHAR") > 0)
    {
        arr.push_back("Text");
        auto lpos = temp.find_first_of("(");
        auto rpos = temp.find_first_of(")");
        auto lenth = temp.substr(lpos + 1, rpos - lpos - 1);
        arr.push_back(lenth);
        arr.push_back("0");

    }
    else if (strstr(substr, "NUMBER") > 0)
    {
        arr.push_back("Numeric");   
        auto lpos = temp.find_first_of("(");
        auto rpos = temp.find_first_of(")");
        auto lenth = temp.substr(lpos + 1, rpos - lpos - 1);

        auto pointPos = temp.find_first_of(",");
        if (pointPos > 0 && pointPos < 1000000)
        {
            lenth = strtok(const_cast<char*>(lenth.c_str()), ",");
            arr.push_back(lenth);
            auto declen = temp.substr(pointPos + 1,temp.find_first_of(")")- pointPos - 1);
            arr.push_back(declen);

        }
        else {
            if (atoi(lenth.c_str()) > 4)
                arr.push_back("8");
            else
                arr.push_back("4");
            arr.push_back("0");
        }
           
    }
    else
    {
        arr.push_back("Text");
        auto lpos = temp.find_first_of("(");
        auto rpos = temp.find_first_of(")");
        auto lenth = temp.substr(lpos + 1, rpos - lpos - 1);
        arr.push_back(lenth);
        arr.push_back("0");
    }


    return arr;
}

vector<string> splitType2(string str, string sp)
{
    auto arr = vector<string>();
    char* substr = "";
    substr = strtok(const_cast<char*>(str.c_str()), sp.data());

    arr.push_back(substr);
    while (substr != NULL) {
        substr = strtok(NULL, sp.data());
        if (substr == NULL)
            break;
        arr.push_back(substr);
    }
    return arr;
}

void createField(vector<string> &info, vector<fieldDefinition> &fields)
{
    fieldDefinition newField = {};
    strncpy(newField.cFieldName, info.at(0).c_str(), 11);

    auto lenth = atoi(info.at(2).c_str());


    newField.uLength = atoi(info.at(2).c_str());

    newField.uNumberOfDecimalPlaces = atoi(info.at(3).c_str());
    if (_stricmp(info.at(1).c_str(), "Text") == 0)
    {
        newField.cFieldType = 'C';
    }
    else if (_stricmp(info.at(1).c_str(), "Float") == 0)
    {
        newField.cFieldType = 'N';
        if (lenth > 18)
            newField.uLength = 18;
        newField.uNumberOfDecimalPlaces = 2;
    }
    else if (_stricmp(info.at(1).c_str(), "Numeric") == 0)
    {
        newField.cFieldType = 'N';
    }
    if (lenth >= 18) {
        cout << "1";
    }

    cout << info << endl;
    fields.push_back(newField);
}


void string_replace(string& s1, const string& s2, const string& s3)
{
    string::size_type pos = 0;
    string::size_type a = s2.size();
    string::size_type b = s3.size();
    while ((pos = s1.find(s2, pos)) != string::npos)
    {
        s1.replace(pos, a, s3);
        pos += b;
    }
}

vector<string> getInfo(string str)
{
    auto ret = smatch()
}