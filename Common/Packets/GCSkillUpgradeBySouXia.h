/********************************************************************
	created:	2010/05/10
	filename: 	Common\Packets\GCSkillUpgradeBySouXia.h
	file path:	Common\Packets
	file base:	GCSkillUpgradeBySouXia
	file ext:	h
	author:		genghao
	purpose:	server验证client通过捜侠录的技能学习请求后向client返回结果
*********************************************************************/
#ifndef _GC_SOUXIA_SKILL_ADD_H_
#define _GC_SOUXIA_SKILL_ADD_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
namespace Packets
{
	class GCAddSouXiaSkill:public Packet
	{

	public:
		GCAddSouXiaSkill();
		virtual				~GCAddSouXiaSkill(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_SOUXIASKILL_ADD; }
		virtual UINT			GetPacketSize() const { return sizeof(SHORT)+sizeof(BYTE)+sizeof(SkillID_t)+sizeof(SHORT)+sizeof(BYTE);}

	public:
		__inline	SHORT	GetCurSouXiaPos() const		{ return m_nCurSouXiaPos;};
		__inline	VOID	SetCurSouXiaPos(SHORT curSouXia){ m_nCurSouXiaPos = curSouXia;}

		__inline	BYTE	GetSkillType() const		{ return m_nStudySkillType;}
		__inline	VOID	SetSkillType(BYTE val)		{ m_nStudySkillType = val;}

		__inline	SHORT	GetCurUseTime() const		{ return m_nCurUseTime;}
		__inline	VOID	SetCurUseTime(SHORT val)	{ m_nCurUseTime = val;}
		
		//************************************
		// Method:    GetSkillID
		// FullName:  Packets::GCSkillUpgradeBySouXia::GetSkillID
		// Access:    public 
		// Returns:   INT
		// Qualifier: const
		// 获取技能ID
		//************************************
		SkillID_t		GetSkillID() const;
		//************************************
		// Method:    SetSkillID
		// FullName:  Packets::GCSkillUpgradeBySouXia::SetSkillID
		// Access:    public 
		// Returns:   VOID
		// Qualifier:
		// Parameter: INT val
		// 设置技能ID
		//************************************
		VOID			SetSkillID(SkillID_t val);
		VOID			SetResult(BYTE Result){ m_nResult = Result; }
		BYTE			GetResult(){return m_nResult;}
		
	private:
		SHORT			m_nCurSouXiaPos;//当前装备的捜侠录的位置
		BYTE			m_nStudySkillType;//当前打开捜侠录的页类型
		//技能ID
		SkillID_t		m_nSkillID;
		SHORT			m_nCurUseTime;	// 当前可以用的次数
		
		BYTE			m_nResult;		//是否成功，不成功包含错误信息
	};

	class GCAddSouXiaSkillFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCAddSouXiaSkill(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_SOUXIASKILL_ADD; }
		UINT		GetPacketMaxSize() const {return sizeof(SHORT)+sizeof(BYTE)+sizeof(SkillID_t)+sizeof(SHORT)+sizeof(BYTE); }
	};

	class GCAddSouXiaSkillHandler
	{
	public:
		static UINT Execute( GCAddSouXiaSkill* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // _GC_SOUXIA_SKILL_ADD_H_

