#include "stdafx.h"
#include "AI_Human.h"
#include "AI_Pet.h"
#include "PlayerPool.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "GCNewPlayer.h"
#include "GCNewPlayer_Death.h"
#include "GCNewPlayer_Move.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GCOtherAttack.h"
#include "GCErrorAttack.h"
#include "GCOperateResult.h"
#include "Config.h"
#include "Obj_Monster.h"
#include "GCErrorSkill.h"
#include "GCOtherSkill.h"
#include "GameTable.h"
#include "GCSkillPrepare.h"
#include "GCLevelUp.h"
#include "AreaManager.h"
#include "GCDetailEquipList.h"
#include "GCCharStopAction.h"

#include "GCMissionAdd.h"
#include "GCMissionRemove.h"
#include "GCMissionModify.h"
#include "GCMissionHaveDoneFlag.h"
//#include "GCAddCanPickMissionItem.h"
//#include "GCRemoveCanPickMissionItem.h"

#include "ItemManager.h"
#include "ItemRuler.h"
#include "GCDiscardItemResult.h"
#include "AbilityManager.h"
#include "Ability.h"
#include "Obj_Platform.h"
#include "Obj_Pet.h"

#include "WGRetUserData.h"
#include "GWCallOfHuman.h"
#include "GCAbilityResult.h"
#include "GCAbilityAction.h"
#include "GCCoolDownUpdate.h"
#include "GCDetailXinFaList.h"
#include "GCDetailSkillList.h"
#include "GWCityOpt.h"
#include "TimeManager.h"
#include "GCNotifyEquip.h"
#include "AvoidOverlap.h"
#include "GCItemInfo.h"
#include "Obj_Character.h"
#include "ScriptDef.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "GCDetailAttrib_Pet.h"
#include "GCCharBaseAttrib.h"
#include "GCReturnTeamFollow.h"
#include "GWStopTeamFollow.h"
#include "GCTeamFollowList.h"
#include "Obj_Pet.h"
#include "GCNotifyChangeScene.h"
#include "PetManager.h"
#include "GCManipulatePetRet.h"
#include "GCRemovePet.h"
#include "SkillInfo.h"
#include "Obj_Bus.h"
#include "BusRule.h"

#include "HumanDB.h"
#include "AvoidOverlap.h"
#include "ShopManager.h"
#include "Team.h"

#include "ExchangBox.h"			//交易盒
#include "StallBox.h"			//摊位盒
#include "GWCityApplyNewCity.h" //申请建立城市
#include "GWCityClose.h" //申请建立城市
#include "GCCharAllTitles.h"
#include "HumanItemContainer.h"
#include "HumanPetContainer.h"

#include "Attributes.h"
#include "HumanRelation.h"

#include "Obj_Human.h"
#include "Obj_Human.inl"
#include "GWLevelUp.h"
#include "GCLevelUpResult.h"
#include "GWChangeMenpai.h"
#include "ActionDelegator.h"
#include "ScriptLogic.h"
#include "LogDefine.h"
#include "GWUpdateTitle.h"
#include "GameStruct_Pet.h"
#include "GCNotifyGoodBad.h"

#include "HumanSkillStudy.h"
#include "HorseManager.h"

#include "HorseMatingRule.h"

#include "GCCharEquipment.h"
#include "Obj_Horse.h"
#include "LogDefine.h"
#include "GWTeamLeaderDie.h"
#include "SouXia.h"
#include "GCSouXiaUpdate.h"
#include "GCSouXiaSkillUpdate.h"
//////////////////////////////////////////////////////
// 与客户端同步备份数据
//////////////////////////////////////////////////////
_HUMAN_ATTR_BACKUP::_HUMAN_ATTR_BACKUP(VOID)
{
	m_bNameModified		= FALSE;
	m_bTitleModified	= FALSE;
	m_Level				= -1;
	m_Exp				= -1;
	m_HP				= -1;
	m_MP				= -1;
	m_nPneuma			= -1;
	m_MaxHp				= -1;
	m_MaxMp				= -1;
	m_MaxPneuma			= -1;
	m_MoveSpeed			= -1;
	m_nDataID			= INVALID_ID;
	m_nPortraitID		= INVALID_ID;
	m_nModelID			= INVALID_ID;
	m_nMountID			= INVALID_ID;
	m_uFaceMeshID		= INVALID_ID;
	m_uHairMeshID		= INVALID_ID;
	m_uHairColor		= 0xFFFFFFFF;
	m_nStealthLevel		= -1;
	m_nMoodState		= INVALID_MOOD_STATE;

	m_Money				= -1;
	m_nGoodBadValue		= -1;
	m_nStr				= -1;
	//m_nSpr				= -1;
	m_nCon				= -1;
	m_nInt				= -1;
	m_nDex				= -1;
	m_nPointRemain		= -1;
	m_nSkillPointRemain = -1;
	
	memset(m_nS_SkillPoints, 0, sizeof(INT)*COMMON_S_NUMBER);
	m_nReHpSpeed		= -1;
	m_nReMpSpeed		= -1;
	m_nReVigorSpeed		= -1;
	//m_nAttPhysics		= -1;
	m_nAttNear          = -1;
	m_nAttFar           = -1;

	m_nAttMagicNear		= -1;
	m_nAttMagicFar		= -1;

	m_nDefNear          = -1;
	m_nDefFar           = -1;
	
	m_nDefMagicNear		= -1;
	m_nDefMagicFar		= -1;

	m_nToughness		= -1;
	m_nHit				= -1;
	m_nMiss				= -1;
	m_nCritic			= -1;
	m_nCriticHurt		= -1;

	m_MaxRage			= -1;
	m_nRage				= -1;
	m_nStrikePoint		= -1;

	m_nAttSpeed			= -1;

	m_nAttGold			= -1;
	m_nDefGold			= -1;

	m_nAttWood			= -1;
	m_nDefWood			= -1;

	m_nAttWater			= -1;
	m_nDefWater			= -1;

	m_nAttFire			= -1;
	m_nDefFire			= -1;

	m_nAttSoil			= -1;
	m_nDefSoil			= -1;

	m_nNearReduce		= -1;		//近程减免
	m_nFarReduce		= -1;		//远程减免

	m_nMagicNearReduce	= -1;		//近程内功减免
	m_nMagicFarReduce	= -1;		//近程外功减免

	m_nDreadReduce		= -1;		//抵抗恐惧
	m_nComaReduce		= -1;		//抵抗昏迷
	m_nHushReduce		= -1;		//抵抗沉默
	m_nUnarmReduce		= -1;		//抵抗缴械
	m_nAttSpeedResist	= -1;	//抗攻击减速
	m_nSkillSpeedResist	= -1;//抗施法
	m_nMoveSpeedResist	= -1;	//抗减速

	m_nCountry          = -1;
	m_nMenpai			= -1;
	//m_CampData
	m_nOwnerID			= INVALID_ID;
	m_guidCurrentHorse.Reset();

	m_Vigor				= -1;
	m_MaxVigor			= -1;
	m_Energy			= -1;
	m_MaxEnergy			= -1;

	m_bLimitMove		= FALSE;
	m_bCanActionFlag1	= FALSE;
	m_bCanActionFlag2	= FALSE;

	m_bMissionHaveDoneFlagsChanged	= FALSE;


	m_bMainWeaponModified		= FALSE;		//主手
	m_bAssiWeaponModified		= FALSE;		//副手
	m_bCapModified				= FALSE;				//帽子
	m_bShoulderModified			= FALSE;		//衬肩
	m_bArmorModified			= FALSE;			//衣服
	m_bCuffModified				= FALSE;			//护腕
	m_bBootModified				= FALSE;
	m_bSuitModified				= FALSE;			// 外装 - 资源表ID
	m_bRiderModified			= FALSE;			// 座骑 - 资源表ID

	m_bStallIsOpen		= FALSE;
	m_bStallNameChanged	= FALSE;
	m_bStallIsPublic	= FALSE;

	m_GuildID			=	INVALID_ID;
}

VOID _HUMAN_ATTR_BACKUP::CleanUp(VOID)
{
	m_bNameModified		= FALSE;
	m_bTitleModified	= FALSE;
	m_Level				= -1;
	m_Exp				= -1;
	m_HP				= -1;
	m_MP				= -1;
	m_nPneuma			= -1;
	m_MaxHp				= -1;
	m_MaxMp				= -1;
	m_MaxPneuma			= -1;
	m_MoveSpeed			= -1;
	m_nDataID			= INVALID_ID;
	m_nPortraitID		= INVALID_ID;
	m_nModelID			= INVALID_ID;
	m_nMountID			= INVALID_ID;
	m_uFaceMeshID		= INVALID_ID;
	m_uHairMeshID		= INVALID_ID;
	m_uHairColor		= 0xFFFFFFFF;
	m_nStealthLevel		= -1;
	m_nMoodState		= INVALID_MOOD_STATE;

	m_Money				= -1;
	m_nGoodBadValue		= -1;
	m_nStr				= -1;
	//m_nSpr				= -1;
	m_nCon				= -1;
	m_nInt				= -1;
	m_nDex				= -1;
	m_nPointRemain		= -1;
	m_nSkillPointRemain = -1;
	memset(m_nS_SkillPoints, 0, sizeof(INT)*COMMON_S_NUMBER);

	m_nReHpSpeed		= -1;
	m_nReMpSpeed		= -1;
	m_nReVigorSpeed		= -1;
	m_nAttNear          = -1;
	m_nAttFar           = -1;
	
	m_nAttMagicNear		= -1;
	m_nAttMagicFar		= -1;
	
	m_nDefNear          = -1;
	m_nDefFar           = -1;

	m_nDefMagicNear		= -1;
	m_nDefMagicFar		= -1;

	m_nToughness		= -1;
	m_nHit				= -1;
	m_nMiss				= -1;
	m_nCritic			= -1;
	m_nCriticHurt		= -1;

	m_MaxRage			= -1;
	m_nRage				= -1;
	m_nStrikePoint		= -1;

	m_nAttSpeed			= -1;

	m_nAttGold			= -1;
	m_nDefGold			= -1;

	m_nAttWood			= -1;
	m_nDefWood			= -1;

	m_nAttWater			= -1;
	m_nDefWater			= -1;

	m_nAttFire			= -1;
	m_nDefFire			= -1;

	m_nAttSoil			= -1;
	m_nDefSoil			= -1;

	m_nNearReduce		= -1;		//近程减免
	m_nFarReduce		= -1;		//远程减免

	m_nMagicNearReduce	= -1;		//近程内功减免
	m_nMagicFarReduce	= -1;		//近程外功减免

	m_nDreadReduce		= -1;		//抵抗恐惧
	m_nComaReduce		= -1;		//抵抗昏迷
	m_nHushReduce		= -1;		//抵抗沉默
	m_nUnarmReduce		= -1;		//抵抗缴械
	m_nAttSpeedResist	= -1;	//抗攻击减速
	m_nSkillSpeedResist	= -1;//抗施法
	m_nMoveSpeedResist	= -1;	//抗减速


	m_nCountry          = -1;
	m_nMenpai			= -1;
	m_CampData.CleanUp();
	m_nOwnerID			= INVALID_ID;
	m_guidCurrentHorse.Reset();

	m_Vigor				= -1;
	m_MaxVigor			= -1;
	m_Energy			= -1;
	m_MaxEnergy			= -1;
    m_nGP               = 0;
    m_nHonor            = 0;
    m_nPK               = 0;


	m_bLimitMove		= FALSE;
	m_bCanActionFlag1	= FALSE;
	m_bCanActionFlag2	= FALSE;

	m_bMissionHaveDoneFlagsChanged	= FALSE;

	m_bMainWeaponModified		= FALSE;		//主手
	m_bAssiWeaponModified		= FALSE;		//副手
	m_bCapModified				= FALSE;				//帽子
	m_bShoulderModified			= FALSE;		//衬肩
	m_bArmorModified			= FALSE;			//衣服
	m_bCuffModified				= FALSE;			//护腕
	m_bBootModified				= FALSE;
	m_bSuitModified				= FALSE;			// 外装 - 资源表ID
	m_bRiderModified			= FALSE;			// 座骑 - 资源表ID

	m_bStallIsOpen		= FALSE;
	m_bStallNameChanged	= FALSE;

	m_bStallIsPublic	= FALSE;	

	m_GuildID			=	INVALID_ID;
}

using namespace Combat_Module;
using namespace Action_Module;
using namespace Script_Module;
using namespace Combat_Module::Skill_Module;

extern ENUM_REFESH_ATTR_TYPE CalcRefeshAttrType(Obj_Character *pSour, Obj_Character *pTarget );

Obj_Human::Obj_Human( )
{
__ENTER_FUNCTION
	m_EquipImpactNum = 0;
	m_EffectSkillNum = 0;
	m_iTargetForExp = 0;
	m_pHumanNode = new _OBJ_LIST_NODE(this) ;
	Assert( m_pHumanNode ) ;

	m_bCanSkillRelive = FALSE;
	//m_DefaultReliveInfo.Reset();
	//m_SkillReliveInfo.Reset();

	m_EquipVer	=	0;

	//m_uMissionIndex_KillObjectEvent_Num	= 0;
	//m_uMissionIndex_EnterAreaEvent_Num		= 0;
	//m_uMissionIndex_ItemChangedEvent_Num	= 0;

	//////////////////////////////////////////////////////////////////////////
	//辅助功能开发相关

	//当前商店id
	m_pShop		=	NULL;
	BankIsOpen	=	FALSE;

	//交易盒初始化
	m_ExchangBox.Init(this);

	//摊位盒初始化
	m_StallBox.Init(this);

	//开宝箱相关容器初始化
	m_HumanOpenBox.Init();

	//辅助功能开发相关
	//////////////////////////////////////////////////////////////////////////

	
	m_pAvoidOverlap = NULL;
	m_pAvoidOverlap = new AvoidOverlap;
	Assert( m_pAvoidOverlap );

	CleanUp( ) ;

	m_pUseTimer = new INT[g_Config.m_ConfigInfo.m_nMaxHumanUseTimer];
	memset((VOID*)m_pUseTimer,-1,sizeof(INT) * g_Config.m_ConfigInfo.m_nMaxHumanUseTimer);

	m_uAreaID = UINT_MAX;
	m_uAreaScript = UINT_MAX;

	//基本杂项物品包
	_HUMAN_ITEMCONTAINER_INIT	containInit;
	containInit.m_nContainerSize	=	m_DB.GetBaseBagSize();
	containInit.m_pHumanDB			=	&m_DB;
	containInit.m_ICTType			=	ICT_BASE_CONTAINER;
	containInit.m_DBOffSet			=	BASE_CONTAINER_OFFSET;

	m_BaseItem.Init(&containInit);
	
	
	for(UINT i=0;i<m_DB.GetBaseBagSize();i++)
	{
		_ITEM_INIT	itemInit(m_DB.GetBagItem(BASE_CONTAINER_OFFSET+i));
		m_BaseItem.GetItem(i)->Init(&itemInit);
	}
	
	//基本杂项物品包
	_HUMAN_ITEMCONTAINER_INIT	ExtracontainInit;
	ExtracontainInit.m_nContainerSize	=	m_DB.GetExtraContainerSize();
	ExtracontainInit.m_pHumanDB			=	&m_DB;
	ExtracontainInit.m_ICTType			=	ICT_EXTRA_CONTAINER;
	ExtracontainInit.m_DBOffSet			=	EXTRA_CONTAINER_OFFSET;

	m_ExtraContainerItem.Init(&ExtracontainInit);


	for(UINT i=0;i<m_DB.GetExtraContainerSize();i++)
	{
		_ITEM_INIT	itemInit(m_DB.GetBagItem(EXTRA_CONTAINER_OFFSET+i));
		m_ExtraContainerItem.GetItem(i)->Init(&itemInit);
	}

	//杂项物品扩展包1
	_HUMAN_ITEMEXTRABAG_INIT	ExtraBagInit;
	ExtraBagInit.m_nContainerSize	=	0;
	ExtraBagInit.m_pHumanDB			=	&m_DB;
	ExtraBagInit.m_ICTType			=	ICT_EXTRABAG_CONTAINER;
	ExtraBagInit.m_DBOffSet			=	EXTRA_BAG1_OFFSET;
	ExtraBagInit.m_pExtraBagItem	=   m_ExtraContainerItem.GetItem(BAG_EXTRA1);

	m_ExtraBagItem[BAG_EXTRA1].Init(&ExtraBagInit);

	
	//杂项物品扩展包2
	ExtraBagInit.m_nContainerSize	=	0;
	ExtraBagInit.m_pHumanDB			=	&m_DB;
	ExtraBagInit.m_ICTType			=	ICT_EXTRABAG_CONTAINER;
	ExtraBagInit.m_DBOffSet			=	EXTRA_BAG2_OFFSET;
	ExtraBagInit.m_pExtraBagItem	=   m_ExtraContainerItem.GetItem(BAG_EXTRA2);
	
	m_ExtraBagItem[BAG_EXTRA2].Init(&ExtraBagInit);
	
	//杂项物品扩展包3
	ExtraBagInit.m_nContainerSize	=	0;
	ExtraBagInit.m_pHumanDB			=	&m_DB;
	ExtraBagInit.m_ICTType			=	ICT_EXTRABAG_CONTAINER;
	ExtraBagInit.m_DBOffSet			=	EXTRA_BAG3_OFFSET;
	ExtraBagInit.m_pExtraBagItem	=   m_ExtraContainerItem.GetItem(BAG_EXTRA3);

	m_ExtraBagItem[BAG_EXTRA3].Init(&ExtraBagInit);

	//杂项物品扩展包4
	ExtraBagInit.m_nContainerSize	=	0;
	ExtraBagInit.m_pHumanDB			=	&m_DB;
	ExtraBagInit.m_ICTType			=	ICT_EXTRABAG_CONTAINER;
	ExtraBagInit.m_DBOffSet			=	EXTRA_BAG4_OFFSET;
	ExtraBagInit.m_pExtraBagItem	=   m_ExtraContainerItem.GetItem(BAG_EXTRA4);

	m_ExtraBagItem[BAG_EXTRA4].Init(&ExtraBagInit);

	//材料物品包
	//containInit.m_nContainerSize	=	m_DB.GetMatBagSize();
	//containInit.m_ICTType			=	ICT_MAT_CONTAINER;
	//containInit.m_DBOffSet			=	MAT_CONTAINER_OFFSET;
	//
	//m_MatItem.Init(&containInit);


	//
	//for(UINT i=0;i<m_DB.GetMatBagSize();i++)
	//{
	//	_ITEM_INIT	itemInit(m_DB.GetBagItem(MAT_CONTAINER_OFFSET+i));
	//	m_MatItem.GetItem(i)->Init(&itemInit);
	//}

	////任务物品包
	//containInit.m_nContainerSize	=	m_DB.GetTaskBagSize();
	//containInit.m_ICTType			=	ICT_TASK_CONTAINER;
	//containInit.m_DBOffSet			=	TASK_CONTAINER_OFFSET;
	//
	//m_TaskItem.Init(&containInit);

	//for(UINT i=0;i<m_DB.GetTaskBagSize();i++)
	//{
	//	_ITEM_INIT	itemInit(m_DB.GetBagItem(TASK_CONTAINER_OFFSET+i));
	//	m_TaskItem.GetItem(i)->Init(&itemInit);
	//}

	containInit.m_nContainerSize	=	HEQUIP_NUMBER;
	containInit.m_ICTType			=	ICT_EQUIP_CONTAINER;
	//装备
	m_EquipItem.Init(&containInit);
	for(UINT i=0;i<HEQUIP_NUMBER;i++)
	{
		_ITEM_INIT	itemInit(m_DB.GetEquipItem((HUMAN_EQUIP)i));
		m_EquipItem.GetItem(i)->Init(&itemInit);
	}

    // [7/8/2010 陈军龙]--------------------------------------
	containInit.m_nContainerSize	=	MAX_MOUNT_SIZE;
	containInit.m_ICTType			=	ICT_MOUNT_CONTAINER;
	//坐骑
	m_MountItem.Init(&containInit);
	for(UINT i=0;i<MAX_MOUNT_SIZE;i++)
	{
		_ITEM_INIT	itemInit(m_DB.GetMountItem(i));
		m_MountItem.GetItem(i)->Init(&itemInit);
	}
	// --------------------------------------[7/8/2010 陈军龙]

	containInit.m_nContainerSize	=	MAX_BANK_SIZE;
	containInit.m_ICTType			=	ICT_BANK_CONTAINER;
	//银行
	m_BankItem.Init(&containInit);
	for(int i=0;i<MAX_BANK_SIZE;i++)
	{
		_ITEM_INIT	itemInit(m_DB.GetBankItem(i));
		m_BankItem.GetItem(i)->Init(&itemInit);
	}

	_HUMAN_PETCONTAINER_INIT	PetContainInit;
	PetContainInit.m_nContainerSize	=	HUMAN_PET_MAX_COUNT;
	PetContainInit.m_pHumanDB		=	&m_DB;
	PetContainInit.m_ICTType		=	ICT_HUMAN_PET_CONTAINER;

	//宠物
	m_PetItem.Init(&PetContainInit);
	for(UINT i = 0 ;i<HUMAN_PET_MAX_COUNT;i++)
	{
		_ITEM_INIT itemInit(m_DB.GetPet(i));
		m_PetItem.GetItem(i)->Init(&itemInit);
	}

	//骑乘
//	memset(m_pHorse, sizeof(Obj_Horse*) * MAX_MONSTER_HORSE_TAKE);

	//帮会
    this->SetGuildID(INVALID_GUILD_ID);

	m_bIsPasswdUnlock = FALSE;
	m_bTeamFollowSpeedUp = FALSE;
	m_OutofTeamFollowRangeTime = 0;

	m_GUIDOfCallUpPet.Reset();
	m_pAI_Character = new AI_Human ;
	Assert( m_pAI_Character ) ;
	m_ActiveHorse.SetOnwer( this );
	m_iDataID = INVALID_ID;
	m_nCurSouXiaCount = 0;

	m_CreateCopySceneID = INVALID_ID ;
__LEAVE_FUNCTION
}

Obj_Human::~Obj_Human( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

	SAFE_DELETE( m_pHumanNode ) ;

	SAFE_DELETE( m_pAvoidOverlap ) ;

	SAFE_DELETE( m_pUseTimer ) ;

__LEAVE_FUNCTION
}

PlayerID_t Obj_Human::GetPlayerID( )
{
	return m_pPlayer->PlayerID() ;
}

UINT Obj_Human::GetUniqueID() const
{
	return GetGUID();
}

BOOL Obj_Human::Init( const _OBJ_INIT *pInit )
{
	__ENTER_FUNCTION


	if ( !Obj_Character::Init( pInit ) )
		return FALSE;


	_OBJ_HUMAN_INIT *pHumanInit = (_OBJ_HUMAN_INIT*)pInit;
	if ( pHumanInit != NULL )
	{
		if ( pHumanInit->m_pUseData != NULL )
		{
			const FULLUSERDATA	*pData = pHumanInit->m_pUseData;

			GetDB()->WriteHumanDB(&pData->m_Human);
			GetDB()->WriteEquipDB(&pData->m_Equip);
			GetDB()->WriteBagDB(&pData->m_Bag);
			GetDB()->WriteBankDB(&pData->m_Bank);
			GetDB()->WriteSkillDB(&pData->m_Skill);
			GetDB()->WriteCooldownDB(&pData->m_Cooldown);	
			GetDB()->WriteAbilityDB(&pData->m_Ability);
			GetDB()->WriteXinFaDB(&pData->m_XinFa);
			GetDB()->WriteImpactDB(&pData->m_Impact);
			GetDB()->WriteMissionDB(&pData->m_Mission);
			memcpy( GetDB()->m_dbPetList, &(pData->m_PetList ), sizeof(_PET_DB_LIST_LOAD) ) ;
			GetDB()->WriteRelationDB(&pData->m_Relation);
			GetDB()->WriteSettingDB(&pData->m_Setting);
			GetDB()->WritePrivateInfo(&pData->m_PrivateInfo);
			GetDB()->WriteSouXiaDB(&pData->m_SouXia); // add by gh
			GetDB()->WriteMountDB(&pData->m_Mount);   // [7/16/2010 陈军龙]

			if(pData->m_bIsPasswdUnlock) {__UnlockPassword();}
		}
	}
	
	ReInitExtraBag();

	m_BankItem.SetSize(__GetBankSize());
	//物品Runtime数据填充 DB--->Runtime

	InitBackupAttr();
	
	UINT	RefreshIntervalTime		= g_Config.m_ConfigInfo.m_DefaultRefreshRate;
	UINT	CreateTime				= g_pTimeManager->CurrentTime();

	m_TeamInfo.CleanUp();
	m_TeamInfo.SetMyGUID( GetGUID() );

	m_HumanRelation.Init( this ) ;//初始化联系人功能模块

	m_RefeshTimer.BeginTimer(RefreshIntervalTime,CreateTime);

	UINT	CheckZoneIntervalTime		= g_Config.m_ConfigInfo.m_DefaultCheckZoneRate;
	m_CheckZoneTimer.BeginTimer(CheckZoneIntervalTime,CreateTime);

	m_AttrBackUp.m_bMissionHaveDoneFlagsChanged	= TRUE;

	UINT	AttrUpdateIntervalTime = DB_2_SM_TIME;
	m_UpdateTimer.BeginTimer(AttrUpdateIntervalTime,CreateTime);

	//////////////////////////////////////////////////////////////////////////
	//辅助功能开发相关
	//初始化商品的回购列表
	InitialSoldList();

	if( (GetMoney() + __GetBankMoney())> (UINT)g_LevelMoneyTbl.Get(GetLevel()) )
	{
		//原则是先扣身上的钱
		UINT	iMaxMoney = g_LevelMoneyTbl.Get(GetLevel());
		if(GetMoney()>iMaxMoney)
		{//身上的钱已经大于上限
			SetMoney(iMaxMoney);
		}
		else
		{//清空身上的，再在银行里减
			__SetBankMoney(__GetBankMoney() - (iMaxMoney - GetMoney()) );
			SetMoney(0);
		}
	}
	
    SetTitleLoginTime(g_pTimeManager->GetANSITime());
	//辅助功能开发相关
	//////////////////////////////////////////////////////////////////////////

	m_bTeamFollowSpeedUp = FALSE;
	m_OutofTeamFollowRangeTime = 0;
	m_bCanIncreaseRage = TRUE;

	m_EquipImpactNum = 0;
	m_EffectSkillNum = 0;
	m_VERecoverTimer.BeginTimer(g_Config.m_ConfigInfo.m_HumanVERecoverInterval, g_pTimeManager->CurrentTime());

	m_CreateCopySceneID = INVALID_ID ;
		
	SKILL_LOG_PARAM oSkillLog;
	XINFA_LOG_PARAM oXinFaLog;

	if( GetDB()->IsFirstEnterScene() )
	{
		ChangeMenpai( &oSkillLog, &oXinFaLog, GetDB()->GetMenPai() );
	}

	// 捜侠录的初始化
	SOUXIA_DATA* pSouXiaData = NULL;
	for( BYTE i=0; i<MAX_SOUXIA_CONTAINER; ++i )
	{
		pSouXiaData = GetDB()->GetSouXiaInfo(i);
		Assert(pSouXiaData);
		if (0 < pSouXiaData->m_SouXiaID)
		{
			SOUXIA_DATA SouXiaData = GetDB()->GetSouXiaDB()->m_SouXiaData[i];
			m_SouXia[i].SetSouXiaData(SouXiaData);
			m_nCurSouXiaCount++;
		}
	}

	return TRUE;

	__LEAVE_FUNCTION
	return FALSE;
}
VOID Obj_Human::OnEnterScene(VOID)
{
	__ENTER_FUNCTION
	if(IsFirstEnterScene())
	{	
		getScene()->GetLuaInterface()->ExeScript_DD( SCENE_SCRIPTID,"OnScenePlayerFirstEnter", (INT)getScene()->SceneID(), (INT)GetID() );
		ClearFirstEnterScene();
	}

	Obj_Character::OnEnterScene();
    SetLoginProtectTime(g_pTimeManager->GetANSITime());

	//技能数据
	Skill_InitSkillList();
	//心法数据
	Skill_InitXinFaList();
	//物品效果刷新
	ItemEffectFlush( ) ;

	INT iHP = GetHP() ;
	if( iHP > GetMaxHP() )
	{
		SetHP(GetMaxHP() );
	}
	INT iMP = GetMP() ;
	if( iMP > GetMaxMP() )
	{
		SetMP(GetMaxMP() ) ;
	}

	INT iPneuma = GetPneuma() ;
	if( iPneuma > GetMaxPneuma() )
	{
		SetPneuma(GetMaxPneuma() ) ;
	}
	
	__LEAVE_FUNCTION
}

VOID Obj_Human::ValidatePlayerID()
{
	
	GetDB()->ValidatePlayerID(GetPlayerID());
}
VOID Obj_Human::InitAIObj( )
{
	Assert( m_pAI_Character );
	m_pAI_Character->Init( this );
}

VOID Obj_Human::CleanUp( VOID )
{
__ENTER_FUNCTION

	// 复活点相关
	m_bCanSkillRelive = FALSE;
	m_DefaultReliveInfo.Reset();
	m_SkillReliveInfo.Reset();

	m_DB.CleanUp() ;

	if( m_pHumanNode->m_pNext != NULL )
	{
		Assert(FALSE) ;
	}
	if( m_pHumanNode->m_pPrev != NULL )
	{
		Assert(FALSE) ;
	}

	m_PlayerID = INVALID_ID ;
	m_AssistantID = INVALID_ID ;
	m_MoveIndex = 0 ;
	
	m_TransSceneID = INVALID_ID ;
	m_EquipVer = 0 ;
	
	
	reset_AbilityOpera( );
	Obj_Character::CleanUp( ) ;
	m_bChangeSceneFlag = FALSE ;
	m_bGod = FALSE ;
	SetRage(0);

	//m_uMissionIndex_KillObjectEvent_Num	= 0;
	//m_uMissionIndex_EnterAreaEvent_Num		= 0;
	//m_uMissionIndex_ItemChangedEvent_Num	= 0;

	m_bIsPasswdUnlock = FALSE;
	m_TeamInfo.CleanUp();
	m_bTeamFollowSpeedUp = FALSE;
	m_OutofTeamFollowRangeTime = 0;
	m_HumanRelation.CleanUp() ;
	
	//m_uCanPickMissionItemIDNum				= 0;
	//UINT i;
	//for ( i = 0; i < MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM; i++ )
	//	m_aCanPickMissionItemList[i]		= UINT_MAX;

	m_uAreaID = UINT_MAX;
	m_uAreaScript = UINT_MAX;
	m_nCommonCooldown = 0;
	memset( m_pPet, 0, sizeof(m_pPet) );
	m_GUIDOfCallUpPet.Reset();
	m_bMatingHorseConfirm = FALSE;

	//开宝箱相关的容器的CleanUp
	m_HumanOpenBox.CleanUp();
	m_nCurSouXiaCount	= 0;
__LEAVE_FUNCTION
}

//INT g_iSceneHeartBeatTime = 0;

BOOL Obj_Human::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)(GetPlayer()) ;
	if( pGamePlayer==NULL || pGamePlayer->GetPlayerStatus()!=PS_SERVER_NORMAL )
	{
		return FALSE ;
	}

	if(FALSE==Obj_Character::HeartBeat(uTime))
	{
		return FALSE;
	}

	if(m_CheckZoneTimer.CountingTimer(uTime))
	{
		CheckArea();
	}

	if(m_RefeshTimer.CountingTimer(uTime))
	{
		RefeshAttrMsg();
        //计时称号
        UpdateTitleTime();
	}

	if(m_UpdateTimer.CountingTimer(uTime))
	{
		m_bUpdateAttr = TRUE;
	}
	


	ValidateShareMem(FALSE); //最后更新数据到共享内存

	HorseHeartBeat( uTime );

	//////////////////////////////////////////////////////////////////////////
	//  [6/4/2008 wuwenbin] 只向World刷新简单的数据，该操作只会修改
	//  USER.m_SimpleUserData, 不会影响FULLUSERDATA
	RefreshAttrToWorld( uTime );


	return TRUE;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID Obj_Human::OnRegisterToZone(VOID)
{
__ENTER_FUNCTION

	Obj_Character::OnRegisterToZone();
	CreatePetOnRegisterZone();

	if(IsDie())
	{
		GetAIObj()->Relive(CITY_POS_RELIVE);
	}

__LEAVE_FUNCTION
}

VOID Obj_Human::OnUnregisterFromZone( )
{
__ENTER_FUNCTION

	Obj_Character::OnUnregisterFromZone();
	
	SaveMonsterPetToDB();

	ReleasePet();
__LEAVE_FUNCTION
}

BOOL Obj_Human::IsFirstEnterScene()
{
	return GetDB()->IsFirstEnterScene();
}

VOID Obj_Human::ClearFirstEnterScene()
{
	if (IsFirstEnterScene())
	{
		GetDB()->ClearFirstEnterScene();
	}
}

// 比较二级密码
BOOL Obj_Human::ComparePasswd( const CHAR* szPasswd )
{
	return ( strcmp(szPasswd, __GetPassword()) == 0 );
}

VOID Obj_Human::RefeshAttrMsg()
{
	INT NowVigor = GetVigor() ;
	NowVigor += 1 ;
	SetVigor( NowVigor ) ;
}

VOID Obj_Human::ValidateShareMem(BOOL bForceAll)
{
	__ENTER_FUNCTION
	
		GetDB()->ValidateShareMem(bForceAll,m_bUpdateAttr);

	__LEAVE_FUNCTION
}
BOOL Obj_Human::HeartBeat_Recover(UINT uTime)
{
	__ENTER_FUNCTION

	int nHPCount=0;
	//int nRageCount=0;
	int nPneumaCount=0;

	m_nElapsedRevocerTick += GetLogicTime();
	//m_nElapsedRageRevocerTick += GetLogicTime();
	m_nElapsedPneumaRevocerTick += GetLogicTime();

	if(FALSE==IsAliveInDeed())
	{
		return TRUE;
	}
	while(0<m_nElapsedRevocerTick)
	{
		m_nElapsedRevocerTick-= g_Config.m_ConfigInfo.m_HumanHGRecoverInterval;
		++nHPCount;
	}
	//while (0<m_nElapsedRageRevocerTick)
	//{
	//	m_nElapsedRageRevocerTick-= g_Config.m_ConfigInfo.m_RAGERecoverInterval;
	//	++nRageCount;
	//}
	while(0<m_nElapsedPneumaRevocerTick)
	{
		m_nElapsedPneumaRevocerTick-= g_Config.m_ConfigInfo.m_HumanPneumaRecoverInterval;
		++nPneumaCount;
	}

	if(0<nHPCount)
	{
		/** 进行回红操作 */
		INT nCur = GetHP( ); 
		INT nMax = GetMaxHP();
		INT nRegenerate = GetHPRegenerate();
		if(nCur<nMax && 0<nRegenerate)
		{
			nCur += nRegenerate*nHPCount ;
			nCur<nMax?nCur:nCur=nMax;
			SetHP(nCur) ;
		}	
		/** 进行回兰操作 */
		nCur = GetMP( ); 
		nMax = GetMaxMP();
		nRegenerate = GetMPRegenerate();
		if(nCur<nMax && 0<nRegenerate)
		{
			nCur += nRegenerate*nHPCount ;
			nCur<nMax?nCur:nCur=nMax;
			SetMP(nCur) ;
		}
	}
	//if (0<nRageCount)
	//{
	//	/** 进行回怒气操作 */
	//	INT nCur = GetRage( ); 
	//	INT nMax = GetMaxRage();
	//	INT nRegenerate = GetRageRegenerate();
	//	if(( 0!=nRegenerate)&&(nCur<=nMax))
	//	{
	//		nCur += nRegenerate*nRageCount ;
	//		nCur<nMax?nCur:nCur=nMax;
	//		if (nCur<0)
	//		{
	//			nCur = 0;
	//		}
	//		SetRage(nCur) ;
	//	}
	//}

	if (0<nPneumaCount)
	{
		/** 进行回元气操作 */
		INT nCur = GetPneuma( ); 
		INT nMax = GetMaxPneuma();
		INT nRegenerate = GetPneumaRegenerate();
		if(( 0!=nRegenerate)&&(nCur<=nMax))
		{
			nCur += nRegenerate*nPneumaCount ;
			nCur<nMax?nCur:nCur=nMax;
			if (nCur<0)
			{
				nCur = 0;
			}
			SetPneuma(nCur) ;
		}
	}

	if(m_VERecoverTimer.CountingTimer(uTime))
	{
		if(GetVigor() != GetMaxVigor())
		{
			INT nVigorRecover = (INT)(((FLOAT)(GetMaxVigor()) / 100.f + 2.f) * (1.f + (FLOAT)(GetVigorRegeneRate()) / 100.f));
			if(nVigorRecover > 0)
			{
				SetVigor(GetVigor() + nVigorRecover);
			}
		}
		if(GetEnergy() != GetMaxEnergy())
		{
			INT nEnergyRecover = (INT)(((FLOAT)(GetMaxEnergy()) / 100.f + 2.f) * (1.f + (FLOAT)(GetEnergyRegeneRate()) / 100.f));
			if(nEnergyRecover > 0)
			{
				SetEnergy(GetEnergy() + nEnergyRecover);
			}
		}
	}
	__LEAVE_FUNCTION
	return TRUE;
}


VOID Obj_Human::CheckArea()
{
__ENTER_FUNCTION

	if ( getZoneID() == INVALID_ID )
		return;

	const Area* pArea;
	UINT uAreaID;

	pArea = getScene()->CheckArea(this);
	if ( pArea != NULL )
	{
		uAreaID = pArea->m_uAreaID;
	}
	else
	{
		uAreaID = UINT_MAX;
	}

	SceneID_t sceneid = getScene()->SceneID() ;
	if ( uAreaID != m_uAreaID )
	{
		LuaInterface* pLuaInterface = getScene()->GetLuaInterface();

		if ( m_uAreaID != UINT_MAX && m_uAreaScript != UINT_MAX )
		{ // OnLeaveZone();
			pLuaInterface->ExeScript_DDD(m_uAreaScript, DEF_ON_LEAVE_AREA_FUNC_NAME, (INT)sceneid, (INT)GetID(), 0);
		}

		m_uAreaID = uAreaID;

		if ( m_uAreaID != UINT_MAX )
		{
			// 设置玩家进入 Area 的时刻
			m_uTimeStamp = NowTime();
			m_uAreaScript = pArea->m_uScriptID;

			OnEnterArea( m_uAreaID );

			// 执行 Area 脚本，这个脚本可能是具有杀死玩家（导致位置变化）或者传送的功能
			// 所以需要在玩家脚本执行结束以后执行
			if ( m_uAreaScript != UINT_MAX )
			{
				pLuaInterface->ExeScript_DDD(m_uAreaScript, DEF_ON_ENTER_AREA_FUNC_NAME, (INT)sceneid, (INT)GetID(), 0);
			}
		}
		else
		{
			m_uAreaScript = UINT_MAX;
		}
	}
	else if ( m_uAreaScript != UINT_MAX )
	{
		// 调用 OnTimer 来执行持续事件，但是需要判断该 Area 是否含有持续事件
		getScene()->GetLuaInterface()->ExeScript_DDD(m_uAreaScript, DEF_EVENT_ON_TIMER, (INT)sceneid, GetID(), 0);
	}

__LEAVE_FUNCTION
}

/****************************************************************************
 *	功能说明：
 *				根据前一个队员的方位，为当前队员选择一个合适目标位置
 *				只从 6 个备用方位选择，优先顺序如下所示：
 *								4—— * ——5
 *								    / | \
 *								   /  |	 \
 *								  2   1   3
 *				如果找不到合适的位置，则和前一个队员重合（第 6 个方位）
 *	参数说明：
 *				Pos
 *					用作输出
 *				pPrevPos
 *					前一个队员的位置
 *				fDir
 *					前一个队员的朝向
 *	输出说明：
 *				Pos
 *				当前队员应该移动到的位置
 *				成功返回 TRUE，失败返回 FALSE
*****************************************************************************/
BOOL Obj_Human::GetPosition(WORLD_POS& Pos, const WORLD_POS* pRefPos, FLOAT fDir)
{
	Pos.m_fX = pRefPos->m_fX - sin( fDir );
	Pos.m_fZ = pRefPos->m_fZ - cos( fDir );
	return TRUE;
}

INT		Obj_Human::GetStrikePoint(VOID)
{
	__ENTER_FUNCTION
		if(m_DB.GetStrikePoint()>GetMaxStrikePoint())
		{
			m_DB.SetStrikePoint(GetMaxStrikePoint());
		}
		return m_DB.GetStrikePoint();
	__LEAVE_FUNCTION
		return 0 ;
}
VOID	Obj_Human::SetStrikePoint(INT nStrikePoint)
{
	__ENTER_FUNCTION
		m_DB.SetStrikePoint(nStrikePoint);
	__LEAVE_FUNCTION
}

INT		Obj_Human::GetVigor(VOID)
{
	__ENTER_FUNCTION
		return m_DB.GetVigor();
	__LEAVE_FUNCTION
		return 0 ;
}
VOID	Obj_Human::SetVigor(INT nVigor)
{
	__ENTER_FUNCTION
		if(nVigor < 0)
		{
			nVigor = 0;
		}
		else
		{
			INT nMaxVigor = GetMaxVigor();
			if(nVigor > nMaxVigor)
			{
				nVigor = nMaxVigor;
			}
		}
		m_DB.SetVigor(nVigor);
	__LEAVE_FUNCTION
}

INT		Obj_Human::GetMaxVigor(VOID)
{
	__ENTER_FUNCTION
	_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetMaxVigorDirtyFlag())
	{
		INT nBaseAttr=m_DB.GetMaxVigor();
		INT nAbilityRefix=0;
		INT nItemPointRefix=0;
		INT nItemRateRefix=0;
		INT nValue=0;

		//////////////////////////////////////////////////////////////////////
		//生活技能修正
		//nAbilityRefix = MaxVigerRefix_ByAbility(*this);

		pIE = ItemEffect(IATTRIBUTE_MAX_VIGOR);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}

		nBaseAttr = nBaseAttr + nItemPointRefix + nAbilityRefix;

		//装备对属性的百分比影响
		pIE = ItemEffect(IATTRIBUTE_RATE_MAX_VIGOR);
		Assert( pIE ) ;
		if( pIE->IsActive() )
		{
			nItemRateRefix = Float2Int((nBaseAttr*(pIE->m_Attr.m_Value))/100.0f);
		}

		nValue = nBaseAttr + nItemRateRefix;

		nValue = MINLV0(nValue);
		
		SetIntAttr(CharIntAttrs_T::ATTR_MAX_VIGOR, nValue);
		ClearMaxVigorDirtyFlag();
		if(GetVigor()>nValue)
		{
			SetVigor(nValue);
		}
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_MAX_VIGOR);
	__LEAVE_FUNCTION
	return 0 ;
}
VOID	Obj_Human::SetMaxVigor(INT nMaxVigor)
{
	__ENTER_FUNCTION
	m_DB.SetMaxVigor(nMaxVigor);
	MarkMaxVigorDirtyFlag();
	__LEAVE_FUNCTION
}

INT		Obj_Human::GetVigorRegeneRate(VOID)
{
	//////////////////////////////////////////////////////////////////////////
	__ENTER_FUNCTION
	_ITEM_EFFECT* pIE=NULL;
	if(TRUE==GetVigorRegenerateDirtyFlag())
	{
		INT nBaseAttr=m_DB.GetVigorRegeneRate();
		INT nAbilityRefix=0;
		INT nValue=0;
		INT nItemPointRefix=0;

		//////////////////////////////////////////////////////////////////////////
		INT nMenPaiID = GetMenPai();
		MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
		if(NULL == pMenPaiLogic)
		{
			AssertEx(FALSE,"[Obj_Human::RestoreRageFromEXP]: Can't not find MenPai Logic!");
			return 0;
		}
		//////////////////////////////////////////////////////////////////////////

		INT nBaseVigorRestore = pMenPaiLogic->GetBaseVigorRestore();

		//////////////////////////////////////////////////////////////////////
		//生活技能修正
		//nAbilityRefix = VigorRegenerateRefix_ByAbility(*this);
				
		pIE = ItemEffect(IATTRIBUTE_MAX_VIGOR);
		Assert( pIE );
		if( pIE->IsActive() )
		{
			nItemPointRefix= pIE->m_Attr.m_Value;
		}
		
		nValue = nBaseAttr + Float2Int(nBaseVigorRestore/100) + nItemPointRefix + nAbilityRefix;
		
		nValue = MINLV0(nValue);
		
		SetIntAttr(CharIntAttrs_T::ATTR_VIGOR_REGENERATE, nValue);
		ClearVigorRegenerateDirtyFlag();
	}
	return GetIntAttr(CharIntAttrs_T::ATTR_VIGOR_REGENERATE);
	__LEAVE_FUNCTION
		return 0 ;
}
VOID	Obj_Human::SetVigorRegeneRate(INT nVigorRegeneRate)
{
	__ENTER_FUNCTION
		m_DB.SetVigorRegeneRate(nVigorRegeneRate);
		MarkVigorRegenerateDirtyFlag();
	__LEAVE_FUNCTION
}

INT		Obj_Human::GetEnergy(VOID)
{
	__ENTER_FUNCTION
		return m_DB.GetEnergy();
	__LEAVE_FUNCTION
		return 0 ;
}
VOID	Obj_Human::SetEnergy(INT nEnergy)
{
	__ENTER_FUNCTION

		if(nEnergy < 0)
		{
			nEnergy = 0;
		}
		else
		{
			INT nMaxEnergy = GetMaxEnergy();
			if(nEnergy > nMaxEnergy)
			{
				nEnergy = nMaxEnergy;
			}
		}
		m_DB.SetEnergy(nEnergy);
	__LEAVE_FUNCTION
}

INT		Obj_Human::GetMaxEnergy(VOID)
{
	__ENTER_FUNCTION
		return m_DB.GetMaxEnergy();
	__LEAVE_FUNCTION
		return 0 ;
}
VOID	Obj_Human::SetMaxEnergy(INT nMaxEnergy)
{
	__ENTER_FUNCTION
		m_DB.SetMaxEnergy(nMaxEnergy);
	__LEAVE_FUNCTION
}

INT		Obj_Human::GetEnergyRegeneRate(VOID)
{
	__ENTER_FUNCTION
		return m_DB.GetEnergyRegeneRate();
	__LEAVE_FUNCTION
		return 0 ;
}
VOID	Obj_Human::SetEnergyRegeneRate(INT nEnergyRegeneRate)
{
	__ENTER_FUNCTION
		m_DB.SetEnergyRegeneRate(nEnergyRegeneRate);
	__LEAVE_FUNCTION
}

INT 	Obj_Human::GetRage( VOID )
{
	__ENTER_FUNCTION
		if(m_DB.GetRage()>GetMaxRage())
		{
			m_DB.SetRage(GetMaxRage());
		}
		return m_DB.GetRage();
	__LEAVE_FUNCTION
		return 0 ;
}
VOID	Obj_Human::SetRage( INT nRage )
{
	__ENTER_FUNCTION
		if(0>nRage)
		{
			nRage=0;
		}
		if(nRage>GetMaxRage())
		{
			nRage = GetMaxRage();
		}
		m_DB.SetRage(nRage);
	__LEAVE_FUNCTION
}

//从杀怪的经验当中产生怒气
VOID Obj_Human::RestoreRageFromEXP(INT nExp)
{
	//门派
	INT nMenPaiID = GetMenPai();
	MenPai_T const* pMenPaiLogic = g_MenPaiLogicList.GetLogicById(nMenPaiID);
	if(NULL == pMenPaiLogic)
	{
		AssertEx(FALSE,"[Obj_Human::RestoreRageFromEXP]: Can't not find MenPai Logic!");
		return;
	}

	INT nRestoreRageFromEXP = pMenPaiLogic->GetRestoreRageFromEXP();

	//计算基础基础属性
	INT nBase = nExp * nRestoreRageFromEXP;

	nBase=Float2Int((nBase)/100); //表里面的数值都是乘了100的，计算后应该修正回去	

	0<=nBase?NULL:nBase=0;	

	if (nBase > 0)
	{
		RageIncrement(nBase);
	}
}

INT		Obj_Human::GetHP( VOID )
{
	if(m_DB.GetHP() > GetMaxHP())
	{
		m_DB.SetHP(GetMaxHP());
	}
	return m_DB.GetHP(); 
}
INT		Obj_Human::GetMP( VOID )
{
	if(m_DB.GetMP() > GetMaxMP())
	{
		m_DB.SetMP(GetMaxMP());
	}
	return m_DB.GetMP(); 
}

INT		Obj_Human::GetPneuma( VOID )
{
	if(m_DB.GetPneuma() > GetMaxPneuma())
	{
		m_DB.SetPneuma(GetMaxPneuma());
	}
	return m_DB.GetPneuma(); 
}

VOID Obj_Human::Teleport( const WORLD_POS* pos )
{
__ENTER_FUNCTION

	Obj_Character::Teleport( pos );
	for( INT i=0; i<MAX_MONSTER_PET_TAKE; ++i )
	{
		if ( m_pPet[i] != NULL )
		{
			m_pPet[i]->Teleport( pos );
		}
	}

	m_DB.SetDBPosition(*getWorldPos());

__LEAVE_FUNCTION
}

