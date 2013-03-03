// Obj_Monster.h
//
/////////////////////////////////////////////////////////

#ifndef __OBJ_MONSTER_H__
#define __OBJ_MONSTER_H__

#include "Obj_Character.h"

/////////////////////////////////////////////////////////
//怪物初始化数据
/////////////////////////////////////////////////////////

#define MAX_SHOP_INIT 4
struct _OBJ_MONSTER_INIT :
	public _OBJ_CHARACTER_INIT
{
	GUID_t				m_GUID;
	UINT				m_uDataID;
	CHAR				m_szName[NPC_NAME_LEN];
	CHAR				m_szTitle[NPC_TITLE_LEN];
	ScriptID_t			m_idScript;
	INT					m_RespawnTime;
	UINT				m_uGroupID;
	UINT				m_uTeamID;
	INT					m_BaseAI;
	INT					m_ExtAIScript;
	INT					m_nPatrolID;
	INT					m_nShop[MAX_SHOP_INIT];
	UINT				m_LeaderID;
	BOOL				m_bPet;

	_OBJ_MONSTER_INIT( VOID )
	{
		m_GUID			= -1;
		m_uDataID		= -1;
		m_szName[0]		= 0;
		m_szTitle[0]	= 0;
		m_RespawnTime	= 60000;
		m_uGroupID     = -1;
		m_uTeamID		= -1;
		m_BaseAI		= 0;
		m_idScript		= -1;
		m_ExtAIScript	= 0;
		m_nPatrolID		= -1;
		m_LeaderID		= -1;
		m_bPet			= FALSE;
		for( INT i=0; i<MAX_SHOP_INIT; i++ )
		{
			m_nShop[i] = -1 ;
		}
	}

	virtual VOID CleanUp( VOID )
	{
		m_GUID			= -1;
		m_uDataID		= -1;
		m_szName[0]		= 0;
		m_szTitle[0]	= 0;
		m_RespawnTime	= 60000;
		m_uGroupID     = -1;
		m_uTeamID		= -1;
		m_BaseAI		= 0;
		m_ExtAIScript	= -1;
		m_idScript		= -1;
		m_nPatrolID		= -1;
		m_LeaderID		= -1;
		m_bPet			= FALSE;
		for( INT i=0; i<MAX_SHOP_INIT; i++ )
		{
			m_nShop[i] = -1 ;
		}

		_OBJ_CHARACTER_INIT::CleanUp();
	}
};

/////////////////////////////////////////////////////////
// 属性刷新的数据备份
/////////////////////////////////////////////////////////
struct _MONSTER_ATTR_BACKUP
{
	BOOL		m_bNameModified;
	BOOL		m_bTitleModified;
	INT			m_HP;
	INT			m_MP;			//	add by gh 2010/03/25
	INT			m_MaxHp;
	INT			m_MaxMp;		//	add by gh 2010/03/25
	FLOAT       m_MoveSpeed;
	_CAMP_DATA	m_CampData;
	INT			m_nDataID;
	INT			m_nModelID;
	INT			m_nMountID;
	INT			m_nAIType;
	ObjID_t		m_OwnerID;
	GUID_t		m_OccupantGUID;
	INT			m_nStealthLevel;	// 隐身级别
	INT			m_nMoodState;
	INT			m_iWeaponID;
	INT			m_iHorseID;

	_MONSTER_ATTR_BACKUP(VOID)
	{
		m_bNameModified		= FALSE;
		m_bTitleModified	= FALSE;
		m_HP				= 0;
		m_MaxHp				= 1;
		m_MoveSpeed			= 0.f;
		//m_CampData
		m_nDataID			= INVALID_ID;
		m_nModelID			= INVALID_ID;
		m_nMountID			= INVALID_ID;
		m_OwnerID			= INVALID_ID;
		m_OccupantGUID		= INVALID_GUID;
		m_nStealthLevel		= -1;
		m_nMoodState		= INVALID_MOOD_STATE;
		m_iWeaponID			= -1;
		m_iHorseID			= -1;
	}

	VOID CleanUp(VOID)
	{
		m_bNameModified		= FALSE;
		m_bTitleModified	= FALSE;
		m_HP				= 0;
		m_MaxHp				= 1;
		m_MoveSpeed			= 0.f;
		m_CampData.CleanUp();
		m_nDataID			= INVALID_ID;
		m_nModelID			= INVALID_ID;
		m_nMountID			= INVALID_ID;
		m_OwnerID			= INVALID_ID;
		m_OccupantGUID		= INVALID_GUID;
		m_nStealthLevel		= -1;
		m_nMoodState		= INVALID_MOOD_STATE;
		m_iWeaponID			= -1;
		m_iHorseID			= -1;

	}
};

class Obj_Human;
class AI_Monster;
class DynamicShopManager;
class PetPlacardSystem;
class Obj_Monster :
	public Obj_Character
{
//===========================================
// Obj接口继承
//===========================================
public:
    Obj_Monster( VOID );
	virtual ~Obj_Monster( VOID );

	virtual ObjType		GetObjType( VOID )const{ return OBJ_TYPE_MONSTER; }

	virtual VOID		CleanUp( VOID );
	virtual BOOL		Init( const _OBJ_INIT *pInit );
	virtual BOOL		HeartBeat( UINT uTime = 0 );
	virtual BOOL		HeartBeat_OutZone( UINT uTime=0 );

//===========================================
// Character接口继承
//===========================================
public:
	virtual VOID		InitAIObj( );
	virtual VOID		OnDie( ObjID_t idKiller );			//死了
	virtual BOOL		IsEnemy(Obj_Character* pCharacter );
	virtual BOOL		IsFriend(Obj_Character* pCharacter);
	virtual BOOL		IsPartner(Obj_Character* pCharacter);

public:
	VOID				OnDie_Before( ObjID_t idKiller ) ;	//死亡前处理的逻辑
	VOID				OnDie_After( ObjID_t idKiller );	//死亡后处理的逻辑
	//马掉落
	VOID DropHorseOnDie_After();
//===========================================
// 消息/属性刷新
//===========================================
public:
	virtual Packet			*CreateNewObjPacket( VOID );
	virtual VOID			DestroyNewObjPacket( Packet *pPacket );

	virtual VOID			InitBackupAttr( );
	virtual VOID			SendMsg_RefeshAttrib( VOID );
	virtual VOID			AskMsg_BaseAttrib( Obj_Human *pTargetHuman );
protected:
	//为技能同步，实时发送CharBase消息，并清除HP脏标记，组织自动刷属性时重复发送
	virtual VOID SendCharBaseDirectForHP();
protected:
	_MONSTER_ATTR_BACKUP	m_AttrBackUp;

//===========================================
// 技能/心法
//===========================================
public:
	virtual BOOL 	Skill_HaveSkill( SkillID_t const nID ) const{return TRUE;}
	virtual BOOL 	Skill_ModifySkill(SkillID_t	const nID, INT const nModifyFlag){return TRUE;}
	virtual BOOL 	Skill_HaveXinFa( SkillID_t const nID ) const{return TRUE;}
	virtual INT 	Skill_GetXinFaLevel(SkillID_t const nID) const{return 1;}
	virtual BOOL 	Skill_SetXinFaLevel(SkillID_t const nID, INT const nLevel){return TRUE;}
	virtual VOID 	Skill_OnEquipItem(VOID){}

//===========================================
// CoolDown
//===========================================
public:
	virtual BOOL		IsCooldowned(CooldownID_t const nID) const;
	virtual VOID		SetCooldown(CooldownID_t const nID, Time_t nCooldown);
	virtual BOOL 		HeartBeat_Cooldown(UINT uTime = 0);

protected:
	INT					m_nCooldown;

//===========================================
// HP，MP，Rage的变化规则
//===========================================
public:
	virtual BOOL		HeartBeat_Recover(UINT uTime = 0);

//===========================================
// 效果
//===========================================
protected:
	virtual _IMPACT_LIST&	Impact_GetImpactList(VOID){return m_ImpactList;}

private:
	virtual VOID 			Impact_InitList(VOID);

private:
	_IMPACT_LIST			m_ImpactList;

//===========================================
// 重生
//===========================================
public:
	VOID					Enter_Respawn( VOID );
	BOOL					Respawn( CharacterVisitor* pVisitor );
	INT						CannotFight();

	INT						GetRespawnTime( VOID )const{ return m_RespawnTime; }

	VOID					SetRespawnPos( const WORLD_POS *pPos ){ m_RespawnPos = *pPos; }
	const WORLD_POS			*GetRespawnPos( VOID )const{ return &m_RespawnPos; }

	VOID					SetRespawnDir( FLOAT fDir ){ m_RespawnDir = fDir; }
	FLOAT					GetRespawnDir( VOID )const{ return m_RespawnDir; }

private:
	INT						m_RespawnTime;
	CMyTimer				m_RespawnTimer;		//尸体消失后到重生的时间间隔
	FLOAT					m_RespawnDir;
	WORLD_POS				m_RespawnPos;

//===========================================
// 伤害列表
//===========================================
public:
	VOID					SetOccupantTeamID( TeamID_t tid ){ m_Own_TeamID = tid ; }
	TeamID_t				GetOccupantTeamID( ){ return m_Own_TeamID ; }

	VOID					SetOccupantGUID( GUID_t guid ){ m_OccupantGUID = guid ; }
	GUID_t					GetOccupantGUID( ){ return m_OccupantGUID ; }

	VOID					SetOccupantLevel( UINT nLevel ){ m_OccupantLevel = nLevel ; }
	UINT					GetOccupantLevel( ){ return m_OccupantLevel; }

	VOID					UpdateDamageList(INT nDamage, Obj_Character* pCharacter);

	DAMAGE_MEM_LIST&		GetKillerRec();
	MONSTER_OWNER_LIST&		GetOwnerList();
	void					ClearKiller()
	{
							m_DamageMemList.CleanUp();
							m_Own_TeamID = INVALID_ID ;
							m_OccupantGUID = INVALID_ID ;
	}
	
	// 设置分配模式
	inline VOID				SetDistribMode(BYTE nMode){ m_DistribMode = nMode;}

	// 获得分配模式
	inline const BYTE		GetDistribMode() const { return m_DistribMode;}

	// 设置上一次分配者
	inline VOID				SetLastOwnerPos(BYTE nPos){ m_LastOwnerPos = nPos;}

	// 获得上一次分配者
	inline const BYTE		GetLastOwnerPos() const { return m_LastOwnerPos;}

	// 设置拥有者team leader
	inline VOID				SetOwnTeamLeader(GUID_t nGuid){ m_OwnTeamLeaderGUID = nGuid;}

	// 获得拥有者team leader
	inline const GUID_t		GetOwnTeamLeader() const { return m_OwnTeamLeaderGUID;}
protected:
	TeamID_t				m_Own_TeamID ;//此npc的归属
	GUID_t					m_OccupantGUID ;//此npc的归属（仅在m_Own_TeamID等于INVALID_ID时有效）
	UINT					m_OccupantLevel; //此NPC归属的level
	DAMAGE_MEM_LIST			m_DamageMemList;
	MONSTER_OWNER_LIST		m_OwnerList;

	GUID_t					m_OwnTeamLeaderGUID ;//此npc的归属队伍队长GUID
	BYTE					m_DistribMode;
	BYTE					m_LastOwnerPos;

	
//===========================================
// AI
//===========================================
public:
	AI_Monster				*GetMonsterAI( VOID )const{ return ((AI_Monster*)m_pAI_Character); }
	VOID					SetAIType(INT AIType) { m_AIType = AIType; }
	INT						GetAIType( VOID )const{ return m_AIType; }
	INT						GetMonsterAIType(VOID);
	INT						GetPosRange( VOID )const{ return m_PositionRange; }

	VOID					PaoPao(INT idPaoPaoBase, INT idPaoPaoStep);
	VOID					ProcessPaoPao(VOID);

	VOID					SetLeaderID(ObjID_t LeaderID) { m_LeaderID = LeaderID; 	}
	ObjID_t					GetLeaderID(VOID) const { return m_LeaderID; }
	INT						GetAIScript() const { return m_AIScript; }
	VOID					SetAIScript(INT val) { m_AIScript = val; }
private:
	INT						m_AIType;
	INT						m_AIScript;

	ObjID_t					m_LeaderID;
	INT						m_PositionRange;
//-------------------------------------------------------------
	Time_t					m_nOwnerElapsedTick;	//拥有者逝去的时间
//===========================================
// NPC商店
//===========================================
public:
	DynamicShopManager*		GetShopManager(){ return m_pShopManager; }
	INT						BindDynamicShop(INT shopindex);
	VOID					InitDynamicShop();

protected:
	//动态商店管理器
	DynamicShopManager*		m_pShopManager;

//===========================================
// 宠物公告板
//===========================================
public:
	PetPlacardSystem		*GetPetPlacardSystem(VOID){ return m_pPetPlacardSystem; }
	BOOL					CreatePetPlacardSystem(VOID);

protected:
	VOID					ReleasePetPlacardSystem(VOID);

protected:
	PetPlacardSystem		*m_pPetPlacardSystem;

//===========================================
// 其他
//===========================================
public:
	BOOL					isPatrolMonster( VOID ) { return m_nPatrolID != INVALID_ID ? TRUE : FALSE; }
	INT						GetPatrolPathIndex( VOID ) const { return m_nPatrolID; }
	VOID					SetPatrolId( INT PatrolID );
	//VOID					StartPatrol( VOID );
	//VOID					StopPatrol( VOID );

	FLOAT					GetDropSearchRange() const;		//掉落搜索半径
	INT						GetDropTeamCount() const;		//掉落有效组数量
	VOID					SetCreateFlag(BOOL Flag){ m_CreateFlag = Flag; }
	
	INT						GetMonsterDropSetID(){ return m_MonsterDropItemSetID; }
	INT						GetMonsterDropRate(){ return m_nMonsterDropRate; }

	//获取最近可掉落的点(dropType代表类型)
	virtual	BOOL			GetNearCanDropPos(WORLD_POS& dropPos);
private:
	INT						m_nPatrolID;
	FLOAT					m_DropSearchRange;
	INT						m_DropTeamCount;
	BOOL					m_CreateFlag;
	INT						m_MonsterDropItemSetID; //掉落物品组
	INT						m_nMonsterDropRate;		//掉落概率
	BOX_DISTRIBUTE_RULER	m_DropRuler ;

public:
	//OnKillObject_begin
	enum
	{
		BEKILL_MISSION_NUM = 16,
	};
	typedef struct SKillObj
	{
		INT m_nMissionId;
		
	}tagKillObj[BEKILL_MISSION_NUM];
	tagKillObj m_sKillObj;
	INT m_nKillCount;
	//OnKillObject_end

//===========================================
// 一级属性
//===========================================
public:
	virtual GUID_t			GetGUID( VOID )const{ return m_GUID; }

	virtual UINT			GetDataID( VOID )const{ return m_uDataID; }

	virtual	VOID			SetName( const CHAR *pszName );
	virtual const CHAR		*GetName( VOID )const{ return m_szName; }

	virtual	VOID			SetTitle( const CHAR *pszTitle );
	virtual const CHAR		*GetTitle( VOID )const{ return m_szTitle; }

	virtual VOID			SetHP( INT nHP );
	virtual INT				GetHP( VOID );

	virtual VOID			SetMP( INT nMP );
	virtual INT				GetMP( VOID );
	
	virtual INT 			GetRage( VOID ){ return 0; }
	virtual VOID			SetRage( INT nRage );

	virtual VOID			SetLevel( INT nLevel );
	virtual INT				GetLevel( VOID )const{ return m_Level; }

	virtual VOID			SetScriptID( ScriptID_t idScript ){ m_idScript = idScript; }
	virtual ScriptID_t		GetScriptID( VOID )const{ return m_idScript; }

	INT						GetBaseExp( VOID )const{ return m_BaseExp; }
	VOID					SetBaseExp( INT iBaseExp ){ m_BaseExp = iBaseExp; }

	//最小伤血百分比
	INT						GetMinDamagePercent() const; 			
	VOID					SetMinDamagePercent( INT iPercent ){m_MinDamagePercent=iPercent;}

	UINT					GetGroupID(){ return m_uGroupID ; }
	UINT					GetTeamID(){ return m_uTeamID ; }

	INT						GetAttackAnimTime(){return m_AttackAnimTime;}
	INT						GetAttackCooldownTime(){return m_AttackCooldownTime;}

private:
	GUID_t					m_GUID;
	UINT					m_uDataID;			// 资源ID
	CHAR					m_szName[NPC_NAME_LEN];
	CHAR					m_szTitle[NPC_TITLE_LEN];
	INT						m_HP;
	INT						m_MP;				//	add by gh 2010/03/25
	INT						m_Level;
	INT						m_BaseExp;
	ScriptID_t				m_idScript;
	_CAMP_DATA				m_CampData;
	INT						m_MinDamagePercent;
	UINT					m_uGroupID;
	UINT					m_uTeamID;
	INT						m_AttackAnimTime;
	INT						m_AttackCooldownTime;

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

	//Pneuma
	virtual INT 				GetMaxPneuma(VOID) {return 0;}
	virtual INT 				GetBaseMaxPneuma(VOID){return 0;}
	virtual VOID 				SetBaseMaxPneuma(INT const nPneuma){return;}
	virtual INT					GetPneumaRegenerate(VOID){return 0;}
	virtual INT 				GetBasePneumaRegenerate(VOID){Assert(FALSE);return 0;}
	virtual VOID 				SetBasePneumaRegenerate(INT const nValue){return;}

	//CampID
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
	virtual BOOL				IsUnbreakable(VOID);	
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
private:
	//************************************
	// Method:    ClearSelf
	// FullName:  Obj_Monster::ClearSelf
	// Access:    private 
	// Returns:   
	// Qualifier:
	// 将所有属性初始化
	//************************************
	VOID ClearSelf();
public:
	//************************************
	// Method:    HorseID
	// FullName:  Obj_Monster::HorseID
	// Access:    private 
	// Returns:   INT
	// Qualifier: const
	// 获得马ID
	//************************************
	INT GetHorseID() const { return m_iHorseID; }
	//************************************
	// Method:    HorseID
	// FullName:  Obj_Monster::HorseID
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT val
	// 设置马ID
	//************************************
	VOID SetHorseID(INT val) { m_iHorseID = val; }
	//************************************
	// Method:    WeaponID
	// FullName:  Obj_Monster::WeaponID
	// Access:    private 
	// Returns:   INT
	// Qualifier: const
	// 获得武器ID
	//************************************
	INT GetWeaponID() const { return m_iWeaponID; }
	//************************************
	// Method:    WeaponID
	// FullName:  Obj_Monster::WeaponID
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT val
	// 设置武器ID
	//************************************
	VOID SetWeaponID(INT val) { m_iWeaponID = val; }
	//每次重生都会改变的属性，由怪物自己缓存
private:
	
	INT m_iAttackNear;		//近程攻击
	INT m_iAttackFar;		//远程攻击

	INT m_iAttackMagicNear;	//近程内功攻击
	INT m_iAttackMagicFar;	//近程外功攻击

	INT m_iDefenceNear;		//近程防御
	INT m_iDefenceFar;		//远程防御

	INT m_iDefenceMagicNear;//近程内功防御
	INT m_iDefenceMagicFar;//近程外功防御

	INT m_iHorseID;			//马ID
	INT m_iWeaponID;		//武器ID
	INT m_iMaxHP;			//HP上限
	INT m_iMaxMP;			//MP上限
	INT m_iHPRestore;		//HP回复
	INT m_iMPRestore;		//MP回复
	INT m_iHitRate;			//命中率
	INT m_iMissRate;		//闪避率
	INT m_iCriticalRate;	//会心率

	INT m_iAttackGold;		//金攻击
	INT m_iAttackWood;		//木攻击

	INT m_iAttackWater;		//水攻击
	INT m_iAttackFire;		//火攻击
	INT m_iAttackSoil;		//土攻击
	

	INT m_iDefenceGold;		//金防守
	INT m_iDefenceWood;		//木防守

	INT m_iDefenceWater;	//水防守
	INT m_iDefenceFire;		//火防守
	INT m_iDefenceSoil;		//土防守
};

#endif
