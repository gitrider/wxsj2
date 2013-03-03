#ifndef __PACKAGETOOLS_H__
#define __PACKAGETOOLS_H__

#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <list>
using namespace std;

typedef struct _FILE_DESC
{
	char szFileName[_MAX_PATH];
	unsigned long dwOffset;
	unsigned long dwSize;
	unsigned long dwCRC;
} FILE_DESC, *LPFILE_DESC;

typedef struct _DUMPPATH_DESC
{
	char szPath[_MAX_PATH];
	bool bRecursive;
	vector<string> vecExt;
} DUMPPATH_DESC, *LPDUMPPATH_DESC;

//字段数据类型
enum FIELD_TYPE
{
	T_INT		= 0,	//整数
	T_FLOAT		= 1,	//浮点数
	T_STRING	= 2,	//字符串
};
typedef vector< FIELD_TYPE >	FILEDS_TYPE;

//数据段
union FIELD
{
	FLOAT		fValue;
	INT			iValue;
	const CHAR*	pString;	// Just for runtime!

	//Construct
	FIELD() {}
	FIELD(INT value) { iValue = value; }
	FIELD(FLOAT value) { fValue = value; }
	FIELD(const CHAR* value) { pString = value; }
};
//数据区
typedef vector< FIELD >		DATA_BUF;

const unsigned long MAX_PACKAGE_SIZE = 60 * 1024 * 1024;

class CPackageTools
{
public:
	CPackageTools();
	~CPackageTools();
	bool ResourcePackageGen( vector<DUMPPATH_DESC>& vecResPaths, const string& strPackFileName, const vector<string>& vecGroupBasePathNames );

private:
	void FindDump( bool bDump, bool bRecursive, vector<string>& exts, const vector<string>& basepaths, unsigned long& filesnum );
	int CheckFileName( string strFileName, vector<string>& exts );
	void HeadDump( string& strFileName, unsigned long dwOffset, unsigned long dwSize, unsigned long dwCRC );
	bool FileDump( string& strFileName, string strRelativeName, unsigned long dwFileSize );
	long FindOptiMatch( const string& strPackFileName, unsigned long dwNeed );
	DWORD Memory32Win32(LPVOID pMemoryBuf, DWORD dwBufSize, DWORD &dwCrc32);
	void CalcCrc32(const BYTE byte, DWORD &dwCrc32);
	DWORD OpenDBC( char* pMemory, char* pDeadEnd );
	const CHAR* _GetLineFromMemory(CHAR* pStringBuf, INT nBufSize, const CHAR* pMemory, const CHAR* pDeadEnd);
	INT	_ConvertStringToVector(const CHAR* strStrINTgSource, vector< std::string >& vRet, const CHAR* szKey, BOOL bOneOfKey, BOOL bIgnoreEmpty);

private:
	bool           m_bSpecialLua;
	char*          m_lpLua;
	long           m_dwLuaLen;

	bool           m_bSpecialTxt;
	char*          m_lpTxt;
	long           m_dwTxtLen;


	FILE*          m_LogFile;
	string         m_strRelativePath;
	string         m_strRecursiveBasePath;
	string         m_strAppendBasePath;
	string         m_strBasePath;
	string         m_strPakFileName;
	HANDLE         m_hFileMapping;
	unsigned long  m_dwFileSize;
	unsigned long  m_dwHeadSize;
	unsigned long  m_dwFarOffset;
	unsigned long  m_dwThisFile;
	unsigned long  m_dwThisFileSize;
	unsigned long  m_dwLeaveSize;
	unsigned long m_dwSameFiles;
	unsigned char* m_lpFileBaseAddress;
	unsigned char* m_lpHeadBaseAddress;
	unsigned char* m_lpHeadAddress;
	unsigned char* m_lpFileAddress;
	map<string, string> m_mapFileNames;
	typedef pair<string, string> m_FileNamePair;
};

#endif
