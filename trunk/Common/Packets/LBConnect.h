#ifndef _LB_CONNECT_H_
#define _LB_CONNECT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	


	class LBConnect : public BillPacket 
	{
	public:
		LBConnect( )
		{
			mIPSize = 0;
			memset(mIP,0,IP_SIZE);
		} ;
		virtual ~LBConnect( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LB_CONNECT ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(USHORT)+sizeof(USHORT)+sizeof(BYTE)+mIPSize;
		}

	public:
		//使用数据接口
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

		CHAR*					GetIPAddr()
		{
			return mIP;
		}

		VOID					SetIPAddr(const CHAR* IP)
		{
			strncpy(mIP,IP,MAX_BILLING_IP_SIZE);
			mIPSize = 0;

			for(INT i =0;i<MAX_BILLING_IP_SIZE;i++)
			{
				if(IP[i])	mIPSize++;
			}
		}


	private:
		//数据(服务器组ID) TLBB中为游戏世界
		USHORT					mZoneID;
		USHORT					mWorldID;
		BYTE					mIPSize;
		CHAR					mIP[IP_SIZE];
	};

	class LBConnectFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBConnect() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LB_CONNECT; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(USHORT)+sizeof(USHORT)+sizeof(BYTE)+MAX_BILLING_IP_SIZE;
		}
	};


	class LBConnectHandler 
	{
	public:
		static UINT Execute( LBConnect* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif