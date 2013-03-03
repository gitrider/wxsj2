#include "StdAfx.h"
#include "AxpUtilPatchMaker.h"

namespace AXP
{
	
/*************************************************************************
	Constructor
*************************************************************************/
Lua_PatchMaker::Lua_PatchMaker(LuaPlus::LuaState* pLuaState)
	: LuaExport< Lua_PatchMaker >("PatchMaker", pLuaState)
{
	//注册导出函数
	RegisterFunction("clear",  &AXP::Lua_PatchMaker::Lua_clear);
	RegisterFunction("analysePath", &AXP::Lua_PatchMaker::Lua_analysePath);
	RegisterFunction("getPathVersion", &AXP::Lua_PatchMaker::Lua_getPathVersion);
	RegisterFunction("generatePatch", &AXP::Lua_PatchMaker::Lua_generatePatch);
	RegisterFunction("generateUpdateExe", &AXP::Lua_PatchMaker::Lua_generateUpdateExe);

	m_pPatchMakerImpl = createPatchMaker();
}

/*************************************************************************
	Destructor
*************************************************************************/
Lua_PatchMaker::~Lua_PatchMaker()
{
	if(m_pPatchMakerImpl)
	{
		destroyPatchMaker(m_pPatchMakerImpl);
		m_pPatchMakerImpl = 0;
	}
}

/*************************************************************************
	清空
*************************************************************************/
int Lua_PatchMaker::Lua_clear(LuaPlus::LuaState* state)
{
	if(m_pPatchMakerImpl)
	{
		destroyPatchMaker(m_pPatchMakerImpl);
		m_pPatchMakerImpl = 0;
	}

	m_pPatchMakerImpl = createPatchMaker();

	return 0;
}

/*************************************************************************
	分析目录
*************************************************************************/
int Lua_PatchMaker::Lua_analysePath(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("PatchMaker::analysePath[2] param parameter error");
	}

	if(!(args[3].IsString()))
	{
		throw std::exception("PatchMaker::analysePath[3] param parameter error");
	}

	const char* szPath = args[2].GetString();
	const char* szVersion = args[3].GetString();

	bool bSuccess = m_pPatchMakerImpl->analysePath(szPath, szVersion);

	state->PushBoolean(bSuccess);
	return 1;
}

/*************************************************************************
	获得版本号
*************************************************************************/
int Lua_PatchMaker::Lua_getPathVersion(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("PatchMaker::getPathVersion[2] param parameter error");
	}

	const char* szPath = args[2].GetString();

	state->PushString(m_pPatchMakerImpl->getPathVersion(szPath));

	return 1;
}

/*************************************************************************
	生成Patch文件
*************************************************************************/
int Lua_PatchMaker::Lua_generatePatch(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("PatchMaker::generatePatch[2] param parameter error");
	}

	if(!(args[3].IsString()))
	{
		throw std::exception("PatchMaker::generatePatch[3] param parameter error");
	}
	
	if(!(args[4].IsString()))
	{
		throw std::exception("PatchMaker::generatePatch[4] param parameter error");
	}

	const char* szOldVerPath = args[2].GetString();
	const char* szNewVerPath = args[3].GetString();
	const char* szPatchFile	 = args[4].GetString();

	bool bSuccess = m_pPatchMakerImpl->generatePatch(szOldVerPath, szNewVerPath, 
						szPatchFile, _patchCallBack);

	state->PushBoolean(bSuccess);
	return 1;
}

/*************************************************************************
	生成升级补丁
*************************************************************************/
int Lua_PatchMaker::Lua_generateUpdateExe(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("PatchMaker::generateUpdateExe[2] param parameter error");
	}

	if(!(args[3].IsString()))
	{
		throw std::exception("PatchMaker::generateUpdateExe[3] param parameter error");
	}

	if(!(args[4].IsString()))
	{
		throw std::exception("PatchMaker::generateUpdateExe[4] param parameter error");
	}

	if(!(args[5].IsString()))
	{
		throw std::exception("PatchMaker::generateUpdateExe[5] param parameter error");
	}

	const char* szVersionFrom = args[2].GetString();
	const char* szVersionTo = args[3].GetString();
	const char* szPatchZip = args[4].GetString();
	const char* szExeHead = args[5].GetString();

	char szPatchExe[MAX_PATH] = {0};
	strncpy(szPatchExe, szPatchZip, MAX_PATH);
	::PathRenameExtension(szPatchExe, ".exe");

	char pTempBuf[1024] = {0};

	//创建
	FILE* fpWrite = fopen(szPatchExe, "wb");
	if(!fpWrite) throw std::exception("Can't create update exe!");

	//copy exe head
	FILE* fpRead = fopen(szExeHead, "rb");
	if(!fpRead) 
	{
		fclose(fpWrite);
		throw std::exception("Can't open update exe head!");
	}

	while(true)
	{
		size_t nReadSize = fread(pTempBuf, 1, 1024, fpRead);
		if(nReadSize == 0) break;

		fwrite(pTempBuf, 1, nReadSize, fpWrite);
		if(feof(fpRead)) break;
	};

	fclose(fpRead); fpRead=0;

	//write version from
	int nLen = (int)strlen(szVersionFrom)+1;
	fwrite(&nLen, sizeof(int), 1, fpWrite);
	fwrite(szVersionFrom, 1, nLen, fpWrite);

	//write version to
	nLen = (int)strlen(szVersionTo)+1;
	fwrite(&nLen, sizeof(int), 1, fpWrite);
	fwrite(szVersionTo, 1, nLen, fpWrite);

	//write patch data
	fpRead = fopen(szPatchZip, "rb");
	if(!fpRead) 
	{
		fclose(fpWrite);
		throw std::exception("Can't open patch zip!");
	}

	while(true)
	{
		size_t nReadSize = fread(pTempBuf, 1, 1024, fpRead);
		if(nReadSize == 0) break;

		fwrite(pTempBuf, 1, nReadSize, fpWrite);
		if(feof(fpRead)) break;
	};
	fclose(fpRead); fpRead=0;

	//close file
	fclose(fpWrite); fpWrite=0;

	state->PushBoolean(true);
	return 1;
}

/*************************************************************************
	生成Patch时的回调函数, 屏蔽Launch.exe
*************************************************************************/
bool Lua_PatchMaker::_patchCallBack(const char *szPakName, const char* szFileName)
{
	if(szPakName[0]==0 && 
		(_stricmp(szFileName, "Launch.exe")==0 || _stricmp(szFileName, "Launch.bin")==0))
		return false;

	printf("%s:%s\n", szPakName, szFileName);
	return true;
}

}