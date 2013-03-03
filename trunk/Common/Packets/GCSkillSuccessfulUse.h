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

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_SKILL_SUCCESSFUL_USE; }
		virtual UINT			GetPacketSize()const;

	public: //GCTargetListAndHitFlags
		//使用数据接口
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
		BYTE				m_nDataType;							// 是角色使用技能还是陷阱之类的特殊对象爆炸或激活
		ObjID_t				m_nObjID;								// ObjID， 技能使用者或正在激活的对象
		INT					m_nLogicCount;							// 逻辑计数， 技能使用者或正在激活的对象的逻辑计数
		SkillID_t			m_nSkillOrSpecialObjDataID;				// 技能或特殊对象的资源ID
		WORLD_POS			m_posUser;								// 使用者坐标或正在激活的对象的坐标
		ObjID_t				m_nTargetID;							// 目标角色，主要是使用者需要面向的目标
		WORLD_POS			m_posTarget;							// 目标坐标，主要是使用者需要面向的位置
		FLOAT				m_fDir;									// 技能的方向，主要是使用者需要面向的方向
		HitFlagList_T		m_HitFlagList;							// 目标被击中与否的标记列表，一个标记对应下面的目标列表中的一个目标对象，用索引对应。
		BYTE				m_nTargetNum;							// 影响的目标数目
		TargetListAndBallID	m_aTargetList[MAX_TARGET_LIST_SIZE];	// 影响的目标ID列表

	public:	//GCCharSkill_Send
		//使用数据接口

		VOID				setSkillDataID(SkillID_t ID) { m_SkillDataID = ID; }
		SkillID_t			getSkillDataID(VOID)const { return m_SkillDataID; }
		VOID				setTotalTime( INT nTotalTime ) { m_nTotalTime = nTotalTime; }
		INT					getTotalTime( VOID )const { return m_nTotalTime; }
	private:
		INT					m_nTotalTime;
		SkillID_t			m_SkillDataID;							// 技能的资源ID
	public://GCCharDirectImpact
		//使用数据接口
		ImpactID_t		GetImpactID(VOID) const {return m_nImpactID;}
		VOID			SetImpactID(ImpactID_t nID) {m_nImpactID=nID;}

		SkillID_t		GetSkillID(VOID) const {return m_nSkillID;}
		VOID			SetSkillID(SkillID_t nID) {m_nSkillID=nID;}

	private:

		ImpactID_t		m_nImpactID;		// 效果ID //参考GameStruct_Impact.h的DIRECT_IMPACT_SEID
		SkillID_t		m_nSkillID;			// 技能的ID

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
		INT				m_nSenderLogicCount;		//效果创建者的逻辑计数
		INT				m_nHpModification;			//生命变化量
		INT				m_nMpModification;			//内力变化量
		INT				m_nPneumaModification;		//元气变化量
		INT 			m_nRageModification;		//怒气变化量
		INT				m_nStrikePointModification; //连击点变化量
		BOOL			m_bIsCriticalHit;			//是否是会心一击
		DirtyFlagSet_T 	m_DirtyFlags;				//有哪些域有数据的脏标记
		BOOL			m_bBeatBack;				//是否被击退
		BOOL			m_bBeatFly;					//是否被击飞
		WORLD_POS		m_BeatBackPos;				//被击退到的位置
		WORLD_POS		m_CloseToPos;				//击打者跟进被击退者到达的位置
		FLOAT			m_fBeatFlyDistance;			//击飞距离
		FLOAT			m_fAttackerAngle;			//跟进时击打者的朝向
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
