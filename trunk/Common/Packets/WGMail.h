

#ifndef __WGMAIL_H__
#define __WGMAIL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGMail : public Packet 
	{
	public:
		WGMail( ){} ;
		virtual ~WGMail( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_WG_MAIL ; }
		virtual UINT			GetPacketSize()const 
		{ 
			return	sizeof(PlayerID_t)+GetMailListSize(m_MailList) ;
		}

	public:
		//ʹ�����ݽӿ�
		VOID		SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
		PlayerID_t	GetPlayerID( ){ return m_PlayerID ; } ;

		VOID		SetMailList( MAIL_LIST* pMailList ){ m_MailList = *pMailList; }
		MAIL_LIST*	GetMailList( ){ return &m_MailList ; }

	private:
		//����
		PlayerID_t	m_PlayerID ;//�����˵�PlayerID
		MAIL_LIST	m_MailList ;

	};


	class WGMailFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGMail() ; }
		PacketID_t	GetPacketID()const { return PACKET_WG_MAIL ; }
		UINT		GetPacketMaxSize()const { return	sizeof(PlayerID_t)+
														sizeof(MAIL_LIST) ; }
	};


	class WGMailHandler 
	{
	public:
		static UINT Execute( WGMail* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