ORESULT Obj_Human::TestCallUpPet(const PET_GUID_t guidPet)
{
__ENTER_FUNCTION

	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT Obj_Human::CallUpPet(VOID)
{
	__ENTER_FUNCTION

	INT iIndex = GetEmptyPetIndex();
	//如果达到召唤兽的最大数量
	if( iIndex == INVALID_ID )
	{
		//暂时释放最后一只,应该有替换规则
		iIndex = MAX_MONSTER_PET_TAKE-1;
		ReleasePet( iIndex );
	}

	ORESULT oResult =CreatePet( iIndex );


    return OR_OK;

	__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT Obj_Human::CallUpHorse(VOID)
{
	__ENTER_FUNCTION

	INT iIndex = GetEmptyHorseIndex();
	//如果达到召唤兽的最大数量
	if( iIndex == INVALID_ID )
	{
		//暂时释放最后一只,应该有替换规则
		iIndex = MAX_MONSTER_HORSE_TAKE-1;
		ReleaseHorse( iIndex );
	}

	ORESULT oResult =CreateHorse( iIndex );


	return OR_OK;

	__LEAVE_FUNCTION

		return OR_ERROR;
}


ORESULT	Obj_Human::ReCallHorse(VOID)
{
	PET_GUID_t oPetGUID;
	SetMountID( INVALID_ID );
	SetGUIDOfCallUpHorse( oPetGUID );
	//TODO:清除加在人身上的各种影响
	CleanupHorseBuff();

	ReleaseHorse();
	return OR_OK;
}

ORESULT Obj_Human::ReCallPet(VOID)
{
	__ENTER_FUNCTION

	// 当收回宠物时将主人身上所有宠物种的BUFF统统清掉
	for( INT i=0; i<MAX_MONSTER_PET_TAKE; ++i)
	{
		if ( NULL != m_pPet[i] )
		{
			Impact_CleanAllImpactWhenPetDead(m_pPet[i]->GetID());
			m_pPet[i]->Impact_CleanAllImpactWhenPetDead(m_pPet[i]->GetID());
		}
	}

	ReleasePet();

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT Obj_Human::ReCallPet( INT iIndex )
{
	__ENTER_FUNCTION

	// 当收回宠物时将主人身上所有宠物种的BUFF统统清掉

	if ( NULL != m_pPet[iIndex] )
	{
		Impact_CleanAllImpactWhenPetDead(m_pPet[iIndex]->GetID());
		m_pPet[iIndex]->Impact_CleanAllImpactWhenPetDead(m_pPet[iIndex]->GetID());
	}
	ReleasePet( iIndex );

	return OR_OK;

	__LEAVE_FUNCTION

	return OR_ERROR;
}
BOOL Obj_Human::AddHorse( const _PET_DB *pPetDB )
{
	__ENTER_FUNCTION

	if(pPetDB == NULL)
		return FALSE;

	for ( INT i = 0; i < HUMAN_PET_MAX_COUNT; ++i )
	{
		Item* pPetFind = GetPetContain()->GetItem(i);
		if (pPetFind != NULL && pPetFind->IsLock() == FALSE)
		{// 确保要操作的宠物现在处于非锁定状态
			PET_GUID_t guidFind = pPetFind->GetPetGUID();
			if ( guidFind.IsNull() )
			{
				g_ItemOperator.SetItemValue(GetPetContain(), i, pPetDB);
				RefeshHorseDetailAttrib(pPetDB->m_GUID);

				OnPetChanged(pPetDB->m_nDataID) ;

				return TRUE;
			}
		}
	}
	return FALSE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL Obj_Human::RemovePet(PET_LOG_PARAM* pPetLogParam,PET_GUID_t guid)
{
	__ENTER_FUNCTION

	return FALSE;

	__LEAVE_FUNCTION

	return FALSE;
}

ORESULT Obj_Human::CreatePetOnRegisterZone(VOID)
{
	__ENTER_FUNCTION

	const _MONSTER_PET_DB* pMonsterPetDB = GetDB()->GetMonsterPetDB();
	_MONSTER_PET_ATTR oMonsterPetAttr;
	memset( &oMonsterPetAttr, -1, sizeof(oMonsterPetAttr) );
	if( NULL == pMonsterPetDB )
	{
		return OR_ERROR;
	}

	PetManager* pPetManager = getScene()->GetPetManager();
	if( NULL == pPetManager )
	{
		return OR_ERROR;
	}

	for( INT i=0; i<MAX_MONSTER_PET_TAKE; ++i )
	{
		INT iDataID = pMonsterPetDB->m_aMonsterPet[i].m_iDataIndex;
		if( INVALID_ID == iDataID )
		{
			continue;
		}

		SetMonsterPetDataID( iDataID );
		memcpy( oMonsterPetAttr.aSkillID, pMonsterPetDB->m_aMonsterPet[i].m_aSkillList, sizeof(oMonsterPetAttr.aSkillID) );
		oMonsterPetAttr.iLifeTime = pMonsterPetDB->m_aMonsterPet[i].iLifeTime;
		pPetManager->SetInitData( oMonsterPetAttr );
		CreatePet( i );
		m_pPet[i]->SetHP( pMonsterPetDB->m_aMonsterPet[i].m_iHP );
	}

	return OR_OK;

	__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT Obj_Human::CreatePet( INT iIndex )
{
	__ENTER_FUNCTION

	Scene* pScene = getScene();
	if (!pScene)
	{
		return OR_ERROR;
	}
	Obj_Pet *pPet = (Obj_Pet*)(getScene()->NewObject( Obj::OBJ_TYPE_PET ));
	if ( !pPet )
	{
		return OR_ERROR;
	}
	AI_Pet* pAI = static_cast<AI_Pet*>(pPet->GetAIObj());
	BOOL bResult;
	_OBJ_PET_INIT initPet;
	const WORLD_POS* pPos = getWorldPos();
	pAI->GetStandPos( this, iIndex, pPos, 1.0f, initPet.m_Pos );
	// 对该位置进行校验
	pPos = &initPet.m_Pos;
	pScene->GetMap()->VerifyPos(&initPet.m_Pos);
	if (FALSE == pScene->GetMap()->IsCanGo(*pPos)) 
	{
		initPet.m_Pos = *getWorldPos();
	}
	initPet.m_Dir		= 0.f;

	initPet.m_eLogic	= CHARACTER_LOGIC_IDLE;
	initPet.m_CreatorID	= GetID();
	initPet.m_nIndex	= iIndex;
	bResult = pPet->Init( &initPet );
	if ( !bResult )
	{
		pPet->CleanUp();
		getScene()->DeleteObject( pPet );
		pPet = NULL;
		return OR_ERROR;
	}
	pPet->SetActiveFlag( TRUE );
	m_pPet[iIndex] = pPet;

	pPet->InitSkill();

	return OR_OK;

	__LEAVE_FUNCTION

		return OR_ERROR;
}

BOOL Obj_Human::ReleasePet(VOID)
{
	__ENTER_FUNCTION
	PetManager* pPetMgr = getScene()->GetPetManager();
	if ( NULL == pPetMgr )
	{
		return FALSE;
	}

	for( INT i=0; i<MAX_MONSTER_PET_TAKE; ++i )
	{
		if ( m_pPet[i] != NULL )
		{
			pPetMgr->RemovePet(m_pPet[i]->GetID());

			m_pPet[i] = NULL;
		}
	}

	return TRUE;

__LEAVE_FUNCTION
	return TRUE;
}

BOOL Obj_Human::ReleasePet( INT iIndex )
{
	__ENTER_FUNCTION

	_Monster_Pet_DB_Struct oMonsterPet;
	memset( &oMonsterPet, -1, sizeof(oMonsterPet) );
	GetDB()->SetMonsterPetAttr( iIndex, &oMonsterPet );

	if ( m_pPet[iIndex] != NULL )
	{
		PetManager* pPetMgr = getScene()->GetPetManager();
		if (pPetMgr)
			pPetMgr->RemovePet(m_pPet[iIndex]->GetID());

		m_pPet[iIndex] = NULL;
		
		return TRUE;
	}
	return FALSE;
	__LEAVE_FUNCTION
	return TRUE;
}

ORESULT Obj_Human::CreateHorse( INT iIndex )
{
	__ENTER_FUNCTION

	//Scene* pScene = getScene();
	//if (!pScene)
	//{
	//	return OR_ERROR;
	//}
	//BOOL bResult;
	//_OBJ_HORSE_INIT initHorse;
	//Obj_Horse *pHorse = (Obj_Horse*)(getScene()->NewObject( Obj::OBJ_TYPE_HORSE));
	//if ( !pHorse )
	//{
	//	return OR_ERROR;
	//}
	//initHorse.m_Pos = *getWorldPos();
	//initHorse.m_Dir		= 0.f;
	//initHorse.m_eLogic	= CHARACTER_LOGIC_IDLE;
	//initHorse.m_CreatorID	= GetID();
	//initHorse.m_nIndex	= iIndex;
	//bResult = pHorse->Init( &initHorse );
	//if ( !bResult )
	//{
	//	pHorse->CleanUp();
	//	getScene()->DeleteObject( pHorse );
	//	pHorse = NULL;
	//	return OR_ERROR;
	//}
	//pHorse->SetActiveFlag( TRUE );
	//m_pHorse[iIndex] = pHorse;

	//pHorse->InitSkill();

	return OR_OK;

	__LEAVE_FUNCTION

		return OR_ERROR;
}

BOOL Obj_Human::ReleaseHorse(VOID)
{
	__ENTER_FUNCTION
	//HorseManager* pHorseMgr = getScene()->GetHorseManager();
	//if ( NULL == pHorseMgr )
	//{
	//	return FALSE;
	//}

	//for( INT i=0; i<MAX_MONSTER_PET_TAKE; ++i )
	//{
	//	if ( m_pHorse[i] != NULL )
	//	{
	//		pHorseMgr->RemoveHorse(m_pHorse[i]->GetID());

	//		m_pHorse[i] = NULL;
	//	}
	//}

	//return TRUE;

	__LEAVE_FUNCTION
		return TRUE;
}

BOOL Obj_Human::ReleaseHorse( INT iIndex )
{
	__ENTER_FUNCTION

	//_Monster_Pet_DB_Struct oMonsterPet;
	//memset( &oMonsterPet, -1, sizeof(oMonsterPet) );
	//GetDB()->SetMonsterPetAttr( iIndex, &oMonsterPet );

	//if ( m_pHorse[iIndex] != NULL )
	//{
	//	HorseManager* pHorseMgr = getScene()->GetHorseManager();
	//	if (m_pHorse)
	//		pHorseMgr->RemoveHorse(m_pHorse[iIndex]->GetID());

	//	m_pHorse[iIndex] = NULL;

	//	return TRUE;
	//}
	//return FALSE;
	__LEAVE_FUNCTION
		return TRUE;
}


ORESULT Obj_Human::FreePetToNature(PET_LOG_PARAM* pPetLogParam,PET_GUID_t guidPet)
{
__ENTER_FUNCTION
	
	//Assert(pPetLogParam);
	//if(guidPet.IsNull())
	//{
	//	return OR_ERROR;
	//}

	//if(guidPet == GetCurrentPetGUID())
	//{
	//	ReCallPet();
	//}

	//RemovePet(pPetLogParam,guidPet);

	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;
}

INT Obj_Human::GetEmptyPetIndex()
{
	__ENTER_FUNCTION

	for( INT i=0; i<MAX_MONSTER_PET_TAKE; ++i)
	{
		if( NULL == m_pPet[i] )
		{
			return i;
		}
	}
	return -1;
	__LEAVE_FUNCTION
	return -1;
}

INT Obj_Human::GetEmptyHorseIndex()
{
	__ENTER_FUNCTION

		//for( INT i=0; i<MAX_MONSTER_HORSE_TAKE; ++i)
		//{
		//	if( NULL == m_pHorse[i] )
		//	{
		//		return i;
		//	}
		//}
		return -1;
		__LEAVE_FUNCTION
			return -1;
}

ORESULT Obj_Human::AddPetByDataID(PET_LOG_PARAM* pPetLogParam,const UINT uDataID)
{
__ENTER_FUNCTION

	_PET_DB dbPet;
	Assert(pPetLogParam);
	PET_ATTR* pPetAttr = g_PetAttrTbl.GetAttr(uDataID);
	if (!pPetAttr) {
		return OR_ERROR;
	}

	Scene* pScene = getScene();
	if (!pScene)
	{
		return OR_ERROR;
	}	
	dbPet.m_nDataID = uDataID;
	dbPet.m_GUID.Init(GetGUID(), g_pTimeManager->CurrentTime());
	tsnprintf(dbPet.m_szName, sizeof(dbPet.m_szName), pPetAttr->m_Name);
	dbPet.m_nLevel = pPetAttr->m_TakeLevel;
	dbPet.m_nTakeLevel = pPetAttr->m_TakeLevel;

	pScene->GetPetManager()->MakeCapturePetAttrib(&dbPet);
	
	AddHorse(&dbPet);
	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;

}

VOID Obj_Human::BabyToAttack(VOID)
{
__ENTER_FUNCTION
	for( INT i=0; i<MAX_MONSTER_PET_TAKE; ++i )
	{
		if ( m_pPet[i] == NULL )
		{
			continue;		
		}
		// 让宠物进入攻击状态
		if (m_pPet[i]->IsAlive())
		{
			ObjID_t TargetID = GetCurTarget();
			m_pPet[i]->SetTargetID(TargetID);
			AI_Pet* pAIPet = ((AI_Pet*)m_pPet[i]->GetAIObj() );
			pAIPet->BabyToAttack();
		}
	}

__LEAVE_FUNCTION
}

VOID Obj_Human::BabyToProtectMe( ObjID_t iObjID )
{
	__ENTER_FUNCTION
	for( INT i=0; i<MAX_MONSTER_PET_TAKE; ++i )
	{
		if ( m_pPet[i] == NULL )
		{
			continue;		
		}

		// 让宠物进入攻击状态
		if (m_pPet[i]->IsAlive())
		{
			m_pPet[i]->SetTargetID(iObjID);
			AI_Pet* pAIPet = ((AI_Pet*)m_pPet[i]->GetAIObj() );
			pAIPet->BabyToAttack();
		}
	}
	__LEAVE_FUNCTION
}

BOOL Obj_Human::CaptureHorse( PET_LOG_PARAM* pPetLogParam, ObjID_t PetID )
{
__ENTER_FUNCTION
	
	Assert(pPetLogParam);
	// 检测有效性
	Obj* pObj = getScene()->GetObjManager()->GetObj( PetID );
	if ( !pObj ||
		 pObj->GetObjType() != Obj::OBJ_TYPE_HORSE ||
		 !((Obj_Character*)pObj)->IsAlive() )
	{
		return FALSE;
	}
	Obj_Horse* pHorse = (Obj_Horse*)pObj;
	if (!pHorse)
	{
		Assert(NULL && "Obj_Human::CapturePet...pPet=NULL...");
		return FALSE;
	}

	if (NULL != pHorse->GetOwner())
	{// 说明该宠物已经有主人了则返回
		SendOperateResultMsg(OR_INVALID_TARGET);
		return FALSE;
	}
	// 如果小于宠物的携带等级则返回
	INT nTakeLevel = pHorse->GetPetDB()->m_nTakeLevel;
	if (GetLevel() < nTakeLevel)
	{
		SendOperateResultMsg(OR_NEED_HIGH_LEVEL);
		return FALSE;
	}

	// 是否属于自己的规则判断
	INT nCapturerCount = pHorse->GetCaptureCount();	
	if (!nCapturerCount)
		return FALSE;

	INT nRet = rand() % nCapturerCount;
	if (0 != nRet)
	{// 如果不在范围之内则捕获失败
        return FALSE;
	}

	
	_PET_DB* pPetDB = const_cast<_PET_DB*>(pHorse->GetPetDB());
	BOOL bResult = getScene()->GetHorseManager()->CreateGUIDOfHorse(pPetDB, GetID(), PetID);
	pPetDB->m_iMatingLevel = 0;
	if(!bResult)
	{
		return FALSE;
	}

	bResult = AddHorse(pPetDB);
	if(!bResult)
	{
		return FALSE;
	}
	
	pPetLogParam->CharGUID		=	GetGUID();
	pPetLogParam->PetGUID		=	pPetDB->m_GUID;
	pPetLogParam->DataID		=	pPetDB->m_nDataID;
	pPetLogParam->SceneID		=	getScene()->SceneID();
	pPetLogParam->XPos			=	getWorldPos()->m_fX;
	pPetLogParam->ZPos			=	getWorldPos()->m_fZ;


	return TRUE;

__LEAVE_FUNCTION
	return TRUE;

}

BOOL Obj_Human::CanHorseReturnToChild( PET_GUID_t guid )
{
__ENTER_FUNCTION
	Scene* pScene = getScene();	
	if (!pScene)
	{
		Assert(NULL && "Obj_Human::CanReturnToChild...pScene=NULL...");
		return FALSE;
	}

	_PET_DB* pPetDB = NULL;
	INT nIndex = GetHorseIndexByGUID(guid);
	pPetDB = GetHorseDB(nIndex);
	if (!pPetDB)
	{
		return FALSE;
	}

	if ( !pPetDB->m_SpouseGUID.IsNull() )
	{// 已经有配偶的宠物不能进行还童
		SendOperateResultMsg(OR_HASSPOUSE_CANNOT_RETURNTOCHILD);
		return FALSE;
	}

	_HORSE_ATTR* pHorseAttr = g_HorseAttrTbl.GetAttr( pPetDB->m_nDataID );
	if( pHorseAttr == NULL )
		return FALSE;
	
	if ( pHorseAttr->m_bVariation )
	{// 变异宠不能进行还童
		SendOperateResultMsg(OR_VARIANCEPET_CANNOT_RETURNTOCHILD);
		return FALSE;
	}

	if ( GetGUIDOfCallUpHorse() == guid)
	{// 如果要还童的宠物正在出战状态则提示先收回宠物
		SendOperateResultMsg(OR_COMBAT_CANNOT_RETURNTOCHILD);
		return FALSE;
	}

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

BOOL Obj_Human::HorseReturnToChild( PET_GUID_t guid )
{
	__ENTER_FUNCTION
	
	Scene* pScene = getScene();	
	if (!pScene)
	{
		Assert(NULL && "Obj_Human::ReturnToChild...pScene=NULL...");
		return FALSE;
	}

	_PET_DB* pPetDB = NULL;
	INT nIndex = GetHorseIndexByGUID(guid);
	pPetDB = GetHorseDB(nIndex);
	if (!pPetDB)
	{
		return FALSE;
	}
	
	//在马基础属性还童ID中随机选取还童后的基础表对应ID
	_HORSE_ATTR *pPetAttr = NULL;
	if ( pPetDB->m_nDataID != INVALID_ID )
	{
		pPetAttr = g_HorseAttrTbl.GetAttr( pPetDB->m_nDataID );
	}
	if( pPetAttr == NULL )
		return FALSE;

	if( pPetAttr->m_iReturnToChildIDCount <= 0 )
		return FALSE;
	INT iTmpIndex = getScene()->GetRand100() % pPetAttr->m_iReturnToChildIDCount;
	if( iTmpIndex >= pPetAttr->m_iReturnToChildIDCount )
	{
		iTmpIndex = pPetAttr->m_iReturnToChildIDCount - 1;
	}
	pPetDB->m_nDataID = pPetAttr->m_aReturnToChildID[iTmpIndex];
	
	pScene->GetHorseManager()->InitHorseDB( pPetDB, pPetDB->m_nDataID );
	pPetDB->m_nLevel = 1;
	RefeshHorseDetailAttrib(guid);

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

INT Obj_Human::GetHorseCount( VOID ) const
{
__ENTER_FUNCTION

	INT i, nCount;
	nCount = 0;
	for ( i = 0; i < HUMAN_PET_MAX_COUNT; i++ )
	{
		if ( !m_DB.m_dbPetList->m_aPetDB[i].m_GUID.IsNull() )
		{
			nCount++;
		}
	}
	return nCount;

__LEAVE_FUNCTION

	return 0;
}

_PET_DB * Obj_Human::GetHorseDB( INT nIndex ) const
{
__ENTER_FUNCTION

	for (INT i = 0; i < HUMAN_PET_MAX_COUNT; i++ )
	{
		if ( !m_DB.m_dbPetList->m_aPetDB[i].m_GUID.IsNull() )
		{
			if ( i == nIndex )
			{
				return &(m_DB.m_dbPetList->m_aPetDB[i]);
			}
		}
	}
	return NULL;

__LEAVE_FUNCTION

	return NULL;
}

_PET_DB * Obj_Human::GetEmptyHorseDB()
{
	__ENTER_FUNCTION

	for (INT i = 0; i < HUMAN_PET_MAX_COUNT; i++ )
	{
		if ( m_DB.m_dbPetList->m_aPetDB[i].m_GUID.IsNull() )
		{
			return &(m_DB.m_dbPetList->m_aPetDB[i]);
		}
	}
	return NULL;

	__LEAVE_FUNCTION

	return NULL;
}

const _PET_DB * Obj_Human::GetConstHorseDB( INT nIndex ) const
{
	return GetHorseDB( nIndex );
}

BYTE Obj_Human::GetHorseHappiness( PET_GUID_t guid )
{
__ENTER_FUNCTION

	Item *pPetItem = GetHorseItem(guid);
	if(pPetItem != NULL)
	{
		return pPetItem->GetHappiness();
	}
	return 0;

__LEAVE_FUNCTION

	return 0;
}

VOID Obj_Human::UpdateHouseExp( INT iExp )
{
	__ENTER_FUNCTION
	m_ActiveHorse.OnExpChange( iExp );
	__LEAVE_FUNCTION
}

VOID Obj_Human::SetPetHappiness( PET_GUID_t guid, BYTE byHappiness )
{
__ENTER_FUNCTION

	INT nIndex = GetHorseIndexByGUID(guid);
	g_ItemOperator.SetHappiness(GetPetContain(), nIndex, byHappiness);

__LEAVE_FUNCTION
}

INT Obj_Human::GetPetHP( PET_GUID_t guid )
{
__ENTER_FUNCTION

	//Item *pPetItem = GetPetItem(guid);
	//if(pPetItem != NULL)
	//{
	//	return pPetItem->GetHP();
	//}
	return 0;

__LEAVE_FUNCTION

	return 0;
}

VOID Obj_Human::SetPetHP( PET_GUID_t guid, INT nHP )
{
__ENTER_FUNCTION

	INT nIndex = GetHorseIndexByGUID(guid);
	g_ItemOperator.SetHP(GetPetContain(), nIndex, nHP);

__LEAVE_FUNCTION
}

INT Obj_Human::GetPetMaxHP( PET_GUID_t guid )
{
__ENTER_FUNCTION

	//if(guid == m_GUIDOfCallUpPet && m_pPet != NULL)
	//{
	//	return m_pPet->GetMaxHP();
	//}
	//else
	//{
	//	Item *pPetItem = GetPetItem(guid);
	//	if(pPetItem != NULL)
	//	{
	//		return pPetItem->GetBaseMaxHP();
	//	}
	//}

	return 1;

__LEAVE_FUNCTION

	return 1;
}

INT Obj_Human::GetPetLife( PET_GUID_t guid )
{
__ENTER_FUNCTION

	//Item *pPetItem = GetPetItem(guid);
	//if(pPetItem != NULL)
	//{
	//	return pPetItem->GetLife();
	//}
	return 0;

__LEAVE_FUNCTION

	return 0;
}

VOID Obj_Human::SetPetLife( PET_GUID_t guid, INT nLife )
{
__ENTER_FUNCTION

	INT nIndex = GetHorseIndexByGUID(guid);
	g_ItemOperator.SetLife(GetPetContain(), nIndex, nLife);

__LEAVE_FUNCTION
}

Item * Obj_Human::GetHorseItem( PET_GUID_t guid )
{
__ENTER_FUNCTION

	Item* pPetItem = NULL;
	for ( INT i = 0; i < HUMAN_PET_MAX_COUNT; ++i )
	{
		Item* pPetFind = GetPetContain()->GetItem(i);
		PET_GUID_t guidFind = pPetFind->GetPetGUID();
		if ( (guidFind == guid) )
		{
			pPetItem = pPetFind;
			break;
		}
	}
	return pPetItem;

__LEAVE_FUNCTION

	return NULL;
}

INT Obj_Human::GetHorseIndexByGUID( PET_GUID_t guid )
{
__ENTER_FUNCTION

	Item* pPetItem = NULL;
	for ( INT i = 0; i < HUMAN_PET_MAX_COUNT; ++i )
	{
		Item* pPetFind = GetPetContain()->GetItem(i);
		PET_GUID_t guidFind = pPetFind->GetPetGUID();
		if ( (guidFind == guid) )
		{
			return i;
		}
	}
	return -1;

__LEAVE_FUNCTION

	return -1;
}

// 临时函数
PET_GUID_t Obj_Human::GetPetGUIDByIndex(INT nIndex)
{
__ENTER_FUNCTION

	if(nIndex >= 0 && nIndex < HUMAN_PET_MAX_COUNT)
	{
		Item* pPetFind = GetPetContain()->GetItem(nIndex);
		return  pPetFind->GetPetGUID();
	}

	PET_GUID_t guidPet;
	guidPet.Reset();
	return guidPet;

__LEAVE_FUNCTION

	PET_GUID_t guidPet;
	guidPet.Reset();
	return guidPet;
}

// 临时函数
PET_GUID_t Obj_Human::GetPetGUIDByValidIndex(INT nIndex)
{
__ENTER_FUNCTION

	if(nIndex >= 0 && nIndex < HUMAN_PET_MAX_COUNT)
	{
		INT i, nCount;
		nCount = 0;
		for(i = 0; i < HUMAN_PET_MAX_COUNT; i++)
		{
			Item* pPetFind = GetPetContain()->GetItem(nIndex);
			if(pPetFind != NULL)
			{
				if(nCount == nIndex)
				{
					return pPetFind->GetPetGUID();
				}
				nCount++;
			}
		}
	}

	PET_GUID_t guidPet;
	guidPet.Reset();
	return guidPet;

__LEAVE_FUNCTION

	PET_GUID_t guidPet;
	guidPet.Reset();
	return guidPet;
}

VOID Obj_Human::SetMoney(UINT money)
{
	__ENTER_FUNCTION
	
	m_DB.SetMoney(money);	

	__LEAVE_FUNCTION
}

INT Obj_Human::GetGoodBadValue()
{
__ENTER_FUNCTION

	return m_DB.GetGoodBadValue();

__LEAVE_FUNCTION

	return 0;
}

INT Obj_Human::SetGoodBadValue(INT nValue)
{
__ENTER_FUNCTION

	if( nValue < g_Config.m_ConfigInfo.m_nMinGoodBadValue )
	{
		nValue = g_Config.m_ConfigInfo.m_nMinGoodBadValue;
	}
	else if( nValue > g_Config.m_ConfigInfo.m_nMaxGoodBadValue )
	{
		nValue = g_Config.m_ConfigInfo.m_nMaxGoodBadValue;
	}

	return m_DB.SetGoodBadValue( nValue );

__LEAVE_FUNCTION

	return 0;
}

INT Obj_Human::IncGoodBadValue(INT nValue)
{
__ENTER_FUNCTION

	if( nValue < 0 )
	{
		Assert( FALSE && "nValue must be positive." );
		return 0;
	}

	INT nOldValue;

	nOldValue = GetGoodBadValue();

	return SetGoodBadValue( nOldValue + nValue ) - nOldValue;

__LEAVE_FUNCTION

	return 0;
}

INT Obj_Human::DecGoodBadValue(INT nValue)
{
__ENTER_FUNCTION

	if( nValue < 0 )
	{
		Assert( FALSE && "nValue must be positive." );
		return 0;
	}

	INT nOldValue;

	nOldValue = GetGoodBadValue();

	return nOldValue - SetGoodBadValue( nOldValue - nValue );

__LEAVE_FUNCTION

	return 0;
}

_PLAYERSHOP_GUID	Obj_Human::GetShopGuid(INT nIndex)
{
	__ENTER_FUNCTION
	if(nIndex >= 0 && nIndex < MAX_SHOP_NUM_PER_PLAYER)
	{
		return	m_DB.GetShopGuid(nIndex);
	}

	_PLAYERSHOP_GUID guidShop;
	return guidShop;
	__LEAVE_FUNCTION
	_PLAYERSHOP_GUID guidShop;
	return guidShop;
}

VOID	Obj_Human::SetShopGuid(INT nIndex, _PLAYERSHOP_GUID pGuid)
{
	__ENTER_FUNCTION
		if(nIndex >= 0 && nIndex < MAX_SHOP_NUM_PER_PLAYER)
		{
			m_DB.SetShopGuid(nIndex, pGuid);
		}
	__LEAVE_FUNCTION
}

_PLAYERSHOP_GUID	Obj_Human::GetFavoriteShopGuid(INT nIndex)
{
	__ENTER_FUNCTION
		if(nIndex >= 0 && nIndex < MAX_FAVORITE_SHOPS)
		{
			return	m_DB.GetFavoriteShopGuid(nIndex);
		}

		_PLAYERSHOP_GUID guidShop;
		return guidShop;
	__LEAVE_FUNCTION
		_PLAYERSHOP_GUID guidShop;
		return guidShop;

}

VOID	Obj_Human::SetFavoriteShopGuid(INT nIndex, _PLAYERSHOP_GUID pGuid)
{
	__ENTER_FUNCTION
		if(nIndex >= 0 && nIndex < MAX_FAVORITE_SHOPS)
		{
			m_DB.SetFavoriteShopGuid(nIndex, pGuid);
		}
	__LEAVE_FUNCTION
}

BOOL Obj_Human::IsInFavorite(_PLAYERSHOP_GUID pGuid)
{
	__ENTER_FUNCTION
		for(INT i = 0; i<MAX_FAVORITE_SHOPS; i++)
		{
			if(GetFavoriteShopGuid(i) == pGuid)
			{
				return TRUE;
			}
		}
		return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL Obj_Human::AddFavorite(_PLAYERSHOP_GUID pGuid)
{
	__ENTER_FUNCTION
	if(IsInFavorite(pGuid))
	{
		return TRUE;
	}
	for(INT i =0; i<MAX_FAVORITE_SHOPS; i++)
	{
		if(GetFavoriteShopGuid(i).isNull() )
		{
			SetFavoriteShopGuid(i, pGuid);
			return TRUE;
		}
	}
	return FALSE;
	__LEAVE_FUNCTION
	return FALSE;

}

BOOL Obj_Human::DelFavorite(_PLAYERSHOP_GUID pGuid)
{
	__ENTER_FUNCTION
	if(IsInFavorite(pGuid) == FALSE)
	{
		return TRUE;
	}

	for(INT i =0; i<MAX_FAVORITE_SHOPS; i++)
	{
		if( GetFavoriteShopGuid(i) == pGuid )
		{
			_PLAYERSHOP_GUID guidShop;
			SetFavoriteShopGuid(i, guidShop);
			return TRUE;
		}
	}
	return FALSE;
	__LEAVE_FUNCTION
	return FALSE;

}

VOID Obj_Human::UpdateFavorite(_PLAYERSHOP_GUID* pGuidList)
{
	//传入的是确实存在的商店列表
	//根据这个列表刷新自己的收藏家列表,真e心
	__ENTER_FUNCTION
	for(INT i=0; i<MAX_FAVORITE_SHOPS; i++)
	{
		BOOL	bDeleteFlag = TRUE;
		_PLAYERSHOP_GUID OldGuid= GetFavoriteShopGuid(i);
		if(OldGuid.isNull())
			continue;
		for(INT j = 0; j<MAX_FAVORITE_SHOPS; j++)
		{
			if(OldGuid == pGuidList[j])
			{
				bDeleteFlag = FALSE;
				break;				
			}
		}

		if(bDeleteFlag == TRUE)
		{
			DelFavorite(OldGuid);
		}
	}
	__LEAVE_FUNCTION
}


UINT Obj_Human::GetDataID( VOID )const
{
	return  m_DB.GetHumanDB()->m_Sex;
}

///////////////////////////////////////////////////////////
// 复活点相关
///////////////////////////////////////////////////////////
VOID Obj_Human::SetReliveInfo( BOOL bSkillRelive, const _RELIVE_INFO *pReliveInfo )
{
	__ENTER_FUNCTION
	if ( pReliveInfo == NULL )
	{
		Assert( pReliveInfo != NULL && "Obj_Human::SetReliveInfo" );
		return;
	}

	Obj_Character::SetReliveInfo(bSkillRelive, pReliveInfo);

	if ( !bSkillRelive )
	{
		m_DefaultReliveInfo = *pReliveInfo;
	}
	OnReliveInfoChanged();
	__LEAVE_FUNCTION
}

const Obj_Character::_RELIVE_INFO *Obj_Human::GetReliveInfo( BOOL bSkillRelive )const
{
	if ( bSkillRelive && m_bCanSkillRelive )
	{
		return &m_SkillReliveInfo;
	}
	else
	{
		return &m_DefaultReliveInfo;
	}
}

VOID Obj_Human::OnReliveInfoChanged( VOID )
{
	__ENTER_FUNCTION
	if ( m_pAI_Character != NULL )
	{
		((AI_Human*)m_pAI_Character)->OnReliveInfoChanged();
	}
	__LEAVE_FUNCTION
}

///////////////////////////////////////////////////////////
// 复活点相关
///////////////////////////////////////////////////////////
VOID Obj_Human::SetCallOfInfo(const _CALLOF_INFO *pCallOfInfo)
{
	__ENTER_FUNCTION
	if(!IsDie())
	{
		if ( pCallOfInfo == NULL )
		{
			Assert( pCallOfInfo != NULL && "Obj_Human::SetCallOfInfo" );
			return;
		}
		m_CallOfInfo = *pCallOfInfo;
		OnCallOfInfoChanged();
	}
	__LEAVE_FUNCTION
}

VOID Obj_Human::CallOfHuman(GUID_t guidTarget, UINT uDuration)
{
__ENTER_FUNCTION

	if(guidTarget != INVALID_GUID)
	{
		_CALLOF_INFO infoCallOf;
		infoCallOf.m_guidCaller		= GetGUID();
		infoCallOf.m_Pos			= *getWorldPos();
		infoCallOf.m_SceneID		= getScene()->SceneID();
		infoCallOf.m_uDuration		= uDuration;

		GWCallOfHuman* pMsg = (GWCallOfHuman*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_CALLOFHUMAN);
		pMsg->SetCallOfInfo(&infoCallOf);
		pMsg->SetCallOfTargetGUID(guidTarget);
		g_pServerManager->SendPacket(pMsg, INVALID_ID);
	}

__LEAVE_FUNCTION
}

const _CALLOF_INFO *Obj_Human::GetCallOfInfo(VOID)const
{
	if(m_CallOfInfo.m_uDuration > 0)
	{
		return &m_CallOfInfo;
	}
	else
	{
		return NULL;
	}
}

VOID Obj_Human::ResetCallOfInfo(VOID)
{
	m_CallOfInfo.Reset();
}

VOID Obj_Human::OnCallOfInfoChanged( VOID )
{
	__ENTER_FUNCTION
	if ( m_pAI_Character != NULL )
	{
		((AI_Human*)m_pAI_Character)->OnCallOfInfoChanged();
	}
	__LEAVE_FUNCTION
}

UINT Obj_Human::GetMoney()
{
	return m_DB.GetMoney();
}


BOOL	Obj_Human::IsVisualPart(HUMAN_EQUIP EquipPoint)
{
	switch(EquipPoint) 
	{
		case HEQUIP_MAINHAND:		//主手
		case HEQUIP_ASSIHAND:		//副手
		case HEQUIP_HEAD:			//头

		case HEQUIP_SHOULDER:		//肩
		case HEQUIP_BACK:			//躯干
		case HEQUIP_HAND:			//shou

		case HEQUIP_FEET:			//脚

		case HEQUIP_SUIT:			//外装
			{
				return TRUE;
			}
		default:
			return FALSE;
	}
}

VOID	Obj_Human::UpdataEquipDamagePoint(INT nType)	//更新攻击装备损伤点
{
	__ENTER_FUNCTION

	ItemContainer* pEquipContainer = GetEquipContain();
	if(NULL==pEquipContainer)
	{
		AssertEx(FALSE,"[Obj_Human::UpdataEquipDamagePoint]: NULL EquipContainer pointer found!");
	}

	UINT RetPart = 0;
	BOOL UpData  = FALSE;
	GCDetailEquipList	SelfMsg;
	SelfMsg.setObjID( GetID() );
	
	switch(nType)
	{
	case DURSPOIL_ATTACT:
		{
			for (UINT i=0; i<HEQUIP_NUMBER; ++i)
			{
				Item*	pEquipItem = pEquipContainer->GetItem((UINT)i);
				if(NULL==pEquipItem)
				{
					AssertEx(FALSE,"[Obj_Human::ItemEffectFlush]: NULL EquipItem pointer found!");
					continue;
				}
				if(TRUE==pEquipItem->IsEmpty())
				{
					continue;//这个位置是空的
				}
				BYTE  nDurPoints = pEquipItem->GetDurPoints();
				if(0==nDurPoints)
				{
					continue;
				}
				_DURSPOIL_VALUE_TBL::SPOIL_VALUE spoilInfo = g_DurSpoilValueTbl.Get(pEquipItem->GetItemType());
				if (spoilInfo.m_nDamageMode == DURSPOIL_ATTACT)
				{
					INT nDamagePoint = pEquipItem->GetDamagePoint();
	
					if (nDamagePoint<spoilInfo.m_nValue)
					{
						++nDamagePoint;
						BOOL Ret = g_ItemOperator.SetItemDamagePoint(pEquipContainer, i, nDamagePoint);
						Assert(Ret);
					}
					if (pEquipItem->GetDamagePoint()>=spoilInfo.m_nValue)
					{
						--nDurPoints;
						BOOL Ret = g_ItemOperator.SetItemDur(pEquipContainer, i, nDurPoints);
						Assert(Ret);
						Ret = g_ItemOperator.SetItemDamagePoint(pEquipContainer, i, 0);
						Assert(Ret);
						if (nDurPoints <= 0)
						{
							UpData = TRUE;
						}
						pEquipItem->SaveValueTo(SelfMsg.GetEquipData((HUMAN_EQUIP)i));
						RetPart|=(1<<((UINT)i));
					}
				}
			}
		}
		break;
	case DURSPOIL_DEFENCE:
		{
			for (UINT i=0; i<HEQUIP_NUMBER; ++i)
			{
				Item*	pEquipItem = pEquipContainer->GetItem((UINT)i);
				if(NULL==pEquipItem)
				{
					AssertEx(FALSE,"[Obj_Human::ItemEffectFlush]: NULL EquipItem pointer found!");
					continue;
				}
				if(TRUE==pEquipItem->IsEmpty())
				{
					continue;//这个位置是空的
				}
				BYTE  nDurPoints = pEquipItem->GetDurPoints();
				if(0==nDurPoints)
				{
					continue;
				}
				_DURSPOIL_VALUE_TBL::SPOIL_VALUE spoilInfo = g_DurSpoilValueTbl.Get(pEquipItem->GetItemType());
				if (spoilInfo.m_nDamageMode == DURSPOIL_DEFENCE)
				{
					INT nDamagePoint = pEquipItem->GetDamagePoint();

					if (nDamagePoint<spoilInfo.m_nValue)
					{
						++nDamagePoint;
						BOOL Ret = g_ItemOperator.SetItemDamagePoint(pEquipContainer, i, nDamagePoint);
						Assert(Ret);
					}
					if (pEquipItem->GetDamagePoint()>=spoilInfo.m_nValue)
					{
						--nDurPoints;
						BOOL Ret = g_ItemOperator.SetItemDur(pEquipContainer, i, nDurPoints);
						Assert(Ret);
						Ret = g_ItemOperator.SetItemDamagePoint(pEquipContainer, i, 0);
						Assert(Ret);
						if (nDurPoints <= 0)
						{
							UpData = TRUE;
						}
						pEquipItem->SaveValueTo(SelfMsg.GetEquipData((HUMAN_EQUIP)i));
						RetPart|=(1<<((UINT)i));
					}
				}
			}
		}
	    break;
	default:
		Assert(FALSE);
	    break;
	}
	
	if (RetPart>0)
	{
		SelfMsg.SetAskPart(RetPart);
		GetPlayer()->SendPacket( &SelfMsg ) ;
	}
	if (UpData)
	{
		ItemEffectFlush();
	}
	__LEAVE_FUNCTION
}

INT		Obj_Human::UseStoreMap(uint BagIndex)
{
	
	__ENTER_FUNCTION
		
		
		Item*		pStoreMapItem = HumanItemLogic::GetBagItem(this,BagIndex);
		Assert(pStoreMapItem);
	
		USEITEM_RESULT	Result;
		if(pStoreMapItem->GetItemClass()!=ICLASS_STOREMAP)
		{
			Result	=	USEITEM_TYPE_FAIL;
		}
		else
		{
			// 玩家处于不可行动状态
			if( !IsAlive() )
			{ 
				return USEITEM_SKILL_FAIL;
			}
			
			STORE_MAP_INFO_TB*	pGET = g_ItemTable.GetStoreMapTB(pStoreMapItem->GetItemTableIndex());
			Assert(pGET);
			if (!pGET)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::UseStoreMap] GetStoreMapTB index is error [%d]",pStoreMapItem->GetItemTableIndex());
				return USEITEM_SKILL_FAIL;
			}
			if(pGET->m_ScriptID>0)
			{
				Result = (USEITEM_RESULT)getScene()->GetLuaInterface()->ExeScript_DDD(	
					pGET->m_ScriptID,"OnUse",(INT)GetID(),(INT)BagIndex,(INT)getScene()->SceneID() );
			}
			if(Result==USEITEM_SUCCESS)
			{
				ITEM_LOG_PARAM ItemLogParam;
				BOOL bRet=	HumanItemLogic::EraseBagItem(&ItemLogParam,this,BagIndex);
				if(bRet)
				{
					SaveItemLog(&ItemLogParam);
				}
			}
		}

	return Result;
	
	__LEAVE_FUNCTION
	
	return USEITEM_SKILL_FAIL;
}

INT	Obj_Human::UseIdentScroll(UINT BagIndex,UINT TargetBagIndex)
{
	__ENTER_FUNCTION


	Item*		pScrollItem = HumanItemLogic::GetBagItem(this,BagIndex);
	Assert(pScrollItem);

	Item*		pTargetItem = HumanItemLogic::GetBagItem(this,TargetBagIndex);
	Assert(pTargetItem);


	USEITEM_RESULT	Result;
	if(pScrollItem->GetItemClass()!=ICLASS_IDENT)
	{
		Result	=	USEITEM_IDENT_TYPE_FAIL;
	}
	else if(pTargetItem->GetItemClass()!=ICLASS_EQUIP || !pTargetItem->IsRuler(IRL_IDENT))
	{
		Result = USEITEM_IDENT_TARGET_TYPE_FAIL;
	}
	else
	{
		// 玩家处于不可行动状态
		if( !IsAlive() )
		{ 
			return USEITEM_SKILL_FAIL;
		}
		switch(pTargetItem->GetEquipPoint()) 
		{
		case HEQUIP_MAINHAND:	//主手
		case HEQUIP_ASSIHAND:	//副手
			{
				if(pScrollItem->GetItemType() != IDENT_WPIDENT)
					return USEITEM_IDENT_TARGET_TYPE_FAIL;	
				break;
			}
		case HEQUIP_HEAD	:		//头
		case HEQUIP_SHOULDER:		//肩
		case HEQUIP_BACK	:		//躯干
		case HEQUIP_WRIST	:		//腕
		case HEQUIP_HAND	:		//手
		case HEQUIP_WAIST	:		//腰
		case HEQUIP_FEET	:		//脚
			{
				if(pScrollItem->GetItemType() != IDENT_ARIDENT)
					return USEITEM_IDENT_TARGET_TYPE_FAIL;
				break;
			}
		case HEQUIP_NECK:	//颈
		case HEQUIP_RING1:	//戒指1	
		case HEQUIP_RING2:	//戒指2
		case HEQUIP_ADORN1:	//饰品1
		case HEQUIP_ADORN2:	//饰品2
			{
				if(pScrollItem->GetItemType() != IDENT_NCIDENT)
					return USEITEM_IDENT_TARGET_TYPE_FAIL;
				break;
			}
		default:
			{
				Assert(FALSE);
			}
	
		}
		
		INT ScrollLevel	=  pScrollItem->GetItemLevel();
			
		
		INT	EquipLevel = pTargetItem->GetRequireLevel();
		
		INT ScrollRequireLevel = pScrollItem->GetItemLevel();


		if(ScrollRequireLevel>GetLevel())
		{
			return USEITEM_LEVEL_FAIL;
		}

#define IDENT_LEVEL_RANGE 9

		if(ScrollLevel+IDENT_LEVEL_RANGE<EquipLevel)
		{
			return USEITEM_IDENT_LEVEL_FAIL;
		}
		
		if(HumanItemLogic::SetBagItemIdent(this,TargetBagIndex))
		{
			Result = USEITEM_SUCCESS;
		}
		else
		{
			Result = USEITEM_SKILL_FAIL;	
		}
		if(Result==USEITEM_SUCCESS)
		{
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.OpType	= ITEM_USE_IDENT_LOST;
			ItemLogParam.CharGUID	=	GetGUID();
			ItemLogParam.Count		=	1;
			ItemLogParam.ItemGuid	=	pScrollItem->GetGUID();
			ItemLogParam.ItemType	=	pScrollItem->GetItemTableIndex();
			ItemLogParam.SceneID	=	getScene()->SceneID();
			ItemLogParam.XPos		=	getWorldPos()->m_fX;
			ItemLogParam.ZPos		=	getWorldPos()->m_fZ;

			BOOL bRet	= HumanItemLogic::DecBagItemLayCount(this,BagIndex);
		
			if(bRet) SaveItemLog(&ItemLogParam);
		}
	}

	return Result;

	__LEAVE_FUNCTION

	return USEITEM_SKILL_FAIL;
}

BOOL Obj_Human::VerifyItem(VOID)
{
	__ENTER_FUNCTION
	TargetingAndDepletingParams_T& rParams = GetTargetingAndDepletingParams();
	//检查物品是否合法
	INT nBagIndex = rParams.GetBagIndexOfDepletedItem();
	Item*	pBagItem = HumanItemLogic::GetBagItem(this,nBagIndex);
	INT nItemTableIndex =  rParams.GetItemIndexOfDepletedItem();
	if(pBagItem->IsEmpty() || pBagItem->GetItemTableIndex() != nItemTableIndex)
	{
		if(0<HumanItemLogic::CalcBagItemCount(this, nItemTableIndex))
		{
			nBagIndex=HumanItemLogic::GetBagItemPosByType(this, nItemTableIndex);
			if(0 <= nBagIndex)
			{
				rParams.SetBagIndexOfDepletedItem(nBagIndex);
				pBagItem = HumanItemLogic::GetBagItem(this,nBagIndex);
			}
		}
	}
	if(FALSE==pBagItem->IsEmpty())
	{
		if(rParams.GetItemIndexOfDepletedItem()==pBagItem->GetItemTableIndex())
		{
			if(rParams.GetActivatedScript()==pBagItem->GetScriptID())
			{
				if(rParams.GetActivatedSkill()==pBagItem->GetSkillID())
				{

					//如果是药品,并且当前不需要修补那么通知客户端.
					if(pBagItem->GetItemClass() == ICLASS_COMITEM)
					{
						if (pBagItem->GetItemType() == COMMON_ITEM_HP && GetHP() >= GetMaxHP() )
						{
							SendOperateResultMsg(OR_HP_IS_FULL);
							return FALSE;
						}
						else if (pBagItem->GetItemType() == COMMON_ITEM_MP && GetMP() >= GetMaxMP())
						{
							SendOperateResultMsg(OR_MP_IS_FULL);
							return FALSE;
						}
					}
					return TRUE;
				}
			}
		}
	}
	SendOperateResultMsg(OR_CAN_NOT_FIND_SPECIFIC_ITEM);
	return FALSE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL Obj_Human::ActivateItem(VOID)
{
	__ENTER_FUNCTION
	SkillInfo_T& rSkillInfo = GetSkillInfo();
	TargetingAndDepletingParams_T& rParams = GetTargetingAndDepletingParams();
	BOOL bRet = FALSE;
	rSkillInfo.Init();
	if(FALSE==g_SkillCore.InstanceSkill(rSkillInfo, *this, rParams.GetActivatedSkill()))
	{
		rParams.SetErrCode(OR_INVALID_SKILL);
		rParams.SetErrParam(rParams.GetActivatedSkill());
		return FALSE;
	}
	switch (rSkillInfo.GetSkillType())
	{
		case SKILL_INSTANT_LAUNCHING:
			bRet = g_ScriptLogic.StartLaunching(*this);
			break;
		case SKILL_NEED_CHARGING:
			bRet = g_ScriptLogic.StartCharging(*this);
			break;
		case SKILL_NEED_CHANNELING:
			bRet = g_ScriptLogic.StartChanneling(*this);
			break;
		default:
			bRet = FALSE;
			AssertEx(FALSE,"[Obj_Human::ActiveItem]: Unknow Item script or skill type!!");
			break;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL Obj_Human::UseItem(ID_t nBagIndex, ObjID_t nTargetObj, const WORLD_POS& posTarget, 
		PET_GUID_t const& guidTargetPet, ID_t nTargetItem)
{
	__ENTER_FUNCTION

	if(!IsCharacterLogicStopped())
		StopCharacterLogic(TRUE);

	TargetingAndDepletingParams_T& rParams = GetTargetingAndDepletingParams();
	if(FALSE==IsAlive()||FALSE==IsActiveObj())
	{
		rParams.SetErrCode(OR_DIE);
		return FALSE;
	}
	//检查物品是否合法
	Item*	pBagItem = HumanItemLogic::GetBagItem(this,nBagIndex);
	if(pBagItem->IsEmpty())
	{
		return FALSE;
	}

	AddLogicCount();

	// need gaoqi add...
	//设置物品使用需要的参数
	rParams.SetTargetBagSlot(nTargetItem);
	rParams.SetBagIndexOfDepletedItem(nBagIndex);
	rParams.SetDepletedItemGuid(pBagItem->GetGUID());
	rParams.SetItemIndexOfDepletedItem(pBagItem->GetItemTableIndex());
	rParams.SetActivatedScript(pBagItem->GetScriptID());
	rParams.SetActivatedSkill(pBagItem->GetSkillID());
	rParams.SetTargetObj(nTargetObj);
	rParams.SetTargetPetGuid(guidTargetPet);
	rParams.SetTargetPosition(posTarget);
	//设置结束
	//Activate script and skill
	INT nRet = ActivateItem();
	if (GetCharacterLogic() != CHARACTER_LOGIC_USE_SKILL || TRUE==IsCharacterLogicStopped())
	{
		SetCharacterLogic( CHARACTER_LOGIC_USE_SKILL );
	}
	if (TRUE==GetGlobalActionDelegator().CanDoNextAction(*this))
	{
		if (GetCharacterLogic() == CHARACTER_LOGIC_USE_SKILL || FALSE==IsCharacterLogicStopped())
		{
			StopCharacterLogic();
		}		
	}	
	return nRet;
	__LEAVE_FUNCTION
	return FALSE;

}

INT  Obj_Human::UseEquip(INT BagIndex)
{
	__ENTER_FUNCTION
	Assert(BagIndex>=0);
	
	Item* pUseItem = HumanItemLogic::GetBagItem(this, BagIndex);
	Assert(pUseItem);
	if (pUseItem->IsEmpty())
	{
		Assert(0);
		return INVALID_INDEX;
	}

	if(!pUseItem->IsWuMenPai())
	{
		//需求等级判断,职业判断
		if(pUseItem->GetRequireLevel()>GetLevel())
		{
			return INVALID_INDEX;
		}
		else if (!pUseItem->InReqJob(GetMenPai()))
		{
			return INVALID_INDEX;
		}
	}

	HUMAN_EQUIP		Equip_Point =(HUMAN_EQUIP)pUseItem->GetEquipPoint();
	ItemContainer*	pEquipContainer = GetEquipContain();

	if(!pEquipContainer)
	{
		Assert(0);
		return INVALID_INDEX;;
	}

	Item*	pEquipItem = pEquipContainer->GetItem((UINT)Equip_Point);

	if(!pEquipItem)
	{
		Assert(0);
		return INVALID_INDEX;
	}

	BOOL			bEquipStatus = (!pEquipItem->IsEmpty());

	ItemContainer*	pBagContainer =
		HumanItemLogic::GetBagContainer(this,BagIndex);

	if(bEquipStatus)
	{
		if(Equip_Point == HEQUIP_RING1)
		{
			pEquipItem = pEquipContainer->GetItem((UINT)HEQUIP_RING2);

			if(!pEquipItem)
			{
				Assert(0);
				return INVALID_INDEX;
			}

			BOOL b2EquipStatus = (pEquipItem->IsEmpty());
			if(b2EquipStatus)
			{
				g_ItemOperator.MoveItem(pBagContainer,
					pBagContainer->BagIndex2ConIndex(BagIndex),
					pEquipContainer,
					(UINT)HEQUIP_RING2);

				Equip_Point = HEQUIP_RING2;
			}

			else
			{
				g_ItemOperator.ExchangeItem(pBagContainer,
					pBagContainer->BagIndex2ConIndex(BagIndex),
					pEquipContainer,
					(UINT)Equip_Point);
			}
		}
		else if(Equip_Point == HEQUIP_ADORN1)
		{
			pEquipItem = pEquipContainer->GetItem((UINT)HEQUIP_ADORN2);

			if(!pEquipItem)
			{
				Assert(0);
				return INVALID_INDEX;
			}

			BOOL b2EquipStatus = (pEquipItem->IsEmpty());
			if(b2EquipStatus)
			{
				g_ItemOperator.MoveItem(pBagContainer,
					pBagContainer->BagIndex2ConIndex(BagIndex),
					pEquipContainer,
					(UINT)HEQUIP_ADORN2);

				Equip_Point = HEQUIP_ADORN2;
			}
			else
			{
				g_ItemOperator.ExchangeItem(pBagContainer,
					pBagContainer->BagIndex2ConIndex(BagIndex),
					pEquipContainer,
					(UINT)Equip_Point);
			}
		}
		else
		{
			g_ItemOperator.ExchangeItem(pBagContainer,
				pBagContainer->BagIndex2ConIndex(BagIndex),
				pEquipContainer,
				(UINT)Equip_Point);
		}
	}
	else
	{ 
		g_ItemOperator.MoveItem(pBagContainer,
			pBagContainer->BagIndex2ConIndex(BagIndex),
			pEquipContainer,
			(UINT)Equip_Point);
	}

	Item* pEquip = pEquipContainer->GetItem((UINT)Equip_Point);

	if(pEquip&&pEquip->IsCanEquipBind())
	{
		g_ItemOperator.SetItemBind(pEquipContainer,(UINT)Equip_Point);	
	}

	SetEquipVer(GetEquipVer()+1);
	//刷新装备对人物的属性影响

	__LEAVE_FUNCTION
		return INVALID_INDEX;
}

INT  Obj_Human::UseGem(Item* pEquipItem, uint GemBagIndex, UINT GemHolePos)
{
	__ENTER_FUNCTION
	
	Assert(pEquipItem != NULL);

	Item*		 pGemItem		=	HumanItemLogic::GetBagItem(this,GemBagIndex);
	if(pGemItem == NULL)
	{
		Assert(0);
	}

	if(pGemItem->IsEmpty())
	{
		Assert(0);
	}
	
	Assert(pGemItem->GetItemClass()==ICLASS_GEM);
	Assert(pEquipItem->GetItemClass()==ICLASS_EQUIP);

	BOOL bOK = pEquipItem->AddGem(GemHolePos, pGemItem->GetItemTableIndex());
	Assert(bOK);

	
	ITEM_LOG_PARAM ItemLogParam;
	ItemLogParam.OpType  	=	ITEM_USEGEM_REMOVE;

	BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,this,GemBagIndex); // 删除宝石
	if(!bRet)
	{
		Assert(0);
	}
	SaveItemLog(&ItemLogParam);

	//去掉丢弃物品消息
	/*GCDiscardItemResult Msg;
	Msg.setItemTableIndex( pGemItem->GetItemTableIndex());
	Msg.setResult( DISCARDITEM_SUCCESS );
	Msg.setBagIndex( GemBagIndex );
	((GamePlayer*)GetPlayer())->SendPacket( &Msg );*/

	GCItemInfo	 Msg;
	Msg.setIsNull(TRUE);
	Msg.setID(GemBagIndex);
	GetPlayer()->SendPacket(&Msg);

	return USEGEM_SUCCESS;

	__LEAVE_FUNCTION

	return USEGEM_FAIL ;
}


INT Obj_Human::RemoveGem(uint BagIndex,uint GemIndex)
{
	__ENTER_FUNCTION

	/*Assert(BagIndex<MAX_BAG_SIZE);
	
	Item* pEquipItem = HumanItemLogic::GetBagItem(this, BagIndex);
	Assert(pEquipItem);

	if(pEquipItem->IsEmpty())
	{
		return REMOVEGEM_NO_ITEM;
	}

	if(pEquipItem->GetItemClass() != ICLASS_EQUIP)
	{
		return REMOVEGEM_INVALID_EQUIP;
	}

	if(GemIndex >= pEquipItem->GetGemSlotMax())
	{
		return REMOVEGEM_ERROR_GEMINDEX;
	}

	UINT GemID = pEquipItem->GetEquipGemInfo(GemIndex).m_GemID;

	if(GemID == INVALID_ID)
	{
		return REMOVEGEM_NO_GEM;
	}
	
	ITEM_LOG_PARAM	ItemLogParam;
	ItemLogParam.OpType		= ITEM_CREATE_REMOVE_GEM;
	ItemLogParam.CharGUID	= GetGUID();
	ItemLogParam.SceneID	= getScene()->SceneID();
	ItemLogParam.XPos		= getWorldPos()->m_fX;
	ItemLogParam.ZPos		= getWorldPos()->m_fZ;

	UINT GemBagIndex;
	if(!HumanItemLogic::CreateItemToBag(&ItemLogParam,this, GemID, GemBagIndex))
	{
		return	REMOVEGEM_BAG_FULL;	
	}

	SaveItemLog(&ItemLogParam);

	GCNotifyEquip Msg;
	Msg.SetBagIndex( GemBagIndex );
	Item* pItem = HumanItemLogic::GetBagItem(this, GemBagIndex);
	Assert(pItem);
	pItem->SaveValueTo(Msg.GetItem());
	GetPlayer()->SendPacket(&Msg);

	ItemContainer*	pContainer = HumanItemLogic::GetContainer(this,BagIndex);
	if(!pContainer)
	{
		return REMOVEGEM_NO_ITEM;
	}

	g_ItemOperator.DelItemAttr(pContainer,
							   pContainer->BagIndex2ConIndex(BagIndex),
							   pItem->GetGemAttr());

	g_ItemOperator.DelGemInfo(pContainer,
							  pContainer->BagIndex2ConIndex(BagIndex),
							  GemIndex);

	ItemEffectFlush();
	return  REMOVEGEM_SUCCESS;*/

	__LEAVE_FUNCTION

	return REMOVEGEM_UNKNOW_ERROR;
}

INT		Obj_Human::AddHole(Item* pEquipItem)
{
	__ENTER_FUNCTION

	Assert(pEquipItem);
	if(pEquipItem->GetCurGemHole()== MAX_GEM_HOLE)
	{
		return ADDHOLE_FULL_FAIL;
	}
	
	INT nCurHoleNum = pEquipItem->GetCurGemHole();
	EQUIPHOLE_INFO_TB* pHoleInfoTb = g_ItemTable.GetEquipHoleInfoTB(++nCurHoleNum);
	Assert(pHoleInfoTb);
	if (!pHoleInfoTb)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::AddHole] GetEquipHoleInfoTB index is error [%d]",nCurHoleNum);
		return ADDHOLE_FAIL;
	}
	if(pHoleInfoTb->m_ReqMoney > GetMoney())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::AddHole Money is not enough") ;
		return ADDHOLE_MONEY_FAIL;
	}

	if(pHoleInfoTb->m_ReqItem != INVALID_ITEM_ID)
	{
		INT nItemCount = HumanItemLogic::CalcBagItemCount(this, pHoleInfoTb->m_ReqItem);
		if((UINT)nItemCount < pHoleInfoTb->m_ReqItemNum)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::AddHole Item is not enough") ;
			return ADDHOLE_ITEM_FAIL;
		}

		BOOL bAdded = pEquipItem->AddHole();
		if(bAdded)
		{
			ITEM_LOG_PARAM	ItemLogParam;
			ItemLogParam.ItemType	= pHoleInfoTb->m_ReqItem;
			ItemLogParam.OpType		= ITEM_ADDHOLE_CONSUME; 

			BOOL bUseOK = HumanItemLogic::EraseBagItem(&ItemLogParam, this,  pHoleInfoTb->m_ReqItem,  pHoleInfoTb->m_ReqItemNum);
			if(!bUseOK)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::AddHole HumanItemLogic::EraseBagItem error") ;
				return ADDHOLE_FAIL;
			}
			SaveItemLog(&ItemLogParam);
			
			SetMoney(GetMoney()-pHoleInfoTb->m_ReqMoney);
			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	GetGUID();
			MoneyLogParam.OPType	=	MONEY_ADDHOLE_MONEYREDUCE;	
			MoneyLogParam.Count		=	(INT)pHoleInfoTb->m_ReqMoney;
			MoneyLogParam.SceneID	=	getScene()->SceneID();
			MoneyLogParam.XPos		=	getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);

			g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::AddHole success") ;
			
		}
	}	
	return ADDHOLE_SUCCESS;

	__LEAVE_FUNCTION

		return ADDHOLE_FAIL ;
}

INT		Obj_Human::EquipLevelup(Item* pEquipItem, UINT nGemID, UINT nGemCount)
{
	__ENTER_FUNCTION

	Assert(pEquipItem);
	Assert(nGemID != INVALID_ITEM_ID);
	Assert(nGemCount<=MAX_LEVELUP_GEMREQ_NUM);
	
	COMMITEM_INFO_TB* pCommItem = g_ItemTable.GetCommItemInfoTB(nGemID);
	if(!pCommItem)
	{
		return EQUIPLEVEUP_FAIL;
	}

	UINT nCurLevel = pEquipItem->GetCurLevel();
	if(nCurLevel >= MAX_EQUIP_LEVELUP_NUM)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup CurLevel MAX") ;
		return EQUIPLEVEUP_FULL_FAIL;
	}

	EQUIP_LEVELUP_TB* pLevelupInfoTb = g_ItemTable.GetEquipLeveupTB(++nCurLevel);
	Assert(pLevelupInfoTb);
	if (!pLevelupInfoTb)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::EquipLevelup] GetEquipLeveupTB index is error [%d]",nCurLevel);
		return EQUIPLEVEUP_FAIL ;
	}
	if(pLevelupInfoTb->m_ReqMoney > GetMoney())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup Money is not enough") ;
		return EQUIPLEVEUP_MONEY_FAIL;
	}

	if (pEquipItem->GetRequireLevel()<60)
	{
		if(pLevelupInfoTb->m_LowLevelGemID != nGemID)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup GemID error") ;
			return EQUIPLEVEUP_ITEMTYPE_FAIL;
		}
	}
	
	else
	{
		if(pLevelupInfoTb->m_HighLevelGemID != nGemID)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup GemID error") ;
			return EQUIPLEVEUP_ITEMTYPE_FAIL;
		}
	}
	INT nItemCount = HumanItemLogic::CalcBagItemCount(this, nGemID);
	if((UINT)nItemCount < nGemCount)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup Gem is not enough") ;
		return EQUIPLEVEUP_ITEMNUM_FAIL;
	}

	UINT nRate = RandGen::GetRand(0, 100);
	if(nRate<pLevelupInfoTb->m_SuccessRate[nGemCount-1])
	{
		pEquipItem->EquipLevelup(1);
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.ItemType	= nGemID;
		ItemLogParam.CharGUID	= GetGUID();
		ItemLogParam.OpType		= ITEM_EQUIPLEVELUP_ITEM_CONSUME; 
		BOOL bUseOK = HumanItemLogic::EraseBagItem(&ItemLogParam, this,  nGemID,  nGemCount);
		if(!bUseOK)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup HumanItemLogic::EraseBagItem error") ;
			return EQUIPLEVEUP_FAIL;
		}
		SaveItemLog(&ItemLogParam);
		SetMoney(GetMoney()-pLevelupInfoTb->m_ReqMoney);

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	GetGUID();
		MoneyLogParam.OPType	=	MONEY_EQUIPLEVEL_REDUCE;	
		MoneyLogParam.Count		=	(INT)pLevelupInfoTb->m_ReqMoney;
		MoneyLogParam.SceneID	=	getScene()->SceneID();
		MoneyLogParam.XPos		=	getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup success") ;
		return EQUIPLEVEUP_SUCCESS;
	}
	else
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup Fail CurLevel=%d", nCurLevel) ;
		INT nPunishment = pLevelupInfoTb->m_Punishment;
		pEquipItem->EquipLevelup(-nPunishment);

		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.ItemType	= nGemID;
		ItemLogParam.CharGUID	= GetGUID();
		ItemLogParam.OpType		= ITEM_EQUIPLEVELUP_ITEM_CONSUME; 
		BOOL bUseOK = HumanItemLogic::EraseBagItem(&ItemLogParam, this,  nGemID,  nGemCount);
		if(!bUseOK)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup HumanItemLogic::EraseBagItem error") ;
			return EQUIPLEVEUP_FAIL;
		}
		SaveItemLog(&ItemLogParam);
		SetMoney(GetMoney()-pLevelupInfoTb->m_ReqMoney);

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	GetGUID();
		MoneyLogParam.OPType	=	MONEY_EQUIPLEVEL_REDUCE;	
		MoneyLogParam.Count		=	(INT)pLevelupInfoTb->m_ReqMoney;
		MoneyLogParam.SceneID	=	getScene()->SceneID();
		MoneyLogParam.XPos		=	getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		g_pLog->FastSaveLog( LOG_FILE_1, "Obj_Human::EquipLevelup Fail") ;
		return EQUIPLEVEUP_FAIL_SUCCESS;
	}
	
	return EQUIPLEVEUP_SUCCESS;

	__LEAVE_FUNCTION

		return EQUIPLEVEUP_FAIL ;
}

INT		Obj_Human::EquipAdsorbMagic(Item *pEquipItem, Item *pBeadItem)
{
	__ENTER_FUNCTION
	Assert(pEquipItem);
	Assert(pBeadItem);

	BYTE Kind = 0;		  // 魂珠种类

	Kind = pBeadItem->GetItemType();

	// 如果附魂成功
	if ( pEquipItem->AdsorbMagic(pBeadItem->GetItemData() , Kind ) )
	{
		return EQUIPADSORMAGIC_SUCCESS;
	}
	

	__LEAVE_FUNCTION
		return EQUIPADSORMAGIC_EQUIP_FAIL;
}

INT		Obj_Human::EquipCancelMagic(Item *pEquipItem)
{
	__ENTER_FUNCTION
	Assert(pEquipItem);

	if ( pEquipItem->CancelMagic() )
	{
		return EQUIPCANCELMAGIC_SUCCESS;
	}

	__LEAVE_FUNCTION
		return EQUIPCANCELMAGIC_EQUIP_FAIL;
}


INT		Obj_Human::EquipRemould(Item* pEquipItem, UINT nMaterialScore)
{
	Assert(pEquipItem != NULL);

	MATERIAL_LEVEL_INFO_TB* pMaxMaterialLevelInfo = g_ItemTable.GetMaterialInfoTB(MAX_MATERIAL_LEVEL);
	if (!pMaxMaterialLevelInfo)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::EquipRemould] GetMaterialInfoTB index is error [%d]",MAX_MATERIAL_LEVEL);
		return EQUIPREMOULD_EQUIP_FAIL;
	}

	UINT nScore = pEquipItem->GetEquipScore() + nMaterialScore;
	if (nScore>=pMaxMaterialLevelInfo->m_MaterialValue)
	{
		nScore = pMaxMaterialLevelInfo->m_MaterialValue;
	}
	pEquipItem->SetEquipScore(nScore);
	UINT nPointValue = g_ItemTable.GetEquipPointValue((HUMAN_EQUIP)pEquipItem->GetEquipPoint());
	Assert(nPointValue != 0);
	UINT nJob = GetMenPai();
	EQUIPEXTRATT_TB* pEquipExtraAtt	= g_ItemTable.GetEquipExtraAttInfoTB(nScore);
	Assert(pEquipExtraAtt);
	if (!pEquipExtraAtt)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::EquipRemould] GetEquipExtraAttInfoTB index is error [%d]",nScore);	
		return EQUIPREMOULD_EQUIP_FAIL;
	}
	EQUIPEXTRATT equipExtraAtt = pEquipExtraAtt->m_EquipExtrAtt[nJob];
	UINT nBaseAttCount = 0;
	for (UINT i=0; i<pEquipItem->GetAttrCount(); ++i)
	{
		_ITEM_ATTR ItemAttr = pEquipItem->GetEquipAttr(i);
		switch(ItemAttr.m_AttrType)
		{
		case IATTRIBUTE_BASE_ATTACK_NEAR:
			{
				++nBaseAttCount;
				continue;
			}
			break;
		case IATTRIBUTE_BASE_ATTACK_FAR:
			{
				++nBaseAttCount;
				continue;
			}
			break;
		case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR:
			{
				++nBaseAttCount;
				continue;
			}
		    break;
		case IATTRIBUTE_BASE_ATTACK_MAGIC_FAR:
			{
				++nBaseAttCount;
				continue;
			}
			break;
		case IATTRIBUTE_BASE_DEFENCE_NEAR:
			{
				++nBaseAttCount;
				continue;
			}
		    break;
		case IATTRIBUTE_BASE_DEFENCE_FAR:
			{
				++nBaseAttCount;
				continue;
			}
			break;
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR:
			{
				++nBaseAttCount;
				continue;
			}
			break;
		case IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR:
			{
				++nBaseAttCount;
				continue;
			}
			break;
		default:
		    break;
		}
		Assert(equipExtraAtt.m_GenAttr[i-nBaseAttCount].m_AttrCount != 0);
		UINT nSet = RandGen::GetRand(0, equipExtraAtt.m_ValidCount-1);
		UINT nAtt = RandGen::GetRand(0, equipExtraAtt.m_GenAttr[nSet].m_AttrCount-1);
		EQUIP_ATT_LEVEL_TB* pAttTB = g_ItemTable.GetAttLevelValue((ITEM_ATTRIBUTE)equipExtraAtt.m_GenAttr[nSet].m_Attr[nAtt]);
		if (!pAttTB)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::EquipRemould] GetAttLevelValue index is error [%d]",(ITEM_ATTRIBUTE)equipExtraAtt.m_GenAttr[nSet].m_Attr[nAtt]);	
			return EQUIPREMOULD_EQUIP_FAIL;
		}
		UINT nValue = pAttTB->m_Attr[pEquipItem->GetRequireLevel()];
		//基础数值*被选中的次数*装备分值加成。
		double nLogScore = log((double)nScore);
		nValue = (UINT)((double)nValue*nPointValue*nLogScore/(double)100);
		if (nValue == 0)
		{
			nValue = 1;
		}

		ItemAttr.m_AttrType			= equipExtraAtt.m_GenAttr[nSet].m_Attr[nAtt];
		ItemAttr.m_Value.m_Value	= nValue;
		pEquipItem->SetEquipAttr(i, ItemAttr);
	}
	
	return EQUIPREMOULD_SUCCESS;

}

INT		Obj_Human::EquipBind(Item* pEquipItem,uint GemBagIndex)
{
	__ENTER_FUNCTION

	Assert(pEquipItem != NULL);

	Item*		 pGemItem		=	HumanItemLogic::GetBagItem(this,GemBagIndex);
	if(pGemItem == NULL)
	{
		return EQUIPBIND_ITEM_FAIL;
	}

	if(pGemItem->IsEmpty())
	{
		return EQUIPBIND_ITEM_FAIL;
	}
	
	Assert(pEquipItem->GetItemClass()==ICLASS_EQUIP);

	INT nCurBind = pEquipItem->GetEquipBindLevel();

	if(nCurBind == MAX_EQUIP_BIND_LEVEL)
	{
		return EQUIPBIND_FULL_FAIL;
	}
	EQUIP_BIND_TB* pEquipBindTb = g_ItemTable.GetEquipBindTB(++nCurBind);
	Assert(pEquipBindTb);
	if (!pEquipBindTb)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::EquipBind] GetAttLevelValue index is error [%d]",nCurBind);	
		return EQUIPBIND_FULL_FAIL;
	}

	if(pEquipBindTb->m_GemID != pGemItem->GetItemTableIndex())
	{
		return EQUIPBIND_ITEM_FAIL;
	}

	pEquipItem->EquipBind(nCurBind);

	
	ITEM_LOG_PARAM ItemLogParam;
	ItemLogParam.OpType		=	ITEM_EQUIPBIND_ITEM_CONSUME;
	ItemLogParam.ItemType	=   pGemItem->GetItemTableIndex();
	BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,this,pGemItem->GetItemTableIndex(), 1); // 删除宝石
	if(!bRet)
	{
		Assert(0);
	}
	SaveItemLog(&ItemLogParam);

	return EQUIPBIND_SUCCESS;

	__LEAVE_FUNCTION

		return EQUIPBIND_FAIL ;
}

INT		Obj_Human::EquipChangePai(Item* pEquipItem, uint GemBagIndex)
{
	__ENTER_FUNCTION

		Assert(pEquipItem != NULL);

	Item*		 pGemItem		=	HumanItemLogic::GetBagItem(this,GemBagIndex);
	if(pGemItem == NULL)
	{
		return EQUIPCHANGEPAITTYPE_ITEM_FAIL;
	}

	if(pGemItem->IsEmpty())
	{
		return EQUIPCHANGEPAITTYPE_ITEM_FAIL;
	}
	
	Assert(pEquipItem->GetItemClass()==ICLASS_EQUIP);

	COMMITEM_INFO_TB* pCommItem = g_ItemTable.GetCommItemInfoTB(pGemItem->GetItemTableIndex());
	if(!pCommItem)
	{
		return EQUIPCHANGEPAITTYPE_ITEM_FAIL;
	}
	
	CHANGEPAI_INFO_TB* pPaiInfo = g_ItemTable.GetChangePaiInfoTB(pGemItem->GetItemTableIndex());
	if (!pPaiInfo)
	{
		return EQUIPCHANGEPAITTYPE_ITEM_FAIL;
	}
	
	if ((pPaiInfo->m_DestPaiType == INVALID_PAI_TYPE)||(pPaiInfo->m_DestPaiType == EQUIPPAI_ANYFEIZI))
	{
		UINT nCount = g_ItemTable.GetNoZiPaiCount();
		if (nCount>0)
		{
			INT nRand = RandGen::GetRand(0, nCount-1);
			pEquipItem->SetEquipPaiType(g_ItemTable.GetNoZiPai(nRand));
		}
		
	}
	else
		pEquipItem->SetEquipPaiType(pPaiInfo->m_DestPaiType);

	ITEM_LOG_PARAM ItemLogParam;
	ItemLogParam.OpType	=	ITEM_EQUIPCHANGEPAI_ITEM_CONSUME;
	ItemLogParam.ItemType = pGemItem->GetItemTableIndex();
	BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,this,pGemItem->GetItemTableIndex(), 1); // 删除宝石
	if(!bRet)
	{
		Assert(0);
	}
	SaveItemLog(&ItemLogParam);

	return EQUIPCHANGEPAITTYPE_SUCCESS;

	__LEAVE_FUNCTION

		return EQUIPCHANGEPAITTYPE_FAIL ;
}

INT		Obj_Human::EquipChangePai(Item* pEquip1Item, Item* pEquip2Item, uint GemBagIndex)
{
	__ENTER_FUNCTION

		Assert(pEquip1Item != NULL);
	Assert(pEquip2Item != NULL);

	Item*		 pGemItem		=	HumanItemLogic::GetBagItem(this,GemBagIndex);
	if(pGemItem == NULL)
	{
		return EQUIPEXCHANGEPAITTYPE_ITEM_FAIL;
	}

	if(pGemItem->IsEmpty())
	{
		return EQUIPEXCHANGEPAITTYPE_ITEM_FAIL;
	}
	
	COMMITEM_INFO_TB* pCommItem = g_ItemTable.GetCommItemInfoTB(pGemItem->GetItemTableIndex());
	if(!pCommItem)
	{
		return EQUIPEXCHANGEPAITTYPE_ITEM_FAIL;
	}
	
	if (pGemItem->GetItemTableIndex() != 11970001)
	{
		return EQUIPEXCHANGEPAITTYPE_ITEM_FAIL;
	}

	Assert(pEquip1Item->GetItemClass()==ICLASS_EQUIP);
	Assert(pEquip2Item->GetItemClass()==ICLASS_EQUIP);
	
	UINT	nPai1 = pEquip1Item->GetEquipPaiType();
	UINT	nPai2 = pEquip2Item->GetEquipPaiType();

	if ((nPai1 == INVALID_PAI_TYPE)&&(nPai2 == INVALID_PAI_TYPE))
	{
		return EQUIPEXCHANGEPAITTYPE_EQUIPPAI_FAIL;
	}

	if ((!pEquip1Item->GetItemBind())||(!pEquip2Item->GetItemBind()))
	{
		Assert(0);
	}

	pEquip1Item->SetEquipPaiType(nPai2);
	pEquip2Item->SetEquipPaiType(nPai1);

	ITEM_LOG_PARAM ItemLogParam;
	ItemLogParam.OpType	  =	ITEM_EQUIPCHANGEPAI_ITEM_CONSUME;
	ItemLogParam.ItemType = pGemItem->GetItemTableIndex();
	BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,this,pGemItem->GetItemTableIndex(), 1); // 删除宝石
	if(!bRet)
	{
		Assert(0);
	}
	SaveItemLog(&ItemLogParam);

	return EQUIPEXCHANGEPAITTYPE_SUCCESS;

	__LEAVE_FUNCTION

		return EQUIPEXCHANGEPAITTYPE_FAIL ;
}

VOID	Obj_Human::ChangeMenpai(SKILL_LOG_PARAM* pSkillLog,XINFA_LOG_PARAM* pXinFaLog,INT Menpai)
{
	__ENTER_FUNCTION
	HumanSkillStudy oSkillStudy;
	oSkillStudy.StudySkill( pSkillLog, pXinFaLog, Menpai, this );
	return;
	__LEAVE_FUNCTION
}

INT Obj_Human::SkillCharacter( Obj_Character* pCharacter, SkillID_t SkillID, BOOL bNow )
{
__ENTER_FUNCTION

	return 0 ;

__LEAVE_FUNCTION

	return 0 ;
}

VOID Obj_Human::SetAlterExp( INT AlterExp )//改变的经验
{
__ENTER_FUNCTION

	if (!IsAlive())
	{
		return;
	}

	_ITEM_EFFECT* pIE = ItemEffect(IATTRIBUTE_EXPRENCE_GET);
	Assert(pIE);
	if(pIE->IsActive())
	{
		AlterExp = Float2Int(AlterExp * (100 + pIE->m_Attr.m_Value) /100.0f);
	}
	
	UINT CurExp = m_DB.GetExp( ) ;
	SetExp( CurExp+AlterExp ) ;

	//判断是否升级
	//...
	UINT	CurrentExp	=	GetExp();
	UINT	iLevel		=	GetLevel();
	UINT	LevelExp	=	g_LevelExpTbl.Get(iLevel-1);	

	if( CurrentExp>=LevelExp && iLevel<100 )
	{
		CurrentExp-=LevelExp;
		this->SetExp(CurrentExp);
		this->Levelup();

		this->SetHP( this->GetMaxHP() ) ;
		this->SetMP( this->GetMaxMP() ) ;
		this->SetPneuma( this->GetMaxPneuma() ) ;

		GCLevelUpResult ResultMsg;
		ResultMsg.SetRemainExp(CurrentExp);
		ResultMsg.SetLevelUpResult(LEVELUP_SUCCESS);

		GetPlayer()->SendPacket(&ResultMsg);
		
		GCLevelUp	otherMsg;
		otherMsg.setObjId(this->GetID());
		otherMsg.setLevel(this->GetLevel());

		getScene()->BroadCast(&otherMsg,this,TRUE);
	   
		GWLevelUp* pWorldMsg = (GWLevelUp*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_LEVEUP));
		pWorldMsg->SetGUID( this->GetGUID() );
		pWorldMsg->SetLevel( this->GetLevel() );
		pWorldMsg->SetExp( this->GetExp() );

		g_pServerManager->SendPacket( pWorldMsg, INVALID_ID );
	}



	UINT afterExp	= GetExp();
	if( afterExp==CurExp )
	{//经验到达上限后需要客户端提示一下
		//SendOperateResultMsg(OR_EXP_FULL);//孔祥来质疑这个提示的合理性。
	}



__LEAVE_FUNCTION
}

VOID	Obj_Human::SetExp(UINT uExp)
{
#define MAX_EXP_RATE 4
	//限制经验为4倍当前等级升级经验
	UINT	LevelExp =		g_LevelExpTbl.Get(GetLevel()-1);
	if(uExp>LevelExp*MAX_EXP_RATE)
	{
		uExp = LevelExp*MAX_EXP_RATE;
	}
	m_DB.SetExp(uExp);
}

BOOL Obj_Human::IsEnemy(Obj_Character* pCharacter )
{
__ENTER_FUNCTION
	
	Assert( pCharacter ) ;
	if(GetID()==pCharacter->GetID())
	{
		return FALSE;
	}
	if(TRUE==IsPartner(pCharacter))
	{
		return FALSE;
	}
	BOOL bIsEnemy = IsEnemyCamp(*pCharacter);
	
	return bIsEnemy ;

__LEAVE_FUNCTION

	return FALSE ;
}
BOOL Obj_Human::IsFriend(Obj_Character* pCharacter)
{
__ENTER_FUNCTION
	
	Assert( pCharacter ) ;
	return FALSE==IsEnemy(pCharacter);

__LEAVE_FUNCTION

	return FALSE ;
}
BOOL Obj_Human::IsPartner(Obj_Character* pCharacter)
{
__ENTER_FUNCTION
	
	Assert( pCharacter ) ;
	if(NULL==pCharacter)
	{
		return FALSE;
	}
	if(GetID()==pCharacter->GetID())
	{
		return TRUE;
	}
	TeamID_t nTeamIDSelf = __GetTeamID();
	if(INVALID_ID==nTeamIDSelf)
	{
		return FALSE;
	}
	TeamID_t nTeamIDTar = INVALID_ID;
	if(OBJ_TYPE_HUMAN == pCharacter->GetObjType())
	{
		nTeamIDTar = ((Obj_Human*)pCharacter)->__GetTeamID();
	}
	else if(OBJ_TYPE_PET == pCharacter->GetObjType())
	{
		Obj_Character* pOwner=((Obj_Pet*)pCharacter)->GetOwner();
		
		if(NULL != pOwner&& OBJ_TYPE_HUMAN==pOwner->GetObjType())
		{
			nTeamIDTar = ((Obj_Human*)pOwner)->__GetTeamID();
		}
	}
	if(INVALID_ID==nTeamIDTar)
	{
		return FALSE;
	}
	if(nTeamIDSelf==nTeamIDTar)
	{
		return TRUE;
	}	
__LEAVE_FUNCTION
	return FALSE ;
}
BOOL Obj_Human::IsMySpouse(Obj_Human& rHuman)
{
	__ENTER_FUNCTION
	// 判断
	if(GetHumanRelation()->GetSpouseGUID() == rHuman.GetGUID())
	{
		return TRUE;
	}
	return FALSE;
	__LEAVE_FUNCTION
	return FALSE;
}

//
//VOID Obj_Human::Relive( )
//{
//__ENTER_FUNCTION
//
//	SetHP(GetMaxHP()/2) ;
//	SetMP(GetMaxMP()/2) ;
//
//
//	//GCNewPlayer Msg ;
//	//Msg.setObjID( GetID() ) ;
//	//Msg.setEquipVer( GetEquipVer() ) ;
//	//Msg.setWorldPos( *getWorldPos() ) ;
//
//	//getScene()->BroadCast( &Msg, this, TRUE ) ;
//	//m_bSendMsgDirty	= TRUE;
//
//	Obj_Character::Relive( ) ;
//
//	SendMsg_RefeshAttrib() ;
//
//__LEAVE_FUNCTION
//}


// 生活技能相关的函数

// 返回背包中 Item 类型的物品数量
/*
INT Obj_Human::HasItemCount(uint ItemSerial)
{
__ENTER_FUNCTION

	Assert(ItemSerial >0);

	INT count = 0;

	for(uint i=0; i<MAX_BAG_SIZE; ++i)
	{
		if( m_DB.GetBagItem(i)->m_ItemIndex == ItemSerial )
		{
			count += m_DB.GetBagItem(i)->GetItemCount();
		}
	}

	return count;
__LEAVE_FUNCTION
	return 0 ;
}*/


//是否携带物品
BOOL Obj_Human::IsCarrayItem(UINT ItemType,UINT ItemCount) 
{
	__ENTER_FUNCTION

		UINT count =0;

		count = HumanItemLogic::CalcEquipItemCount(this,ItemType);

		count += HumanItemLogic::CalcBagItemCount(this,ItemType);

		return count>= ItemCount;

	__LEAVE_FUNCTION
		return 0 ;
}


VOID Obj_Human::InterruptCurrentAbilityOpera()
{
__ENTER_FUNCTION
	Ability *pAbility = g_pAbilityManager->GetAbility(m_LastOpera.m_AbilityID);
	if (pAbility != NULL && GetCharacterLogic()==CHARACTER_LOGIC_USE_ABILITY)//modify by gh 
	{
		//pAbility->OnProcInterrupt(this);
		StopCharacterLogic(TRUE);
	}

	reset_AbilityOpera();

__LEAVE_FUNCTION
}
// add by gh 2010/07/12
VOID Obj_Human::CheckAllAbility()
{
	__ENTER_FUNCTION

		INT nAbilityLevel = 0;
	INT nAbilityExp = 0;
	INT nMaxLevel = 0;
	INT nMaxExp = 0;
	Ability *pAbility = NULL;
	const AbilityLevelUpConfig *pLevelUpConfig = NULL;

	const AbilityLevelUpConfigManager *pConfigMgr = g_pAbilityManager->GetLevelUpConfigManager();
	Assert(pConfigMgr);

	for ( INT i=0; i<MAX_CHAR_ABILITY_NUM; i++ )
	{
		nAbilityLevel = __GetAbilityLevel(i);
		nAbilityExp = __GetAbilityExp(i);

		// 如果玩家身上没有该生活技能则不进行检测....
		if ( nAbilityLevel <= 0 )
			continue;

		pAbility = g_pAbilityManager->GetAbility(i);
		if ( NULL == pAbility ) return;

		pLevelUpConfig = pConfigMgr->GetLevelUpConfig(pAbility->LevelUpConfigIndex());
		if ( NULL == pLevelUpConfig ) return;

		// 修正过高的等级....
		nMaxLevel = pLevelUpConfig->GetMaxLevel();
		if ( nMaxLevel > 0 && nAbilityLevel > nMaxLevel )
		{
			nAbilityLevel = nMaxLevel;
			__SetAbilityLevel( i, nAbilityLevel );
		}

		// 修正过高的熟练度....
		nMaxExp = pLevelUpConfig->GetLevelUpInfo(nAbilityLevel)->GetCurrentLevelAbilityExpTop();
		if ( nMaxExp >= 0 && nAbilityExp > nMaxExp )
		{
			nAbilityExp = nMaxExp;
			__SetAbilityExp( i, nAbilityExp );
		}

	}

	__LEAVE_FUNCTION
}
// end of add
ORESULT Obj_Human::Do_UseAbility( VOID )
{
__ENTER_FUNCTION

	AddLogicCount();

	GCAbilityAction Msg;
	Msg.setObjID( GetID() );
	Msg.setLogicCount( GetLogicCount() );
	Msg.setAction( m_LastOpera.m_AbilityID );
	Msg.setPrescriptionID( m_LastOpera.m_PresID );
	Msg.setTargetID( m_LastOpera.m_Obj );
	Msg.setBeginOrEnd(GCAbilityAction::ABILITY_BEGIN);
	getScene()->BroadCast( &Msg, this, TRUE );

	SetCharacterLogic( CHARACTER_LOGIC_USE_ABILITY );
	m_LastOpera.m_nCurTime = 0;
	return OR_OK;

__LEAVE_FUNCTION
	return OR_ERROR;
}

BOOL Obj_Human::Obj_Logic_UseAbility( UINT uTime )
{
__ENTER_FUNCTION

	m_LastOpera.m_nCurTime += (INT)(GetLogicTime());

	if( m_LastOpera.m_nCurTime >= m_LastOpera.m_nMaxTime )
	{
		Ability* pAbility;
		ORESULT res;
		GamePlayer *pGamePlayer = (GamePlayer*)(GetPlayer());

		Assert( pGamePlayer != NULL );

		pAbility = g_pAbilityManager->GetAbility(m_LastOpera.m_AbilityID);
		Assert( pAbility != NULL );
		//GetHumanAI()->PushCommand_Idle();

		//GCAbilityAction StopMsg;
		//StopMsg.setObjID( GetID() );
		//StopMsg.setLogicCount( GetLogicCount() );
		//StopMsg.setAction( m_LastOpera.m_AbilityID );
		//StopMsg.setTargetID( m_LastOpera.m_Obj );
		//StopMsg.setBeginOrEnd( GCAbilityAction::ABILITY_END );
		//getScene()->BroadCast( &StopMsg, this, TRUE );

		//m_bSendMsgDirty	= TRUE;

		// reset_AbilityOpera(); // 这里不重设生活技能数据，以免今后需要批量合成物品，但是在用户指令发出的时候进行重设
		StopCharacterLogic(FALSE);

		res = pAbility->OnProcOver(this);

		// 将 res 形成消息发送出去
		if( res != OR_OK )
		{
			GCAbilityResult Msg;
			Msg.SetAbilityID( m_LastOpera.m_AbilityID );
			Msg.SetPrescriptionID( m_LastOpera.m_PresID );
			Msg.setResult( res );
			pGamePlayer->SendPacket( &Msg );
		}

	}
	else
	{ // 做一些合法性验证
		Ability* pAbility;

		pAbility = g_pAbilityManager->GetAbility(m_LastOpera.m_AbilityID);
		Assert( pAbility != NULL );

		if( pAbility->OperationToolID()!=INVALID_ID
		 && HumanItemLogic::CalcEquipItemCount(this, pAbility->OperationToolID())<1
			&& HumanItemLogic::CalcBagItemCount(this, pAbility->OperationToolID())<1
		 )
		{
			pAbility->OnProcInterrupt(this);
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID Obj_Human::OnCharacterLogicStopped(BOOL bAbortive)
{
__ENTER_FUNCTION

	ENUM_CHARACTER_LOGIC eLogic = GetCharacterLogic();
	if(eLogic == CHARACTER_LOGIC_MOVE)
	{
		m_DB.SetDBPosition(*getWorldPos());
	}

	switch(eLogic)
	{
	case CHARACTER_LOGIC_USE_ABILITY:
		{
			if(bAbortive)
			{
				Ability *pAbility = g_pAbilityManager->GetAbility(m_LastOpera.m_AbilityID);
				Assert( pAbility != NULL && "Obj_Human::OnCharacterLogicStopped");
				if(pAbility != NULL)
				{
					//pAbility->OnProcInterrupt(this);
				}
			}
			//// 不管是强行的还是自然停的都要求发停止
			//GCAbilityAction StopMsg;
			//StopMsg.setObjID( GetID() );
			//StopMsg.setLogicCount( GetLogicCount() );
			//StopMsg.setAction( m_LastOpera.m_AbilityID );
			//StopMsg.setPrescriptionID( m_LastOpera.m_PresID );
			//StopMsg.setTargetID( m_LastOpera.m_Obj );
			//StopMsg.setBeginOrEnd( GCAbilityAction::ABILITY_END );
			//getScene()->BroadCast( &StopMsg, this, TRUE );

			//if(bAbortive)
			{
				GCCharStopAction	msgStopAction;
				msgStopAction.setObjID( GetID() );
				msgStopAction.setLogicCount( GetLogicCount() );
				msgStopAction.setStopTime( (UINT)(m_LastOpera.m_nCurTime) );

				getScene()->BroadCast( &msgStopAction, this, TRUE ) ;
			}
		}
		break;
	default:
		Obj_Character::OnCharacterLogicStopped(bAbortive);
		break;
	}

__LEAVE_FUNCTION
}

// 生活技能相关的函数

VOID Obj_Human::OnKillObject( ObjID_t idObj )
{
__ENTER_FUNCTION
	Obj_Character *pObj = (Obj_Character*)(getScene()->GetObjManager()->GetObj( idObj ));
	if ( pObj == NULL )
		return ;
	m_iTargetForExp = idObj;
	
	if(pObj->GetObjType( ) != Obj::OBJ_TYPE_MONSTER)
		return;

	//if ( !IsCharacterObj( pObj->GetObjType() ) )
	//	return ;
	Obj_Monster* pMonster = (Obj_Monster*)pObj;

	UINT i,uDataID;
	uDataID = pObj->GetDataID();
	for(i=0;i<MAX_CHAR_MISSION_NUM;i++)
	{
		//保证任务脚本只执行一次,以后是否需要挪到更合适的地方?
		const _OWN_MISSION* pMission =	GetMission(i) ;
		if( !pMission->IsActiveMission() || !pMission->IsFlags_KillObject() )
			continue ;

		INT nMission = pMission->m_idMission;

		BOOL bToggle = FALSE;	
		for(INT j=0;j<pMonster->m_nKillCount;j++)
		{
			if(pMonster->m_sKillObj[j].m_nMissionId == nMission)
			{
				bToggle = TRUE;
				break;
			}
		}

		if(!bToggle)
		{
			if(pMonster->m_nKillCount < Obj_Monster::BEKILL_MISSION_NUM)
			{
				pMonster->m_sKillObj[pMonster->m_nKillCount++].m_nMissionId = nMission;
				getScene()->GetLuaInterface()->ExeScript_DDDD( 
					pMission->m_idScript,
					"OnKillObject",
					(INT)getScene()->SceneID(),
					(INT)GetID(),
					(INT)uDataID,
					(INT)idObj );
			}
			else
			{
				AssertEx(pMonster->m_nKillCount<Obj_Monster::BEKILL_MISSION_NUM,"overflow exception");
				pMonster->m_nKillCount--;
				break;
			}	
		}
	}
__LEAVE_FUNCTION
}

VOID Obj_Human::OnItemChanged( UINT uItemDataID )
{
__ENTER_FUNCTION

	UINT i;
	for(i=0;i<MAX_CHAR_MISSION_NUM;i++)
	{	
		const _OWN_MISSION* pMission = GetMission(i) ;
		if( !pMission->IsActiveMission() || !pMission->IsFlags_ItemChanged() )
			continue ;

		getScene()->GetLuaInterface()->ExeScript_DDD( 
			pMission->m_idScript, 
			"OnItemChanged",
			(INT)getScene()->SceneID(),
			(INT)GetID(),
			(INT)uItemDataID );
	}

__LEAVE_FUNCTION
}

VOID Obj_Human::OnPetChanged( UINT uPetDataID )
{
__ENTER_FUNCTION

	UINT i;
	for(i=0;i<MAX_CHAR_MISSION_NUM;i++)
	{	
		const _OWN_MISSION* pMission = GetMission(i) ;
		if( !pMission->IsActiveMission() || !pMission->IsFlags_PetChanged() )
			continue ;

		getScene()->GetLuaInterface()->ExeScript_DDD( 
			pMission->m_idScript, 
			"OnPetChanged",
			(INT)getScene()->SceneID(),
			(INT)GetID(),
			(INT)uPetDataID );
	}

__LEAVE_FUNCTION
}

VOID Obj_Human::OnDie( ObjID_t idKiller )
{
__ENTER_FUNCTION

	ResetCallOfInfo();
	Scene *pScene = getScene();
	if(pScene != NULL)
	{
		// 在这调用脚本，进行死亡惩罚
		//pScene->OnSceneHumanDie(this, idKiller);
	}

	SetMountID( INVALID_ID );
	PET_GUID_t oPetGUID;
	SetGUIDOfCallUpHorse( oPetGUID );
	Obj_Character::OnDie(idKiller);

	if ( __IsTeamLeader() == TRUE )
	{//是队长，需要通知GL切换队长
		GWTeamLeaderDie *pMsg = (GWTeamLeaderDie*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAM_LEADER_DIE);
		pMsg->SetSourGUID( GetGUID() );
		g_pServerManager->SendPacket(pMsg,INVALID_ID);
	}

	// 如果是被老手带着的新手，则扣除老手的善恶值
	if( GetLevel() <= g_Config.m_ConfigInfo.m_nMemberLevel )
	{
		//if( GetTeamInfo()->HasTeam() && GetTeamInfo()->IsLeader() != TRUE )
		//{ // 组队非队长
		//	Obj_Human* pTeamLeader = NULL;

		//	if( GetTeamInfo()->Leader()->m_SceneID != getScene()->SceneID() )
		//	{
		//		return;
		//	}

		//	pTeamLeader = getScene()->GetHumanManager()->GetHuman(GetTeamInfo()->Leader()->m_ObjID);
		//	if( pTeamLeader == NULL )
		//	{
		//		Assert(FALSE);
		//		return;
		//	}

		//	if( pTeamLeader->GetLevel() >= g_Config.m_ConfigInfo.m_nLevelNeeded
		//	 && IsInValidRadius( pTeamLeader, g_Config.m_ConfigInfo.m_fGoodBadRadius )
		//	 )
		//	{
		//		INT nPenalty;

		//		nPenalty = g_Config.m_ConfigInfo.m_nPenaltyWhenMemberDie;
		//		nPenalty = pTeamLeader->DecGoodBadValue( nPenalty );
		//		// nPenalty 是实际扣除值
		//		// 发送消息给客户端，以显示信息提示
		//		GCNotifyGoodBad Msg;
		//		Msg.SetNotifyMode( NOTIFY_GOODBAD_NEWBIEDIE );
		//		Msg.SetValue( nPenalty );

		//		pTeamLeader->GetPlayer()->SendPacket( &Msg );
		//	}
		//}
	}

__LEAVE_FUNCTION
}

VOID Obj_Human::OnEnterArea( UINT uAreaID )
{
__ENTER_FUNCTION

	UINT i;

	//一个区域可被数个任务响应,需要和策划商榷
	for(i=0;i<MAX_CHAR_MISSION_NUM;i++)
	{
		const _OWN_MISSION* pMission = GetMission(i) ;
		if( !pMission->IsActiveMission() || !pMission->IsFlags_EnterArea() )
			continue ;

		getScene()->GetLuaInterface()->ExeScript_DDD( 
			pMission->m_idScript, 
			DEF_ON_ENTER_AREA_FUNC_NAME,
			(INT)getScene()->SceneID(),
			(INT)GetID(),
			(INT)uAreaID );
	}

__LEAVE_FUNCTION
}

Packet *Obj_Human::CreateNewObjPacket( VOID )
{
__ENTER_FUNCTION

	if ( getScene() != NULL )
	{
		if( !IsAlive() )
		{//尸体
			GCNewPlayer_Death *pPacket = (GCNewPlayer_Death*)(getScene()->m_pPacket_NewPlayer_Death);
			pPacket->setObjID( GetID() );
			pPacket->setWorldPos( *(getWorldPos()) );
			pPacket->setDir( getDir() );
			pPacket->setEquipVer( GetEquipVer() );
			pPacket->setMoveSpeed( GetMoveSpeed() ) ;
			return pPacket;
		}
		else if( IsMoving() )
		{
			GCNewPlayer_Move *pPacket = (GCNewPlayer_Move*)(getScene()->m_pPacket_NewPlayer_Move);
			pPacket->setObjID( GetID() ) ;
			pPacket->setHandleID( GetLogicCount() ) ;
			pPacket->setWorldPos( *getWorldPos() ) ;
			pPacket->setTargetPos( *GetFinalTarPos() ) ;
			pPacket->setEquipVer( GetEquipVer() ) ;
			pPacket->setMoveSpeed( GetMoveSpeed() ) ;
			return pPacket;
		}
		else
		{
			GCNewPlayer *pPacket = (GCNewPlayer*)(getScene()->m_pPacket_NewPlayer);
			pPacket->setObjID( GetID() ) ;
			pPacket->setWorldPos( *(getWorldPos()) ) ;
			pPacket->setDir( getDir() );
			pPacket->setEquipVer( GetEquipVer() ) ;
			pPacket->setMoveSpeed( GetMoveSpeed() ) ;
			return pPacket;
		}
	}
	else
	{
		Assert( getScene() != NULL && "Obj_Human::CreateNewObjPacket" );
		return NULL;
	}

__LEAVE_FUNCTION
	return NULL ;
}

VOID Obj_Human::DestroyNewObjPacket( Packet *pPacket )
{
}

VOID Obj_Human::ReInitExtraBag()
{
	__ENTER_FUNCTION

	//杂项物品扩展包1
	_HUMAN_ITEMEXTRABAG_INIT	ExtraBagInit;
	ExtraBagInit.m_nContainerSize	=	m_DB.GetExtraBagSize(BAG_EXTRA1);
	ExtraBagInit.m_pHumanDB			=	&m_DB;
	ExtraBagInit.m_ICTType			=	ICT_EXTRABAG_CONTAINER;
	ExtraBagInit.m_DBOffSet			=	EXTRA_BAG1_OFFSET;
	ExtraBagInit.m_pExtraBagItem	=   m_ExtraContainerItem.GetItem(BAG_EXTRA1);

	m_ExtraBagItem[BAG_EXTRA1].Init(&ExtraBagInit);

	for(UINT i=0;i<m_DB.GetExtraBagSize(BAG_EXTRA1);i++)
	{
		_ITEM_INIT	itemInit(m_DB.GetBagItem(EXTRA_BAG1_OFFSET+i));
		m_ExtraBagItem[BAG_EXTRA1].GetItem(i)->Init(&itemInit);
	}

	//杂项物品扩展包2
	ExtraBagInit.m_nContainerSize	=	m_DB.GetExtraBagSize(BAG_EXTRA2);
	ExtraBagInit.m_pHumanDB			=	&m_DB;
	ExtraBagInit.m_ICTType			=	ICT_EXTRABAG_CONTAINER;
	ExtraBagInit.m_DBOffSet			=	EXTRA_BAG2_OFFSET;
	ExtraBagInit.m_pExtraBagItem	=   m_ExtraContainerItem.GetItem(BAG_EXTRA2);

	m_ExtraBagItem[BAG_EXTRA2].Init(&ExtraBagInit);

	for(UINT i=0;i<m_DB.GetExtraBagSize(BAG_EXTRA2);i++)
	{
		_ITEM_INIT	itemInit(m_DB.GetBagItem(EXTRA_BAG2_OFFSET+i));
		m_ExtraBagItem[BAG_EXTRA2].GetItem(i)->Init(&itemInit);
	}

	//杂项物品扩展包3
	ExtraBagInit.m_nContainerSize	=	m_DB.GetExtraBagSize(BAG_EXTRA3);
	ExtraBagInit.m_pHumanDB			=	&m_DB;
	ExtraBagInit.m_ICTType			=	ICT_EXTRABAG_CONTAINER;
	ExtraBagInit.m_DBOffSet			=	EXTRA_BAG3_OFFSET;
	ExtraBagInit.m_pExtraBagItem	=   m_ExtraContainerItem.GetItem(BAG_EXTRA3);

	m_ExtraBagItem[BAG_EXTRA3].Init(&ExtraBagInit);

	for(UINT i=0;i<m_DB.GetExtraBagSize(BAG_EXTRA3);i++)
	{
		_ITEM_INIT	itemInit(m_DB.GetBagItem(EXTRA_BAG3_OFFSET+i));
		m_ExtraBagItem[BAG_EXTRA3].GetItem(i)->Init(&itemInit);
	}

	//杂项物品扩展包4
	ExtraBagInit.m_nContainerSize	=	m_DB.GetExtraBagSize(BAG_EXTRA4);
	ExtraBagInit.m_pHumanDB			=	&m_DB;
	ExtraBagInit.m_ICTType			=	ICT_EXTRABAG_CONTAINER;
	ExtraBagInit.m_DBOffSet			=	EXTRA_BAG4_OFFSET;
	ExtraBagInit.m_pExtraBagItem	=   m_ExtraContainerItem.GetItem(BAG_EXTRA4);

	m_ExtraBagItem[BAG_EXTRA4].Init(&ExtraBagInit);

	for(UINT i=0;i<m_DB.GetExtraBagSize(BAG_EXTRA4);i++)
	{
		_ITEM_INIT	itemInit(m_DB.GetBagItem(EXTRA_BAG4_OFFSET+i));
		m_ExtraBagItem[BAG_EXTRA4].GetItem(i)->Init(&itemInit);
	}

	__LEAVE_FUNCTION
}

VOID Obj_Human::OnMissionHaveDoneFlagsChanged( MissionID_t idMission )
{
__ENTER_FUNCTION
	m_AttrBackUp.m_bMissionHaveDoneFlagsChanged	= TRUE;
	GamePlayer *pGamePlayer = (GamePlayer*)(GetPlayer());
	Assert( pGamePlayer != NULL && "Obj_Human::OnMissionHaveDoneFlagsChanged" );
	if ( pGamePlayer != NULL )
	{
		GCMissionHaveDoneFlag msgMissionHaveDoneFlags;
		msgMissionHaveDoneFlags.Init(idMission, TRUE);
		pGamePlayer->SendPacket( &msgMissionHaveDoneFlags );
	}
__LEAVE_FUNCTION
}

VOID Obj_Human::OnMissionParamChanged( UINT uIndexMission, UINT uIndexParam ,int index)
{
__ENTER_FUNCTION
	GamePlayer *pGamePlayer = (GamePlayer*)(GetPlayer());
	Assert( pGamePlayer != NULL && "Obj_Human::OnMissionParamChanged" );
	if ( pGamePlayer != NULL )
	{	
		GCMissionModify msgMissionModify;
		msgMissionModify.SetFlag(index) ;
		switch(index)
		{
		case 0:
			{
				const _OWN_MISSION* pMission = &(GetConstMissionList()->m_aMission[uIndexMission]) ;
				msgMissionModify.SetMission( pMission );
				pGamePlayer->SendPacket( &msgMissionModify );

				MISSION_LOG_PARAM sParam ;
				sParam.CharGUID = GetGUID();
				sParam.OPType	= MISSION_OP_PARAM ;
				sParam.SceneID	= getScene()->SceneID() ;
				sParam.XPos		= getWorldPos()->m_fX ;
				sParam.ZPos		= getWorldPos()->m_fZ ;
				sParam.idMission= pMission->m_idMission ;
				sParam.idScript = pMission->m_idScript ;
				sParam.yFlags	= pMission->m_yFlags ;
				memcpy( sParam.aParam, pMission->m_aParam, sizeof(UINT)*MAX_MISSION_PARAM_NUM ) ;
				SaveMissionLog(&sParam) ;
			}
			break;
		case 1:
			{
				msgMissionModify.SetMissionData( (GetConstMissionList()->m_aMissionData) );
				pGamePlayer->SendPacket( &msgMissionModify );

				MISSION_LOG_PARAM sParam ;
				sParam.CharGUID = GetGUID();
				sParam.OPType	= MISSION_OP_DATA ;
				sParam.SceneID	= getScene()->SceneID() ;
				sParam.XPos		= getWorldPos()->m_fX ;
				sParam.ZPos		= getWorldPos()->m_fZ ;
				sParam.nDataIndex	  = uIndexMission ;
				sParam.nDoneFlagValue = uIndexParam ;
				SaveMissionLog(&sParam) ;
			}
			break;
		}
	}
__LEAVE_FUNCTION
}

VOID Obj_Human::OnAddMission( UINT uIndex )
{
__ENTER_FUNCTION
	GamePlayer *pGamePlayer = (GamePlayer*)(GetPlayer());
	Assert( pGamePlayer != NULL && "Obj_Human::OnAddMission" );
	const _OWN_MISSION* pMission = &(GetConstMissionList()->m_aMission[uIndex]) ;
	if ( pGamePlayer != NULL )
	{
		GCMissionAdd msgMissionAdd;
		msgMissionAdd.SetMission( pMission );
		pGamePlayer->SendPacket( &msgMissionAdd );
	}
	MISSION_LOG_PARAM sParam ;
	sParam.CharGUID = GetGUID();
	sParam.OPType	= MISSION_OP_ADDMISSION ;
	sParam.SceneID	= getScene()->SceneID() ;
	sParam.XPos		= getWorldPos()->m_fX ;
	sParam.ZPos		= getWorldPos()->m_fZ ;
	sParam.idMission= pMission->m_idMission ;
	sParam.idScript = pMission->m_idScript ;
	sParam.yFlags	= pMission->m_yFlags ;
	memcpy( sParam.aParam, pMission->m_aParam, sizeof(UINT)*MAX_MISSION_PARAM_NUM ) ;
	SaveMissionLog(&sParam) ;

__LEAVE_FUNCTION
}

VOID Obj_Human::OnRemoveMission( MissionID_t idMission )
{
__ENTER_FUNCTION
	GamePlayer *pGamePlayer = (GamePlayer*)(GetPlayer());
	Assert( pGamePlayer != NULL && "Obj_Human::OnRemoveMission" );
	if ( pGamePlayer != NULL )
	{
		GCMissionRemove msgMissionRemove;
		msgMissionRemove.SetMissionID( idMission );
		pGamePlayer->SendPacket( &msgMissionRemove );
	}
	MISSION_LOG_PARAM sParam ;
	sParam.CharGUID = GetGUID();
	sParam.OPType	= MISSION_OP_DELMISSION ;
	sParam.SceneID	= getScene()->SceneID() ;
	sParam.XPos		= getWorldPos()->m_fX ;
	sParam.ZPos		= getWorldPos()->m_fZ ;
	sParam.idMission= idMission ;
	SaveMissionLog(&sParam) ;
__LEAVE_FUNCTION
}

//VOID Obj_Human::OnAddCanPickMissionItem( UINT uItemDataID )
//{
//__ENTER_FUNCTION
//	GamePlayer *pGamePlayer = (GamePlayer*)(GetPlayer());
//	Assert( pGamePlayer != NULL && "Obj_Human::OnAddCanPickMissionItem" );
//	if ( pGamePlayer != NULL )
//	{
//		GCAddCanPickMissionItem msg;
//		msg.SetItemDataID(uItemDataID );
//		pGamePlayer->SendPacket( &msg );
//	}
//__LEAVE_FUNCTION
//}
//
//VOID Obj_Human::OnRemoveCanPickMissionItem( UINT uItemDataID )
//{
//__ENTER_FUNCTION
//	GamePlayer *pGamePlayer = (GamePlayer*)(GetPlayer());
//	Assert( pGamePlayer != NULL && "Obj_Human::OnRemoveCanPickMissionItem" );
//	if ( pGamePlayer != NULL )
//	{
//		GCRemoveCanPickMissionItem msg;
//		msg.SetItemDataID(uItemDataID );
//		pGamePlayer->SendPacket( &msg );
//	}
//__LEAVE_FUNCTION
//}

const _MISSION_DB_LOAD *Obj_Human::GetMissionList( VOID )
{
	return GetDB()->GetMissionDB();
}

const _SETTING_DB_LOAD *Obj_Human::GetSettingList( VOID )
{
	return GetDB()->GetSettingDB();
}

const _MISSION_DB_LOAD *Obj_Human::GetConstMissionList( VOID )const
{
	return m_DB.GetMissionDB();
}

const _SETTING_DB_LOAD *Obj_Human::GetConstSettingList( VOID )const
{
	return m_DB.GetSettingDB();
}

ORESULT Obj_Human::AddMission( MissionID_t idMission, ScriptID_t idScript, BOOL bKillObjectEvent/* = TRUE*/, BOOL bEnterAreaEvent/* = TRUE*/, BOOL bItemChangedEvent/* = TRUE*/ )
{
__ENTER_FUNCTION
	if ( GetConstMissionList()->m_Count >= MAX_CHAR_MISSION_NUM )
		return OR_MISSION_LIST_FULL;

	UINT uIndex = GetMissionIndexByID( idMission );
	if (uIndex != UINT_MAX )
		return OR_MISSION_HAVE;

	uIndex = UINT_MAX;
	UINT i;
	for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	{
		if ( GetConstMissionList()->m_aMission[i].m_idMission == INVALID_ID )
		{
			uIndex = i;
			break;
		}
	}

	if (uIndex == UINT_MAX )
		return OR_ERROR;

	const _OWN_MISSION *pMission=GetMission(uIndex);

	m_DB.SetMission(uIndex,idMission,idScript);
	
	m_DB.SetFlags_KillObject( uIndex,bKillObjectEvent );
	m_DB.SetFlags_EnterArea( uIndex,bEnterAreaEvent );
	m_DB.SetFlags_ItemChanged( uIndex,bItemChangedEvent );

	m_DB.SetMissionCount(GetMissionList()->m_Count+1);

	OnAddMission(uIndex );

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT Obj_Human::AddMissionEx( MissionID_t idMission, ScriptID_t idScript )
{
__ENTER_FUNCTION
	if ( GetConstMissionList()->m_Count >= MAX_CHAR_MISSION_NUM )
		return OR_MISSION_LIST_FULL;

	UINT uIndex = GetMissionIndexByID( idMission );
	if (uIndex != UINT_MAX )
		return OR_MISSION_HAVE;

	uIndex = UINT_MAX;
	UINT i;
	for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	{
		if ( GetConstMissionList()->m_aMission[i].m_idMission == INVALID_ID )
		{
			uIndex = i;
			break;
		}
	}

	if (uIndex == UINT_MAX )
		return OR_ERROR;

	const _OWN_MISSION *pMission=GetMission(uIndex);

	m_DB.SetMission(uIndex,idMission,idScript);
	
	m_DB.SetMissionCount(GetMissionList()->m_Count+1);

	OnAddMission(uIndex );

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

ORESULT Obj_Human::SetMissionEvent( MissionID_t idMission, INT EventID/*TASK_EVENT*/ )
{
__ENTER_FUNCTION

	UINT uIndex = GetMissionIndexByID( idMission );
	if (uIndex == UINT_MAX )
		return OR_ERROR;

	const _OWN_MISSION *pMission=GetMission(uIndex);
	if( pMission==NULL )
		return OR_ERROR;

	switch( EventID )
	{
	case TASK_EVENT_KILLOBJ:
		m_DB.SetFlags_KillObject( uIndex, TRUE );
		break ;
	case TASK_EVENT_ENTERAREA:
		m_DB.SetFlags_EnterArea( uIndex, TRUE );
		break ;
	case TASK_EVENT_ITEMCHANGED:
		m_DB.SetFlags_ItemChanged( uIndex, TRUE );
		break ;
	case TASK_EVENT_PETCHANGED:
		m_DB.SetFlags_PetChanged( uIndex, TRUE );
		break ;
	default :
		Assert(FALSE) ;
		break ;
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}


VOID Obj_Human::DelMission( MissionID_t idMission )
{
__ENTER_FUNCTION
	UINT uIndex = GetMissionIndexByID( idMission );
	if (uIndex == UINT_MAX )
		return ;

	DelMissionByIndex(uIndex );
__LEAVE_FUNCTION
}

VOID Obj_Human::DelMissionByIndex( UINT uIndex )
{
__ENTER_FUNCTION

	if (uIndex < MAX_CHAR_MISSION_NUM && GetConstMissionList()->m_aMission[uIndex].m_idMission != INVALID_ID )
	{
		const _OWN_MISSION *pMission = GetMission(uIndex);

		OnRemoveMission( pMission->m_idMission );

		m_DB.ClearMission(uIndex);

	}

__LEAVE_FUNCTION
}

UINT Obj_Human::GetMissionIndexByID( MissionID_t idMission )const
{
__ENTER_FUNCTION
	UINT i;
	for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	{
		if ( GetConstMissionList()->m_aMission[i].m_idMission == idMission )
		{
			return i;
		}
	}
	return UINT_MAX;
__LEAVE_FUNCTION
	return UINT_MAX;
}


VOID Obj_Human::AddTimer(INT sceneTimerIndex)		
{
__ENTER_FUNCTION

	for(INT i=0; i<g_Config.m_ConfigInfo.m_nMaxHumanUseTimer; i++)
	{
		if(m_pUseTimer[i] == -1)
		{
			m_pUseTimer[i] = sceneTimerIndex;	

			m_nUseTimer++; 
			break;
		}	
	}	
__LEAVE_FUNCTION
}

VOID Obj_Human::RemoveTimer(INT sceneTimerIndex)	
{
__ENTER_FUNCTION

	for(INT i=0; i<g_Config.m_ConfigInfo.m_nMaxHumanUseTimer; i++)
	{
		if(m_pUseTimer[i] == sceneTimerIndex)
		{
			m_pUseTimer[i] = sceneTimerIndex;	

			m_nUseTimer--;	
		}
	}
__LEAVE_FUNCTION
}

INT Obj_Human::GetUseTimerCount()	
{
	return m_nUseTimer;		
}

INT Obj_Human::GetMaxTimerCount()	
{		
	return g_Config.m_ConfigInfo.m_nMaxHumanUseTimer;	
}

BOOL Obj_Human::HaveTimer(INT sceneTimerIndex)
{
__ENTER_FUNCTION

	Assert(sceneTimerIndex>=0 && sceneTimerIndex < g_Config.m_ConfigInfo.m_MaxTimerCount);
	if(sceneTimerIndex>=0 && sceneTimerIndex < g_Config.m_ConfigInfo.m_MaxTimerCount)
	{
		for(INT i=0; i<g_Config.m_ConfigInfo.m_nMaxHumanUseTimer; i++)
		{
			if(m_pUseTimer[i] == sceneTimerIndex)
				return TRUE;
		}

	}

__LEAVE_FUNCTION

	return FALSE;
}

VOID Obj_Human::SetPortraitID( INT nID )
{
	m_DB.SetPortraitID(nID); 
}

INT Obj_Human::GetPortraitID( VOID ) const
{
	return m_DB.GetPortraitID(); 
}

VOID Obj_Human::SetName( const CHAR *pszName )
{
}

VOID Obj_Human::SetTitle( const CHAR *pszTitle )
{
__ENTER_FUNCTION

    //Fixme: Amos  同步GLServer
	//if(tstricmp(pszTitle, m_DB.GetCurTitle()) != 0)
	//{
	//	m_AttrBackUp.m_bTitleModified = TRUE;
	//	m_DB.SetCurTitle(pszTitle, (BYTE)strlen(pszTitle)); 
	//}

	//CHAR szWorldTitle[MAX_CHARACTER_TITLE] = {0};

	//if( GetTitleType() == _TITLE::MOOD_TITLE )
	//{
	//	strncpy(szWorldTitle, "", sizeof(szWorldTitle));
	//}
	//else
	//{
	//	strncpy(szWorldTitle, GetTitle(), sizeof(szWorldTitle));
	//}

	GWUpdateTitle* pMsg = (GWUpdateTitle*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_UPDATETITLE);
	pMsg->SetGUID( GetGUID() );
	//pMsg->SetTitle( szWorldTitle );

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

__LEAVE_FUNCTION
}

VOID Obj_Human::SetTitleType( BYTE	nType)
{
	__ENTER_FUNCTION
    //Fixme: Amos  
	//m_AttrBackUp.m_bTitleModified = TRUE;
	//m_DB.SetCurTitleType(nType); 
	__LEAVE_FUNCTION
}

VOID Obj_Human::SetHP( INT nHP )
{
	INT nMaxHP = GetMaxHP();
	if( nHP > nMaxHP )
	{
		nHP = nMaxHP;
	}
	m_DB.SetHP( nHP );
}

VOID Obj_Human::SetMP( INT nMP )
{
	INT nMaxMP = GetMaxMP();
	if( nMP > nMaxMP )
	{
		nMP = nMaxMP;
	}
	m_DB.SetMP( nMP );
}

VOID Obj_Human::SetPneuma( INT nPneuma)
{
	INT nMaxPneuma = GetMaxPneuma();
	if( nPneuma > nMaxPneuma )
	{
		nPneuma = nMaxPneuma;
	}
	m_DB.SetPneuma( nPneuma );
}

VOID Obj_Human::SetLevel( INT nLevel )
{
	if(nLevel != m_DB.GetLevel())
	{
		m_DB.SetLevel( nLevel );
		OnLevelChanged();
	}
}

VOID Obj_Human::SetPKValue( INT nValue )
{
	m_DB.SetPKValue( nValue );
}

VOID Obj_Human::SetCurrentHorseGUID( PET_GUID_t guid )
{
	m_DB.SetCurrentPetGUID( guid );
}

VOID Obj_Human::InitialSoldList()
{
__ENTER_FUNCTION

	memset(m_ItemHasBeenSold, 0,MAX_BOOTH_SOLD_NUMBER*sizeof(_ITEM));
	memset(m_SoldPrice, 0,MAX_BOOTH_SOLD_NUMBER*sizeof(UINT));

	m_nCurSold = 0;
	
	//初始化container
	_ITEMCONTAINER_INIT	containInit;
	containInit.m_nContainerSize	=	MAX_BOOTH_SOLD_NUMBER;
	m_SoldContainer.Init(&containInit);

	for(INT i=0;i<MAX_BOOTH_SOLD_NUMBER;i++)
	{
		_ITEM_INIT	itemInit(&m_ItemHasBeenSold[i]);
		m_SoldContainer.GetItem(i)->Init(&itemInit);
	}

__LEAVE_FUNCTION
}

VOID	Obj_Human::AddToSoldList(INT index, UINT uPrice, UINT uCount)
{
__ENTER_FUNCTION

    INT result = 0;
    UINT nCount= 0;

    for (INT i=0; i < MAX_BOOTH_SOLD_NUMBER; i++)
    {
        if (m_SoldContainer.GetItem(i)->IsEmpty())
        {
            ItemContainer*	pBagContainer = HumanItemLogic::GetBagContainer(this, index);
			Assert(pBagContainer);
            Item* pItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(index));
			Assert(pItem);
            if(!pItem->IsEmpty())
            {
				
				// 20100413 ModifyCodeBegin 发现在卖出背包物品时，是通过物品类型来判断能否拆分卖出的，
				// 现在需要改为根据item_formula.tab设置的物品类型是否允许堆叠的字段来判断是只能全部卖出还是允许拆分卖出
				if( pItem->IsRuler( IRL_TILE ) == TRUE )
                {
                    nCount = pItem->GetLayedNum();
                }
				// 20100413 ModifyCodeEnd 
            }

            //卖出数量小于该位置物品堆叠树  拆分处理
            if (uCount < nCount)
            {
                HumanItemLogic::SplitItem
                    (this, 
                    pBagContainer,
                    pBagContainer->BagIndex2ConIndex(index),
                    uCount, &m_SoldContainer, i, GCItemInfo::OPERATOR_BUY_BACK);
            }
            else//直接移动
            {
                result = 
                    g_ItemOperator.MoveItem
                    (
                    pBagContainer,
                    pBagContainer->BagIndex2ConIndex(index),
                    &m_SoldContainer,
                    i
                    );

                if(result>=0)
                {
                    m_SoldPrice[i] = uPrice;
                    GCItemInfo  Msg;
                    Msg.setIsNull(TRUE);
                    Msg.setID(index);
                    m_pPlayer->SendPacket(&Msg);
                }
                else
                {
                    Assert(FALSE);
                }
            }
            return;
        }
    }

    //回购列表满了，删除最早卖出的物品，所有物品位置前移
    g_ItemOperator.EraseItem(&m_SoldContainer, 0);//清空第一个格子
	INT i=1;
    for( i=1; i < MAX_BOOTH_SOLD_NUMBER; i++)
    {
        g_ItemOperator.MoveItem(&m_SoldContainer, i, &m_SoldContainer, i-1);
        m_SoldPrice[i-1] = m_SoldPrice[i];
    }

    //新卖出的物品添加到回购列表尾部
    ItemContainer*	pBagContainer = HumanItemLogic::GetBagContainer(this, index);

    Item* pItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(index));
    if(!pItem->IsEmpty())
    {
        if(pItem->GetItemClass() == ICLASS_COMITEM || pItem->GetItemClass() == ICLASS_IDENT)
        {
            nCount = pItem->GetLayedNum();
        }
    }

    //卖出数量小于该位置物品堆叠树  拆分处理
    if (uCount < nCount)
    {
            HumanItemLogic::SplitItem
            (this, 
            pBagContainer,
            pBagContainer->BagIndex2ConIndex(index),
            uCount, &m_SoldContainer, i, GCItemInfo::OPERATOR_BUY_BACK);
    }
    else//直接移动
    {
        result = 
            g_ItemOperator.MoveItem
            (
            pBagContainer,
            pBagContainer->BagIndex2ConIndex(index),
            &m_SoldContainer,
            MAX_BOOTH_SOLD_NUMBER-1
            );

        if(result>=0)
        {
            m_SoldPrice[MAX_BOOTH_SOLD_NUMBER-1] = uPrice;
            GCItemInfo  Msg;
            Msg.setIsNull(TRUE);
            Msg.setID(index);
            m_pPlayer->SendPacket(&Msg);
        }
        else
        {
            Assert(FALSE);
        }
    }

__LEAVE_FUNCTION
}

// 20100413 AddCodeBegin
VOID	Obj_Human::ReorderSoldList()	// 重新整理回购物品列表，去除中间的空物品格
{
	INT nIndexFull;		// 找到的非空物品格，用于挪到空格上
	INT nMaxListSearch;	// 遍历回购物品列表的最大个数

	nIndexFull       = 1;	// 初始时设置开始找到的非空物品格为第二格
	nMaxListSearch   = MAX_BOOTH_SOLD_NUMBER -1; 

	if ( m_SoldContainer.CalcItemSpace() != 0 && !m_SoldContainer.IsEmpty() ) // 如果回购队列有空格且不全空的话才计算
	{
		for ( INT i=0; i< nMaxListSearch; ++i ) // 从回购物品列表的第一个格开始找
		{
			if ( m_SoldContainer.GetItem(i)->IsEmpty() )  // 找到空格
			{
				while( nIndexFull <= MAX_BOOTH_SOLD_NUMBER  ) // 从空格后开始找，直到找到一个不为空的格
				{
					if ( m_SoldContainer.GetItem( nIndexFull ) != NULL )
					{
						break;
					}

					nIndexFull++;
				}

				if ( nIndexFull != (MAX_BOOTH_SOLD_NUMBER + 1) ) // 如果保存的不为空物品格序号是有效序号
				{
					m_SoldPrice[i] = m_SoldPrice[nIndexFull]; // 将价格也在价格数组里移动到对应位置
					m_SoldPrice[nIndexFull] = 0;

					g_ItemOperator.MoveItem( &m_SoldContainer, nIndexFull, &m_SoldContainer, i ); // 移动不空格到前面的空格
					g_ItemOperator.EraseItem( &m_SoldContainer, nIndexFull ); // 移动后删除上步找到的不空格				
					
				}
			}

			nIndexFull++;
		}
	}
}
// 20100413 AddCodeEnd

VOID    Obj_Human::GetSlodListInfo(GCShopSoldList::_MERCHANDISE_ITEM *pItem)
{
    __ENTER_FUNCTION	

    for (INT i=0; i< MAX_BOOTH_SOLD_NUMBER; ++i)
    {
        m_SoldContainer.GetItem(i)->SaveValueTo(&pItem->item_data);
        pItem->iIndex = i;
        pItem->iPrice = m_SoldPrice[i];
        pItem++;
    }

    __LEAVE_FUNCTION
}

BOOL	Obj_Human::GetCurFromSoldList(_ITEM& itemref, UINT& uPrice )
{
    __ENTER_FUNCTION

	//次函数获得当前物品的实际数据
	if(ITEMISVALID(m_ItemHasBeenSold[m_nCurSold]))
	{
		itemref	= m_ItemHasBeenSold[m_nCurSold];
		uPrice  = m_SoldPrice[m_nCurSold];
		return	TRUE; 
	}
	else
	{
		INT temp = m_nCurSold--;
		m_nCurSold = (m_nCurSold<0)? (MAX_BOOTH_SOLD_NUMBER-1):m_nCurSold; 
		while(m_nCurSold != temp)
		{
			if(ITEMISVALID(m_ItemHasBeenSold[m_nCurSold]))
				break;
			m_nCurSold--;
			m_nCurSold = (m_nCurSold<0)? (MAX_BOOTH_SOLD_NUMBER-1):m_nCurSold; 
		}
		if(m_nCurSold == temp)
		{//回购表已空
			return FALSE;
		}
		else
		{
			itemref	= m_ItemHasBeenSold[m_nCurSold];
			uPrice  = m_SoldPrice[m_nCurSold];
			return	TRUE; 
		}
	}
	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL    Obj_Human::GetFromSoldListByIndex(INT iIndex, _ITEM& itemref, UINT& uPrice)
{
    __ENTER_FUNCTION

        //次函数获得当前物品的实际数据
        if(iIndex >= 0 && iIndex < MAX_BOOTH_SOLD_NUMBER)
        {
            m_SoldContainer.GetItem(iIndex)->SaveValueTo(&itemref);

            if(ITEMISVALID(itemref))
            {
                uPrice  = m_SoldPrice[iIndex] * m_SoldContainer.GetItem(iIndex)->GetLayedNum();
                return TRUE;
            }
        }
    __LEAVE_FUNCTION

    return FALSE ;    
}

BOOL	Obj_Human::RecieveOneFromSoldList(INT iIndex)
{
    BOOL bRet = HumanItemLogic::RecieveItemToBag(this, &m_SoldContainer, iIndex);
    if (bRet)
    {
        for (INT i=0; i < MAX_BOOTH_SOLD_NUMBER; i++)
        {
            if (NULL == m_SoldContainer.GetItem(i))
            {
                    g_ItemOperator.MoveItem
                    (
                    &m_SoldContainer,
                    i+1,
                    &m_SoldContainer,
                    i
                    );
            }
        }
    }
	return bRet;
}

VOID	Obj_Human::DeleteFromSoldList()
{
	g_ItemOperator.EraseItem(&m_SoldContainer, m_nCurSold--);
	m_nCurSold = (m_nCurSold<0)? (MAX_BOOTH_SOLD_NUMBER-1):m_nCurSold; 
}

VOID	Obj_Human::ClearSoldList()
{
	ZeroMemory(m_ItemHasBeenSold, MAX_BOOTH_SOLD_NUMBER*sizeof(_ITEM));
	m_nCurSold = 0;
}




/////////////////////////////////////////////////////////////////////////////////
//obj 公共属性接口


/////////////////////////////////////////////////////////////////////////////////
//character 公共属性接口


/////////////////////////////////////////////////////////////////////////////////
//human 公共属性接口
const INT Obj_Human::__GetSex( ) const//性别 human
{
	return 0 ;
}
VOID Obj_Human::__SetSex( const INT sex )
{
}
const UINT Obj_Human::__GetCreateDate( ) const//创建时间 human
{
	return 0 ;
}
VOID Obj_Human::__SetCreateDate( const UINT createdate )
{
}
const CHAR* Obj_Human::__GetNick( ) const//昵称 human
{
	return NULL ;
}
VOID Obj_Human::__SetNick( const CHAR* nick )
{
}

VOID Obj_Human::__SetPasswordProtect(INT nType)
{
    m_PWProtect.SetFlag(nType);
}

VOID Obj_Human::__SetProtectFlag(BOOL bFlag)
{
    m_PWProtect.SetProtectFlag(bFlag);
}

BOOL Obj_Human::__GetProtectFlag()
{
    return m_PWProtect.GetProtectFlag();
}

BOOL Obj_Human::__SetProtectTime(CHAR cTime)
{
    if (cTime > 0 && cTime <= 30)
    {
        m_PWProtect.SetProtectTime(cTime);
        return TRUE;
    }
    return FALSE;
}

CHAR Obj_Human::__GetProtectTime()
{
    return m_PWProtect.GetProtectTime();
}

BOOL Obj_Human::__IsPasswordProtect(MINORPASSWD_INFO::PROTECT_TYPE nType)
{
    return m_PWProtect.CompareFlag( nType );
}

BOOL Obj_Human::__IsPasswordSetup()
{
	return ( strlen( GetDB()->GetPasswd() ) > MIN_PWD );
}
BOOL Obj_Human::__IsPasswordUnlock()
{
	return m_bIsPasswdUnlock;
}
VOID Obj_Human::__LockPassword() // 输入二级密码错误，则调用
{
	m_bIsPasswdUnlock = FALSE;
}
VOID Obj_Human::__UnlockPassword() // 输入二级密码正确
{
	m_bIsPasswdUnlock = TRUE;
}
const CHAR* Obj_Human::__GetPassword( ) //二级密码 human
{
	return GetDB()->GetPasswd();
}
BOOL Obj_Human::__SetPassword( const CHAR* password )
{
	if ( password == NULL
	 || strlen(password) >= MAX_PWD || strlen(password) < MIN_PWD
	 )
	{
		Assert( password );
		Assert( strlen(password) < MAX_PWD );
		return FALSE;
	}

    // 如果新密码为空  则去掉密码
    if ( 0 == strlen(password) )
    {
	    GetDB()->DelPasswd();
    }
    else
    {
        GetDB()->SetPasswd( password );
    }

	return TRUE;
}

VOID Obj_Human::__DelPassword( ) // 删除二级密码
{
	GetDB()->DelPasswd();
}
UINT Obj_Human::__GetPasswordDeleteTime( ) // 强制解除二级密码的时间
{
	return GetDB()->GetPasswdDeleteTime();
}
VOID Obj_Human::__SetPasswordDeleteTime( UINT uTime ) // 设置强制解除二级密码的时间
{
	GetDB()->SetPasswdDeleteTime( uTime );
}

UINT Obj_Human::__GetPasswordDeleteRemainTime( ) // 得到强制解除密码的剩余时间
{
	if( __GetPasswordDeleteTime() > 0 )
	{
		INT nReturnTime;

		UINT	DeltaTime = (UINT)g_pTimeManager->GetANSITime() - __GetPasswordDeleteTime();
		nReturnTime = g_Config.m_ConfigInfo.m_nDeleteDelayTime - DeltaTime;
		if( nReturnTime < 0 )
		{
			return 0;
		}

		return nReturnTime;
	}

	return UINT_MAX;
}

BOOL Obj_Human::__IsValidToDeletePassword( ) // 是否满足强制解除二级密码条件
{
__ENTER_FUNCTION

	if( __GetPasswordDeleteTime() > 0 )
	{
		INT nTime;

		nTime = INT(g_pTimeManager->GetANSITime() - __GetPasswordDeleteTime());

		if( nTime >= g_Config.m_ConfigInfo.m_nDeleteDelayTime )
		{
			return TRUE;
		}
	}

__LEAVE_FUNCTION

	return FALSE;
}

UINT Obj_Human::__GetHairColor( ) const//头发颜色 human
{
	Obj_Human& rMe = (Obj_Human&)(*this);

	_ITEM_EFFECT* pIE = rMe.ItemEffect(IATTRIBUTE_COLOR_HAIR);
	Assert( pIE );
	if( pIE->IsActive() )
	{
		return pIE->m_Attr.m_Value;
	}
	return m_DB.GetHairColor() ;
}
VOID Obj_Human::__SetHairColor( UINT haircolor )
{
	m_DB.SetHairColor(haircolor);
	GetDB()->SetHairColor(haircolor);
}

BYTE Obj_Human::__GetFaceColor( ) const//脸颜色 human
{
	return m_DB.GetFaceColor() ;
}
VOID Obj_Human::__SetFaceColor( BYTE facecolor )
{
	m_DB.SetFaceColor(facecolor);
}
BYTE Obj_Human::__GetHairModel( ) const//头发模型 human
{
	Obj_Human& rMe = (Obj_Human&)(*this);

	_ITEM_EFFECT* pIE = rMe.ItemEffect(IATTRIBUTE_MODEL_HAIR);

	Assert( pIE );
	if( pIE->IsActive() )
	{
		return pIE->m_Attr.m_Value;
	}

	return m_DB.GetHairModel() ;
}
VOID Obj_Human::__SetHairModel( BYTE hairmodel )
{
	m_DB.SetHairModel(hairmodel);
}
BYTE Obj_Human::__GetFaceModel( ) const//脸模型 human
{
	return m_DB.GetFaceModel() ;
}
VOID Obj_Human::__SetFaceModel( BYTE facemodel )
{
	m_DB.SetFaceModel(facemodel);
}
const UINT Obj_Human::__GetOnlineTime( ) const//在线时间 human
{
	return 0 ;
}
VOID Obj_Human::__SetOnlineTime( const UINT onlinetime )
{
}
const UINT Obj_Human::__GetLastLevelUpTime( ) const//上次升级时间 human
{
	return 0 ;
}
VOID Obj_Human::__SetLastLevelUpTime( const UINT lastleveluptime )
{
}
const UINT Obj_Human::__GetLastLoginTime( ) const//上次登录时间 human
{
	return 0 ;
}
VOID Obj_Human::__SetLastLoginTime( const UINT lastlogintime )
{
}
const UINT Obj_Human::__GetLastLogoutTime( ) const//上次登出时间 human
{
	return 0 ;
}
VOID Obj_Human::__SetLastLogoutTime( const UINT lastlogouttime )
{
}
const INT Obj_Human::__GetBagItemCount( ) const//背包中物品数量 human
{
	return 0 ;
}
VOID Obj_Human::__SetBagItemCount( const INT bagitemcount )
{
}
const _ITEM* Obj_Human::__GetBagItem( const INT bagindex ) const//背包中某个位置的物品 human
{
	return m_DB.GetBagItem(bagindex);
}
VOID Obj_Human::__SetBagItem( const INT bagindex, const _ITEM* item )
{
}
const INT Obj_Human::__GetBankItemCount( ) const//银行中物品数量 human
{
	return m_DB.GetBankItemCount();
}
VOID Obj_Human::__SetBankItemCount( const INT Bankitemcount )
{
	m_DB.SetBankItemCount(Bankitemcount);
}
const _ITEM* Obj_Human::__GetBankItem( const INT bankindex ) const//银行中某个位置的物品 human
{
	return m_DB.GetBankItem(bankindex);
}
VOID Obj_Human::__SetBankItem( const INT bankindex, const _ITEM* item )
{
	m_DB.SetBankItem(bankindex, item);
}
const INT	Obj_Human::__GetBankEndIndex( ) const//当前银行的大小
{
	return m_DB.GetBankEndIndex();
}
VOID Obj_Human::__SetBankEndIndex( const INT bankindex )
{
	return m_DB.SetBankEndIndex(bankindex);
}
const INT	Obj_Human::__GetBankMoney( ) const//当前银行内的钱数
{
	return m_DB.GetBankMoney();
}

const INT	Obj_Human::__GetBankSize( ) const//当前银行格子数
{
    return m_DB.GetBankEndIndex();
}

VOID        Obj_Human::__ExpandBankSize( const INT size )//扩展银行格子
{
    if (m_BankItem.ExpandSize(size))
    {
        m_DB.ExpandBankSize(size);
    }
}

VOID Obj_Human::__SetBankMoney( const INT money )
{
	m_DB.SetBankMoney(money);
}


const INT Obj_Human::__GetEquipItemCount( ) const//装备数量 human
{
	return 0 ;
}
VOID Obj_Human::__SetEquipItemCount( const INT equipitemcount )
{
}
const _ITEM* Obj_Human::__GetEquipItem( const INT equipindex ) const//身上某个位置的装备 human
{
	return NULL ;
}
VOID Obj_Human::__SetEquipItem( const INT equipindex, const _ITEM* item )
{
}
const INT Obj_Human::__GetMissionCount( ) const//接受的任务数量 human
{
	return 0 ;
}
VOID Obj_Human::__SetMissionCount( const INT missioncount )
{
}
const _OWN_MISSION* Obj_Human::__GetMission( const INT missionindex ) const//某个任务数据 human
{
	return NULL ;
}
VOID Obj_Human::__SetMission( const INT missionindex, _OWN_MISSION* mission )
{
}
const BOOL Obj_Human::__IsMissionHaveDone( MissionID_t missionid ) const//判断某个任务是否完成 human
{
	return FALSE ;
}
VOID Obj_Human::__SetMissionDoneFalg( MissionID_t missionid, BOOL flag )
{
}
const BOOL Obj_Human::__IsAbilityHaveLearned(AbilityID_t abilityid) const
{
__ENTER_FUNCTION

	if( abilityid<1 || abilityid>MAX_CHAR_ABILITY_NUM )
	{
		return FALSE;
	}

	if( GetAbilityList()->Get_Ability(abilityid).m_Level > 0 )
	{
		return TRUE;
	}

__LEAVE_FUNCTION

	return FALSE;
}
const INT Obj_Human::__GetAbilityLevel( const AbilityID_t abilityid ) const//某个生活技能级别 human
{
__ENTER_FUNCTION

	if( abilityid<1 || abilityid>MAX_CHAR_ABILITY_NUM )
	{
		return 0;
	}

	return GetAbilityList()->Get_Ability(abilityid).m_Level;

__LEAVE_FUNCTION

	return 0 ;
}
VOID Obj_Human::__SetAbilityLevel( const AbilityID_t abilityid, INT lvl)
{
__ENTER_FUNCTION

	m_DB.SetAbilityLevel(abilityid,lvl);

__LEAVE_FUNCTION
}
const INT Obj_Human::__GetAbilityExp( const AbilityID_t abilityid ) const//某个生活技能熟练度 human
{
__ENTER_FUNCTION

	if( abilityid<1 || abilityid>MAX_CHAR_ABILITY_NUM )
	{
		return 0;
	}

	return GetAbilityList()->Get_Ability(abilityid).m_Exp;

__LEAVE_FUNCTION

	return 0 ;
}
VOID Obj_Human::__SetAbilityExp( const AbilityID_t abilityid, INT exp )
{
__ENTER_FUNCTION
	m_DB.SetAbilityExp(abilityid,exp);
__LEAVE_FUNCTION
}
const BOOL Obj_Human::__IsPrescrHaveLearned( const PrescriptionID_t prescrid ) const //某个配方是否学会 human
{
__ENTER_FUNCTION

	INT idx;

	idx = (INT)prescrid;
	if( idx<0 || idx>=MAX_ABILITY_PRESCRIPTION_NUM )
	{
		Assert(FALSE);
		return FALSE;
	}

	CHAR cPrescr;
	cPrescr = GetAbilityList()->m_aPrescr[idx>>3];
	cPrescr >>= idx%8;

	if( cPrescr & 0x01 )
	{
		return TRUE;
	}

__LEAVE_FUNCTION

	return FALSE;
}
VOID Obj_Human::__SetPrescrLearnedFlag( const PrescriptionID_t prescrid, BOOL flag )
{
__ENTER_FUNCTION
	m_DB.SetPrescrLearnedFlag(prescrid,flag);
__LEAVE_FUNCTION
}
//add by gh 2010/07/19 是否能学习生活技能
BOOL Obj_Human::__IsCanLearnAbility( const AbilityID_t abilityid)
{
	if( abilityid<1 || abilityid>MAX_CHAR_ABILITY_NUM )
	{
		return FALSE;
	}
	return TRUE;
}
const _ITEM_EFFECT* Obj_Human::__GetItemEffect( const INT index ) const//装备效果 human
{
	return &m_pItemEffect[index] ;
}
VOID Obj_Human::__SetItemEffect( const INT index, _ITEM_EFFECT* itemeffect )
{
}
const _ITEM_VALUE Obj_Human::__GetItemValue( const INT index ) const//装备效果值 human
{
	return m_pItemEffect[index].m_Attr ;
}
VOID Obj_Human::__SetItemValue( const INT index, _ITEM_VALUE value)
{
	m_pItemEffect[index].m_Attr = value ;
}

const UINT Obj_Human::__GetLastTurnPickTime() const
{
	return	m_uLastTurnPickTime;
}

VOID Obj_Human::__SetLastTurnPickTime(UINT uTime)
{
	m_uLastTurnPickTime = uTime;
}

const TeamID_t Obj_Human::__GetTeamID( ) const //队伍号 human
{
	return m_TeamInfo.GetTeamID();
}
VOID Obj_Human::__SetTeamID( const TeamID_t teamid )
{
	m_TeamInfo.SetTeamID( teamid );
}
BOOL Obj_Human::__IsTeamLeader( ) const // 是否队长
{
	return m_TeamInfo.IsLeader();
}
const INT Obj_Human::__GetTeamMemberCount( ) const //队伍中成员数量 human
{
	return m_TeamInfo.GetTeamMemberCount();
}
const TEAMMEMBER* Obj_Human::__GetTeamMember( const INT memberindex ) const //取得队伍中某个成员
{
	return m_TeamInfo.GetTeamMember( memberindex );
}
VOID Obj_Human::__AddTeamMember( const TEAMMEMBER* member )
{
	m_TeamInfo.AddMember( member );
}
const INT Obj_Human::__GetTeamSceneMemberCount( ) const //队伍中同场景成员数量 human
{
	return m_TeamInfo.GetSceneMemberCount();
}
const ObjID_t Obj_Human::__GetTeamSceneMember( const INT memberindex ) const //取得队伍中某同场景成员的 OBJID
{
	return m_TeamInfo.GetSceneMemberObjID(memberindex);
}

VOID Obj_Human::__SetTeamFollowFlag( BOOL flag ) // 设置队伍跟随状态
{
	m_TeamInfo.SetTeamFollowFlag(flag);
}

const BOOL Obj_Human::__GetTeamFollowFlag( ) const // 获得队伍跟随状态
{
	return m_TeamInfo.GetTeamFollowFlag();
}

VOID Obj_Human::__AddFollowedMember( const _FOLLOWEDMEMBER& FollowedMember) // 增加一个跟随的队员
{
	m_TeamInfo.AddFollowedMember( FollowedMember );
}

VOID Obj_Human::__DelFollowedMember( GUID_t guid ) // 移出一个跟随队员
{
	m_TeamInfo.DelFollowedMember( guid );
}

const INT Obj_Human::__GetFollowedMembersCount() const // 得到跟随状态的队友数量
{
	return m_TeamInfo.GetFollowedMembersCount();
}

const _FOLLOWEDMEMBER* Obj_Human::__GetFollowedMember( INT i ) const // 得到第 i 个跟随的队友
{
	Assert( i>-1 && i<m_TeamInfo.GetFollowedMembersCount() );
	return m_TeamInfo.GetFollowedMember(i);
}

VOID Obj_Human::__ClearFollowedMembers() // 清除所有的跟随队友列表
{
	m_TeamInfo.ClearFollowedMembers();
}

const FLOAT Obj_Human::__GetTeamFollowSpeed( ) const // 得到组队跟随的速度
{
	return m_fTeamFollowSpeed;
}

VOID Obj_Human::__SetTeamFollowSpeed( FLOAT fSpeed ) // 设置组队跟随的速度
{
__ENTER_FUNCTION

	m_fTeamFollowSpeed = fSpeed;

	GCCharBaseAttrib Msg;
	Msg.setObjID( GetID() );
	Msg.setMoveSpeed( fSpeed );

	getScene()->BroadCast( &Msg, this, TRUE );

__LEAVE_FUNCTION
}

VOID Obj_Human::__OutOfTeamFollowRange() // 超出组队跟随范围
{
	if( m_OutofTeamFollowRangeTime == 0 )
	{
		m_OutofTeamFollowRangeTime = g_pTimeManager->GetANSITime();
	}
	else if( g_pTimeManager->GetANSITime() - m_OutofTeamFollowRangeTime > g_Config.m_ConfigInfo.m_nTimeForLoseFollow )
	{
		__StopTeamFollow();
		m_OutofTeamFollowRangeTime = 0;
	}
}

VOID Obj_Human::__InTeamFollowRange() // 在组队跟随范围内
{
	if( m_OutofTeamFollowRangeTime > 0 )
	{
		m_OutofTeamFollowRangeTime = 0;
	}
}

const BOOL Obj_Human::__GetTeamFollowSpeedUp( ) const // 获得组队跟随加速状态
{
	return m_bTeamFollowSpeedUp;
}

VOID Obj_Human::__SetTeamFollowSpeedUp( BOOL bSpeedUp ) // 设置组队跟随加速状态
{
	m_bTeamFollowSpeedUp = bSpeedUp;
}

VOID Obj_Human::__StopTeamFollow( BOOL bWorldFlag ) // 停止跟随
{
__ENTER_FUNCTION

	TeamInfo* pTeamInfo = GetTeamInfo();
	if( pTeamInfo->HasTeam() == FALSE )
	{
		Assert(FALSE);
		return;
	}

	Obj_Human* pLeader;
	BOOL bLeaderExist = TRUE;

	if ( __GetTeamFollowFlag() )
	{
		if ( pTeamInfo->IsLeader() )
		{ // 队长退出
			for( INT i=__GetFollowedMembersCount()-1; i>=0; --i)
			{ // 每个人都自动退出
				Obj_Human* pMember;
				
				pMember = __GetFollowedMember(i)->m_pHuman;

				if ( pMember != NULL )
				{
					pMember->__ClearFollowedMembers();
					pMember->GetHumanAI()->PushCommand_StopTeamFollow();
					pMember->__SetTeamFollowFlag( FALSE );

					GCReturnTeamFollow RetMsg;

					RetMsg.SetReturn( TF_RESULT_STOP_FOLLOW );
					RetMsg.SetGUID( pMember->GetGUID() );

					pMember->GetPlayer()->SendPacket( &RetMsg );
				}
			}

			pLeader = this;
		}
		else
		{
			pLeader = __GetFollowedMember(0)->m_pHuman;
			if( pLeader==NULL )
			{ // 没有找到队长
				bLeaderExist = FALSE;
			}

			GUID_t guid = GetGUID();

			__DelFollowedMember( guid ); // 先自己把自己清了
			GetHumanAI()->PushCommand_StopTeamFollow();
			__SetTeamFollowFlag( FALSE );

			for( INT i=__GetFollowedMembersCount()-1; i>=0; --i)
			{ // 其他跟随状态的队友都移出他
				Obj_Human* pMember = __GetFollowedMember(i)->m_pHuman;

				if ( pMember!=NULL )
				{
					pMember->__DelFollowedMember( guid );
				}
			}

			__ClearFollowedMembers(); // 清除整个跟随列表

			GCReturnTeamFollow RetMsg;

			RetMsg.SetReturn( TF_RESULT_STOP_FOLLOW );
			RetMsg.SetGUID( guid );

			GetPlayer()->SendPacket( &RetMsg ); // 发给自己

			if ( bLeaderExist )
			{
				pLeader->GetPlayer()->SendPacket( &RetMsg ); // 发给队长
			}
		}

		if ( bWorldFlag )
		{
			GWStopTeamFollow* pMsg = (GWStopTeamFollow*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_STOP_TEAMFOLLOW);
			pMsg->SetGUID( GetGUID() );
			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		}

		if ( bLeaderExist )
		{
			GCTeamFollowList Msg;
			Msg.SetObjID( pLeader->GetID() );

			for( INT i=0; i<pLeader->__GetFollowedMembersCount(); ++i )
			{
				Msg.AddFollowMember( pLeader->__GetFollowedMember(i)->m_GUID );
			}

			getScene()->BroadCast( &Msg, pLeader, TRUE );
		}
	}

__LEAVE_FUNCTION
}

VOID Obj_Human::ChangeScene( SceneID_t sSceneID, SceneID_t dSceneID, const WORLD_POS& pos, BYTE uDir )
{
	__ENTER_FUNCTION

	GCNotifyChangeScene Msg ;
	Msg.setCurrentSceneID( sSceneID ) ;
	Msg.setTargetSceneID( dSceneID ) ;
	Msg.setTargetPos( &pos ) ;
	Msg.setTargetDir( uDir ) ;
	Msg.setResID( g_pSceneManager->GetSceneResID(dSceneID) ) ;

	m_CreateCopySceneID = INVALID_ID ;//如果是在创建副本，则取消创建标志

	Scene* pDestScene = g_pSceneManager->GetScene(dSceneID) ;
	if( pDestScene )
	{
		if( pDestScene->GetSceneType() != SCENE_TYPE_GAMELOGIC )
		{
			GetDB()->SetDBBakScene( sSceneID ) ;
			GetDB()->SetDBBakPosition( *getWorldPos() ) ;
		}
		if( pDestScene->GetSceneType() == SCENE_TYPE_COPY )
		{
			if( pDestScene->GetSceneStatus() != SCENE_STATUS_RUNNING )
			{//如果目标场景不是运行状态的副本，不能发送传送消息
				return ;
			}

			pDestScene->m_CopyData.AddMemberGUID(GetGUID()) ;
		}
	}

	GetPlayer()->SendPacket( &Msg ) ;

	SetChangeSceneFlag( TRUE );

	__LEAVE_FUNCTION
}

VOID Obj_Human::CreateCity( SceneID_t PortSceneID)
{
	__ENTER_FUNCTION
	//暂时一个场景只能挂一个城市
	Scene* pPortScene = g_pSceneManager->GetScene(PortSceneID);
	for(INT	i = 0; i<MAX_CITY_PORT; i++)
	{
		if(pPortScene->m_CityData.m_ScenePortList[i] != INVALID_ID)
		{
			return;
		}
	}

	GWCityApplyNewCity* pMsgToWorld = (GWCityApplyNewCity*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_CITYAPPLYNEWCITY);
	pMsgToWorld->SetSceneID(PortSceneID);
	pMsgToWorld->SetPlayerGuid(GetGUID());

	g_pServerManager->SendPacket( pMsgToWorld, INVALID_ID );
	__LEAVE_FUNCTION

}
VOID Obj_Human::DeleteCity( SceneID_t PortSceneID, INT iSceneIndex)
{
	__ENTER_FUNCTION
	Scene* pPortScene = g_pSceneManager->GetScene(PortSceneID);
	if(pPortScene->m_CityData.m_ScenePortList[iSceneIndex] != INVALID_ID)
	{//存在此城市
		Scene* pCityScene = g_pSceneManager->GetScene(pPortScene->m_CityData.m_ScenePortList[iSceneIndex]);
		Assert(pCityScene);
		if(!pCityScene->m_SceneInitData.m_CityData.m_Guid.isNull() && pCityScene->m_SceneInitData.m_nDataType == SCENE_TYPE_CIT)
		{//城市ID有效
			_CITY_GUID CityGuid = pCityScene->m_SceneInitData.m_CityData.m_Guid;
			GWCityClose* pMsgToWorld = (GWCityClose*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_CITYCLOSE);
			pMsgToWorld->SetCityGuid(CityGuid);
			pMsgToWorld->SetPlayerGuid(GetGUID());
			g_pServerManager->SendPacket( pMsgToWorld, INVALID_ID );
		}
	}
	__LEAVE_FUNCTION
}

VOID Obj_Human::ChangeCityBuilding( SceneID_t SceneID, INT BuildingID, INT BuildingLevel)
{
	__ENTER_FUNCTION
	Scene* pCityScene = g_pSceneManager->GetScene(SceneID);
	Assert(pCityScene);
	if(!pCityScene->m_SceneInitData.m_CityData.m_Guid.isNull() && pCityScene->m_SceneInitData.m_nDataType == SCENE_TYPE_CIT)
	{
		_CITY_GUID	CityGuid = pCityScene->m_SceneInitData.m_CityData.m_Guid;
		INT			iParam[MAX_IPARAM_NUM] = {0};

		GWCityOpt* pMsgToWorld = (GWCityOpt*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_CITYOPT);
		pMsgToWorld->SetCityGuid(CityGuid);
		pMsgToWorld->SetOpt(GWCityOpt::OPT_CHANGE_BUILDING_DATA);
		iParam[0] = BuildingID;
		iParam[1] = BuildingLevel;
		pMsgToWorld->SetiParam(iParam);
		g_pServerManager->SendPacket( pMsgToWorld, INVALID_ID );
	}
	__LEAVE_FUNCTION
	return;
}

VOID Obj_Human::UpdateViewCharacter(VOID)
{
__ENTER_FUNCTION

	Obj_Character::UpdateViewCharacter();

	if(getZoneID() != INVALID_ID)
	{
		// 反隐数据改变
		if(m_nView_PrevLevel != GetLevel()
			|| m_nView_PrevDetectLevel != GetDetectLevel())
		{
			OBJLIST listObj;
			getScene()->ScanObj( getZoneID(), MAX_REFESH_OBJ_ZONE_RADIUS, &(listObj) );

			Obj *pFindObj;
			BOOL bPrevCanViewTarget;
			BOOL bCurrentCanViewTarget;
			BYTE i;
			for ( i = 0; i < listObj.m_Count; i++ )
			{
				pFindObj				= listObj.m_aObj[i];
				bPrevCanViewTarget		= pFindObj->IsPrevCanViewMe(this);
				bCurrentCanViewTarget	= pFindObj->IsCanViewMe(this);
				if(bPrevCanViewTarget && !bCurrentCanViewTarget)
				{
					Packet *pPacket = pFindObj->CreateDeleteObjPacket();
					if ( pPacket != NULL )
					{
						GetPlayer()->SendPacket( pPacket );

						pFindObj->DestroyDeleteObjPacket( pPacket );
					}
				}
				else if(!bPrevCanViewTarget && bCurrentCanViewTarget)
				{
					Packet *pPacket = pFindObj->CreateNewObjPacket();
					if ( pPacket != NULL )
					{
						GetPlayer()->SendPacket( pPacket );

						pFindObj->DestroyNewObjPacket( pPacket );
					}
				}
			}
			// 放在最后
			m_nView_PrevLevel		= GetLevel();
			m_nView_PrevDetectLevel	= GetDetectLevel();
		}
	}

__LEAVE_FUNCTION
}


VOID Obj_Human::UpdateTitlesToClient(BYTE cType)
{
	__ENTER_FUNCTION

	//PACKET_ALL_TITLES_MSG
    _TITLE titleInfo;
    memcpy(&titleInfo, m_DB.GetTitleInfo(), sizeof(_TITLE));
    
    for (INT i=0; i<MAX_TITLE_SIZE; ++i)
    {
        UINT uTime = titleInfo.m_TitleArray[i].m_uTime;
        time_t time = g_pTimeManager->GetANSITime();
        if (uTime > 0)//计时称号,转换成剩余的分钟数 发给客户端
        {
            if ((UINT)time <= uTime)
            {
                titleInfo.m_TitleArray[i].m_uTime = uTime - (UINT)time / 60;
            }
        }
    }

	GCCharAllTitles Msg;
    Msg.SetTitleInfo(&titleInfo);
    Msg.SetType(cType);
 	GetPlayer()->SendPacket(&Msg);

    SendMsg_RefeshAttrib(); 

	__LEAVE_FUNCTION
}

VOID Obj_Human::UpdateTitleTime()
{
    if (m_DB.UpdateTitleTime())
    {
        UpdateTitlesToClient();
    }
}

//上限保护时间
VOID Obj_Human::SetLoginProtectTime(time_t time)
{
    m_LoginProtectTime = time + (time_t)(m_PWProtect.GetProtectTime()*60);    
}

BOOL Obj_Human::IsInLoginProtectTime()
{
    return (g_pTimeManager->GetANSITime()>= m_LoginProtectTime) ? FALSE:TRUE;
}

VOID Obj_Human::SetTitleLoginTime (time_t time)
{
    __ENTER_FUNCTION
    m_TitleLoginTime = time;
    __LEAVE_FUNCTION
}

time_t Obj_Human::GetTitleLoginTime ()
{
    __ENTER_FUNCTION
    return m_TitleLoginTime;
    __LEAVE_FUNCTION
}

VOID Obj_Human::SendOperateResultMsg(INT nCode)
{
	__ENTER_FUNCTION
	GCOperateResult Msg ;
	Msg.setResult(nCode) ;
	if(NULL!=m_pPlayer)
	{
		m_pPlayer->SendPacket( &Msg ) ;
	}
	__LEAVE_FUNCTION
}

VOID Obj_Human::HorseHeartBeat( UINT uTime )
{
	__ENTER_FUNCTION
	m_ActiveHorse.HeartBeat( uTime );
	//如果没有马在交配
	if( !(GetDB()->GetHumanDB()->m_bHorseMating) )
	{
		return;
	}
	//找到处于交配状态的马

	UINT uDurnTime;
	UINT uCurrentTime = (UINT)g_pTimeManager->GetANSITime();
	INT iIndex = GetMatingEndHorseIndex( uCurrentTime, uDurnTime );

	if( iIndex == INVALID_ID )
	{
		return;
	}

	MatingHorseHeartBeat(iIndex, uDurnTime);

	__LEAVE_FUNCTION
}

VOID Obj_Human::SendHorseMatingSuccessMSG()
{
	__ENTER_FUNCTION

	//通知邮件
	CHAR  szName[MAX_CHARACTER_NAME] = {0};
	memcpy(szName, GetName(), MAX_CHARACTER_NAME);
	CHAR szMsgContent[MAX_MAIL_CONTEX] = {0};
	sprintf(szMsgContent, "%s 恭喜您的骑乘已经繁殖成功，请务必于48小时内组队前来领取，如果逾期不至的话，可怜的骑乘将会被我们收养", GetName());
	getScene()->SendNormalMail(this, szName, szMsgContent);
	__LEAVE_FUNCTION
}

INT Obj_Human::GetMatingEndHorseIndex( UINT uTime, UINT& uDurnTime )
{
	__ENTER_FUNCTION

	INT iIndex = INVALID_ID;
	for ( INT i = 0; i < HUMAN_PET_MAX_COUNT; i++ )
	{
		if ( m_DB.m_dbPetList->m_aPetDB[i].m_GUID.IsNull() )
		{
			continue;
		}
		if( !m_DB.m_dbPetList->m_aPetDB[i].m_bMating )
		{
			continue;
		}
		uDurnTime = uTime - m_DB.m_dbPetList->m_aPetDB[i].m_uMatingStartTime;
		if( uDurnTime >= HorseMatingFinishTime )
		{
			iIndex = i;
			break;
		}
	}
	return iIndex;
	__LEAVE_FUNCTION
	return INVALID_ID;
}

VOID Obj_Human::SendHorseMatingDeleteMSG()
{
	__ENTER_FUNCTION

	//通知邮件
	CHAR  szName[MAX_CHARACTER_NAME] = {0};
	memcpy(szName, GetName(), MAX_CHARACTER_NAME);
	CHAR szMsgContent[MAX_MAIL_CONTEX] = {0};
	sprintf(szMsgContent, "%s 由于你没有及时前来领取你在这里繁殖成功的骑乘，它和它的后代已经被我们代为收养了，特此通知。", GetName());
	getScene()->SendNormalMail(this, szName, szMsgContent);
	__LEAVE_FUNCTION

}

VOID Obj_Human::RemoveMatingTimeOutHorse( INT iIndex )
{
	__ENTER_FUNCTION

	GCRemovePet msgRemovePet;
	msgRemovePet.SetGUID( m_DB.m_dbPetList->m_aPetDB[iIndex].m_GUID );
	((GamePlayer*)(GetPlayer()))->SendPacket(&msgRemovePet);
	INT iItemIndex = GetPetContain()->GetIndexByGUID( &m_DB.m_dbPetList->m_aPetDB[iIndex].m_GUID );
	_PET_DB oPetDB;
	oPetDB.CleanUp();
	g_ItemOperator.SetItemValue(GetPetContain(), iItemIndex, &oPetDB);
	__LEAVE_FUNCTION
}

VOID Obj_Human::MatingHorseHeartBeat( INT iIndex, UINT uDurnTime )
{
	__ENTER_FUNCTION

	if( m_DB.m_dbPetList->m_aPetDB[iIndex].m_bMatingMsgSend	== FALSE )
	{
		SendHorseMatingSuccessMSG();
		INT iItemIndex = GetPetContain()->GetIndexByGUID( &m_DB.m_dbPetList->m_aPetDB[iIndex].m_GUID );

		g_ItemOperator.SetMatingFinishFlag( GetPetContain(), iItemIndex, TRUE );
	}

	if( uDurnTime > HorseMatingDeleteTime )
	{
		SendHorseMatingDeleteMSG();

		RemoveMatingTimeOutHorse(iIndex);
		//const_cast<_HUMAN_DB_LOAD*>(GetDB()->GetHumanDB())->m_bHorseMating = FALSE;
		GetDB()->SetHorseMattingFlag( FALSE );
	}
	__LEAVE_FUNCTION

}

VOID Obj_Human::SetGUIDOfCallUpHorse( PET_GUID_t val )
{
	__ENTER_FUNCTION

	m_GUIDOfCallUpHorse = val;
	SetCurrentHorseGUID(val);
	if( !val.IsNull() )
	{
		m_ActiveHorse.InitCurHappinessTime();
	}
	__LEAVE_FUNCTION
}

Obj_Pet* Obj_Human::GetPet( INT iIndex ) const
{
	return m_pPet[iIndex];
}

VOID Obj_Human::SaveMoneyLogByHuman( MONEY_OP_TYPE eReason, INT iModify )
{
	__ENTER_FUNCTION

	MONEY_LOG_PARAM	MoneyLogParam;
	MoneyLogParam.CharGUID	=	GetGUID();
	MoneyLogParam.OPType	=	eReason;	
	MoneyLogParam.Count		=	iModify;
	MoneyLogParam.SceneID	=	getScene()->SceneID();
	MoneyLogParam.XPos		=	getWorldPos()->m_fX;
	MoneyLogParam.ZPos		=	getWorldPos()->m_fZ;
	SaveMoneyLog(&MoneyLogParam);
	__LEAVE_FUNCTION
}

VOID Obj_Human::SaveMonsterPetToDB()
{
	__ENTER_FUNCTION

	_Monster_Pet_DB_Struct oMonsterPet;
	for( INT i=0; i<MAX_MONSTER_PET_TAKE; ++i )
	{
		if( NULL == m_pPet[i] )
		{
			continue;
		}
		m_pPet[i]->InitMonsterPetDB( oMonsterPet );
		GetDB()->SetMonsterPetAttr( i, &oMonsterPet );
	}
	__LEAVE_FUNCTION
}

BOOL Obj_Human::GetNearCanDropPos(WORLD_POS& dropPos)
{
	__ENTER_FUNCTION

	SceneDropPosManager* pDropPosMag = getScene()->GetSceneDropPosManager();
	Assert(pDropPosMag);

	WORLD_POS	Pos	=	*getWorldPos() ;
	UINT pos_x = (UINT)(Pos.m_fX/g_Config.m_ConfigInfo.m_DropPosDistance);
	UINT pos_z = (UINT)(Pos.m_fZ/g_Config.m_ConfigInfo.m_DropPosDistance);

	Pos.m_fX = pos_x*g_Config.m_ConfigInfo.m_DropPosDistance;
	Pos.m_fZ = pos_z*g_Config.m_ConfigInfo.m_DropPosDistance;

	INT nRandPos = RandGen::GetRand(0, 3);
	DROP_POS_TB* pDropPos = g_ItemTable.GetDropPosTB(nRandPos);
	Assert(pDropPos);
	if (!pDropPos)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::GetNearCanDropPos] GetDropPosTB index is error [%d]",nRandPos);	
		return FALSE;
	}
	dropPos.m_fX =  Pos.m_fX + pDropPos->m_DropX;
	dropPos.m_fZ =  Pos.m_fZ + pDropPos->m_DropZ;

	for (INT i=0; i<g_Config.m_ConfigInfo.m_DropSearchRange; ++i)
	{
		if (nRandPos>=g_Config.m_ConfigInfo.m_DropSearchRange)
		{
			nRandPos = 0;
		}
		pDropPos = g_ItemTable.GetDropPosTB(nRandPos);
		Assert(pDropPos);
		if (!pDropPos)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "[Obj_Human::GetNearCanDropPos] GetDropPosTB index is error [%d]",nRandPos);
			return FALSE;
		}
		dropPos.m_fX =  Pos.m_fX + pDropPos->m_DropX;
		dropPos.m_fZ =  Pos.m_fZ + pDropPos->m_DropZ;

		if (pDropPosMag->CanDrop(dropPos.m_fX, dropPos.m_fZ))
		{
			return TRUE;
		}
		++nRandPos;
	}

	return FALSE;

	__LEAVE_FUNCTION

		return FALSE;	
}

//SouXia	m_SouXia[MAX_SOUXIA_CONTAINER];

BOOL Obj_Human::CanAddSouXia()
{
	if(m_nCurSouXiaCount < MAX_SOUXIA_CONTAINER)
	{
		return TRUE;
	}
	return FALSE;
}
// 找到空位置
INT	Obj_Human::FindEmptyPos()
{
	if (!CanAddSouXia())
	{
		return -1;
	}

	for(UINT i=0; i<MAX_SOUXIA_CONTAINER; ++i)
	{
		if (m_SouXia[i].GetSouXiaData().GetCurPos() <0 )
		{
			return i;
		}
	}
	return -1;
}


INT	Obj_Human::AddSouXia(const INT souXiaIndex)
{
	Assert(souXiaIndex > 0);
	INT	newPos = FindEmptyPos(); 
	if(0 <= newPos)
	{
		SOUXIA_DATA SouXiaData;
		SouXiaData.m_SouXiaID = souXiaIndex;
		SouXiaData.SetCurPos(newPos);
		m_SouXia[newPos].SetSouXiaData(SouXiaData);

		m_nCurSouXiaCount++;
		return newPos;
	}
	return -1;
}

BOOL	Obj_Human::EraseSouXia(const INT humanSouXiaPos)
{
	Assert(humanSouXiaPos >= 0);
	m_nCurSouXiaCount--;
	m_SouXia[humanSouXiaPos].CleanUp();
	//保存信息到文件
	_SOUXIA_DB_LOAD		SouXia_db_Load;
	SouXia_db_Load.m_SouXiaData[humanSouXiaPos] = m_SouXia[humanSouXiaPos].GetSouXiaData();
	GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, humanSouXiaPos);	

	return TRUE;
}
//获取当前装备的捜侠录
SouXia  Obj_Human::GetCurEquipSouXia()
{
	//获取当前装备的搜侠录
	Item* pEquipSouXiaItem = GetEquipContain()->GetItem(HEQUIP_SOUXIA);
	Assert(pEquipSouXiaItem != NULL);
	Assert(pEquipSouXiaItem->GetItemClass() == ICLASS_EQUIP && EQUIP_SOUXIA == pEquipSouXiaItem->GetItemType());

	ItemParamValue ipv = (ItemParamValue)IPV_INT;
	INT tmpParamPos = pEquipSouXiaItem->GetItemParam(4, ipv);
	Assert(tmpParamPos >= 0 && tmpParamPos < MAX_SOUXIA_CONTAINER);
	return GetSouXia(tmpParamPos);
}

