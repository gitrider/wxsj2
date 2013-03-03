// AbilityLevelUpConfig.cpp
#include "stdafx.h"
#include "WXSJ_DBC.h"
#include "Log.h"

#include "AbilityLevelUpConfig.h"

using namespace DBC;


AbilityLevelUpInfo::AbilityLevelUpInfo(VOID)
{
__ENTER_FUNCTION
	m_bUsed = FALSE;
	m_nLevel = -1;
	m_nDemandMoney = -1;
	m_nDemandExp = -1;
	m_nLimitAbilityExp = -1;
	m_nLimitAbilityExpShow = -1;
	m_nCurrentLevelAbilityExpTop = -1;
	m_nLimitLevel = -1;
	m_iExtraNeedMoney =	-1;
	m_iExtraExp	=	-1;
	memset(m_pszNpcName, 0, MAX_CHARACTER_NAME+1);
__LEAVE_FUNCTION
}

AbilityLevelUpInfo::~AbilityLevelUpInfo(VOID)
{
}

BOOL AbilityLevelUpInfo::Init(INT nLevel, 
							INT nDemandMoney, 
							INT nDemandExp, 
							INT nLimitAbilityExp, 
							INT nLimitAbilityExpShow, 
							INT nCurrentLevelAbilityExpTop, 
							INT nLimitLevel, 
							INT nExtraNeedMoney, 
							INT iExtraExp, 
							const CHAR* pszNpcName)
{
__ENTER_FUNCTION
	m_bUsed = TRUE;
	m_nLevel = nLevel;
	m_nDemandMoney = nDemandMoney;
	m_nDemandExp = nDemandExp;
	m_nLimitAbilityExp = nLimitAbilityExp;
	m_nLimitAbilityExpShow = nLimitAbilityExpShow;
	m_nCurrentLevelAbilityExpTop = nCurrentLevelAbilityExpTop;
	m_nLimitLevel = nLimitLevel;
	m_iExtraNeedMoney = nExtraNeedMoney;
	m_iExtraExp	= iExtraExp;
	if(pszNpcName)
	{
		strncpy(m_pszNpcName, pszNpcName, strlen(pszNpcName));
	}
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

VOID AbilityLevelUpInfo::CleanUp(VOID)
{
__ENTER_FUNCTION
	m_bUsed = FALSE;
	m_nLevel = -1;
	m_nDemandMoney = -1;
	m_nDemandExp = -1;
	m_nLimitAbilityExp = -1;
	m_nLimitAbilityExpShow = -1;
	m_nCurrentLevelAbilityExpTop = -1;
	m_nLimitLevel = -1;
	m_iExtraNeedMoney = -1;
	m_iExtraExp	=	-1;
	memset(m_pszNpcName, 0, MAX_CHARACTER_NAME+1);
__LEAVE_FUNCTION
}


AbilityLevelUpConfig::AbilityLevelUpConfig(VOID)
{
__ENTER_FUNCTION
	memset(m_szFileName, 0, sizeof(m_szFileName));
	m_nMaxLevel = 0;
	m_nInfoCount = 0;
	m_paLevelUpInfos = NULL;
__LEAVE_FUNCTION
}

AbilityLevelUpConfig::~AbilityLevelUpConfig(VOID)
{
__ENTER_FUNCTION
	Assert(NULL == m_paLevelUpInfos);
__LEAVE_FUNCTION
}

BOOL AbilityLevelUpConfig::Init(const CHAR *pszFileName)
{
__ENTER_FUNCTION
	BOOL bRet = SetFileName(pszFileName);
	if(!bRet)
	{
		return FALSE;
	}
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

VOID AbilityLevelUpConfig::CleanUp(VOID)
{
__ENTER_FUNCTION
	if(NULL != m_paLevelUpInfos)
	{
		delete [] m_paLevelUpInfos;
		m_paLevelUpInfos = NULL;
	}
	m_nMaxLevel = 0;
	m_nInfoCount = 0;

	memset(m_szFileName, 0, sizeof(m_szFileName));
__LEAVE_FUNCTION
}

const AbilityLevelUpInfo *AbilityLevelUpConfig::GetLevelUpInfo(INT nLevel)const
{
__ENTER_FUNCTION
	if(0 <= nLevel && m_nInfoCount > nLevel)
	{
		if(m_paLevelUpInfos[nLevel].IsUsed())
		{
			return &m_paLevelUpInfos[nLevel];
		}
	}
	return NULL;
__LEAVE_FUNCTION
	return NULL;
}

BOOL AbilityLevelUpConfig::LoadFromDB(const CHAR *pszPath)
{
__ENTER_FUNCTION
	if(NULL == pszPath)
	{
		return FALSE;
	}

	INT nPathLen = (INT)(strlen(pszPath));
	if(nPathLen <= 0)
	{
		return FALSE;
	}

	if(strlen(m_szFileName) <= 0)
	{
		return FALSE;
	}

	DBCFile ThirdFile(0);

	CHAR szPathName[_MAX_PATH];
	memset(szPathName, 0, sizeof(szPathName));
	strncpy(szPathName, pszPath, sizeof(szPathName));
	if(pszPath[nPathLen - 1] != '\\' && pszPath[nPathLen - 1] != '/')
	{
		strncat(szPathName, "\\", sizeof(szPathName));
	}
	strncat(szPathName, m_szFileName, sizeof(szPathName));
	szPathName[_MAX_PATH - 1] = '\0';

	//INT cI;
	//for(cI = 0; _MAX_PATH > cI; cI++)
	//{
	//	if('/' == szPathName[cI])
	//	{
	//		szPathName[cI] = '\\';
	//	}
	//	else if('\0' == szPathName[cI])
	//	{
	//		break;
	//	}
	//}

	BOOL ret = ThirdFile.OpenFromTXT(szPathName);

	INT nTableCount = ThirdFile.GetRecordsNum();
	INT i;
	m_nMaxLevel = 0;
	for(i = 0; nTableCount > i; i++)
	{
		INT nLevel = ThirdFile.Search_Posistion(i, 0)->iValue;
		if(nLevel > m_nMaxLevel)
		{
			m_nMaxLevel = nLevel;
		}
	}

	if(m_nMaxLevel > 0)
	{
		m_nInfoCount = m_nMaxLevel + 1;
	}
	else
	{
		m_nInfoCount = 0;
	}

	if(0 < m_nInfoCount)
	{
		m_paLevelUpInfos = new AbilityLevelUpInfo [m_nInfoCount];
	}
	else
	{
		m_paLevelUpInfos = NULL;
	}

	AbilityLevelUpInfo *pAbilityLevelUpInfo;
	INT nLevel;
	INT nDemandMoney;
	INT nDemandExp;
	INT nLimitAbilityExp;
	INT nLimitAbilityExpShow;
	INT nCurrentLevelAbilityExpTop;
	INT nLimitLevel;
	INT	iExtraNeedMoney;
	INT	iExtraExp;
	const CHAR* pszNpcName = NULL;

	INT nColumnIndex;
	for(i = 0; nTableCount > i; i++)
	{
		nColumnIndex = 0;
		nLevel = ThirdFile.Search_Posistion(i, nColumnIndex++)->iValue;
		if(nLevel < 0 || nLevel >= m_nInfoCount)
		{
			continue;
		}

		pAbilityLevelUpInfo = &(m_paLevelUpInfos[nLevel]);

		// Ãû³Æ
		nColumnIndex++;
		nDemandMoney = ThirdFile.Search_Posistion(i, nColumnIndex++)->iValue;
		nDemandExp = ThirdFile.Search_Posistion(i, nColumnIndex++)->iValue;
		nLimitAbilityExp = ThirdFile.Search_Posistion(i, nColumnIndex++)->iValue;
		nLimitAbilityExpShow = ThirdFile.Search_Posistion(i, nColumnIndex++)->iValue;
		nCurrentLevelAbilityExpTop = ThirdFile.Search_Posistion(i, nColumnIndex++)->iValue;
		nLimitLevel = ThirdFile.Search_Posistion(i, nColumnIndex++)->iValue;
		pszNpcName = ThirdFile.Search_Posistion(i,nColumnIndex++)->pString;
		iExtraNeedMoney	=	ThirdFile.Search_Posistion(i, nColumnIndex++)->iValue;
		iExtraExp	=	ThirdFile.Search_Posistion(i, nColumnIndex++)->iValue;
		
		BOOL bRet = pAbilityLevelUpInfo->Init(nLevel, 
											nDemandMoney, 
											nDemandExp, 
											nLimitAbilityExp, 
											nLimitAbilityExpShow, 
											nCurrentLevelAbilityExpTop, 
											nLimitLevel, 
											iExtraNeedMoney, 
											iExtraExp,
											pszNpcName);
		if(!bRet)
		{
			pAbilityLevelUpInfo->CleanUp();
			continue;
		}
	}

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL AbilityLevelUpConfig::SetFileName(const CHAR *pszFileName)
{
__ENTER_FUNCTION
	if(NULL == pszFileName)
	{
		return FALSE;
	}
	memset(m_szFileName, 0, sizeof(m_szFileName));
	strncpy(m_szFileName, pszFileName, sizeof(m_szFileName));
	m_szFileName[sizeof(m_szFileName) - 1] = '\0';
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}


AbilityLevelUpConfigManager::AbilityLevelUpConfigManager(VOID)
{
__ENTER_FUNCTION
	memset(m_szConfigPath, 0, sizeof(m_szConfigPath));
	m_nConfigCount = 0;
	m_nSize = 0;
	m_papConfigs = NULL;
__LEAVE_FUNCTION
}

AbilityLevelUpConfigManager::~AbilityLevelUpConfigManager(VOID)
{
__ENTER_FUNCTION
	Assert(NULL == m_papConfigs);
__LEAVE_FUNCTION
}

BOOL AbilityLevelUpConfigManager::Init(const CHAR *pszFilePath)
{
__ENTER_FUNCTION
	BOOL bRet = SetConfigPath(pszFilePath);
	if(!bRet)
	{
		return FALSE;
	}
	m_papConfigs = NULL;
	m_nConfigCount = 0;
	m_nSize = 0;
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

VOID AbilityLevelUpConfigManager::CleanUp(VOID)
{
__ENTER_FUNCTION
	if(NULL != m_papConfigs)
	{
		INT i;
		for(i = 0; m_nConfigCount > i; i++)
		{
			if(NULL != m_papConfigs[i])
			{
				delete m_papConfigs[i];
				m_papConfigs[i] = NULL;
			}
		}
		delete [] m_papConfigs;
		m_papConfigs = NULL;
	}
	m_nConfigCount = 0;
	m_nSize = 0;

	memset(m_szConfigPath, 0, sizeof(m_szConfigPath));
__LEAVE_FUNCTION
}

BOOL AbilityLevelUpConfigManager::AddConfigName(const CHAR *pszFileName, INT *pnOutIndex)
{
__ENTER_FUNCTION
	if(NULL == pszFileName)
	{
		return FALSE;
	}

	if(strlen(pszFileName) <= 0)
	{
		return FALSE;
	}

	INT i;
	for(i = 0; i < m_nConfigCount; i++)
	{
		if(NULL != m_papConfigs[i] && 0 == strcmp(pszFileName, m_papConfigs[i]->GetFileName()))
		{
			if(NULL != pnOutIndex)
			{
				*pnOutIndex = i;
			}
			return TRUE;
		}
	}

	if(m_nConfigCount >= m_nSize)
	{
		INT nAddSize = m_nSize;
		if(nAddSize > 128)
		{
			nAddSize = 128;
		}
		else if(nAddSize < 8)
		{
			nAddSize = 8;
		}

		if(!Resize(nAddSize + m_nSize))
		{
			return FALSE;
		}
	}

	AbilityLevelUpConfig *pLevelUpConfig = new AbilityLevelUpConfig;
	BOOL bRet = pLevelUpConfig->Init(pszFileName);
	if(!bRet)
	{
		pLevelUpConfig->CleanUp();
		delete pLevelUpConfig;
		pLevelUpConfig = NULL;
		return FALSE;
	}

	if(NULL != pnOutIndex)
	{
		*pnOutIndex = m_nConfigCount;
	}
	m_papConfigs[m_nConfigCount++] = pLevelUpConfig;
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL AbilityLevelUpConfigManager::LoadAllLevelUpConfig(VOID)
{
__ENTER_FUNCTION
	INT i;
	AbilityLevelUpConfig *pAbilityLevelUpConfig;
	for(i = 0; i < m_nConfigCount; i++)
	{
		pAbilityLevelUpConfig = m_papConfigs[i];
		if(NULL != pAbilityLevelUpConfig)
		{
			if(!pAbilityLevelUpConfig->LoadFromDB(&m_szConfigPath[0]))
			{
				CHAR szOutLog[1024];
				if(NULL != pAbilityLevelUpConfig->GetFileName() && strlen(pAbilityLevelUpConfig->GetFileName()) < 512)
				{
					sprintf(szOutLog, "Load LoadAllAbilityLevelUpConfig ... Failed! (FileName=%s)", pAbilityLevelUpConfig->GetFileName());
				}
				else
				{
					sprintf(szOutLog, "Load LoadAllAbilityLevelUpConfig ... Failed! (Index=%d)", i);
				}
				DiskLog(SERVER_LOGFILE, szOutLog);
				return FALSE;
			}
		}
	}
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL AbilityLevelUpConfigManager::Resize(INT nSize)
{
__ENTER_FUNCTION
	if(nSize <= m_nSize)
	{
		return FALSE;
	}
	AbilityLevelUpConfig **pSaveConfigs = m_papConfigs;
	m_papConfigs = new AbilityLevelUpConfig* [nSize];
	Assert(NULL != m_papConfigs);
	memcpy(m_papConfigs, pSaveConfigs, sizeof(AbilityLevelUpConfig*) * m_nSize);
	memset(&(m_papConfigs[m_nSize]), 0, sizeof(AbilityLevelUpConfig*) * (nSize - m_nSize));
	delete [] pSaveConfigs;
	pSaveConfigs = NULL;
	m_nSize = nSize;
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL AbilityLevelUpConfigManager::SetConfigPath(const CHAR *pszFilePath)
{
__ENTER_FUNCTION
	if(NULL == pszFilePath)
	{
		return FALSE;
	}
	memset(m_szConfigPath, 0, sizeof(m_szConfigPath));
	strncpy(m_szConfigPath, pszFilePath, sizeof(m_szConfigPath));
	m_szConfigPath[sizeof(m_szConfigPath) - 1] = '\0';
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}
