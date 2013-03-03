#ifndef _WL_BILLINGKEEP_H_
#define _WL_BILLINGKEEP_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	
	class WLBillingKeep : public Packet 
	{
	public:
		WLBillingKeep( )
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
		};

		virtual ~WLBillingKeep( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WL_BILLINGKEEP; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*(MAX_BILLACCOUNT+1)+sizeof(SHORT)+sizeof(UINT)*2;
		}

	public:
		VOID					SetAccount(const CHAR* pAcc)
		{
			strncpy(mAccount,pAcc,MAX_BILLACCOUNT);
		}
		const CHAR*				GetAccount() const
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
		CHAR				mAccount[MAX_BILLACCOUNT+1]; //�û�����
		USHORT				mCharLevel;					 //�û���ɫ����
		UINT				mStartTime;					 //��ʼʱ��
		UINT				mEndTime;					 //����ʱ��
	};

	class WLBillingKeepFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket()		{ return new WLBillingKeep() ; }
		PacketID_t		GetPacketID() const { return PACKET_WL_BILLINGKEEP; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(CHAR)*(MAX_BILLACCOUNT+1)+sizeof(SHORT)+sizeof(UINT)*2;
		}
	};


	class WLBillingKeepHandler 
	{
	public:
		static UINT Execute( WLBillingKeep* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif