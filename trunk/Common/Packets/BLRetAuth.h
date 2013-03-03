#ifndef _BLRET_AUTH_H_
#define _BLRET_AUTH_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class BLRetAuth : public Packet 
	{
	public:
		BLRetAuth( )
		{
			memset(mServerName,0,MAX_SERVER_NAME+1);
			memset(szAccount,0,MAX_ACCOUNT+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);

			mIsFatigue = 'N';
			mIsPhoneBind = 'N';
			mIsIPBind = 'N';
			mIsMiBaoBind = 'N';
			mIsMacBind = 'N';
			mIsRealNameBind = 'N';
			mIsInputNameBind = 'N';
		} ;
		virtual ~BLRetAuth( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_BL_RETAUTH ; }
		virtual UINT			GetPacketSize() const 
		{
			return	sizeof(CHAR)*MAX_ACCOUNT+sizeof(LOGIN_RESULT)+sizeof(MAX_SERVER_NAME)+
				sizeof(CHAR)*(MAX_BILLING_IP_SIZE)+sizeof(CHAR)+sizeof(UINT)+sizeof(CHAR)+
				sizeof(CHAR)+sizeof(CHAR)+sizeof(CHAR)+sizeof(CHAR)+sizeof(CHAR);
		}

	public:
		//使用数据接口
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);

		LOGIN_RESULT			GetResult() const;
		VOID					SetResult(LOGIN_RESULT result);

		const	PlayerID_t		GetPlayerID() const;
		VOID					SetPlayerID(PlayerID_t	pID);

		const	GUID_t			GetPlayerGUID() const;
		VOID					SetPlayerGUID(GUID_t guid);
		const	CHAR*			GetServerName() const
		{
			return mServerName;
		}
		VOID					SetServerName(const CHAR* pServerName)
		{
			strncpy(mServerName,pServerName,MAX_SERVER_NAME);
		}

		const CHAR*				GetServerIP() const
		{
			return mIP;
		}

		VOID					SetServerIP(const CHAR* pServerIP)
		{
			strncpy(mIP,pServerIP,MAX_BILLING_IP_SIZE);
		}

		CHAR					GetFatigueSign(){return mIsFatigue;}
		VOID					SetFatigueSign(CHAR cSign){mIsFatigue=cSign;}

		UINT					GetTotalOnlineTime(){return mAccTotalOnlineSecond;}
		VOID					SetTotalOnlineTime(UINT uTotalSecond)
		{
			mAccTotalOnlineSecond = uTotalSecond;
		}

		CHAR					GetPhoneBind(){return mIsPhoneBind;}
		VOID					SetPhoneBind(CHAR cSign){mIsPhoneBind=cSign;}

		CHAR					GetIPBind(){return mIsIPBind;}
		VOID					SetIPBind(CHAR cSign){mIsIPBind=cSign;}

		CHAR					GetMiBaoBind(){return mIsMiBaoBind;}
		VOID					SetMiBaoBind(CHAR cSign){mIsMiBaoBind=cSign;}

		CHAR					GetMacBind(){return mIsMacBind;}
		VOID					SetMacBind(CHAR cSign){mIsMacBind=cSign;}

		CHAR					GetRealNameBind(){return mIsRealNameBind;}
		VOID					SetRealNameBind(CHAR cSign){mIsRealNameBind=cSign;}

		CHAR					GetInputNameBind(){return mIsInputNameBind;}
		VOID					SetInputNameBind(CHAR cSign){mIsInputNameBind=cSign;}
	private:
		//数据
		CHAR					szAccount[MAX_ACCOUNT+1];	//用户名称
		LOGIN_RESULT			Result;
		PlayerID_t				PlayerID;
		GUID_t					PlayerGUID;
		CHAR					mServerName[MAX_SERVER_NAME+1];	//服务器名字
		CHAR					mIP[MAX_BILLING_IP_SIZE+1];	//服务器IP地址
		CHAR					mIsFatigue;					//是否加入防沉迷
		UINT					mAccTotalOnlineSecond;		//用户累计在线时间
		CHAR					mIsPhoneBind;				//是否手机绑定
		CHAR					mIsIPBind;					//是否IP绑定
		CHAR					mIsMiBaoBind;				//是否密保绑定
		CHAR					mIsMacBind;					//是否MAC绑定
		CHAR					mIsRealNameBind;			//是否实名绑定
		CHAR					mIsInputNameBind;			//是否输入实名
	};

	class BLRetAuthFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new BLRetAuth() ; }
		PacketID_t	GetPacketID()const { return PACKET_BL_RETAUTH; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT+sizeof(LOGIN_RESULT)+(MAX_SERVER_NAME+1)+
				sizeof(CHAR)*(MAX_BILLING_IP_SIZE+1)+sizeof(CHAR)+sizeof(UINT)+sizeof(CHAR)+
				sizeof(CHAR)+sizeof(CHAR)+sizeof(CHAR)+sizeof(CHAR)+sizeof(CHAR);
		}
	};


	class BLRetAuthHandler 
	{
	public:
		static UINT Execute( BLRetAuth* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif