


#ifndef __GCSKILLPREPARE_H__
#define __GCSKILLPREPARE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GCSkillPrepare : public Packet 
{
public:
	GCSkillPrepare( ){} ;
	virtual ~GCSkillPrepare( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_SKILLPREPARE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t)*2 +
															sizeof(SkillID_t) ; }
	

public:
	//ʹ�����ݽӿ�
	ObjID_t					getSourID(VOID) { return m_ObjSourID; }
	VOID					setSourID(ObjID_t idSour) { m_ObjSourID = idSour; }

	ObjID_t					getDestID(VOID) { return m_ObjDestID; }
	VOID					setDestID(ObjID_t idDest) { m_ObjDestID = idDest; }

	SkillID_t				getSkillID(VOID) { return m_SkillID; }
	VOID					setSkillID(SkillID_t skillid) { m_SkillID = skillid; }


private:
	//����
	ObjID_t					m_ObjSourID ;	//�����ߵ�ObjID
	ObjID_t					m_ObjDestID ;	//���������ObjID
	SkillID_t				m_SkillID;	//����ID

};


class GCSkillPrepareFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCSkillPrepare() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_SKILLPREPARE ; }
	UINT		GetPacketMaxSize()const { return 	sizeof(ObjID_t)*2 +
													sizeof(SkillID_t) ; }
};


class GCSkillPrepareHandler 
{
public:
	static UINT Execute( GCSkillPrepare* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
