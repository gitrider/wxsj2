#ifndef _LWRETBILLING_START_H_
#define _LWRETBILLING_START_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{


	class LWRetBillingStart : public Packet 
	{
	public:
		LWRetBillingStart( )
		{
			memset(mAccount,0,MAX_ACCOUNT+1);
		};
		virtual ~LWRetBillingStart( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LW_RETBILLINGSTART ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*(MAX_ACCOUNT+1)+sizeof(BYTE)
				+sizeof(BYTE)+sizeof(UINT)*3+sizeof(BYTE)+sizeof(PlayerID_t);
		}

	public:
		//使用数据接口
		VOID				SetAccount(const CHAR* pAccount)
		{
			strncpy(mAccount,pAccount,MAX_ACCOUNT);
		}
		const CHAR*			GetAccount()
		{
			return mAccount;
		}

		VOID				SetResult(BYTE bResult)
		{
			mResult = bResult;
		}
		BYTE				GetResult()
		{
			return mResult;
		}

		VOID				SetFeeType(BYTE	 bType)
		{
			mFeeType	= bType;
		}
		BYTE				GetFeeType()
		{
			return mFeeType;
		}
		VOID				SetLeftTime(UINT uLeftTime)
		{
			mLeftTime = uLeftTime;
		}
		UINT				GetLeftTime()
		{
			return mLeftTime;
		}
		VOID				SetStorePoint(UINT uStorePoint)
		{
			mStorePoint = uStorePoint;
		}
		UINT				GetStorePoint()
		{
			return mStorePoint;
		}
		VOID				SetUserPoint(UINT UserPoint)
		{
			mUserPoint	 = UserPoint;
		}
		UINT				GetUserPoint()
		{
			return mUserPoint;
		}
		VOID				SetCode(BYTE bCode)
		{
			mCode = bCode;
		}
		BYTE				GetCode()
		{
			return mCode;
		}

		PlayerID_t			GetPlayerID()
		{
			return mPlayerID;
		}

		VOID				SetPlayerID(PlayerID_t pID)
		{
			mPlayerID = pID;
		}

	private:
		//数据
		CHAR				mAccount[MAX_ACCOUNT+1];		//账号
		BYTE				mResult;//Billing Start 结果
		//mResult = 0x01
		BYTE				mFeeType;		//计费类型
		UINT				mLeftTime;		//剩余时间
		UINT				mStorePoint;	//商店点数
		UINT				mUserPoint;		//玩家剩余点数
		//mResult = 0x00
		BYTE				mCode;			//代码

		PlayerID_t			mPlayerID;		//玩家池ID
	};

	class LWRetBillingStartFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LWRetBillingStart() ; }
		PacketID_t	GetPacketID()const { return PACKET_LW_RETBILLINGSTART; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(CHAR)*(MAX_ACCOUNT+1)+sizeof(BYTE)
				   +sizeof(BYTE)+sizeof(UINT)*3+sizeof(BYTE)
				   +sizeof(PlayerID_t);
		}
	};

	class LWRetBillingStartHandler 
	{
	public:
		static UINT Execute( LWRetBillingStart* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets;
#endif
