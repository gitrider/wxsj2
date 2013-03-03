// GCNewPet_Move.h
//
///////////////////////////////////////////////////////

#ifndef __GCNEWPET_MOVE_H__
#define __GCNEWPET_MOVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCNewPet_Move : public Packet
	{
	public:
		GCNewPet_Move( ){} ;
		virtual ~GCNewPet_Move( ){} ;

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWPET_MOVE; }
		virtual UINT			GetPacketSize()const
		{
			return	sizeof(ObjID_t) + sizeof( INT ) + sizeof(WORLD_POS) + sizeof(FLOAT) + sizeof(WORLD_POS);
		}

	public:
		//ʹ�����ݽӿ�
		VOID				setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t				getObjID(VOID)const { return m_ObjID; }

		VOID				setHandleID( INT nID ) { m_nHandleID = nID; }
		INT					getHandleID( VOID )const{ return m_nHandleID; }

		VOID				setWorldPos(const WORLD_POS& pos) { m_posWorld = pos; }
		const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

		VOID				setMoveSpeed( FLOAT fMoveSpeed ){ m_fMoveSpeed = fMoveSpeed; }
		FLOAT				getMoveSpeed( VOID )const{ return m_fMoveSpeed; }

		VOID				setTargetPos( const WORLD_POS& pos) { m_posTarget = pos; }
		const WORLD_POS&	getTargetPos(VOID)const { return m_posTarget; }

	private:
		ObjID_t			m_ObjID;		// ObjID
		INT				m_nHandleID;
		WORLD_POS		m_posWorld;		// λ��

		FLOAT			m_fMoveSpeed;	// �ƶ��ٶ�
		WORLD_POS		m_posTarget;	// �ƶ���Ŀ���
	};


	class GCNewPet_MoveFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCNewPet_Move() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_NEWPET_MOVE; }
		UINT		GetPacketMaxSize()const
		{
			return	sizeof(ObjID_t) + sizeof( INT) + sizeof(WORLD_POS) + sizeof(FLOAT) + sizeof(WORLD_POS);
		}
	};

	class GCNewPet_MoveHandler 
	{
	public:
		static UINT Execute( GCNewPet_Move* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
