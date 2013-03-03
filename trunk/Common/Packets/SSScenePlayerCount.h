

#ifndef __SSSCENEPLAYERCOUNT_H__
#define __SSSCENEPLAYERCOUNT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class SSScenePlayerCount : public Packet 
	{
	public:
		SSScenePlayerCount( ){m_nChangeCount=0;m_ServerID=INVALID_ID;} ;
	virtual ~SSScenePlayerCount( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_SS_SCENEPLAYERCOUNT ; }
	virtual UINT			GetPacketSize() const 
	{ 
			if( m_nChangeCount>0 )
			{
				return	sizeof(SHORT)+	//m_nChangeCount
				(sizeof(SHORT)+sizeof(SceneID_t))*m_nChangeCount+
				sizeof(SHORT) ; 
	}
			else
			{
				return sizeof(SHORT)+	//m_nChangeCount
					sizeof(ID_t)+sizeof(INT) ;
			}
		}

	public:
	//ʹ�����ݽӿ�
	SHORT					GetChangeCount( ){ return m_nChangeCount ; }

	SceneID_t				GetChangeSceneID( INT nIndex ){
		if( nIndex<0 || nIndex>=MAX_SCENE ) 
		{
			Assert(FALSE) ;
			return INVALID_ID ;
		}
		return m_aChangeSceneID[nIndex] ;
	};
	SHORT					GetChangeScenePlayerCount( INT nIndex ){
		if( nIndex<0 || nIndex>=MAX_SCENE ) 
		{
			Assert(FALSE) ;
			return 0 ;
		}
		return m_aChangeScenePlayerCount[nIndex] ;
	};

	BOOL					AddChangeScene( SceneID_t sceneid, INT playercount ){
		if( m_nChangeCount>=MAX_SCENE )
		{
			Assert(FALSE) ;
			return FALSE ;
		}
		m_aChangeSceneID[m_nChangeCount] = sceneid ;
		m_aChangeScenePlayerCount[m_nChangeCount] = (SHORT)playercount ;
		m_nChangeCount ++ ;
		return TRUE ;
	};

	VOID					SetTotalPlayerCount( INT nCount ){ m_nTotalPlayerCount = (SHORT)nCount ; }
	SHORT					GetTotalPlayerCount(){ return m_nTotalPlayerCount ; }

		VOID					SetServerID( ID_t ServerID ){ m_ServerID = ServerID ; }
		ID_t					GetServerID(){ return m_ServerID ; }

		VOID					SetPlayerPoolSize( INT PlayerPoolSize ){ m_PlayerPoolSize = PlayerPoolSize ; }
		INT						GetPlayerPoolSize(){ return m_PlayerPoolSize ; }

	private:
	//����
	SHORT					m_nChangeCount ;//�仯�����ĳ�������
	SceneID_t				m_aChangeSceneID[MAX_SCENE] ;//�������仯�ĳ�����
	SHORT					m_aChangeScenePlayerCount[MAX_SCENE] ;//�������仯�ĳ�����ǰ����
	SHORT					m_nTotalPlayerCount ;
		ID_t					m_ServerID ;//ȱʡΪINVALID_ID
		INT						m_PlayerPoolSize ;//��m_ServerID�ڵ���ҳ�����
	};


	class SSScenePlayerCountFactory : public PacketFactory 
	{
	public:
	Packet*		CreatePacket() { return new SSScenePlayerCount() ; }
	PacketID_t	GetPacketID() const { return PACKET_SS_SCENEPLAYERCOUNT ; }
	UINT		GetPacketMaxSize() const { return	sizeof(SHORT)+
													(sizeof(SceneID_t)+sizeof(SHORT))*MAX_SCENE+
			sizeof(SHORT)+
			sizeof(ID_t)+
			sizeof(INT) ; }
	};


	class SSScenePlayerCountHandler 
	{
	public:
	static UINT Execute( SSScenePlayerCount* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