BOOL Obj_Human::HasLearnedProduct(UINT index)
{
	Assert(index>0);
	BOOL bRet = FALSE;

	bRet = GetCurEquipSouXia().HasLearnedProduct(index);
	if (bRet)
	{
		return bRet;
	}
	return bRet;
}
	
//设置神兽召唤技能次数
VOID Obj_Human::SetPetZhaoHuanSkillTimes( SkillID_t skillId, SHORT iDeplete )
{
	SouXia souxia = GetCurEquipSouXia();
	SOUXIA_DATA souXiaData = souxia.GetSouXiaData();
	for (int i=0; i<MAX_PET_ZHAOHUAN_COUNT; ++i)
	{
		if (souXiaData.m_Pet.StudyPet[i].StudyZhaoHuan == skillId && souXiaData.m_Pet.StudyPet[i].LeftUseTime > 0)
		{
			souXiaData.m_Pet.StudyPet[i].LeftUseTime += iDeplete;

			GCSouXiaSkillUpdate msgSkill;
			msgSkill.SetCurSouXiaPos(souXiaData.m_CurPos);
			msgSkill.SetSouXiaSkillType(ITEM_PET_ZHAOHUAN);
			msgSkill.SetSouXiaSkillID(skillId);
			SHORT tmpSkillValue = GetPetZhaoHuanSkillTimes(skillId );
			tmpSkillValue--;
			msgSkill.SetSouXiaSkillTime(tmpSkillValue);

			if(NULL!=m_pPlayer)
			{
				m_pPlayer->SendPacket( &msgSkill );
			}
			//如果次数减为0后更新位置
			if (souXiaData.m_Pet.StudyPet[i].LeftUseTime == 0 && souXiaData.m_CurPos >= 0)
			{
				souXiaData.m_Pet.StudyPet[i].StudyZhaoHuan = 0;
				//循环做交换
				ZhaoHuan  petTmp;
				for (INT j=i; j<souXiaData.GetCurPetCount()-1; ++j)
				{
					petTmp = souXiaData.m_Pet.StudyPet[j];
					souXiaData.m_Pet.StudyPet[i] = souXiaData.m_Pet.StudyPet[j+1];
					souXiaData.m_Pet.StudyPet[j+1] = petTmp;
					i=j+1;
				}
				souXiaData.DecCurPetCount();//总数减1

				//发送更新后的捜侠录数据
				GCSouXiaUpdate msg;
				msg.SetCurSouXiaPos(souXiaData.m_CurPos);
				msg.SetSouXiaType(ITEM_PET_ZHAOHUAN);
				msg.SetSouXiapetZhaoHuanCount(souXiaData.GetCurPetCount());
				msg.SetSouXiaPetZhaoHuan(souXiaData.m_Pet);//总数减1
				if(NULL!=m_pPlayer)
				{
					m_pPlayer->SendPacket( &msg );
				}
			}

			souxia.SetSouXiaData(souXiaData);//设置数据
			SetSouXia(souXiaData, souXiaData.m_CurPos);

			_SOUXIA_DB_LOAD		SouXia_db_Load;
			SouXia_db_Load.m_SouXiaData[souXiaData.m_CurPos] = souXiaData;
			GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, souXiaData.m_CurPos);	

			break;//目前是只减少第一个就可以退出了
		}	
	}
}
//获取神兽召唤技能次数
SHORT Obj_Human::GetPetZhaoHuanSkillTimes( SkillID_t skillId )
{
	SHORT times = 0;
	SouXia souxia = GetCurEquipSouXia();
	for (int i=0; i<MAX_PET_ZHAOHUAN_COUNT; ++i)
	{
		if (souxia.GetSouXiaData().m_Pet.StudyPet[i].StudyZhaoHuan == skillId)
		{
			times += souxia.GetSouXiaData().m_Pet.StudyPet[i].LeftUseTime;
		}	
	}
	return times;
}

