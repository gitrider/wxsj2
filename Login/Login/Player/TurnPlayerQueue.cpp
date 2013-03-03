#include "stdafx.h" 
#include "PlayerPool.h"
#include "TurnPlayerQueue.h"
#include "DB_Struct.h"
#include "Config.h"

WorldPlayerCounter	g_WorldPlayerCounter1;
WorldPlayerCounter	g_WorldPlayerCounter;

TurnPlayerQueue*	g_pProcessPlayerQueue = NULL;

WorldPlayerQueue*	g_pWorldPlayerQueue = NULL;

TurnPlayerQueue::TurnPlayerQueue()
{
	__ENTER_FUNCTION
	
	m_Size				=	MAX_TURN_PLAYER;
	m_pTurnPlayerQueue  =	new TURN_PLAYER_INFO[m_Size];
	m_Head	= 0;
    m_Tail	= 0;

	m_ValidCount		= 0;
	m_InvalidCount		= 0;
	m_pSortPlayerQueue	=	new TURN_PLAYER_INFO*[m_Size];

	__LEAVE_FUNCTION
}

TurnPlayerQueue::~TurnPlayerQueue()
{
	__ENTER_FUNCTION

	SAFE_DELETE_ARRAY(m_pTurnPlayerQueue);
	SAFE_DELETE_ARRAY(m_pSortPlayerQueue);

	__LEAVE_FUNCTION
}

BOOL TurnPlayerQueue::Init()
{
	g_WorldPlayerCounter1.m_MaxWorldUserCount = g_Config.m_ConfigInfo.m_nHashOnlineUserCount ;
	return TRUE;
}

BOOL TurnPlayerQueue::AddInPlayer(PlayerID_t pID,const CHAR* Name,UINT& QueuePos)
{

	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_pTurnPlayerQueue[m_Tail].Used)
	{//队列满
		return FALSE;
	}

	m_pTurnPlayerQueue[m_Tail].Used	= TRUE;
	m_pTurnPlayerQueue[m_Tail].PlayerID		= pID ;
	
	strncpy(m_pTurnPlayerQueue[m_Tail].PlayerName,Name,MAX_ACCOUNT);
	m_pTurnPlayerQueue[m_Tail].PlayerName[MAX_ACCOUNT] = '\0';
	m_pTurnPlayerQueue[m_Tail].QueuePos		= m_Tail;
	
	QueuePos = m_Tail;

	m_Tail ++ ;
	if( m_Tail>=m_Size ) 
		m_Tail = 0 ;

	return TRUE;
	
	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	TurnPlayerQueue::GetOutPlayer(PlayerID_t& pID,CHAR* NameBuff)
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_pTurnPlayerQueue[m_Head].Used== FALSE )
	{//没有队列玩家
		return FALSE ;
	}
	
	//获得玩家ID和名称
	Assert(NameBuff);
	strncpy(NameBuff,
			m_pTurnPlayerQueue[m_Head].PlayerName,
			MAX_ACCOUNT);
	NameBuff[MAX_ACCOUNT] = '\0';

	pID = m_pTurnPlayerQueue[m_Head].PlayerID ;

	//清空位置
	m_pTurnPlayerQueue[m_Head].PlayerName[0] = '\0';
	m_pTurnPlayerQueue[m_Head].Used = FALSE;
	
	m_Head ++ ;
	if( m_Head>=m_Size ) 
		m_Head = 0 ;


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

