#ifndef _LB_BILLINGKEEP_H_
#define _LB_BILLINGKEEP_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	
	class LBBillingKeep : public BillPacket 
	{
	public:
		LBBillingKeep( ){} ;

		virtual ~LBBillingKeep( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LB_BILLINGKEEP; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(CHAR)*mAccLength+sizeof(SHORT)+sizeof(UINT)*2;
		}

	public:

		BYTE				GetAccLength()
		{
			return mAccLength;
		}
		
		VOID					SetAccount(const CHAR* pAcc)
		{
			strncpy(mAccount,pAcc,MAX_BILLACCOUNT);
			mAccLength = 0;
			for(INT i =0;i<MAX_BILLACCOUNT;i++)
			{
				if(mAccount[i]) mAccLength++;
			}
		}

		VOID				SetCharLevel(USHORT lvl)
		{
			mCharLevel = lvl;
		}
		USHORT				GetCharLevel()
		{
			return mCharLevel;
		}
		
		VOID				SetStartTime(UINT startTime)
		{
			mStartTime = startTime;
		}
		UINT				GetStartTime()
		{
			return mStartTime;
		}
		VOID				SetEndTime(UINT endTime)
		{
			mEndTime = endTime;
		}
		UINT				GetEndTime()
		{
			return mEndTime;
		}
		//ʹ�����ݽӿ�
		
	private:
		BYTE				mAccLength;//�û�������
		CHAR				mAccount[MAX_BILLACCOUNT];	//�û�����
		USHORT				mCharLevel;					//�û���ɫ����
		UINT				mStartTime;					//��ʼʱ��
		UINT				mEndTime;					//����ʱ��
	};

	class LBBillingKeepFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBBillingKeep() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LB_BILLINGKEEP; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(CHAR)*(MAX_BILLACCOUNT)+sizeof(SHORT)+sizeof(UINT)*2+sizeof(BYTE);
		}
	};


	class LBBillingKeepHandler 
	{
	public:
		static UINT Execute( LBBillingKeep* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif