/*
		Axia|FilePack
			工具函数
*/

#pragma once

namespace AXP
{

/*!
\brief
	规范文件名

\param fileName
	需要转化的文件名

\param  toLowCase
	是否转化为小写，考虑GBK编码，需要外界设置好locale

\param norSlash
	是否将 '\' 转化为 '/'

\return 
	转化后的文件名
*/
std::string normaliseName(const std::string& fileName, bool toLowCase=true, bool norSlash=true);

/*!
\brief
	获取文件大小, 不支持超过2G的大文件

\param szFileName
	文件名

\return 
	文件大小，如果文件不存在，返回0
*/
unsigned int getDiskFileSize(const char* szFileName);

/*!
\brief
	将字符串按照指定的分隔符分割

\param strResource
	字符串

\param vRet
	返回的字符串数组

\param szKey
	用于分割的关键字

\param bOneOfKey
	是否使用关键字中的任意一个字符作为分隔符

\param bIgnoreEmpty
	忽略空结果

\return 
	文件大小，如果文件不存在，返回0
*/
int convertStringToVector(const char* strResource, std::vector< std::string >& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty);

/*!
\brief
	检测一个文件的crc校验码

\param szFileName 
	文件名

\param dwCrc32
	返回的crc码

\param bIgnoreTail
	是否忽略最后四个字节

\return 
	是否成功
*/
bool crcFile32(const char* szFilename, unsigned int &dwCrc32, bool bIgnoreTail=false);

/*!
\brief
	计算一段内存的crc校验码

\param pMemoryBuf
	内存指针

\param dwBufSize
	内存大小(bytes)

\param dwCrc32
	返回的crc码

\return 
	是否成功
*/
bool crcMemory32(const void* pMemoryBuf, unsigned int dwBufSize, unsigned int& dwCrc32);

/*!
\brief
	在文件尾部附上crc校验码

\param szFileName
	文件名

\return 
	是否成功
*/
bool attachCRCFlag(const char* szFileName);

/*
\brief
	检查文件尾部的CRC校验码

\param szFileName 
	文件名

\return 
	是否成功
*/
bool checkCRCFlag(const char* szFileName);

/*
\brief
	递归创建目录

\param szPath
	目录名

\return
	是否成功
*/
bool forceCreatePath(const char* szPath);

/*!
\brief
	比较两个文件名是否相同，考虑大小写和长短文件名的问题

\param szFileName1
	文件名1

\param szFileName2
	文件名2

\return
	比较结果，意义等同c中的strcmp
*/
int compareTwoFileName(const char* szFileName1, const char* szFileName2);

/** 得到加密异或代码
*/
unsigned char getZipEncryptXORKey(unsigned int pos);

/*
\brief
	将指定文件的zip文件加密/解密

\param szFileName
	文件名

\param ignoreTail
	是否抛弃后面的四个字节

\return 
	是否成功
*/
bool encryptFile(const char* szFileName, bool ignoreTail);

/*
\brief
	某文件是否是加密后zip文件

\param szFileName
	文件名

\return 
	是否是加密后的zip文件
*/
bool isEncryptedZipFile(const char* szFileName);

}
