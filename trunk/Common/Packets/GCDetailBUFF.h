#ifndef __GCDETAILBUFF_H__
#define __GCDETAILBUFF_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Skill.h"

namespace Packets
{
	class GCDetailBuff : public Packet
	{
	public:
		GCDetailBuff(){
			Reset();
		}
		virtual ~GCDetailBuff(){}

		VOID Reset(VOID){
			m_nReceiverID		= INVALID_ID;
			m_nSenderID			= INVALID_ID;
			m_nSkillID			= INVALID_ID;
			m_nBuffID			= INVALID_ID;
			m_bEnable			= FALSE;
			m_nSenderLogicCount = 0;
			m_nSN				= 0;
			m_nContinuance		= -1;
			m_iDelayTime		= 0;
		}

		//���ü̳нӿ�
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);

		virtual PacketID_t		GetPacketID()const {return PACKET_GC_DETAIL_BUFF;}
		virtual UINT			GetPacketSize()const 
								{
									UINT uSize = 0;
									uSize	= 		sizeof(m_nReceiverID)
												+	sizeof(m_nSenderID)
												+ 	sizeof(m_nBuffID)
												+	sizeof(m_nSkillID)
												+ 	sizeof(m_bEnable)
												+	sizeof(m_nSenderLogicCount)
												+	sizeof(m_nSN)
												+	sizeof(m_nContinuance)
												+	sizeof(m_iDelayTime);
									return uSize;
								}

	public:
		//ʹ�����ݽӿ�
		VOID			SetReceiverID(ObjID_t id) {m_nReceiverID = id;}
		ObjID_t			GetReceiverID(VOID)const {return m_nReceiverID;}

		VOID			SetSenderID(ObjID_t id) {m_nSenderID = id;}
		ObjID_t			GetSenderID(VOID)const {return m_nSenderID;}

		VOID			SetBuffID(ImpactID_t id) {m_nBuffID = id;}
		ImpactID_t		GetBuffID(VOID)const {return m_nBuffID;}

		VOID			SetSkillID(SkillID_t nID) {m_nSkillID = nID;}
		SkillID_t		GetSkillID(VOID) {return m_nSkillID;}

		VOID			SetEnable(BOOL bEnable) {m_bEnable = bEnable;}
		BOOL			GetEnable(VOID)const {return m_bEnable;}

		VOID			SetSenderLogicCount(INT nCount) {m_nSenderLogicCount=nCount;}
		INT				GetSenderLogicCount(VOID) const {return m_nSenderLogicCount;}

		VOID			SetSN(UINT nSN) {m_nSN=nSN;}
		UINT			GetSN(VOID) {return m_nSN;}

		VOID			SetContinuance(Time_t nContinuance) {m_nContinuance=nContinuance;};
		Time_t			GetContinuance(VOID) const {return m_nContinuance;};

		Time_t			GetDelayTime() const { return m_iDelayTime; }
		VOID			SetDelayTime(Time_t val) { m_iDelayTime = val; }

	private:
		ObjID_t			m_nReceiverID;		// Ч�������ߵ�ID
		ObjID_t			m_nSenderID;		// Ч���ͷ��ߵ�ID
		ImpactID_t		m_nBuffID;			// ��Ч���ݵ�ID(����)
		SkillID_t		m_nSkillID;  		// Skill ID
		BOOL			m_bEnable; 			// ��Ч����Ч��Ϣ����Ч����ʧ����Ϣ
		INT				m_nSenderLogicCount; //Ч�������ߵ��߼�����
		UINT			m_nSN;				// Ч�����к�
		Time_t			m_nContinuance;		// Ч���ĳ���ʱ��
		Time_t			m_iDelayTime;
	};

	class GCDetailBuffFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() {return new GCDetailBuff();}
		PacketID_t	GetPacketID()const {return PACKET_GC_DETAIL_BUFF;}
		UINT		GetPacketMaxSize()const 
					{
						return 	sizeof(ObjID_t)
							+	sizeof(ObjID_t)
							+ 	sizeof(ImpactID_t)
							+	sizeof(SkillID_t)
							+ 	sizeof(BOOL)
							+	sizeof(INT)
							+	sizeof(UINT)
							+	sizeof(Time_t)
							+	sizeof(Time_t);
					}
	};

	class GCDetailBuffHandler 
	{
	public:
		static UINT Execute(GCDetailBuff* pPacket, Player* pPlayer);
	};
}

using namespace Packets;



#endif
