#ifndef _CL_REGSAFESIGN_H_
#define _CL_REGSAFESIGN_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{

	class CLRegSafeSign : public Packet 
	{
	public:
		enum
		{
			RSSR_SUCCESS = 0,
			RSSR_FAIL = 1,
		};

		CLRegSafeSign( ):m_UserAnswer(RSSR_SUCCESS){memset(szAccount,0,MAX_ACCOUNT+1);} ;
		virtual ~CLRegSafeSign( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_REGSAFESIGN ; }
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

	class CLRegSafeSignFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLRegSafeSign() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_REGSAFESIGN ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_ACCOUNT;
		}
	};


	class CLRegSafeSignHandler 
	{
	public:
		static UINT Execute( CLRegSafeSign* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;

#endif