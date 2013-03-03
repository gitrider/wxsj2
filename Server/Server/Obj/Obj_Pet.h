// Obj_Pet.h
//
// 功能：宠物在SERVER的实现
// 
///////////////////////////////////////////////////////

#ifndef __PET_H__
#define __PET_H__

#include "Obj_Character.h"
#include "GameStruct_Pet.h"
//////////////////////////////////////////////////////
// 初始化Obj_Pet的结构
//////////////////////////////////////////////////////
struct _OBJ_PET_INIT :
	public _OBJ_CHARACTER_INIT
{
	GUID_t			m_GUID;			// 角色在场景中的GUID，与PetGUID是两回事
	_PET_DB			m_PetDB;
	ObjID_t			m_CreatorID;	// 所有者的ID，即宠物的主人
	INT				m_nIndex;		// 在主人宠物槽中的索引
	INT				m_RespawnTime;

	_OBJ_PET_INIT( VOID )
	{
		m_GUID			= INVALID_GUID;
		m_PetDB.CleanUp();
		m_CreatorID		= INVALID_ID;
		m_nIndex		= -1;
		m_RespawnTime	= -1;
	}

	virtual VOID CleanUp( VOID )
	{
		m_GUID			= INVALID_GUID;
		m_PetDB.CleanUp();
		m_CreatorID		= INVALID_ID;
		m_nIndex		= -1;
		m_RespawnTime	= -1;

		_OBJ_CHARACTER_INIT::CleanUp();
	}
};

//////////////////////////////////////////////////////
// 属性刷新的上一次属性的保存结构
//////////////////////////////////////////////////////
struct _PET_ATTR_BACKUP
{
public:
	BOOL		m_bNameModified;
	BOOL		m_bTitleModified;

	FLOAT       m_MoveSpeed;
	_CAMP_DATA	m_CampData;						//阵营		
	ObjID_t		m_nOwnerID;
	PET_GUID_t	m_SpouseGUID;					// 配偶GUID
	INT			m_nAIType;						// 性格
	INT			m_Level;						// 等级
	INT			m_iSex;							// 性别
	INT			m_HP;
	INT			m_MaxHp;						
	INT			m_nDataID;						// 数据ID
	INT			m_nModelID;						// 外形
	INT			m_nMountID;						// 座骑ID

	PET_GUID_t	m_GUID;							// ID
	BYTE		m_byGeneration;					// 几代宠
	BYTE		m_byHappiness;					// 快乐度

	INT			m_nExp;							// 经验
	//资质
	INT			m_iStrengthPerception;			// 力量资质
	INT			m_iSmartnessPerception;			// 敏捷资质
	INT 		m_iMindPerception;				// 智力资质
	INT 		m_iConstitutionPerception;		// 体质资质
	//养成属性
	INT			m_iSavvy;						// 悟性
	INT			m_GenGu;						// 根骨
	INT			m_iPullulationRate;				// 成长率

	INT			m_nRemainPoint;					// 一级属性剩余点数

	//一级战斗属性
	INT			m_iStrength;					// 力量
	INT			m_iSmartness;					// 敏捷
	INT 		m_iMind;						// 智力
	INT 		m_iConstitution;				// 体质

	//二级战斗属性
	INT			m_nAtt_Near;           
	INT			m_nAtt_Far;            

	INT			m_nAtt_MagicNear;		
	INT			m_nAtt_MagicFar;		

	INT			m_nDef_Near;           
	INT			m_nDef_Far;            

	INT			m_nDef_MagicNear;
	INT			m_nDef_MagicFar;

	INT			m_nHit;				
	INT			m_nMiss;
	INT			m_nCritical;

public:
	_PET_ATTR_BACKUP( VOID );

	VOID CleanUp( VOID );
};



//////////////////////////////////////////////////////
// 宠物
//////////////////////////////////////////////////////

#define MAX_CAPTURER_NUM (6)
#define LEVELDELTA_GETEXP (5)
#define SEND_LIFE_INTERVAL (30)

