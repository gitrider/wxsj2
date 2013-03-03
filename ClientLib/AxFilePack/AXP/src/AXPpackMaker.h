/*
		Axia|FilePack
			全新打包文件制作
*/

#pragma once

#include "AXP.h"

namespace AXP
{

 //Pak文件制作者接口,用于制作全新的Pak文件
class PakMaker : public IPakMaker
{
public:
	/********************************************
					外部接口
	*********************************************/
	//将磁盘上一个目录加入到包中
	virtual bool addDiskFold(const char* szFoldInDisk, const char* szFoldInPak, const char* szExtFilter, bool bRecursive=true);

	//加入磁盘上单个文件到包中
	virtual bool addDiskFile(const char* szFileInDisk, const char* szFileInPak);

	//将加入的文件保存成pak文件,
	virtual bool savePakFile(const char* szPakFileName, AXP_PAKMAKER_SAVECALLBACK callbackFunc);

	//将加入的文件和现有的pak文件作内容比较，用于程序测试
	virtual bool comparePakFile(const char* szPakFileName, AXP_PAKMAKER_COMPARECALLBACK callbackFunc);

public:
	/********************************************
					构造/析构
	*********************************************/
	PakMaker();
	virtual ~PakMaker();

private:
	//递归加入一批文件, szFoldInDisk=路径名 sFilter=过滤器, strFoldInPak=Pak中的路径 
	bool _addDiskFold(const std::string& strFoldInDisk, 
			const std::set< std::string >& sFilter, 
			const std::string& strFoldInPak,
			bool bRecursive);
	
	//生成文件列表(list)文件
	bool _genrateListFile(std::string& strTempListFile, int nActFileCount, unsigned int nTotalCRC);

protected:
	/********************************************
					内部数据
	*********************************************/

	//文件临时保存数据结构
	struct FileNode
	{
		bool bExcute;				//被排除
		std::string	strFileInPak;	//Pak中的文件名
		std::string strFileInDisk;	//磁盘上的文件名
		unsigned int nFileSize;		//文件大小
		unsigned int nCRC;			//文件校验码
	};

	//key=经过规范后的文件名
	typedef std::map< std::string, FileNode > FileNodeBuf;

	FileNodeBuf m_AllFiles;	//文件缓冲区
};


};
