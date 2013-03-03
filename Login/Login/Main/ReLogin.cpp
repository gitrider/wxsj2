#include "stdafx.h"
#include "ReLogin.h"
#include "Log.h"
#include "Config.h"
#include "TimeManager.h"

ReLoginManager*					g_pReLoginManager = NULL;

tag_ReLoginUnit::tag_ReLoginUnit()
{
	CleanUp();
}

VOID tag_ReLoginUnit::CleanUp()
{
	m_LoginTime = 0;
	m_LoginCount = MAX_RELOGIN_COUNT_PERHOUR;
}

//////////////////////////////////////////////////////////////////////////
BOOL	ReLoginManager::Init()
{
__ENTER_FUNCTION
	m_LoginAccTable.InitTable(MAX_RELOGIN_ACCNUM,MAX_ACCOUNT_LENGTH);
	m_CurPos = 0;
__LEAVE_FUNCTION
	return TRUE;
}

BOOL	ReLoginManager::IsAccCanReLogin(const CHAR* szAccName)
{
__ENTER_FUNCTION
	INT nEnable = g_Config.m_LoginInfo.ReLoginLimit;
	if(nEnable<=0)	return TRUE;

	if(NULL == szAccName || 0 == szAccName[0])	
		return FALSE;

	ReLoginUnit* pUnit = (ReLoginUnit*)(m_LoginAccTable.Get(szAccName));
	if(NULL == pUnit)
	{
		INT prePos = m_CurPos;
		m_CurPos = (m_CurPos+1==MAX_RELOGIN_ACCNUM)?0:m_CurPos+1;

		m_LoginUnit[prePos].CleanUp();
		m_LoginUnit[prePos].m_LoginTime = g_pTimeManager->CurrentTime();
		m_LoginUnit[prePos].m_LoginCount--;

		return m_LoginAccTable.Add(szAccName,&(m_LoginUnit[prePos]));
	}
	else
	{
		UINT nPreLoginTime = pUnit->m_LoginTime;
		INT nPreLoginCount = pUnit->m_LoginCount;
		UINT uTime = g_pTimeManager->CurrentTime();

		if(nPreLoginTime+MIN_RELOGIN_TIME_GAP>uTime)
		{
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:ReLoginManager::IsAccCanReLogin...acc=%s must wait %dms!!!", szAccName,MIN_RELOGIN_TIME_GAP);
			return FALSE;
		}

		pUnit->m_LoginTime = uTime;

		if(nPreLoginCount <= 0)
		{
			g_pLog->FastSaveLog(LOG_FILE_0, "ERROR:ReLoginManager::IsAccCanReLogin...acc=%s nPreLoginCount is Empty!!!", szAccName);
			return FALSE;
		}

		pUnit->m_LoginCount = --nPreLoginCount;
		return TRUE;
	}
__LEAVE_FUNCTION
	return FALSE;
}

BOOL	ReLoginManager::HeartBeat()
{
__ENTER_FUNCTION
	UINT uTime = g_pTimeManager->CurrentTime();

	if(!m_CheckOldTimer.IsSetTimer())
	{
		m_CheckOldTimer.BeginTimer(RELOGIN_CLEAR_INTERVAL,uTime);
	}
	
	//每2秒检查一下是不是需要重设登录时间
	if(m_CheckOldTimer.CountingTimer(uTime))
	{
		INT nEmptyCount = 0;
		for(UINT i = 0; i < MAX_RELOGIN_ACCNUM; ++i)
		{
			ReLoginUnit* pUnit = (ReLoginUnit*)(m_LoginAccTable.GetByPos(i));
			if(pUnit)
			{
				if((pUnit->m_LoginTime)+MIN_RELOGIN_TIME_GAP<uTime && 0 != pUnit->m_LoginTime)
				{
					pUnit->m_LoginTime = 0;
					g_pLog->FastSaveLog(LOG_FILE_0, "ReLoginManager::HeartBeat Clear ReLoginUnit Pos=%u m_LoginTime...OK", i);
				}
			}
			else
			{
				nEmptyCount++;
			}
		}

		if(nEmptyCount<=20)
		{
			m_LoginAccTable.CleanUp();
			g_pLog->FastSaveLog(LOG_FILE_0, "ReLoginManager::HeartBeat Rest All AccTable...OK");
		}
	}

	//每小时清空一次登录帐号限制列表
	if(!m_RestCountTimer.IsSetTimer())
	{
		m_RestCountTimer.BeginTimer(RESET_RELOGIN_COUNT_TIME,uTime);
	}

	if(m_RestCountTimer.CountingTimer(uTime))
	{
		for(UINT i = 0; i < MAX_RELOGIN_ACCNUM; ++i)
		{
			ReLoginUnit* pUnit = (ReLoginUnit*)(m_LoginAccTable.GetByPos(i));
			if(pUnit)
			{
				pUnit->m_LoginCount = MAX_RELOGIN_COUNT_PERHOUR;
				g_pLog->FastSaveLog(LOG_FILE_0, "ReLoginManager::HeartBeat Reset ReLoginUnit Pos=%u m_LoginCount...OK", i);
			}
		}
	}
__LEAVE_FUNCTION
	return TRUE;
}