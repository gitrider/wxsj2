// GWCityApplyNewCity.h
// 
// �������뽨���µĳ���
// 
//////////////////////////////////////////////////////

#ifndef __GWCITYAPPLYNEWCITY_H__
#define __GWCITYAPPLYNEWCITY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWCityApplyNewCity : public Packet 
	{
	public:
		GWCityApplyNewCity( ){} ;
		virtual ~GWCityApplyNewCity( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_CITYAPPLYNEWCITY; }
		virtual UINT			GetPacketSize() const { return sizeof(SceneID_t) + sizeof(GUID_t); }

	public:
		//ʹ�����ݽӿ�
		SceneID_t				GetSceneID(){ return m_PortSceneID;}
		VOID					SetSceneID(SceneID_t Sceneid){m_PortSceneID = Sceneid;}

		GUID_t					GetPlayerGuid(){ return m_ApplicantGuid;}
		VOID					SetPlayerGuid(GUID_t PlayerGuid){m_ApplicantGuid = PlayerGuid;}

	private:
		//��ڳ���ID
		SceneID_t				m_PortSceneID;
		//������GUID
		GUID_t					m_ApplicantGuid;

	};

	class GWCityApplyNewCityFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWCityApplyNewCity() ; }
		PacketID_t	GetPacketID() const { return PACKET_GW_CITYAPPLYNEWCITY ; }
		UINT		GetPacketMaxSize() const { return sizeof(SceneID_t) + sizeof(GUID_t); }
	};

	class GWCityApplyNewCityHandler 
	{
	public:
		static UINT Execute( GWCityApplyNewCity* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif
