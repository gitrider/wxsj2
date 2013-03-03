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

//��Ҫִ�еĽű�
std::string  g_strScriptFile;

//����汾��Ϣ
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

//����������
bool parserCmdLine(int argc, char* argv[])
{
	if(argc <= 1)
	{
		//���һ���˵�
		std::cout << "����Ҫִ�еĽű�:";

		//��������
		char szTemp[1024] = {0};
		std::cin >> szTemp;
		
		g_strScriptFile = szTemp;
	}
	else
	{
		std::cout << "Run: " << argv[1] << std::endl;
		g_strScriptFile = argv[1];
	}

	//�ļ��Ƿ����
	if(_access(g_strScriptFile.c_str(), 4) != 0)
	{
		std::cout << std::endl << "�ļ�:\"" << (const char*)g_strScriptFile.c_str() << "\"������!" << std::endl;
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
		"AXP_ERR_SUCCESS",			//�ɹ�

		"AXP_ERR_FILE_NOTFOUND",	//�ļ�/·��������
		"AXP_ERR_FILE_FORMAT",		//�ļ���ʽ����
		"AXP_ERR_FILE_ACCESS",		//�޷���Ȩ��
		"AXP_ERR_FILE_READ",		//���ļ�����
		"AXP_ERR_FILE_WRITE",		//д���ļ�����
		"AXP_ERR_FILE_POINT",		//�ƶ��ļ�ָ�����
		"AXP_ERR_FILE_EDITMODE",	//�༭ģʽ����
		"AXP_ERR_FILE_DIFF",		//�ļ���ͬ
		"AXP_ERR_MEMORY",			//û���㹻���ڴ�
		"AXP_ERR_PARAM",			//��������
		"AXP_ERR_HASHFULL",			//Hash������
		"AXP_ERR_BLOCKFULL",		//Block������
		"AXP_ERR_ANALYSENEED",		//Ŀ¼���뾭������
		"AXP_ERR_ZIPFILE",			//ZIP�ļ���������
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

	//��Ȩ
	printfCopyright();

	//����������
	if(!parserCmdLine(argc, argv)) return 0;

	//��ʼ��Lua����
	LuaPlus::LuaState* pLuaState = LuaPlus::LuaState::Create(true, true);

	//ע�ᵼ������
	{
		LuaPlus::LuaObject objGlobal = pLuaState->GetGlobals();
		objGlobal.Register("getLastError", Lua_GetLastError);
	}

	//ִ�нű�
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

	//�ͷ�Lua����
	LuaPlus::LuaState::Destroy(pLuaState); pLuaState = 0;

	system("pause");
	return 0;
}

