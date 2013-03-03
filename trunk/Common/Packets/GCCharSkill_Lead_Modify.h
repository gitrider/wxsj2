// GCCharSkill_Lead_Modify.h
// 
// ��������ʱ������̵ȵ�
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARSKILL_LEAD_MODIFY_H__
#define __GCCHARSKILL_LEAD_MODIFY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCCharSkill_Lead_Modify : public Packet
{
public:
	GCCharSkill_Lead_Modify( ){}
	virtual ~GCCharSkill_Lead_Modify( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARSKILL_LEAD_MODIFY; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(FLOAT); }

public:
	//ʹ�����ݽӿ�
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID			setSubTime(INT nSubTime) { m_nSubTime = nSubTime; }
	INT				getSubTime(VOID)const { return m_nSubTime; }

private:
	ObjID_t			m_ObjID;			// ObjID
	INT				m_nSubTime;			// ���̵�����ʱ��
};


class GCCharSkill_Lead_ModifyFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCCharSkill_Lead_Modify() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_CHARSKILL_LEAD_MODIFY; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(FLOAT); }
};

class GCCharSkill_Lead_ModifyHandler 
{
public:
	static UINT Execute( GCCharSkill_Lead_Modify* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif