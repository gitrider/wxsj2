#ifndef _BL_RETKEEPLIVE_H_
#define _BL_RETKEEPLIVE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class BLRetKeepLive : public BillPacket 
	{
	public:
		BLRetKeepLive( )
		{
			mReserve = 0;
		} ;
		virtual ~BLRetKeepLive( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_RETKEEPLIVE; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(mReserve);
		}

	public:
		//ʹ�����ݽӿ�

	private:
		//����
		USHORT					mReserve;
		
	};

	class BLRetKeepLiveFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetKeepLive() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_RETKEEPLIVE; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(USHORT);
		}
	};


	class BLRetKeepLiveHandler 
	{
	public:
		static UINT Execute( BLRetKeepLive* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif