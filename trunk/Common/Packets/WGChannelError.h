

#ifndef __WGCHANNELERROR_H__
#define __WGCHANNELERROR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGChannelError : public Packet 
	{
	public:
		WGChannelError( ){} ;
		virtual ~WGChannelError( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_WG_CHANNELERROR ; }
			virtual UINT			GetPacketSize()const { return	sizeof(PlayerID_t)+
																	sizeof(BYTE) ; }

	public :
		VOID				SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
		PlayerID_t			GetPlayerID( ){ return m_PlayerID ; } ;

		VOID				SetErrorCode( BYTE Error ){ m_Error = Error ; }
		BYTE				GetErrorCode( ){ return m_Error ; }

	public :
		PlayerID_t			m_PlayerID ;	//������
		BYTE				m_Error ;

	};


	class WGChannelErrorFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGChannelError() ; }
		PacketID_t	GetPacketID()const { return PACKET_WG_CHANNELERROR ; }
			UINT		GetPacketMaxSize()const		{ return	sizeof(PlayerID_t)+
																sizeof(BYTE) ; }			
	};


	class WGChannelErrorHandler 
	{
	public:
		static UINT Execute( WGChannelError* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
