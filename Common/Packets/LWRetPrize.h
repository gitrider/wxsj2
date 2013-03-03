#ifndef _LWRETPRIZE_H_
#define _LWRETPRIZE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	enum BillingPrizeResult
	{
		BPR_SUCCESS			= 0x00,		//成功
		BPR_ALREADY_GET		= 0x02,		//已经领过的奖品
		BPR_OTHER_ERR		= 0x03,		//其他错误
		BPR_INVALID_ACC		= 0x04,		//帐号非法
		BPR_NO_PRIZE		= 0x05,		//没有中奖
		BPR_EXPIRE_PRIZE	= 0x06		//抽奖过期
	};

	enum BillingAskPointResult
	{
		BAPR_SUCCESS		= 0x00,		//查询人物剩余点数
	};

	enum BillingAskBuyResult
	{
		BABR_SUCCESS			= 0x00,		//成功
		BABR_ALREADY_GET		= 0x01,		//已经购买的奖品
		BABR_NOENOUGH_POINT		= 0x02,		//金额不足
		BABR_INVALID_GOODSCODE	= 0x03,		//物品编码非法
		BABR_OTHER_ERR			= 0x09		//其他错误
	};

	enum BillingNewUserCardResult
	{
		BNCR_SUCCESS			= 0x00,		//成功
		BNCR_WRONG_CARDNUMBER	= 0x01,		//卡号错误
		BNCR_ALREADY_GET		= 0x02,		//已经领过的奖品
		BNCR_OTHER_ERR			= 0x03,		//其他错误
		BNCR_NO_NEWUSERCARD		= 0x05,		//没有新手卡
		BNCR_EXPIRE_CARD		= 0x06,		//抽奖过期
		BNCR_OTHER_USER_ALREADY	= 0x09,		//不同人领取
		BNCR_OVER_LEVEL10		= 0x0A		//用户已10级，不能再使用新手卡
	};

	enum BillingNewPrizeResult
	{
		BNPR_SUCCESS			= 0x00,		//成功
		BNPR_ALREADY_GET		= 0x02,		//已经领过的奖品
		BNPR_OTHER_ERR			= 0x03,		//其他错误
		BNPR_INVALID_ACC		= 0x04,		//帐号非法
		BNPR_NO_PRIZE			= 0x05,		//未中奖
		BNPR_EXPIRE_PRIZE		= 0x06,		//领奖过期
		BNPR_INVALID_SERVER		= 0x07		//不是规定的领奖服务器
	};

	enum BillingNewCheckPrizeResult
	{
		BNCPR_SUCCESS			= 0x00,		//成功
		BNCPR_OTHER_ERR			= 0x03,		//其他错误
		BNCPR_INVALID_ACC		= 0x04,		//帐号非法
		BNCPR_NO_PRIZE			= 0x05,		//未中奖
		BNCPR_EXPIRE_PRIZE		= 0x06,		//领奖过期
		BNCPR_INVALID_SERVER	= 0x07		//不是规定的领奖服务器
	};

	class LWRetPrize : public Packet 
	{
	public:
		LWRetPrize( )
		{
			mPrizeUserID = INVALID_ID;
			memset(mAccName,0,MAX_ACCOUNT+1);
			mPrizeResult = BPR_OTHER_ERR;

			mRetType = ASK_INVALID;
			mPrizeNum = 0;
			mLeftPoint = 0;
			memset(mPrize,0,sizeof(_BUY_DATA)*MAX_PRIZE_NUMBER);
		};
		virtual ~LWRetPrize( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LW_RETPRIZE ; }
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = sizeof(ID_t)+sizeof(CHAR)*(MAX_ACCOUNT)+sizeof(BYTE)+sizeof(BYTE);
			if( (ASK_PRIZE == mRetType && BPR_SUCCESS == mPrizeResult) ||
				(ASK_NEWUSER_CARD == mRetType && BNCR_SUCCESS == mPrizeResult) ||
				(ASK_NEWPRIZE == mRetType && BNPR_SUCCESS == mPrizeResult) ||
				(ASK_NEWCHECKPRIZE == mRetType && BNCPR_SUCCESS == mPrizeResult))
			{
				uSize += sizeof(USHORT);
				uSize += sizeof(_BUY_DATA)*mPrizeNum;
			}
			else if(ASK_BUY == mRetType && BABR_SUCCESS == mPrizeResult)
			{
				uSize += sizeof(UINT);
				uSize += sizeof(USHORT);
				uSize += sizeof(_BUY_DATA)*mPrizeNum;
			}
			else if(ASK_POINT == mRetType && BAPR_SUCCESS == mPrizeResult)
			{
				uSize += sizeof(UINT);
			}
			return uSize;
		}

	public:
		//使用数据接口
		ID_t					GetPrizeUserID(){return mPrizeUserID;}
		VOID					SetPrizeUserID(ID_t id)
		{
			mPrizeUserID = id;
		}

		CHAR*					GetAccName(){return mAccName;}
		VOID					SetAccName(const CHAR* pAccName)
		{
			Assert(pAccName);
			strncpy(mAccName,pAccName,MAX_ACCOUNT);
			mAccName[MAX_ACCOUNT] = 0;
		}

		BYTE					GetPrizeResult(){return mPrizeResult;}
		VOID					SetPrizeResult(BYTE rs)
		{
			mPrizeResult = rs;
		}
		VOID					SetPrizeResultOtherError()
		{
			switch(mRetType) 
			{
			case ASK_PRIZE:			//CD-KEY领取
				mPrizeResult = BPR_OTHER_ERR;break;
			case ASK_BUY:			//购买请求
				mPrizeResult = BABR_OTHER_ERR;break;
			case ASK_NEWPRIZE:		//新抽奖请求	
				mPrizeResult = BNPR_OTHER_ERR;break;
			case ASK_NEWUSER_CARD:	//新手卡		
				mPrizeResult = BNCR_OTHER_ERR;break;
			case ASK_NEWCHECKPRIZE:	//新奖品查询	
				mPrizeResult = BNCPR_OTHER_ERR;break;
			case ASK_POINT:			//人物点数查询
			case ASK_INVALID:		//空
			default:
				mPrizeResult = BPR_OTHER_ERR;break;
			}
		}

		BYTE					GetRetType(){return mRetType;}
		VOID					SetRetType(BYTE ty)
		{
			Assert(ty > ASK_INVALID && ty < ASK_NUMBER);
			mRetType = ty;
		}

		USHORT					GetPrizeNum(){return mPrizeNum;}
		
		_BUY_DATA*				GetPrize(INT idx)
		{
			Assert(idx >= 0 && idx < MAX_PRIZE_NUMBER);
			return &(mPrize[idx]);
		}

		VOID					AddPrize(_RETBUY_DATA* pData)
		{
			Assert(mRetType == ASK_BUY);
			Assert(mPrizeNum<MAX_PRIZE_NUMBER);
			Assert(pData);
			mPrize[mPrizeNum].m_BuyInt = pData->m_BuyInt;
			mPrize[mPrizeNum].m_BuyNumber = pData->m_BuyNumber;
			mPrizeNum++;
		}

		VOID					AddPrize(_PRIZE_DATA* pData)
		{
			Assert(mRetType == ASK_PRIZE || mRetType == ASK_NEWUSER_CARD ||
				   mRetType == ASK_NEWPRIZE || mRetType == ASK_NEWCHECKPRIZE);
			Assert(mPrizeNum<MAX_PRIZE_NUMBER);
			Assert(pData);
			
			memcpy(mPrize[mPrizeNum].m_BuyString,pData->m_PrizeString,MAX_PRIZE_STRING);
			mPrize[mPrizeNum].m_BuyString[MAX_PRIZE_STRING] = 0;
			mPrize[mPrizeNum].m_BuyNumber = pData->m_PrizeNum;
			mPrizeNum++;
		}

		UINT					GetLeftPoint(){return mLeftPoint;}
		VOID					SetLeftPoint(UINT pt)
		{
			mLeftPoint = pt;
		}
	private:
		//数据
		ID_t					mPrizeUserID;
		CHAR					mAccName[MAX_ACCOUNT+1];		//账号
		BYTE					mPrizeResult;					//抽奖结果

		BYTE					mRetType;						//返回的请求类型
		UINT					mLeftPoint;						//剩余点数
		USHORT					mPrizeNum;						//返回类型数量
		_BUY_DATA				mPrize[MAX_PRIZE_NUMBER];		//返回结果
	};

	class LWRetPrizeFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LWRetPrize() ; }
		PacketID_t	GetPacketID()const { return PACKET_LW_RETPRIZE; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(ID_t)+sizeof(CHAR)*MAX_ACCOUNT+sizeof(BYTE)+sizeof(BYTE)+sizeof(UINT)+
				   sizeof(USHORT)+sizeof(_BUY_DATA)*MAX_PRIZE_NUMBER;
		}
	};

	class LWRetPrizeHandler 
	{
	public:
		static UINT Execute( LWRetPrize* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets;
#endif
