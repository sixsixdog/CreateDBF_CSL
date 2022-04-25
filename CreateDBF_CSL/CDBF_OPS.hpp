#ifndef __C_DBF_OPS_HPP__
#define __C_DBF_OPS_HPP__

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "windows.h"

using namespace std;

typedef unsigned char uint8;
typedef short int uint16;
typedef int uint32;

#define MAX_FIELDS 255   //根据文献为255 大于255
#define DBF_DELETED_RECORD_FLAG '*' //删除标志符号 0x30为* 0x03未知?
#define MAX_RECORD_SIZE 0xffff*50 //最大容量
#define FOXBASE 0x03 //数据类型 FoxBASE+/Dbase III plus, no memo

//文件头定义
struct fileHeader
{
    uint8 u8FileType; //数据版本类型
    uint8 u8LastUpdateYear; //最后更新年
    uint8 u8LastUpdateMonth; //最后更新月
    uint8 u8LastUpdateDay; //最后更新日
    uint32 uRecordsInFile; //总记录
    uint16 uPositionOfFirstRecord; //第一个数据偏移
    uint16 uRecordLength; // 数据大小加删除标记大小
    /**
     * 共20字节系统保留
     */
    uint8 Reserved16[4 * 4]; // 16字节保留，未知用途，默认为0
    uint8 uTableFlags; //表结构
    uint8 uCodePage;    //代码页 此处和sytem页相同
    uint8 Reserved2[2]; // 2字节保留
};

//字段定义
struct fieldDefinition
{
    char cFieldName[11]; //文件名
    char cFieldType; //数据类型
    uint32 uFieldOffset; //数据偏移大小
    uint8 uLength; //字段长度
    uint8 uNumberOfDecimalPlaces; //小数长度
    uint8 FieldFlags; //字段标识 未知用途
    uint8 uNextAutoIncrementValue[4]; // 下一个自增值
    uint8 uAutoIncrementStep; //自增大小
    uint8 Reserved8[8]; // 保留大小
};

typedef class C_DBF_OPS
{
public:
    C_DBF_OPS(string output,vector<fieldDefinition> fields):h_dbf(nullptr)
    {
        //创建文件
        h_dbf = fopen(output.c_str(), "wb+");
        if (h_dbf == INVALID_HANDLE_VALUE)
        {
            cerr << "创建文件失败！" << endl;
            return;
        }
        //指定数据类型
        m_FileHeader.u8FileType = FOXBASE;

        auto t= time(NULL);
        auto loc = localtime(&t);

        //时间
        int nYear = loc->tm_year; //只取2位
        m_FileHeader.u8LastUpdateDay = loc->tm_mday;
        m_FileHeader.u8LastUpdateMonth = loc->tm_mon + 1;
        m_FileHeader.u8LastUpdateYear = nYear;
        //数据位置距离文件头偏移
        m_FileHeader.uPositionOfFirstRecord = 32 + 32 * fields.size()*sizeof(char) + 1; //第一个数据所在位置加一位删除标志位
        size_t dataLen = 1;//加一位删除标志位
        //计算字段大小
        for (auto &fd : fields)
        {
            dataLen += fd.uLength;
        }
        //设置文件头数据大小记录
        m_FileHeader.uRecordLength = dataLen;
        //写入文件头
        auto writeSize= fwrite(&m_FileHeader, 1, sizeof(m_FileHeader), h_dbf);

        //写入字段定义
        writeSize = fwrite(fields.data(), 1, sizeof(fieldDefinition)*fields.size(), h_dbf);
        char defineEnd[2]{};
        defineEnd[0] = 0x0d;
        defineEnd[1] = 0x1a;
        //写入结束标志
        writeSize = fwrite(defineEnd, 1, 2, h_dbf);

        fflush(h_dbf);

        
    }
    ~C_DBF_OPS()
    {
        fclose(h_dbf);
    }



private:
    FILE* h_dbf;
    string m_sFileName = {};
    fileHeader m_FileHeader = {};
}DBF;


#endif
