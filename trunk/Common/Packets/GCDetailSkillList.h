// GCDetailSkillList.h
// 
// 当前主角的详细技能表
// 
//////////////////////////////////////////////////////

#ifndef __GCDETAILSKILLLIST_H__
#define __GCDETAILSKILLLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Skill.h"

namespace Packets
{
class GCDetailSkillList : public Packet
{
public:
	GCDetailSkillList( ){}
	virtual ~GCDetailSkillList( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_DETAILSKILLLIST ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) + sizeof(WORD) +
															sizeof(_OWN_SKILL) * m_wNumSkill; }

public:
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	WORD		getNumSkill( VOID )const{ return m_wNumSkill; }
	const _OWN_SKILL	*getSkill( WORD wIndex )const{ return &m_aSkill[wIndex]; }
	const _OWN_SKILL	*getSkillList( VOID )const{ return &m_aSkill[0]; }

	VOID		setSkillList( WORD wNum, const _OWN_SKILL *paSkill ){
					m_wNumSkill = wNum;
					memcpy( m_aSkill, paSkill, sizeof(_OWN_SKILL)*m_wNumSkill );
				}
private:
	ObjID_t		m_ObjID;	// 所有Obj类型的ObjID
	WORD		m_wNumSkill;
	_OWN_SKILL	m_aSkill[MAX_CHAR_SKILL_NUM];
};


class GCDetailSkillListFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCDetailSkillList() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_DETAILSKILLLIST; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) + sizeof(WORD) +
													sizeof(_OWN_SKILL) * MAX_CHAR_SKILL_NUM; }
};

class GCDetailSkillListHandler 
{
public:
	static UINT Execute( GCDetailSkillList* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif
