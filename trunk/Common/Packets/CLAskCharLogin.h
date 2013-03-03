#ifndef __CLASKCHARLOGIN_H__
#define __CLASKCHARLOGIN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class CLAskCharLogin : public Packet 
	{
	public:
		CLAskCharLogin( ){} ;
		virtual ~CLAskCharLogin( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKCHARLOGIN ; }
		virtual UINT			GetPacketSize() const 
		{
			return 	sizeof(GUID_t)+sizeof(PlayerID_t)+sizeof(SceneID_t);

		}

	public:
		//ʹ�����ݽӿ�
		GUID_t					GetCharGuid()	const;
		VOID					SetCharGuid(GUID_t	guid);

		PlayerID_t				GetPlayerID() const
		{
			return PlayerID;
		}

		VOID					SetPlayerID(PlayerID_t	 pID)
		{
			PlayerID = pID;
		}

		SceneID_t				GetSceneID()	const
		{
			return SceneID;
		}

		VOID					SetSceneID(SceneID_t	sid)
		{
			SceneID = sid;
		}
	private:
		//����
		GUID_t					CharGuid;
		//��ҳ�id���ͻ��˲�����д
		PlayerID_t				PlayerID;
		//������
		SceneID_t				SceneID;
	};

	class CLAskCharLoginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskCharLogin() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKCHARLOGIN ; }
		UINT		GetPacketMaxSize() const
		{ 
			return 	sizeof(GUID_t)+sizeof(PlayerID_t)+sizeof(SceneID_t);
		}
	};


	class CLAskCharLoginHandler 
	{
	public:
		static UINT Execute( CLAskCharLogin* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif