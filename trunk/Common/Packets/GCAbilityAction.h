// GCAbilityAction.h

#ifndef __GCABILITYACTION_H__
#define __GCABILITYACTION_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class GCAbilityAction : public Packet 
{
public:

	enum
	{
		ABILITY_BEGIN = 1,
		ABILITY_END,
	};

	GCAbilityAction( )
	{
		m_BeginOrEnd = 0;
	} ;
	virtual ~GCAbilityAction( ){} ;

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream ) const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID() const { return PACKET_GC_ABILITY_ACTION; }
	virtual UINT			GetPacketSize() const { return sizeof(ObjID_t) * 2
															+ sizeof(m_nLogicCount)
															+ sizeof(AbilityID_t)
															+ sizeof(PrescriptionID_t)
															+ sizeof(BYTE); }
	
public:
	//ʹ�����ݽӿ�
	VOID					setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t					getObjID(VOID)const { return m_ObjID; }

	VOID					setLogicCount(INT nCount) { m_nLogicCount = nCount; }
	INT						getLogicCount(VOID)const { return m_nLogicCount; }

	UINT					getAction(VOID) { return m_AbilityID; }
	VOID					setAction(INT nAbilityID) { m_AbilityID = nAbilityID; }

	VOID					setPrescriptionID(PrescriptionID_t pid) { m_PrescriptionID = pid; }
	PrescriptionID_t		getPrescriptionID(VOID) { return m_PrescriptionID; }

	VOID					setTargetID(ObjID_t id) { m_TargetID = id; }
	ObjID_t					getTargetID(VOID)const { return m_TargetID; }

	BYTE					getBeginOrEnd(VOID) { return m_BeginOrEnd; }
	VOID					setBeginOrEnd(BYTE BeginOrEnd) { m_BeginOrEnd = BeginOrEnd; }
private:
	//����
	ObjID_t					m_ObjID;			// ����Obj���͵�ObjID
	INT						m_nLogicCount;		// �߼�����
	AbilityID_t				m_AbilityID;		// �����ID
	PrescriptionID_t		m_PrescriptionID;	// �䷽ ID
	ObjID_t					m_TargetID;			// ����̨�����󣩵� ObjID
	BYTE					m_BeginOrEnd;		// 
};


class GCAbilityActionFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCAbilityAction() ; }
	PacketID_t	GetPacketID() const { return PACKET_GC_ABILITY_ACTION ; }
	UINT		GetPacketMaxSize() const { return sizeof(GCAbilityAction) - sizeof(Packet); }
};


class GCAbilityActionHandler 
{
public:
	static UINT Execute( GCAbilityAction* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;

#endif //  __GCABILITYACTION_H__