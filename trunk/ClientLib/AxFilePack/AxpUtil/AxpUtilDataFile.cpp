#include "StdAfx.h"
#include "AxpUtilDataFile.h"
#include "WXSJ_DBC.h"

namespace AXP
{

std::string Lua_DataFile::m_strTempPath;	//!< ��ʱĿ¼

/*************************************************************************
	Constructor
*************************************************************************/
Lua_DataFile::Lua_DataFile(LuaPlus::LuaState* pLuaState)
	: LuaExport< Lua_DataFile >("DataFile", pLuaState)
{
	//ע�ᵼ������
	RegisterFunction("convertDBCFile", &AXP::Lua_DataFile::Lua_convertDBCFile);
	RegisterFunction("convertLUAFile", &AXP::Lua_DataFile::Lua_convertLUAFile);

	//������ʱĿ¼
	char szTempPath[MAX_PATH] = {0};
	char szTempFileName[MAX_PATH] = {0};

	::GetTempPath(MAX_PATH, szTempPath);
	::GetTempFileName(szTempPath, "ax", MAX_PATH, szTempFileName);

	::DeleteFile(szTempFileName);

	::CreateDirectory(szTempFileName, 0);
	m_strTempPath = szTempFileName;
}

/*************************************************************************
	Destructor
*************************************************************************/
Lua_DataFile::~Lua_DataFile()
{
	//�����ʱĿ¼
	char szTemp[MAX_PATH];
	strncpy(szTemp, m_strTempPath.c_str(), MAX_PATH);
	PathAppend(szTemp, "*.*");

	SHFILEOPSTRUCT shf;
	memset(&shf,0,sizeof(SHFILEOPSTRUCT));
	shf.hwnd = NULL;
	shf.pFrom = szTemp;
	shf.wFunc = FO_DELETE;
	shf.fFlags = FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_SILENT;
	SHFileOperation(&shf);
}

//��׼����ص�
bool Lua_DataFile::_standSaveCallBack(const char *szNameInDiskOrg, 
						const char* szNameInPak, char* szNameInDisk, int nNameBufLen)
{
	strncpy(szNameInDisk, szNameInDiskOrg, nNameBufLen);
	std::cout << "PakIn: " << szNameInPak << std::endl;
	return true;
}

//DBC������ű��ļ�����ص��ӿ�(Hack)
bool Lua_DataFile::_configHackSaveCallBack(const char *szNameInDiskOrg, 
						const char* szNameInPak, char* szNameInDisk, int nNameBufLen)
{
	const char* szExt = ::PathFindExtension(szNameInDiskOrg);
	if(!szExt) 
	{
		return _standSaveCallBack(szNameInDiskOrg, szNameInPak, szNameInDisk, nNameBufLen);
	}

	if(_stricmp(szExt, ".lua") == 0 && strstr(szNameInDiskOrg, "Interface") != 0)
	{
		return _standSaveCallBack(szNameInDiskOrg, szNameInPak, szNameInDisk, nNameBufLen);
	}

	return _configSaveCallBack(szNameInDiskOrg, szNameInPak, szNameInDisk, nNameBufLen);
}

//DBC������ű�ת��
bool Lua_DataFile::_configSaveCallBack(const char *szNameInDiskOrg, 
						const char* szNameInPak, char* szNameInDisk, int nNameBufLen)
{
	const char* szExt = ::PathFindExtension(szNameInDiskOrg);
	if(!szExt) 
	{
		return _standSaveCallBack(szNameInDiskOrg, szNameInPak, szNameInDisk, nNameBufLen);
	}

	//������ʱ�ļ�
	char szTempFileName[MAX_PATH] = {0};
	strncpy(szTempFileName, m_strTempPath.c_str(), MAX_PATH);
	PathAppend(szTempFileName, PathFindFileName(szNameInDiskOrg));

	//DBC�ļ�
	if(_stricmp(szExt, ".txt") == 0)
	{
		convertDBCFile(szNameInDiskOrg, szTempFileName);
		std::cout << "Compress: " << szNameInPak << std::endl;
		strncpy(szNameInDisk, szTempFileName, nNameBufLen);
	}
	//����ű�
	else if(_stricmp(szExt, ".lua") == 0)
	{
		if(!convertLUAFile(szNameInDiskOrg, szTempFileName)) return false;

		std::cout<< "ConvetLua:" << szNameInPak << std::endl;
		strncpy(szNameInDisk, szTempFileName, nNameBufLen);
	}
	else
	{
		return _standSaveCallBack(szNameInDiskOrg, szNameInPak, szNameInDisk, nNameBufLen);
	}

	return true;
}

//��׼�Ƚϻص�
bool Lua_DataFile::_standCompareCallBack(const char* szFileNameInDisk, IStream* pDataStream)
{
	std::cout << "Compare: " << szFileNameInDisk << std::endl;

	if(!pDataStream)
	{
		throw std::exception("data stram null");
	}

	//��Դ�����ļ�
	HANDLE hDataFile = CreateFile(szFileNameInDisk,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | 
		FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
	if(hDataFile == INVALID_HANDLE_VALUE)
	{
		throw std::exception("open disk file error");
	}

	//�Ƚ��ļ���С
	unsigned int nFileSize = GetFileSize(hDataFile, 0);
	if(nFileSize != pDataStream->size())
	{
		CloseHandle(hDataFile);

		char szErrMsg[MAX_PATH] = {0};
		_snprintf(szErrMsg, MAX_PATH, "FileSize %d!=%d", nFileSize, pDataStream->size());
		throw std::exception(szErrMsg);
	}

	const int MAX_BUFFER_SIZE = 2048;

	BYTE buffer1[MAX_BUFFER_SIZE] = {0};
	BYTE buffer2[MAX_BUFFER_SIZE] = {0};
	DWORD dwBytesRead;
	BOOL bReadSuccess = ReadFile(hDataFile, buffer1, sizeof(buffer1), &dwBytesRead, 0);

	//ʵ�ʱȽϵĳߴ�
	unsigned int nActCompareSize = 0;

	do
	{
		//�ļ�����
		if(dwBytesRead==0 || (!bReadSuccess && ERROR_HANDLE_EOF==GetLastError())) break;

		//�������ļ�ʧ��
		if(!bReadSuccess)
		{
			CloseHandle(hDataFile);
			throw std::exception("read diskfile err");
		}

		//��������
		if(dwBytesRead != pDataStream->read(buffer2, sizeof(buffer2)))
		{
			CloseHandle(hDataFile);
			throw std::exception("read stream err");
		}

		//�Ƚ��ļ�����
		if(memcmp(buffer1, buffer2, dwBytesRead) != 0)
		{
			CloseHandle(hDataFile);
			return false;
		}

		nActCompareSize += dwBytesRead;
		if(nActCompareSize >= nFileSize)
		{
			break;
		}

		bReadSuccess = ReadFile(hDataFile, buffer1, sizeof(buffer1), &dwBytesRead, 0);
	}while(true);

	//�ر�Դ�ļ�
	CloseHandle(hDataFile); hDataFile=0;

	std::cout << " SUCCESS!" << std::endl;

	return true;
}

//DBC������ű��ļ�����ص��ӿ�
bool Lua_DataFile::_configCompareCallBack(const char* szFileNameInDisk, IStream* pDataStream)
{
	const char* szExt = ::PathFindExtension(szFileNameInDisk);
	if(!szExt) 
	{
		return _standCompareCallBack(szFileNameInDisk, pDataStream);
	}

	//������ʱ�ļ�
	char szTempFileName[MAX_PATH] = {0};
	strncpy(szTempFileName, m_strTempPath.c_str(), MAX_PATH);
	PathAppend(szTempFileName, PathFindFileName(szFileNameInDisk));

	//DBC�ļ�
	if(_stricmp(szExt, ".txt") == 0)
	{
		std::cout << "Compress: " << szFileNameInDisk << std::endl;
		convertDBCFile(szFileNameInDisk, szTempFileName);
		bool bRet = _standCompareCallBack(szTempFileName, pDataStream);
		DeleteFile(szTempFileName);
		return bRet;
	}
	//����ű�
	else if(_stricmp(szExt, ".lua") == 0)
	{
		std::cout << "ConvetLua: " << szFileNameInDisk << std::endl;
		if(!convertLUAFile(szFileNameInDisk, szTempFileName)) 
		{
			if(pDataStream != 0)
			{
				throw std::exception("not history lua!");
			}
			return true;
		}

		bool bRet = _standCompareCallBack(szTempFileName, pDataStream);
		DeleteFile(szTempFileName);
		return bRet;
	}

	return _standCompareCallBack(szTempFileName, pDataStream);
}

void Lua_DataFile::convertDBCFile(const char* szSource, const char* szTarget)
{
	DBC::DBCFile file(0);

	//���ļ�
	if(!file.OpenFromTXT(szSource))
	{
		std::string strErr = "Open DBC file:";
		strErr += szSource;
		throw std::exception(strErr.c_str());
	}

	//д�������ļ�
	if(!file.WriteToBinFile(szTarget))
	{
		std::string strErr = "Write DBC file:";
		strErr += szTarget;
		throw std::exception(strErr.c_str());
	}
}

int Lua_DataFile::Lua_convertDBCFile(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Lua_DataFile::convertDBCFile[2] param parameter error");
	}

	const char* szDBCFile = args[2].GetString();

	convertDBCFile(szDBCFile, szDBCFile);

	return 0;
}

int Lua_DataFile::Lua_convertLUAFile(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Lua_DataFile::convertLUAFile[2] param parameter error");
	}

	const char* szLUAFile = args[2].GetString();

	char szTempFile[MAX_PATH] = {0};
	strncpy(szTempFile, m_strTempPath.c_str(), MAX_PATH);
	PathAppend(szTempFile, ::PathFindFileName(szLUAFile));

	convertLUAFile(szLUAFile, szTempFile);

	::CopyFile(szTempFile, szLUAFile, FALSE);

	return 0;
}

bool Lua_DataFile::convertLUAFile(const char* szSource, const char* szTarget)
{
	FILE* fpIn = fopen(szSource, "r");
	if(!fpIn) 
	{
		std::string strErr = "Open Lua file:";
		strErr += szSource;

		throw std::exception(strErr.c_str());
	}

	FILE* fpOut = 0;

	char* pTempLine = new char[2048];
	bool bWriteBegin = false;
	int nWriteLine = 0;

	const char* pParserBeginFlag = "--MisDescBegin";
	int nParserBeginFlagLen = (int)strlen(pParserBeginFlag);

	const char* pParserEndFlag = "--MisDescEnd";
	int nParserEndFlagLen = (int)strlen(pParserEndFlag);

	do
	{
		if(0 == fgets(pTempLine, 2048, fpIn)) break;
		if(pTempLine[0]==0) continue;

		std::string strLine = pTempLine;
		std::string::size_type nStart = strLine.find_first_not_of(" \t\r\n");
		if(nStart == std::string::npos) continue;

		strLine = strLine.substr(nStart);
		if(strLine.empty()) continue;

		if(!bWriteBegin && _strnicmp(strLine.c_str(), pParserBeginFlag, nParserBeginFlagLen) == 0)
		{
			bWriteBegin = true;
		}
		else if(bWriteBegin && _strnicmp(strLine.c_str(), pParserEndFlag, nParserEndFlagLen) == 0)
		{
			bWriteBegin = false;
			fprintf(fpOut, "%s", strLine.c_str());
			nWriteLine++;
			break;
		}
		else if(bWriteBegin && (strLine[0]!='-' && strLine[1]!='-'))
		{
			//dummy, to write this line
		}
		else
		{
			continue;
		}

		if(fpOut == 0)
		{
			fpOut = fopen(szTarget, "w");
			if(!fpIn) 
			{
				std::string strErr = "Write Lua file:";
				strErr += szTarget;

				throw std::exception(strErr.c_str());
			}
		}

		//write this line
		fprintf(fpOut, "%s", strLine.c_str());
		nWriteLine++;
	}while(true);

	fclose(fpIn); fpIn = 0;
	if(fpOut)
	{
		fclose(fpOut); fpOut = 0;
	}

	return nWriteLine > 2;
}



}