#ifndef _LW_RETBILLINGKEEP_H_
#define _LW_RETBILLINGKEEP_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	
	class LWRetBillingKeep : public Packet 
	{
	public:
		LWRetBillingKeep( )
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
		};

		virtual ~LWRetBillingKeep( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LW_RETBILLINGKEEP; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*(MAX_BILLACCOUNT+1)+sizeof(UINT)*3+sizeof(BYTE);
		}

	public:
		VOID					SetAccount(const CHAR* pAcc)
		{
			strncpy(mAccount,pAcc,MAX_BILLACCOUNT);
		}
		const CHAR*				GetAccount()	const
		{
			return mAccount;
		}
		BYTE					GetResult()
		{
			return mResult;
		}
		VOID					SetResult(BYTE bResult)
		{
			mResult = bResult;
		}
		
		VOID				SetLeftTime(UINT leftTime)
		{
			mLeftTime = leftTime;
		}
		UINT				GetLeftTime()
		{
			return mLeftTime;
		}
		VOID				SetStorePoint(UINT storePoint)
		{
			mStorePoint = storePoint;
		}
		UINT				GetStorePoint()
		{
			return mStorePoint;
		}
		VOID				SetUserPoint(UINT userPoint)
		{
			mUserPoint = userPoint;
		}
		UINT				GetUserPoint()
		{
			return mUserPoint;
		}

		//ʹ�����ݽӿ�
		
	private:
		CHAR					mAccount[MAX_BILLACCOUNT+1];//�û�����
		BYTE					mResult;					//��Ӧ���
		UINT					mLeftTime;					//ʣ��ʱ��
		UINT					mStorePoint;				//�̵����
		UINT					mUserPoint;					//�û�ʣ�����
	};

	class LWRetBillingKeepFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket()		{ return new LWRetBillingKeep() ; }
		PacketID_t		GetPacketID() const { return PACKET_LW_RETBILLINGKEEP; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(CHAR)*(MAX_BILLACCOUNT+1)+sizeof(BYTE)+sizeof(UINT)*3;
		}
	};


	class LWRetBillingKeepHandler 
	{
	public:
		static UINT Execute( LWRetBillingKeep* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif