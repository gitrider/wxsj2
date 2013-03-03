/********************************************************************
	created:	2010/05/10
	filename: 	Common\Packets\GCSkillUpgradeBySouXia.h
	file path:	Common\Packets
	file base:	GCSkillUpgradeBySouXia
	file ext:	h
	author:		genghao
	purpose:	server��֤clientͨ������¼�ļ���ѧϰ�������client���ؽ��
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
		// ��ȡ����ID
		//************************************
		SkillID_t		GetSkillID() const;
		//************************************
		// Method:    SetSkillID
		// FullName:  Packets::GCSkillUpgradeBySouXia::SetSkillID
		// Access:    public 
		// Returns:   VOID
		// Qualifier:
		// Parameter: INT val
		// ���ü���ID
		//************************************
		VOID			SetSkillID(SkillID_t val);
		VOID			SetResult(BYTE Result){ m_nResult = Result; }
		BYTE			GetResult(){return m_nResult;}
		
	private:
		SHORT			m_nCurSouXiaPos;//��ǰװ���Ē���¼��λ��
		BYTE			m_nStudySkillType;//��ǰ�򿪒���¼��ҳ����
		//����ID
		SkillID_t		m_nSkillID;
		SHORT			m_nCurUseTime;	// ��ǰ�����õĴ���
		
		BYTE			m_nResult;		//�Ƿ�ɹ������ɹ�����������Ϣ
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

