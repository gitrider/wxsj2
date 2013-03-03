#ifndef __GCCHARBUFF_H__
#define __GCCHARBUFF_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCCharBuff : public Packet
{
public:
	GCCharBuff(){Reset();}
	virtual ~GCCharBuff(){}

	VOID Reset( VOID )
	{
		m_nReceiverID		= INVALID_ID;
		m_nSenderID			= INVALID_ID;
		m_nBuffID			= INVALID_ID;
		m_nSkillID			= INVALID_ID;
		m_bEnable			= FALSE;
		m_nSenderLogicCount = 0;
		m_nNumOfLayer		= 1;
		m_nSN				= 0;
		m_iDelayTime		= 0;
	}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream )const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID()const {return PACKET_GC_CHAR_BUFF;}
	virtual UINT			GetPacketSize()const
							{
								UINT uSize = 0;

								uSize =	sizeof(m_nReceiverID)
									+	sizeof(m_nBuffID)
									+	sizeof(m_bEnable)
									+	sizeof(m_nSenderLogicCount)
									+	sizeof(m_nSN)
									+	sizeof(m_nSenderID)
									+	sizeof(m_nSkillID)
									+	sizeof(m_nNumOfLayer)
									+	sizeof(Time_t)*2;


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

	VOID			SetSkillID(SkillID_t id) {m_nSkillID = id;}
	ImpactID_t		GetSkillID(VOID)const {return m_nSkillID;}

	VOID			SetEnable(BOOL bEnable) {m_bEnable = bEnable;}
	BOOL			GetEnable(VOID)const {return m_bEnable;}

	VOID			SetSenderLogicCount(INT nCount) {m_nSenderLogicCount=nCount;};
	INT				GetSenderLogicCount(VOID) const {return m_nSenderLogicCount;};

	VOID			SetSN(UINT nSN) {m_nSN=nSN;}
	UINT			GetSN(VOID) const {return m_nSN;}

	VOID			SetLayerCount( INT uID ) { m_nNumOfLayer = uID; }
	UINT			GetLayerCount(VOID) { return m_nNumOfLayer; }

	Time_t			GetDelayTime() const { return m_iDelayTime; }
	VOID			SetDelayTime(Time_t val) { m_iDelayTime = val; }

	// 设置技能效果持续时间 20100531 BLL
	VOID			SetContinuance(Time_t nContinuance) {m_nContinuance=nContinuance;};
	Time_t			GetContinuance(VOID) const {return m_nContinuance;};

private:
	ObjID_t			m_nReceiverID;		// 效果接受者的ID
	ObjID_t			m_nSenderID;		// 效果施放者的ID
	ImpactID_t		m_nBuffID;			// 特效数据的ID(索引)
	SkillID_t		m_nSkillID;			// 技能ID
	BOOL			m_bEnable; 			// 是效果生效消息还是
	INT				m_nSenderLogicCount; //效果发起者的逻辑计数
	UINT			m_nSN;				 //持续性效果的序列号(用于识别)
	INT				m_nNumOfLayer;		 //先知或同类技能的小球的全局ID
	Time_t			m_iDelayTime;		 //延迟释放时间

	Time_t			m_nContinuance;		// 效果的持续时间 20100531 BLL

};	


class GCCharBuffFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() {return new GCCharBuff();}
	PacketID_t	GetPacketID()const {return PACKET_GC_CHAR_BUFF;}
	UINT		GetPacketMaxSize()const
					{
						return 	sizeof(ObjID_t)
							+	sizeof(ObjID_t)
							+ 	sizeof(ImpactID_t)
							+	sizeof(SkillID_t)
							+ 	sizeof(BOOL)
							+	sizeof(INT)
							+	sizeof(UINT)
							+	sizeof(UINT)
							+	sizeof(Time_t)*2;
					}
};

class GCCharBuffHandler 
{
public:
	static UINT Execute( GCCharBuff* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif
