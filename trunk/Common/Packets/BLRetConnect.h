#ifndef _BL_RETCONNECT_H_
#define _BL_RETCONNECT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class BLRetConnect : public BillPacket 
	{
	public:
		BLRetConnect( )
		{
			mReserve = 0;
		} ;
		virtual ~BLRetConnect( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_RETCONNECT ; }
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

	class BLRetConnectFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetConnect() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_RETCONNECT; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(USHORT);
		}
	};


	class BLRetConnectHandler 
	{
	public:
		static UINT Execute( BLRetConnect* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif