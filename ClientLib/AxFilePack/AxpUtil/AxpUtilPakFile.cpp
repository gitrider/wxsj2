#include "StdAfx.h"
#include "AxpUtilPakFile.h"
#include "AxpUtilDataFile.h"

namespace AXP
{

/*************************************************************************
	Constructor
*************************************************************************/
Lua_PakFile::Lua_PakFile(LuaPlus::LuaState* pLuaState)
	: LuaExport< Lua_PakFile >("PakFile", pLuaState)
{

	//注册导出函数
	RegisterFunction("clear", &AXP::Lua_PakFile::Lua_clear);
	RegisterFunction("openPakFile",  &AXP::Lua_PakFile::Lua_openPakFile);
	RegisterFunction("extractAllToPath",  &AXP::Lua_PakFile::Lua_extractAllToPath);
	RegisterFunction("extractFileToPath",  &AXP::Lua_PakFile::Lua_extractFileToPath);
	RegisterFunction("updateFile",  &AXP::Lua_PakFile::Lua_updateFile);
	RegisterFunction("removeFile",  &AXP::Lua_PakFile::Lua_removeFile);
	RegisterFunction("test",  &AXP::Lua_PakFile::Lua_test);

	


	m_pPakImpl = createPakFile();
}

/*************************************************************************
	Destructor
*************************************************************************/
Lua_PakFile::~Lua_PakFile()
{
	if(m_pPakImpl)
	{
		destroyPakFile(m_pPakImpl);
		m_pPakImpl = 0;
	}
}

/*************************************************************************
	Clear 
*************************************************************************/
int Lua_PakFile::Lua_clear(LuaPlus::LuaState* state)
{
	if(m_pPakImpl)
	{
		destroyPakFile(m_pPakImpl);
		m_pPakImpl = 0;
	}

	m_pPakImpl = createPakFile();

	return 0;
}


	int Lua_PakFile::Lua_test(LuaPlus::LuaState* state)
	{

		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			throw std::exception("Lua_PakFile::extractAllToPath[2] param parameter error");
		}

		const char* szTargetPath = args[2].GetString();

		//清空目录
		char szTemp[MAX_PATH];
		strncpy(szTemp, szTargetPath, MAX_PATH);
		PathAppend(szTemp, "*.*");

		SHFILEOPSTRUCT shf;
		memset(&shf,0,sizeof(SHFILEOPSTRUCT));
		shf.hwnd = NULL;
		shf.pFrom = szTemp;
		shf.wFunc = FO_DELETE;
		shf.fFlags = FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_SILENT;
		SHFileOperation(&shf);

		//创建
		CreateDirectory(szTargetPath, 0);

		IStream* pListStream = m_pPakImpl->openFile("(list)");
		if(!pListStream) 
		{
			state->PushBoolean(false);
			return 1;
		}

		//skip first line
		pListStream->skipLine();

		//read second line
		char szLine[MAX_PATH*4] = {0};
		pListStream->readLine(szLine, MAX_PATH*4);

		int nFileCounts = atoi(szLine);

		/* Process the file line for line */
		while (!pListStream->eof())
		{
			int nLineLength = pListStream->readLine(szLine, MAX_PATH*4);
			if(0 == nLineLength) continue;

			//分析文件描述
			char* pDot = strchr(szLine, '|');

			if(!pDot)
			{
				pListStream->close();

				char szMsg[MAX_PATH];
				_snprintf(szMsg, MAX_PATH, "AXP_ERR_FILE_FORMAT:%s", szLine);
				throw std::exception(szMsg);
			}

			*pDot = 0;

			if(!_extractFile(szLine, szTargetPath))
			{
				pListStream->close();
				state->PushBoolean(false);
				return 1;
			}
		};
		pListStream->close(); pListStream=0;

		state->PushBoolean(true);
		return 1;
	}

/*************************************************************************
	打开Pak文件
*************************************************************************/
int Lua_PakFile::Lua_openPakFile(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Lua_PakFile::openPakFile[2] param parameter error");
	}

	if(!(args[3].IsBoolean()))
	{
		throw std::exception("Lua_PakFile::openPakFile[3] param parameter error");
	}

	const char* szPakFile = args[2].GetString();
	bool bConst = args[3].GetBoolean();

	bool bSuccess = m_pPakImpl->openPakFile(szPakFile, bConst);

	state->PushBoolean(bSuccess);
	return 1;
}

/*************************************************************************
	全部解压到指定目录
*************************************************************************/
int Lua_PakFile::Lua_extractAllToPath(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Lua_PakFile::extractAllToPath[2] param parameter error");
	}

	const char* szTargetPath = args[2].GetString();

	//清空目录
	char szTemp[MAX_PATH];
	strncpy(szTemp, szTargetPath, MAX_PATH);
	PathAppend(szTemp, "*.*");

	SHFILEOPSTRUCT shf;
	memset(&shf,0,sizeof(SHFILEOPSTRUCT));
	shf.hwnd = NULL;
	shf.pFrom = szTemp;
	shf.wFunc = FO_DELETE;
	shf.fFlags = FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_SILENT;
	SHFileOperation(&shf);

	//创建
	CreateDirectory(szTargetPath, 0);

	IStream* pListStream = m_pPakImpl->openFile("(list)");
	if(!pListStream) 
	{
		state->PushBoolean(false);
		return 1;
	}

	//skip first line
	pListStream->skipLine();

	//read second line
	char szLine[MAX_PATH*4] = {0};
	pListStream->readLine(szLine, MAX_PATH*4);

	int nFileCounts = atoi(szLine);

	/* Process the file line for line */
	while (!pListStream->eof())
	{
		int nLineLength = pListStream->readLine(szLine, MAX_PATH*4);
		if(0 == nLineLength) continue;

		//分析文件描述
		char* pDot = strchr(szLine, '|');

		if(!pDot)
		{
			pListStream->close();

			char szMsg[MAX_PATH];
			_snprintf(szMsg, MAX_PATH, "AXP_ERR_FILE_FORMAT:%s", szLine);
			throw std::exception(szMsg);
		}

		*pDot = 0;

		if(!_extractFile(szLine, szTargetPath))
		{
			pListStream->close();
			state->PushBoolean(false);
			return 1;
		}
	};
	pListStream->close(); pListStream=0;

	state->PushBoolean(true);
	return 1;
}

//解压某文件到指定目录
int Lua_PakFile::Lua_extractFileToPath(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Lua_PakFile::extraceFileToPath[2] param parameter error");
	}

	if(!(args[3].IsString()))
	{
		throw std::exception("Lua_PakFile::extraceFileToPath[3] param parameter error");
	}

	const char* szFileName = args[2].GetString();
	const char* szTargetPath = args[3].GetString();

	bool bSuccess = _extractFile(szFileName, szTargetPath);

	state->PushBoolean(bSuccess);
	return 1;
}

//将某文件更新入包
int Lua_PakFile::Lua_updateFile(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Lua_PakFile::updateFile[2] param parameter error");
	}

	if(!(args[3].IsString()))
	{
		throw std::exception("Lua_PakFile::updateFile[3] param parameter error");
	}

	if(!(args[4].IsBoolean()))
	{
		throw std::exception("Lua_PakFile::updateFile[4] param parameter error");
	}

	const char* szFileNameInDisk = args[2].GetString();
	const char* szFileNameInPak  = args[3].GetString();
	bool bAsConfig				 = args[4].GetBoolean();

	char szTempFile[MAX_PATH] = {0};
	if(bAsConfig)
	{
		Lua_DataFile::_configSaveCallBack(szFileNameInDisk, szFileNameInPak, szTempFile, MAX_PATH);
	}
	else
	{
		strncpy(szTempFile, szFileNameInDisk, MAX_PATH);
	}

	bool bSuccess = m_pPakImpl->insertContents(szTempFile, 0, szFileNameInPak, AC_DISKFILE, true);

	if(!bSuccess)
	{
		state->PushBoolean(bSuccess);
		return 1;
	}

	//重新更新list文件
	//TODO...



	state->PushBoolean(bSuccess);
	return 1;
}

//将某文件从包中删除
int Lua_PakFile::Lua_removeFile(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Lua_PakFile::removeFile[2] param parameter error");
	}

	const char* szFileNameInPak  = args[2].GetString();

	bool bSuccess = m_pPakImpl->removeFile(szFileNameInPak, true);

	state->PushBoolean(bSuccess);
	return 1;
}


//解压指定文件
bool Lua_PakFile::_extractFile(const char* szFileName, const char* szTargetPath)
{
	IStream* pStream = m_pPakImpl->openFile(szFileName);
	if(!pStream) 
	{
		return false;
	}

	char szTargetFilePath[MAX_PATH] = {0};
	strncpy(szTargetFilePath, szTargetPath, MAX_PATH);
	PathAppend(szTargetFilePath, szFileName);

	std::replace(szTargetFilePath, szTargetFilePath+strlen(szTargetFilePath), '/', '\\');
	::PathRemoveFileSpec(szTargetFilePath);

	if(szTargetFilePath[0]!=0 && !_forceCreatePath(szTargetFilePath))
	{
		pStream->close();

		char szErrMsg[MAX_PATH];
		_snprintf(szErrMsg, MAX_PATH, "Can't create path:%s", szTargetFilePath);
		throw std::exception(szErrMsg);
	}

	strncpy(szTargetFilePath, szTargetPath, MAX_PATH);
	PathAppend(szTargetFilePath, szFileName);

	//生成文件
	HANDLE hFile = ::CreateFile(szTargetFilePath, 
						FILE_READ_DATA|FILE_WRITE_DATA|FILE_APPEND_DATA,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						0,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_ARCHIVE,
						0);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		pStream->close();

		char szErrMsg[MAX_PATH];
		_snprintf(szErrMsg, MAX_PATH, "Can't create file: %s", szTargetFilePath);
		throw std::exception(szErrMsg);
	}

	BYTE buf[4096];
	do
	{
		unsigned int nReadSize = pStream->read(buf, 4096);
		if(nReadSize==0) break;

		DWORD dwWriteBytes=0;
		if(!WriteFile(hFile, buf, nReadSize, &dwWriteBytes, 0) ||
			dwWriteBytes != nReadSize)
		{
			CloseHandle(hFile);
			pStream->close();

			char szErrMsg[MAX_PATH];
			_snprintf(szErrMsg, MAX_PATH, "Can't write file: %s[winErr=%d]", szTargetFilePath, ::GetLastError());
			throw std::exception(szErrMsg);
		}
		
		if(pStream->eof()) break;
	}while(true);

	CloseHandle(hFile); hFile=0;
	pStream->close(); pStream=0;
	
	printf("Extract:%s\n", szFileName);
	return true;
}

//创建目录
bool Lua_PakFile::_forceCreatePath(const char* szPath)
{
	if(::PathFileExists(szPath)) return true;
	if(::CreateDirectory(szPath, 0)) return true;

	char szParentPath[MAX_PATH] = {0};
	strncpy(szParentPath, szPath, MAX_PATH);
	if(!::PathRemoveFileSpec(szParentPath)) return false;

	if(!_forceCreatePath(szParentPath)) return false;

	return TRUE==::CreateDirectory(szPath, 0);
}



}