//***************************************************************************************************
// 文件名称:	Obj_Character.h
// 系统名称:	
// 模块名称:	Obj_Character
// 功能说明:	
//
// 相关文档:	
// 作    者:	liam
// 建立日期:	
// 审    核:	
// 软件版权:	
//
//	兼做访问者模式中的元素类的基类，所有派生类可以根据需要实现具体Accept方法
//	因为角色类中有很多从逻辑上并不紧密相连的
//
//修改记录:
//          1.
//				修 改 人:liam
//				修改时间:2007/11/02
//				修改内容:属性第二部分重构(具体参见文档)
//***************************************************************************************************

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "OResultDef.h"
#include "DB_Struct.h"
#include "Attributes.h"
#include "SkillInfo.h"
#include "GameUtil.h"

#include "Obj_Dynamic.h"
#include "Obj_Special.h"
#include "ActionCallBack.h"
#include "ActionLogic.h"
#include "ActionDelegator.h"
#include "TargetingAndDepletingParams.h"

using namespace Combat_Module;
using namespace Action_Module;
using namespace Combat_Module::Skill_Module;

//延时生效技能缓冲区尺寸，_DELAY_TIME_IMPACT_LIST使用
const INT MAX_DELAY_TIME_IMPACT_NUM = 30;
const INT MAX_DELAY_TIME_SKILL_MISS_NUM = 15;
const INT MAX_HORSE_DATA_INDEX = 6;
enum ENUM_CHARACTER_LOGIC
{
	CHARACTER_LOGIC_INVALID	= -1,
	CHARACTER_LOGIC_IDLE,			//休息
	CHARACTER_LOGIC_MOVE,			//行动
	CHARACTER_LOGIC_USE_SKILL,		//战斗技能
	//CHARACTER_LOGIC_DEAD,
	//CHARACTER_LOGIC_SIT,
	CHARACTER_LOGIC_USE_ABILITY,	// 使用生活技能

	CHARACTER_LOGIC_NUMBERS
};
//物品特效
struct _ITEM_EFFECT
{
	BOOL			m_bActive ;
	_ITEM_VALUE		m_Attr ;

	_ITEM_EFFECT( )
	{
		m_bActive = FALSE ;
	};

	BOOL			IsActive( ){ return m_bActive ; }
	VOID			SetActive( BOOL bActive ){ m_bActive = bActive ; }
};
//技能特效
struct _SKILL_EFFECT
{
	BOOL			m_bActive ;

	_SKILL_EFFECT( )
	{
		m_bActive = FALSE ;
	};

	BOOL			IsActive( ){ return m_bActive ; }
	VOID			SetActive( BOOL bActive ){ m_bActive = bActive ; }
};

struct _OBJ_CHARACTER_INIT : public _OBJ_DYNAMIC_INIT
{
	ENUM_CHARACTER_LOGIC		m_eLogic;

	_OBJ_CHARACTER_INIT( VOID )
	{
		m_eLogic	= CHARACTER_LOGIC_INVALID;
	}

	virtual VOID CleanUp( VOID )
	{
		m_eLogic	= CHARACTER_LOGIC_INVALID;
		_OBJ_DYNAMIC_INIT::CleanUp();
	}
};

struct _LOGICPARAM_MOVE
{
	//INT			m_nHandleID;
	WORD		m_wNumNode;
	WORD		m_wCurNode;
	WORLD_POS	m_aTargetPos[MAX_CHAR_PATH_NODE_NUMBER];

	_LOGICPARAM_MOVE(VOID)
	{
		//m_nHandleID = 0;
		Reset();
	}

	VOID Reset( VOID )
	{
		m_wNumNode = 0;
		m_wCurNode = 0;
	}

	BOOL IsEmpty( VOID )const
	{
		return (( (m_wNumNode==0) || (m_wNumNode==m_wCurNode) )?(TRUE):(FALSE)) ;
	}

	VOID AddTargetPos( const WORLD_POS *pTargetPos )
	{
		if ( m_wNumNode < MAX_CHAR_PATH_NODE_NUMBER )
			m_aTargetPos[m_wNumNode++] = *pTargetPos;
	}

	const WORLD_POS *GetCurTargetPos( VOID )const
	{
		return &m_aTargetPos[m_wCurNode];
	}
	
	const WORLD_POS* GetFinalTarPos( VOID ) const
	{
		return &m_aTargetPos[m_wNumNode - 1];
	}
	// do ... 
	BOOL NextTargetPos( VOID )
	{
		if ( m_wCurNode < m_wNumNode )
			m_wCurNode++;

		if ( m_wCurNode < m_wNumNode )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
};
enum ENUM_REFESH_ATTR_TYPE
{
	REFESH_ATTR_TYPE_INVALID = -1,
	REFESH_ATTR_TYPE_NONE,
	REFESH_ATTR_TYPE_PUBLIC,
	REFESH_ATTR_TYPE_PUBLIC_AND_LEVEL,
	REFESH_ATTR_TYPE_PROTECTED,

	REFESH_ATTR_TYPE_NUMBERS
};

struct TEAMMEMBER;
class State;
class Scene;
class Obj_Human;
class AI_Character;
class CharacterVisitor;

class Obj_Character : public Obj_Dynamic
{
public :
	friend class State;
	friend class Action_Module::ActionDelegator_T;
public:
	enum CharacterClass
	{
		CHARACTER_CLASS_INVALID = -1,
		CHARACTER_CLASS_HUMAN,
		CHARACTER_CLASS_PET,
		CHARACTER_CLASS_MONSTER,
	};
	enum
	{
		RECOVER_INTERVAL = 5000,
	};
	//技能延时生效专用
	typedef struct DELAY_TIME_IMPACT
	{
		BYTE			m_Count;
		_OWN_IMPACT		m_aImpacts[MAX_DELAY_TIME_IMPACT_NUM];

		DELAY_TIME_IMPACT( )
		{
			CleanUp( );
		};
		VOID CleanUp( )
		{
			memset( this, 0, sizeof(DELAY_TIME_IMPACT) );
		}

		//将指定下标的元素从数组移除
		VOID ReplaseArrayItem( INT iIndex )
		{
			if( iIndex < 0 || iIndex >= m_Count )
				return;

			if( iIndex < m_Count-1 )
				memmove( &m_aImpacts[iIndex], &m_aImpacts[iIndex+1], sizeof(_OWN_IMPACT)*(m_Count-iIndex-1) );

			--m_Count;
		}

	} _DELAY_TIME_IMPACT_LIST;

public :
	Obj_Character( );
	~Obj_Character( );

	//************************************
	// Method:    Accept
	// FullName:  Obj_Character::Accept
	// Access:    public 
	// Returns:   VOID
	// Qualifier:
	// Parameter: const CharacterVisitor * pVisitor
	// 与角色访问者握手
	//************************************
	virtual VOID Accept( CharacterVisitor* pVisitor );

	virtual VOID OnRegisterToZone(VOID);

	virtual VOID	CleanUp( VOID );
	virtual BOOL	Init( const _OBJ_INIT *pInit );
	virtual VOID	OnEnterScene(VOID);
	virtual ObjType	GetObjType( VOID )const{ return OBJ_TYPE_INVALID; }

	virtual BOOL	HeartBeat( UINT uTime = 0 );
	virtual BOOL	HeartBeat_OutZone( UINT uTime=0 );
	virtual VOID	InitAIObj( );

// Event hanlder
public:
	virtual VOID OnDie( ObjID_t idKiller );
	virtual VOID OnRelive( ObjID_t idKiller );
	virtual VOID OnKillObject( ObjID_t idObj ){}
	virtual VOID OnItemChanged( UINT uItemDataID ){}
	virtual VOID OnEnterArea( UINT uAreaID ){}
	virtual VOID OnStealthLevelChanged(VOID);
	virtual VOID OnDetectLevelChanged(VOID);
	virtual VOID OnLevelChanged(VOID);
	//更新装备损伤点
	virtual VOID UpdataEquipDamagePoint(INT nType){}

	//处理同一个技能具有多种伤害的情况，调用具体ImpactLogic_T对象将每种类型伤害进行特定换算
	//然后把所有伤害类型的伤害值相加再调用Impact_OnDamage进行实际的伤害
	VOID OnDamages(INT const* const pDamagelist, ObjID_t nAttackerID, BOOL bCritical=FALSE, SkillID_t nSkillID=INVALID_ID, INT iLogicCount=INVALID_ID);
	//单一伤害类型的处理
	VOID OnDamage(INT nDamage, ObjID_t nAttackerID, BOOL bCritical=FALSE, SkillID_t nSkillID=INVALID_ID, INT iLogicCount=INVALID_ID);
	VOID OnDamageTarget(INT& nDamage, Obj_Character& rTar, SkillID_t nSkillID=INVALID_ID);
	INT  OnFiltrateImpact(OWN_IMPACT& rImpact);
	VOID RefixSkill(SkillInfo_T& rSkill);
	VOID RefixImpact(OWN_IMPACT& rImpactNeedRedix);
	VOID RefixRageRegeneration(INT& rRageRegeneration);
	VOID OnBeSkill(Obj_Character& rAttacker, INT nBehaviorType);
	VOID OnBeHit(Obj_Character& rAttacker);
	VOID OnHitTarget(Obj_Character& rTarget);
	VOID OnBeMiss(Obj_Character& rAttacker);	//当自己闪避成功
	VOID OnMissTarget(Obj_Character& rTarget);	//当攻击对方，对方闪避成功
	VOID OnCriticalHitTarget(SkillID_t const nSkill, Obj_Character& rTarget);
	VOID OnBeCriticalHit(SkillID_t const nSkill, Obj_Character& rAttacker);
	VOID OnHealTarget(SkillID_t const nSkill, Obj_Character& rTarget, INT nHealedHP);
	VOID OnBeHeal(SkillID_t const nSkill, Obj_Character& rCaster, INT nHealedHP);
	VOID OnBeAttack(Obj_Character& rAttacker);
	VOID OnBeAssisted(Obj_Character& rAssistor);
	VOID OnUseSkillSuccessfully(SkillInfo_T& rSkill);
	VOID OnActionStarted(VOID);
	VOID OnMyTrapActivated(Obj_Special& rTrap);
	VOID OnDepleteStrikePoints(INT nStrikePoints);

// 效果层处理
public:
	//获得当前效果列表中的效果个数
	INT			Impact_GetListSize(VOID)const;
	//处理角色身上的效果列表
	VOID 		Impact_HeartBeat(VOID);
	//使效果生效,如果是延时生效的技能则push到pool
	//如果迟时将为0则会直接调用Impact_RegisterImpactImp
	BOOL 		Impact_RegisterImpact(OWN_IMPACT& rImpact);
	//使用效果Sn取消效果列表中的效果
	BOOL 		Impact_UnRegisterImpactBySN(INT const nSn);
	//使用SkillIDAndImpactID取消效果列表中的效果
	BOOL 		Impact_UnRegisterImpactBySkillIDAndImpactID(SkillID_t const nSkillID, ImpactID_t const nImpactID);
	BOOL		Impact_UnregisterImpactByDataIndex( INT iDataIndex );
	VOID		Impact_CancelImpactInSpecificCollection(INT nCollectionID);
	//此特殊对象是否可见
	BOOL 		Impact_CanViewThisSpecialObj(Obj_Special& rObj);
	//查询效果列表中是否有特定的效果
	BOOL 		Impact_HaveImpactOfSpecificImpactID(INT const nImpactID);
	BOOL 		Impact_HaveImpactOfSpecificLogicID(INT const nLogicID);
	BOOL 		Impact_HaveImpactOfSpecificMutexID(INT const nMutexID);
	BOOL 		Impact_HaveImpactOfSpecificSkillID(INT const nSkillID);
	BOOL 		Impact_HaveImpactInSpecificCollection(INT const nCollectionID);
	//用不同的索引方法驱散指定的效果
	INT			Impact_DispelSpecificImpactByDataIndex(INT nDataIndex, INT nNeedDispel=-1);//-1是驱散所有的此类效果
	INT			Impact_DispelImpactOfSpecificImpactID(INT nImpactID, INT nNeedDispel=-1); //-1是驱散所有的此类效果
	INT			Impact_DispelImpactOfSpecificStand(ID_t nStandID, INT nNeedDispel=-1); //-1是驱散所有同影响性质的效果
	INT			Impact_DispelImpactOfSpecificMutexID(INT nMutexID, INT nNeedDispel=-1);//-1是驱散所有的此类效果
	INT			Impact_DispelImpactOfSpecificLogicID(INT nLogicID, INT nNeedDispel=-1);//-1是驱散所有的此类效果
	INT			Impact_DispelImpactOfSpecificSkillID(INT nSkillID, INT nNeedDispel=-1);//-1是驱散所有的此类效果
	INT			Impact_DispelImpactInSpecificCollection(INT nCollectionID, INT nDispelLevel, INT nNeedDispel=-1);//-1是驱散所有的此类效果
	//用不同的索引方法找到效果列表中的效果对象
	OWN_IMPACT* Impact_GetFirstImpactOfSpecificImpactID(INT nImpactID);
	OWN_IMPACT* Impact_GetFirstImpactOfSpecificLogicID(INT nLogicID);
	OWN_IMPACT* Impact_GetFirstImpactOfSpecificMutexID(INT nMutexID);
	OWN_IMPACT* Impact_GetFirstImpactOfSpecificSkillID(INT nSkillID);
	OWN_IMPACT* Impact_GetFirstImpactInSpecificCollection(INT nCollectionID);
	//指定的效果是否消散
	VOID	Impact_OnImpactFadeOut(OWN_IMPACT& rImp);
	//宠物死亡后消除此宠物造成的效果
	VOID	Impact_CleanAllImpactWhenPetDead(ObjID_t idPet);
	////获得先知或同类技能发送给角色对象的小球ID,在角色身上记录此ID，只做缓存和传递的用途
	//UINT	Impact_GetImpactUniqueID(){ return m_uUniqueIDForImpact; };
	////设置先知或同类技能发送给角色对象的小球ID,在角色身上记录此ID，只做缓存和传递的用途
	//VOID	Impact_SetImpactUniqueID( UINT uID ){ m_uUniqueIDForImpact = uID; };
	//先知类技能的小球是否可以向本对象投送
	//因为技能有延时生效所以在延时期间有可能重复投递，此方法负责判断延时期间的列表里
	//是否具有同类效果对象，如果有则FALSE, 如果没有则TRUE
	BOOL	Impact_IsCanReciveBallImpact( OWN_IMPACT& rImp );
	//存储未命中延时发送问题相关数据结构的访问方法
	_SkillMissMSGDelayTime* GetDelayTimeSkillMissList( INT iIndex );
	VOID AddDelayTimeSkillMissList( _SkillMissMSGDelayTime& val );

	//从某人身上偷技能
	VOID	Impact_StealImpact(Obj_Character& rTar, INT nCollectionID, INT nStandFlag, INT nStealCount, INT nRefixRate);

protected:
	INT 	Impact_OnFiltrateImpact(OWN_IMPACT& rImpact);
	//执行自身所有效果逻辑调用每一个效果对象的OnDamages
	VOID	Impact_OnDamages(INT* const pDamage, Obj_Character* const pAttacker, SkillID_t nSkillID=INVALID_ID);
	//执行nDamage指定的效果逻辑对象的OnDamage
	VOID 	Impact_OnDamage(INT& nDamage, Obj_Character* const pAttacker, SkillID_t nSkillID=INVALID_ID);
	VOID	Impact_OnDamageTarget(INT& nDamage, Obj_Character& rTar, SkillID_t nSkillID=INVALID_ID);
	VOID 	Impact_RefixSkill(SkillInfo_T& rSkill);
	VOID	Impact_RefixImpact(OWN_IMPACT& rImpactNeedRedix);
	VOID	Impact_RefixRageRegeneration(INT& rRageRegeneration);
	BOOL	Impact_GetIntAttrRefix(CharIntAttrRefixs_T::Index_T nIdx, INT& rIntAttrRefix);
	BOOL	Impact_GetBoolAttrRefix(CharBoolAttrs_T::Index_T nIdx, BOOL& rBoolAttrRefix);
	VOID	Impact_OnDie(VOID);
	VOID 	Impact_OnBeHit(Obj_Character& rAttacker);
	VOID 	Impact_OnHitTarget(Obj_Character& rTarget);
	VOID 	Impact_OnMissHit(Obj_Character& rAttacker);
	VOID 	Impact_OnMissTarget(Obj_Character& rTarget);
	VOID	Impact_OnCriticalHitTarget(SkillID_t const nSkill, Obj_Character& rTarget);
	VOID 	Impact_OnBeCriticalHit(SkillID_t const nSkill, Obj_Character& rAttacker);
	VOID 	Impact_OnHealTarget(SkillID_t const nSkill, Obj_Character& rTarget, INT& rHealedHP);
	VOID 	Impact_OnBeHeal(SkillID_t const nSkill, Obj_Character& rCaster, INT& rHealedHP);
	VOID	Impact_OnUseSkillSuccessfully(SkillInfo_T& nSkill);
	VOID	Impact_OnActionStarted(VOID);
private:
	//使效果生效，调用具体效果对象的OnActive
	VOID	Impact_OnImpactActived(OWN_IMPACT& rImp);
	VOID Impact_SendImpactMsg(OWN_IMPACT& rImp, Time_t iDelayTime );
	BOOL 	Impact_RemoveImpactByIndex(INT const nIdx);
	//使效果真正生效,Impact_RegisterImpact将延时生效的技能则push到pool
	//如果迟时将为0则会直接调用此方法
	VOID 	Impact_RegisterImpactImp(OWN_IMPACT& rImp);
	//是否可以用rImpactIn替换自身效果列表中的效果
	//TRUE为成功替换
	BOOL 	Impact_ReplaceMutuallyExclusiveImpact(OWN_IMPACT& rImpactIn);
	VOID	Impact_ReplaceLastGoodImpact(OWN_IMPACT& rImpactIn);
	BOOL	Impact_AddNewImpact(OWN_IMPACT& rImpactIn);
	//使rDest消散,使rSrc生效
	VOID	Impact_ReplaceImpact(OWN_IMPACT& rDest, OWN_IMPACT& rSrc);
	VOID	Impact_CleanupAllFadeOutImpacts(VOID);
	//如果效果有延迟时间则推入效果池，等时间到再处理
	//如果延迟时间为0则立即处理，使效果生效
	VOID	Impact_PushToDelayTimeList( OWN_IMPACT& oImpact );
	//延时技能生效处理
	VOID	ImpactDelayTimeHeartBeat();
private:
	virtual VOID 	Impact_InitList(VOID);
	UINT 	m_nSNSeedOfImpacts;
//持续性效果
protected:
	UINT					Impact_GetSNSeed(VOID) const{return m_nSNSeedOfImpacts;};
	VOID					Impact_SetSNSeed(INT const nValue) {m_nSNSeedOfImpacts=nValue;};
	VOID					Impact_AddSNSeed(VOID){++m_nSNSeedOfImpacts;};
	virtual _IMPACT_LIST&	Impact_GetImpactList(VOID)=0;
private:
	_DELAY_TIME_IMPACT_LIST	m_DelayTimeImpactList;
	_SkillMissMSGDelayTime	m_DelayTimeSkillMissList[MAX_DELAY_TIME_SKILL_MISS_NUM];
	
///////////////////////////////////////////////////////////////////////////////////
// 技能层和战斗相关
public:
	Obj* 			GetMyShadowGuard(VOID);
	Obj* 			GetMyPet(INT iIndex);		//获取角色宠物
	Obj* 			GetMyMaster(VOID);			//宠物获取主人
	VOID			GM_killObj(VOID);
	VOID 			GM_ForceDie(Obj_Character* pKiller);
	VOID			HealthIncrement(INT nIncrement, INT iLogicCount=INVALID_ID, Obj_Character* pCharacter=NULL, BOOL bCritical=FALSE, BOOL bDOT=FALSE);
	VOID			ManaIncrement(INT nIncrement, INT iLogicCount=INVALID_ID, Obj_Character* pCharacter=NULL);
	VOID			PneumaIncrement(INT nIncrement, INT iLogicCount=INVALID_ID, Obj_Character* pCharacter=NULL);
	VOID			RageIncrement(INT nIncrement, INT iLogicCount=INVALID_ID, Obj_Character* pCharacter=NULL);
	VOID			StrikePointIncrement(INT nIncrement, Obj_Character* pCharacter=NULL);

	virtual Time_t	GetCooldownRemained(CooldownID_t const nID) const {return 0;};
	virtual VOID	SetCooldown(CooldownID_t const nID, Time_t nCooldown){};
	virtual BOOL	IsCooldowned(CooldownID_t const nID) const {return FALSE;}
	virtual BOOL 	HeartBeat_Cooldown(UINT uTime = 0){return TRUE;};
	virtual BOOL	HeartBeat_Recover(UINT uTime = 0){return TRUE;};
	BOOL			HeartBeat_Action(UINT uTime = 0);
	BOOL			HeartBeat_AutoRepeatCooldown(UINT uTime = 0);
	ObjID_t 		GetLockedTarget(VOID) const {return m_nLockedTargetID;};
	VOID			SetLockedTarget(ObjID_t const nID){m_nLockedTargetID = nID;};
	BOOL			CanUseSkillNow(SkillID_t nID=INVALID_ID);

	Time_t			GetActionTime(VOID) const {return m_nActionTime;};
	VOID			SetActionTime(Time_t nTime) {m_nActionTime=nTime;};
	Time_t			GetAutoRepeatCooldown(VOID) const {return m_nAutoRepeatCooldown;};
	VOID			SetAutoRepeatCooldown(Time_t nCooldown){m_nAutoRepeatCooldown = nCooldown;};
	BOOL			DepletingUsedItem(VOID);
	SkillInfo_T&	GetSkillInfo(VOID) {return m_SkillInfo;};
	VOID			SetSubSkillID( INT iIndex, INT iSkill ){ m_aSubSkill[iIndex] = iSkill; }
	TargetingAndDepletingParams_T& GetTargetingAndDepletingParams(VOID) {return m_TargetingAndDepletingParams;};
	////add by gh for souxia 2010/05/20
	//VOID			PetZhaoHuanTimesIncrement(INT nIncrement, INT iLogicCount, Obj_Character* pCharacter, SkillID_t skillId);
	//VOID			ZuoJiZhaoHuanTimesIncrement(INT nIncrement, INT iLogicCount, Obj_Character* pCharacter, SkillID_t skillId);

protected:
	Time_t	m_nElapsedRevocerTick;									//已经逝去的恢复时间
	//Time_t	m_nElapsedRageRevocerTick;
	Time_t	m_nElapsedPneumaRevocerTick;
protected:
	VOID			SubSkillAutoUseHeartBeat();
	VOID			DelayTimeSkillMissHeartBeat();
	//为技能同步，实时发送CharBase消息，并清除HP脏标记，组织自动刷属性时重复发送
	virtual VOID	SendCharBaseDirectForHP(){}
	VOID			SendHealAndDamageMSG( INT nIncrement, Obj_Character* pCharacter, BOOL bCritical, INT iLogicCount );
	VOID			OnIncementHp( INT nIncrement, Obj_Character* pCharacter );
	VOID			OnDecementHP( INT nIncrement, BOOL bDOT, Obj_Character* pCharacter );
private:
	ObjID_t m_nLockedTargetID;										//锁定的目标 
	SkillInfo_T m_SkillInfo;										//技能实例
	TargetingAndDepletingParams_T m_TargetingAndDepletingParams;	//目标和消耗数据
	Time_t	m_nActionTime;											//配合客户端作动作演示的时间控制，时间不为零时不要进行下一个动作。相当于身体僵直不能动作.
	Time_t  m_nAutoRepeatCooldown;
	INT		m_aSubSkill[SkillTemplateData_T::MAX_SUB_SKILL];		//待发放的子技能列表
//技能层和心法
public:
	VOID			Skill_ExecFromItemScript(SkillID_t const nID);
	VOID			Skill_ExecFromScript(SkillID_t const nID, ObjID_t nTargetID, WORLD_POS& pos, FLOAT fDir, BOOL bPassSkillCheck);
	virtual BOOL 	Skill_HaveSkill( SkillID_t const nID ) const{return TRUE;};
	virtual BOOL 	Skill_ModifySkill(SkillID_t	const nID, INT const nModifyFlag){return TRUE;};
	virtual BOOL 	Skill_HaveXinFa( SkillID_t const nID ) const{return TRUE;};
	virtual INT 	Skill_GetXinFaLevel(SkillID_t const nID) const{return 1;};
	virtual BOOL 	Skill_SetXinFaLevel(SkillID_t const nID, INT const nLevel){return TRUE;};
	virtual VOID 	Skill_OnEquipItem(VOID){};
	//如果此技能需要冷却状态则判断是否已冷却，否则忽略冷却影响
	BOOL 			Skill_IsSkillCooldowned(SkillID_t nID);
	BOOL			Skill_CanUseThisSkillInThisStatus(SkillID_t nSkillID);
	Obj* 			Skill_CreateObjSpecial(WORLD_POS const& rPosition, INT nDataID);
	BOOL			Skill_RefixSkill(SkillInfo_T& rSkillInfo);
	Obj_Character*	CreateGuard(UINT uDataID, INT nBaseAI = 0, INT nExtAI = 0);
	Obj_Character*	GetGuard(VOID) const { return m_pGuard; }
	VOID			SetGuard(Obj_Character* pGuard) { m_pGuard = pGuard; }
	//WORLD_POS规格化
	VOID  NormalizeWorldPos( WORLD_POS& sPos ) const;
	FLOAT WordPosLength( FLOAT fX, FLOAT fY ) const;
    INT             CalRelationPlus(BOOL bHuman_A, BOOL bHuman_B, Obj_Human *pHumanA, Obj_Human *pHumanB, const _CAMP_DATA *pCamp_A, const _CAMP_DATA *pCamp_B);

protected:
	VOID  BeatBack( Obj_Character* pAttacker, INT iMyHPValue, VOID* pMsg, const SkillTemplateData_T* pSkillTemplate );
	VOID  BeatFly( Obj_Character* pAttacker, INT iMyHPValue, VOID* pMsg, const SkillTemplateData_T* pSkillTemplate );
	//是否被击飞
	BOOL  IsBeatFly( Obj_Character& rMe, Skill_Module::SkillTemplateData_T const* pTemplate) const;
	//是否被击退
	BOOL  IsBeatBack( Obj_Character& rMe, Skill_Module::SkillTemplateData_T const* pTemplate ) const;
	//根据技能属性中的概率和fRate进行比较，看是否会发生
	BOOL  IsHappenByRate( FLOAT fRate, Obj_Character& rMe ) const;
	//根据格子信息计算击退的位置
	VOID  CalculateBeatBackPos( Obj_Character& rMe, Obj* rTag, SkillTemplateData_T const* pTemplate, WORLD_POS& sPos ) const;
	//计算长度
	FLOAT KLU_GetYAngle(const WORLD_POS& fvPos1, const WORLD_POS& fvPos2);
	VOID  OnSubSkill( SkillInfo_T& rSkillInf );
	BOOL  IsSubSkillCanSuccess( INT iRate );


private:
	Obj_Character*	m_pGuard; // 分身指针成员

// 动作层
// 动作管理和规则在这里实现
public:
	//动作的处理委托给ActionDelegator_T处理
protected:
	ActionParams_T&	GetActionParams(VOID) {return m_ActionParams;}
	ActionLogic_T const* GetActionLogic(VOID) {return m_pActionLogic;}
	VOID SetActionLogic(ActionLogic_T const* pLogic) {m_pActionLogic = pLogic;}
private:
	ActionParams_T			m_ActionParams;
	ActionLogic_T const* 	m_pActionLogic;
// 门派特性的处理
public:
	VOID MenPai_OnDamage(INT& nDamage);
	VOID MenPai_OnDamageTarget(Obj_Character& rTar, INT& nDamage);
	VOID MenPai_OnMyTrapActivated(Obj_Special& rTrap);
	VOID MenPai_OnHealTarget(INT nHealed);
	VOID MenPai_OnDepleteStrikePoints(INT nStrikePoints);
protected:
private:

// 常用属性和标记
public:
	//Obj_Character id 是当前角色在CharacterManager类中的数组下标，其值由
	//CharacterManager统一负责处理
	VOID	SetCharacterID( ObjID_t id ){ m_CharacterID = id ; }
	ObjID_t	GetCharacterID( ){ return m_CharacterID ; }

	// 获取AI数据
	AI_Character*	GetAIObj( VOID ) const {  return m_pAI_Character; }
// Flags
public:
	//camp and stand
	BOOL			IsEnemyCamp(Obj_Character& rTar);
	BOOL			IsFriendCamp(Obj_Character& rTar);
	virtual BOOL	IsEnemy(Obj_Character* pCharacter){ return TRUE;}
	virtual BOOL	IsFriend(Obj_Character* pCharacter){return FALSE;}
	virtual BOOL	IsPartner(Obj_Character* pCharacter){return FALSE;}
	virtual BOOL	IsPrevCanViewMe( const Obj *pObj );
	virtual BOOL	IsCanViewMe( const Obj *pObj );
	BOOL			InStealthMode(VOID);
// logic
public:
	ENUM_CHARACTER_LOGIC	GetCharacterLogic( VOID )const{ return m_eCharacterLogic; }
	BOOL					IsCharacterLogicStopped( VOID )const{ return m_bCharacterLogicStopped; }
	VOID					StopCharacterLogic(BOOL bAbortive = TRUE);
	BOOL					IsMoving( );
	const WORLD_POS			*GetCurrentTargetPos( VOID )const{ return m_paramLogic_Move.GetCurTargetPos(); }
	const WORLD_POS			*GetFinalTarPos( VOID )const{ return m_paramLogic_Move.GetFinalTarPos(); }

	VOID					DoAction(INT nActionID, ObjID_t nTargetObjID = INVALID_ID);

	//INT						GetMoveHandleID( VOID )const{ return m_paramLogic_Move.m_nHandleID; }

protected:
	// 禁止开放
	VOID					SetCharacterLogic( ENUM_CHARACTER_LOGIC eLogic );

	virtual VOID			OnCharacterLogicStopped(BOOL bAbortive);

public:
	VOID					FaceTo( const WORLD_POS* pPos );
	VOID					FaceTo( const Obj* pObj );
	BOOL					IsLimitMove( VOID );			// 是否限制了移动操作
	VOID					DirectMoveTo(const WORLD_POS* pTar) ;
	//瞬移
	VOID					DirectTo( const WORLD_POS* pTar, BOOL bMove=TRUE );
	/*
	BOOL					IsLimitUseSkill( VOID );		// 是否限制了使用技能的操作
	BOOL					IsLimitAttack( VOID );			// 是否限制了攻击
	BOOL					IsLimitAllHandle( VOID );		// 是否限制了所有操作
	*/

//=========================================================================================
// 以下行为、逻辑只能由Obj自身或AI才能调用(禁止开放!!!) --frankwu 06/03/16
//=========================================================================================
private:
	ORESULT					Do_Idle( VOID );
	ORESULT					Do_Jump( VOID );
	ORESULT					Do_Stop( VOID );
	ORESULT					Do_Move( INT nHandleID, WORD wNumTargetPos, const WORLD_POS *paTargetPos, BOOL bFlag=FALSE );
	ORESULT					Do_UseSkill( SkillID_t idSkill, ObjID_t idTarget, const WORLD_POS *pTargetPos, INT iHurtDelayTime, FLOAT fDir, GUID_t guidTarget);

	VOID					MoveTo( INT nHandleID, WORLD_POS* pTar ) ;
	VOID					MoveTo( INT nHandleID, WORLD_POS* pCur, WORLD_POS* pTar ) ;

private:
	BOOL					Obj_Logic_Idle( UINT uTime );
	BOOL					Obj_Logic_Move( UINT uTime );
	BOOL					Obj_Logic_UseSkill( UINT uTime );

//=========================================================================================
// 以下两个成员函数为生活技能服务，放在这里不太适合，建议与技能系统合并 --frankwu 06/03/16
//=========================================================================================
protected:
	virtual ORESULT			Do_UseAbility( VOID ) { return OR_OK; }
	virtual BOOL			Obj_Logic_UseAbility( UINT uTime ) { return TRUE; }

///////////////////////////////////////////////////////////////////////////////////////////

//Sync
public:
	virtual VOID			SyncTeamMemberInfo( ) {}
	virtual VOID			SendMsg_RefeshAttrib( VOID );
	virtual VOID			AskMsg_BaseAttrib( Obj_Human *pTargetHuman );

//=========================================================================================
// 可见性
//=========================================================================================
public:
	virtual VOID			UpdateViewCharacter(VOID);

	INT						GetPrevLevel(VOID)const{return m_nView_PrevLevel;}
	INT						GetPrevStealthLevel(VOID)const{return m_nView_PrevStealthLevel;}
	INT						GetPrevDetectLevel(VOID)const{return m_nView_PrevDetectLevel;}

protected:
	INT			m_nView_PrevLevel;					// 等级
	INT			m_nView_PrevStealthLevel;			// 隐身等级
	INT			m_nView_PrevDetectLevel;			// 反隐等级
	CMyTimer	m_UpdateViewCharacterTimer;			// 隐身变化的心跳间隔

//=========================================================================================
// 复活点相关
//=========================================================================================
public:

	struct _RELIVE_INFO
	{
		// HP
		BOOL		m_bHPUsePercent;
		union
		{
			INT			m_nHP;
			INT			m_nHPPercent;
		};

		// MP
		BOOL		m_bMPUsePercent;
		union
		{
			INT			m_nMP;
			INT			m_nMPPercent;
		};

		// SP---另一种怒气叫法。。以后再改rage吧。
		BOOL		m_bSPUsePercent;
		union
		{
			INT			m_nSP;
			INT			m_nSPPercent;
		};

		// 复活点
		SceneID_t		m_SceneID;
		WORLD_POS		m_Pos;

		_RELIVE_INFO( VOID )
		{
			m_bHPUsePercent		= FALSE;
			m_nHP				= 0;
			m_bMPUsePercent		= FALSE;
			m_nMP				= 0;
			m_bSPUsePercent		= FALSE;
			m_nSP				= 0;
			m_SceneID			= INVALID_ID;
			m_Pos				= WORLD_POS( -1.f, -1.f );
		}

		VOID Reset( VOID )
		{
			m_bHPUsePercent		= FALSE;
			m_nHP				= 0;
			m_bMPUsePercent		= FALSE;
			m_nMP				= 0;
			m_bSPUsePercent		= FALSE;
			m_nSP				= 0;
			m_SceneID			= INVALID_ID;
			m_Pos				= WORLD_POS( -1.f, -1.f );
		}
	};

public:
	// 是否可以技能使用技能复活
	BOOL					IsCanRelive( VOID )const;

	// 得到复活信息
	// bSkillRelive		:	是否为技能复活
	virtual const _RELIVE_INFO		*GetReliveInfo( BOOL bSkillRelive )const;

	// 设置复活属性
	// bSkillRelive		:	是否为技能复活
	// pReliveInfo		:	复活信息
	virtual VOID			SetReliveInfo( BOOL bSkillRelive, const _RELIVE_INFO *pReliveInfo );

	// 重置复活的信息
	VOID					ResetReliveInfo( VOID );

	//获取最近可掉落的点
	virtual BOOL			GetNearCanDropPos(WORLD_POS& dropPos);
protected:
	BOOL					m_bCanSkillRelive;			// 是否有技能复活
	_RELIVE_INFO			m_SkillReliveInfo;			// 技能复活的信息


public:
	virtual	VOID			SetName( const CHAR *pszName )=0;
	virtual const CHAR		*GetName( VOID )const=0;

	virtual	VOID			SetTitle( const CHAR *pszTitle )=0;
	virtual const CHAR		*GetTitle( VOID )const=0;

	virtual VOID			SetHP( INT nHP )=0;
	virtual INT				GetHP( VOID )=0;

	virtual VOID			SetMP( INT nMP )=0;
	virtual INT				GetMP( VOID )=0;

	virtual VOID			SetPneuma( INT nPneuma ){}
	virtual INT				GetPneuma( VOID ){return 0;}

	virtual VOID			SetLevel( INT nLevel )=0;
	virtual INT				GetLevel( VOID )const=0;

	virtual UINT			GetExp( VOID ){ return 0; } 
	
	virtual INT				GetStrikePoint(VOID) {return 0;}
	virtual VOID			SetStrikePoint(INT nStrikePoint) {}

	virtual INT				GetVigor(VOID) {return 0;}
	virtual VOID			SetVigor(INT nVigor) {}

	virtual INT				GetMaxVigor(VOID) {return 1;}
	virtual VOID			SetMaxVigor(INT nMaxVigor) {}

	virtual	INT				GetVigorRegeneRate(VOID){ return 0; }
	virtual	VOID			SetVigorRegeneRate(INT nRate){}

	virtual INT				GetEnergy(VOID) {return 0;}
	virtual VOID			SetEnergy(INT nVigor) {}

	virtual INT				GetMaxEnergy(VOID) {return 1;}
	virtual VOID			SetMaxEnergy(INT nMaxVigor) {}

	virtual	INT				GetEnergyRegeneRate(VOID){ return 0; }
	virtual	VOID			SetEnergyRegeneRate(INT nRate){}

	virtual INT 			GetRage( VOID )=0;
	virtual VOID			SetRage( INT nRage )=0;

	virtual INT				GetMoodState(VOID)const{ return m_nMoodState; }
	virtual VOID			SetMoodState(INT nState){ m_nMoodState = nState; }

	virtual UINT			GetDataID( VOID )const{ return UINT_MAX; }

	virtual FLOAT			GetDropSearchRange() const {return 20.0f;}  //掉落搜索半径
	virtual INT				GetDropTeamCount() const {return 1;}		//掉落有效组数量
	virtual INT				GetMinDamagePercent() const {return 20;};	//最小伤血百分比

	// add by gh for souxia 2010/05/21
	virtual VOID			SetPetZhaoHuanSkillTimes(SkillID_t skillId, SHORT iDeplete){}//设置神兽召唤技能次数
	virtual SHORT			GetPetZhaoHuanSkillTimes(SkillID_t skillId){return 0;}		//获取神兽召唤技能次数
	
	virtual VOID			SetZuoJiZhaoHuanSkillTimes(SkillID_t skillId, SHORT iDeplete){}//设置坐骑召唤技能次数
	virtual SHORT			GetZuoJiZhaoHuanSkillTimes(SkillID_t skillId){return 0;}	//获取坐骑召唤技能次数

protected:
	INT						m_nMoodState;	// 表情状态

public:
//物品部分虚接口
	virtual	BOOL			IsCarrayItem(UINT ItemType,UINT ItemCount)	{ return TRUE;}
	virtual _ITEM_VALUE		ItemValue( BYTE AttrType )
	{//AttrType=enum ITEM_ATTRIBUTE 
		_ITEM_VALUE a ;
		a.CleanUp() ;
		return a ;
	};

///////////////////////////////////////////////////////////////////////////////////////
// 改变速度部分
public:
	enum ENUM_MOVE_MODE{
		MOVE_MODE_HOBBLE = 0,	// 蹒跚
		MOVE_MODE_WALK,			// 走	
		MOVE_MODE_RUN,			// 跑
		MOVE_MODE_SPRINT,		// 疾跑
	};

	VOID SetMoveMode(ENUM_MOVE_MODE eMoveMode) { m_eMoveMode = eMoveMode; }
    ENUM_MOVE_MODE GetMoveMode(VOID) const { return m_eMoveMode; }

private:
	ENUM_MOVE_MODE m_eMoveMode;

///////////////////////////////////////////////////////////////////////////////////////
protected :
	CMyTimer				m_SendMsgTimer ;	// 发送网络消息的计时
	BOOL					m_bSendMsgDirty;	// 是否发送过网络消息

	CMyTimer				m_AttrMsgTimer ;				// 属性刷新消息的计时
	BYTE					m_AttrMsgToTeamIntervalCount;	// 属性刷新消息刷新到队员的间隔次数
	BYTE					m_PublicAttrVersion;			// 公开属性的版本
	BYTE					m_ProtectedAttrVersion;			// 限制属性的版本

protected :
	ObjID_t					m_CharacterID ;		// 角色ID
	CMyTimer				m_RecoverTimer ;	// 回复时间控制器
	_LOGICPARAM_MOVE		m_paramLogic_Move;
	ENUM_CHARACTER_LOGIC	m_eCharacterLogic;
	BOOL					m_bCharacterLogicStopped;

	AI_Character*			m_pAI_Character;	//AI数据

public:
	virtual ObjID_t			GetOwnerID( VOID ) const { return INVALID_ID; }
	virtual Obj_Character	*GetOwner( VOID ) { return NULL; }

protected :
	_CAMP_DATA				m_CampDataRefix;

//角色属性部分
public:
	//StrikePoint
	virtual INT 	GetMaxStrikePoint(VOID)		=	0;
	INT				GetBaseMaxStrikePoint(VOID) {return BASE_MAX_STRIKE_POINT;};
	INT				GetMaxStrikePointRefix(VOID);
	VOID			SetMaxStrikePointRefix(INT const nValue);
	BOOL			GetMaxStrikePointDirtyFlag(VOID);
	VOID			ClearMaxStrikePointDirtyFlag(VOID);
	VOID			MarkMaxStrikePointDirtyFlag(VOID);
	BOOL			GetMaxStrikePointRefixDirtyFlag(VOID);
	VOID			ClearMaxStrikePointRefixDirtyFlag(VOID);
	VOID			MarkMaxStrikePointRefixDirtyFlag(VOID);
	//Rage
	virtual INT   	GetMaxRage(VOID)=0;
	virtual INT		GetBaseMaxRage(VOID)=0;
	virtual VOID 	SetBaseMaxRage(INT const nRage)=0;
	INT				GetMaxRageRefix(VOID);
	VOID			SetMaxRageRefix(INT const nValue);
	BOOL			GetMaxRageDirtyFlag(VOID);
	VOID			ClearMaxRageDirtyFlag(VOID);
	VOID			MarkMaxRageDirtyFlag(VOID);
	BOOL			GetMaxRageRefixDirtyFlag(VOID);
	VOID			ClearMaxRageRefixDirtyFlag(VOID);
	VOID			MarkMaxRageRefixDirtyFlag(VOID);

	virtual INT		GetRageRegenerate(VOID)=0; 
	virtual INT 	GetBaseRageRegenerate(VOID)=0;
	virtual VOID 	SetBaseRageRegenerate(INT const nValue)=0;
	INT 			GetRageRegenerateRefix(VOID);
	VOID 			SetRageRegenerateRefix(INT const nValue);
	BOOL			GetRageRegenerateDirtyFlag(VOID);
	VOID			ClearRageRegenerateDirtyFlag(VOID);
	VOID			MarkRageRegenerateDirtyFlag(VOID);
	BOOL			GetRageRegenerateRefixDirtyFlag(VOID);
	VOID			ClearRageRegenerateRefixDirtyFlag(VOID);
	VOID			MarkRageRegenerateRefixDirtyFlag(VOID);

	//Attr1: Str
	virtual INT		GetStr(VOID)=0;
	virtual INT		GetBaseStr(VOID)=0;
	virtual VOID 	SetBaseStr(INT const nValue)=0;
	INT				GetStrRefix(VOID);
	VOID			SetStrRefix(INT const nValue);
	BOOL			GetStrDirtyFlag(VOID);
	VOID			ClearStrDirtyFlag(VOID);
	VOID			MarkStrDirtyFlag(VOID);
	BOOL			GetStrRefixDirtyFlag(VOID);
	VOID			ClearStrRefixDirtyFlag(VOID);
	VOID			MarkStrRefixDirtyFlag(VOID);

	//Attr1: Con
	virtual INT		GetCon(VOID)=0;
	virtual INT		GetBaseCon(VOID)=0;
	virtual VOID	SetBaseCon(INT const nValue)=0;
	INT				GetConRefix(VOID);
	VOID			SetConRefix(INT const nValue);
	BOOL			GetConDirtyFlag(VOID);
	VOID			ClearConDirtyFlag(VOID);
	VOID			MarkConDirtyFlag(VOID);
	BOOL			GetConRefixDirtyFlag(VOID);
	VOID			ClearConRefixDirtyFlag(VOID);
	VOID			MarkConRefixDirtyFlag(VOID);
	//Attr1: Int
	virtual INT		GetInt(VOID)=0;
	virtual INT		GetBaseInt(VOID)=0;
	virtual VOID	SetBaseInt(INT const nValue)=0;
	INT				GetIntRefix(VOID);
	VOID			SetIntRefix(INT const nValue);
	BOOL			GetIntDirtyFlag(VOID);
	VOID			ClearIntDirtyFlag(VOID);
	VOID			MarkIntDirtyFlag(VOID);
	BOOL			GetIntRefixDirtyFlag(VOID);
	VOID			ClearIntRefixDirtyFlag(VOID);
	VOID			MarkIntRefixDirtyFlag(VOID);
	//Attr1: Dex
	virtual INT		GetDex(VOID)=0;
	virtual INT		GetBaseDex(VOID)=0;
	virtual VOID	SetBaseDex(INT const nValue)=0;
	INT				GetDexRefix(VOID);
	VOID			SetDexRefix(INT const nValue);
	BOOL			GetDexDirtyFlag(VOID);
	VOID			ClearDexDirtyFlag(VOID);
	VOID			MarkDexDirtyFlag(VOID);
	BOOL			GetDexRefixDirtyFlag(VOID);
	VOID			ClearDexRefixDirtyFlag(VOID);
	VOID			MarkDexRefixDirtyFlag(VOID);
	//HP
	virtual INT		GetMaxHP(VOID) =0; 
	virtual INT 	GetBaseMaxHP(VOID)=0;
	virtual VOID 	SetBaseMaxHP(INT const nHp)=0;
	INT 			GetMaxHPRefix(VOID);
	VOID 			SetMaxHPRefix(INT const nHp);
	BOOL			GetMaxHPDirtyFlag(VOID);
	VOID			ClearMaxHPDirtyFlag(VOID);
	VOID			MarkMaxHPDirtyFlag(VOID);
	BOOL			GetMaxHPRefixDirtyFlag(VOID);
	VOID			ClearMaxHPRefixDirtyFlag(VOID);
	VOID			MarkMaxHPRefixDirtyFlag(VOID);

	virtual INT		GetHPRegenerate(VOID) =0; 
	virtual INT 	GetBaseHPRegenerate(VOID)=0;
	virtual VOID 	SetBaseHPRegenerate(INT const nValue)=0;
	INT 			GetHPRegenerateRefix(VOID);
	VOID 			SetHPRegenerateRefix(INT const nValue);
	BOOL			GetHPRegenerateDirtyFlag(VOID);
	VOID			ClearHPRegenerateDirtyFlag(VOID);
	VOID			MarkHPRegenerateDirtyFlag(VOID);
	BOOL			GetHPRegenerateRefixDirtyFlag(VOID);
	VOID			ClearHPRegenerateRefixDirtyFlag(VOID);
	VOID			MarkHPRegenerateRefixDirtyFlag(VOID);

	//MP
	virtual INT 	GetMaxMP(VOID)=0;
	virtual INT 	GetBaseMaxMP(VOID)=0;
	virtual VOID 	SetBaseMaxMp(INT const nMp)=0;
	INT 			GetMaxMPRefix(VOID);
	VOID			SetMaxMPRefix(INT const nMp);
	BOOL			GetMaxMPDirtyFlag(VOID);
	VOID			ClearMaxMPDirtyFlag(VOID);
	VOID			MarkMaxMPDirtyFlag(VOID);
	BOOL			GetMaxMPRefixDirtyFlag(VOID);
	VOID			ClearMaxMPRefixDirtyFlag(VOID);
	VOID			MarkMaxMPRefixDirtyFlag(VOID);

	virtual INT		GetMPRegenerate(VOID)=0; 
	virtual INT 	GetBaseMPRegenerate(VOID)=0;
	virtual VOID 	SetBaseMPRegenerate(INT const nValue)=0;
	INT 			GetMPRegenerateRefix(VOID);
	VOID 			SetMPRegenerateRefix(INT const nValue);
	BOOL			GetMPRegenerateDirtyFlag(VOID);
	VOID			ClearMPRegenerateDirtyFlag(VOID);
	VOID			MarkMPRegenerateDirtyFlag(VOID);
	BOOL			GetMPRegenerateRefixDirtyFlag(VOID);
	VOID			ClearMPRegenerateRefixDirtyFlag(VOID);
	VOID			MarkMPRegenerateRefixDirtyFlag(VOID);

	//Pneuma
	virtual INT 	GetMaxPneuma(VOID)=0;
	virtual INT 	GetBaseMaxPneuma(VOID)=0;
	virtual VOID 	SetBaseMaxPneuma(INT const nPneuma)=0;
	INT 			GetMaxPneumaRefix(VOID);
	VOID			SetMaxPneumaRefix(INT const nPneuma);
	BOOL			GetMaxPneumaDirtyFlag(VOID);
	VOID			ClearMaxPneumaDirtyFlag(VOID);
	VOID			MarkMaxPneumaDirtyFlag(VOID);
	BOOL			GetMaxPneumaRefixDirtyFlag(VOID);
	VOID			ClearMaxPneumaRefixDirtyFlag(VOID);
	VOID			MarkMaxPneumaRefixDirtyFlag(VOID);

	virtual INT		GetPneumaRegenerate(VOID)=0; 
	virtual INT 	GetBasePneumaRegenerate(VOID)=0;
	virtual VOID 	SetBasePneumaRegenerate(INT const nValue)=0;
	INT 			GetPneumaRegenerateRefix(VOID);
	VOID 			SetPneumaRegenerateRefix(INT const nValue);
	BOOL			GetPneumaRegenerateDirtyFlag(VOID);
	VOID			ClearPneumaRegenerateDirtyFlag(VOID);
	VOID			MarkPneumaRegenerateDirtyFlag(VOID);
	BOOL			GetPneumaRegenerateRefixDirtyFlag(VOID);
	VOID			ClearPneumaRegenerateRefixDirtyFlag(VOID);
	VOID			MarkPneumaRegenerateRefixDirtyFlag(VOID);

	//CampID
	virtual const _CAMP_DATA	*GetCampData(VOID);
	virtual const _CAMP_DATA	*GetBaseCampData(VOID)const=0;
	virtual VOID				SetBaseCampData(const _CAMP_DATA *pCampData)=0;
	virtual INT 	GetCampID(VOID)=0;
	virtual INT 	GetBaseCampID(VOID) const=0;
	virtual VOID	SetBaseCampID(INT const nID)=0;
	INT				GetCampIDRefix(VOID);
	VOID			SetCampIDRefix(CampID_t const nID);
	BOOL			GetCampIDDirtyFlag(VOID);
	VOID			ClearCampIDDirtyFlag(VOID);
	VOID			MarkCampIDDirtyFlag(VOID);
	BOOL			GetCampIDRefixDirtyFlag(VOID);
	VOID			ClearCampIDRefixDirtyFlag(VOID);
	VOID			MarkCampIDRefixDirtyFlag(VOID);
	//modelID
	virtual INT 	GetModelID(VOID) =0;
	virtual INT 	GetBaseModelID(VOID) const=0;
	virtual VOID	SetBaseModelID(INT const nID)=0;
	INT				GetModelIDRefix(VOID);
	VOID			SetModelIDRefix(ID_t const nID);
	BOOL			GetModelIDDirtyFlag(VOID);
	VOID			ClearModelIDDirtyFlag(VOID);
	VOID			MarkModelIDDirtyFlag(VOID);
	BOOL			GetModelIDRefixDirtyFlag(VOID);
	VOID			ClearModelIDRefixDirtyFlag(VOID);
	VOID			MarkModelIDRefixDirtyFlag(VOID);
	//MountID
	virtual INT 	GetMountID(VOID);
	virtual VOID	SetMountID( INT iMountID );
	virtual INT 	GetBaseMountID(VOID) const{return INVALID_ID;};
	virtual VOID	SetBaseMountID(INT const nID){}; //不允许进行修改
	INT				GetMountIDRefix(VOID);
	VOID			SetMountIDRefix(ID_t const nID);
	BOOL			GetMountIDDirtyFlag(VOID);
	VOID			ClearMountIDDirtyFlag(VOID);
	VOID			MarkMountIDDirtyFlag(VOID);
	BOOL			GetMountIDRefixDirtyFlag(VOID);
	VOID			ClearMountIDRefixDirtyFlag(VOID);
	VOID			MarkMountIDRefixDirtyFlag(VOID);
	//Exp Refix
	INT 			GetExpRefix(VOID);
	INT 			GetBaseExpRefix(VOID) const{return 100;};//100%
	VOID			SetBaseExpRefix(INT const nID){}; //不允许进行修改
	INT				GetExpRefixRefix(VOID);
	VOID			SetExpRefixRefix(ID_t const nID);
	BOOL			GetExpRefixDirtyFlag(VOID);
	VOID			ClearExpRefixDirtyFlag(VOID);
	VOID			MarkExpRefixDirtyFlag(VOID);
	BOOL			GetExpRefixRefixDirtyFlag(VOID);
	VOID			ClearExpRefixRefixDirtyFlag(VOID);
	VOID			MarkExpRefixRefixDirtyFlag(VOID);
	//StealthLevel
	virtual INT 	GetStealthLevel(VOID);
	INT				GetStealthLevelRefix(VOID);
	VOID			SetStealthLevelRefix(ID_t const nID);
	BOOL			GetStealthLevelDirtyFlag(VOID);
	VOID			ClearStealthLevelDirtyFlag(VOID);
	VOID			MarkStealthLevelDirtyFlag(VOID);
	BOOL			GetStealthLevelRefixDirtyFlag(VOID);
	VOID			ClearStealthLevelRefixDirtyFlag(VOID);
	VOID			MarkStealthLevelRefixDirtyFlag(VOID);

	//DetectLevel
	virtual INT 	GetDetectLevel(VOID);
	INT				GetDetectLevelRefix(VOID);
	VOID			SetDetectLevelRefix(ID_t const nID);
	BOOL			GetDetectLevelDirtyFlag(VOID);
	VOID			ClearDetectLevelDirtyFlag(VOID);
	VOID			MarkDetectLevelDirtyFlag(VOID);
	BOOL			GetDetectLevelRefixDirtyFlag(VOID);
	VOID			ClearDetectLevelRefixDirtyFlag(VOID);
	VOID			MarkDetectLevelRefixDirtyFlag(VOID);

	//Alive flag
	BOOL			IsAlive(VOID);
	BOOL			IsAliveInDeed(VOID);
	BOOL			IsDie(VOID);
	VOID			ClearAliveFlag(VOID);
	VOID			MarkAliveFlag(VOID);
	BOOL			GetAliveDirtyFlag(VOID);
	VOID			ClearAliveDirtyFlag(VOID);
	VOID			MarkAliveDirtyFlag(VOID);
	//BOOL			GetAliveRefixDirtyFlag(VOID);
	//VOID			ClearAliveRefixDirtyFlag(VOID);
	//VOID			MarkAliveRefixDirtyFlag(VOID);
	//In Combat flag
	BOOL			InCombat(VOID) const;
	VOID			ClearCombatFlag(VOID);
	VOID			MarkCombatFlag(VOID);
	// Can move flag
	BOOL			CanMove(VOID);
	VOID			ClearCanMoveFlag(VOID);
	VOID			MarkCanMoveFlag(VOID);
	BOOL			GetCanMoveDirtyFlag(VOID);
	VOID			ClearCanMoveDirtyFlag(VOID);
	VOID			MarkCanMoveDirtyFlag(VOID);
	//BOOL			GetCanMoveRefixDirtyFlag(VOID);
	//VOID			ClearCanMoveRefixDirtyFlag(VOID);
	//VOID			MarkCanMoveRefixDirtyFlag(VOID);
	// Can Action flag 1
	BOOL			CanActionFlag1(VOID);
	VOID			ClearCanActionFlag1(VOID);
	VOID			MarkCanActionFlag1(VOID);
	BOOL			GetCanAction1DirtyFlag(VOID);
	VOID			ClearCanAction1DirtyFlag(VOID);
	VOID			MarkCanAction1DirtyFlag(VOID);
	//BOOL			GetCanAction1RefixDirtyFlag(VOID);
	//VOID			ClearCanAction1RefixDirtyFlag(VOID);
	//VOID			MarkCanAction1RefixDirtyFlag(VOID);
	// Can Action flag 2
	BOOL			CanActionFlag2(VOID);
	VOID			ClearCanActionFlag2(VOID);
	VOID			MarkCanActionFlag2(VOID);
	BOOL			GetCanAction2DirtyFlag(VOID);
	VOID			ClearCanAction2DirtyFlag(VOID);
	VOID			MarkCanAction2DirtyFlag(VOID);
	//BOOL			GetCanAction2RefixDirtyFlag(VOID);
	//VOID			ClearCanAction2RefixDirtyFlag(VOID);
	//VOID			MarkCanAction2RefixDirtyFlag(VOID);
	// Unbreakable flag
	virtual BOOL	IsUnbreakable(VOID);
	VOID			ClearUnbreakableFlag(VOID);
	VOID			MarkUnbreakableFlag(VOID);
	BOOL			GetUnbreakableDirtyFlag(VOID);
	VOID			ClearUnbreakableDirtyFlag(VOID);
	VOID			MarkUnbreakableDirtyFlag(VOID);
	//BOOL			GetUnbreakableRefixDirtyFlag(VOID);
	//VOID			ClearUnbreakableRefixDirtyFlag(VOID);
	//VOID			MarkUnbreakableRefixDirtyFlag(VOID);
	
	// Attr2: Move Speed
	virtual FLOAT	GetMoveSpeed(VOID)=0;
	virtual INT		GetBaseMoveSpeed(VOID)=0;
	virtual VOID	SetBaseMoveSpeed(INT const nValue)=0;
	INT 			GetMoveSpeedRefix(VOID);
	VOID			SetMoveSpeedRefix(INT const nValue);
	BOOL			GetMoveSpeedDirtyFlag(VOID);
	VOID			ClearMoveSpeedDirtyFlag(VOID);
	VOID			MarkMoveSpeedDirtyFlag(VOID);
	BOOL			GetMoveSpeedRefixDirtyFlag(VOID);
	VOID			ClearMoveSpeedRefixDirtyFlag(VOID);
	VOID			MarkMoveSpeedRefixDirtyFlag(VOID);
	// Attr2: Attack Speed
	virtual INT 	GetAttackSpeed(VOID)=0;
	virtual INT 	GetBaseAttackSpeed(VOID)=0;
	virtual VOID	SetBaseAttackSpeed(INT const nValue)=0;
	INT				GetAttackSpeedRefix(VOID);
	VOID			SetAttackSpeedRefix(INT const nValue);
	BOOL			GetAttackSpeedDirtyFlag(VOID);
	VOID			ClearAttackSpeedDirtyFlag(VOID);
	VOID			MarkAttackSpeedDirtyFlag(VOID);
	BOOL			GetAttackSpeedRefixDirtyFlag(VOID);
	VOID			ClearAttackSpeedRefixDirtyFlag(VOID);
	VOID			MarkAttackSpeedRefixDirtyFlag(VOID);
	// Attr2: Miss
	virtual INT 	GetMiss(VOID)=0;
	virtual INT 	GetBaseMiss(VOID)=0;
	virtual VOID	SetBaseMiss(INT const nValue)=0;
	INT				GetMissRefix(VOID);
	VOID			SetMissRefix(INT const nValue);
	BOOL			GetMissDirtyFlag(VOID);
	VOID			ClearMissDirtyFlag(VOID);
	VOID			MarkMissDirtyFlag(VOID);
	BOOL			GetMissRefixDirtyFlag(VOID);
	VOID			ClearMissRefixDirtyFlag(VOID);
	VOID			MarkMissRefixDirtyFlag(VOID);
	// Attr2 Hit
	virtual INT 	GetHit(VOID)=0;
	virtual INT 	GetBaseHit(VOID)=0;
	virtual VOID	SetBaseHit(INT const nValue)=0;
	INT				GetHitRefix(VOID);
	VOID			SetHitRefix(INT const nValue);
	BOOL			GetHitDirtyFlag(VOID);
	VOID			ClearHitDirtyFlag(VOID);
	VOID			MarkHitDirtyFlag(VOID);
	BOOL			GetHitRefixDirtyFlag(VOID);
	VOID			ClearHitRefixDirtyFlag(VOID);
	VOID			MarkHitRefixDirtyFlag(VOID);
	// Attr2 Critical
	virtual INT 	GetCritical(VOID)=0;
	virtual INT 	GetBaseCritical(VOID)=0;
	virtual VOID	SetBaseCritical(INT const nValue)=0;
	INT				GetCriticalRefix(VOID);
	VOID			SetCriticalRefix(INT const nValue);
	BOOL			GetCriticalDirtyFlag(VOID);
	VOID			ClearCriticalDirtyFlag(VOID);
	VOID			MarkCriticalDirtyFlag(VOID);
	BOOL			GetCriticalRefixDirtyFlag(VOID);
	VOID			ClearCriticalRefixDirtyFlag(VOID);
	VOID			MarkCriticalRefixDirtyFlag(VOID);
	

	//add by liam----------------------------------------------
	virtual INT 	GetDefenceNear(VOID)=0;
	virtual INT 	GetBaseDefenceNear (VOID)=0;
	virtual VOID	SetBaseDefenceNear (INT const nValue)=0;
	INT			    GetDefenceNearRefix(VOID);
	VOID			SetDefenceNearRefix(INT const nValue);
	BOOL			GetDefenceNearDirtyFlag(VOID);
	VOID			ClearDefenceNearDirtyFlag(VOID);
	VOID			MarkDefenceNearDirtyFlag(VOID);
	BOOL			GetDefenceNearRefixDirtyFlag(VOID);
	VOID			ClearDefenceNearRefixDirtyFlag(VOID);
	VOID			MarkDefenceNearRefixDirtyFlag(VOID);

	virtual INT 	GetDefenceFar(VOID)=0;
	virtual INT 	GetBaseDefenceFar (VOID)=0;
	virtual VOID	SetBaseDefenceFar (INT const nValue)=0;
	INT			    GetDefenceFarRefix(VOID);
	VOID			SetDefenceFarRefix(INT const nValue);
	BOOL			GetDefenceFarDirtyFlag(VOID);
	VOID			ClearDefenceFarDirtyFlag(VOID);
	VOID			MarkDefenceFarDirtyFlag(VOID);
	BOOL			GetDefenceFarRefixDirtyFlag(VOID);
	VOID			ClearDefenceFarRefixDirtyFlag(VOID);
	VOID			MarkDefenceFarRefixDirtyFlag(VOID);

	virtual INT 	GetAttackNear(VOID)=0;
	virtual INT 	GetBaseAttackNear(VOID)=0;
	virtual VOID	SetBaseAttackNear(INT const nValue)=0;
	INT			    GetAttackNearRefix(VOID);
	VOID			SetAttackNearRefix(INT const nValue);
	BOOL			GetAttackNearDirtyFlag(VOID);
	VOID			ClearAttackNearDirtyFlag(VOID);
	VOID			MarkAttackNearDirtyFlag(VOID);
	BOOL			GetAttackNearRefixDirtyFlag(VOID);
	VOID			ClearAttackNearRefixDirtyFlag(VOID);
	VOID			MarkAttackNearRefixDirtyFlag(VOID);

	virtual INT 	GetAttackFar(VOID)=0;
	virtual INT 	GetBaseAttackFar(VOID)=0;
	virtual VOID	SetBaseAttackFar(INT const nValue)=0;
	INT			    GetAttackFarRefix(VOID);
	VOID			SetAttackFarRefix(INT const nValue);
	BOOL			GetAttackFarDirtyFlag(VOID);
	VOID			ClearAttackFarDirtyFlag(VOID);
	VOID			MarkAttackFarDirtyFlag(VOID);
	BOOL			GetAttackFarRefixDirtyFlag(VOID);
	VOID			ClearAttackFarRefixDirtyFlag(VOID);
	VOID			MarkAttackFarRefixDirtyFlag(VOID);

	

	// Attr2: Toughness
	virtual INT 	GetToughness(VOID)=0;
	virtual INT 	GetBaseToughness(VOID)=0;
	virtual VOID	SetBaseToughness(INT const nValue)=0;
	INT				GetToughnessRefix(VOID);
	VOID			SetToughnessRefix(INT const nValue);
	BOOL			GetToughnessDirtyFlag(VOID);
	VOID			ClearToughnessDirtyFlag(VOID);
	VOID			MarkToughnessDirtyFlag(VOID);
	BOOL			GetToughnessRefixDirtyFlag(VOID);
	VOID			ClearToughnessRefixDirtyFlag(VOID);
	VOID			MarkToughnessRefixDirtyFlag(VOID);

	//近程减免
	virtual INT 	GetNearAttReduce(VOID)=0;
	virtual INT 	GetBaseNearAttReduce(VOID)=0;
	virtual VOID	SetBaseNearAttReduce(INT const nValue)=0;
	INT				GetNearAttReduceRefix(VOID);
	VOID			SetNearAttReduceRefix(INT const nValue);
	BOOL			GetNearAttReduceDirtyFlag(VOID);
	VOID			ClearNearAttReduceDirtyFlag(VOID);
	VOID			MarkNearAttReduceDirtyFlag(VOID);
	BOOL			GetNearAttReduceRefixDirtyFlag(VOID);
	VOID			ClearNearAttReduceRefixDirtyFlag(VOID);
	VOID			MarkNearAttReduceRefixDirtyFlag(VOID);

	//远程减免
	virtual INT 	GetFarAttReduce(VOID)=0;
	virtual INT 	GetBaseFarAttReduce(VOID)=0;
	virtual VOID	SetBaseFarAttReduce(INT const nValue)=0;
	INT				GetFarAttReduceRefix(VOID);
	VOID			SetFarAttReduceRefix(INT const nValue);
	BOOL			GetFarAttReduceDirtyFlag(VOID);
	VOID			ClearFarAttReduceDirtyFlag(VOID);
	VOID			MarkFarAttReduceDirtyFlag(VOID);
	BOOL			GetFarAttReduceRefixDirtyFlag(VOID);
	VOID			ClearFarAttReduceRefixDirtyFlag(VOID);
	VOID			MarkFarAttReduceRefixDirtyFlag(VOID);

	//近程内功减免
	virtual INT 	GetMagicNearAttReduce(VOID)=0;
	virtual INT 	GetMagicFarAttReduce(VOID)=0;

	virtual INT 	GetBaseMagicNearAttReduce(VOID)=0;
	virtual INT 	GetBaseMagicFarAttReduce(VOID)=0;

	virtual VOID	SetBaseMagicNearAttReduce(INT const nValue)=0;
	virtual VOID	SetBaseMagicFarAttReduce(INT const nValue)=0;

	INT				GetMagicNearAttReduceRefix(VOID);
	INT				GetMagicFarAttReduceRefix(VOID);

	VOID			SetMagicNearAttReduceRefix(INT const nValue);
	VOID			SetMagicFarAttReduceRefix(INT const nValue);

	BOOL			GetMagicNearAttReduceDirtyFlag(VOID);
	BOOL			GetMagicFarAttReduceDirtyFlag(VOID);

	VOID			ClearMagicNearAttReduceDirtyFlag(VOID);
	VOID			ClearMagicFarAttReduceDirtyFlag(VOID);

	VOID			MarkMagicNearAttReduceDirtyFlag(VOID);
	VOID			MarkMagicFarAttReduceDirtyFlag(VOID);

	BOOL			GetMagicNearAttReduceRefixDirtyFlag(VOID);
	BOOL			GetMagicFarAttReduceRefixDirtyFlag(VOID);

	VOID			ClearMagicNearAttReduceRefixDirtyFlag(VOID);
	VOID			ClearMagicFarAttReduceRefixDirtyFlag(VOID);

	VOID			MarkMagicNearAttReduceRefixDirtyFlag(VOID);
	VOID			MarkMagicFarAttReduceRefixDirtyFlag(VOID);

	//恐惧抗性
	virtual INT 	GetDreadAttResist(VOID)=0;
	virtual INT 	GetBaseDreadAttResist(VOID)=0;
	virtual VOID	SetBaseDreadAttResist(INT const nValue)=0;
	INT				GetDreadAttResistRefix(VOID);
	VOID			SetDreadAttResistRefix(INT const nValue);
	BOOL			GetDreadAttResistDirtyFlag(VOID);
	VOID			ClearDreadAttResistDirtyFlag(VOID);
	VOID			MarkDreadAttResistDirtyFlag(VOID);
	BOOL			GetDreadAttResistRefixDirtyFlag(VOID);
	VOID			ClearDreadAttResistRefixDirtyFlag(VOID);
	VOID			MarkDreadAttResistRefixDirtyFlag(VOID);


	//昏迷抗性
	virtual INT 	GetComaAttResist(VOID)=0;
	virtual INT 	GetBaseComaAttResist(VOID)=0;
	virtual VOID	SetBaseComaAttResist(INT const nValue)=0;
	INT				GetComaAttResistRefix(VOID);
	VOID			SetComaAttResistRefix(INT const nValue);
	BOOL			GetComaAttResistDirtyFlag(VOID);
	VOID			ClearComaAttResistDirtyFlag(VOID);
	VOID			MarkComaAttResistDirtyFlag(VOID);
	BOOL			GetComaAttResistRefixDirtyFlag(VOID);
	VOID			ClearComaAttResistRefixDirtyFlag(VOID);
	VOID			MarkComaAttResistRefixDirtyFlag(VOID);

	//沉默抗性
	virtual INT 	GetHushAttResist(VOID)=0;
	virtual INT 	GetBaseHushAttResist(VOID)=0;
	virtual VOID	SetBaseHushAttResist(INT const nValue)=0;
	INT				GetHushAttResistRefix(VOID);
	VOID			SetHushAttResistRefix(INT const nValue);
	BOOL			GetHushAttResistDirtyFlag(VOID);
	VOID			ClearHushAttResistDirtyFlag(VOID);
	VOID			MarkHushAttResistDirtyFlag(VOID);
	BOOL			GetHushAttResistRefixDirtyFlag(VOID);
	VOID			ClearHushAttResistRefixDirtyFlag(VOID);
	VOID			MarkHushAttResistRefixDirtyFlag(VOID);
	
	//缴械抗性
	virtual INT 	GetUnArmyAttResist(VOID)=0;
	virtual INT 	GetBaseUnArmyAttResist(VOID)=0;
	virtual VOID	SetBaseUnArmyAttResist(INT const nValue)=0;
	INT				GetUnArmyAttResistRefix(VOID);
	VOID			SetUnArmyAttResistRefix(INT const nValue);
	BOOL			GetUnArmyAttResistDirtyFlag(VOID);
	VOID			ClearUnArmyAttResistDirtyFlag(VOID);
	VOID			MarkUnArmyAttResistDirtyFlag(VOID);
	BOOL			GetUnArmyAttResistRefixDirtyFlag(VOID);
	VOID			ClearUnArmyAttResistRefixDirtyFlag(VOID);
	VOID			MarkUnArmyAttResistRefixDirtyFlag(VOID);

