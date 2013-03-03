
/*
	���MPQ������Դ����ʱ��Ϊ���㣬�������һ���ڲ��汾
	��Դ�ĸ����ڸ�����ʵĿ¼�½��У������ڿ���
*/


#ifndef TEST_UPDATE_SYSTEM_H

#define TEST_UPDATE_SYSTEM_H

#include "md5.h"
#include "crc32.h"
#include "kpmLib.h"
#include <string>
using namespace std;
using namespace KPM;


class TestUpdateSystem
{
public:
	TestUpdateSystem(void);
	~TestUpdateSystem(void);

	//ɾ��һ�������İ�,�ڲ����԰棺ɾ������԰�����ɾ��������ɢ�ļ�
	void DelMpqPackage(string& PackName);

	//�Ӱ���ɾ��һ���ļ����ڲ����԰棺ɾ������԰�������ɾ��Ŀ¼�µ���ɢ�ļ�
	bool DelFileFromPackage(string& FileName);

	//���һ�������İ�,�ڲ����԰棺��Ӳ���԰��������������ɢ�ļ�
	BOOL AddPackage(string& ZipName,string PackName);

	//���һ���ļ�������,�ڲ����԰棺��Ӳ���԰���������ӵ���ӦĿ¼��
	BOOL AddFileToPackage(string& PackName,string& FileName);

	BOOL IsAllRightCRC32AndMD5(TKPMFile* pFile,unsigned char * input, int ilen);


private:


};

#endif