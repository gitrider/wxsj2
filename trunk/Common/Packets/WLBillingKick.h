#ifndef _WLBILLING_KICK_H_
#define _WLBILLING_KICK_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{

	class WLBillingKick : public Packet 
	{
	public:
		WLBillingKick( )
		{
			
		};
		virtual ~WLBillingKick( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WL_BILLINGKICK ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(USHORT);
		}

	public:
		//ʹ�����ݽӿ�
		VOID				SetWorldID(USHORT worldID)
		{
			mWorldID = worldID;
		}

		USHORT				GetWorldID()
		{
			return mWorldID;
		}
	private:
		//����
		USHORT				mWorldID;	//����������
		
	};

	class WLBillingKickFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WLBillingKick() ; }
		PacketID_t	GetPacketID()const { return PACKET_WL_BILLINGKICK; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(USHORT);
		}
	};

	class WLBillingKickHandler 
	{
	public:
		static UINT Execute( WLBillingKick* pPacket, Player* pPlayer ) ;
	};
};
using namespace Packets;

#endif
