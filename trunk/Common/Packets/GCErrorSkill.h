

#ifndef __GCERRORSKILL_H__
#define __GCERRORSKILL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GCErrorSkill : public Packet 
{
public:
	GCErrorSkill( ){} ;
	virtual ~GCErrorSkill( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_ERRORSKILL ; }
	virtual UINT			GetPacketSize()const { return 	sizeof(BYTE) +
															sizeof(ObjID_t)*2 +
															sizeof(SkillID_t) ; }
	
public:
	//ʹ�����ݽӿ�
	BYTE			getReason(VOID) { return m_byReason; }
	VOID			setReason(BYTE byReason) { m_byReason = byReason; }

	ObjID_t			getAttackID(VOID) { return m_AttObjID; }
	VOID			setAttackID(ObjID_t idAttack) { m_AttObjID = idAttack; }

	ObjID_t			getTargetID(VOID) { return m_TarObjID; }
	VOID			setTargetID(ObjID_t idTarget) { m_TarObjID = idTarget; }

	SkillID_t				getSkillID(VOID) { return m_SkillID; }
	VOID					setSkillID(SkillID_t skillid) { m_SkillID = skillid; }

private:
	//����
	BYTE			m_byReason;		//ʧ��ԭ�� enum ATTACK_ERROR
	ObjID_t			m_AttObjID;		//��������ID
	ObjID_t			m_TarObjID;		//����������ID
	SkillID_t		m_SkillID ;						//ʹ�õļ���ID

};


class GCErrorSkillFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCErrorSkill() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_ERRORSKILL ; }
	UINT		GetPacketMaxSize()const { return 	sizeof(BYTE) +
													sizeof(ObjID_t)*2 +
													sizeof(SkillID_t) ; }
};


class GCErrorSkillHandler 
{
public:
	static UINT Execute( GCErrorSkill* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