	//减攻速抗性
	virtual INT 	GetAttSpeedAttResist(VOID)=0;
	virtual INT 	GetBaseAttSpeedAttResist(VOID)=0;
	virtual VOID	SetBaseAttSpeedAttResist(INT const nValue)=0;
	INT				GetAttSpeedAttResistRefix(VOID);
	VOID			SetAttSpeedAttResistRefix(INT const nValue);
	BOOL			GetAttSpeedAttResistDirtyFlag(VOID);
	VOID			ClearAttSpeedAttResistDirtyFlag(VOID);
	VOID			MarkAttSpeedAttResistDirtyFlag(VOID);
	BOOL			GetAttSpeedAttResistRefixDirtyFlag(VOID);
	VOID			ClearAttSpeedAttResistRefixDirtyFlag(VOID);
	VOID			MarkAttSpeedAttResistRefixDirtyFlag(VOID);

	//减施法抗性
	virtual INT 	GetSkillSpeedAttResist(VOID)=0;
	virtual INT 	GetBaseSkillSpeedAttResist(VOID)=0;
	virtual VOID	SetBaseSkillSpeedAttResist(INT const nValue)=0;
	INT				GetSkillSpeedAttResistRefix(VOID);
	VOID			SetSkillSpeedAttResistRefix(INT const nValue);
	BOOL			GetSkillSpeedAttResistDirtyFlag(VOID);
	VOID			ClearSkillSpeedAttResistDirtyFlag(VOID);
	VOID			MarkSkillSpeedAttResistDirtyFlag(VOID);
	BOOL			GetSkillSpeedAttResistRefixDirtyFlag(VOID);
	VOID			ClearSkillSpeedAttResistRefixDirtyFlag(VOID);
	VOID			MarkSkillSpeedAttResistRefixDirtyFlag(VOID);
	
	//减移速抗性
	virtual INT 	GetMoveSpeedAttResist(VOID)=0;
	virtual INT 	GetBaseMoveSpeedAttResist(VOID)=0;
	virtual VOID	SetBaseMoveSpeedAttResist(INT const nValue)=0;
	INT				GetMoveSpeedAttResistRefix(VOID);
	VOID			SetMoveSpeedAttResistRefix(INT const nValue);
	BOOL			GetMoveSpeedAttResistDirtyFlag(VOID);
	VOID			ClearMoveSpeedAttResistDirtyFlag(VOID);
	VOID			MarkMoveSpeedAttResistDirtyFlag(VOID);
	BOOL			GetMoveSpeedAttResistRefixDirtyFlag(VOID);
	VOID			ClearMoveSpeedAttResistRefixDirtyFlag(VOID);
	VOID			MarkMoveSpeedAttResistRefixDirtyFlag(VOID);
	//---------------------------------------------------------
	// Attr2: 近程内功攻击力
	virtual INT 	GetAttackMagicNear(VOID)=0;
	virtual INT 	GetAttackMagicFar(VOID)=0;
	
	virtual INT 	GetBaseAttackMagicNear(VOID)=0;
	virtual INT 	GetBaseAttackMagicFar(VOID)=0;

	virtual VOID	SetBaseAttackMagicNear(INT const nValue)=0;
	virtual VOID	SetBaseAttackMagicFar(INT const nValue)=0;
	
	INT				GetAttackMagicNearRefix(VOID);
	INT				GetAttackMagicFarRefix(VOID);

	VOID			SetAttackMagicNearRefix(INT const nValue);
	VOID			SetAttackMagicFarRefix(INT const nValue);

	BOOL			GetAttackMagicNearDirtyFlag(VOID);
	BOOL			GetAttackMagicFarDirtyFlag(VOID);
	
	VOID			ClearAttackMagicNearDirtyFlag(VOID);
	VOID			ClearAttackMagicFarDirtyFlag(VOID);

	VOID			MarkAttackMagicNearDirtyFlag(VOID);
	VOID			MarkAttackMagicFarDirtyFlag(VOID);

	BOOL			GetAttackMagicNearRefixDirtyFlag(VOID);
	BOOL			GetAttackMagicFarRefixDirtyFlag(VOID);

	VOID			ClearAttackMagicNearRefixDirtyFlag(VOID);
	VOID			ClearAttackMagicFarRefixDirtyFlag(VOID);

	VOID			MarkAttackMagicNearRefixDirtyFlag(VOID);
	VOID			MarkAttackMagicFarRefixDirtyFlag(VOID);

	//近程内功
	virtual INT 	GetDefenceMagicNear(VOID)=0;
	virtual INT 	GetDefenceMagicFar(VOID)=0;

	virtual INT 	GetBaseDefenceMagicNear(VOID)=0;
	virtual INT 	GetBaseDefenceMagicFar(VOID)=0;

	virtual VOID	SetBaseDefenceMagicNear(INT const nValue)=0;
	virtual VOID	SetBaseDefenceMagicFar(INT const nValue)=0;
	
	INT				GetDefenceMagicNearRefix(VOID);
	INT				GetDefenceMagicFarRefix(VOID);

	VOID			SetDefenceMagicNearRefix(INT const nValue);
	VOID			SetDefenceMagicFarRefix(INT const nValue);

	BOOL			GetDefenceMagicNearDirtyFlag(VOID);
	BOOL			GetDefenceMagicFarDirtyFlag(VOID);

	VOID			ClearDefenceMagicNearDirtyFlag(VOID);
	VOID			ClearDefenceMagicFarDirtyFlag(VOID);

	VOID			MarkDefenceMagicNearDirtyFlag(VOID);
	VOID			MarkDefenceMagicFarDirtyFlag(VOID);

	BOOL			GetDefenceMagicNearRefixDirtyFlag(VOID);
	BOOL			GetDefenceMagicFarRefixDirtyFlag(VOID);

	VOID			ClearDefenceMagicNearRefixDirtyFlag(VOID);
	VOID			ClearDefenceMagicFarRefixDirtyFlag(VOID);

	VOID			MarkDefenceMagicNearRefixDirtyFlag(VOID);
	VOID			MarkDefenceMagicFarRefixDirtyFlag(VOID);

	// Attr2: Attack and Defence power of gold
	virtual INT 	GetAttackGold(VOID)=0;
	virtual INT 	GetBaseAttackGold(VOID)=0;
	virtual VOID	SetBaseAttackGold(INT const nValue)=0;
	INT				GetAttackGoldRefix(VOID);
	VOID			SetAttackGoldRefix(INT const nValue);
	BOOL			GetAttackGoldDirtyFlag(VOID);
	VOID			ClearAttackGoldDirtyFlag(VOID);
	VOID			MarkAttackGoldDirtyFlag(VOID);
	BOOL			GetAttackGoldRefixDirtyFlag(VOID);
	VOID			ClearAttackGoldRefixDirtyFlag(VOID);
	VOID			MarkAttackGoldRefixDirtyFlag(VOID);

	virtual INT 	GetDefenceGold(VOID)=0;
	virtual INT 	GetBaseDefenceGold(VOID)=0;
	virtual VOID	SetBaseDefenceGold(INT const nValue)=0;
	INT				GetDefenceGoldRefix(VOID);
	VOID			SetDefenceGoldRefix(INT const nValue);
	BOOL			GetResistGoldDirtyFlag(VOID);
	VOID			ClearResistGoldDirtyFlag(VOID);
	VOID			MarkResistGoldDirtyFlag(VOID);
	BOOL			GetResistGoldRefixDirtyFlag(VOID);
	VOID			ClearResistGoldRefixDirtyFlag(VOID);
	VOID			MarkResistGoldRefixDirtyFlag(VOID);

	// Attr2: Attack and Defence power of Wood
	virtual INT 	GetAttackWood(VOID)=0;
	virtual INT 	GetBaseAttackWood(VOID)=0;
	virtual VOID	SetBaseAttackWood(INT const nValue)=0;
	INT				GetAttackWoodRefix(VOID);
	VOID			SetAttackWoodRefix(INT const nValue);
	BOOL			GetAttackWoodDirtyFlag(VOID);
	VOID			ClearAttackWoodDirtyFlag(VOID);
	VOID			MarkAttackWoodDirtyFlag(VOID);
	BOOL			GetAttackWoodRefixDirtyFlag(VOID);
	VOID			ClearAttackWoodRefixDirtyFlag(VOID);
	VOID			MarkAttackWoodRefixDirtyFlag(VOID);

	virtual INT 	GetDefenceWood(VOID)=0;
	virtual INT 	GetBaseDefenceWood(VOID)=0;
	virtual VOID	SetBaseDefenceWood(INT const nValue)=0;
	INT				GetDefenceWoodRefix(VOID);
	VOID			SetDefenceWoodRefix(INT const nValue);
	BOOL			GetResistWoodDirtyFlag(VOID);
	VOID			ClearResistWoodDirtyFlag(VOID);
	VOID			MarkResistWoodDirtyFlag(VOID);
	BOOL			GetResistWoodRefixDirtyFlag(VOID);
	VOID			ClearResistWoodRefixDirtyFlag(VOID);
	VOID			MarkResistWoodRefixDirtyFlag(VOID);

	// Attr2: Attack and Defence power of Water
	virtual INT 	GetAttackWater(VOID)=0;
	virtual INT 	GetBaseAttackWater(VOID)=0;
	virtual VOID	SetBaseAttackWater(INT const nValue)=0;
	INT				GetAttackWaterRefix(VOID);
	VOID			SetAttackWaterRefix(INT const nValue);
	BOOL			GetAttackWaterDirtyFlag(VOID);
	VOID			ClearAttackWaterDirtyFlag(VOID);
	VOID			MarkAttackWaterDirtyFlag(VOID);
	BOOL			GetAttackWaterRefixDirtyFlag(VOID);
	VOID			ClearAttackWaterRefixDirtyFlag(VOID);
	VOID			MarkAttackWaterRefixDirtyFlag(VOID);

	virtual INT 	GetDefenceWater(VOID)=0;
	virtual INT 	GetBaseDefenceWater(VOID)=0;
	virtual VOID	SetBaseDefenceWater(INT const nValue)=0;
	INT				GetDefenceWaterRefix(VOID);
	VOID			SetDefenceWaterRefix(INT const nValue);
	BOOL			GetResistWaterDirtyFlag(VOID);
	VOID			ClearResistWaterDirtyFlag(VOID);
	VOID			MarkResistWaterDirtyFlag(VOID);
	BOOL			GetResistWaterRefixDirtyFlag(VOID);
	VOID			ClearResistWaterRefixDirtyFlag(VOID);
	VOID			MarkResistWaterRefixDirtyFlag(VOID);

	// Attr2: Attack and Defence power of Fire
	virtual INT 	GetAttackFire(VOID)=0;
	virtual INT 	GetBaseAttackFire(VOID)=0;
	virtual VOID	SetBaseAttackFire(INT const nValue)=0;
	INT				GetAttackFireRefix(VOID);
	VOID			SetAttackFireRefix(INT const nValue);
	BOOL			GetAttackFireDirtyFlag(VOID);
	VOID			ClearAttackFireDirtyFlag(VOID);
	VOID			MarkAttackFireDirtyFlag(VOID);
	BOOL			GetAttackFireRefixDirtyFlag(VOID);
	VOID			ClearAttackFireRefixDirtyFlag(VOID);
	VOID			MarkAttackFireRefixDirtyFlag(VOID);

	virtual INT 	GetDefenceFire(VOID)=0;
	virtual INT 	GetBaseDefenceFire(VOID)=0;
	virtual VOID	SetBaseDefenceFire(INT const nValue)=0;
	INT				GetDefenceFireRefix(VOID);
	VOID			SetDefenceFireRefix(INT const nValue);
	BOOL			GetResistFireDirtyFlag(VOID);
	VOID			ClearResistFireDirtyFlag(VOID);
	VOID			MarkResistFireDirtyFlag(VOID);
	BOOL			GetResistFireRefixDirtyFlag(VOID);
	VOID			ClearResistFireRefixDirtyFlag(VOID);
	VOID			MarkResistFireRefixDirtyFlag(VOID);

