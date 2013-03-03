#pragma once
#include "GameConfig.h"

class CGameConfigManager
{
public:
	CGameConfigManager(void);
	~CGameConfigManager(void);

public:
	//加载所有配置文件，返回配置文件数
	int	LoadAllConfigFile(void);
	//返回最新的一个配置文件
	CGameConfig* GetLastestConfig(void);
	//设置某值
	void SetValue(const char* szValueName, int nValue);
	//设置某值
	void SetValue(const char* szValueName, char* szValue);
	//是否配置更改
	bool NeedUpdate(void);
	//保存修改
	void SaveModifed(void);
	//某变量是否修改过
	bool IsVariableModifyed(const char* szName);
	//取得修改后的值
	const char* GetModifyedValue(const char* szName);
	//删除个人设置文件
	void DeletePrivateFile(void);

protected:
	void _tryAddConfigPath(const char* szConfigPath);
	void _tryAddConfigFile(const char* szConfigFile);

protected:
	typedef std::map< ULONGLONG, CGameConfig* > GAMECONFIG_BUF;

	GAMECONFIG_BUF	m_allConfigFile;	//所有的配置文件
	CGameConfig*	m_pLastestConfig;	//更新日期最新的配置文件

	CONFIG_BUF		m_mapToSave;	//需要保存的变量
};
