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
	BOOL m_bUsed;						// 是否是可用的
	INT m_nLevel;						// 等级
	INT m_nDemandMoney;					// 需求金钱
	INT m_nDemandExp;					// 需求经验
	INT m_nLimitAbilityExp;				// 技能熟练度限制
	INT m_nLimitAbilityExpShow;			// 技能熟练度限制(客户端显示)
	INT m_nCurrentLevelAbilityExpTop;	// 本级熟练度的上限
	INT m_nLimitLevel;					// 等级限制
	INT	m_iExtraNeedMoney;				// 附加需要的金钱
	INT	m_iExtraExp;					// 附加经验值
	CHAR m_pszNpcName[MAX_CHARACTER_NAME+1];			// npc 名称
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
