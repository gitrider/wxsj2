/********************************************************************
	
	��������:	2005��12��19��
	����ʱ��:	10:21
	�ļ�����:	DBLogicManager
	�ļ�·��:	d:\Prj\Login\Login\Db
	������:		������
	�ļ����ܣ�DB �����첽���ݵ���Ϣ����ϵͳ
	�޸ļ�¼��
*********************************************************************/


#ifndef _DBLOGIC_MANAGER_H_
#define _DBLOGIC_MANAGER_H_
#include "Type.h"
#include "Packet.h"
#include "Player.h"

#define	  DB_QUEUE_SIZE 100

//DB �߳��첽���ݵĹ�����
class DBLogicManager
{
public:
	
	DBLogicManager();
	~DBLogicManager();


	BOOL					HeartBeat(UINT uTime);

	//��������Ϣ
	BOOL					ProcessCacheCommands( ) ;

	//��ȡ������Ϣ
	BOOL					RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag ) ;

	//ɾ��ĳ��Player������Ϣ�����е���Ϣ
	BOOL					MovePacket( PlayerID_t PlayerID ) ;

	//*********
	//*********
	//�˽ӿ�֧������ͬ�����������ڲ�ͬ�߳��ڵ���
	//�˽ӿ����첽ͨѶ��Ψһ�ӿ�
	//ע�⣺pPacket��Ϣ��Ҫ��g_pPacketFactoryManager�����������������ɾ��
	BOOL					SendPacket( Packet* pPacket, 
							PlayerID_t PlayerID, 
							uint Flag=PF_NONE ) ;

	TID						m_ThreadID;
private:
	MyLock					m_Lock ;
private:
	//��ǰģ�����Ϣ����
	ASYNC_PACKET*			m_PacketQue ;
	uint					m_QueSize ;
	uint					m_Head ;
	uint					m_Tail ;
};

extern DBLogicManager	g_DBLogicManager;

#endif