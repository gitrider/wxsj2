// GCCharSkill_CreateBullet.h
// 
// �������������ӵ�
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARSKILL_CREATEBULLET_H__
#define __GCCHARSKILL_CREATEBULLET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCCharSkill_CreateBullet : public Packet
{
public:
	GCCharSkill_CreateBullet( ){}
	virtual ~GCCharSkill_CreateBullet( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARSKILL_CREATEBULLET; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(SkillID_t) +
															sizeof(ObjID_t) +
															sizeof(WORLD_POS); }

public:
	//ʹ�����ݽӿ�
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID			setSkillDataID(SkillID_t ID) { m_SkillDataID = ID; }
	SkillID_t		getSkillDataID(VOID)const { return m_SkillDataID; }

	VOID			setTargetID(ObjID_t ID) { m_TargetID = ID; }
	ObjID_t			getTargetID(VOID)const { return m_TargetID; }

	VOID				setTargetPos(WORLD_POS& pos) { m_posTarget = pos; }
	const WORLD_POS&	getTargetPos(VOID)const { return m_posTarget; }

private:
	ObjID_t			m_ObjID;			// ObjID
	SkillID_t		m_SkillDataID;		// ���ܵ���ԴID
	//union{
		ObjID_t		m_TargetID;			// Ŀ���ɫ
		WORLD_POS	m_posTarget;		// Ŀ������
	//};
};


class GCCharSkill_CreateBulletFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCCharSkill_CreateBullet() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_CHARSKILL_CREATEBULLET; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(SkillID_t) +
													sizeof(ObjID_t) +
													sizeof(WORLD_POS); }
};

class GCCharSkill_CreateBulletHandler 
{
public:
	static UINT Execute( GCCharSkill_CreateBullet* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif