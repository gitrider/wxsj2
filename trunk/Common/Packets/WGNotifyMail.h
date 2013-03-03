

#ifndef __WGNOTIFYMAIL_H__
#define __WGNOTIFYMAIL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGNotifyMail : public Packet 
	{
	public:
		WGNotifyMail( ){} ;
		virtual ~WGNotifyMail( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_WG_NOTIFYMAIL ; }
		virtual UINT			GetPacketSize()const { return	sizeof(PlayerID_t)+
																sizeof(BYTE) ; }

	public :

	public:
		//ʹ�����ݽӿ�
		VOID		SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
		PlayerID_t	GetPlayerID( ){ return m_PlayerID ; } ;

		VOID		SetMailCount( BYTE bCount ){ m_MailCount = bCount ; } ;
		BYTE		GetMailCount( ){ return m_MailCount ; } ;

	private:
		//����
		PlayerID_t	m_PlayerID ;
		BYTE		m_MailCount ;

	};


	class WGNotifyMailFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGNotifyMail() ; }
		PacketID_t	GetPacketID()const { return PACKET_WG_NOTIFYMAIL ; }
		UINT		GetPacketMaxSize()const { return	sizeof(PlayerID_t)+
														sizeof(BYTE) ; }
	};


	class WGNotifyMailHandler 
	{
	public:
		static UINT Execute( WGNotifyMail* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
