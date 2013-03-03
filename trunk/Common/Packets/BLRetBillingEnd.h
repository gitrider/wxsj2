#ifndef _BLRET_BILLINGEND_H_
#define _BLRET_BILLINGEND_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	


	class BLRetBillingEnd : public BillPacket 
	{
	public:
		BLRetBillingEnd( )
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
		};
		virtual ~BLRetBillingEnd( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_RETBILLINGEND ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(CHAR)*mAccLength+sizeof(BYTE) ;
		}

	public:
		//ʹ�����ݽӿ�
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
	private:
		//����
		BYTE				mAccLength;//�û�������
		CHAR				mAccount[MAX_BILLACCOUNT+1];	//�û�����
		BYTE				mResult;//Billing End ���
	};

	class BLRetBillingEndFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetBillingEnd() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_RETBILLINGEND; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT+sizeof(BYTE) ;
		}
	};


	class BLRetBillingEndHandler 
	{
	public:
		static UINT Execute( BLRetBillingEnd* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif