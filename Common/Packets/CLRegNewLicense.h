#ifndef _CL_REGNEWLICENSE_H_
#define _CL_REGNEWLICENSE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{

	class CLRegNewLicense : public Packet 
	{
	public:
		CLRegNewLicense( ):m_UserAnswer(1){memset(szAccount,0,MAX_ACCOUNT+1);} ;
		virtual ~CLRegNewLicense( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_REGNEWLICENSE ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE) +sizeof(CHAR)*MAX_ACCOUNT;
		}

	public:
		//使用数据接口
		VOID					SetUserAnswer(BYTE nUse){m_UserAnswer = nUse;}
		BYTE					GetUserAnswer(){return m_UserAnswer;}
		const	CHAR*			GetAccount()	const{return	szAccount;}
		VOID					SetAccount(CHAR*	pAccount)
		{
			Assert(pAccount);
			strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
			szAccount[MAX_ACCOUNT] = 0;
		}
	private:
		//数据
		CHAR					szAccount[MAX_ACCOUNT+1] ;					//用户名称
		BYTE					m_UserAnswer;
	};

	class CLRegNewLicenseFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLRegNewLicense() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_REGNEWLICENSE ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_ACCOUNT;
		}
	};


	class CLRegNewLicenseHandler 
	{
	public:
		static UINT Execute( CLRegNewLicense* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif