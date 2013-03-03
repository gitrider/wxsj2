#ifndef _BLRET_BILLINGSTART_H_
#define _BLRET_BILLINGSTART_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "FoxWin32.h"


namespace Packets
{
	


	class BLRetBillingStart : public BillPacket 
	{
	public:
		BLRetBillingStart( )
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
		};
		virtual ~BLRetBillingStart( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_RETBILLINGSTART ; }
		virtual UINT			GetPacketSize() const 
		{
			INT	VarLength = 0;
			if(mResult==0x01) VarLength = sizeof(UINT)*3;

			return sizeof(BYTE)+sizeof(CHAR)*mAccLength+sizeof(BYTE)
				+sizeof(BYTE)+VarLength;
		}

	public:
		//ʹ�����ݽӿ�
		BYTE				GetAccLength()
		{
			return mAccLength;
		}
		VOID		SetAccount(const CHAR* pSrc	)
		{
			Assert(pSrc);
			strncpy(mAccount, pSrc, sizeof(mAccount));
			mAccLength = min(strlen(mAccount), MAX_BILLACCOUNT);
			mAccount[mAccLength] = 0;
			
		}
		const CHAR*			GetAccount()
		{
			return mAccount;
		}
		VOID				SetResult(BYTE res) { mResult = res; }
		BYTE				GetResult()
		{
			return mResult;
		}
		VOID				SetCode(BYTE code) { mCode = code; }
		BYTE				GetCode()
		{
			return mCode;
		}
		VOID				SetFeeType(BYTE val) { mFeeType = val; }
		BYTE				GetFeeType()
		{
			return mFeeType;
		}
		VOID				SetLeftTime(UINT val) { mLeftTime = val; }
		UINT				GetLeftTime()
		{
			return mLeftTime;
		}
		VOID				SetStorePoint(UINT val) { mStorePoint = val; }
		UINT				GetStorePoint()
		{
			return mStorePoint;
		}
		VOID				SetUserPoint(UINT val) { mUserPoint = val; }
		UINT				GetUserPoint()
		{
			return mUserPoint;
		}
	private:
		//����
		BYTE				mAccLength;//�û�������
		CHAR				mAccount[MAX_BILLACCOUNT+1];	//�û�����
		BYTE				mResult;//Billing Start ���
		
		//mResult = 0x01
		BYTE				mFeeType;		//�Ʒ�����
		UINT				mLeftTime;		//ʣ��ʱ��
		UINT				mStorePoint;	//�̵����
		UINT				mUserPoint;		//���ʣ�����
		//mResult = 0x00
		BYTE				mCode;			//����
	};

	class BLRetBillingStartFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetBillingStart() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_RETBILLINGSTART; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT+sizeof(BYTE)
				   +sizeof(BYTE)+sizeof(UINT)*3;
		}
	};


	class BLRetBillingStartHandler 
	{
	public:
		static UINT Execute( BLRetBillingStart* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif