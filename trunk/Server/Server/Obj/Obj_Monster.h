// Obj_Monster.h
//
/////////////////////////////////////////////////////////

#ifndef __OBJ_MONSTER_H__
#define __OBJ_MONSTER_H__

#include "Obj_Character.h"

/////////////////////////////////////////////////////////
//�����ʼ������
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
// ����ˢ�µ����ݱ���
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
	INT			m_nStealthLevel;	// ������
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
// Obj�ӿڼ̳�
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
// Character�ӿڼ̳�
//===========================================
public:
	virtual VOID		InitAIObj( );
	virtual VOID		OnDie( ObjID_t idKiller );			//����
	virtual BOOL		IsEnemy(Obj_Character* pCharacter );
	virtual BOOL		IsFriend(Obj_Character* pCharacter);
	virtual BOOL		IsPartner(Obj_Character* pCharacter);

public:
	VOID				OnDie_Before( ObjID_t idKiller ) ;	//����ǰ������߼�
	VOID				OnDie_After( ObjID_t idKiller );	//����������߼�
	//�����
	VOID DropHorseOnDie_After();
//===========================================
// ��Ϣ/����ˢ��
//===========================================
public:
	virtual Packet			*CreateNewObjPacket( VOID );
	virtual VOID			DestroyNewObjPacket( Packet *pPacket );

	virtual VOID			InitBackupAttr( );
	virtual VOID			SendMsg_RefeshAttrib( VOID );
	virtual VOID			AskMsg_BaseAttrib( Obj_Human *pTargetHuman );
protected:
	//Ϊ����ͬ����ʵʱ����CharBase��Ϣ�������HP���ǣ���֯�Զ�ˢ����ʱ�ظ�����
	virtual VOID SendCharBaseDirectForHP();
protected:
	_MONSTER_ATTR_BACKUP	m_AttrBackUp;

//===========================================
// ����/�ķ�
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
// HP��MP��Rage�ı仯����
//===========================================
public:
	virtual BOOL		HeartBeat_Recover(UINT uTime = 0);

//===========================================
// Ч��
//===========================================
protected:
	virtual _IMPACT_LIST&	Impact_GetImpactList(VOID){return m_ImpactList;}

private:
	virtual VOID 			Impact_InitList(VOID);

private:
	_IMPACT_LIST			m_ImpactList;

//===========================================
// ����
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
	CMyTimer				m_RespawnTimer;		//ʬ����ʧ��������ʱ����
	FLOAT					m_RespawnDir;
	WORLD_POS				m_RespawnPos;

//===========================================
// �˺��б�
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
	
	// ���÷���ģʽ
	inline VOID				SetDistribMode(BYTE nMode){ m_DistribMode = nMode;}

	// ��÷���ģʽ
	inline const BYTE		GetDistribMode() const { return m_DistribMode;}

	// ������һ�η�����
	inline VOID				SetLastOwnerPos(BYTE nPos){ m_LastOwnerPos = nPos;}

	// �����һ�η�����
	inline const BYTE		GetLastOwnerPos() const { return m_LastOwnerPos;}

	// ����ӵ����team leader
	inline VOID				SetOwnTeamLeader(GUID_t nGuid){ m_OwnTeamLeaderGUID = nGuid;}

	// ���ӵ����team leader
	inline const GUID_t		GetOwnTeamLeader() const { return m_OwnTeamLeaderGUID;}
protected:
	TeamID_t				m_Own_TeamID ;//��npc�Ĺ���
	GUID_t					m_OccupantGUID ;//��npc�Ĺ���������m_Own_TeamID����INVALID_IDʱ��Ч��
	UINT					m_OccupantLevel; //��NPC������level
	DAMAGE_MEM_LIST			m_DamageMemList;
	MONSTER_OWNER_LIST		m_OwnerList;

	GUID_t					m_OwnTeamLeaderGUID ;//��npc�Ĺ�������ӳ�GUID
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
	Time_t					m_nOwnerElapsedTick;	//ӵ������ȥ��ʱ��
//===========================================
// NPC�̵�
//===========================================
public:
	DynamicShopManager*		GetShopManager(){ return m_pShopManager; }
	INT						BindDynamicShop(INT shopindex);
	VOID					InitDynamicShop();

protected:
	//��̬�̵������
	DynamicShopManager*		m_pShopManager;

//===========================================
// ���﹫���
//===========================================
public:
	PetPlacardSystem		*GetPetPlacardSystem(VOID){ return m_pPetPlacardSystem; }
	BOOL					CreatePetPlacardSystem(VOID);

protected:
	VOID					ReleasePetPlacardSystem(VOID);

protected:
	PetPlacardSystem		*m_pPetPlacardSystem;

//===========================================
// ����
//===========================================
public:
	BOOL					isPatrolMonster( VOID ) { return m_nPatrolID != INVALID_ID ? TRUE : FALSE; }
	INT						GetPatrolPathIndex( VOID ) const { return m_nPatrolID; }
	VOID					SetPatrolId( INT PatrolID );
	//VOID					StartPatrol( VOID );
	//VOID					StopPatrol( VOID );

	FLOAT					GetDropSearchRange() const;		//���������뾶
	INT						GetDropTeamCount() const;		//������Ч������
	VOID					SetCreateFlag(BOOL Flag){ m_CreateFlag = Flag; }
	
	INT						GetMonsterDropSetID(){ return m_MonsterDropItemSetID; }
	INT						GetMonsterDropRate(){ return m_nMonsterDropRate; }

	//��ȡ����ɵ���ĵ�(dropType��������)
	virtual	BOOL			GetNearCanDropPos(WORLD_POS& dropPos);
private:
	INT						m_nPatrolID;
	FLOAT					m_DropSearchRange;
	INT						m_DropTeamCount;
	BOOL					m_CreateFlag;
	INT						m_MonsterDropItemSetID; //������Ʒ��
	INT						m_nMonsterDropRate;		//�������
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
// һ������
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

	//��С��Ѫ�ٷֱ�
	INT						GetMinDamagePercent() const; 			
	VOID					SetMinDamagePercent( INT iPercent ){m_MinDamagePercent=iPercent;}

	UINT					GetGroupID(){ return m_uGroupID ; }
	UINT					GetTeamID(){ return m_uTeamID ; }

	INT						GetAttackAnimTime(){return m_AttackAnimTime;}
	INT						GetAttackCooldownTime(){return m_AttackCooldownTime;}

private:
	GUID_t					m_GUID;
	UINT					m_uDataID;			// ��ԴID
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
// ��������
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

	//��Ĭ����
	virtual INT 				GetHushAttResist(VOID);
	virtual INT 				GetBaseHushAttResist(VOID);
	virtual VOID				SetBaseHushAttResist(INT const nValue);

	//��е����
	virtual INT 				GetUnArmyAttResist(VOID);
	virtual INT 				GetBaseUnArmyAttResist(VOID);
	virtual VOID				SetBaseUnArmyAttResist(INT const nValue);

	//�����ٿ���
	virtual INT 				GetAttSpeedAttResist(VOID);
	virtual INT 				GetBaseAttSpeedAttResist(VOID);
	virtual VOID				SetBaseAttSpeedAttResist(INT const nValue);

	//�����ٿ���
	virtual INT 				GetSkillSpeedAttResist(VOID);
	virtual INT 				GetBaseSkillSpeedAttResist(VOID);
	virtual VOID				SetBaseSkillSpeedAttResist(INT const nValue);

	//�����ٿ���
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
	// ���������Գ�ʼ��
	//************************************
	VOID ClearSelf();
public:
	//************************************
	// Method:    HorseID
	// FullName:  Obj_Monster::HorseID
	// Access:    private 
	// Returns:   INT
	// Qualifier: const
	// �����ID
	//************************************
	INT GetHorseID() const { return m_iHorseID; }
	//************************************
	// Method:    HorseID
	// FullName:  Obj_Monster::HorseID
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT val
	// ������ID
	//************************************
	VOID SetHorseID(INT val) { m_iHorseID = val; }
	//************************************
	// Method:    WeaponID
	// FullName:  Obj_Monster::WeaponID
	// Access:    private 
	// Returns:   INT
	// Qualifier: const
	// �������ID
	//************************************
	INT GetWeaponID() const { return m_iWeaponID; }
	//************************************
	// Method:    WeaponID
	// FullName:  Obj_Monster::WeaponID
	// Access:    private 
	// Returns:   VOID
	// Qualifier:
	// Parameter: INT val
	// ��������ID
	//************************************
	VOID SetWeaponID(INT val) { m_iWeaponID = val; }
	//ÿ����������ı�����ԣ��ɹ����Լ�����
private:
	
	INT m_iAttackNear;		//���̹���
	INT m_iAttackFar;		//Զ�̹���

	INT m_iAttackMagicNear;	//�����ڹ�����
	INT m_iAttackMagicFar;	//�����⹦����

	INT m_iDefenceNear;		//���̷���
	INT m_iDefenceFar;		//Զ�̷���

	INT m_iDefenceMagicNear;//�����ڹ�����
	INT m_iDefenceMagicFar;//�����⹦����

	INT m_iHorseID;			//��ID
	INT m_iWeaponID;		//����ID
	INT m_iMaxHP;			//HP����
	INT m_iMaxMP;			//MP����
	INT m_iHPRestore;		//HP�ظ�
	INT m_iMPRestore;		//MP�ظ�
	INT m_iHitRate;			//������
	INT m_iMissRate;		//������
	INT m_iCriticalRate;	//������

	INT m_iAttackGold;		//�𹥻�
	INT m_iAttackWood;		//ľ����

	INT m_iAttackWater;		//ˮ����
	INT m_iAttackFire;		//�𹥻�
	INT m_iAttackSoil;		//������
	

	INT m_iDefenceGold;		//�����
	INT m_iDefenceWood;		//ľ����

	INT m_iDefenceWater;	//ˮ����
	INT m_iDefenceFire;		//�����
	INT m_iDefenceSoil;		//������
};

#endif
