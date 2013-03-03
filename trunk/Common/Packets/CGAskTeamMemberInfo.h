// CGAskTeamMemberInfo.h

#ifndef __CGASKTEAMMEMBERINFO_H__
#define __CGASKTEAMMEMBERINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGAskTeamMemberInfo : public Packet
	{
	public:
		CGAskTeamMemberInfo( ){}
		virtual ~CGAskTeamMemberInfo( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream )const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKTEAMMEMBERINFO; }
		virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) + sizeof(GUID_t) + sizeof(SceneID_t); }

	public:
		//ʹ�����ݽӿ�
		ObjID_t					getObjID(VOID) { return m_ObjID; }
		VOID					setObjID(ObjID_t idTarget) { m_ObjID = idTarget; }

		VOID					SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t					GetGUID( ){ return m_GUID ; }

		VOID					SetSceneID( SceneID_t SceneID ){ m_SceneID = SceneID ; }
		SceneID_t				GetSceneID( ){ return m_SceneID ; }

	private:
		//����
		SceneID_t				m_SceneID;	//�Է����ڵĳ���ID
		GUID_t					m_GUID;		//�Է���GUID
		ObjID_t					m_ObjID;	//�Է���ObjID
	};


	class CGAskTeamMemberInfoFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskTeamMemberInfo(); }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKTEAMMEMBERINFO; }
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) + sizeof(GUID_t) + sizeof(SceneID_t); }
	};

	class CGAskTeamMemberInfoHandler 
	{
	public:
		static UINT Execute( CGAskTeamMemberInfo* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // __CGASKTEAMMEMBERINFO_H__
