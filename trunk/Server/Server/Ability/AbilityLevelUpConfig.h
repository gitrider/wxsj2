// AbilityLevelUpConfig.h
// 
//////////////////////////////////////////////////////

#ifndef __ABILITYLEVELUPCONFIG_H__
#define __ABILITYLEVELUPCONFIG_H__

#include "Type.h"

class AbilityLevelUpInfo
{
public:
	AbilityLevelUpInfo(VOID);
	~AbilityLevelUpInfo(VOID);

	BOOL Init(INT nLevel, 
			INT nDemandMoney, 
			INT nDemandExp, 
			INT nLimitAbilityExp, 
			INT nLimitAbilityExpShow, 
			INT nCurrentLevelAbilityExpTop, 
			INT nLimitLevel, 
			INT nExtraNeedMoney = 0, 
			INT iExtraExp = 0, 
			const CHAR* pszNpcName = NULL);

	VOID CleanUp(VOID);

	BOOL IsUsed(VOID)const
	{
		return m_bUsed;
	}

	INT GetLevel(VOID)const
	{
		return m_nLevel;
	}

	INT GetDemandMoney(VOID)const
	{
		return m_nDemandMoney;
	}

	INT GetDemandExp(VOID)const
	{
		return m_nDemandExp;
	}

	INT GetLimitAbilityExp(VOID)const
	{
		return m_nLimitAbilityExp;
	}

	INT GetLimitAbilityExpShow(VOID)const
	{
		return m_nLimitAbilityExpShow;
	}

	INT GetCurrentLevelAbilityExpTop(VOID)const
	{
		return m_nCurrentLevelAbilityExpTop;
	}

	INT GetLimitLevel(VOID)const
	{
		return m_nLimitLevel;
	}

	INT GetExtraNeedMoney(VOID)const
	{
		return m_iExtraNeedMoney;
	}

	INT GetExtraExp(VOID)const
	{
		return m_iExtraExp;
	}

	const CHAR* GetNpcName(VOID)const
	{
		return m_pszNpcName;
	}
protected:
	BOOL m_bUsed;						// �Ƿ��ǿ��õ�
	INT m_nLevel;						// �ȼ�
	INT m_nDemandMoney;					// �����Ǯ
	INT m_nDemandExp;					// ������
	INT m_nLimitAbilityExp;				// ��������������
	INT m_nLimitAbilityExpShow;			// ��������������(�ͻ�����ʾ)
	INT m_nCurrentLevelAbilityExpTop;	// ���������ȵ�����
	INT m_nLimitLevel;					// �ȼ�����
	INT	m_iExtraNeedMoney;				// ������Ҫ�Ľ�Ǯ
	INT	m_iExtraExp;					// ���Ӿ���ֵ
	CHAR m_pszNpcName[MAX_CHARACTER_NAME+1];			// npc ����
};

class AbilityLevelUpConfig
{
public:
	AbilityLevelUpConfig(VOID);
	~AbilityLevelUpConfig(VOID);

	BOOL Init(const CHAR *pszFileName);
	VOID CleanUp(VOID);

	const AbilityLevelUpInfo *GetLevelUpInfo(INT nLevel)const;

	BOOL LoadFromDB(const CHAR *pszPath);

	const CHAR *GetFileName(VOID)const
	{
		return &m_szFileName[0];
	}

	INT GetMaxLevel(VOID)const
	{
		return m_nMaxLevel;
	}

protected:
	BOOL SetFileName(const CHAR *pszFileName);

protected:
	CHAR m_szFileName[_MAX_PATH];
	INT m_nMaxLevel;
	INT m_nInfoCount;
	AbilityLevelUpInfo *m_paLevelUpInfos;
};


class AbilityLevelUpConfigManager
{
public:
	AbilityLevelUpConfigManager(VOID);
	~AbilityLevelUpConfigManager(VOID);

	BOOL Init(const CHAR *pszFilePath);
	VOID CleanUp(VOID);

	BOOL AddConfigName(const CHAR *pszFileName, INT *pnOutIndex);
	BOOL LoadAllLevelUpConfig(VOID);

	const AbilityLevelUpConfig *GetLevelUpConfig(INT nIndex)const
	{
		if(nIndex >= 0 && nIndex < m_nConfigCount)
		{
			return m_papConfigs[nIndex];
		}
		return NULL;
	}

protected:
	BOOL Resize(INT nSize);
	BOOL SetConfigPath(const CHAR *pszFilePath);

protected:
	CHAR m_szConfigPath[_MAX_PATH];
	INT m_nConfigCount;
	INT m_nSize;
	AbilityLevelUpConfig **m_papConfigs;
};

#endif // __ABILITYLEVELUPCONFIG_H__
