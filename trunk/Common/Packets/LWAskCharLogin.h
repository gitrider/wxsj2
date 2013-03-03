#ifndef _LWASKCHARLOGIN_H_
#define _LWASKCHARLOGIN_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#include "DB_Struct.h"

namespace Packets
{
	enum	ASK_LOGINSTATUS
	{
		ALS_ASKSTATUS,
		ALS_SENDDATA,
	};

	class LWAskCharLogin : public Packet 
	{
	public:
		LWAskCharLogin( ){} ;
		virtual ~LWAskCharLogin( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LW_ASKCHARLOGIN ; }
		virtual UINT			GetPacketSize() const 
		{
			if(AskStatus == ALS_SENDDATA)
			{
				return 	sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(PlayerID_t)+
					sizeof(GUID_t)+
					sizeof(UINT)+
					sizeof(ASK_LOGINSTATUS)+
					sizeof(FULLUSERDATA);
			}
			else
			{
				
				return 	sizeof(CHAR)*MAX_ACCOUNT+
						sizeof(PlayerID_t)+
						sizeof(GUID_t)+
						sizeof(UINT)+
						sizeof(ASK_LOGINSTATUS);
			}
					
		}

	public:
		//ʹ�����ݽӿ�
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);

		PlayerID_t				GetPlayerID() const
		{
			return PlayerID;
		}

		VOID					SetPlayerID(PlayerID_t	 pID)
		{
			PlayerID = pID;
		}

		GUID_t					GetPlayerGUID()
		{
			return PlayerGUID;
		}

		VOID					SetPlayerGUID(GUID_t guid)
		{
			PlayerGUID	= guid;
		}

		ASK_LOGINSTATUS			GetAskStatus()
		{
			return AskStatus;
		}
		VOID					SetAskStatus(ASK_LOGINSTATUS als)
		{
			AskStatus = als;
		}
		FULLUSERDATA*			GetUserData()
		{
			return &UserData;
		}

		VOID					SetUserKey(UINT key)
		{
			uKey = key;
		}

		UINT					GetUserKey()
		{
			return uKey;
		}

		VOID					SetUserData(FULLUSERDATA* pUserData)
		{
			memcpy(&UserData,pUserData,sizeof(FULLUSERDATA));
		}
	private:
		//����
		CHAR					szAccount[MAX_ACCOUNT+1];	//�û�����
		//��ҳ�id
		PlayerID_t				PlayerID;
		//���GUID_t
		GUID_t					PlayerGUID;
		//��ѯ״̬
		ASK_LOGINSTATUS			AskStatus;
		//��ɫ����
		FULLUSERDATA			UserData;
		//��֤��
		UINT					uKey;
	};

	class LWAskCharLoginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LWAskCharLogin() ; }
		PacketID_t	GetPacketID()const { return PACKET_LW_ASKCHARLOGIN ; }
		UINT		GetPacketMaxSize() const
		{ 
			return 	sizeof(CHAR)*MAX_ACCOUNT+
				sizeof(PlayerID_t)+
				sizeof(GUID_t)+
				sizeof(UINT)+
				sizeof(ASK_LOGINSTATUS)+
				sizeof(FULLUSERDATA);
				
		}
	};


	class LWAskCharLoginHandler 
	{
	public:
		static UINT Execute( LWAskCharLogin* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif