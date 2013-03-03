#ifndef _BL_RETBILLINGKEEP_H_
#define _BL_RETBILLINGKEEP_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	
	class BLRetBillingKeep : public BillPacket 
	{
	public:
		BLRetBillingKeep( )
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
		} ;

		virtual ~BLRetBillingKeep( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_RETBILLINGKEEP; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(CHAR)*mAccLength+sizeof(BYTE)+sizeof(UINT)*3;
		}

	public:

		BYTE				GetAccLength()
		{
			return mAccLength;
		}
		
		const CHAR*			GetAccount()
		{
			return mAccount;
		}

		BYTE				GetResult()
		{
			return mResult;
		}

		UINT				GetTimeLeft()
		{
			return mLeftTime;
		}
		
		UINT				GetStorePoint()
		{
			return mStorePoint;
		}
		
		UINT				GetUserPoint()
		{
			return mUserPoint;
		}
		//ʹ�����ݽӿ�
		
	private:
		BYTE					mAccLength;					//�û�������
		CHAR					mAccount[MAX_BILLACCOUNT+1];//�û�����
		BYTE					mResult;					//��Ӧ���
		UINT					mLeftTime;					//ʣ��ʱ��
		UINT					mStorePoint;				//�̵����
		UINT					mUserPoint;					//�û�ʣ�����
	};

	class BLRetBillingKeepFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetBillingKeep() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_RETBILLINGKEEP; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*(MAX_BILLACCOUNT)+sizeof(BYTE)+sizeof(UINT)*3;
		}
	};


	class BLRetBillingKeepHandler 
	{
	public:
		static UINT Execute( BLRetBillingKeep* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif