#ifndef _LWRETPRIZE_H_
#define _LWRETPRIZE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	enum BillingPrizeResult
	{
		BPR_SUCCESS			= 0x00,		//�ɹ�
		BPR_ALREADY_GET		= 0x02,		//�Ѿ�����Ľ�Ʒ
		BPR_OTHER_ERR		= 0x03,		//��������
		BPR_INVALID_ACC		= 0x04,		//�ʺŷǷ�
		BPR_NO_PRIZE		= 0x05,		//û���н�
		BPR_EXPIRE_PRIZE	= 0x06		//�齱����
	};

	enum BillingAskPointResult
	{
		BAPR_SUCCESS		= 0x00,		//��ѯ����ʣ�����
	};

	enum BillingAskBuyResult
	{
		BABR_SUCCESS			= 0x00,		//�ɹ�
		BABR_ALREADY_GET		= 0x01,		//�Ѿ�����Ľ�Ʒ
		BABR_NOENOUGH_POINT		= 0x02,		//����
		BABR_INVALID_GOODSCODE	= 0x03,		//��Ʒ����Ƿ�
		BABR_OTHER_ERR			= 0x09		//��������
	};

	enum BillingNewUserCardResult
	{
		BNCR_SUCCESS			= 0x00,		//�ɹ�
		BNCR_WRONG_CARDNUMBER	= 0x01,		//���Ŵ���
		BNCR_ALREADY_GET		= 0x02,		//�Ѿ�����Ľ�Ʒ
		BNCR_OTHER_ERR			= 0x03,		//��������
		BNCR_NO_NEWUSERCARD		= 0x05,		//û�����ֿ�
		BNCR_EXPIRE_CARD		= 0x06,		//�齱����
		BNCR_OTHER_USER_ALREADY	= 0x09,		//��ͬ����ȡ
		BNCR_OVER_LEVEL10		= 0x0A		//�û���10����������ʹ�����ֿ�
	};

	enum BillingNewPrizeResult
	{
		BNPR_SUCCESS			= 0x00,		//�ɹ�
		BNPR_ALREADY_GET		= 0x02,		//�Ѿ�����Ľ�Ʒ
		BNPR_OTHER_ERR			= 0x03,		//��������
		BNPR_INVALID_ACC		= 0x04,		//�ʺŷǷ�
		BNPR_NO_PRIZE			= 0x05,		//δ�н�
		BNPR_EXPIRE_PRIZE		= 0x06,		//�콱����
		BNPR_INVALID_SERVER		= 0x07		//���ǹ涨���콱������
	};

	enum BillingNewCheckPrizeResult
	{
		BNCPR_SUCCESS			= 0x00,		//�ɹ�
		BNCPR_OTHER_ERR			= 0x03,		//��������
		BNCPR_INVALID_ACC		= 0x04,		//�ʺŷǷ�
		BNCPR_NO_PRIZE			= 0x05,		//δ�н�
		BNCPR_EXPIRE_PRIZE		= 0x06,		//�콱����
		BNCPR_INVALID_SERVER	= 0x07		//���ǹ涨���콱������
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

		//���ü̳нӿ�
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
		//ʹ�����ݽӿ�
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
			case ASK_PRIZE:			//CD-KEY��ȡ
				mPrizeResult = BPR_OTHER_ERR;break;
			case ASK_BUY:			//��������
				mPrizeResult = BABR_OTHER_ERR;break;
			case ASK_NEWPRIZE:		//�³齱����	
				mPrizeResult = BNPR_OTHER_ERR;break;
			case ASK_NEWUSER_CARD:	//���ֿ�		
				mPrizeResult = BNCR_OTHER_ERR;break;
			case ASK_NEWCHECKPRIZE:	//�½�Ʒ��ѯ	
				mPrizeResult = BNCPR_OTHER_ERR;break;
			case ASK_POINT:			//���������ѯ
			case ASK_INVALID:		//��
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
		//����
		ID_t					mPrizeUserID;
		CHAR					mAccName[MAX_ACCOUNT+1];		//�˺�
		BYTE					mPrizeResult;					//�齱���

		BYTE					mRetType;						//���ص���������
		UINT					mLeftPoint;						//ʣ�����
		USHORT					mPrizeNum;						//������������
		_BUY_DATA				mPrize[MAX_PRIZE_NUMBER];		//���ؽ��
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
