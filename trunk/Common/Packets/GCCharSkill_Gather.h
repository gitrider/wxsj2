// GCCharSkill_Gather.h
// 
// ���ܾ���
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARSKILL_GATHER_H__
#define __GCCHARSKILL_GATHER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCCharSkill_Gather : public Packet
	{
	public:
		GCCharSkill_Gather( ){}
		virtual ~GCCharSkill_Gather( ){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARSKILL_GATHER; }
		virtual UINT			GetPacketSize()const
		{
			return sizeof(m_ObjID)
				+ sizeof(m_nLogicCount)
				+ sizeof(m_SkillDataID)
				+ sizeof(m_posUser)
				+ sizeof(m_TargetID)
				+ sizeof(m_posTarget)
				+ sizeof(m_fDir)
				+ sizeof(m_nTotalTime);
		}

	public:
		//ʹ�����ݽӿ�
		VOID				setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t				getObjID(VOID)const { return m_ObjID; }

		VOID				setLogicCount(INT nCount) { m_nLogicCount = nCount; }
		INT					getLogicCount(VOID)const { return m_nLogicCount; }

		VOID				setSkillDataID(SkillID_t ID) { m_SkillDataID = ID; }
		SkillID_t			getSkillDataID(VOID)const { return m_SkillDataID; }

		VOID				setUserPos(const WORLD_POS& pos) { m_posUser = pos; }
		const WORLD_POS&	getUserPos(VOID)const { return m_posUser; }

		VOID				setTargetID(ObjID_t ID) { m_TargetID = ID; }
		ObjID_t				getTargetID(VOID)const { return m_TargetID; }

		VOID				setTargetPos(const WORLD_POS& pos) { m_posTarget = pos; }
		const WORLD_POS&	getTargetPos(VOID)const { return m_posTarget; }

		VOID				setDir(FLOAT fDir) { m_fDir = fDir; }
		FLOAT				getDir(VOID)const { return m_fDir; }

		VOID				setTotalTime( INT nTotalTime ) { m_nTotalTime = nTotalTime; }
		INT					getTotalTime( VOID )const { return m_nTotalTime; }

	private:
		ObjID_t				m_ObjID;			// ObjID
		INT					m_nLogicCount;		// �߼�����
		SkillID_t			m_SkillDataID;		// ���ܵ���ԴID
		WORLD_POS			m_posUser;			// ʹ�ö�����
		//union{
		ObjID_t				m_TargetID;			// Ŀ���ɫ
		WORLD_POS			m_posTarget;		// Ŀ������
		FLOAT				m_fDir;				// ���ܵķ���
		//};
		INT					m_nTotalTime;		// ��ʱ��
	};


	class GCCharSkill_GatherFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharSkill_Gather() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARSKILL_GATHER; }
		UINT		GetPacketMaxSize()const { return sizeof(GCCharSkill_Gather) - sizeof(Packet); }
	};

	class GCCharSkill_GatherHandler 
	{
	public:
		static UINT Execute( GCCharSkill_Gather* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCCHARSKILL_GATHER_H__
