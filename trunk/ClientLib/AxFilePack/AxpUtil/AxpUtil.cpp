// AxpUtil.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AxpUtilGlobal.h"
#include "AxpUtilPakMaker.h"
#include "AxpUtilDataFile.h"
#include "AxpUtilPatchMaker.h"
#include "AxpUtilUpdater.h"
#include "AxpUtilTest.h"
#include "AxpUtilPakFile.h"

#include <LuaPlus.h>

//需要执行的脚本
std::string  g_strScriptFile;

//输出版本信息
void printfCopyright(void)
{
	char szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	std::cout<< "-------------------------------------------" << std::endl;
	std::cout<< "AxpUtil: " << AXPU::g_szProductName << "Ver" << AXPU::g_szVersion << std::endl;
	std::cout<< "CurrentDirection: " << szCurrentDir << std::endl;
	std::cout<< "-------------------------------------------" << std::endl;
}

char acceptChoice(const char* szMessage, char cMin, char cMax)
{
	printf("%s:", szMessage);
	char cInput;

	do
	{
		cInput = _getch();
		cInput = toupper(cInput);
	} while(cInput < toupper(cMin) || cInput > toupper(cMax));
	printf("%c\n", cInput);

	return cMin;
}

//分析命令行
bool parserCmdLine(int argc, char* argv[])
{
	if(argc <= 1)
	{
		//输出一级菜单
		std::cout << "输入要执行的脚本:";

		//接受输入
		char szTemp[1024] = {0};
		std::cin >> szTemp;
		
		g_strScriptFile = szTemp;
	}
	else
	{
		std::cout << "Run: " << argv[1] << std::endl;
		g_strScriptFile = argv[1];
	}

	//文件是否存在
	if(_access(g_strScriptFile.c_str(), 4) != 0)
	{
		std::cout << std::endl << "文件:\"" << (const char*)g_strScriptFile.c_str() << "\"不存在!" << std::endl;
		system("pause");
		return false;
	}

	return true;
}

//Error export interface
int Lua_GetLastError(LuaPlus::LuaState* pState)
{
	const char* szErr[] = 
	{
		"AXP_ERR_SUCCESS",			//成功

		"AXP_ERR_FILE_NOTFOUND",	//文件/路径不存在
		"AXP_ERR_FILE_FORMAT",		//文件格式错误
		"AXP_ERR_FILE_ACCESS",		//无访问权限
		"AXP_ERR_FILE_READ",		//读文件错误
		"AXP_ERR_FILE_WRITE",		//写入文件错误
		"AXP_ERR_FILE_POINT",		//移动文件指针错误
		"AXP_ERR_FILE_EDITMODE",	//编辑模式错误
		"AXP_ERR_FILE_DIFF",		//文件不同
		"AXP_ERR_MEMORY",			//没有足够的内存
		"AXP_ERR_PARAM",			//参数错误
		"AXP_ERR_HASHFULL",			//Hash表满了
		"AXP_ERR_BLOCKFULL",		//Block表满了
		"AXP_ERR_ANALYSENEED",		//目录必须经过分析
		"AXP_ERR_ZIPFILE",			//ZIP文件操作错误
	};
	
	int nErrID = AXP::getLastError();
	if(nErrID < 0 || AXP::getLastError() >= sizeof(szErr)/sizeof(const char*))
	{
		pState->PushInteger(nErrID);
	}
	else
	{
		pState->PushString(szErr[nErrID]);
	}
	pState->PushString(AXP::getLastErrorDesc());
	return 2;
}

/***************************************************************************
	Main
****************************************************************************/
int _tmain(int argc, _TCHAR* argv[])
{
#if 1
	//change current dir to exe dir
	CHAR szFileName[MAX_PATH] = {0};
	GetModuleFileName(GetModuleHandle(NULL), szFileName, MAX_PATH);
	PathRemoveFileSpec(szFileName);
	PathAddBackslash(szFileName);
	SetCurrentDirectory(szFileName);
#endif

	//版权
	printfCopyright();

	//分析命令行
	if(!parserCmdLine(argc, argv)) return 0;

	//初始化Lua环境
	LuaPlus::LuaState* pLuaState = LuaPlus::LuaState::Create(true, true);

	//注册导出函数
	{
		LuaPlus::LuaObject objGlobal = pLuaState->GetGlobals();
		objGlobal.Register("getLastError", Lua_GetLastError);
	}

	//执行脚本
	try
	{
		AXP::Lua_PakMaker	thePakMaker(pLuaState);
		AXP::Lua_DataFile	theDataFile(pLuaState);
		AXP::Lua_PatchMaker	thePatchMaker(pLuaState);
		AXP::Lua_Updater	theUpdater(pLuaState);
		AXP::Lua_Test		theTestUtil(pLuaState);
		AXP::Lua_PakFile	thePakFile(pLuaState);

		pLuaState->DoFile(g_strScriptFile.c_str());
		pLuaState->DoString("Main()");
	}
	catch(std::exception e)
	{
		std::cout<< "**********************************"<<std::endl;
		std::cout<< e.what() << std::endl;
		std::cout<< "**********************************"<<std::endl;
	}

	//释放Lua环境
	LuaPlus::LuaState::Destroy(pLuaState); pLuaState = 0;

	system("pause");
	return 0;
}

