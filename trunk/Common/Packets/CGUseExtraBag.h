#ifndef _CG_USE_EXTRABAG_H_
#define _CG_USE_EXTRABAG_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class CGUseExtraBag:public Packet
	{

	public:
		CGUseExtraBag(){};
		virtual				~CGUseExtraBag(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_USEEXTRABAG; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(INT);
		}

	public:
		VOID			setBagIndex(INT index){m_BagIndex = index;}
		INT				getBagIndex(){return m_BagIndex;}
	private:

		INT				m_BagIndex;		//使用Bag中的位置存放的位置
	};


	class CGUseExtraBagFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGUseExtraBag(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_USEEXTRABAG; }
		UINT		GetPacketMaxSize() const { return	sizeof(INT); }
	};


	class CGUseExtraBagHandler
	{
	public:
		static UINT	Execute( CGUseExtraBag* pPacket, Player* pPlayer );
	};

}


#endif