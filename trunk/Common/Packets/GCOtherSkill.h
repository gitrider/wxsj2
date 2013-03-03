

#ifndef __GCOTHERSKILL_H__
#define __GCOTHERSKILL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCAttack.h"


namespace Packets
{


class GCOtherSkill : public Packet 
{
public:
	GCOtherSkill( ){} ;
	virtual ~GCOtherSkill( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_OTHERSKILL ; }
	virtual UINT			GetPacketSize()const { return 	sizeof(ObjID_t) +
															sizeof(BYTE) +
															sizeof(_DAMAGE_INFO)*m_byListNum +
															sizeof(SkillID_t) ; }

public:
	
	typedef GCAttack::_DAMAGE_INFO _DAMAGE_INFO;
	
public:
	//ʹ�����ݽӿ�
	ObjID_t					getAttackID(VOID) { return m_ObjID; }
	VOID					setAttackID(ObjID_t idAttack) { m_ObjID = idAttack; }

	BYTE					getListNum(VOID) { return m_byListNum; }
	VOID					setListNum(BYTE byListNum) { m_byListNum = byListNum; }

	_DAMAGE_INFO*				getDamList(VOID) { return m_listDam; }
	VOID					setDamList(_DAMAGE_INFO* pSour, BYTE byListNum)
	{
		if(pSour == NULL) return;
		m_byListNum = byListNum > MAX_DAM_LIST_NUM ? MAX_DAM_LIST_NUM : byListNum;

		memcpy(m_listDam, pSour, sizeof(_DAMAGE_INFO)*m_byListNum);
	}

	SkillID_t				getSkillID(VOID) { return m_SkillID; }
	VOID					setSkillID(SkillID_t skillid) { m_SkillID = skillid; }

private:
	//����
	ObjID_t					m_ObjID;						//��������ObjID
	BYTE					m_byListNum;					//�ܵ������Ķ���ĸ���[0, 128]
	_DAMAGE_INFO				m_listDam[MAX_DAM_LIST_NUM];	//�ܵ������Ķ����б�
	SkillID_t				m_SkillID ;						//ʹ�õļ���ID

};


class GCOtherSkillFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCOtherSkill() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_OTHERSKILL ; }
	UINT		GetPacketMaxSize()const { return 	sizeof(ObjID_t) +
													sizeof(BYTE) +
													sizeof(GCOtherSkill::_DAMAGE_INFO)*MAX_DAM_LIST_NUM +
													sizeof(SkillID_t) ; }
};


class GCOtherSkillHandler 
{
public:
	static UINT Execute( GCOtherSkill* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
