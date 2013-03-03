#ifndef _BLRET_ACCCHECK_H_
#define _BLRET_ACCCHECK_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

enum	AccCheckResult
{
	ACR_SUCCESS				=	0x01,			//成功
	ACR_NO_ACCOUNT			=	0x02,			//账户不存在
	ACR_WRONG_PASSWORD		=	0x03,			//密码错误
	ACR_USER_ONLINE			=	0x04,			//角色在本组在线
	ACR_OTHER_ONLINE		=	0x05,			//角色在其他组在线
	ACR_OTHER_ERROR			=	0x06,			//其他错误
	ACR_FORBIT				=	0x07,			//禁止登录
	ACR_NO_POINT			=	0x08,			//没有足够点数
	ACR_REG_PASSPORT		=	0x09,			//需要注册SOHU的PassPort
	ACR_ACT_ACCOUNT			=	0xA0,			//帐号未激活		
	ACR_REG_WEB_PASSPORT	=	0xA1,			//必须WEB注册
	ACR_WRONG_IP_ADDR		=	0xB1,			//IP地址错误
	ACR_LOCK_BY_MOBILE		=	0xB2,			//帐户已经手机锁定
	ACR_NO_MIBAO			=	0xC1,			//用户未申请密宝卡功能
	ACR_MIBAO_NOT_USE		=	0xC2,			//用户申请了密宝卡功能但没有使用
	ACR_MIBAO_ERR			=	0xC3,			//密宝卡密码不对
	ACR_MAC_ERR				=	0xD1,			//MAC地址MD5值不对
	ACR_FATIGUE_ONLYONE		=	0xE1,			//防沉迷用户只许同时登录一个帐号(1身份证->n帐号)
	ACR_NO_NEW_LICENSE		=	0xF1,			//用户还没有接受新的许可协议
};

namespace Packets
{



	class BLRetAccCheck : public BillPacket
	{
	public:
		BLRetAccCheck()
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
			memset(mSName,0,MAX_SERVER_NAME+1);

			mIsFatigue = 'N';
			mIsPhoneBind = 'N';
			mIsIPBind = 'N';
			mIsMiBaoBind = 'N';
			mIsMacBind = 'N';
			mIsRealNameBind = 'N';
			mIsInputNameBind = 'N';
			mAccTotalOnlineSecond = 0;
		};
		virtual ~BLRetAccCheck(){};

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_RETACCCHECK;}
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+mAccLength+sizeof(BYTE);
		}

		public:
			
			const	CHAR*			GetAccount()	const;
			VOID					SetAccount(const CHAR*	pAccount);

			BYTE			GetResult() const;
			VOID					SetResult(BYTE result);

			const	PlayerID_t		GetPlayerID() const;
			VOID					SetPlayerID(PlayerID_t	pID);

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
			BYTE					mAccLength;					//用户名长度
			CHAR					mAccount[MAX_BILLACCOUNT+1];//用户名串
			BYTE					mAccResult;					//认证结果
		
			//可选结果mAccResult = 0x01或0xF1 时
			UINT					mCardPoint;					//点卡数
			USHORT					mCardDay;					//剩余天数
			UINT					mScore;						//积分
			CHAR					mIsFatigue;					//是否加入防沉迷
			UINT					mAccTotalOnlineSecond;		//用户累计在线时间
			CHAR					mIsPhoneBind;				//是否手机绑定
			CHAR					mIsIPBind;					//是否IP绑定
			CHAR					mIsMiBaoBind;				//是否密保绑定
			CHAR					mIsMacBind;					//是否MAC绑定
			CHAR					mIsRealNameBind;			//是否实名绑定
			CHAR					mIsInputNameBind;			//是否输入实名

			//mAccResult = 0x05 时
			USHORT					mWorldID;					//用户所在服务器组
			BYTE					mIPLength;					//服务器IP长度
			CHAR					mIP[MAX_BILLING_IP_SIZE+1];	//服务器IP地址
			BYTE					mServerNameLength;			//服务器名字长度
			CHAR					mSName[MAX_SERVER_NAME+1];	//服务器名字

	};


	class BLRetAccCheckFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetAccCheck() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_RETACCCHECK; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT+sizeof(BYTE)+
				sizeof(USHORT)+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLING_IP_SIZE+sizeof(BYTE)+
				sizeof(CHAR)*MAX_SERVER_NAME;
		}
	};


	class BLRetAccCheckHandler 
	{
	public:
		static UINT Execute( BLRetAccCheck* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif