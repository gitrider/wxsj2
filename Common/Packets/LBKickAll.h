#ifndef _LB_KICKALL_H_
#define _LB_KICKALL_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	


	class LBKickAll : public BillPacket 
	{
	public:
		LBKickAll( )
		{
			mReasonLength	=	0;
			memset(mReason,0,MAX_REBOOT_INFO);
			mExtInfo1Length	=	0;
			memset(mExtInfo1,0,MAX_EXT_INFO);
			mExtInfo2Length	=	0;
			memset(mExtInfo2,0,MAX_EXT_INFO);
			mExtInfo3Length	=	0;
			memset(mExtInfo3,0,MAX_EXT_INFO);
		} ;
		virtual ~LBKickAll( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LB_KICKALL ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(USHORT)+sizeof(USHORT)+sizeof(BYTE)*4+
				sizeof(CHAR)*mReasonLength+sizeof(CHAR)*mExtInfo1Length+sizeof(CHAR)*mExtInfo2Length
				+sizeof(CHAR)*mExtInfo3Length;
		}

	public:
		//使用数据接口
		VOID					SetWorldID(USHORT WorldID)
		{
			mWorldID = WorldID;
		}
		USHORT					GetZoneID(){return mZoneID;}
		VOID					SetZoneID(USHORT ZoneID){mZoneID=ZoneID;}
		VOID					SetReason(const CHAR* pStr)
		{
			strncpy(mReason,pStr,MAX_REBOOT_INFO);
			mReasonLength = 0;
			for(INT i =0;i<MAX_REBOOT_INFO;i++)
			{
				if(mReason[i])	mReasonLength++;
			}
		}


		VOID					SetExtInfo1(const CHAR* pStr)
		{
			strncpy(mExtInfo1,pStr,MAX_EXT_INFO);
			mExtInfo1Length = 0;
			for(INT i =0;i<MAX_EXT_INFO;i++)
			{
				if(mExtInfo1[i])	mExtInfo1Length++;
			}
		}

		VOID					SetExtInfo2(const CHAR* pStr)
		{
			strncpy(mExtInfo2,pStr,MAX_EXT_INFO);
			mExtInfo2Length = 0;
			for(INT i =0;i<MAX_EXT_INFO;i++)
			{
				if(mExtInfo2[i])	mExtInfo2Length++;
			}
		}


		VOID					SetExtInfo3(const CHAR* pStr)
		{
			strncpy(mExtInfo3,pStr,MAX_EXT_INFO);
			mExtInfo3Length = 0;
			for(INT i =0;i<MAX_EXT_INFO;i++)
			{
				if(mExtInfo3[i])	mExtInfo3Length++;
			}
		}
	

	private:
		USHORT					mZoneID;
		USHORT					mWorldID;
		BYTE					mReasonLength;
		CHAR					mReason[MAX_REBOOT_INFO];
		BYTE					mExtInfo1Length;
		CHAR					mExtInfo1[MAX_EXT_INFO];
		BYTE					mExtInfo2Length;
		CHAR					mExtInfo2[MAX_EXT_INFO];
		BYTE					mExtInfo3Length;
		CHAR					mExtInfo3[MAX_EXT_INFO];


	};

	class LBKickAllFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBKickAll() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LB_KICKALL; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(USHORT)+sizeof(USHORT)+sizeof(BYTE)*4+
				sizeof(CHAR)*MAX_REBOOT_INFO+sizeof(CHAR)*MAX_EXT_INFO*3;
		}
	};


	class LBKickAllHandler 
	{
	public:
		static UINT Execute( LBKickAll* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif