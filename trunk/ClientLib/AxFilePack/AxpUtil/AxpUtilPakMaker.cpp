#include "StdAfx.h"
#include "AxpUtilPakMaker.h"
#include "AxpUtilDataFile.h"

namespace AXP
{

/*************************************************************************
	Constructor
*************************************************************************/
Lua_PakMaker::Lua_PakMaker(LuaPlus::LuaState* pLuaState)
	: LuaExport< Lua_PakMaker >("PakMaker", pLuaState)
{

	//注册导出函数
	RegisterFunction("clear",  &AXP::Lua_PakMaker::Lua_clear);
	RegisterFunction("addDiskFold", &AXP::Lua_PakMaker::Lua_addDiskFold);
	RegisterFunction("savePakFile", &AXP::Lua_PakMaker::Lua_savePakFile);
	RegisterFunction("comparePakFile", &AXP::Lua_PakMaker::Lua_comparePakFile);
	RegisterFunction("moveSkeleton", &AXP::Lua_PakMaker::Lua_moveSkeleton);

	m_pPakMakerImpl = createPakMaker();
}

/*************************************************************************
	Destructor
*************************************************************************/
Lua_PakMaker::~Lua_PakMaker()
{
	if(m_pPakMakerImpl)
	{
		destroyPakMaker(m_pPakMakerImpl);
		m_pPakMakerImpl = 0;
	}
}

/*************************************************************************
	Clear 
*************************************************************************/
int Lua_PakMaker::Lua_clear(LuaPlus::LuaState* state)
{
	if(m_pPakMakerImpl)
	{
		destroyPakMaker(m_pPakMakerImpl);
		m_pPakMakerImpl = 0;
	}

	m_pPakMakerImpl = createPakMaker();

	return 0;
}

/*************************************************************************
	加入目录
*************************************************************************/
int Lua_PakMaker::Lua_addDiskFold(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Lua_PakMaker::addDiskFold[2] param parameter error");
	}

	if(!(args[3].IsString()))
	{
		throw std::exception("Lua_PakMaker::addDiskFold[3] param parameter error");
	}

	if(!(args[4].IsString()))
	{
		throw std::exception("Lua_PakMaker::addDiskFold[4] param parameter error");
	}

	if(!(args[5].IsBoolean()))
	{
		throw std::exception("Lua_PakMaker::addDiskFold[5] param parameter error");
	}

	const char* szDiskFold = args[2].GetString();
	const char* szDiskPak = args[3].GetString();
	const char* szFilter = args[4].GetString();
	bool bRecursive = args[5].GetBoolean();

	bool bSuccess = m_pPakMakerImpl->addDiskFold(szDiskFold, szDiskPak, szFilter, bRecursive);

	state->PushBoolean(bSuccess);
	return 1;
}

/*************************************************************************
	存盘
*************************************************************************/
int Lua_PakMaker::Lua_savePakFile(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Lua_PakMaker::addDiskFold[2] param parameter error");
	}

	if(!(args[3].IsString()))
	{
		throw std::exception("Lua_PakMaker::addDiskFold[3] param parameter error");
	}
	
	const char* szDiskFile = args[2].GetString();
	const char* szCallbackName = args[3].GetString();

	IPakMaker::AXP_PAKMAKER_SAVECALLBACK pCallbackFunc = 0;
	if(_stricmp(szCallbackName, "config") == 0)
	{
		pCallbackFunc = Lua_DataFile::_configSaveCallBack;
	}
	else if(_stricmp(szCallbackName, "config-hack") == 0)
	{
		pCallbackFunc = Lua_DataFile::_configHackSaveCallBack;
	}
	else
	{
		pCallbackFunc = Lua_DataFile::_standSaveCallBack;
	}

	bool bSuccess = m_pPakMakerImpl->savePakFile(szDiskFile, pCallbackFunc);

	state->PushBoolean(bSuccess);
	return 1;
}

//比较
int Lua_PakMaker::Lua_comparePakFile(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Lua_PakMaker::addDiskFold[2] param parameter error");
	}

	if(!(args[3].IsString()))
	{
		throw std::exception("Lua_PakMaker::addDiskFold[3] param parameter error");
	}
	
	const char* szDiskFile = args[2].GetString();
	const char* szCallbackName = args[3].GetString();

	IPakMaker::AXP_PAKMAKER_COMPARECALLBACK pCallbackFunc = 0;
	if(_stricmp(szCallbackName, "config") == 0)
	{
		pCallbackFunc = Lua_DataFile::_configCompareCallBack;
	}
	else
	{
		pCallbackFunc = Lua_DataFile::_standCompareCallBack;
	}

	bool bSuccess = m_pPakMakerImpl->comparePakFile(szDiskFile, pCallbackFunc);

	state->PushBoolean(bSuccess);

	return 1;
}

//移动Skeleton文件
int Lua_PakMaker::Lua_moveSkeleton(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Lua_PakMaker::moveSkeleton[2] param parameter error");
	}

	const char* szPath = args[2].GetString();

	FILENAME_SET setSkeletonToMove;
	_moveSkeleton(szPath, setSkeletonToMove);

	char szTarget[MAX_PATH];
	FILENAME_SET::iterator it;
	for(it=setSkeletonToMove.begin(); it!=setSkeletonToMove.end(); it++)
	{
		strncpy(szTarget, szPath, MAX_PATH);
		::PathAppend(szTarget, ::PathFindFileName(it->c_str()));

		::MoveFile(it->c_str(), szTarget);
	}

	return 0;
}

//移动Skeleton文件
void Lua_PakMaker::_moveSkeleton(const char* szPath, FILENAME_SET& setSkeletonToMove)
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

	BOOL bFind = FALSE;
	do
	{
		if(theFindData.cFileName[0] != '.')
		{
			//File in Disk
			CHAR szPathFileName[MAX_PATH];
			strncpy(szPathFileName, szPath, MAX_PATH);
			PathAppend(szPathFileName, theFindData.cFileName);

			//文件夹
			if(theFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				_moveSkeleton(szPathFileName, setSkeletonToMove);
			}
			//文件
			else
			{
				const char* pExt = ::PathFindExtension(szPathFileName);
				if(!pExt) goto FIND_NEXT;
				if(_stricmp(pExt, ".skeleton") == 0)
				{
					setSkeletonToMove.insert(szPathFileName);
				}
			}

		}

FIND_NEXT:
		// Find Next file.
		bFind = FindNextFile(hFind, &theFindData);
	}while(bFind);

	FindClose(hFind);
}

}