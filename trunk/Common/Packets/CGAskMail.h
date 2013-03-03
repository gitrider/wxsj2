

#ifndef __CGASKMAIL_H__
#define __CGASKMAIL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskMail : public Packet 
	{
	public:
		CGAskMail( ){} ;
		virtual ~CGAskMail( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKMAIL ; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE) ; }

	public :

	public:
		//ʹ�����ݽӿ�
		VOID					SetAskType( BYTE type ){ m_AskType = type ; }
		BYTE					GetAskType( ){ return m_AskType ; }


	private:
		//����
		BYTE					m_AskType ;// enum ASK_TYPE
	};


	class CGAskMailFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskMail() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKMAIL ; }
		UINT		GetPacketMaxSize()const { return sizeof(BYTE) ; }
	};


	class CGAskMailHandler 
	{
	public:
		static UINT Execute( CGAskMail* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
