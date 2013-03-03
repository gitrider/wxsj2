#ifndef __LBASKREGLICENSE_H__
#define __LBASKREGLICENSE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LBAskRegLicense : public Packet 
	{
	public:
		LBAskRegLicense( )
		{
			memset(szAccount,0,MAX_ACCOUNT+1);
			UserAnswer = 1;
			PlayerID = INVALID_ID;
		} ;
		virtual ~LBAskRegLicense( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LB_ASKREGLICENSE ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT+sizeof(BYTE)+sizeof(PlayerID_t)+sizeof(CHAR)*MAX_BILLING_IP_SIZE;
		}

	public:
		//ʹ�����ݽӿ�
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);
		const	BYTE			GetUserAnswer()	const;
		VOID					SetUserAnswer(BYTE rt);
		const	PlayerID_t		GetPlayerID() const;
		VOID					SetPlayerID(PlayerID_t	pID);
		const	CHAR*			GetIPAddr()		const;
		VOID					SetIPAddr(const CHAR* pIP);
	private:
		//����
		CHAR					szAccount[MAX_ACCOUNT+1] ;	//�û�����
		BYTE					UserAnswer;
		PlayerID_t				PlayerID;
		CHAR					szIP[MAX_BILLING_IP_SIZE+1];	//�û�IP��ַ
	};

	class LBAskRegLicenseFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LBAskRegLicense() ; }
		PacketID_t	GetPacketID()const { return PACKET_LB_ASKREGLICENSE ; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT+sizeof(BYTE)+sizeof(PlayerID_t)+sizeof(CHAR)*MAX_BILLING_IP_SIZE;
		}
	};


	class LBAskRegLicenseHandler 
	{
	public:
		static UINT Execute( LBAskRegLicense* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif