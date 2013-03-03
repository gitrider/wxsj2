//***************************************************************************************************
// �ļ�����:	Obj_Character.h
// ϵͳ����:	
// ģ������:	Obj_Character
// ����˵��:	
//
// ����ĵ�:	
// ��    ��:	liam
// ��������:	
// ��    ��:	
// �����Ȩ:	
//
//	����������ģʽ�е�Ԫ����Ļ��࣬������������Ը�����Ҫʵ�־���Accept����
//	��Ϊ��ɫ�����кܶ���߼��ϲ�������������
//
//�޸ļ�¼:
//          1.
//				�� �� ��:liam
//				�޸�ʱ��:2007/11/02
//				�޸�����:���Եڶ������ع�(����μ��ĵ�)
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

//��ʱ��Ч���ܻ������ߴ磬_DELAY_TIME_IMPACT_LISTʹ��
const INT MAX_DELAY_TIME_IMPACT_NUM = 30;
const INT MAX_DELAY_TIME_SKILL_MISS_NUM = 15;
const INT MAX_HORSE_DATA_INDEX = 6;
enum ENUM_CHARACTER_LOGIC
{
	CHARACTER_LOGIC_INVALID	= -1,
	CHARACTER_LOGIC_IDLE,			//��Ϣ
	CHARACTER_LOGIC_MOVE,			//�ж�
	CHARACTER_LOGIC_USE_SKILL,		//ս������
	//CHARACTER_LOGIC_DEAD,
	//CHARACTER_LOGIC_SIT,
	CHARACTER_LOGIC_USE_ABILITY,	// ʹ�������

	CHARACTER_LOGIC_NUMBERS
};
//��Ʒ��Ч
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
//������Ч
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
	//������ʱ��Чר��
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

		//��ָ���±��Ԫ�ش������Ƴ�
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
	// ���ɫ����������
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
	//����װ�����˵�
	virtual VOID UpdataEquipDamagePoint(INT nType){}

	//����ͬһ�����ܾ��ж����˺�����������þ���ImpactLogic_T����ÿ�������˺������ض�����
	//Ȼ��������˺����͵��˺�ֵ����ٵ���Impact_OnDamage����ʵ�ʵ��˺�
	VOID OnDamages(INT const* const pDamagelist, ObjID_t nAttackerID, BOOL bCritical=FALSE, SkillID_t nSkillID=INVALID_ID, INT iLogicCount=INVALID_ID);
	//��һ�˺����͵Ĵ���
	VOID OnDamage(INT nDamage, ObjID_t nAttackerID, BOOL bCritical=FALSE, SkillID_t nSkillID=INVALID_ID, INT iLogicCount=INVALID_ID);
	VOID OnDamageTarget(INT& nDamage, Obj_Character& rTar, SkillID_t nSkillID=INVALID_ID);
	INT  OnFiltrateImpact(OWN_IMPACT& rImpact);
	VOID RefixSkill(SkillInfo_T& rSkill);
	VOID RefixImpact(OWN_IMPACT& rImpactNeedRedix);
	VOID RefixRageRegeneration(INT& rRageRegeneration);
	VOID OnBeSkill(Obj_Character& rAttacker, INT nBehaviorType);
	VOID OnBeHit(Obj_Character& rAttacker);
	VOID OnHitTarget(Obj_Character& rTarget);
	VOID OnBeMiss(Obj_Character& rAttacker);	//���Լ����ܳɹ�
	VOID OnMissTarget(Obj_Character& rTarget);	//�������Է����Է����ܳɹ�
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

// Ч���㴦��
public:
	//��õ�ǰЧ���б��е�Ч������
	INT			Impact_GetListSize(VOID)const;
	//�����ɫ���ϵ�Ч���б�
	VOID 		Impact_HeartBeat(VOID);
	//ʹЧ����Ч,�������ʱ��Ч�ļ�����push��pool
	//�����ʱ��Ϊ0���ֱ�ӵ���Impact_RegisterImpactImp
	BOOL 		Impact_RegisterImpact(OWN_IMPACT& rImpact);
	//ʹ��Ч��Snȡ��Ч���б��е�Ч��
	BOOL 		Impact_UnRegisterImpactBySN(INT const nSn);
	//ʹ��SkillIDAndImpactIDȡ��Ч���б��е�Ч��
	BOOL 		Impact_UnRegisterImpactBySkillIDAndImpactID(SkillID_t const nSkillID, ImpactID_t const nImpactID);
	BOOL		Impact_UnregisterImpactByDataIndex( INT iDataIndex );
	VOID		Impact_CancelImpactInSpecificCollection(INT nCollectionID);
	//����������Ƿ�ɼ�
	BOOL 		Impact_CanViewThisSpecialObj(Obj_Special& rObj);
	//��ѯЧ���б����Ƿ����ض���Ч��
	BOOL 		Impact_HaveImpactOfSpecificImpactID(INT const nImpactID);
	BOOL 		Impact_HaveImpactOfSpecificLogicID(INT const nLogicID);
	BOOL 		Impact_HaveImpactOfSpecificMutexID(INT const nMutexID);
	BOOL 		Impact_HaveImpactOfSpecificSkillID(INT const nSkillID);
	BOOL 		Impact_HaveImpactInSpecificCollection(INT const nCollectionID);
	//�ò�ͬ������������ɢָ����Ч��
	INT			Impact_DispelSpecificImpactByDataIndex(INT nDataIndex, INT nNeedDispel=-1);//-1����ɢ���еĴ���Ч��
	INT			Impact_DispelImpactOfSpecificImpactID(INT nImpactID, INT nNeedDispel=-1); //-1����ɢ���еĴ���Ч��
	INT			Impact_DispelImpactOfSpecificStand(ID_t nStandID, INT nNeedDispel=-1); //-1����ɢ����ͬӰ�����ʵ�Ч��
	INT			Impact_DispelImpactOfSpecificMutexID(INT nMutexID, INT nNeedDispel=-1);//-1����ɢ���еĴ���Ч��
	INT			Impact_DispelImpactOfSpecificLogicID(INT nLogicID, INT nNeedDispel=-1);//-1����ɢ���еĴ���Ч��
	INT			Impact_DispelImpactOfSpecificSkillID(INT nSkillID, INT nNeedDispel=-1);//-1����ɢ���еĴ���Ч��
	INT			Impact_DispelImpactInSpecificCollection(INT nCollectionID, INT nDispelLevel, INT nNeedDispel=-1);//-1����ɢ���еĴ���Ч��
	//�ò�ͬ�����������ҵ�Ч���б��е�Ч������
	OWN_IMPACT* Impact_GetFirstImpactOfSpecificImpactID(INT nImpactID);
	OWN_IMPACT* Impact_GetFirstImpactOfSpecificLogicID(INT nLogicID);
	OWN_IMPACT* Impact_GetFirstImpactOfSpecificMutexID(INT nMutexID);
	OWN_IMPACT* Impact_GetFirstImpactOfSpecificSkillID(INT nSkillID);
	OWN_IMPACT* Impact_GetFirstImpactInSpecificCollection(INT nCollectionID);
	//ָ����Ч���Ƿ���ɢ
	VOID	Impact_OnImpactFadeOut(OWN_IMPACT& rImp);
	//���������������˳�����ɵ�Ч��
	VOID	Impact_CleanAllImpactWhenPetDead(ObjID_t idPet);
	////�����֪��ͬ�༼�ܷ��͸���ɫ�����С��ID,�ڽ�ɫ���ϼ�¼��ID��ֻ������ʹ��ݵ���;
	//UINT	Impact_GetImpactUniqueID(){ return m_uUniqueIDForImpact; };
	////������֪��ͬ�༼�ܷ��͸���ɫ�����С��ID,�ڽ�ɫ���ϼ�¼��ID��ֻ������ʹ��ݵ���;
	//VOID	Impact_SetImpactUniqueID( UINT uID ){ m_uUniqueIDForImpact = uID; };
	//��֪�༼�ܵ�С���Ƿ�����򱾶���Ͷ��
	//��Ϊ��������ʱ��Ч��������ʱ�ڼ��п����ظ�Ͷ�ݣ��˷��������ж���ʱ�ڼ���б���
	//�Ƿ����ͬ��Ч�������������FALSE, ���û����TRUE
	BOOL	Impact_IsCanReciveBallImpact( OWN_IMPACT& rImp );
	//�洢δ������ʱ��������������ݽṹ�ķ��ʷ���
	_SkillMissMSGDelayTime* GetDelayTimeSkillMissList( INT iIndex );
	VOID AddDelayTimeSkillMissList( _SkillMissMSGDelayTime& val );

	//��ĳ������͵����
	VOID	Impact_StealImpact(Obj_Character& rTar, INT nCollectionID, INT nStandFlag, INT nStealCount, INT nRefixRate);

protected:
	INT 	Impact_OnFiltrateImpact(OWN_IMPACT& rImpact);
	//ִ����������Ч���߼�����ÿһ��Ч�������OnDamages
	VOID	Impact_OnDamages(INT* const pDamage, Obj_Character* const pAttacker, SkillID_t nSkillID=INVALID_ID);
	//ִ��nDamageָ����Ч���߼������OnDamage
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
	//ʹЧ����Ч�����þ���Ч�������OnActive
	VOID	Impact_OnImpactActived(OWN_IMPACT& rImp);
	VOID Impact_SendImpactMsg(OWN_IMPACT& rImp, Time_t iDelayTime );
	BOOL 	Impact_RemoveImpactByIndex(INT const nIdx);
	//ʹЧ��������Ч,Impact_RegisterImpact����ʱ��Ч�ļ�����push��pool
	//�����ʱ��Ϊ0���ֱ�ӵ��ô˷���
	VOID 	Impact_RegisterImpactImp(OWN_IMPACT& rImp);
	//�Ƿ������rImpactIn�滻����Ч���б��е�Ч��
	//TRUEΪ�ɹ��滻
	BOOL 	Impact_ReplaceMutuallyExclusiveImpact(OWN_IMPACT& rImpactIn);
	VOID	Impact_ReplaceLastGoodImpact(OWN_IMPACT& rImpactIn);
	BOOL	Impact_AddNewImpact(OWN_IMPACT& rImpactIn);
	//ʹrDest��ɢ,ʹrSrc��Ч
	VOID	Impact_ReplaceImpact(OWN_IMPACT& rDest, OWN_IMPACT& rSrc);
	VOID	Impact_CleanupAllFadeOutImpacts(VOID);
	//���Ч�����ӳ�ʱ��������Ч���أ���ʱ�䵽�ٴ���
	//����ӳ�ʱ��Ϊ0����������ʹЧ����Ч
	VOID	Impact_PushToDelayTimeList( OWN_IMPACT& oImpact );
	//��ʱ������Ч����
	VOID	ImpactDelayTimeHeartBeat();
private:
	virtual VOID 	Impact_InitList(VOID);
	UINT 	m_nSNSeedOfImpacts;
//������Ч��
protected:
	UINT					Impact_GetSNSeed(VOID) const{return m_nSNSeedOfImpacts;};
	VOID					Impact_SetSNSeed(INT const nValue) {m_nSNSeedOfImpacts=nValue;};
	VOID					Impact_AddSNSeed(VOID){++m_nSNSeedOfImpacts;};
	virtual _IMPACT_LIST&	Impact_GetImpactList(VOID)=0;
private:
	_DELAY_TIME_IMPACT_LIST	m_DelayTimeImpactList;
	_SkillMissMSGDelayTime	m_DelayTimeSkillMissList[MAX_DELAY_TIME_SKILL_MISS_NUM];
	
///////////////////////////////////////////////////////////////////////////////////
// ���ܲ��ս�����
public:
	Obj* 			GetMyShadowGuard(VOID);
	Obj* 			GetMyPet(INT iIndex);		//��ȡ��ɫ����
	Obj* 			GetMyMaster(VOID);			//�����ȡ����
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
	Time_t	m_nElapsedRevocerTick;									//�Ѿ���ȥ�Ļָ�ʱ��
	//Time_t	m_nElapsedRageRevocerTick;
	Time_t	m_nElapsedPneumaRevocerTick;
protected:
	VOID			SubSkillAutoUseHeartBeat();
	VOID			DelayTimeSkillMissHeartBeat();
	//Ϊ����ͬ����ʵʱ����CharBase��Ϣ�������HP���ǣ���֯�Զ�ˢ����ʱ�ظ�����
	virtual VOID	SendCharBaseDirectForHP(){}
	VOID			SendHealAndDamageMSG( INT nIncrement, Obj_Character* pCharacter, BOOL bCritical, INT iLogicCount );
	VOID			OnIncementHp( INT nIncrement, Obj_Character* pCharacter );
	VOID			OnDecementHP( INT nIncrement, BOOL bDOT, Obj_Character* pCharacter );
private:
	ObjID_t m_nLockedTargetID;										//������Ŀ�� 
	SkillInfo_T m_SkillInfo;										//����ʵ��
	TargetingAndDepletingParams_T m_TargetingAndDepletingParams;	//Ŀ�����������
	Time_t	m_nActionTime;											//��Ͽͻ�����������ʾ��ʱ����ƣ�ʱ�䲻Ϊ��ʱ��Ҫ������һ���������൱�����彩ֱ���ܶ���.
	Time_t  m_nAutoRepeatCooldown;
	INT		m_aSubSkill[SkillTemplateData_T::MAX_SUB_SKILL];		//�����ŵ��Ӽ����б�
//���ܲ���ķ�
public:
	VOID			Skill_ExecFromItemScript(SkillID_t const nID);
	VOID			Skill_ExecFromScript(SkillID_t const nID, ObjID_t nTargetID, WORLD_POS& pos, FLOAT fDir, BOOL bPassSkillCheck);
	virtual BOOL 	Skill_HaveSkill( SkillID_t const nID ) const{return TRUE;};
	virtual BOOL 	Skill_ModifySkill(SkillID_t	const nID, INT const nModifyFlag){return TRUE;};
	virtual BOOL 	Skill_HaveXinFa( SkillID_t const nID ) const{return TRUE;};
	virtual INT 	Skill_GetXinFaLevel(SkillID_t const nID) const{return 1;};
	virtual BOOL 	Skill_SetXinFaLevel(SkillID_t const nID, INT const nLevel){return TRUE;};
	virtual VOID 	Skill_OnEquipItem(VOID){};
	//����˼�����Ҫ��ȴ״̬���ж��Ƿ�����ȴ�����������ȴӰ��
	BOOL 			Skill_IsSkillCooldowned(SkillID_t nID);
	BOOL			Skill_CanUseThisSkillInThisStatus(SkillID_t nSkillID);
	Obj* 			Skill_CreateObjSpecial(WORLD_POS const& rPosition, INT nDataID);
	BOOL			Skill_RefixSkill(SkillInfo_T& rSkillInfo);
	Obj_Character*	CreateGuard(UINT uDataID, INT nBaseAI = 0, INT nExtAI = 0);
	Obj_Character*	GetGuard(VOID) const { return m_pGuard; }
	VOID			SetGuard(Obj_Character* pGuard) { m_pGuard = pGuard; }
	//WORLD_POS���
	VOID  NormalizeWorldPos( WORLD_POS& sPos ) const;
	FLOAT WordPosLength( FLOAT fX, FLOAT fY ) const;
    INT             CalRelationPlus(BOOL bHuman_A, BOOL bHuman_B, Obj_Human *pHumanA, Obj_Human *pHumanB, const _CAMP_DATA *pCamp_A, const _CAMP_DATA *pCamp_B);

