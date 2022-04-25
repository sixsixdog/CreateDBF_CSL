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

#define MAX_FIELDS 255   //��������Ϊ255 ����255
#define DBF_DELETED_RECORD_FLAG '*' //ɾ����־���� 0x30Ϊ* 0x03δ֪?
#define MAX_RECORD_SIZE 0xffff*50 //�������
#define FOXBASE 0x03 //�������� FoxBASE+/Dbase III plus, no memo

//�ļ�ͷ����
struct fileHeader
{
    uint8 u8FileType; //���ݰ汾����
    uint8 u8LastUpdateYear; //��������
    uint8 u8LastUpdateMonth; //��������
    uint8 u8LastUpdateDay; //��������
    uint32 uRecordsInFile; //�ܼ�¼
    uint16 uPositionOfFirstRecord; //��һ������ƫ��
    uint16 uRecordLength; // ���ݴ�С��ɾ����Ǵ�С
    /**
     * ��20�ֽ�ϵͳ����
     */
    uint8 Reserved16[4 * 4]; // 16�ֽڱ�����δ֪��;��Ĭ��Ϊ0
    uint8 uTableFlags; //��ṹ
    uint8 uCodePage;    //����ҳ �˴���sytemҳ��ͬ
    uint8 Reserved2[2]; // 2�ֽڱ���
};

//�ֶζ���
struct fieldDefinition
{
    char cFieldName[11]; //�ļ���
    char cFieldType; //��������
    uint32 uFieldOffset; //����ƫ�ƴ�С
    uint8 uLength; //�ֶγ���
    uint8 uNumberOfDecimalPlaces; //С������
    uint8 FieldFlags; //�ֶα�ʶ δ֪��;
    uint8 uNextAutoIncrementValue[4]; // ��һ������ֵ
    uint8 uAutoIncrementStep; //������С
    uint8 Reserved8[8]; // ������С
};

typedef class C_DBF_OPS
{
public:
    C_DBF_OPS(string output,vector<fieldDefinition> fields):h_dbf(nullptr)
    {
        //�����ļ�
        h_dbf = fopen(output.c_str(), "wb+");
        if (h_dbf == INVALID_HANDLE_VALUE)
        {
            cerr << "�����ļ�ʧ�ܣ�" << endl;
            return;
        }
        //ָ����������
        m_FileHeader.u8FileType = FOXBASE;

        auto t= time(NULL);
        auto loc = localtime(&t);

        //ʱ��
        int nYear = loc->tm_year; //ֻȡ2λ
        m_FileHeader.u8LastUpdateDay = loc->tm_mday;
        m_FileHeader.u8LastUpdateMonth = loc->tm_mon + 1;
        m_FileHeader.u8LastUpdateYear = nYear;
        //����λ�þ����ļ�ͷƫ��
        m_FileHeader.uPositionOfFirstRecord = 32 + 32 * fields.size()*sizeof(char) + 1; //��һ����������λ�ü�һλɾ����־λ
        size_t dataLen = 1;//��һλɾ����־λ
        //�����ֶδ�С
        for (auto &fd : fields)
        {
            dataLen += fd.uLength;
        }
        //�����ļ�ͷ���ݴ�С��¼
        m_FileHeader.uRecordLength = dataLen;
        //д���ļ�ͷ
        auto writeSize= fwrite(&m_FileHeader, 1, sizeof(m_FileHeader), h_dbf);

        //д���ֶζ���
        writeSize = fwrite(fields.data(), 1, sizeof(fieldDefinition)*fields.size(), h_dbf);
        char defineEnd[2]{};
        defineEnd[0] = 0x0d;
        defineEnd[1] = 0x1a;
        //д�������־
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
