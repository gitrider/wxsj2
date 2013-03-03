#ifndef _LWRETBILLING_END_H_
#define _LWRETBILLING_END_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{


	class LWRetBillingEnd : public Packet 
	{
	public:
		LWRetBillingEnd( )
		{
			memset(mAccount,0,MAX_ACCOUNT+1);
		};
		virtual ~LWRetBillingEnd( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LW_RETBILLINGEND ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*(MAX_ACCOUNT+1)+sizeof(BYTE);
		}

	public:
		//ʹ�����ݽӿ�
		VOID				SetAccount(const CHAR* pAccount)
		{
			strncpy(mAccount,pAccount,MAX_ACCOUNT);
		}
		const CHAR*			GetAccount()
		{
			return mAccount;
		}

		VOID				SetResult(BYTE bResult)
		{
			mResult = bResult;
		}
		BYTE				GetResult()
		{
			return mResult;
		}
	private:
		//����
		CHAR				mAccount[MAX_ACCOUNT+1];		//�˺�
		BYTE				mResult;						//Billing End���
	};

	class LWRetBillingEndFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LWRetBillingEnd() ; }
		PacketID_t	GetPacketID()const { return PACKET_LW_RETBILLINGEND; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(CHAR)*(MAX_ACCOUNT+1)+sizeof(BYTE);
		}
	};

	class LWRetBillingEndHandler 
	{
	public:
		static UINT Execute( LWRetBillingEnd* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets;
#endif
