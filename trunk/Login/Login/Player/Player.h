//
// Player ��ָ�Ѿ�ͨ���������ӽ���������˳���Ŀͻ�
//
//
//

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Type.h"
#include "Socket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "Packet.h"



#define MAX_CACHE_SIZE 1024

//�����һ��ʱ����û���յ��κ���Ϣ����Ͽ��˿ͻ��˵���������
#define MAX_KICK_TIME 300000
//�Ŷ���Ϣ����ʱ��
#define MAX_TRUN_MESSAGE_TIME	5000 


enum PACKET_FLAG
{
	PF_NONE=0 ,
	PF_REMOVE ,
};

struct ASYNC_PACKET
{
	Packet*		m_pPacket ;
	PlayerID_t	m_PlayerID ;
	uint		m_Flag ;

	ASYNC_PACKET( )
	{
		m_pPacket = NULL ;
		m_PlayerID = INVALID_ID ;
		m_Flag = PF_NONE ;
	};
	~ASYNC_PACKET( )
	{
		SAFE_DELETE( m_pPacket ) ;
		m_PlayerID = INVALID_ID ;
		m_Flag = PF_NONE ;
	};
};



class Player
{
public :
	Player( BOOL bIsServer=FALSE ) ;
	~Player( ) ;

	virtual BOOL	ProcessInput( ) ;
	virtual BOOL	ProcessOutput( ) ;
	virtual BOOL	ProcessCommand( BOOL Option = TRUE ) ;

	virtual BOOL	HeartBeat( UINT uTime=0 ) ;

	virtual BOOL	SendPacket( Packet* pPacket ) ;


public :
	//ͨ�ýӿ�

	virtual BOOL	IsLoginPlayer( )=0 ;
	virtual BOOL	IsServerPlayer( )=0 ;
	//������ͨ���Ż������ж��ǲ���ʹ�ô�����������û���ʹ���ĸ�������������û�
	INT				WhereThisPlayerFrom(VOID) const;
	virtual BOOL	IsBillingPlayer( ) = 0;

	//��ȡ�������PID�Ľӿڣ�PlayerID ����PlayerPool��������������ֵ
	//PlayerID���ڱ�ʶһ���ͻ�������������ϵͳ�е�����λ����Ϣ
	PlayerID_t	PlayerID(){ return m_PID ; } ;
	VOID		SetPlayerID( PlayerID_t id ){ m_PID = id ; } ;

	//��ȡ�������UID�Ľӿڣ�UserID ����UserPool��������������ֵ
	//UserID���ڱ�ʶ��ҵ���Ϸ�洢λ����Ϣ
	ID_t		UserID(){ return m_UID ; } ;
	VOID		SetUserID( ID_t id ){ m_UID = id ; } ;

	//��ȡ�������PMID�Ľӿڣ�PlayerManagerID ����PlayerManager��������������ֵ
	//PlayerManagerID���ڱ�ʶ��ǰ������ڵ�ĳ��PlayerManager�е�λ����Ϣ��ÿ��
	//PlayerManagerλ��һ���������߳��
	ID_t		PlayerManagerID(){ return m_PlayerManagerID ; } ;
	VOID		SetPlayerManagerID( ID_t id ){ m_PlayerManagerID = id ; } ;

	//��ȡ��ǰ��ҵ�Socket��
	//�������ӽӿ�
	Socket*		GetSocket(){ return m_pSocket ; } ;

	//�Ͽ��뵱ǰ��ҵ���������
	virtual VOID			Disconnect( ) ;

	//�жϵ�ǰ��ҵ����������Ƿ���Ч
	virtual	BOOL			IsValid( ) ; 

	//�����ǰ��������������ݺͻ�������
	virtual	VOID			CleanUp( ) ;

	//�жϵ�ǰ������ݿ��Ƿ��ǿտ飻
	//����ǿտ飬����Ա�PlayerPool�����ȥ���ڽ���������
	BOOL					IsEmpty( ) { return m_IsEmpty ; }
	VOID					SetEmpty( BOOL opt = TRUE ) { m_IsEmpty = opt ; }

	BOOL					IsDisconnect( ) { return m_IsDisconnect ; }
	VOID					SetDisconnect( BOOL opt = TRUE ) { m_IsDisconnect = opt ; }

	virtual VOID			ResetKick( ) ;

protected :
	BOOL					m_IsEmpty ;
	BOOL					m_IsDisconnect ;


protected :
	//Player ID
	//ͳһ��PlayerPool����
	PlayerID_t				m_PID ;	
	//User ID
	//ͳһ��UserPool����
	ID_t					m_UID ;	
	//��PlayerManager����player id������±�
	ID_t					m_PlayerManagerID ;

	//�������Ӿ��
	Socket*					m_pSocket ;

	//����������ݻ���
	SocketInputStream*		m_pSocketInputStream ;
	SocketOutputStream*		m_pSocketOutputStream ;



	BYTE					m_PacketIndex ;
};

CHAR* ProxyTool_GetCorrespondingIP(Player const* pPlayer, ID_t ServerID);
UINT  ProxyTool_GetCorrespondingPort(Player const* pPlayer, ID_t ServerID);

#endif
