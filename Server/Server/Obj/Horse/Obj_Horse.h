/********************************************************************
	created:	2008/03/18
	created:	18:3:2008   10:18
	filename: 	MapServer\Server\Obj\Horse\Obj_Horse.h
	file path:	MapServer\Server\Obj\Horse
	file base:	Obj_Horse
	file ext:	h
	author:		Richard
	
	purpose:	
*********************************************************************/

#ifndef Obj_Horse_h__
#define Obj_Horse_h__

#include "Obj_Character.h"

#include "HorseStruct.h"
//////////////////////////////////////////////////////
// 宠物
//////////////////////////////////////////////////////

#define MAX_CAPTURER_NUM (6)
#define LEVELDELTA_GETEXP (5)
#define SEND_LIFE_INTERVAL (30)

class AI_Horse;
class Obj_Horse :
	public Obj_Character
{
public:
	friend class AI_Horse;

//===========================================
// Obj公有接口继承
//===========================================
public:
	Obj_Horse( VOID );
	virtual ~Obj_Horse( VOID );

	virtual ObjType		GetObjType( VOID )const{ return OBJ_TYPE_HORSE; }
	virtual VOID 		CleanUp( );
	virtual BOOL 		Init( const _OBJ_INIT *pInit );
	virtual BOOL 		HeartBeat( UINT uTime = 0 );

	virtual VOID		InitAIObj( );

	const _PET_DB*		GetPetDB(VOID) const { return &m_PetDB; }
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

	//virtual VOID 		InitBackupAttr( );
	//virtual VOID 		SendMsg_RefeshAttrib( VOID );
	//virtual VOID 		AskMsg_BaseAttrib( Obj_Human *pTargetHuman );
	//virtual VOID 		AskMsg_DetailAttrib( Obj_Human *pTargetHuman );

protected:
	_HORSE_ATTR_BACKUP	m_AttrBackUp;

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
	CMyTimer			m_LifeTimer;		// 无主马存活时间的计时器

//===========================================
// 其他规则
//===========================================
public:
	VOID				LevelUp(VOID);
	virtual VOID		SetReliveInfo( BOOL bSkillRelive, const _RELIVE_INFO *pReliveInfo );
	virtual VOID 		AskMsg_BaseAttrib( Obj_Human *pTargetHuman );
	virtual VOID 		AskMsg_DetailAttrib( Obj_Human *pTargetHuman );


//===========================================
// 技能/心法
//===========================================
public:
	//virtual BOOL		Skill_HaveSkill( SkillID_t const nID ) const;

	// nIndex	:	ENUM_PET_SKILL_INDEX
	//const _PET_SKILL	*Skill_GetSkill( INT nIndex ) const;

	//INT 				Skill_GetCount_CortrolByPlayer(VOID);
	//INT 				Skill_GetMaxCount_CortrolByPlayer(VOID);
	//INT 				Skill_GetCount_CortrolByAI(VOID);
	//INT 				Skill_GetMaxCount_CortrolByAI(VOID);

	//BOOL 				Skill_RegisterSkill(SkillID_t const nID, INT nIndex);
	//BOOL 				Skill_UnregisterSkill(SkillID_t const nID, INT nIndex);
	//BOOL 				Skill_ModifySkill_Study(SkillID_t nID);
	//BOOL 				Skill_ModifySkill_Realize(SkillID_t nID);
	//BOOL 				Skill_ModifySkill_MenPai(SkillID_t nID);
	//BOOL 				Skill_Apperceive(VOID);

	//virtual INT			Skill_GetXinFaLevel(SkillID_t const nID) const;
	//virtual BOOL 		Skill_HaveXinFa( SkillID_t const nID ) const;
	//virtual BOOL 		Skill_SetXinFaLevel(SkillID_t const nID, INT const nLevel);
	//virtual VOID 		Skill_OnEquipItem(VOID);
//===========================================
// 效果
//===========================================
protected:
	virtual _IMPACT_LIST&	Impact_GetImpactList(VOID){return m_ImpactList;};

private:
	//virtual VOID 			Impact_InitList(VOID);

private:
	_IMPACT_LIST			m_ImpactList;

//===========================================
// CoolDown
//===========================================
public:
	//virtual Time_t		GetCooldownRemained(CooldownID_t const nID) const;
	//virtual BOOL		IsCooldowned(CooldownID_t const nID) const;
	//virtual VOID 		SetCooldown(CooldownID_t const nID, Time_t nCooldown);
	//virtual BOOL 		HeartBeat_Cooldown(UINT uTime = 0);
	//virtual BOOL 		HeartBeat_Recover(UINT uTime = 0);

protected:

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
	//virtual ObjID_t			GetOwnerID(VOID)const;
	//virtual Obj_Character	*GetOwner( VOID );

	AI_Horse*				GetPetAI( VOID ) { return (AI_Horse*)m_pAI_Character; }
	//VOID					SetTargetID( ObjID_t TargetID );
	//ObjID_t					GetTargetID( VOID )const;
	const WORLD_POS*		GetRespawnPos( VOID )const{ return &m_RespawnPos; }

	//VOID					ToAttack( ObjID_t TargetID );
	BOOL					PushSkillToCache(INT idSkill);
	VOID					GetRateOfBuffTakeEffect(INT idSkill, INT& rateOfBuffTakeEffect);

protected:
	ObjID_t					m_OwnerID;			// 当前所有者的ObjID
	ObjID_t					m_TargetID;			// 目标ID
	WORLD_POS				m_RespawnPos;
	INT						m_RespawnTime;
	// 只用来缓存被动技能和护主型技能(即起持续性效果的技能)
	INT						m_aCacheOfSkill[HUMAN_PET_MAX_COUNT]; 
	INT						m_AIType;


//===========================================
// 一级属性
//===========================================
public:
	virtual GUID_t			GetGUID( VOID )const{return m_GUID;};

	Obj_Human				*GetCreator( VOID )const{ return m_pCreator; }
	//const Obj_Human			*GetConstCreator( VOID )const;

	VOID					SetPetGUID(PET_GUID_t guid){ m_PetDB.m_GUID = guid; }
	PET_GUID_t				GetPetGUID(VOID)const{ return m_PetDB.m_GUID; }

	//VOID					SetSpouseGUID(PET_GUID_t guid);
	//PET_GUID_t				GetSpouseGUID(VOID)const;

	VOID					SetDataID(const UINT dataID){ m_PetDB.m_nDataID = dataID; }
	virtual UINT			GetDataID( VOID )const{ return m_PetDB.m_nDataID; } 

	virtual	VOID			SetName( const CHAR *pszName ){};
	virtual const CHAR		*GetName( VOID )const{return m_PetDB.m_szNick;};

	virtual	VOID			SetTitle( const CHAR *pszTitle ){};
	virtual const CHAR		*GetTitle( VOID )const{return m_PetDB.m_szName;};

	//VOID					IncrementExp(INT nIncrement);
	virtual INT				GetExp( VOID )const{ return 0; };
	virtual VOID			SetExp( INT nExp ){};

	virtual VOID			SetHP( INT nHP ){};
	virtual INT				GetHP( VOID ){return 100;};

	virtual VOID			SetMP( INT nMP ){};
	virtual INT				GetMP( VOID ){return 0;};

	virtual INT 			GetRage( VOID ){ return 0; }
	virtual VOID			SetRage( INT nRage ){};

	virtual INT				GetAIType( VOID )const{ return m_PetDB.m_AIType; };
	virtual VOID			SetAIType( INT nAIType ){};

	virtual INT				GetLevel( VOID )const{return 0;};
	virtual VOID			SetLevel( INT nLevel ){};

	virtual INT				GetLife( VOID )const{return 100;};
	virtual VOID			SetLife( INT nLife ){};

	virtual BYTE			GetGeneration( VOID )const{ return m_PetDB.m_byGeneration; };
	virtual VOID			SetGeneration( BYTE byGeneration ){};

	//VOID 					IncrementHappyness(INT nIncrement);
	//BYTE 					GetHappiness( VOID )const;
	//VOID 					SetHappiness( BYTE byHappiness );

	//INT						GetGenGu( VOID )const;
	//VOID					SetGenGu( INT nGenGu );

	virtual INT 			GetStrPerception( VOID )const{ return 0; };
	virtual INT 			GetSmartnessPerception( VOID )const{ return 0; };
	virtual INT 			GetMindPerception( VOID )const{ return 0; };
	virtual INT 			GetConstitutionPerception( VOID )const{ return 0; };

	virtual INT				GetRemainPoint( VOID )const{ return 0; };
	virtual VOID			SetRemainPoint( INT nRemainPoint ){}

protected:
	GUID_t					m_GUID;				// 角色在场景中的GUID，与PetGUID是两回事
	ObjID_t					m_CreatorID;		// 创建者的ObjID
	Obj_Human*				m_pCreator;			// 宠物的创建者
	INT						m_nIndex;			// 标识该宠物是玩家宠物槽中的第几个
	_PET_DB					m_PetDB;


//===========================================
// 二级属性
//===========================================
public:
	//StrikePoint
	virtual INT 				GetMaxStrikePoint(VOID){ return 0;};
	//Rage
	virtual INT 				GetMaxRage(VOID){ return 0;}
	virtual INT 				GetBaseMaxRage(VOID){ return 0;}
	virtual VOID 				SetBaseMaxRage(INT const nRage){};
	virtual INT					GetRageRegenerate(VOID){ return 0;}
	virtual INT 				GetBaseRageRegenerate(VOID){ return 0;}
	virtual VOID 				SetBaseRageRegenerate(INT const nValue){};
	

	////Attr1: Str
	virtual INT					GetStr(VOID){ return 0;};
	virtual INT					GetBaseStr(VOID){ return 0;};
	virtual VOID 				SetBaseStr(INT const nValue){};
	//////Attr1: Spr
	////virtual INT					GetSpr(VOID);
	////virtual INT					GetBaseSpr(VOID);
	////virtual VOID				SetBaseSpr(INT const nValue);
	//Attr1: Con
	virtual INT					GetCon(VOID){ return 0;};
	virtual INT					GetBaseCon(VOID){ return 0;};
	virtual VOID				SetBaseCon(INT const nValue){};
	//Attr1: Int
	virtual INT					GetInt(VOID){ return 0;};
	virtual INT					GetBaseInt(VOID){ return 0;};
	virtual VOID				SetBaseInt(INT const nValue){};
	//Attr1: Dex
	virtual INT					GetDex(VOID){ return 0;};
	virtual INT					GetBaseDex(VOID){ return 0;};
	virtual VOID				SetBaseDex(INT const nValue){};
	//HP
	virtual INT					GetMaxHP(VOID){ return 100;}
	virtual INT 				GetBaseMaxHP(VOID){ return 0;};
	virtual VOID 				SetBaseMaxHP(INT const nHp){};
	virtual INT					GetHPRegenerate(VOID){ return 0;} ; 
	virtual INT 				GetBaseHPRegenerate(VOID){ return 0;};
	virtual VOID 				SetBaseHPRegenerate(INT const nValue){};

	//MP
	virtual INT 				GetMaxMP(VOID){ return 0;};
	virtual INT 				GetBaseMaxMP(VOID){ return 0;};
	virtual VOID 				SetBaseMaxMp(INT const nMp){};
	virtual INT					GetMPRegenerate(VOID){ return 0;}; 
	virtual INT 				GetBaseMPRegenerate(VOID){ return 0;};
	virtual VOID 				SetBaseMPRegenerate(INT const nValue){};

	virtual INT 				GetMaxPneuma(VOID) {Assert(FALSE);return 0;}
	virtual INT 				GetBaseMaxPneuma(VOID){Assert(FALSE);return 0;}
	virtual VOID 				SetBaseMaxPneuma(INT const nPneuma){Assert(FALSE);return;}
	virtual INT					GetPneumaRegenerate(VOID){Assert(FALSE);return 0;}
	virtual INT 				GetBasePneumaRegenerate(VOID){Assert(FALSE);return 0;}
	virtual VOID 				SetBasePneumaRegenerate(INT const nValue){Assert(FALSE);return;}

	//CampID
	virtual const _CAMP_DATA	*GetCampData(VOID){return &m_PetDB.m_CampData;};
	virtual const _CAMP_DATA	*GetBaseCampData(VOID)const{return &m_PetDB.m_CampData;}
	virtual VOID				SetBaseCampData(const _CAMP_DATA *pCampData){};
	virtual INT 				GetCampID(VOID){return m_PetDB.m_CampData.m_nCampID;}
	virtual INT 				GetBaseCampID(VOID) const{return m_PetDB.m_CampData.m_nCampID;};
	virtual VOID				SetBaseCampID(INT const nID){};
	////modelID
	virtual INT 				GetModelID(VOID);
	virtual INT 				GetMountID(VOID);

	virtual INT 				GetBaseModelID(VOID) const {return 0;};
	virtual VOID				SetBaseModelID(INT const nID){};
	//Alive flag
	//In Combat flag
	// Can move flag
	// Can Action flag
	// Unbreakable flag
	// Attr2: Move Speed
	virtual FLOAT				GetMoveSpeed(VOID);
	virtual INT					GetBaseMoveSpeed(VOID){ return 0; };
	virtual VOID				SetBaseMoveSpeed(INT const nValue){};
	// Attr2: Attack Speed
	virtual INT 				GetAttackSpeed(VOID){ return 0; };
	virtual INT 				GetBaseAttackSpeed(VOID){ return 0; };
	virtual VOID				SetBaseAttackSpeed(INT const nValue){};
	// Attr2: Miss
	virtual INT 				GetMiss(VOID){ return 0; };
	virtual INT 				GetBaseMiss(VOID){ return 0; };
	virtual VOID				SetBaseMiss(INT const nValue){};
	// Attr2 Hit
	virtual INT 				GetHit(VOID){ return 0; };
	virtual INT 				GetBaseHit(VOID){ return 0; };
	virtual VOID				SetBaseHit(INT const nValue){};
	// Attr2 Critical
	virtual INT 				GetCritical(VOID){ return 0; };
	virtual INT 				GetBaseCritical(VOID){ return 0; };
	virtual VOID				SetBaseCritical(INT const nValue){};
	// Attr2: Attack and Defence power physics
	/*virtual INT 				GetAttackPhysics(VOID);
	virtual INT 				GetBaseAttackPhysics(VOID);
	virtual VOID				SetBaseAttackPhysics(INT const nValue);
	virtual INT 				GetDefencePhysics(VOID);
	virtual INT 				GetBaseDefencePhysics(VOID);
	virtual VOID				SetBaseDefencePhysics(INT const nValue);*/
	
	//add by liam------------------------------------
	virtual INT 				GetDefenceNear(VOID){ return 0; };
	virtual INT 				GetBaseDefenceNear (VOID){ return 0; };
	virtual VOID				SetBaseDefenceNear (INT const nValue){};
	
	virtual INT 				GetDefenceFar(VOID){ return 0; };
	virtual INT 				GetBaseDefenceFar (VOID){ return 0; };
	virtual VOID				SetBaseDefenceFar (INT const nValue){};
	

	virtual INT 				GetAttackNear(VOID){ return 0; };
	virtual INT 				GetBaseAttackNear(VOID){ return 0; };
	virtual VOID				SetBaseAttackNear(INT const nValue){};
	

	virtual INT 				GetAttackFar(VOID){ return 0; };
	virtual INT 				GetBaseAttackFar(VOID){ return 0; };
	virtual VOID				SetBaseAttackFar(INT const nValue){};

	virtual INT 				GetToughness(VOID){ return 0; };
	virtual INT 				GetBaseToughness(VOID){ return 0; };
	virtual VOID				SetBaseToughness(INT const nValue){};

	virtual INT 				GetNearAttReduce(VOID){ return 0; };
	virtual INT 				GetBaseNearAttReduce(VOID){ return 0; };
	virtual VOID				SetBaseNearAttReduce(INT const nValue){};

	virtual INT 				GetFarAttReduce(VOID){ return 0; };
	virtual INT 				GetBaseFarAttReduce(VOID){ return 0; };
	virtual VOID				SetBaseFarAttReduce(INT const nValue){};

	virtual INT 				GetMagicNearAttReduce(VOID){ return 0; };
	virtual INT 				GetMagicFarAttReduce(VOID){ return 0; };

	virtual INT 				GetBaseMagicNearAttReduce(VOID){ return 0; };
	virtual INT 				GetBaseMagicFarAttReduce(VOID){ return 0; };

	virtual VOID				SetBaseMagicNearAttReduce(INT const nValue){};
	virtual VOID				SetBaseMagicFarAttReduce(INT const nValue){};

	virtual INT 				GetDreadAttResist(VOID){ return 0; };
	virtual INT 				GetBaseDreadAttResist(VOID){ return 0; };
	virtual VOID				SetBaseDreadAttResist(INT const nValue){};

	virtual INT 				GetComaAttResist(VOID){ return 0; };
	virtual INT 				GetBaseComaAttResist(VOID){ return 0; };
	virtual VOID				SetBaseComaAttResist(INT const nValue){};

	//沉默抗性
	virtual INT 				GetHushAttResist(VOID){ return 0; };
	virtual INT 				GetBaseHushAttResist(VOID){ return 0; };
	virtual VOID				SetBaseHushAttResist(INT const nValue){};

	//缴械抗性
	virtual INT 				GetUnArmyAttResist(VOID){ return 0; };
	virtual INT 				GetBaseUnArmyAttResist(VOID){ return 0; };
	virtual VOID				SetBaseUnArmyAttResist(INT const nValue){};

	//减攻速抗性
	virtual INT 				GetAttSpeedAttResist(VOID){ return 0; };
	virtual INT 				GetBaseAttSpeedAttResist(VOID){ return 0; };
	virtual VOID				SetBaseAttSpeedAttResist(INT const nValue){};

	//减攻速抗性
	virtual INT 				GetSkillSpeedAttResist(VOID){ return 0; };
	virtual INT 				GetBaseSkillSpeedAttResist(VOID){ return 0; };
	virtual VOID				SetBaseSkillSpeedAttResist(INT const nValue){};

	//减移速抗性
	virtual INT 				GetMoveSpeedAttResist(VOID){ return 0; };
	virtual INT 				GetBaseMoveSpeedAttResist(VOID){ return 0; };
	virtual VOID				SetBaseMoveSpeedAttResist(INT const nValue){};
	//-----------------------------------------------------------

	// Attr2: 近程内功攻击力
	virtual INT 				GetAttackMagicNear(VOID){ return 0; };
	virtual INT 				GetAttackMagicFar(VOID){ return 0; };

	virtual INT 				GetBaseAttackMagicNear(VOID){ return 0; };
	virtual INT 				GetBaseAttackMagicFar(VOID){ return 0; };

	virtual VOID				SetBaseAttackMagicNear(INT const nValue){};
	virtual VOID				SetBaseAttackMagicFar(INT const nValue){};

	virtual INT 				GetDefenceMagicNear(VOID){ return 0; };
	virtual INT 				GetDefenceMagicFar(VOID){ return 0; };

	virtual INT 				GetBaseDefenceMagicNear(VOID){ return 0; };
	virtual INT 				GetBaseDefenceMagicFar(VOID){ return 0; };

	virtual VOID				SetBaseDefenceMagicNear(INT const nValue){};
	virtual VOID				SetBaseDefenceMagicFar(INT const nValue){};

	// Attr2: Attack and Defence power of gold
	virtual INT 				GetAttackGold(VOID){ return 0; };
	virtual INT 				GetBaseAttackGold(VOID){ return 0; };
	virtual VOID				SetBaseAttackGold(INT const nValue){};
	virtual INT 				GetDefenceGold(VOID){ return 0; };
	virtual INT 				GetBaseDefenceGold(VOID){ return 0; };
	virtual VOID				SetBaseDefenceGold(INT const nValue){};

	// Attr2: Attack and Defence power of Wood
	virtual INT 				GetAttackWood(VOID){ return 0; };
	virtual INT 				GetBaseAttackWood(VOID){ return 0; };
	virtual VOID				SetBaseAttackWood(INT const nValue){};
	virtual INT 				GetDefenceWood(VOID){ return 0; };
	virtual INT 				GetBaseDefenceWood(VOID){ return 0; };
	virtual VOID				SetBaseDefenceWood(INT const nValue){};

	// Attr2: Attack and Defence power of Water
	virtual INT 				GetAttackWater(VOID){ return 0; };
	virtual INT 				GetBaseAttackWater(VOID){ return 0; };
	virtual VOID				SetBaseAttackWater(INT const nValue){};
	virtual INT 				GetDefenceWater(VOID){ return 0; };
	virtual INT 				GetBaseDefenceWater(VOID){ return 0; };
	virtual VOID				SetBaseDefenceWater(INT const nValue){};

	// Attr2: Attack and Defence power of Fire
	virtual INT 				GetAttackFire(VOID){ return 0; };
	virtual INT 				GetBaseAttackFire(VOID){ return 0; };
	virtual VOID				SetBaseAttackFire(INT const nValue){};
	virtual INT 				GetDefenceFire(VOID){ return 0; };
	virtual INT 				GetBaseDefenceFire(VOID){ return 0; };
	virtual VOID				SetBaseDefenceFire(INT const nValue){};

	// Attr2: Attack and Defence power of Soil
	virtual INT 				GetAttackSoil(VOID){ return 0; };
	virtual INT 				GetBaseAttackSoil(VOID){ return 0; };
	virtual VOID				SetBaseAttackSoil(INT const nValue){};
	virtual INT 				GetDefenceSoil(VOID){ return 0; };
	virtual INT 				GetBaseDefenceSoil(VOID){ return 0; };
	virtual VOID				SetBaseDefenceSoil(INT const nValue){};

	// Attr2 Reduce Slower Duration
	virtual INT 				GetReduceSlowerDuration(VOID){ return 0; };
	// Attr2 Reduce Weaken Duration
	virtual INT 				GetReduceWeakenDuration(VOID){ return 0; };
	// Attr2 Reduce Faint Duration
	virtual INT 				GetReduceFaintDuration(VOID){ return 0; };
	// Attr2 Reduce Poisoned Duration
	virtual INT 				GetReducePoisonedDuration(VOID){ return 0; };
	// Attr2 VisionRange
	virtual INT 				GetVisionRange(VOID){ return 0; };
	virtual INT 				GetBaseVisionRange(VOID){ return 0; };
	virtual VOID				SetBaseVisionRange(INT const nValue){};
};

#endif // Obj_Horse_h__
