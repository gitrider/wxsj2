/********************************************************************
	
	创建日期:	2005年12月20日
	创建时间:	13:59
	文件名称:	TurnPlayerQueue
	文件路径:	d:\Prj\Login\Login\Main
	创建人:		曾鹏翔
	文件功能：玩家排队队列
	修改纪录：
*********************************************************************/


#ifndef _TURN_PLAYER_QUEUE_H_
#define _TURN_PLAYER_QUEUE_H_
#include "Type.h"



#define MAX_TURN_PLAYER			2048
#define MAX_WORLDTURN_PLAYER	1024
#define REPOS_INVALID_PLAYER	200		//非法数据到一定数量的时候重排队列

struct FULLUSERDATA;

struct TURN_PLAYER_INFO
{
	PlayerID_t	PlayerID;
	CHAR		PlayerName[MAX_ACCOUNT+1];
	BOOL		Used;
	UINT        QueuePos;

	TURN_PLAYER_INFO()
	{
		PlayerID	= INVALID_ID;
		PlayerName[0]	= '\0';
		Used		= FALSE;
		QueuePos	= 0;
	}
};

struct WORLD_PLAYER_INFO
{
	PlayerID_t		PlayerID;
	CHAR			PlayerName[MAX_ACCOUNT+1];
	BOOL			Used;
	GUID_t			Guid;
	SceneID_t		SID;
	UINT			QueuePos;
	WORLD_PLAYER_INFO()
	{
		PlayerID	= INVALID_ID;
		PlayerName[0]	= '\0';
		Used		= FALSE;
		QueuePos	= 0;
		Guid		= INVALID_ID;
		SID			= INVALID_ID;
	}

	VOID	CleanUp()
	{
		PlayerName[0]	= '\0';
		Used		= FALSE;
		QueuePos	= 0;
		Guid		= INVALID_ID;
	
	}
};

/*
 *	玩家排队的先进先出
 */
class TurnPlayerQueue
{
public:
	TurnPlayerQueue();
	~TurnPlayerQueue();

	
	BOOL					Init();
	//添加一个玩家到队列末尾
	BOOL					AddInPlayer(PlayerID_t pID,const CHAR* Name,UINT& QueuePos);
	//取出队列头的玩家,并且将它从队列中删除
	BOOL					GetOutPlayer(PlayerID_t& pID,CHAR* NameBuff);					
	
	BOOL					IsEmpty() {return m_Head == m_Tail;}
	UINT					GetCount() 
	{
		if(m_Head<=m_Tail)
		{
			return m_Tail - m_Head;
		}
		else
		{
			return m_Size - (m_Head - m_Tail);
		}
	} 

	UINT					GetHead() {return m_Head;}

	VOID					SortQueuePos();
	UINT					GetValidCount(){return m_ValidCount;}
	UINT					GetInvalidCount(){return m_InvalidCount;}
private:
	VOID					RePosQueue();
private:
	TURN_PLAYER_INFO*		m_pTurnPlayerQueue;
	
	UINT					m_Size ;
	uint					m_Head ;
	uint					m_Tail ;

	UINT					m_ValidCount;			//有效数据数量
	UINT					m_InvalidCount;			//无效数据数量
	TURN_PLAYER_INFO**		m_pSortPlayerQueue;		//位置队列

	MyLock					m_Lock ;
};



class WorldPlayerQueue
{
public:
	WorldPlayerQueue();
	~WorldPlayerQueue();


	BOOL					Init();

	//添加一个玩家到队列末尾
	BOOL					AddInPlayer(PlayerID_t pID,const CHAR* Name,GUID_t guid,UINT& QueuePos);

	//取出队列头的玩家,并且将它从队列中删除
	BOOL					GetOutPlayer(UINT QueuePos);
	//取出队列头玩家的编号
	BOOL					FindHeadPlayer(UINT& QueuePos);
	
	//取出一个玩家的数据
	WORLD_PLAYER_INFO&		GetPlayer(UINT QueuePos);
	

	BOOL					IsEmpty() {return m_Head == m_Tail;}
	UINT					GetCount() 
	{
		if(m_Head<=m_Tail)
		{
			return m_Tail - m_Head;
		}
		else
		{
			return m_Size - (m_Head - m_Tail);
		}
	} 

	UINT					GetHead() {return m_Head;}
	UINT					GetSendCount(){return m_SendCount;}
	VOID					ResetSendCount(){m_SendCount=0;}
	VOID					AddSendCount(){m_SendCount++;}
	VOID					SortQueuePos();
	UINT					GetValidCount(){return m_ValidCount;}
	UINT					GetInvalidCount(){return m_InvalidCount;}
private:
	VOID					RePosQueue();
private:
	WORLD_PLAYER_INFO*		m_pWorldPlayerQueue;
	UINT					m_Size ;
	uint					m_Head ;
	uint					m_Tail ;

	UINT					m_ValidCount;			//有效数据数量
	UINT					m_InvalidCount;			//无效数据数量
	WORLD_PLAYER_INFO**		m_pSortPlayerQueue;		//位置队列

	uint					m_SendCount;
	MyLock					m_Lock ;
};


class WorldPlayerCounter
{
public:
	WorldPlayerCounter()
	{
		m_WorldPlayerCount	 =	0;
		m_RequirePlayerCount =  1;
		m_MaxWorldUserCount = 0 ;
	}

	~WorldPlayerCounter()
	{

	}

	INT			m_MaxWorldUserCount ;
	INT			m_WorldPlayerCount;
	INT			m_RequirePlayerCount;

	UINT			GetRequirePlayerCount()
	{
		INT ReqPlayer =  m_MaxWorldUserCount - m_WorldPlayerCount;
		if(ReqPlayer>0)
			return ReqPlayer;
		return 0;
	}
	
};

extern WorldPlayerCounter	g_WorldPlayerCounter1;		//ProcessManager所在线程排队使用
extern WorldPlayerCounter	g_WorldPlayerCounter;		//ServerManager所在线程排队使用
//ProcessManager 中属于排队状态的玩家队列
extern TurnPlayerQueue*	g_pProcessPlayerQueue;
//由ProcessManager 到World 之间排队的队列
extern WorldPlayerQueue*	g_pWorldPlayerQueue;

#endif