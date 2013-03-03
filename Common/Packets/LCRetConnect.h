#ifndef _LC_RETCONNECT_H_
#define _LC_RETCONNECT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LCRetConnect : public Packet 
	{
	public:
		LCRetConnect( ){for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)	memset(mAllMiBaoKey[i],0,MIBAOUNIT_NAME_LENGTH+1);} ;
		virtual ~LCRetConnect( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LC_RETCONNECT ; }
		virtual UINT			GetPacketSize() const 
		{
			return	sizeof(LOGIN_CONNECT_RESULT)
				+ sizeof(CHAR)*IP_SIZE
				+ sizeof(UINT)+
				sizeof(CHAR)*MIBAOUNIT_NUMBER*(MIBAOUNIT_NAME_LENGTH+1);
		}

	public:
		//使用数据接口
		LOGIN_CONNECT_RESULT	GetResult()
		{
			return Result;
		}
		VOID					SetResult(LOGIN_CONNECT_RESULT LResult)
		{
			Result = LResult;
		}

		UINT					GetLoginPort()
		{
			return LoginPort;
		}
		VOID					SetLoginPort(UINT LPort)
		{
			LoginPort = LPort;
		}
		CHAR*					GetLoginIP()
		{
			return LoginIP;
		}
		VOID					SetLoginIP(const CHAR* pLoginIP)
		{
			strncpy(LoginIP,pLoginIP,IP_SIZE);
			LoginIP[IP_SIZE-1] = '\0';
		}
		VOID					SetMiBaoKey(INT idx, const CHAR* pKey)
		{
			if(idx < 0 || idx >= MIBAOUNIT_NUMBER)	return;
			strncpy(mAllMiBaoKey[idx],pKey,MIBAOUNIT_NAME_LENGTH);
		}
		
		const	CHAR*			GetMiBaoKey(INT idx)	const
		{
			if(idx < 0 || idx >= MIBAOUNIT_NUMBER)	return	NULL;
			return mAllMiBaoKey[idx];
		}
	private:
		//数据
		CHAR					LoginIP[IP_SIZE];
		UINT					LoginPort;
		LOGIN_CONNECT_RESULT	Result;
		CHAR					mAllMiBaoKey[MIBAOUNIT_NUMBER][MIBAOUNIT_NAME_LENGTH+1];	//密保名
	};

	class LCRetConnectFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LCRetConnect() ; }
		PacketID_t	GetPacketID()const { return PACKET_LC_RETCONNECT; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(LOGIN_CONNECT_RESULT)
					+ sizeof(CHAR)*IP_SIZE
					+ sizeof(UINT)+
					sizeof(CHAR)*MIBAOUNIT_NUMBER*(MIBAOUNIT_NAME_LENGTH+1);
		}
	};


	class LCRetConnectHandler 
	{
	public:
		static UINT Execute( LCRetConnect* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets;

#endif