/********************************************************************
	
	��������:	2005��12��20��
	����ʱ��:	13:59
	�ļ�����:	TurnPlayerQueue
	�ļ�·��:	d:\Prj\Login\Login\Main
	������:		������
	�ļ����ܣ�����ŶӶ���
	�޸ļ�¼��
*********************************************************************/


#ifndef _TURN_PLAYER_QUEUE_H_
#define _TURN_PLAYER_QUEUE_H_
#include "Type.h"



#define MAX_TURN_PLAYER			2048
#define MAX_WORLDTURN_PLAYER	1024
#define REPOS_INVALID_PLAYER	200		//�Ƿ����ݵ�һ��������ʱ�����Ŷ���

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
 *	����Ŷӵ��Ƚ��ȳ�
 */
class TurnPlayerQueue
{
public:
	TurnPlayerQueue();
	~TurnPlayerQueue();

	
	BOOL					Init();
	//���һ����ҵ�����ĩβ
	BOOL					AddInPlayer(PlayerID_t pID,const CHAR* Name,UINT& QueuePos);
	//ȡ������ͷ�����,���ҽ����Ӷ�����ɾ��
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

	UINT					m_ValidCount;			//��Ч��������
	UINT					m_InvalidCount;			//��Ч��������
	TURN_PLAYER_INFO**		m_pSortPlayerQueue;		//λ�ö���

	MyLock					m_Lock ;
};



class WorldPlayerQueue
{
public:
	WorldPlayerQueue();
	~WorldPlayerQueue();


	BOOL					Init();

	//���һ����ҵ�����ĩβ
	BOOL					AddInPlayer(PlayerID_t pID,const CHAR* Name,GUID_t guid,UINT& QueuePos);

	//ȡ������ͷ�����,���ҽ����Ӷ�����ɾ��
	BOOL					GetOutPlayer(UINT QueuePos);
	//ȡ������ͷ��ҵı��
	BOOL					FindHeadPlayer(UINT& QueuePos);
	
	//ȡ��һ����ҵ�����
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

	UINT					m_ValidCount;			//��Ч��������
	UINT					m_InvalidCount;			//��Ч��������
	WORLD_PLAYER_INFO**		m_pSortPlayerQueue;		//λ�ö���

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

extern WorldPlayerCounter	g_WorldPlayerCounter1;		//ProcessManager�����߳��Ŷ�ʹ��
extern WorldPlayerCounter	g_WorldPlayerCounter;		//ServerManager�����߳��Ŷ�ʹ��
//ProcessManager �������Ŷ�״̬����Ҷ���
extern TurnPlayerQueue*	g_pProcessPlayerQueue;
//��ProcessManager ��World ֮���ŶӵĶ���
extern WorldPlayerQueue*	g_pWorldPlayerQueue;

#endif