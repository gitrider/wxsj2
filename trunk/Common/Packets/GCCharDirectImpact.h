
// GCCharDirectImpact.h
// 
// ��������:һ����Ч��(�˺���������)
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARDIRECTIMPACT_H__
#define __GCCHARDIRECTIMPACT_H__

#include "Type.h"
#include "GameStruct_Skill.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCharDirectImpact : public Packet
	{
	public:
		GCCharDirectImpact()
		{
			m_nReceiverID		= INVALID_ID;
			m_nSenderID			= INVALID_ID;
			m_nSenderLogicCount	= -1;
			m_nImpactID			= INVALID_ID;
			m_nSkillID			= INVALID_ID;
		}
		virtual ~GCCharDirectImpact(){}

		//���ü̳нӿ�
		virtual BOOL		Read( SocketInputStream& iStream ) ;
		virtual BOOL		Write( SocketOutputStream& oStream )const ;
		virtual UINT		Execute( Player* pPlayer ) ;

		virtual PacketID_t	GetPacketID()const { return PACKET_GC_CHAR_DIRECT_IMPACT; }
		virtual UINT		GetPacketSize()const 
							{ 
								return	sizeof(m_nReceiverID) 
									+	sizeof(m_nSenderID)
									+	sizeof(m_nSenderLogicCount)
									+	sizeof(m_nImpactID)
									+	sizeof(m_nSkillID)
									+	sizeof(Time_t);

							}
	public:
		//ʹ�����ݽӿ�
		ObjID_t			GetReceiverID(VOID) const {return m_nReceiverID;}
		VOID			SetReceiverID(ObjID_t nID) {m_nReceiverID=nID;}

		ObjID_t			GetSenderID(VOID) const {return m_nSenderID;}
		VOID			SetSenderID(ObjID_t nID) {m_nSenderID=nID;}

		INT				GetSenderLogicCount(VOID) const {return m_nSenderLogicCount;}
		VOID			SetSenderLogicCount(INT nCount){m_nSenderLogicCount = nCount;}

		ImpactID_t		GetImpactID(VOID) const {return m_nImpactID;}
		VOID			SetImpactID(ImpactID_t nID) {m_nImpactID=nID;}

		SkillID_t		GetSkillID(VOID) const {return m_nSkillID;}
		VOID			SetSkillID(SkillID_t nID) {m_nSkillID=nID;}
		
		Time_t			GetDelayTime() const { return m_iDelayTime; }
		VOID			SetDelayTime(Time_t val) { m_iDelayTime = val; }

	private:
		ObjID_t			m_nReceiverID;		// Ч�������ߵ�ID
		ObjID_t			m_nSenderID;		// Ч��ʩ���ߵ�ID
		INT				m_nSenderLogicCount;// �����ߵ��߼�����
		ImpactID_t		m_nImpactID;		// Ч��ID //�ο�GameStruct_Impact.h��DIRECT_IMPACT_SEID
		SkillID_t		m_nSkillID;			// ���ܵ�ID
		Time_t			m_iDelayTime;

	};


	class GCCharDirectImpactFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharDirectImpact() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHAR_DIRECT_IMPACT; }
		UINT		GetPacketMaxSize()const
					{
						return	sizeof(ObjID_t) 
							+	sizeof(ObjID_t)
							+	sizeof(INT)
							+	sizeof(ImpactID_t)
							+	sizeof(SkillID_t)
							+	sizeof(Time_t);
					}
	};

	class GCCharDirectImpactHandler 
	{
	public:
		static UINT Execute( GCCharDirectImpact* pPacket, Player* pPlayer );
	};
}

#endif
