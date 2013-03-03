
/*
	针对MPQ包做资源更新时极为不便，因此增加一个内部版本
	资源的更新在各自真实目录下进行，以利于开发
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

	//删除一个完整的包,内部测试版：删除不针对包，需删除所有零散文件
	void DelMpqPackage(string& PackName);

	//从包中删除一个文件，内部测试版：删除不针对包，而是删除目录下的零散文件
	bool DelFileFromPackage(string& FileName);

	//添加一个完整的包,内部测试版：添加不针对包，需添加所有零散文件
	BOOL AddPackage(string& ZipName,string PackName);

	//添加一个文件到包中,内部测试版：添加不针对包，而是添加到相应目录中
	BOOL AddFileToPackage(string& PackName,string& FileName);

	BOOL IsAllRightCRC32AndMD5(TKPMFile* pFile,unsigned char * input, int ilen);


private:


};

#endif