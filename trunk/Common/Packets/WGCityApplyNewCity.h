// WGCityApplyNewCity.h
// 
// �ظ��������뽨���µĳ���
// 
//////////////////////////////////////////////////////

#ifndef __WGCITYAPPLYNEWCITY_H__
#define __WGCITYAPPLYNEWCITY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGCityApplyNewCity : public Packet 
	{
	public:
		WGCityApplyNewCity( )
		{
			m_PlayerGuid = INVALID_GUID;
		}
		virtual ~WGCityApplyNewCity( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_CITYAPPLYNEWCITY; }
		virtual UINT			GetPacketSize() const { return sizeof(SceneID_t) + sizeof(GUID_t);  }

	public:
		//ʹ�����ݽӿ�
		SceneID_t				GetSceneID(){ return m_PortSceneID;}
		VOID					SetSceneID(SceneID_t Sceneid){m_PortSceneID = Sceneid;}

		//ʹ�����ݽӿ�
		GUID_t					GetPlayerGuid(){ return m_PlayerGuid;}
		VOID					SetPlayerGuid(GUID_t PlayerGuid){m_PlayerGuid = PlayerGuid;}

	private:
		//��ڳ���ID
		SceneID_t				m_PortSceneID;
		GUID_t					m_PlayerGuid;
	};

	class WGCityApplyNewCityFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGCityApplyNewCity() ; }
		PacketID_t	GetPacketID() const { return PACKET_WG_CITYAPPLYNEWCITY; }
		UINT		GetPacketMaxSize() const { return sizeof(SceneID_t) + sizeof(GUID_t); }
	};

	class WGCityApplyNewCityHandler 
	{
	public:
		static UINT Execute( WGCityApplyNewCity* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif
