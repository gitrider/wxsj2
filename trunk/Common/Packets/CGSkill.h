

#ifndef __CGSKILL_H__
#define __CGSKILL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class CGSkill : public Packet 
{
public:
	CGSkill( ){} ;
	virtual ~CGSkill( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_SKILL ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t)+
															sizeof(SkillID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	ObjID_t					getTargetID(VOID) { return m_ObjID; }
	VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }

	SkillID_t				getSkillID(VOID) { return m_SkillID; }
	VOID					setSkillID(SkillID_t skillid) { m_SkillID = skillid; }


private:
	//����
	ObjID_t					m_ObjID;	//�Է���ObjID
	SkillID_t				m_SkillID;	//����ID

};


class CGSkillFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGSkill() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_SKILL ; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t)+
													sizeof(SkillID_t) ; }
};


class CGSkillHandler 
{
public:
	static UINT Execute( CGSkill* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
