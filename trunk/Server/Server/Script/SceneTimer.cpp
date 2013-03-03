//-----------------------------------------------------------------------------
// �ļ��� : SceneTimer.cpp
// ģ��	:	Script
// ����	 :  �����ڳ����ļ�ʱ��,�ű�ϵͳר��
// �޸���ʷ:
//	yangfei����
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "SceneTimer.h"
#include "LuaInterface.h"
#include "TimeManager.h"
#include "Scene.h"

SceneTimer::SceneTimer()
{
	head = NULL;
	index = 0;
	m_pTimers=NULL ;
	m_pSystemTimers = NULL;
	maxCount=0;
	m_pScene=NULL ;

#if _DEBUG
	m_currentUse = 0;
#endif
}

SceneTimer::~SceneTimer()
{
	SAFE_DELETE_ARRAY(m_pTimers);
	SAFE_DELETE_ARRAY(m_pSystemTimers);
}

VOID SceneTimer::CleanUp( )
{
	for(INT i=0;i<maxCount;i++)
	{
		InitOne(i);
		InitSystemOne(i);
	}

	UINT Now = g_pTimeManager->CurrentTime() ;
	mRefeshTimer.BeginTimer(1000,Now);
}

//����ʱ��ʼ�����еļ�ʱ��
VOID SceneTimer::CreateTimer(INT maxCount,Scene* pScene)
{
	this->maxCount = maxCount;

	SAFE_DELETE_ARRAY(m_pTimers);
	SAFE_DELETE_ARRAY(m_pSystemTimers);
	m_pTimers = new MyTimer[maxCount]; 
	m_pSystemTimers = new MyTimer[maxCount]; 

	for(INT i=0;i<maxCount;i++)
	{
		InitOne(i);
		InitSystemOne(i);
	}

	UINT Now = g_pTimeManager->CurrentTime() ;
	mRefeshTimer.BeginTimer(1000,Now);

	m_pScene = pScene;
}

VOID SceneTimer::InitSystemOne(INT i)
{
	Assert(i>=0 && i<maxCount);

	if(i>=0 && i<maxCount)
	{
		memset(m_pSystemTimers[i].scriptFuncName,0,FuncNameLen);
		m_pSystemTimers[i].scriptId = m_pSystemTimers[i].selfId = -1;
		m_pSystemTimers[i].timer.CleanUp();
	}
}


VOID SceneTimer::InitOne(INT i)
{
	Assert(i>=0 && i<maxCount);

	if(i>=0 && i<maxCount)
	{
		memset(m_pTimers[i].scriptFuncName,0,FuncNameLen);
		m_pTimers[i].scriptId = m_pTimers[i].selfId = -1;
		m_pTimers[i].timer.CleanUp();
	}
}

INT SceneTimer::GetTimer()
{
	if(head == NULL)
	{
		if(index < maxCount)
			return index++;
		else
			return -1;
	}
	else
	{	
		MyTimer* temp = head;
		head = *((MyTimer**)head);

		return (INT)( temp -(MyTimer*)m_pTimers);
	}
}

VOID SceneTimer::FreeTimer(INT index)
{
	if( index <0 || index >= maxCount )
	{
#ifdef _DEBUG
		CHAR szTemp[MAX_FILE_PATH];
		sprintf(szTemp,"index = %d",index);
		AssertEx(FALSE,szTemp) ;
#endif
		return ;
	}

	InitOne(index);//�ͷ�ʱ��ʼ��

	MyTimer* temp = (MyTimer*)&(m_pTimers[index]);

	*((MyTimer**)temp) =  head;

	head = temp;	


#if _DEBUG
	m_currentUse--;
#endif

}


BOOL SceneTimer::CheckTimer(INT index)
{
	if( index <0 || index >= maxCount )
	{
		Assert(FALSE) ;
		return FALSE ;
	}

	MyTimer* temp = (MyTimer*)&(m_pTimers[index]);

	return temp->timer.IsSetTimer() ;
}

VOID SceneTimer::StartTheTimer(INT sceneTimerIndex,INT selfId,INT scriptId,const CHAR* funcName,INT tickTime)
{
	if( sceneTimerIndex <0 || sceneTimerIndex >= maxCount )
	{
		Assert(FALSE) ;
		return ;
	}

	size_t len = min(FuncNameLen - 1,strlen(funcName) );
	strncpy(m_pTimers[sceneTimerIndex].scriptFuncName,funcName,len);
	m_pTimers[sceneTimerIndex].selfId = selfId;
	m_pTimers[sceneTimerIndex].scriptId = scriptId;

	UINT uNow = g_pTimeManager->CurrentTime();
	m_pTimers[sceneTimerIndex].timer.BeginTimer(tickTime,uNow);


#if _DEBUG
	m_currentUse++;
#endif
}


VOID SceneTimer::StartSystemTimer(INT TimerIndex,INT scriptId,const CHAR* funcName,INT tickTime)
{
	if( TimerIndex <0 || TimerIndex >= maxCount )
	{
		Assert(FALSE && "ϵͳ��ʱ����������") ;
		return ;
	}

	size_t len = min(FuncNameLen - 1,strlen(funcName) );
	strncpy(m_pSystemTimers[TimerIndex].scriptFuncName,funcName,len);
	m_pSystemTimers[TimerIndex].selfId = INVALID_ID;
	m_pSystemTimers[TimerIndex].scriptId = scriptId;

	UINT uNow = g_pTimeManager->CurrentTime();
	m_pSystemTimers[TimerIndex].timer.BeginTimer(tickTime,uNow);

}


VOID SceneTimer::OnTimer(UINT uTime)
{
__ENTER_FUNCTION

	for(INT i=0;i<maxCount;i++)
	{
		_MY_TRY
		{
			if(m_pTimers[i].timer.IsSetTimer())
			{
				if( m_pTimers[i].timer.CountingTimer(uTime ) )
					GetScene()->GetLuaInterface()->ExeScript_DDDD(	
							m_pTimers[i].scriptId,
							m_pTimers[i].scriptFuncName,
							(INT)GetScene()->SceneID(),
							(INT)m_pTimers[i].selfId,
							(INT)i,
							(INT)0 ) ;
			}

			//����ϵͳ��ʱ��
			if(m_pSystemTimers[i].timer.IsSetTimer())
			{
				if( m_pSystemTimers[i].timer.CountingTimer(uTime ) )
				{
					GetScene()->GetLuaInterface()->ExeScript_DDD(	
							m_pSystemTimers[i].scriptId,
							m_pSystemTimers[i].scriptFuncName,
							(INT)GetScene()->SceneID(),
							(INT)i,
							(INT)uTime ) ;
					InitSystemOne(i);	//�ͷ�ʱ��ʼ��				
				}
			}
		}
		_MY_CATCH
		{
			SaveCodeLog( ) ;
		}
	}

__LEAVE_FUNCTION
}