// GCTargetListAndHitFlags.h
// 
// ���ܷ���
// 
//////////////////////////////////////////////////////

#ifndef __GCTARGETLISTANDHITFLAG_H__
#define __GCTARGETLISTANDHITFLAG_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameUtil.h"

namespace Packets
{
	class GCTargetListAndHitFlags : public Packet
	{
	public:
		enum 
		{
			UNIT_USE_SKILL = 0,
			SPECIAL_OBJ_ACTIVATE,
			BUFF_ACTIVETE
		};
		enum
		{
			MAX_TARGET_LIST_SIZE = 64,
		};
		struct TargetListAndBallID
		{
			ObjID_t m_nTargetID;
			UINT	m_uBallUniqueID;
			TargetListAndBallID()
			{
				memset( this, 0, sizeof(TargetListAndBallID) );
			}
		};
		typedef BitFlagSet_T<MAX_TARGET_LIST_SIZE> HitFlagList_T;
	
		GCTargetListAndHitFlags(){}
		virtual ~GCTargetListAndHitFlags(){}

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_TARGET_LIST_AND_HIT_FLAGS; }
		virtual UINT			GetPacketSize()const
		{
			UINT nSize = sizeof(m_nDataType) +
				sizeof(m_nObjID) +
				sizeof(m_nLogicCount) +
				sizeof(m_nSkillOrSpecialObjDataID) +
				sizeof(m_posUser) +
				sizeof(m_nTargetID) +
				sizeof(m_posTarget) +
				sizeof(m_fDir) +
				sizeof(m_HitFlagList) +
				sizeof(m_nTargetNum);

			nSize += sizeof(m_aTargetList[0])*m_nTargetNum;

			return nSize;
		}

	public:
		//ʹ�����ݽӿ�
		VOID				SetDataType(BYTE nType) { m_nDataType= nType; }
		BYTE				GetDataType(VOID)const { return m_nDataType; }
		VOID				SetObjID(ObjID_t id) { m_nObjID = id; }
		ObjID_t				GetObjID(VOID)const { return m_nObjID; }
		VOID				SetLogicCount(INT nCount) { m_nLogicCount = nCount; }
		INT					GetLogicCount(VOID)const { return m_nLogicCount; }
		VOID				SetSkillOrSpecialObjDataID(SkillID_t ID) { m_nSkillOrSpecialObjDataID = ID; }
		SkillID_t			GetSkillOrSpecialObjDataID(VOID)const { return m_nSkillOrSpecialObjDataID; }
		VOID				SetUserPos(const WORLD_POS& pos) { m_posUser = pos; }
		const WORLD_POS&	GetUserPos(VOID)const { return m_posUser; }
		VOID				SetTargetID(ObjID_t ID) { m_nTargetID = ID; }
		ObjID_t				GetTargetID(VOID)const { return m_nTargetID; }
		VOID				SetTargetPos(const WORLD_POS& pos) { m_posTarget = pos; }
		const WORLD_POS&	GetTargetPos(VOID)const { return m_posTarget; }
		VOID				SetDir(FLOAT fDir) { m_fDir = fDir; }
		FLOAT				GetDir(VOID)const { return m_fDir; }
		HitFlagList_T& 		GetHitFlagList(VOID) {return m_HitFlagList;}
		BYTE				GetTargetNum(VOID) const {return m_nTargetNum;}
		VOID				SetTargetNum(BYTE nNum) {m_nTargetNum = nNum;}

		UINT GetBallIDByIndex( INT nIdx )
		{
			if(0>nIdx || MAX_TARGET_LIST_SIZE<=nIdx)
			{
				return INVALID_ID;
			}

			return m_aTargetList[nIdx].m_uBallUniqueID;
		}

		VOID SetBallIDByIndex( INT nIdx, UINT uBallID )
		{
			if(0<=nIdx && MAX_TARGET_LIST_SIZE>nIdx)
			{
				m_aTargetList[nIdx].m_uBallUniqueID = uBallID;
			}
		}

		ObjID_t	GetTargetObjIDByIndex(INT nIdx) const
		{
			if(0>nIdx || MAX_TARGET_LIST_SIZE<=nIdx)
			{
				return INVALID_ID;
			}
			return m_aTargetList[nIdx].m_nTargetID;
		}

		VOID SetTargetObjIDByIndex(INT nIdx, ObjID_t nObjID)
		{
			if(0<=nIdx && MAX_TARGET_LIST_SIZE>nIdx)
			{
				m_aTargetList[nIdx].m_nTargetID = nObjID;
			}
		}
	private:
		BYTE				m_nDataType;							// �ǽ�ɫʹ�ü��ܻ�������֮����������ը�򼤻�
		ObjID_t				m_nObjID;								// ObjID�� ����ʹ���߻����ڼ���Ķ���
		INT					m_nLogicCount;							// �߼������� ����ʹ���߻����ڼ���Ķ�����߼�����
		SkillID_t			m_nSkillOrSpecialObjDataID;				// ���ܻ�����������ԴID
		WORLD_POS			m_posUser;								// ʹ������������ڼ���Ķ��������
		ObjID_t				m_nTargetID;							// Ŀ���ɫ����Ҫ��ʹ������Ҫ�����Ŀ��
		WORLD_POS			m_posTarget;							// Ŀ�����꣬��Ҫ��ʹ������Ҫ�����λ��
		FLOAT				m_fDir;									// ���ܵķ�����Ҫ��ʹ������Ҫ����ķ���
		HitFlagList_T		m_HitFlagList;							// Ŀ�걻�������ı���б�һ����Ƕ�Ӧ�����Ŀ���б��е�һ��Ŀ�������������Ӧ��
		BYTE				m_nTargetNum;							// Ӱ���Ŀ����Ŀ
		TargetListAndBallID	m_aTargetList[MAX_TARGET_LIST_SIZE];	// Ӱ���Ŀ��ID�б�
	};


	class GCTargetListAndHitFlagsFactory : public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCTargetListAndHitFlags() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_TARGET_LIST_AND_HIT_FLAGS; }
		UINT		GetPacketMaxSize()const
		{
			return sizeof(BYTE) +
				sizeof(ObjID_t) +
				sizeof(INT) +
				sizeof(SkillID_t) +
				sizeof(WORLD_POS) +
				sizeof(ObjID_t) +
				sizeof(WORLD_POS) +
				sizeof(FLOAT) +
				sizeof(GCTargetListAndHitFlags::HitFlagList_T) +
				sizeof(ObjID_List);
		}
	};

	class GCTargetListAndHitFlagsHandler 
	{
	public:
		static UINT Execute( GCTargetListAndHitFlags* pPacket, Player* pPlayer ) ;
	};
}

#endif
