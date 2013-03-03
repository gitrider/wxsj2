#ifndef _LCRET_LOGIN_H_
#define _LCRET_LOGIN_H_
#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LCRetLogin : public Packet 
	{
	public:
		LCRetLogin( )
		{
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
		} ;
		virtual ~LCRetLogin( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LC_RETLOGIN ; }
		virtual UINT			GetPacketSize() const 
		{
			return				sizeof(CHAR)*MAX_ACCOUNT+
								sizeof(LOGIN_RESULT)+sizeof(CHAR)*MAX_BILLING_IP_SIZE+sizeof(AccountSafeSign);
		}

	public:
		//ʹ�����ݽӿ�
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);
	
		LOGIN_RESULT			GetResult() const;
		VOID					SetResult(LOGIN_RESULT result);

		const CHAR*				GetServerIP() const
		{
			return mIP;
		}

		VOID					SetServerIP(const CHAR* pServerIP)
		{
			strncpy(mIP,pServerIP,MAX_BILLING_IP_SIZE);
		}

		VOID					SetSafeSign(AccountSafeSign aSign){mSafeSign=aSign;}
		AccountSafeSign			GetSafeSign(){return mSafeSign;}
	private:
		//����
		//û�п�BillingSystem ǰʹ�ÿͻ����Լ���GUID
		CHAR					szAccount[MAX_ACCOUNT+1];	//�û�����
		LOGIN_RESULT			Result;
		CHAR					mIP[MAX_BILLING_IP_SIZE+1];	//������IP��ַ
		AccountSafeSign			mSafeSign;					//����ڷ������İ�ȫ��ʶ
	};

	class LCRetLoginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LCRetLogin() ; }
		PacketID_t	GetPacketID()const { return PACKET_LC_RETLOGIN ; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(LOGIN_RESULT)+sizeof(CHAR)*MAX_BILLING_IP_SIZE+sizeof(AccountSafeSign);
		}
	};


	class LCRetLoginHandler 
	{
	public:
		static UINT Execute( LCRetLogin* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif