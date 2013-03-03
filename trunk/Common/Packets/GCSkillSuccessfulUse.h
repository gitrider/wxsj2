/********************************************************************
	created:	2008/01/24
	created:	24:1:2008   17:50
	filename: 	d:\project\project_11_30\Common\Packets\GCSkillSuccessfulUse.h
	file path:	d:\project\project_11_30\Common\Packets
	file base:	GCSkillSuccessfulUse
	file ext:	h
	author:		Richard
	
	purpose:	
*********************************************************************/

#ifndef GCSkillSuccessfulUse_h__
#define GCSkillSuccessfulUse_h__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameUtil.h"

namespace Packets
{
	class GCSkillSuccessfulUse : public Packet
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
	
		GCSkillSuccessfulUse();
		virtual ~GCSkillSuccessfulUse();
		VOID Cleanup();

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SKILL_SUCCESSFUL_USE; }
		virtual UINT			GetPacketSize()const;

	public: //GCTargetListAndHitFlags
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
		UINT				GetBallIDByIndex( INT nIdx );
		VOID				SetBallIDByIndex( INT nIdx, UINT uBallID );
		ObjID_t				GetTargetObjIDByIndex(INT nIdx) const;
		VOID				SetTargetObjIDByIndex(INT nIdx, ObjID_t nObjID);

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

	public:	//GCCharSkill_Send
		//ʹ�����ݽӿ�

		VOID				setSkillDataID(SkillID_t ID) { m_SkillDataID = ID; }
		SkillID_t			getSkillDataID(VOID)const { return m_SkillDataID; }
		VOID				setTotalTime( INT nTotalTime ) { m_nTotalTime = nTotalTime; }
		INT					getTotalTime( VOID )const { return m_nTotalTime; }
	private:
		INT					m_nTotalTime;
		SkillID_t			m_SkillDataID;							// ���ܵ���ԴID
	public://GCCharDirectImpact
		//ʹ�����ݽӿ�
		ImpactID_t		GetImpactID(VOID) const {return m_nImpactID;}
		VOID			SetImpactID(ImpactID_t nID) {m_nImpactID=nID;}

		SkillID_t		GetSkillID(VOID) const {return m_nSkillID;}
		VOID			SetSkillID(SkillID_t nID) {m_nSkillID=nID;}

	private:

		ImpactID_t		m_nImpactID;		// Ч��ID //�ο�GameStruct_Impact.h��DIRECT_IMPACT_SEID
		SkillID_t		m_nSkillID;			// ���ܵ�ID

