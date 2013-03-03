#ifndef __GCNEWMONSTER_DEATH_H__
#define __GCNEWMONSTER_DEATH_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCNewMonster_Death : public Packet
{
public:
	GCNewMonster_Death( ){} ;
	virtual ~GCNewMonster_Death( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWMONSTER_DEATH; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(FLOAT) +
															sizeof(FLOAT) +
															sizeof(WORLD_POS); }

public:
	//ʹ�����ݽӿ�
	VOID				setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t				getObjID(VOID)const { return m_ObjID; }

	VOID				setWorldPos(const WORLD_POS& pos) { m_posWorld = pos; }
	const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

	FLOAT				getDir( )const{ return m_fDir; }
	VOID				setDir( FLOAT fDir ){ m_fDir = fDir; }

	VOID				setMoveSpeed( FLOAT fMoveSpeed ){ m_fMoveSpeed = fMoveSpeed; }
	FLOAT				getMoveSpeed( VOID )const{ return m_fMoveSpeed; }

private:
	ObjID_t			m_ObjID;		// ObjID
	WORLD_POS		m_posWorld;		// λ��
	FLOAT			m_fDir;			// ����
	FLOAT			m_fMoveSpeed;	// �ƶ��ٶ�
};


class GCNewMonster_DeathFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCNewMonster_Death() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_NEWMONSTER_DEATH; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(FLOAT) +
													sizeof(FLOAT) +
													sizeof(WORLD_POS); }
};

class GCNewMonster_DeathHandler 
{
public:
	static UINT Execute( GCNewMonster_Death* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif