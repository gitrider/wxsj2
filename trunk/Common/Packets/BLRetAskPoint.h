#ifndef _BLRET_ASKPOINT_H_
#define _BLRET_ASKPOINT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

	class BLRetAskPoint : public BillPacket
	{
	public:
		BLRetAskPoint()
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			mPoint = 0;
		};
		virtual ~BLRetAskPoint(){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_ASKPOINT;}
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+mAccLength+sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH+sizeof(UINT);
		}

		public:
			BYTE					GetAccLength()
			{
				return mAccLength;
			}
			const CHAR*				GetAccount()
			{
				return mAccount;
			}
			
			UINT					GetPoint(){return mPoint;}
		private:
			BYTE					mAccLength;						//�û�������
			CHAR					mAccount[MAX_BILLACCOUNT+1];	//�û�����

			UINT					mPoint;							//�û�����
	};


	class BLRetAskPointFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetAskPoint() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_ASKPOINT; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT+sizeof(UINT);
		}
	};


	class BLRetAskPointHandler 
	{
	public:
		static UINT Execute( BLRetAskPoint* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif