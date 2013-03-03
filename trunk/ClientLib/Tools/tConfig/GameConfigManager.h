#pragma once
#include "GameConfig.h"

class CGameConfigManager
{
public:
	CGameConfigManager(void);
	~CGameConfigManager(void);

public:
	//�������������ļ������������ļ���
	int	LoadAllConfigFile(void);
	//�������µ�һ�������ļ�
	CGameConfig* GetLastestConfig(void);
	//����ĳֵ
	void SetValue(const char* szValueName, int nValue);
	//����ĳֵ
	void SetValue(const char* szValueName, char* szValue);
	//�Ƿ����ø���
	bool NeedUpdate(void);
	//�����޸�
	void SaveModifed(void);
	//ĳ�����Ƿ��޸Ĺ�
	bool IsVariableModifyed(const char* szName);
	//ȡ���޸ĺ��ֵ
	const char* GetModifyedValue(const char* szName);
	//ɾ�����������ļ�
	void DeletePrivateFile(void);

protected:
	void _tryAddConfigPath(const char* szConfigPath);
	void _tryAddConfigFile(const char* szConfigFile);

protected:
	typedef std::map< ULONGLONG, CGameConfig* > GAMECONFIG_BUF;

	GAMECONFIG_BUF	m_allConfigFile;	//���е������ļ�
	CGameConfig*	m_pLastestConfig;	//�����������µ������ļ�

	CONFIG_BUF		m_mapToSave;	//��Ҫ����ı���
};
