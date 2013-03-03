#ifndef _GC_SOUXIA_SKILL_UPDATE_H_
#define _GC_SOUXIA_SKILL_UPDATE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCSouXiaSkillUpdate:public Packet
	{
	public:
		GCSouXiaSkillUpdate(VOID)			{}
		virtual	~GCSouXiaSkillUpdate(VOID){}


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_SOUXIASKILL_UPDATE ; }
		virtual UINT			GetPacketSize() const 
		{ 
			return sizeof(SHORT) + sizeof(BYTE) + sizeof(SkillID_t) + sizeof(SHORT) ;
		}

		VOID					SetCurSouXiaPos(SHORT pos){m_nCurSouXiaPos = pos;}
		SHORT					GetCurSouXiaPos(){ return m_nCurSouXiaPos;}

		VOID					SetSouXiaSkillType(BYTE type){m_nSkillType = type;}
		BYTE					GetSouXiaSkillType(){ return m_nSkillType;}

		VOID					SetSouXiaSkillID(SkillID_t skillId){m_nSkillId = skillId;}
		SkillID_t				GetSouXiaSkillID(){ return m_nSkillId;}

		VOID					SetSouXiaSkillTime(SkillID_t leftTime){m_nLeftTime = leftTime;}
		SHORT					GetSouXiaSkillTime(){ return m_nLeftTime;}

	private:
		SHORT			m_nCurSouXiaPos;//更新的捜侠录在身上的位置
		BYTE			m_nSkillType;//更新类型（页类型）3.神兽召唤技能 4.坐骑召唤技能
		SkillID_t		m_nSkillId;	//技能ID
		SHORT			m_nLeftTime;//还剩余的次数

	};

	class GCSouXiaSkillUpdateFactory : public PacketFactory
	{
	public:
		Packet*		CreatePacket()	 { return new GCSouXiaSkillUpdate() ; }
		PacketID_t	GetPacketID()	const { return PACKET_GC_SOUXIASKILL_UPDATE; }
		UINT		GetPacketMaxSize()const { return sizeof(SHORT) + sizeof(BYTE) + sizeof(SkillID_t) + sizeof(SHORT); }
	};

	class GCSouXiaSkillUpdateHandler
	{
	public:
		static UINT Execute( GCSouXiaSkillUpdate* pPacket, Player* pPlayer );
	};

}

#endif //_GC_SOUXIA_SKILL_TIME_H_