//设置坐骑召唤技能次数
VOID Obj_Human::SetZuoJiZhaoHuanSkillTimes( SkillID_t skillId, SHORT iDeplete )
{
	SouXia souxia = GetCurEquipSouXia();
	SOUXIA_DATA souXiaData = souxia.GetSouXiaData();
	for (int i=0; i<MAX_ZUOJI_ZHAOHUAN_COUNT; ++i)
	{
		if (souXiaData.m_ZuoJi.StudyZuoji[i].StudyZhaoHuan == skillId && souXiaData.m_ZuoJi.StudyZuoji[i].LeftUseTime > 0)
		{
			souXiaData.m_ZuoJi.StudyZuoji[i].LeftUseTime += iDeplete;

			GCSouXiaSkillUpdate msgSkill;
			msgSkill.SetCurSouXiaPos(souXiaData.m_CurPos);
			msgSkill.SetSouXiaSkillType(ITEM_ZUOJI_ZHAOHUAN);
			msgSkill.SetSouXiaSkillID(skillId);
			SHORT tmpSkillValue = GetZuoJiZhaoHuanSkillTimes(skillId);
			tmpSkillValue--;
			msgSkill.SetSouXiaSkillTime(tmpSkillValue);//总数减1

			if(NULL!=m_pPlayer)
			{
				m_pPlayer->SendPacket( &msgSkill );
			}
			//如果次数减为0后更新位置
			if (souXiaData.m_ZuoJi.StudyZuoji[i].LeftUseTime == 0 && souXiaData.m_CurPos >= 0)
			{
				souXiaData.m_ZuoJi.StudyZuoji[i].StudyZhaoHuan = 0;
				//循环做交换
				ZhaoHuan  petTmp;
				for (INT j=i; j<souXiaData.GetCurZuoJiCount()-1; ++j)
				{
					petTmp = souXiaData.m_ZuoJi.StudyZuoji[j];
					souXiaData.m_ZuoJi.StudyZuoji[i] = souXiaData.m_ZuoJi.StudyZuoji[j+1];
					souXiaData.m_ZuoJi.StudyZuoji[j+1] = petTmp;
					i=j+1;
				}
				souXiaData.DecCurZuoJiCount();//坐骑技能总数减1

				//发送更新后的捜侠录数据
				GCSouXiaUpdate msg;
				msg.SetCurSouXiaPos(souXiaData.m_CurPos);
				msg.SetSouXiaType(ITEM_ZUOJI_ZHAOHUAN);
				msg.SetSouXiaZuoJiZhaoHuanCount(souXiaData.GetCurZuoJiCount());
				msg.SetSouXiaZuoJiZhaoHuan(souXiaData.m_ZuoJi);
				if(NULL!=m_pPlayer)
				{
					m_pPlayer->SendPacket( &msg );
				}
			}

			souxia.SetSouXiaData(souXiaData);//设置数据
			SetSouXia(souXiaData, souXiaData.m_CurPos);

			_SOUXIA_DB_LOAD		SouXia_db_Load;
			SouXia_db_Load.m_SouXiaData[souXiaData.m_CurPos] = souXiaData;
			GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, souXiaData.m_CurPos);	

			break;//目前是只减少第一个就可以退出了
		}	
	}
}
//获取坐骑召唤技能次数
SHORT Obj_Human::GetZuoJiZhaoHuanSkillTimes( SkillID_t skillId )
{
	SHORT times = 0;
	SouXia souxia = GetCurEquipSouXia();
	for (int i=0; i<MAX_ZUOJI_ZHAOHUAN_COUNT; ++i)
	{
		if (souxia.GetSouXiaData().m_ZuoJi.StudyZuoji[i].StudyZhaoHuan == skillId)
		{
			times += souxia.GetSouXiaData().m_ZuoJi.StudyZuoji[i].LeftUseTime;
		}	
	}
	return times;
}



BOOL Obj_Human::IsCanLogic( )
{
	__ENTER_FUNCTION

		GamePlayer* pGamePlayer = (GamePlayer*)(GetPlayer()) ;
	Assert( pGamePlayer ) ;

	return pGamePlayer->IsCanLogic( ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

VOID Obj_Human::OnLeaveScene(VOID)
{
	__ENTER_FUNCTION

	if(getScene() == NULL)
		return ;

	Obj_Character::OnLeaveScene();

	if(m_nBusObjID != INVALID_ID)
	{
		BusRule::RemovePassenger(getScene()->SceneID(), m_nBusObjID, GetID());
	}

	__LEAVE_FUNCTION
}

BOOL Obj_Human::IsPassenger(VOID)
{
	__ENTER_FUNCTION
		if(m_nBusObjID == INVALID_ID)
			return FALSE;

	if(getScene() == NULL)
		return FALSE;

	Obj_Bus *pBus = (Obj_Bus*)(getScene()->GetObjManager()->GetObj(m_nBusObjID));
	if(pBus == NULL)
		return FALSE;

	if(pBus->GetObjType() != Obj::OBJ_TYPE_BUS)
		return FALSE;

	return pBus->IsPassenger(GetID());
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL Obj_Human::IsChauffeur(VOID)
{
	__ENTER_FUNCTION
		if(m_nBusObjID == INVALID_ID)
			return FALSE;

	if(getScene() == NULL)
		return FALSE;

	Obj_Bus *pBus = (Obj_Bus*)(getScene()->GetObjManager()->GetObj(m_nBusObjID));
	if(pBus == NULL)
		return FALSE;

	if(pBus->GetObjType() != Obj::OBJ_TYPE_BUS)
		return FALSE;

	return pBus->IsChauffeur(GetID());
	__LEAVE_FUNCTION
		return FALSE;
}

VOID Obj_Human::SetBusObjID(ObjID_t nBusObjID)
{
	__ENTER_FUNCTION
		m_nBusObjID = nBusObjID;
	__LEAVE_FUNCTION
}