protected:
	VOID  BeatBack( Obj_Character* pAttacker, INT iMyHPValue, VOID* pMsg, const SkillTemplateData_T* pSkillTemplate );
	VOID  BeatFly( Obj_Character* pAttacker, INT iMyHPValue, VOID* pMsg, const SkillTemplateData_T* pSkillTemplate );
	//�Ƿ񱻻���
	BOOL  IsBeatFly( Obj_Character& rMe, Skill_Module::SkillTemplateData_T const* pTemplate) const;
	//�Ƿ񱻻���
	BOOL  IsBeatBack( Obj_Character& rMe, Skill_Module::SkillTemplateData_T const* pTemplate ) const;
	//���ݼ��������еĸ��ʺ�fRate���бȽϣ����Ƿ�ᷢ��
	BOOL  IsHappenByRate( FLOAT fRate, Obj_Character& rMe ) const;
	//���ݸ�����Ϣ������˵�λ��
	VOID  CalculateBeatBackPos( Obj_Character& rMe, Obj* rTag, SkillTemplateData_T const* pTemplate, WORLD_POS& sPos ) const;
	//���㳤��
	FLOAT KLU_GetYAngle(const WORLD_POS& fvPos1, const WORLD_POS& fvPos2);
	VOID  OnSubSkill( SkillInfo_T& rSkillInf );
	BOOL  IsSubSkillCanSuccess( INT iRate );


private:
	Obj_Character*	m_pGuard; // ����ָ���Ա

// ������
// ��������͹���������ʵ��
public:
	//�����Ĵ���ί�и�ActionDelegator_T����
protected:
	ActionParams_T&	GetActionParams(VOID) {return m_ActionParams;}
	ActionLogic_T const* GetActionLogic(VOID) {return m_pActionLogic;}
	VOID SetActionLogic(ActionLogic_T const* pLogic) {m_pActionLogic = pLogic;}
private:
	ActionParams_T			m_ActionParams;
	ActionLogic_T const* 	m_pActionLogic;
// �������ԵĴ���
public:
	VOID MenPai_OnDamage(INT& nDamage);
	VOID MenPai_OnDamageTarget(Obj_Character& rTar, INT& nDamage);
	VOID MenPai_OnMyTrapActivated(Obj_Special& rTrap);
	VOID MenPai_OnHealTarget(INT nHealed);
	VOID MenPai_OnDepleteStrikePoints(INT nStrikePoints);
protected:
private:

// �������Ժͱ��
public:
	//Obj_Character id �ǵ�ǰ��ɫ��CharacterManager���е������±꣬��ֵ��
	//CharacterManagerͳһ������
	VOID	SetCharacterID( ObjID_t id ){ m_CharacterID = id ; }
	ObjID_t	GetCharacterID( ){ return m_CharacterID ; }

	// ��ȡAI����
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
	// ��ֹ����
	VOID					SetCharacterLogic( ENUM_CHARACTER_LOGIC eLogic );

	virtual VOID			OnCharacterLogicStopped(BOOL bAbortive);

public:
	VOID					FaceTo( const WORLD_POS* pPos );
	VOID					FaceTo( const Obj* pObj );
	BOOL					IsLimitMove( VOID );			// �Ƿ��������ƶ�����
	VOID					DirectMoveTo(const WORLD_POS* pTar) ;
	//˲��
	VOID					DirectTo( const WORLD_POS* pTar, BOOL bMove=TRUE );
	/*
	BOOL					IsLimitUseSkill( VOID );		// �Ƿ�������ʹ�ü��ܵĲ���
	BOOL					IsLimitAttack( VOID );			// �Ƿ������˹���
	BOOL					IsLimitAllHandle( VOID );		// �Ƿ����������в���
	*/

//=========================================================================================
// ������Ϊ���߼�ֻ����Obj�����AI���ܵ���(��ֹ����!!!) --frankwu 06/03/16
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
// ����������Ա����Ϊ����ܷ��񣬷������ﲻ̫�ʺϣ������뼼��ϵͳ�ϲ� --frankwu 06/03/16
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
// �ɼ���
//=========================================================================================
public:
	virtual VOID			UpdateViewCharacter(VOID);

	INT						GetPrevLevel(VOID)const{return m_nView_PrevLevel;}
	INT						GetPrevStealthLevel(VOID)const{return m_nView_PrevStealthLevel;}
	INT						GetPrevDetectLevel(VOID)const{return m_nView_PrevDetectLevel;}

protected:
	INT			m_nView_PrevLevel;					// �ȼ�
	INT			m_nView_PrevStealthLevel;			// ����ȼ�
	INT			m_nView_PrevDetectLevel;			// �����ȼ�
	CMyTimer	m_UpdateViewCharacterTimer;			// ����仯���������

//=========================================================================================
// ��������
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

		// SP---��һ��ŭ���з������Ժ��ٸ�rage�ɡ�
		BOOL		m_bSPUsePercent;
		union
		{
			INT			m_nSP;
			INT			m_nSPPercent;
		};

		// �����
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
	// �Ƿ���Լ���ʹ�ü��ܸ���
	BOOL					IsCanRelive( VOID )const;

	// �õ�������Ϣ
	// bSkillRelive		:	�Ƿ�Ϊ���ܸ���
	virtual const _RELIVE_INFO		*GetReliveInfo( BOOL bSkillRelive )const;

	// ���ø�������
	// bSkillRelive		:	�Ƿ�Ϊ���ܸ���
	// pReliveInfo		:	������Ϣ
	virtual VOID			SetReliveInfo( BOOL bSkillRelive, const _RELIVE_INFO *pReliveInfo );

	// ���ø������Ϣ
	VOID					ResetReliveInfo( VOID );

	//��ȡ����ɵ���ĵ�
	virtual BOOL			GetNearCanDropPos(WORLD_POS& dropPos);
protected:
	BOOL					m_bCanSkillRelive;			// �Ƿ��м��ܸ���
	_RELIVE_INFO			m_SkillReliveInfo;			// ���ܸ������Ϣ


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

	virtual FLOAT			GetDropSearchRange() const {return 20.0f;}  //���������뾶
	virtual INT				GetDropTeamCount() const {return 1;}		//������Ч������
	virtual INT				GetMinDamagePercent() const {return 20;};	//��С��Ѫ�ٷֱ�

	// add by gh for souxia 2010/05/21
	virtual VOID			SetPetZhaoHuanSkillTimes(SkillID_t skillId, SHORT iDeplete){}//���������ٻ����ܴ���
	virtual SHORT			GetPetZhaoHuanSkillTimes(SkillID_t skillId){return 0;}		//��ȡ�����ٻ����ܴ���
	
	virtual VOID			SetZuoJiZhaoHuanSkillTimes(SkillID_t skillId, SHORT iDeplete){}//���������ٻ����ܴ���
	virtual SHORT			GetZuoJiZhaoHuanSkillTimes(SkillID_t skillId){return 0;}	//��ȡ�����ٻ����ܴ���

protected:
	INT						m_nMoodState;	// ����״̬

public:
//��Ʒ������ӿ�
	virtual	BOOL			IsCarrayItem(UINT ItemType,UINT ItemCount)	{ return TRUE;}
	virtual _ITEM_VALUE		ItemValue( BYTE AttrType )
	{//AttrType=enum ITEM_ATTRIBUTE 
		_ITEM_VALUE a ;
		a.CleanUp() ;
		return a ;
	};

///////////////////////////////////////////////////////////////////////////////////////
// �ı��ٶȲ���
public:
	enum ENUM_MOVE_MODE{
		MOVE_MODE_HOBBLE = 0,	// ����
		MOVE_MODE_WALK,			// ��	
		MOVE_MODE_RUN,			// ��
		MOVE_MODE_SPRINT,		// ����
	};

	VOID SetMoveMode(ENUM_MOVE_MODE eMoveMode) { m_eMoveMode = eMoveMode; }
    ENUM_MOVE_MODE GetMoveMode(VOID) const { return m_eMoveMode; }

private:
	ENUM_MOVE_MODE m_eMoveMode;

///////////////////////////////////////////////////////////////////////////////////////
protected :
	CMyTimer				m_SendMsgTimer ;	// ����������Ϣ�ļ�ʱ
	BOOL					m_bSendMsgDirty;	// �Ƿ��͹�������Ϣ

	CMyTimer				m_AttrMsgTimer ;				// ����ˢ����Ϣ�ļ�ʱ
	BYTE					m_AttrMsgToTeamIntervalCount;	// ����ˢ����Ϣˢ�µ���Ա�ļ������
	BYTE					m_PublicAttrVersion;			// �������Եİ汾
	BYTE					m_ProtectedAttrVersion;			// �������Եİ汾

protected :
	ObjID_t					m_CharacterID ;		// ��ɫID
	CMyTimer				m_RecoverTimer ;	// �ظ�ʱ�������
	_LOGICPARAM_MOVE		m_paramLogic_Move;
	ENUM_CHARACTER_LOGIC	m_eCharacterLogic;
	BOOL					m_bCharacterLogicStopped;

	AI_Character*			m_pAI_Character;	//AI����

public:
	virtual ObjID_t			GetOwnerID( VOID ) const { return INVALID_ID; }
	virtual Obj_Character	*GetOwner( VOID ) { return NULL; }

protected :
	_CAMP_DATA				m_CampDataRefix;

//��ɫ���Բ���
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
	virtual VOID	SetBaseMountID(INT const nID){}; //����������޸�
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
	VOID			SetBaseExpRefix(INT const nID){}; //����������޸�
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

	//���̼���
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

	//Զ�̼���
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

	//�����ڹ�����
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

	//�־忹��
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


	//���Կ���
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

	//��Ĭ����
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
	
	//��е����
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

	//�����ٿ���
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

	//��ʩ������
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
	
	//�����ٿ���
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
	// Attr2: �����ڹ�������
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

	//�����ڹ�
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
	//��������
	INT		GetIntAttr(CharIntAttrs_T::Index_T const nIdx) const {return m_IntAttrs.GetAttr(nIdx);};
	VOID 	SetIntAttr(CharIntAttrs_T::Index_T const nIdx, INT const nValue){m_IntAttrs.SetAttr(nIdx, nValue);};
	VOID 	ResetIntAttrs(VOID);
	
	//������������
	INT		GetIntAttrRefix(CharIntAttrRefixs_T::Index_T const nIdx) const {return m_IntAttrRefixs.GetAttr(nIdx);};
	VOID	SetIntAttrRefix(CharIntAttrRefixs_T::Index_T const nIdx, INT const nValue){m_IntAttrRefixs.SetAttr(nIdx,nValue);};
	VOID 	ResetIntAttrRefixs(VOID);

	//BOOL����
	BOOL	GetBoolAttr(CharBoolAttrs_T::Index_T const nIdx) const{return m_BoolAttrs.GetAttr(nIdx);};
	VOID	SetBoolAttr(CharBoolAttrs_T::Index_T const nIdx, BOOL const bFlag) {m_BoolAttrs.SetAttr(nIdx, bFlag);};
	VOID	ResetBoolAttr(VOID);

	//�������е�����
	VOID	MarkAllIntAttrDirtyFlags(VOID) {m_IntAttrDirtyFlags.MarkAllFlags();};
	VOID	MarkAllBoolAttrDirtyFlags(VOID) {m_BoolAttrDirtyFlags.MarkAllFlags();};	
	VOID	MarkAllIntAttrRefixDirtyFlags(VOID) {m_IntAttrRefixDirtyFlags.MarkAllFlags();};

private:
	//������������
	BOOL	GetIntAttrDirtyFlag(CharIntAttrs_T::Index_T const nIdx) const {return m_IntAttrDirtyFlags.GetFlagByIndex(nIdx);};
	VOID	ClearIntAttrDirtyFlag(CharIntAttrs_T::Index_T const nIdx) {m_IntAttrDirtyFlags.ClearFlagByIndex(nIdx);};
	VOID	MarkIntAttrDirtyFlag(CharIntAttrs_T::Index_T const nIdx) {m_IntAttrDirtyFlags.MarkFlagByIndex(nIdx);};


	//BOOL��������
	BOOL	GetBoolAttrDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) const {m_BoolAttrDirtyFlags.GetFlagByIndex(nIdx);};
	VOID	ClearBoolAttrDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) {m_BoolAttrDirtyFlags.ClearFlagByIndex(nIdx);};
	VOID	MarkBoolAttrDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) {m_BoolAttrDirtyFlags.MarkFlagByIndex(nIdx);};

	//������������������
	BOOL	GetIntAttrRefixDirtyFlag(CharIntAttrRefixs_T::Index_T const nIdx) const {return m_IntAttrRefixDirtyFlags.GetFlagByIndex(nIdx);};
	VOID	ClearIntAttrRefixDirtyFlag(CharIntAttrRefixs_T::Index_T const nIdx) {m_IntAttrRefixDirtyFlags.ClearFlagByIndex(nIdx);};
	VOID	MarkIntAttrRefixDirtyFlag(CharIntAttrRefixs_T::Index_T const nIdx) {m_IntAttrRefixDirtyFlags.MarkFlagByIndex(nIdx);};
/*
	//BOOL��������������
	BOOL	GetBoolAttrRefixDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) const {return m_BoolAttrRefixDirtyFlags.GetFlagByIndex(nIdx);};
	VOID	ClearBoolAttrRefixDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) {m_BoolAttrRefixDirtyFlags.ClearFlagByIndex(nIdx);};
	VOID	MarkBoolAttrRefixDirtyFlag(CharBoolAttrs_T::Index_T const nIdx) {m_BoolAttrRefixDirtyFlags.MarkFlagByIndex(nIdx);};
	VOID	MarkAllBoolAttrRefixDirtyFlags(VOID) {m_BoolAttrRefixDirtyFlags.MarkAllFlags();};
*/
private :
	//��������
	CharIntAttrs_T m_IntAttrs;
	//������������
	CharIntAttrRefixs_T m_IntAttrRefixs;
	//������������
	typedef BitFlagSet_T<CharIntAttrs_T::MAX_INDEX> CharIntAttrDirtyFlags_T;
	CharIntAttrDirtyFlags_T m_IntAttrDirtyFlags;
	//BOOL����
	CharBoolAttrs_T m_BoolAttrs;
	//BOOL��������
	typedef BitFlagSet_T<CharBoolAttrs_T::MAX_INDEX> CharBoolAttrDirtyFlags_T;
	CharBoolAttrDirtyFlags_T m_BoolAttrDirtyFlags;
	//������������������
	typedef BitFlagSet_T<CharIntAttrRefixs_T::MAX_INDEX> CharIntAttrRefixDirtyFlags_T;
	CharIntAttrRefixDirtyFlags_T m_IntAttrRefixDirtyFlags;
/*	//BOOL��������������
	typedef BitFlagSet_T<CharBoolAttrs_T::MAX_INDEX> CharBoolAttrRefixDirtyFlags_T;
	CharBoolAttrRefixDirtyFlags_T m_BoolAttrRefixDirtyFlags;
*/
/////////////////////////////////////////////////////////////////////////////////
//obj �������Խӿ�
	
/////////////////////////////////////////////////////////////////////////////////
//character �������Խӿ�
public :
	INT  GetHorseDataIndex( INT iIndex );
	VOID SetHorseDataIndex( INT iIndex, INT iVal );
	VOID CleanupHorseBuff();
protected:
	//�����߶���ָ�룬ΪĿǰ�ĳ���ṹ���б�ͨ
	//�ڱ��������ڲ���������߶���ָ��,��ָ�벻�ǹ̶�����ģ�����Ϊ��ͬ�������Ļ���
	//�ɲ�ͬ��context���������Accept�������иı�
	CharacterVisitor* m_pVisitor;
	//�������ʹ�õı������ܵ�dataIndex
	INT aHorseDataIndex[MAX_HORSE_DATA_INDEX];
};
#endif
