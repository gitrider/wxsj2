

#ifndef __GCERRORATTACK_H__
#define __GCERRORATTACK_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GCErrorAttack : public Packet 
{
public:
	GCErrorAttack( ){} ;
	virtual ~GCErrorAttack( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_ERRORATTACK ; }
	virtual UINT			GetPacketSize()const { return	sizeof(BYTE) +
															sizeof(ObjID_t)*2; }
public:	
	//ʧ��ԭ��
	//�� GameDefine2.h ATTACK_ERROR

public:
	//ʹ�����ݽӿ�
	BYTE			getReason(VOID) { return m_byReason; }
	VOID			setReason(BYTE byReason) { m_byReason = byReason; }

	ObjID_t			getAttackID(VOID) { return m_AttObjID; }
	VOID			setAttackID(ObjID_t idAttack) { m_AttObjID = idAttack; }

	ObjID_t			getTargetID(VOID) { return m_TarObjID; }
	VOID			setTargetID(ObjID_t idTarget) { m_TarObjID = idTarget; }

private:
	//����
	BYTE			m_byReason;		//ʧ��ԭ�� enum ATTACK_ERROR
	ObjID_t			m_AttObjID;		//��������ID
	ObjID_t			m_TarObjID;		//����������ID
};


class GCErrorAttackFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCErrorAttack() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_ERRORATTACK ; }
	UINT		GetPacketMaxSize()const { return	sizeof(BYTE) +
													sizeof(ObjID_t)*2; }
};


class GCErrorAttackHandler 
{
public:
	static UINT Execute( GCErrorAttack* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
