//�����ļ�����ӿ�

#pragma once

#include "AxpUtilLuaExport.h"


namespace AXP
{

class Lua_DataFile : public LuaExport< Lua_DataFile >
{
public:
	/*************************************************************************
		Lua Interface
	*************************************************************************/
	//ת��DBC�ļ�
	int Lua_convertDBCFile(LuaPlus::LuaState* state);
	//ת��Lua�ļ�
	int Lua_convertLUAFile(LuaPlus::LuaState* state);

	//һ���ļ�����ص��ӿ�
	static bool __stdcall _standSaveCallBack(const char *szNameInDiskOrg, 
							const char* szNameInPak, char* szNameInDisk, int nNameBufLen);

	//DBC������ű��ļ�����ص��ӿ�
	static bool __stdcall _configSaveCallBack(const char *szNameInDiskOrg, 
							const char* szNameInPak, char* szNameInDisk, int nNameBufLen);

	//DBC������ű��ļ�����ص��ӿ�(Hack)
	static bool __stdcall _configHackSaveCallBack(const char *szNameInDiskOrg, 
							const char* szNameInPak, char* szNameInDisk, int nNameBufLen);

	//һ���ļ��Ƚϻص�����
	static bool __stdcall _standCompareCallBack(const char* szFileNameInDisk, 
							IStream* pDataStream);

	//DBC������ű��ļ�����ص��ӿ�
	static bool __stdcall _configCompareCallBack(const char* szFileNameInDisk, 
							IStream* pDataStream);

private:
	static std::string		m_strTempPath;	//!< ��ʱĿ¼

	//��һ��dbc�ļ�ת��
	static void convertDBCFile(const char* szSource, const char* szTarget);
	//��һ������ű�ת��
	static bool convertLUAFile(const char* szSource, const char* szTarget);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	Lua_DataFile(LuaPlus::LuaState* pLuaState);
	~Lua_DataFile();
};

}