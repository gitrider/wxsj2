#ifndef _LB_KEEPLIVE_H_
#define _LB_KEEPLIVE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	
#define MAX_BILLING_IP_SIZE  15

	class LBKeepLive : public BillPacket 
	{
	public:
		LBKeepLive( ){} ;

		virtual ~LBKeepLive( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LB_KEEPLIVE ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(USHORT)+sizeof(USHORT)+sizeof(USHORT);
		}

	public:
		//ʹ�����ݽӿ�
		USHORT					GetZoneID(){return mZoneID;}
		VOID					SetZoneID(USHORT ZoneID){mZoneID=ZoneID;}

		USHORT					GetWorldID() 
		{
			return mWorldID;
		}

		VOID					SetWorldID(USHORT WorldID)
		{
			mWorldID = WorldID;
		}
		USHORT					GetPlayerCount()
		{
			return mPlayerCount;
		}
		VOID					SetPlayerCount(USHORT PlayerCount)
		{
			mPlayerCount = PlayerCount;
		}


	private:
		USHORT					mZoneID;
		//����(��������ID) TLBB��Ϊ��Ϸ����
		USHORT					mWorldID;
		//��Ϸ�������������
		USHORT					mPlayerCount;
	};

	class LBKeepLiveFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBKeepLive() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LB_KEEPLIVE; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(USHORT)+sizeof(USHORT)+sizeof(USHORT);
		}
	};


	class LBKeepLiveHandler 
	{
	public:
		static UINT Execute( LBKeepLive* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif