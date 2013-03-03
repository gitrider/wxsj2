#ifndef _WLBILLING_START_H_
#define _WLBILLING_START_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{


	class WLBillingStart : public Packet 
	{
	public:
		WLBillingStart( )
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mCharName,0,MAX_BILLACCOUNT+1);
			mCharGUID = INVALID_GUID;
		};
		virtual ~WLBillingStart( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WL_BILLINGSTART ; }
		virtual UINT			GetPacketSize() const 
		{
			return 2*(sizeof(CHAR)*(MAX_BILLACCOUNT+1))+sizeof(GUID_t)
				   +sizeof(SHORT)+sizeof(UINT)
				   +sizeof(PlayerID_t);
		}

	public:
		//使用数据接口
		VOID		SetAccount(const CHAR* pAccount)			
		{
			strncpy(mAccount,pAccount,MAX_BILLACCOUNT);
		}
		const CHAR*	GetAccount()
		{
			return mAccount;
		}
		VOID		SetCharName(const CHAR* pCharName)			
		{
			strncpy(mCharName,pCharName,MAX_BILLACCOUNT);
		}
		const CHAR*	GetCharName()
		{
			return mCharName;
		}
		VOID		SetCharLevel(SHORT charLevel)	
		{
			mCharLevel  = charLevel;
		}
		SHORT		GetCharLevel()
		{
			return mCharLevel;
		}
		VOID		SetCharGUID(GUID_t guid)
		{
			mCharGUID	= guid;
		}
		GUID_t		GetCharGUID()
		{
			return mCharGUID;
		}
		VOID		SetStartTime(UINT StartTime)
		{
			mStartTime = StartTime;
		}

		UINT		GetStartTime()
		{
			return mStartTime;
		}

		PlayerID_t	GetPlayerID()
		{
			return mPlayerID;
		}

		VOID		SetPlayerID(PlayerID_t	 pID)
		{
			mPlayerID = pID;
		}

	private:
		//数据
		CHAR		mAccount[MAX_BILLACCOUNT+1];	//账号
		CHAR		mCharName[MAX_BILLACCOUNT+1];	//用户名称
		GUID_t		mCharGUID;					//用户GUID
		SHORT		mCharLevel;					//用户级别
		UINT		mStartTime;					//开始时间
		PlayerID_t	mPlayerID;					//玩家池ID;
	};

	class WLBillingStartFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WLBillingStart() ; }
		PacketID_t	GetPacketID()const { return PACKET_WL_BILLINGSTART; }
		UINT		GetPacketMaxSize() const
		{ 
			return 2*(sizeof(CHAR)*(MAX_BILLACCOUNT+1))+sizeof(GUID_t)
				   +sizeof(SHORT)+sizeof(UINT)
				   +sizeof(PlayerID_t);
		}
	};

	class WLBillingStartHandler 
	{
	public:
		static UINT Execute( WLBillingStart* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets;
#endif
