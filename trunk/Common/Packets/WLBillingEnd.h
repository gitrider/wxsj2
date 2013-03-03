#ifndef _WLBILLING_END_H_
#define _WLBILLING_END_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	
	enum	BillingEndType
	{
		BET_NORMAL		= 0x01,			//��������
		BET_EXCEPTION	= 0x02,			//�쳣����
		BET_FORCE		= 0x03,			//ǿ�ƽ���
	};

	class WLBillingEnd : public Packet 
	{
	public:
		WLBillingEnd( )
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
		};
		virtual ~WLBillingEnd( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WL_BILLINGEND ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*(MAX_BILLACCOUNT+1)
				+sizeof(SHORT)+sizeof(UINT)*2+sizeof(BYTE);
		}

	public:
		//ʹ�����ݽӿ�
		VOID				SetAccount(const CHAR* pAccount)	
		{
			strncpy(mAccount,pAccount,MAX_BILLACCOUNT);
		}
		const CHAR*			GetAccount()
		{
			return mAccount;
		}
		VOID				SetCharLevel(USHORT lvl)
		{
			mCharLevel = lvl;
		}
		USHORT				GetCharLevel()
		{
			return mCharLevel;
		}
		VOID				SetFeeType(BYTE bType)
		{
			mEndType = bType;
		}
		USHORT				GetFeeType()
		{
			return mEndType;
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
	private:
		//����
		CHAR				mAccount[MAX_BILLACCOUNT+1];	//�û�����
		USHORT				mCharLevel;					//�û���ɫ����
		BYTE				mEndType;					//�Ʒѽ�������
		UINT				mStartTime;					//��ʼʱ��
		UINT				mEndTime;					//����ʱ��
	};

	class WLBillingEndFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WLBillingEnd() ; }
		PacketID_t	GetPacketID()const { return PACKET_WL_BILLINGEND; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(CHAR)*(MAX_BILLACCOUNT+1)
				+sizeof(SHORT)+sizeof(UINT)*2+sizeof(BYTE);
		}
	};

	class WLBillingEndHandler 
	{
	public:
		static UINT Execute( WLBillingEnd* pPacket, Player* pPlayer ) ;
	};
};
using namespace Packets;

#endif
