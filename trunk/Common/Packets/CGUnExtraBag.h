#ifndef	_CG_UNEXTRABAG_H_
#define _CG_UNEXTRABAG_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGUnExtraBag:public Packet
	{

	public:
		CGUnExtraBag(){};
		virtual				~CGUnExtraBag(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_UNEXTRABAG; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(INT);
		}

	public:
		VOID			setExtraContainerPoint(INT index){m_BagPoint	= index;}
		INT				getExtraContainerPoint(){return m_BagPoint;}

	private:
		INT				m_BagPoint;		//装配点信息
	};


	class CGUnExtraBagFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGUnExtraBag(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_UNEXTRABAG; }
		UINT		GetPacketMaxSize() const { return	sizeof(INT); }
	};


	class	CGUnExtraBagHandler
	{
	public:
		static UINT	Execute( CGUnExtraBag* pPacket, Player* pPlayer );
	};

}

#endif