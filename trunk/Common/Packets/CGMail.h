

#ifndef __CGMAIL_H__
#define __CGMAIL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGMail : public Packet 
	{
	public:
		CGMail( ){} ;
		virtual ~CGMail( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MAIL ; }
		virtual UINT			GetPacketSize()const 
		{ 
			return	GetMailSize(m_Mail) ;
		}

	public:
		//ʹ�����ݽӿ�
		VOID		SetMail( MAIL* pMail ){ m_Mail = *pMail ; }
		MAIL*		GetMail( ){ return &m_Mail ; }

	private:
		//����
		MAIL		m_Mail ;

	};


	class CGMailFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGMail() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MAIL ; }
		UINT		GetPacketMaxSize()const { return	sizeof(MAIL) ; }
	};


	class CGMailHandler 
	{
	public:
		static UINT Execute( CGMail* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
