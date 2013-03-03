// GCCharSkill_Missed.h
// 
// ����û����
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARSKILL_MISSED_H__
#define __GCCHARSKILL_MISSED_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{


	class GCCharSkill_Missed : public Packet
	{
	public:
		GCCharSkill_Missed() : m_nReceiverID(INVALID_ID), m_nSenderID(INVALID_ID), m_nSkillID(INVALID_ID), m_nSenderLogicCount(0){}
		virtual ~GCCharSkill_Missed( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARSKILL_MISSED; }
		virtual UINT			GetPacketSize()const 
								{
									return 	sizeof(m_nReceiverID)
										+	sizeof(m_nSenderID)
										+	sizeof(m_nSkillID)
										+	sizeof(m_nFlag)
										+	sizeof(m_nSenderLogicCount);
								}

	public:

	public:
		//ʹ�����ݽӿ�
		SkillID_t	GetSkillID(VOID) const {return m_nSkillID;}
		VOID		SetSkillID(SkillID_t nSkillID) {m_nSkillID=nSkillID;}

		ObjID_t		GetReceiverID(VOID) const {return m_nReceiverID;}
		VOID		SetReceiverID(ObjID_t nID) {m_nReceiverID=nID;}

		ObjID_t		GetSenderID(VOID) const {return m_nSenderID;}
		VOID		SetSenderID(ObjID_t nID) {m_nSenderID=nID;}

		ID_t		GetFlag(VOID) const {return m_nFlag;}
		VOID		SetFlag(ID_t nFlag) {m_nFlag = nFlag;}
		
		INT			GetSenderLogicCount(VOID) const {return m_nSenderLogicCount;}
		VOID		SetSenderLogicCount(INT nID) {m_nSenderLogicCount=nID;}
	private:
		ObjID_t		m_nReceiverID;	// Ч�������ߵ�ID
		ObjID_t		m_nSenderID;	// Ч���ͷ��ߵ�ID
		SkillID_t 	m_nSkillID;		// ���ܵ�ID
		ID_t		m_nFlag;		// δ���У����ߣ����գ�ת�Ƶı��
		INT			m_nSenderLogicCount;
	};


	class GCCharSkill_MissedFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharSkill_Missed() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARSKILL_MISSED; }
		UINT		GetPacketMaxSize()const 
					{ 
						return 	sizeof(ObjID_t)
							+	sizeof(ObjID_t)
							+	sizeof(SkillID_t)
							+	sizeof(ID_t)
							+	sizeof(INT);
					}
	};

	class GCCharSkill_MissedHandler 
	{
	public:
		static UINT Execute( GCCharSkill_Missed* pPacket, Player* pPlayer ) ;
	};
}

#endif