class AI_Pet;
class Obj_Pet :
	public Obj_Character
{
public:
	friend class AI_Pet;

//===========================================
// Obj公有接口继承
//===========================================
public:
	Obj_Pet( VOID );
	virtual ~Obj_Pet( VOID );

	virtual ObjType		GetObjType( VOID )const{ return OBJ_TYPE_PET; }
	virtual VOID 		CleanUp( );
	virtual BOOL 		Init( const _OBJ_INIT *pInit );

	virtual BOOL 		HeartBeat( UINT uTime = 0 );

	virtual VOID		InitAIObj( );

	const _PET_DB*		GetPetDB(VOID) const { return &m_PetDB; }
	//根据召唤兽当前情况初始化DB,在跨服时使用
	VOID				InitMonsterPetDB( _Monster_Pet_DB_Struct& oMonsterPet );

	//注意只有存活的对象，才有初始化技能的需要
	BOOL				InitSkill();

protected:
	VOID InitLife();
	
	//获得剩余生命时间
	INT GetLifeTime();
	VOID GetNowImpactList( _Monster_Pet_DB_Struct &oMonsterPet );
//===========================================
// 阵营相关
//===========================================
public:
	virtual BOOL	IsEnemy(Obj_Character* pCharacter);
	virtual BOOL	IsFriend(Obj_Character* pCharacter);
	virtual BOOL	IsPartner(Obj_Character* pCharacter);

//===========================================
// 消息/属性刷新
//===========================================
public:
	virtual Packet		*CreateNewObjPacket( VOID );
	virtual VOID		DestroyNewObjPacket( Packet *pPacket );

	virtual VOID 		InitBackupAttr( );
	virtual VOID 		SendMsg_RefeshAttrib( VOID );
	virtual VOID 		AskMsg_BaseAttrib( Obj_Human *pTargetHuman );
	virtual VOID 		AskMsg_DetailAttrib( Obj_Human *pTargetHuman );
protected:
	//为技能同步，实时发送CharBase消息，并清除HP脏标记，组织自动刷属性时重复发送
	virtual VOID SendCharBaseDirectForHP();

protected:
	_PET_ATTR_BACKUP	m_AttrBackUp;

//===========================================
// 快乐度变化规则
//===========================================
public:
	virtual BOOL		HeartBeat_Happiness( UINT uTime = 0 );

protected:
	CMyTimer			m_HappinessTimer;		// 快乐度每隔多少时间减1

//===========================================
// 寿命变化规则
//===========================================
public:
	virtual BOOL		HeartBeat_Life( UINT uTime = 0 );


protected:
	CMyTimer			m_LifeTimer;		// 快乐度每隔多少时间减1
	UINT				m_BeginTime;		// 生命时间的起始时间
//===========================================
// 其他规则
//===========================================
public:
	VOID				LevelUp(VOID);
	virtual VOID		SetReliveInfo( BOOL bSkillRelive, const _RELIVE_INFO *pReliveInfo );

//===========================================
// 技能/心法
//===========================================
public:
	virtual BOOL		Skill_HaveSkill( SkillID_t const nID ) const;

	// nIndex	:	ENUM_PET_SKILL_INDEX
	const _PET_SKILL	*Skill_GetSkill( INT nIndex ) const;

	INT 				Skill_GetCount_CortrolByPlayer(VOID);
	INT 				Skill_GetMaxCount_CortrolByPlayer(VOID);
	INT 				Skill_GetCount_CortrolByAI(VOID);
	INT 				Skill_GetMaxCount_CortrolByAI(VOID);

	BOOL 				Skill_RegisterSkill(SkillID_t const nID, INT nIndex);
	BOOL 				Skill_UnregisterSkill(SkillID_t const nID, INT nIndex);
	BOOL 				Skill_ModifySkill_Study(SkillID_t nID);
	BOOL 				Skill_ModifySkill_Realize(SkillID_t nID);
	BOOL 				Skill_ModifySkill_MenPai(SkillID_t nID);
	BOOL 				Skill_Apperceive(VOID);

	virtual INT			Skill_GetXinFaLevel(SkillID_t const nID) const;
	virtual BOOL 		Skill_HaveXinFa( SkillID_t const nID ) const;
	virtual BOOL 		Skill_SetXinFaLevel(SkillID_t const nID, INT const nLevel);
	virtual VOID 		Skill_OnEquipItem(VOID);
//===========================================
// 效果
//===========================================
protected:
	virtual _IMPACT_LIST&	Impact_GetImpactList(VOID);

private:
	virtual VOID 			Impact_InitList(VOID);

private:
	_IMPACT_LIST			m_ImpactList;

//===========================================
// CoolDown
//===========================================
public:
	virtual Time_t		GetCooldownRemained(CooldownID_t const nID) const;
	virtual BOOL		IsCooldowned(CooldownID_t const nID) const;
	virtual VOID 		SetCooldown(CooldownID_t const nID, Time_t nCooldown);
	virtual BOOL 		HeartBeat_Cooldown(UINT uTime = 0);
	virtual BOOL 		HeartBeat_Recover(UINT uTime = 0);

protected:
	VOID RecallPet();
//===========================================
// Capturer
//===========================================
public:
	INT					GetCaptureCount(VOID)const;

	BOOL 				AddCapturer(ObjID_t idObj); 
	BOOL 				DelCapturer(ObjID_t idObj);
	VOID 				CleanAllCapturer(VOID);
	VOID 				SendCaptureFailedToOthers(ObjID_t idObj);

protected:
	INT					m_nCapturerCount;	// 当前正在捕获自己的玩家数
	ObjID_t				m_aCapturer[MAX_CAPTURER_NUM];

//===========================================
// AI
//===========================================
public:
	virtual ObjID_t			GetOwnerID(VOID)const;
	virtual Obj_Character	*GetOwner( VOID );

	AI_Pet*					GetPetAI( VOID )const;
	VOID					SetTargetID( ObjID_t TargetID );
	ObjID_t					GetTargetID( VOID )const;
	const WORLD_POS*		GetRespawnPos( VOID )const;

	VOID					ToAttack( ObjID_t TargetID );
	BOOL					PushSkillToCache(INT idSkill);
	VOID					GetRateOfBuffTakeEffect(INT idSkill, INT& rateOfBuffTakeEffect);

protected:
	ObjID_t					m_OwnerID;			// 当前所有者的ObjID
	ObjID_t					m_TargetID;			// 目标ID
	WORLD_POS				m_RespawnPos;
	INT						m_RespawnTime;
	// 只用来缓存被动技能和护主型技能(即起持续性效果的技能)
	INT						m_aCacheOfSkill[MONSTER_PET_MAX_SKILL];

	INT						m_AIType;
	INT						m_iLevel;
	INT						m_iDataID;
	PET_GUID_t				m_guidPet;
	INT						m_iHP;
	INT						m_iMP;

//===========================================
// 一级属性
//===========================================
public:
	virtual GUID_t			GetGUID( VOID )const;

	Obj_Human				*GetCreator( VOID )const;
	const Obj_Human			*GetConstCreator( VOID )const;

	VOID					SetPetGUID(PET_GUID_t guid);
	PET_GUID_t				GetPetGUID(VOID)const;

	VOID					SetDataID(const UINT dataID);
	virtual UINT			GetDataID( VOID )const; 

	virtual	VOID			SetName( const CHAR *pszName );
	virtual const CHAR		*GetName( VOID )const;

	virtual	VOID			SetTitle( const CHAR *pszTitle );
	virtual const CHAR		*GetTitle( VOID )const;

	VOID					IncrementExp(INT nIncrement);
	virtual INT				GetExp( VOID )const{ return 0; };
	virtual VOID			SetExp( INT nExp ){};

	virtual VOID			SetHP( INT nHP );
	virtual INT				GetHP( VOID );

	virtual VOID			SetMP( INT nMP ){};
	virtual INT				GetMP( VOID ){return 0;};

	virtual INT 			GetRage( VOID ){ return 0; }
	virtual VOID			SetRage( INT nRage ){};

	virtual INT				GetAIType( VOID )const;
	virtual VOID			SetAIType( INT nAIType );

	virtual INT				GetLevel( VOID )const;
	virtual VOID			SetLevel( INT nLevel );

	//virtual INT				GetLife( VOID )const;
	//virtual VOID			SetLife( INT nLife );

	virtual BYTE			GetGeneration( VOID )const;
	virtual VOID			SetGeneration( BYTE byGeneration );

	VOID 					IncrementHappyness(INT nIncrement);
	BYTE 					GetHappiness( VOID )const;
	VOID 					SetHappiness( BYTE byHappiness );

	INT						GetGenGu( VOID )const;
	VOID					SetGenGu( INT nGenGu );

	virtual INT 			GetStrPerception( VOID )const;
	virtual INT 			GetSmartnessPerception( VOID )const;
	virtual INT 			GetMindPerception( VOID )const;
	//virtual INT 			GetSprPerception( VOID )const;
	virtual INT 			GetConstitutionPerception( VOID )const;

	virtual INT				GetRemainPoint( VOID )const;
	virtual VOID			SetRemainPoint( INT nRemainPoint );
	INT						GetIndexInOnwer();
protected:
	GUID_t					m_GUID;				// 角色在场景中的GUID，与PetGUID是两回事
	ObjID_t					m_CreatorID;		// 创建者的ObjID
	Obj_Human*				m_pCreator;			// 宠物的创建者
	INT						m_nIndex;			// 标识该宠物是玩家宠物槽中的第几个
	_PET_DB					m_PetDB;
	_MONSTER_PET_ATTR		m_oInit;

//===========================================
// 二级属性
//===========================================
public:
	//StrikePoint
	virtual INT 				GetMaxStrikePoint(VOID);
	//Rage
	virtual INT 				GetMaxRage(VOID);
	virtual INT 				GetBaseMaxRage(VOID);
	virtual VOID 				SetBaseMaxRage(INT const nRage);
	virtual INT					GetRageRegenerate(VOID); 
	virtual INT 				GetBaseRageRegenerate(VOID);
	virtual VOID 				SetBaseRageRegenerate(INT const nValue);
	

	//Attr1: Str
	virtual INT					GetStr(VOID);
	virtual INT					GetBaseStr(VOID);
	virtual VOID 				SetBaseStr(INT const nValue);
	////Attr1: Spr
	//virtual INT					GetSpr(VOID);
	//virtual INT					GetBaseSpr(VOID);
	//virtual VOID				SetBaseSpr(INT const nValue);
	//Attr1: Con
	virtual INT					GetCon(VOID);
	virtual INT					GetBaseCon(VOID);
	virtual VOID				SetBaseCon(INT const nValue);
	//Attr1: Int
	virtual INT					GetInt(VOID);
	virtual INT					GetBaseInt(VOID);
	virtual VOID				SetBaseInt(INT const nValue);
	//Attr1: Dex
	virtual INT					GetDex(VOID);
	virtual INT					GetBaseDex(VOID);
	virtual VOID				SetBaseDex(INT const nValue);
	//HP
	virtual INT					GetMaxHP(VOID) ; 
	virtual INT 				GetBaseMaxHP(VOID);
	virtual VOID 				SetBaseMaxHP(INT const nHp);
	virtual INT					GetHPRegenerate(VOID) ; 
	virtual INT 				GetBaseHPRegenerate(VOID);
	virtual VOID 				SetBaseHPRegenerate(INT const nValue);

	//MP
	virtual INT 				GetMaxMP(VOID);
	virtual INT 				GetBaseMaxMP(VOID);
	virtual VOID 				SetBaseMaxMp(INT const nMp);
	virtual INT					GetMPRegenerate(VOID); 
	virtual INT 				GetBaseMPRegenerate(VOID);
	virtual VOID 				SetBaseMPRegenerate(INT const nValue);

	virtual INT 				GetMaxPneuma(VOID) {Assert(FALSE);return 0;}
	virtual INT 				GetBaseMaxPneuma(VOID){Assert(FALSE);return 0;}
	virtual VOID 				SetBaseMaxPneuma(INT const nPneuma){Assert(FALSE);return;}
	virtual INT					GetPneumaRegenerate(VOID){Assert(FALSE);return 0;}
	virtual INT 				GetBasePneumaRegenerate(VOID){Assert(FALSE);return 0;}
	virtual VOID 				SetBasePneumaRegenerate(INT const nValue){Assert(FALSE);return;}
	//CampID
	virtual const _CAMP_DATA	*GetCampData(VOID);
	virtual const _CAMP_DATA	*GetBaseCampData(VOID)const;
	virtual VOID				SetBaseCampData(const _CAMP_DATA *pCampData);
	virtual INT 				GetCampID(VOID);
	virtual INT 				GetBaseCampID(VOID) const;
	virtual VOID				SetBaseCampID(INT const nID);
	//modelID
	virtual INT 				GetModelID(VOID) ;
	virtual INT 				GetBaseModelID(VOID) const;
	virtual VOID				SetBaseModelID(INT const nID);
	//Alive flag
	//In Combat flag
	// Can move flag
	// Can Action flag
	// Unbreakable flag
	// Attr2: Move Speed
	virtual FLOAT				GetMoveSpeed(VOID);
	virtual INT					GetBaseMoveSpeed(VOID);
	virtual VOID				SetBaseMoveSpeed(INT const nValue);
	// Attr2: Attack Speed
	virtual INT 				GetAttackSpeed(VOID);
	virtual INT 				GetBaseAttackSpeed(VOID);
	virtual VOID				SetBaseAttackSpeed(INT const nValue);
	// Attr2: Miss
	virtual INT 				GetMiss(VOID);
	virtual INT 				GetBaseMiss(VOID);
	virtual VOID				SetBaseMiss(INT const nValue);
	// Attr2 Hit
	virtual INT 				GetHit(VOID);
	virtual INT 				GetBaseHit(VOID);
	virtual VOID				SetBaseHit(INT const nValue);
	// Attr2 Critical
	virtual INT 				GetCritical(VOID);
	virtual INT 				GetBaseCritical(VOID);
	virtual VOID				SetBaseCritical(INT const nValue);
	// Attr2: Attack and Defence power physics
	/*virtual INT 				GetAttackPhysics(VOID);
	virtual INT 				GetBaseAttackPhysics(VOID);
	virtual VOID				SetBaseAttackPhysics(INT const nValue);
	virtual INT 				GetDefencePhysics(VOID);
	virtual INT 				GetBaseDefencePhysics(VOID);
	virtual VOID				SetBaseDefencePhysics(INT const nValue);*/
	
	//add by liam------------------------------------
	virtual INT 				GetDefenceNear(VOID);
	virtual INT 				GetBaseDefenceNear (VOID);
	virtual VOID				SetBaseDefenceNear (INT const nValue);
	
	virtual INT 				GetDefenceFar(VOID);
	virtual INT 				GetBaseDefenceFar (VOID);
	virtual VOID				SetBaseDefenceFar (INT const nValue);
	

	virtual INT 				GetAttackNear(VOID);
	virtual INT 				GetBaseAttackNear(VOID);
	virtual VOID				SetBaseAttackNear(INT const nValue);
	

	virtual INT 				GetAttackFar(VOID);
	virtual INT 				GetBaseAttackFar(VOID);
	virtual VOID				SetBaseAttackFar(INT const nValue);

	virtual INT 				GetToughness(VOID);
	virtual INT 				GetBaseToughness(VOID);
	virtual VOID				SetBaseToughness(INT const nValue);

	virtual INT 				GetNearAttReduce(VOID);
	virtual INT 				GetBaseNearAttReduce(VOID);
	virtual VOID				SetBaseNearAttReduce(INT const nValue);

	virtual INT 				GetFarAttReduce(VOID);
	virtual INT 				GetBaseFarAttReduce(VOID);
	virtual VOID				SetBaseFarAttReduce(INT const nValue);

	virtual INT 				GetMagicNearAttReduce(VOID);
	virtual INT 				GetMagicFarAttReduce(VOID);

	virtual INT 				GetBaseMagicNearAttReduce(VOID);
	virtual INT 				GetBaseMagicFarAttReduce(VOID);

	virtual VOID				SetBaseMagicNearAttReduce(INT const nValue);
	virtual VOID				SetBaseMagicFarAttReduce(INT const nValue);

	virtual INT 				GetDreadAttResist(VOID);
	virtual INT 				GetBaseDreadAttResist(VOID);
	virtual VOID				SetBaseDreadAttResist(INT const nValue);

	virtual INT 				GetComaAttResist(VOID);
	virtual INT 				GetBaseComaAttResist(VOID);
	virtual VOID				SetBaseComaAttResist(INT const nValue);

	//沉默抗性
	virtual INT 				GetHushAttResist(VOID);
	virtual INT 				GetBaseHushAttResist(VOID);
	virtual VOID				SetBaseHushAttResist(INT const nValue);

	//缴械抗性
	virtual INT 				GetUnArmyAttResist(VOID);
	virtual INT 				GetBaseUnArmyAttResist(VOID);
	virtual VOID				SetBaseUnArmyAttResist(INT const nValue);

	//减攻速抗性
	virtual INT 				GetAttSpeedAttResist(VOID);
	virtual INT 				GetBaseAttSpeedAttResist(VOID);
	virtual VOID				SetBaseAttSpeedAttResist(INT const nValue);

	//减攻速抗性
	virtual INT 				GetSkillSpeedAttResist(VOID);
	virtual INT 				GetBaseSkillSpeedAttResist(VOID);
	virtual VOID				SetBaseSkillSpeedAttResist(INT const nValue);

	//减移速抗性
	virtual INT 				GetMoveSpeedAttResist(VOID);
	virtual INT 				GetBaseMoveSpeedAttResist(VOID);
	virtual VOID				SetBaseMoveSpeedAttResist(INT const nValue);
	//-----------------------------------------------------------

	// Attr2: 
	virtual INT 				GetAttackMagicNear(VOID);
	virtual INT 				GetAttackMagicFar(VOID);

	virtual INT 				GetBaseAttackMagicNear(VOID);
	virtual INT 				GetBaseAttackMagicFar(VOID);

	virtual VOID				SetBaseAttackMagicNear(INT const nValue);
	virtual VOID				SetBaseAttackMagicFar(INT const nValue);

	virtual INT 				GetDefenceMagicNear(VOID);
	virtual INT 				GetDefenceMagicFar(VOID);

	virtual INT 				GetBaseDefenceMagicNear(VOID);
	virtual INT 				GetBaseDefenceMagicFar(VOID);

	virtual VOID				SetBaseDefenceMagicNear(INT const nValue);
	virtual VOID				SetBaseDefenceMagicFar(INT const nValue);

	// Attr2: Attack and Defence power of gold
	virtual INT 				GetAttackGold(VOID);
	virtual INT 				GetBaseAttackGold(VOID);
	virtual VOID				SetBaseAttackGold(INT const nValue);
	virtual INT 				GetDefenceGold(VOID);
	virtual INT 				GetBaseDefenceGold(VOID);
	virtual VOID				SetBaseDefenceGold(INT const nValue);

	// Attr2: Attack and Defence power of Wood
	virtual INT 				GetAttackWood(VOID);
	virtual INT 				GetBaseAttackWood(VOID);
	virtual VOID				SetBaseAttackWood(INT const nValue);
	virtual INT 				GetDefenceWood(VOID);
	virtual INT 				GetBaseDefenceWood(VOID);
	virtual VOID				SetBaseDefenceWood(INT const nValue);

	// Attr2: Attack and Defence power of Water
	virtual INT 				GetAttackWater(VOID);
	virtual INT 				GetBaseAttackWater(VOID);
	virtual VOID				SetBaseAttackWater(INT const nValue);
	virtual INT 				GetDefenceWater(VOID);
	virtual INT 				GetBaseDefenceWater(VOID);
	virtual VOID				SetBaseDefenceWater(INT const nValue);

	// Attr2: Attack and Defence power of Fire
	virtual INT 				GetAttackFire(VOID);
	virtual INT 				GetBaseAttackFire(VOID);
	virtual VOID				SetBaseAttackFire(INT const nValue);
	virtual INT 				GetDefenceFire(VOID);
	virtual INT 				GetBaseDefenceFire(VOID);
	virtual VOID				SetBaseDefenceFire(INT const nValue);

	// Attr2: Attack and Defence power of Soil
	virtual INT 				GetAttackSoil(VOID);
	virtual INT 				GetBaseAttackSoil(VOID);
	virtual VOID				SetBaseAttackSoil(INT const nValue);
	virtual INT 				GetDefenceSoil(VOID);
	virtual INT 				GetBaseDefenceSoil(VOID);
	virtual VOID				SetBaseDefenceSoil(INT const nValue);

	// Attr2 Reduce Slower Duration
	virtual INT 				GetReduceSlowerDuration(VOID);
	// Attr2 Reduce Weaken Duration
	virtual INT 				GetReduceWeakenDuration(VOID);
	// Attr2 Reduce Faint Duration
	virtual INT 				GetReduceFaintDuration(VOID);
	// Attr2 Reduce Poisoned Duration
	virtual INT 				GetReducePoisonedDuration(VOID);
	// Attr2 VisionRange
	virtual INT 				GetVisionRange(VOID);
	virtual INT 				GetBaseVisionRange(VOID);
	virtual VOID				SetBaseVisionRange(INT const nValue);
};

#endif	// __PET_H__
