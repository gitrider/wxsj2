#ifndef __GC_NEWMONSTER_INCLUDE__
#define __GC_NEWMONSTER_INCLUDE__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCNewMonster : public Packet
{
public:
	GCNewMonster( ){} ;
	virtual ~GCNewMonster( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWMONSTER ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(FLOAT) +
															sizeof(FLOAT) +
															sizeof(WORLD_POS)+
															sizeof(INT)+
															sizeof(INT); }

public:
	//使用数据接口
	VOID				setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t				getObjID(VOID)const { return m_ObjID; }

	VOID				setWorldPos(const WORLD_POS& pos) { m_posWorld = pos; }
	const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

	FLOAT				getDir( )const{ return m_fDir; }
	VOID				setDir( FLOAT fDir ){ m_fDir = fDir; }

	VOID				setMoveSpeed( FLOAT fMoveSpeed ){ m_fMoveSpeed = fMoveSpeed; }
	FLOAT				getMoveSpeed( VOID )const{ return m_fMoveSpeed; }
	INT					getHorseID() const { return m_iHorseID; }
	VOID				setHorseID(INT val) { m_iHorseID = val; }
	INT					getWeaponID() const { return m_iWeapon; }
	VOID				setWeaponID(INT val) { m_iWeapon = val; }

private:
	ObjID_t			m_ObjID;		// ObjID
	WORLD_POS		m_posWorld;		// 位置
	FLOAT			m_fDir;			// 方向
	FLOAT			m_fMoveSpeed;	// 移动速度
	INT				m_iHorseID;		// 坐骑ID
	INT				m_iWeapon;		// 武器ID
};


class GCNewMonsterFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCNewMonster() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_NEWMONSTER; }
	UINT		GetPacketMaxSize()const { return sizeof(GCNewMonster) - sizeof(Packet); }
};

class GCNewMonsterHandler 
{
public:
	static UINT Execute( GCNewMonster* pPacket, Player* pPlayer ) ;
};


}

using namespace Packets;



#endif