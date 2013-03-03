#ifndef __LBASKREGAUTH_H__
#define __LBASKREGAUTH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LBAskRegAuth : public Packet 
	{
	public:
		LBAskRegAuth( )
		{
			memset(szAccount,0,MAX_ACCOUNT+1);
			memset(szSuperPassWord,0,MAX_SUPER_CODE_SIZE+1);
			memset(szPassWord,0,MAX_MD5_SIZE+1);
			memset(szIP,0,MAX_BILLING_IP_SIZE+1);
			memset(szEmail,0,MAX_EMAIL_SIZE+1);
			PlayerID = INVALID_ID;
		} ;
		virtual ~LBAskRegAuth( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LB_ASKREGAUTH ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT+sizeof(CHAR)*MAX_SUPER_CODE_SIZE+
					sizeof(CHAR)*MAX_MD5_SIZE+sizeof(CHAR)*MAX_BILLING_IP_SIZE+sizeof(CHAR)*MAX_EMAIL_SIZE+sizeof(PlayerID_t);

		}

	public:
		//ʹ�����ݽӿ�
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);
		const	CHAR*			GetSuperPassWord()	const;
		VOID					SetSuperPassWord(const CHAR*	pSuperPassWord);
		const	CHAR*			GetPassWord()	const;
		VOID					SetPassWord(const CHAR*	pPassWord);
		const	CHAR*			GetIP()	const;
		VOID					SetIP(const CHAR*	pIP);
		const	CHAR*			GetEmail()	const;
		VOID					SetEmail(const CHAR*	pEmail);

		const	PlayerID_t		GetPlayerID() const;
		VOID					SetPlayerID(PlayerID_t	pID);
	private:
		//����
		CHAR					szAccount[MAX_ACCOUNT+1] ;	//�û�����
		CHAR					szSuperPassWord[MAX_SUPER_CODE_SIZE+1];	//��������
		CHAR					szPassWord[MAX_MD5_SIZE+1];	//�û�����
		CHAR					szIP[MAX_BILLING_IP_SIZE+1];	//IP��ַ
		CHAR					szEmail[MAX_EMAIL_SIZE+1]; //�ʼ���ַ
		PlayerID_t				PlayerID;
		
	};

	class LBAskRegAuthFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LBAskRegAuth() ; }
		PacketID_t	GetPacketID()const { return PACKET_LB_ASKREGAUTH ; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT+sizeof(CHAR)*MAX_SUPER_CODE_SIZE+
				sizeof(CHAR)*MAX_MD5_SIZE+sizeof(CHAR)*MAX_BILLING_IP_SIZE+sizeof(CHAR)*MAX_EMAIL_SIZE+sizeof(PlayerID_t);
		}
	};


	class LBAskRegAuthHandler 
	{
	public:
		static UINT Execute( LBAskRegAuth* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif