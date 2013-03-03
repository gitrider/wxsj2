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

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LB_ASKREGLICENSE ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT+sizeof(BYTE)+sizeof(PlayerID_t)+sizeof(CHAR)*MAX_BILLING_IP_SIZE;
		}

	public:
		//使用数据接口
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);
		const	BYTE			GetUserAnswer()	const;
		VOID					SetUserAnswer(BYTE rt);
		const	PlayerID_t		GetPlayerID() const;
		VOID					SetPlayerID(PlayerID_t	pID);
		const	CHAR*			GetIPAddr()		const;
		VOID					SetIPAddr(const CHAR* pIP);
	private:
		//数据
		CHAR					szAccount[MAX_ACCOUNT+1] ;	//用户名称
		BYTE					UserAnswer;
		PlayerID_t				PlayerID;
		CHAR					szIP[MAX_BILLING_IP_SIZE+1];	//用户IP地址
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