public://GCDetailHealsAndDamages
	enum
	{
		IDX_HP_MODIFICATION = 0,
		IDX_MP_MODIFICATION,
		IDX_PNEUMA_MODIFICATION,
		IDX_RAGE_MODIFICATION,
		IDX_STRIKE_POINT_MODIFICATION,
		NUM_OF_FLAGS,
	};

	typedef BitFlagSet_T<NUM_OF_FLAGS> DirtyFlagSet_T;
	VOID			SetHpModification(INT nValue) {m_nHpModification = nValue;m_DirtyFlags.MarkFlagByIndex(IDX_HP_MODIFICATION);}
	INT				GetHpModification(VOID) const {return m_nHpModification;}

	VOID			SetMpModification(INT nValue) {m_nMpModification = nValue;m_DirtyFlags.MarkFlagByIndex(IDX_MP_MODIFICATION);}
	INT 			GetMpModification(VOID) const {return m_nMpModification;}

	VOID			SetPneumaModification(INT nValue) {m_nPneumaModification = nValue;m_DirtyFlags.MarkFlagByIndex(IDX_PNEUMA_MODIFICATION);}
	INT 			GetPneumaModification(VOID) const {return m_nPneumaModification;}

	VOID			SetRageModification(INT nValue) {m_nRageModification = nValue;m_DirtyFlags.MarkFlagByIndex(IDX_RAGE_MODIFICATION);}
	INT 			GetRageModification(VOID) const {return m_nRageModification;}

	VOID			SetStrikePointModification(INT nValue) {m_nStrikePointModification = nValue;m_DirtyFlags.MarkFlagByIndex(IDX_STRIKE_POINT_MODIFICATION);}
	INT 			GetStrikePointModification(VOID) const {return m_nStrikePointModification;}

	VOID			SetCriticalHitFlag(BOOL bFlag) {m_bIsCriticalHit = bFlag;}
	BOOL 			IsCriticalHit(VOID) const {return m_bIsCriticalHit;}

	BOOL			IsHpModificationDirty(VOID) const {return m_DirtyFlags.GetFlagByIndex(IDX_HP_MODIFICATION);};
	BOOL			IsMpModificationDirty(VOID) const {return m_DirtyFlags.GetFlagByIndex(IDX_MP_MODIFICATION);};
	BOOL			IsPneumaModificationDirty(VOID) const {return m_DirtyFlags.GetFlagByIndex(IDX_PNEUMA_MODIFICATION);};
	BOOL			IsRageModificationDirty(VOID) const {return m_DirtyFlags.GetFlagByIndex(IDX_RAGE_MODIFICATION);};
	BOOL			IsStrikePointModificationDirty(VOID) const {return m_DirtyFlags.GetFlagByIndex(IDX_STRIKE_POINT_MODIFICATION);};
	VOID			SetBeatBack( BOOL bBeatBack ) { m_bBeatBack = bBeatBack; }
	BOOL			IsBeatBack() const { return m_bBeatBack; }
	VOID			SetBeatFly( BOOL bBeatFly ) { m_bBeatFly = bBeatFly; }
	BOOL			IsBeatFly() const { return m_bBeatFly; }
	const WORLD_POS& GetBeatBackPos() { return m_BeatBackPos; }
	VOID			 SetBeatBackPos( const WORLD_POS& sPos ) { m_BeatBackPos = sPos; }
	const WORLD_POS& GetCloseToPos() { return m_CloseToPos; }
	VOID			 SetCloseToPos( const WORLD_POS& sPos ) { m_CloseToPos = sPos; }
	FLOAT			 GetBeatFlyDistance() { return m_fBeatFlyDistance; }
	VOID			 SetBeatFlyDistance( FLOAT fDistance ) { m_fBeatFlyDistance = fDistance; }
	FLOAT			 GetAttackerAngle() { return m_fAttackerAngle; }
	VOID			 SetAttackerAngle( FLOAT fAngle ) { m_fAttackerAngle = fAngle; }
	VOID			 SetCloseToEnable( BOOL bEnable ) { m_bCloseTo = bEnable; }
	BOOL			 IsCloseTo() const { return m_bCloseTo; }
private:
		INT				m_nSenderLogicCount;		//Ч�������ߵ��߼�����
		INT				m_nHpModification;			//�����仯��
		INT				m_nMpModification;			//�����仯��
		INT				m_nPneumaModification;		//Ԫ���仯��
		INT 			m_nRageModification;		//ŭ���仯��
		INT				m_nStrikePointModification; //������仯��
		BOOL			m_bIsCriticalHit;			//�Ƿ��ǻ���һ��
		DirtyFlagSet_T 	m_DirtyFlags;				//����Щ�������ݵ�����
		BOOL			m_bBeatBack;				//�Ƿ񱻻���
		BOOL			m_bBeatFly;					//�Ƿ񱻻���
		WORLD_POS		m_BeatBackPos;				//�����˵���λ��
		WORLD_POS		m_CloseToPos;				//�����߸����������ߵ����λ��
		FLOAT			m_fBeatFlyDistance;			//���ɾ���
		FLOAT			m_fAttackerAngle;			//����ʱ�����ߵĳ���
		BOOL			m_bCloseTo;


	};


	class GCSkillSuccessfulUseFactory : public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCSkillSuccessfulUse() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_SKILL_SUCCESSFUL_USE; }
		UINT		GetPacketMaxSize()const
		{
			return sizeof(GCSkillSuccessfulUse) - sizeof(Packet);
		}
	};

	class GCSkillSuccessfulUseHandler 
	{
	public:
		static UINT Execute( GCSkillSuccessfulUse* pPacket, Player* pPlayer );
	};
}

#endif // GCSkillSuccessfulUse_h__
