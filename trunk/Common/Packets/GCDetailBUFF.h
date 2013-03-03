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

		//公用继承接口
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
		//使用数据接口
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
		ObjID_t			m_nReceiverID;		// 效果接受者的ID
		ObjID_t			m_nSenderID;		// 效果释放者的ID
		ImpactID_t		m_nBuffID;			// 特效数据的ID(索引)
		SkillID_t		m_nSkillID;  		// Skill ID
		BOOL			m_bEnable; 			// 是效果生效消息还是效果消失的消息
		INT				m_nSenderLogicCount; //效果创建者的逻辑计数
		UINT			m_nSN;				// 效果序列号
		Time_t			m_nContinuance;		// 效果的持续时间
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