VOID	TurnPlayerQueue::SortQueuePos()
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	INT	nCount = GetCount();
	m_ValidCount = 0;
	m_InvalidCount = 0;

	UINT nPos = m_Head;
	for(INT i = 0; i < nCount; ++i)
	{
		LoginPlayer* pLoginPlayer =	g_pPlayerPool->GetPlayer(m_pTurnPlayerQueue[nPos].PlayerID);
		if(	(strcmp(pLoginPlayer->GetAccount(),m_pTurnPlayerQueue[nPos].PlayerName) == 0) &&
			pLoginPlayer->GetPlayerStatus() == PS_LOGIN_PROCESS_TURN )
		{//有效的LoginPlayer
			m_pSortPlayerQueue[m_ValidCount++] = &(m_pTurnPlayerQueue[nPos]);
		}
		else
		{
			m_InvalidCount++;
		}
		
		(nPos+1>=m_Size)?nPos=0:nPos++;
	}

	Assert(nCount>=(INT)m_ValidCount);
	for(INT i = 0; i < (INT)m_ValidCount; ++i)
	{
		TURN_PLAYER_INFO* pChild = m_pSortPlayerQueue[i];
		Assert(pChild);
		LoginPlayer* pLoginPlayer =	g_pPlayerPool->GetPlayer(pChild->PlayerID);
		Assert(pLoginPlayer);
		pLoginPlayer->SetQueuePos(i+1);	//排队队列从0开始，显示给玩家的位置从1开始
	}

	RePosQueue();
__LEAVE_FUNCTION
}

VOID	TurnPlayerQueue::RePosQueue()
{
__ENTER_FUNCTION
	if(m_InvalidCount < REPOS_INVALID_PLAYER) return;
	//AutoLock_T autolock(m_Lock);
	
	TURN_PLAYER_INFO* pNewQueue = new TURN_PLAYER_INFO[m_Size];
	Assert(pNewQueue);
	INT	nCount = GetCount();

	UINT nPos = m_Head;
	UINT nNewPos = 0;

	for(INT i = 0; i < nCount; ++i)
	{
		LoginPlayer* pLoginPlayer =	g_pPlayerPool->GetPlayer(m_pTurnPlayerQueue[nPos].PlayerID);
		if(	(strcmp(pLoginPlayer->GetAccount(),m_pTurnPlayerQueue[nPos].PlayerName) == 0) &&
			pLoginPlayer->GetPlayerStatus() == PS_LOGIN_PROCESS_TURN )
		{//有效的LoginPlayer
			pNewQueue[nNewPos++] = m_pTurnPlayerQueue[nPos];	//等同于memcpy
		}

		(nPos+1>=m_Size)?nPos=0:nPos++;
	}

	Assert(nNewPos == m_ValidCount);

	m_Head = 0;
	(nNewPos>=m_Size)?m_Tail=0:m_Tail=nNewPos;
	
	SAFE_DELETE_ARRAY(m_pTurnPlayerQueue);
	m_pTurnPlayerQueue = pNewQueue;
__LEAVE_FUNCTION
}

WorldPlayerQueue::WorldPlayerQueue()
{
	__ENTER_FUNCTION

	m_Size				=	MAX_WORLDTURN_PLAYER;
	m_pWorldPlayerQueue  =	new WORLD_PLAYER_INFO[m_Size];
	
	m_Head	= 0;
	m_Tail	= 0;
	m_SendCount = 0;

	m_ValidCount		= 0;
	m_InvalidCount		= 0;
	m_pSortPlayerQueue	=	new WORLD_PLAYER_INFO*[m_Size];
	__LEAVE_FUNCTION
}

WorldPlayerQueue::~WorldPlayerQueue()
{
	__ENTER_FUNCTION

		SAFE_DELETE_ARRAY(m_pWorldPlayerQueue);
	SAFE_DELETE_ARRAY(m_pSortPlayerQueue);

	__LEAVE_FUNCTION
}

BOOL WorldPlayerQueue::Init()
{
	g_WorldPlayerCounter.m_MaxWorldUserCount = g_Config.m_ConfigInfo.m_nHashOnlineUserCount ;
	return TRUE;
}

BOOL WorldPlayerQueue::AddInPlayer(PlayerID_t pID,const CHAR* Name,GUID_t	guid,UINT& QueuePos)
{

	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_pWorldPlayerQueue[m_Tail].Used)
	{//队列满
		return FALSE;
	}

	m_pWorldPlayerQueue[m_Tail].Used	= TRUE;
	m_pWorldPlayerQueue[m_Tail].PlayerID		= pID ;

	strncpy(m_pWorldPlayerQueue[m_Tail].PlayerName,Name,MAX_ACCOUNT);
	m_pWorldPlayerQueue[m_Tail].PlayerName[MAX_ACCOUNT] = '\0';
	m_pWorldPlayerQueue[m_Tail].QueuePos		= m_Tail;
	m_pWorldPlayerQueue[m_Tail].Guid			= guid;


	QueuePos = m_Tail;

	m_Tail ++ ;
	if( m_Tail>=m_Size ) 
		m_Tail = 0 ;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	WorldPlayerQueue::GetOutPlayer(UINT QueuePos)
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_pWorldPlayerQueue[m_Head].Used== FALSE )
	{//没有队列玩家
		return FALSE ;
	}
	Assert(QueuePos == m_Head);

	m_pWorldPlayerQueue[m_Head].Used = FALSE;

	m_Head ++ ;
	if( m_Head>=m_Size ) 
		m_Head = 0 ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}
BOOL	WorldPlayerQueue::FindHeadPlayer(UINT& QueuePos)
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_pWorldPlayerQueue[m_Head].Used== FALSE )
	{//没有队列玩家
		return FALSE ;
	}
	QueuePos = m_Head;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

WORLD_PLAYER_INFO&		WorldPlayerQueue::GetPlayer(UINT QueuePos)
{
	return m_pWorldPlayerQueue[QueuePos];
}

VOID	WorldPlayerQueue::SortQueuePos()
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	INT	nCount = GetCount();
	m_ValidCount = 0;
	m_InvalidCount = 0;

	UINT nPos = m_Head;
	for(INT i = 0; i < nCount; ++i)
	{
		LoginPlayer* pLoginPlayer =	g_pPlayerPool->GetPlayer(m_pWorldPlayerQueue[nPos].PlayerID);
		if(	pLoginPlayer->IsGUIDOwner(m_pWorldPlayerQueue[nPos].Guid) &&
			pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WORLD_PROCESS_TURN)
		{//有效的LoginPlayer
			m_pSortPlayerQueue[m_ValidCount++] = &(m_pWorldPlayerQueue[nPos]);
		}
		else
		{
			m_InvalidCount++;
		}

		(nPos+1>=m_Size)?nPos=0:nPos++;
	}

	Assert(nCount>=(INT)m_ValidCount);
	for(INT i = 0; i < (INT)m_ValidCount; ++i)
	{
		WORLD_PLAYER_INFO* pChild = m_pSortPlayerQueue[i];
		Assert(pChild);
		LoginPlayer* pLoginPlayer =	g_pPlayerPool->GetPlayer(pChild->PlayerID);
		Assert(pLoginPlayer);
		pLoginPlayer->SetQueuePos(i+1);	//排队队列从0开始，显示给玩家的位置从1开始
	}

	RePosQueue();
__LEAVE_FUNCTION
}

VOID	WorldPlayerQueue::RePosQueue()
{
__ENTER_FUNCTION
	if(m_InvalidCount < REPOS_INVALID_PLAYER) return;
	//AutoLock_T autolock(m_Lock);

	WORLD_PLAYER_INFO* pNewQueue = new WORLD_PLAYER_INFO[m_Size];
	Assert(pNewQueue);
	INT	nCount = GetCount();

	UINT nPos = m_Head;
	UINT nNewPos = 0;

	for(INT i = 0; i < nCount; ++i)
	{
		LoginPlayer* pLoginPlayer =	g_pPlayerPool->GetPlayer(m_pWorldPlayerQueue[nPos].PlayerID);
		if(	pLoginPlayer->IsGUIDOwner(m_pWorldPlayerQueue[nPos].Guid) &&
			pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WORLD_PROCESS_TURN)
		{//有效的LoginPlayer
			pNewQueue[nNewPos++] = m_pWorldPlayerQueue[nPos];	//等同于memcpy
		}

		(nPos+1>=m_Size)?nPos=0:nPos++;
	}

	Assert(nNewPos == m_ValidCount);

	m_Head = 0;
	(nNewPos>=m_Size)?m_Tail=0:m_Tail=nNewPos;

	SAFE_DELETE_ARRAY(m_pWorldPlayerQueue);
	m_pWorldPlayerQueue = pNewQueue;
__LEAVE_FUNCTION
}