	// Attr2: Attack and Defence power of Soil
	virtual INT 	GetAttackSoil(VOID)=0;
	virtual INT 	GetBaseAttackSoil(VOID)=0;
	virtual VOID	SetBaseAttackSoil(INT const nValue)=0;
	INT				GetAttackSoilRefix(VOID);
	VOID			SetAttackSoilRefix(INT const nValue);
	BOOL			GetAttackSoilDirtyFlag(VOID);
	VOID			ClearAttackSoilDirtyFlag(VOID);
	VOID			MarkAttackSoilDirtyFlag(VOID);
	BOOL			GetAttackSoilRefixDirtyFlag(VOID);
	VOID			ClearAttackSoilRefixDirtyFlag(VOID);
	VOID			MarkAttackSoilRefixDirtyFlag(VOID);

	virtual INT 	GetDefenceSoil(VOID)=0;
	virtual INT 	GetBaseDefenceSoil(VOID)=0;
	virtual VOID	SetBaseDefenceSoil(INT const nValue)=0;
	INT				GetDefenceSoilRefix(VOID);
	VOID			SetDefenceSoilRefix(INT const nValue);
	BOOL			GetResistSoilDirtyFlag(VOID);
	VOID			ClearResistSoilDirtyFlag(VOID);
	VOID			MarkResistSoilDirtyFlag(VOID);
	BOOL			GetResistSoilRefixDirtyFlag(VOID);
	VOID			ClearResistSoilRefixDirtyFlag(VOID);
	VOID			MarkResistSoilRefixDirtyFlag(VOID);

	// Attr2 Reduce Slower Duration
	virtual INT 	GetReduceSlowerDuration(VOID)=0;
	INT				GetReduceSlowerDurationRefix(VOID);
	VOID			SetReduceSlowerDurationRefix(INT const nValue);
	BOOL			GetReduceSlowerDurationDirtyFlag(VOID);
	VOID			ClearReduceSlowerDurationDirtyFlag(VOID);
	VOID			MarkReduceSlowerDurationDirtyFlag(VOID);
	BOOL			GetReduceSlowerDurationRefixDirtyFlag(VOID);
	VOID			ClearReduceSlowerDurationRefixDirtyFlag(VOID);
	VOID			MarkReduceSlowerDurationRefixDirtyFlag(VOID);

	// Attr2 Reduce Weaken Duration
	virtual INT 	GetReduceWeakenDuration(VOID)=0;
	INT				GetReduceWeakenDurationRefix(VOID);
	VOID			SetReduceWeakenDurationRefix(INT const nValue);
	BOOL			GetReduceWeakenDurationDirtyFlag(VOID);
	VOID			ClearReduceWeakenDurationDirtyFlag(VOID);
	VOID			MarkReduceWeakenDurationDirtyFlag(VOID);
	BOOL			GetReduceWeakenDurationRefixDirtyFlag(VOID);
	VOID			ClearReduceWeakenDurationRefixDirtyFlag(VOID);
	VOID			MarkReduceWeakenDurationRefixDirtyFlag(VOID);

	// Attr2 Reduce Faint Duration
	virtual INT 	GetReduceFaintDuration(VOID)=0;
	INT				GetReduceFaintDurationRefix(VOID);
	VOID			SetReduceFaintDurationRefix(INT const nValue);
	BOOL			GetReduceFaintDurationDirtyFlag(VOID);
	VOID			ClearReduceFaintDurationDirtyFlag(VOID);
	VOID			MarkReduceFaintDurationDirtyFlag(VOID);
	BOOL			GetReduceFaintDurationRefixDirtyFlag(VOID);
	VOID			ClearReduceFaintDurationRefixDirtyFlag(VOID);
	VOID			MarkReduceFaintDurationRefixDirtyFlag(VOID);

	// Attr2 Reduce Poisoned Duration
	virtual INT 	GetReducePoisonedDuration(VOID)=0;
	INT				GetReducePoisonedDurationRefix(VOID);
	VOID			SetReducePoisonedDurationRefix(INT const nValue);
	BOOL			GetReducePoisonedDurationDirtyFlag(VOID);
	VOID			ClearReducePoisonedDurationDirtyFlag(VOID);
	VOID			MarkReducePoisonedDurationDirtyFlag(VOID);
	BOOL			GetReducePoisonedDurationRefixDirtyFlag(VOID);
	VOID			ClearReducePoisonedDurationRefixDirtyFlag(VOID);
	VOID			MarkReducePoisonedDurationRefixDirtyFlag(VOID);

	// Attr2 VisionRange
	virtual INT 	GetVisionRange(VOID)=0;
	virtual INT 	GetBaseVisionRange(VOID)=0;
	virtual VOID	SetBaseVisionRange(INT const nValue)=0;
	INT				GetVisionRangeRefix(VOID);
	VOID			SetVisionRangeRefix(INT const nValue);
	BOOL			GetVisionRangeDirtyFlag(VOID);
	VOID			ClearVisionRangeDirtyFlag(VOID);
	VOID			MarkVisionRangeDirtyFlag(VOID);
	BOOL			GetVisionRangeRefixDirtyFlag(VOID);
	VOID			ClearVisionRangeRefixDirtyFlag(VOID);
	VOID			MarkVisionRangeRefixDirtyFlag(VOID);

	//Energy
	BOOL 			GetMaxEnergyDirtyFlag(VOID);
	VOID 			ClearMaxEnergyDirtyFlag(VOID);
	VOID 			MarkMaxEnergyDirtyFlag(VOID);
	//Vigor
	BOOL 			GetMaxVigorDirtyFlag(VOID);
	VOID 			ClearMaxVigorDirtyFlag(VOID);
	VOID 			MarkMaxVigorDirtyFlag(VOID);
	//Energy Regenerate
	BOOL 			GetEnergyRegenerateDirtyFlag(VOID);
	VOID 			ClearEnergyRegenerateDirtyFlag(VOID);
	VOID 			MarkEnergyRegenerateDirtyFlag(VOID);
	//Vigor Regenerate
	BOOL 			GetVigorRegenerateDirtyFlag(VOID);
	VOID 			ClearVigorRegenerateDirtyFlag(VOID);
	VOID 			MarkVigorRegenerateDirtyFlag(VOID);
protected:
	//Here open to sub class to modify those is virtual functions.
	//整数属性
	INT		GetIntAttr(CharIntAttrs_T::Index_T const nIdx) const {return m_IntAttrs.GetAttr(nIdx);};
	VOID 	SetIntAttr(CharIntAttrs_T::Index_T const nIdx, INT const nValue){m_IntAttrs.SetAttr(nIdx, nValue);};
	VOID 	ResetIntAttrs(VOID);
	
	//整数属性修正
	INT		GetIntAttrRefix(CharIntAttrRefixs_T::Index_T const nIdx) const {return m_IntAttrRefixs.GetAttr(nIdx);};
	VOID	SetIntAttrRefix(CharIntAttrRefixs_T::Index_T const nIdx, INT const nValue){m_IntAttrRefixs.SetAttr(nIdx,nValue);};
	VOID 	ResetIntAttrRefixs(VOID);

	//BOOL属性
	BOOL	GetBoolAttr(CharBoolAttrs_T::Index_T const nIdx) const{return m_BoolAttrs.GetAttr(nIdx);};
	VOID	SetBoolAttr(CharBoolAttrs_T::Index_T const nIdx, BOOL const bFlag) {m_BoolAttrs.SetAttr(nIdx, bFlag);};
	VOID	ResetBoolAttr(VOID);

	//设置所有的脏标记
	VOID	MarkAllIntAttrDirtyFlags(VOID) {m_IntAttrDirtyFlags.MarkAllFlags();};
	VOID	MarkAllBoolAttrDirtyFlags(VOID) {m_BoolAttrDirtyFlags.MarkAllFlags();};	
	VOID	MarkAllIntAttrRefixDirtyFlags(VOID) {m_IntAttrRefixDirtyFlags.MarkAllFlags();};

private:
	//整数属性脏标记
	BOOL	GetIntAttrDirtyFlag(CharIntAttrs_T::Index_T const nIdx) const {return m_IntAttrDirtyFlags.GetFlagByIndex(nIdx);};
	VOID	ClearIntAttrDirtyFlag(CharIntAttrs_T::Index_T const nIdx) {m_IntAttrDirtyFlags.ClearFlagByIndex(nIdx);};
	VOID	MarkIntAttrDirtyFlag(CharIntAttrs_T::Index_T const nIdx) {m_IntAttrDirtyFlags.MarkFlagByIndex(nIdx);};


	//BOOL属性脏标记
	BOOL	GetBoolAttrDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) const {m_BoolAttrDirtyFlags.GetFlagByIndex(nIdx);};
	VOID	ClearBoolAttrDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) {m_BoolAttrDirtyFlags.ClearFlagByIndex(nIdx);};
	VOID	MarkBoolAttrDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) {m_BoolAttrDirtyFlags.MarkFlagByIndex(nIdx);};

	//整数属性修正的脏标记
	BOOL	GetIntAttrRefixDirtyFlag(CharIntAttrRefixs_T::Index_T const nIdx) const {return m_IntAttrRefixDirtyFlags.GetFlagByIndex(nIdx);};
	VOID	ClearIntAttrRefixDirtyFlag(CharIntAttrRefixs_T::Index_T const nIdx) {m_IntAttrRefixDirtyFlags.ClearFlagByIndex(nIdx);};
	VOID	MarkIntAttrRefixDirtyFlag(CharIntAttrRefixs_T::Index_T const nIdx) {m_IntAttrRefixDirtyFlags.MarkFlagByIndex(nIdx);};
/*
	//BOOL属性修正的脏标记
	BOOL	GetBoolAttrRefixDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) const {return m_BoolAttrRefixDirtyFlags.GetFlagByIndex(nIdx);};
	VOID	ClearBoolAttrRefixDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) {m_BoolAttrRefixDirtyFlags.ClearFlagByIndex(nIdx);};
	VOID	MarkBoolAttrRefixDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) {m_BoolAttrRefixDirtyFlags.MarkFlagByIndex(nIdx);};
	VOID	MarkAllBoolAttrRefixDirtyFlags(VOID) {m_BoolAttrRefixDirtyFlags.MarkAllFlags();};
*/
private :
	//整数属性
	CharIntAttrs_T m_IntAttrs;
	//整数属性修正
	CharIntAttrRefixs_T m_IntAttrRefixs;
	//整数属性脏标记
	typedef BitFlagSet_T<CharIntAttrs_T::MAX_INDEX> CharIntAttrDirtyFlags_T;
	CharIntAttrDirtyFlags_T m_IntAttrDirtyFlags;
	//BOOL属性
	CharBoolAttrs_T m_BoolAttrs;
	//BOOL属性脏标记
	typedef BitFlagSet_T<CharBoolAttrs_T::MAX_INDEX> CharBoolAttrDirtyFlags_T;
	CharBoolAttrDirtyFlags_T m_BoolAttrDirtyFlags;
	//整数属性修正的脏标记
	typedef BitFlagSet_T<CharIntAttrRefixs_T::MAX_INDEX> CharIntAttrRefixDirtyFlags_T;
	CharIntAttrRefixDirtyFlags_T m_IntAttrRefixDirtyFlags;
/*	//BOOL属性修正的脏标记
	typedef BitFlagSet_T<CharBoolAttrs_T::MAX_INDEX> CharBoolAttrRefixDirtyFlags_T;
	CharBoolAttrRefixDirtyFlags_T m_BoolAttrRefixDirtyFlags;
*/
/////////////////////////////////////////////////////////////////////////////////
//obj 公共属性接口
	
/////////////////////////////////////////////////////////////////////////////////
//character 公共属性接口
public :
	INT  GetHorseDataIndex( INT iIndex );
	VOID SetHorseDataIndex( INT iIndex, INT iVal );
	VOID CleanupHorseBuff();
protected:
	//访问者对象指针，为目前的程序结构进行变通
	//在被访问者内部保存访问者对象指针,此指针不是固定不变的，会因为不同的上下文环境
	//由不同的context对象负责调用Accept方法进行改变
	CharacterVisitor* m_pVisitor;
	//马对主人使用的被动技能的dataIndex
	INT aHorseDataIndex[MAX_HORSE_DATA_INDEX];
};
#endif
