#ifndef _LCRET_CHARLOGIN_H_
#define _LCRET_CHARLOGIN_H_



#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LCRetCharLogin : public Packet 
	{
	public:
		LCRetCharLogin( )
		{
			memset(ServerIP,0,IP_SIZE);
			ServerPort = 0;
			ZoneID = 0;
		} ;
		virtual ~LCRetCharLogin( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LC_RETCHARLOGIN ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(ASKCHARLOGIN_RESULT)
				   + sizeof(CHAR)*IP_SIZE
				   + sizeof(UINT)
				   + sizeof(UINT)
				   + sizeof(BYTE)
				   + sizeof(UINT);
		}

	public:
		//ʹ�����ݽӿ�
		VOID					SetResult(ASKCHARLOGIN_RESULT Res)
		{
			Result = Res;
		}

		ASKCHARLOGIN_RESULT		GetResult()
		{
			return Result;
		}
		
		UINT					GetServerPort()
		{
			return ServerPort;
		}
		VOID					SetServerPort(UINT Port)
		{
			ServerPort = Port;
		}
		CHAR*					GetServerIP()
		{
			return ServerIP;
		}
		VOID					SetServerIP(const CHAR* pServerIP)
		{
			strncpy(ServerIP,pServerIP,IP_SIZE);
			ServerIP[IP_SIZE-1] = '\0';
		}

		UINT					GetUserKey()
		{
			return Key;
		}

		VOID					SetUserKey(UINT key)
		{
			Key	= key;
		}

		BYTE					GetWorldId()
		{
			return WorldID;
		}

		VOID					SetWorldId(BYTE wid)
		{
			WorldID = wid;
		}

		UINT					GetZoneID()
		{
			return ZoneID;
		}

		VOID					SetZoneID(UINT zid)
		{
			ZoneID = zid;
		}

	private:
		//����
		ASKCHARLOGIN_RESULT		Result;	
		CHAR					ServerIP[IP_SIZE];
		UINT					ServerPort;
		UINT					Key;
		BYTE					WorldID;
		UINT					ZoneID;
	};

	class LCRetCharLoginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LCRetCharLogin() ; }
		PacketID_t	GetPacketID()const { return PACKET_LC_RETCHARLOGIN ; }
		UINT		GetPacketMaxSize() const
		{ 
			return sizeof(ASKCHARLOGIN_RESULT)
				   + sizeof(CHAR)*IP_SIZE
				   + sizeof(UINT)
				   + sizeof(UINT)
				   + sizeof(BYTE)
				   + sizeof(UINT);
				
		}
	};


	class LCRetCharLoginHandler 
	{
	public:
		static UINT Execute( LCRetCharLogin* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif