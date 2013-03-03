#include "StdAfx.h"
#include "AxpUtilTest.h"

namespace AXP
{

/*************************************************************************
	Constructor
*************************************************************************/
Lua_Test::Lua_Test(LuaPlus::LuaState* pLuaState)
	: LuaExport< Lua_Test >("TestUtil", pLuaState)
{
	//ע�ᵼ������
	RegisterFunction("clear",  &AXP::Lua_Test::Lua_clear);
	RegisterFunction("selfCheck",  &AXP::Lua_Test::Lua_selfCheck);
	RegisterFunction("distrubPath", &AXP::Lua_Test::Lua_distrubPath);
	RegisterFunction("copyPath", &AXP::Lua_Test::Lua_copyPath);
	RegisterFunction("createPath", &AXP::Lua_Test::Lua_createPath);
	RegisterFunction("deletePath", &AXP::Lua_Test::Lua_deletePath);
	RegisterFunction("log", &AXP::Lua_Test::Lua_log);

	_recreateLog();
}

/*************************************************************************
	Destructor
*************************************************************************/
Lua_Test::~Lua_Test()
{
}

/*************************************************************************
	���
*************************************************************************/
int Lua_Test::Lua_clear(LuaPlus::LuaState* state)
{
	_recreateLog();

	unsigned int nTimeNow = (unsigned)timeGetTime();
	srand(nTimeNow);
	
	_log("srand(%d)", nTimeNow);

	char szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);
	_log("currentDir:%s", szCurrentDir);
	return 0;
}

/*************************************************************************
	�ļ����
*************************************************************************/
int Lua_Test::Lua_selfCheck(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("TestUtil::selfCheck[2] param parameter error");
	}
	
	const char* szPatchFile = args[2].GetString();

	IPakFile* pPakFile = createPakFile();
	
	if(!pPakFile->openPakFile(szPatchFile, true))
	{
		state->PushBoolean(false);
		return 1;
	}

	bool bSuccess = pPakFile->selfCheck(0);

	state->PushBoolean(bSuccess);
	return 1;
}

/*************************************************************************
	Ŀ¼�Ŷ�
*************************************************************************/
int Lua_Test::Lua_distrubPath(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("TestUtil::distrubPath[2] param parameter error");
	}
	
	const char* szPath = args[2].GetString();

	_log("distrubPath: %s", szPath);

	_distrubPath(szPath);
	return 0;
}

//�ݹ��Ŷ�Ŀ¼
void Lua_Test::_distrubPath(const char* szPath)
{
	WIN32_FIND_DATA theFindData;

	// TO Search...
	char szFileToSearch[MAX_PATH] = {0};
	strncpy(szFileToSearch, szPath, MAX_PATH);
	PathAppend(szFileToSearch, "*.*");

	// find first file
	HANDLE hFind = FindFirstFile(szFileToSearch, &theFindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		//empty !
		FindClose(hFind);
		return;
	}

	std::set< std::string > setFileNeedRename;

	BOOL bFind = FALSE;
	do
	{
		if(theFindData.cFileName[0] != '.')
		{
			//File in Disk
			CHAR szPathFileName[MAX_PATH];
			strncpy(szPathFileName, szPath, MAX_PATH);
			PathAppend(szPathFileName, theFindData.cFileName);

			//�ļ���
			if(theFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				_distrubPath(szPathFileName);
			}
			//�ļ�
			else
			{
				if(_distrubFile(szPathFileName))
				{
					setFileNeedRename.insert(szPathFileName);
				}
			}
		}

		// Find Next file.
		bFind = FindNextFile(hFind, &theFindData);
	}while(bFind);
	FindClose(hFind);

	//�޸��ļ���
	std::set< std::string >::const_iterator it;
	for(it=setFileNeedRename.begin(); it!=setFileNeedRename.end(); it++)
	{
		std::string strFileName = *it;

		char szNewFileName[1024] = {0};

		int nTemp = 0;
		do
		{
			_snprintf(szNewFileName, 1024, "%s.%08X", strFileName.c_str(), nTemp++);
			if(!PathFileExists(szNewFileName)) break;
		}while(true);

		if(!MoveFile(strFileName.c_str(), szNewFileName))
		{
			char szErrMsg[1024] = {0};
			_snprintf(szErrMsg, 1024, "Err rename %s -> %s", strFileName.c_str(), szNewFileName);
			throw szErrMsg;
		}

		_log("rename file: %s->%s", strFileName.c_str(), szNewFileName);
	}
}

//�Ŷ��ļ�
bool Lua_Test::_distrubFile(const char* szFileName)
{
	/*
	����0-99�������

	[0  - 50)	- �����κθĶ�
	[50 - 70)	- �ļ������޸�
	[70 - 80)   - �ļ�ɾ��
	[80 - 99)	- �ļ����������൱��ɾ��һ�����ļ������һ�����ļ�

	*/

	int nCmd = rand()%100;
	if(nCmd >= 0 && nCmd < 50)
	{
		//�����κθĶ�
		return false;
	}
	else if(nCmd >= 50 && nCmd < 70)
	{
		//�ļ��޸�...
		printf("modify file: %s", szFileName);

		//���ļ�
		HANDLE hFile = ::CreateFile(szFileName, 
							FILE_READ_DATA|FILE_WRITE_DATA|FILE_APPEND_DATA,
							FILE_SHARE_READ,
							0,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
							0);

		if(hFile == INVALID_HANDLE_VALUE)
		{
			throw std::exception("open file error");
			return false;
		}

		int nFileSize = GetFileSize(hFile, 0);

		//�޸�10��
		for(int i=0; i<10; i++)
		{
			int nOffset = rand()%nFileSize;
			unsigned char nContents = (unsigned char)(rand()%256);
			
			//��λָ��
			if(INVALID_SET_FILE_POINTER == 
				SetFilePointer(hFile, nOffset, 0, FILE_BEGIN))
			{
				CloseHandle(hFile);
				throw std::exception("move file point");
			}

			DWORD dwBytesWrite;
			if(!WriteFile(hFile, &nContents, sizeof(nContents), &dwBytesWrite, 0) || 
				sizeof(nContents) != dwBytesWrite)
			{
				CloseHandle(hFile);
				throw std::exception("write file point");
			}
		}

		CloseHandle(hFile);
		return false;
	}
	else if(nCmd >= 70 && nCmd < 80)
	{
		//�ļ�ɾ��
		_log("del file: %s", szFileName);
		DeleteFile(szFileName);
		return false;
	}
	else
	{
		//����
		return true;
	}
}

//Ŀ¼����
int Lua_Test::Lua_copyPath(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("TestUtil::copyPath[2] param parameter error");
	}
	if(!(args[3].IsString()))
	{
		throw std::exception("TestUtil::copyPath[3] param parameter error");
	}
	if(!(args[4].IsBoolean()))
	{
		throw std::exception("TestUtil::copyPath[4] param parameter error");
	}
	
	const char* szPath1 = args[2].GetString();
	const char* szPath2 = args[3].GetString();
	bool bRecusive = args[4].GetBoolean();

	_log("copy path%s \"%s\" -> \"%s\"", bRecusive ? "[R]" : "", szPath1, szPath2);
	//���Path2
	char szTemp[MAX_PATH];
	strncpy(szTemp, szPath2, MAX_PATH);
	PathAppend(szTemp, "*.*");

	SHFILEOPSTRUCT shf;
	memset(&shf,0,sizeof(SHFILEOPSTRUCT));
	shf.hwnd = NULL;
	shf.pFrom = szTemp;
	shf.wFunc = FO_DELETE;
	shf.fFlags = FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_SILENT;
	SHFileOperation(&shf);

	CreateDirectory(szPath2, 0);

	//copy path1 -> path2 
	strncpy(szTemp, szPath1, MAX_PATH);
	PathAppend(szTemp, "*.*");

	memset(&shf,0,sizeof(SHFILEOPSTRUCT));
	shf.hwnd = NULL;
	shf.pFrom = szTemp;
	shf.pTo = szPath2;
	shf.wFunc = FO_COPY;
	shf.fFlags = bRecusive ? 
					FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_SILENT :
					FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_SILENT|FOF_NORECURSION|FOF_FILESONLY;
	SHFileOperation(&shf);

	return 0;
}

//ɾ��Ŀ¼
int Lua_Test::Lua_deletePath(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("TestUtil::deletePath[2] param parameter error");
	}
	
	const char* szPath = args[2].GetString();

	//���Path2
	char szTemp[MAX_PATH];
	strncpy(szTemp, szPath, MAX_PATH);
	PathAppend(szTemp, "*.*");

	SHFILEOPSTRUCT shf;
	memset(&shf,0,sizeof(SHFILEOPSTRUCT));
	shf.hwnd = NULL;
	shf.pFrom = szTemp;
	shf.wFunc = FO_DELETE;
	shf.fFlags = FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_SILENT;
	SHFileOperation(&shf);

	::RemoveDirectory(szPath);

	return 0;

}

//����Ŀ¼
int Lua_Test::Lua_createPath(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("TestUtil::createPath[2] param parameter error");
	}
	
	const char* szPath = args[2].GetString();

	::CreateDirectory(szPath, 0);

	return 0;
}

//��־
int Lua_Test::Lua_log(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("TestUtil::Log[2] param parameter error");
	}
	
	const char* szLogMsg = args[2].GetString();

	_log("%s", szLogMsg);

	return 0;
}

void Lua_Test::_recreateLog(void)
{
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);

	char szTempFile[MAX_PATH];
	_snprintf(szTempFile, MAX_PATH, "%02d_%02d_%02d_%02d_%02d.log", 
		timeNow.wMonth, timeNow.wDay, timeNow.wHour, timeNow.wMinute, timeNow.wSecond);

	m_strLogFile = szTempFile;
}

//��־
void Lua_Test::_log(const char* pszFmt, ...)
{
	CHAR szTemp[1024] = {0};
	va_list ptr; va_start(ptr, pszFmt);
	_vsnprintf((char *)szTemp, 1024, pszFmt, ptr);
	va_end(ptr);

	printf("%s\n", szTemp);

	FILE* fp = fopen(m_strLogFile.c_str(), "a" );
	if(!fp) fp = fopen(m_strLogFile.c_str(), "w");
	if(!fp) return;

	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);

	fprintf(fp, "[%02d_%02d:%02d:%02d]: %s\n", 
		timeNow.wDay, timeNow.wHour, timeNow.wMinute, timeNow.wSecond, szTemp);
	fclose(fp);
}

}