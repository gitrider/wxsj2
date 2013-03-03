#include "StdAfx.h"
#include "AxpUtilUpdater.h"

namespace AXP
{

/*************************************************************************
	Constructor
*************************************************************************/
Lua_Updater::Lua_Updater(LuaPlus::LuaState* pLuaState)
	: LuaExport< Lua_Updater >("Updater", pLuaState)
{
	//注册导出函数
	RegisterFunction("clear",  &AXP::Lua_Updater::Lua_clear);
	RegisterFunction("addPatchFile", &AXP::Lua_Updater::Lua_addPatchFile);
	RegisterFunction("updateVersion", &AXP::Lua_Updater::Lua_updateVersion);
	RegisterFunction("checkPatchCRC", &AXP::Lua_Updater::Lua_checkPatchCRC);
	RegisterFunction("encryptPatchFile", &AXP::Lua_Updater::Lua_encryptPatchFile);

	m_pUpdaterImpl = createUpdater();
}

/*************************************************************************
	Destructor
*************************************************************************/
Lua_Updater::~Lua_Updater()
{
	if(m_pUpdaterImpl)
	{
		destroyUpdater(m_pUpdaterImpl);
		m_pUpdaterImpl = 0;
	}
}


/*************************************************************************
	清空
*************************************************************************/
int Lua_Updater::Lua_clear(LuaPlus::LuaState* state)
{
	if(m_pUpdaterImpl)
	{
		destroyUpdater(m_pUpdaterImpl);
		m_pUpdaterImpl = 0;
	}

	m_pUpdaterImpl = createUpdater();

	return 0;
}

/*************************************************************************
	加入Patch包
*************************************************************************/
int Lua_Updater::Lua_addPatchFile(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Updater::addPatchFile[2] param parameter error");
	}
	
	const char* szPatchFile = args[2].GetString();

	bool bSuccess = m_pUpdaterImpl->addPatchFile(szPatchFile);

	state->PushBoolean(bSuccess);
	return 1;
}

/*************************************************************************
	对指定目录升级
*************************************************************************/
int Lua_Updater::Lua_updateVersion(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Updater::updateVersion[2] param parameter error");
	}
	
	const char* szPath = args[2].GetString();

	bool bSuccess = m_pUpdaterImpl->updateVersion(szPath, _updateCallBack);

	state->PushBoolean(bSuccess);
	return 1;
}

/*************************************************************************
	检查patch文件crc
*************************************************************************/
int Lua_Updater::Lua_checkPatchCRC(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Updater::CheckPatchCRC[2] param parameter error");
	}
	
	const char* szPatchFile = args[2].GetString();

	state->PushBoolean(m_pUpdaterImpl->checkPatchFileCRC(szPatchFile));

	return 1;
}

/*************************************************************************
	加密/解密更新包
*************************************************************************/
int Lua_Updater::Lua_encryptPatchFile(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		throw std::exception("Updater::CheckPatchCRC[2] param parameter error");
	}
	if(!(args[3].IsString()))
	{
		throw std::exception("Updater::CheckPatchCRC[3] param parameter error");
	}
	
	const char* szPatchFile = args[2].GetString();
	const char* szNewPatchFile = args[3].GetString();

	state->PushBoolean(m_pUpdaterImpl->encryptZipFile(szPatchFile, szNewPatchFile));

	return 1;
}

//升级回调函数
void Lua_Updater::_updateCallBack(const char *szPakName, const char* szFileName, UPDATE_PROCESS process)
{
	switch(process)
	{
	case UP_NEW_FILE:
		std::cout << "Update: " << szFileName << std::endl;
		break;

	case UP_DEL_FILE:
		std::cout << "Remove: " << szFileName << std::endl;
		break;
	
	default:
		std::cout << "ERR: " << process << std::endl;
		break;
	}
}


}