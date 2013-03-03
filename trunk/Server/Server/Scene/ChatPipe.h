
#ifndef __CHATPIPE_H__
#define __CHATPIPE_H__

#include "Type.h"

class Scene ;
class Packet ;
class Obj ;


#define CHAT_ITEM_SIZE 1024
#define COUNTS_PER_TICK 128

struct CHAT_ITEM
{
	Packet*				m_pPacket ;
	ObjID_t				m_SourObjID ;//��ͨ����ʱ˵����
	ObjID_t				m_DestObjID ;//���ġ������ġ�˽�ġ�ϵͳ���Խ�����Ƶ��ʱĿ��OBJ

	CHAT_ITEM( )
	{ 
		CleanUp( ) ; 
	} ;

	VOID CleanUp( )
	{ 
		m_pPacket = NULL ; 
		m_SourObjID = INVALID_ID ;
		m_DestObjID = INVALID_ID ;
	} ;
};


class ChatPipe
{
public :
	ChatPipe( ) ;
	~ChatPipe( ) ;


	VOID				CleanUp( ) ;
	BOOL				Init( Scene* pScene ) ;

	BOOL				HeartBeat( UINT uTime ) ;



	Scene*				GetScene( ){ return m_pScene ; } ;

	//������ܵ��ڷ�����Ϣ,֧���첽����
	BOOL				SendPacket( Packet* pPacket, ObjID_t SourID, ObjID_t DestID ) ;
	//������ܵ���ȡ����Ϣ,֧���첽����
	BOOL				RecvPacket( Packet*& pPacket, ObjID_t* pSourID, ObjID_t* pDestID ) ;


public :
	INT					m_nValidCount ;//�ɷ��͵���Ϣ����
	Scene*				m_pScene ;
	CHAT_ITEM*			m_pChatItems ;
	INT					m_nHead ;
	INT					m_nTail ;

	MyLock				m_Lock ;


};


#endif
