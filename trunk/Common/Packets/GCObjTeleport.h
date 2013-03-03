// GCObjTeleport.h
//
// ���ã�OBJλ�õ�˲��
//
////////////////////////////////////////////////////////

#ifndef __GCOBJTELEPORT_H__
#define __GCOBJTELEPORT_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCObjTeleport : public Packet
	{
	public:
		GCObjTeleport( ){}
		virtual ~GCObjTeleport( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream )const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_OBJ_TELEPORT; }
		virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) + sizeof(WORLD_POS); }

	public:
		//ʹ�����ݽӿ�
		VOID				setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t				getObjID(VOID)const { return m_ObjID; }

		VOID				setWorldPos(WORLD_POS& pos) { m_posWorld = pos; }
		const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

	private:
		//����
		ObjID_t			m_ObjID;	//ID
		WORLD_POS		m_posWorld;	//����

	};

	class GCObjTeleportFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCObjTeleport(); }
		PacketID_t	GetPacketID()const { return PACKET_GC_OBJ_TELEPORT; }
		UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) + sizeof(WORLD_POS); }
	};

	class GCObjTeleportHandler 
	{
	public:
		static UINT Execute( GCObjTeleport* pPacket, Player* pPlayer );
	};
}

#endif // __GCOBJTELEPORT_H__