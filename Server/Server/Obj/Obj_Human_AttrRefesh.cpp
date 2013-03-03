// Obj_Human_AttrRefesh.cpp

#include "stdafx.h"
#include "Scene.h"

#include "GCCharBaseAttrib.h"
#include "GCCharEquipment.h"
#include "GCDetailAttrib.h"
#include "GCDetailAttrib_Pet.h"
#include "GCNotifyTeamInfo.h"
#include "GCTeamFollowList.h"
#include "GCTeamMemberInfo.h"
#include "GCCharImpactListUpdate.h"
#include "GCCanPickMissionItemList.h"
#include "GCDetailImpactListUpdate.h"
#include "GCCoolDownUpdate.h"
#include "DB_Struct.h"
#include "ItemOperator.h"
#include "GameTable.h"
#include "Obj_Human.h"
#include "GWRefreshDataToWorld.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"

using namespace Packets;

extern ENUM_REFESH_ATTR_TYPE CalcRefeshAttrType(Obj_Character *pSour, Obj_Character *pTarget );

UINT GetEquipmentMaxLevelGemID(Item *pEquipment)
{
	UINT uGemID = INVALID_ID;
	if(pEquipment != NULL)
	{
		BYTE byCount = pEquipment->GetEquipGemCount();
		BYTE i;
		INT nMaxLevel = -1;
		_ITEM_GEMINFO stGemInfo;
		if(byCount != 0)
		{
			for(i = 0; i < byCount; i++)
			{
				stGemInfo = pEquipment->GetEquipGemInfo(i);
				/*INT nGemLevel = GetSerialQual(stGemInfo.m_GemType);
				if(nGemLevel > nMaxLevel)
				{
					nMaxLevel = nGemLevel;*/
					uGemID = stGemInfo.m_GemIndex;
				//}
			}
		}
	}
	return uGemID;
}

VOID Obj_Human::InitBackupAttr()
{
__ENTER_FUNCTION

//	m_SyncTeamMemberInfo.m_Flags
	m_SyncTeamMemberInfo.m_uFamily		= GetMenPai();
	m_SyncTeamMemberInfo.m_uLevel		= GetLevel();
	m_SyncTeamMemberInfo.m_WorldPos.m_fX= getWorldPos()->m_fX;
	m_SyncTeamMemberInfo.m_WorldPos.m_fZ= getWorldPos()->m_fZ;
	m_SyncTeamMemberInfo.m_nHP			= GetHP();
	m_SyncTeamMemberInfo.m_uMaxHP		= GetMaxHP();
	m_SyncTeamMemberInfo.m_nMP			= GetMP();
	m_SyncTeamMemberInfo.m_uMaxMP		= GetMaxMP();
	m_SyncTeamMemberInfo.m_nAnger		= GetRage();
	/*m_SyncTeamMemberInfo.m_WeaponID		= GetEquipID( HEQUIP_WEAPON );
	m_SyncTeamMemberInfo.m_CapID		= GetEquipID( HEQUIP_CAP );
	m_SyncTeamMemberInfo.m_ArmourID		= GetEquipID( HEQUIP_ARMOR );
	m_SyncTeamMemberInfo.m_CuffID		= GetEquipID( HEQUIP_CUFF );
	m_SyncTeamMemberInfo.m_FootID		= GetEquipID( HEQUIP_BOOT );*/
	m_SyncTeamMemberInfo.m_bDeadFlag	= IsDie();



	m_AttrBackUp.m_bNameModified		= FALSE;
	m_AttrBackUp.m_bTitleModified		= FALSE;
	m_AttrBackUp.m_Level				= GetLevel();
	m_AttrBackUp.m_Exp					= GetExp();
	m_AttrBackUp.m_HP					= GetHP();
	m_AttrBackUp.m_MP					= GetMP();
	m_AttrBackUp.m_nPneuma				= GetPneuma();
	m_AttrBackUp.m_MaxHp				= GetMaxHP();
	m_AttrBackUp.m_MaxMp				= GetMaxMP();
	m_AttrBackUp.m_MaxPneuma			= GetMaxPneuma();
	m_AttrBackUp.m_nRage				= GetRage();
	m_AttrBackUp.m_MaxRage				= GetMaxRage();
	m_AttrBackUp.m_nReRageSpeed			= GetRageRegenerate();
	m_AttrBackUp.m_MoveSpeed			= GetMoveSpeed();
	m_AttrBackUp.m_nDataID				= GetDataID();
	m_AttrBackUp.m_nPortraitID			= GetPortraitID();
	m_AttrBackUp.m_nMountID				= GetMountID();
	m_AttrBackUp.m_nModelID				= GetModelID();
	m_AttrBackUp.m_uFaceMeshID			= __GetFaceModel();
	m_AttrBackUp.m_uHairMeshID			= __GetHairModel();
	m_AttrBackUp.m_uHairColor			= __GetHairColor();
	m_AttrBackUp.m_nStealthLevel		= GetStealthLevel();
	m_AttrBackUp.m_nMoodState			= GetMoodState();

	m_AttrBackUp.m_Money				= GetMoney();
	m_AttrBackUp.m_nGoodBadValue		= GetGoodBadValue();
	m_AttrBackUp.m_nStr					= GetStr();
	m_AttrBackUp.m_nCon					= GetCon();
	m_AttrBackUp.m_nInt					= GetInt();
	m_AttrBackUp.m_nDex					= GetDex();
	m_AttrBackUp.m_nPointRemain			= Get_RemainPoints();
	m_AttrBackUp.m_nSkillPointRemain	= Get_RemainSkillPoints();
	for(int i=0; i<COMMON_S_NUMBER; ++i)
	{
		m_AttrBackUp.m_nS_SkillPoints[i] = Get_S_SkillPoints(i);
	}
	
	m_AttrBackUp.m_nReHpSpeed			= GetHPRegenerate();
	m_AttrBackUp.m_nReMpSpeed			= GetMPRegenerate();
	m_AttrBackUp.m_nReVigorSpeed		= GetVigorRegeneRate();
	m_AttrBackUp.m_nAttNear             = Attr_VerifyGeneralAttack(GetAttackNear());
	m_AttrBackUp.m_nAttFar              = Attr_VerifyGeneralAttack(GetAttackFar());

	m_AttrBackUp.m_nAttMagicNear		= Attr_VerifyGeneralAttack(GetAttackMagicNear());
	m_AttrBackUp.m_nAttMagicFar			= Attr_VerifyGeneralAttack(GetAttackMagicFar());

	m_AttrBackUp.m_nDefNear             = Attr_VerifyDefence(GetDefenceNear());
	m_AttrBackUp.m_nDefFar              = Attr_VerifyDefence(GetDefenceFar());

	m_AttrBackUp.m_nDefMagicNear		= Attr_VerifyDefence(GetDefenceMagicNear());
	m_AttrBackUp.m_nDefMagicFar			= Attr_VerifyDefence(GetDefenceMagicFar());

	m_AttrBackUp.m_nToughness			= GetToughness();
	m_AttrBackUp.m_nHit					= Attr_VerifyHitMiss(GetHit());
	m_AttrBackUp.m_nMiss				= Attr_VerifyHitMiss(GetMiss());
	m_AttrBackUp.m_nCritic				= Attr_VerifyCritical(GetCritical());
	m_AttrBackUp.m_nCriticHurt			= Attr_VerifyCritical(GetCriticalHurt());
	m_AttrBackUp.m_nStrikePoint			= GetStrikePoint();
	m_AttrBackUp.m_nAttSpeed			= GetAttackSpeed();

	m_AttrBackUp.m_nAttGold				= Attr_VerifyTraitAttack(GetAttackGold());
	m_AttrBackUp.m_nDefGold				= Attr_VerifyResist(GetDefenceGold());

	m_AttrBackUp.m_nAttWood				= Attr_VerifyTraitAttack(GetAttackWood());
	m_AttrBackUp.m_nDefWood				= Attr_VerifyResist(GetDefenceWood());

	m_AttrBackUp.m_nAttFire				= Attr_VerifyTraitAttack(GetAttackFire());
	m_AttrBackUp.m_nDefFire				= Attr_VerifyResist(GetDefenceFire());

	m_AttrBackUp.m_nAttWater			= Attr_VerifyTraitAttack(GetAttackWater());
	m_AttrBackUp.m_nDefWater			= Attr_VerifyResist(GetDefenceWater());

	m_AttrBackUp.m_nAttSoil				= Attr_VerifyTraitAttack(GetAttackSoil());
	m_AttrBackUp.m_nDefSoil				= Attr_VerifyResist(GetDefenceSoil());

	m_AttrBackUp.m_nNearReduce			= Attr_VerifyGeneralAttack(GetNearAttReduce());		//近程减免
	m_AttrBackUp.m_nFarReduce			= Attr_VerifyGeneralAttack(GetFarAttReduce());		//远程减免

	m_AttrBackUp.m_nMagicNearReduce		= Attr_VerifyGeneralAttack(GetMagicNearAttReduce());//近程内功减免
	m_AttrBackUp.m_nMagicFarReduce		= Attr_VerifyGeneralAttack(GetMagicFarAttReduce());//近程外功减免

	m_AttrBackUp.m_nDreadReduce			= Attr_VerifyGeneralAttack(GetDreadAttResist());		//抵抗恐惧
	m_AttrBackUp.m_nComaReduce			= Attr_VerifyGeneralAttack(GetComaAttResist());		//抵抗昏迷
	m_AttrBackUp.m_nHushReduce			= Attr_VerifyGeneralAttack(GetHushAttResist());		//抵抗沉默
	m_AttrBackUp.m_nUnarmReduce			= Attr_VerifyGeneralAttack(GetUnArmyAttResist());		//抵抗缴械
	m_AttrBackUp.m_nAttSpeedResist		= Attr_VerifyGeneralAttack(GetAttSpeedAttResist());	//抗攻击减速
	m_AttrBackUp.m_nSkillSpeedResist	= Attr_VerifyGeneralAttack(GetSkillSpeedAttResist());//抗施法
	m_AttrBackUp.m_nMoveSpeedResist		= Attr_VerifyGeneralAttack(GetMoveSpeedAttResist());	//抗减速

	m_AttrBackUp.m_nCountry             = GetCountry();  //国家
	m_AttrBackUp.m_nMenpai				= GetMenPai();

	const _CAMP_DATA *pCampData = GetCampData();
	if ( pCampData != NULL )
		m_AttrBackUp.m_CampData			= *pCampData;

	m_AttrBackUp.m_nOwnerID				= GetOwnerID();
	m_AttrBackUp.m_guidCurrentHorse		= GetCurrentHorseGUID();
	
	m_AttrBackUp.m_Vigor				= GetVigor();
	m_AttrBackUp.m_MaxVigor				= GetMaxVigor();
	m_AttrBackUp.m_Energy				= GetEnergy();
	m_AttrBackUp.m_MaxEnergy			= GetMaxEnergy();

	m_AttrBackUp.m_bLimitMove			= IsLimitMove();
	m_AttrBackUp.m_bCanActionFlag1		= CanActionFlag1();
	m_AttrBackUp.m_bCanActionFlag2		= CanActionFlag2();

	m_AttrBackUp.m_bMissionHaveDoneFlagsChanged	= FALSE;

	m_AttrBackUp.m_bMainWeaponModified	= FALSE;		//主手
	m_AttrBackUp.m_bAssiWeaponModified	= FALSE;		//副手
	m_AttrBackUp.m_bCapModified			= FALSE;				//帽子
	m_AttrBackUp.m_bShoulderModified	= FALSE;		//衬肩
	m_AttrBackUp.m_bArmorModified		= FALSE;			//衣服
	m_AttrBackUp.m_bCuffModified		= FALSE;			//护腕
	m_AttrBackUp.m_bBootModified		= FALSE;
	m_AttrBackUp.m_bSuitModified		= FALSE;			// 外装 - 资源表ID
	m_AttrBackUp.m_bRiderModified		= FALSE;			// 座骑 - 资源表ID

	m_AttrBackUp.m_bStallIsOpen			= FALSE;
	m_AttrBackUp.m_bStallNameChanged	= FALSE;

	m_AttrBackUp.m_GuildID				= GetGuildID();

__LEAVE_FUNCTION	
}

VOID Obj_Human::SyncTeamMemberInfo( )
{
__ENTER_FUNCTION

	const TeamInfo* pTeamInfo;

	pTeamInfo = GetTeamInfo();
	if ( pTeamInfo == NULL )
	{ // 出问题了
		Assert(FALSE);
		return;
	}

	if ( pTeamInfo->HasTeam() == FALSE )
	{ // 没有组队
		return;
	}

	if ( pTeamInfo->GetSceneMemberCount() < 1 )
	{ // 没有队友在同场景
		return;
	}

	GCTeamMemberInfo msgTeamMember;

	msgTeamMember.setGUID( GetGUID() );

	if ( m_SyncTeamMemberInfo.m_uFamily != GetMenPai() )
	{
		m_SyncTeamMemberInfo.m_uFamily = GetMenPai();
		msgTeamMember.SetFamily( GetMenPai() );
	}

	if ( m_SyncTeamMemberInfo.m_uLevel != GetLevel() )
	{
		m_SyncTeamMemberInfo.m_uLevel = GetLevel();
		msgTeamMember.SetLevel( GetLevel() );
	}

	if ( m_SyncTeamMemberInfo.m_WorldPos.m_fX != getWorldPos()->m_fX
	  || m_SyncTeamMemberInfo.m_WorldPos.m_fZ != getWorldPos()->m_fZ
	  )
	{
		m_SyncTeamMemberInfo.m_WorldPos.m_fX = getWorldPos()->m_fX;
		m_SyncTeamMemberInfo.m_WorldPos.m_fZ = getWorldPos()->m_fZ;

		msgTeamMember.SetWorldPos( &(m_SyncTeamMemberInfo.m_WorldPos) );
	}

	if ( m_SyncTeamMemberInfo.m_uMaxHP != GetMaxHP() )
	{
		m_SyncTeamMemberInfo.m_uMaxHP = GetMaxHP();
		msgTeamMember.SetMaxHP( GetMaxHP() );
	}

	if ( m_SyncTeamMemberInfo.m_nHP != GetHP() )
	{
		m_SyncTeamMemberInfo.m_nHP = GetHP();
		msgTeamMember.SetHP( GetHP() );
	}

	if ( m_SyncTeamMemberInfo.m_uMaxMP != GetMaxMP() )
	{
		m_SyncTeamMemberInfo.m_uMaxMP = GetMaxMP();
		msgTeamMember.SetMaxMP( GetMaxMP() );
	}

	if ( m_SyncTeamMemberInfo.m_nMP != GetMP() )
	{
		m_SyncTeamMemberInfo.m_nMP = GetMP();
		msgTeamMember.SetMP( GetMP() );
	}

	if ( m_SyncTeamMemberInfo.m_nAnger != GetRage() )
	{
		m_SyncTeamMemberInfo.m_nAnger = GetRage();
		msgTeamMember.SetAnger( GetRage() );
	}

	// 装备刷新
	UINT uWeaponID = GetEquipID( HEQUIP_MAINHAND );
	if ( m_SyncTeamMemberInfo.m_WeaponID != uWeaponID )
	{
		m_SyncTeamMemberInfo.m_WeaponID = uWeaponID;
		msgTeamMember.SetWeaponID( uWeaponID );
	}

	//UINT uAssihandID = GetEquipID( HEQUIP_ASSIHAND );
	//if ( m_SyncTeamMemberInfo.m_AssihandID != uAssihandID )
	//{
	//	m_SyncTeamMemberInfo.m_AssihandID = uAssihandID;
	//	msgTeamMember.SetAssihandID( uAssihandID );
	//}
	
	msgTeamMember.SetAssihandID( GetEquipID( HEQUIP_ASSIHAND ) );

	UINT uCapID = GetEquipID( HEQUIP_HEAD );
	if ( m_SyncTeamMemberInfo.m_CapID != uCapID )
	{
		m_SyncTeamMemberInfo.m_CapID = uCapID;
		msgTeamMember.SetCapID( uCapID );
	}

	//UINT ScapularID = GetEquipID( HEQUIP_SHOULDER );
	//if ( m_SyncTeamMemberInfo.m_ScapularID != ScapularID )
	//{
	//	m_SyncTeamMemberInfo.m_ScapularID = ScapularID;
	//	msgTeamMember.SetScapularID( ScapularID );
	//}
	
	msgTeamMember.SetScapularID(  GetEquipID( HEQUIP_SHOULDER ) );

	UINT uArmourID = GetEquipID( HEQUIP_BACK );
	if ( m_SyncTeamMemberInfo.m_ArmourID != uArmourID )
	{
		m_SyncTeamMemberInfo.m_ArmourID = uArmourID;
		msgTeamMember.SetArmourID( uArmourID );
	}

	UINT uCuffID = GetEquipID( HEQUIP_HAND );
	if ( m_SyncTeamMemberInfo.m_CuffID != uCuffID )
	{
		m_SyncTeamMemberInfo.m_CuffID = uCuffID;
		msgTeamMember.SetCuffID( uCuffID );
	}

	UINT uBootID = GetEquipID( HEQUIP_FEET );
	if ( m_SyncTeamMemberInfo.m_FootID != uBootID )
	{
		m_SyncTeamMemberInfo.m_FootID = uBootID;
		msgTeamMember.SetFootID( uBootID );
	}

	BOOL bDeadFlag = IsDie();
	if ( m_SyncTeamMemberInfo.m_bDeadFlag != bDeadFlag )
	{
		m_SyncTeamMemberInfo.m_bDeadFlag = bDeadFlag;
		msgTeamMember.SetDead( bDeadFlag );
	}

	if ( m_SyncTeamMemberInfo.m_uFaceMeshID != __GetFaceModel() )
	{
		m_SyncTeamMemberInfo.m_uFaceMeshID = __GetFaceModel();
		msgTeamMember.SetFaceModel( __GetFaceModel() );
	}

	if ( m_SyncTeamMemberInfo.m_uHairMeshID != __GetHairModel() )
	{
		m_SyncTeamMemberInfo.m_uHairMeshID = __GetHairModel();
		msgTeamMember.SetHairModel( __GetHairModel() );
	}

	if ( m_SyncTeamMemberInfo.m_uHairColor != __GetHairColor() )
	{
		m_SyncTeamMemberInfo.m_uHairColor = __GetHairColor();
		msgTeamMember.SetHairColor( __GetHairColor() );
	}

	BOOL bMemberInfo = FALSE;
	BOOL bImpactInfo = FALSE;

	if ( msgTeamMember.GetFlags() != 0 )
	{ // 没有需要更新的信息
		bMemberInfo = TRUE;
	}

	// BUFF
	GCCharImpactListUpdate msgMemberImpactList;
	_IMPACT_LIST& rList = Impact_GetImpactList();

	if ( m_SyncTeamMemberInfo.m_SimpleImpactList != rList )
	{
		bImpactInfo = TRUE;
	}

	if ( bImpactInfo )
	{
		m_SyncTeamMemberInfo.m_SimpleImpactList.SetImpactList( &rList );

		msgMemberImpactList.SetOwnerID( GetID() );
		msgMemberImpactList.SetImpactList( rList );
	}


	if ( !bMemberInfo && !bImpactInfo )
	{ // 没有任何需要更新的信息
		return;
	}

	for( INT i=0; i<pTeamInfo->GetSceneMemberCount(); ++i )
	{
		Obj_Human* pRecvHuman;

		pRecvHuman = getScene()->GetHumanManager()->GetHuman( pTeamInfo->GetSceneMemberObjID(i) );

		if ( pRecvHuman != NULL )
		{
			Player* pRecvPlayer = pRecvHuman->GetPlayer();

			if ( pRecvPlayer != NULL )
			{
				if ( bMemberInfo )
				{
					pRecvPlayer->SendPacket( &msgTeamMember );
				}

				if ( bImpactInfo )
				{
					pRecvPlayer->SendPacket( &msgMemberImpactList );
				}
			}
		}
	}

__LEAVE_FUNCTION
}

VOID Obj_Human::SendMsg_RefeshAttrib( VOID )
{
__ENTER_FUNCTION
	INT nAttr = 0;


	BOOL bPublicAttrModified, bEquipmentModified, bDetailAttrModified; 
	bPublicAttrModified		= FALSE;
	bEquipmentModified		= FALSE;
	bDetailAttrModified		= FALSE;

	GCCharBaseAttrib msgPublicAttr;
	GCCharEquipment msgEquipment;
	GCDetailAttrib msgDetail;

	msgPublicAttr.setObjID( GetID() );
	msgDetail.SetObjID( GetID() );

	////设置目标ID用,如果经验和金钱发生改变将使用此属性，否则=0
	//TargetingAndDepletingParams_T& rParams = GetTargetingAndDepletingParams();
	//预设值为0，在具体的属性是否变化的判断时付为合法值
	msgDetail.SetTargetID( 0 );


	if ( m_AttrBackUp.m_bNameModified )
	{
		bPublicAttrModified = TRUE;
		msgPublicAttr.setName( GetName() );
		m_AttrBackUp.m_bNameModified = FALSE;
	}

	if ( m_AttrBackUp.m_bTitleModified )
	{
		bPublicAttrModified = TRUE;
		msgPublicAttr.setCountryTitle( GetCurCountryTitleName() );
        msgPublicAttr.setGuildTitle( GetCurGuildTitleName() );
        msgPublicAttr.setNormalTitle( GetCurNormalTitleName() );
		m_AttrBackUp.m_bTitleModified = FALSE;
	}

	if(m_AttrBackUp.m_MaxHp != GetMaxHP())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_MaxHp = GetMaxHP();

		BYTE yHPPercent = (GetMaxHP())?((BYTE)((GetHP()*100 + GetMaxHP() - 1)/GetMaxHP())):(0);
		//msgPublicAttr.setHPPercent( yHPPercent );
		msgDetail.SetMAXHP( GetMaxHP() );
	}

	if(m_AttrBackUp.m_HP != GetHP())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_HP = GetHP();

		BYTE yHPPercent = (GetMaxHP())?((BYTE)((GetHP()*100 + GetMaxHP() - 1)/GetMaxHP())):(0);
		msgPublicAttr.setHPPercent( yHPPercent );
		msgDetail.SetHP( GetHP() );
	}

	if(m_AttrBackUp.m_MaxMp != GetMaxMP())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_MaxMp = GetMaxMP();

		//BYTE yMPPercent = (GetMaxMP())?((BYTE)((GetMP()*100 + GetMaxMP() - 1)/GetMaxMP())):(0);
		//msgPublicAttr.setMPPercent( yMPPercent );
		msgDetail.SetMAXMP( GetMaxMP() );
	}

	if(m_AttrBackUp.m_MP != GetMP())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_MP = GetMP();

		BYTE yMPPercent = (GetMaxMP())?((BYTE)((GetMP()*100 + GetMaxMP() - 1)/GetMaxMP())):(0);
		msgPublicAttr.setMPPercent( yMPPercent );
		msgDetail.SetMP( GetMP() );
	}

	if(m_AttrBackUp.m_MaxPneuma != GetMaxPneuma())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_MaxPneuma = GetMaxPneuma();
		msgDetail.SetMAXPneuma( GetMaxPneuma() );
	}

	if(m_AttrBackUp.m_nPneuma != GetPneuma())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_nPneuma = GetPneuma();

		BYTE yPneumaPercent = (GetMaxPneuma())?((BYTE)((GetPneuma()*100 + GetMaxPneuma() - 1)/GetMaxPneuma())):(0);
		msgPublicAttr.setPneumaPercent( yPneumaPercent );
		msgDetail.SetPneuma( GetPneuma() );
	}

	if(m_AttrBackUp.m_MaxRage != GetMaxRage())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_MaxRage = GetMaxRage();

		BYTE yRagePercent = (GetMaxRage())?((BYTE)((GetRage()*100 + GetMaxRage() - 1)/GetMaxRage())):(0);
		//msgPublicAttr.setMPPercent( yMPPercent );
		msgDetail.SetMaxRage( GetMaxRage() );
	}

	if(m_AttrBackUp.m_nRage != GetRage())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_nRage = GetRage();

		msgPublicAttr.setRage( GetRage() );
		msgDetail.SetRage( GetRage() );
	}
	
	if(m_AttrBackUp.m_nReRageSpeed != GetRageRegenerate())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_nReRageSpeed = GetRageRegenerate();

		//msgPublicAttr.setRage( GetRageRegenerate() );
		msgDetail.SetRage_ReSpeed( GetRageRegenerate() );
	}

	if(fabs(m_AttrBackUp.m_MoveSpeed-GetMoveSpeed()) > 0.001f)
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_MoveSpeed = GetMoveSpeed();

		msgPublicAttr.setMoveSpeed( GetMoveSpeed() );
		msgDetail.SetMoveSpeed( GetMoveSpeed() );
	}

	const _CAMP_DATA *pCampData = GetCampData();
	if ( pCampData != NULL )
	{
		if(m_AttrBackUp.m_CampData != *pCampData)
		{
			bPublicAttrModified = TRUE;
			m_AttrBackUp.m_CampData = *pCampData;

			msgPublicAttr.setCampData( pCampData );
			msgDetail.SetCampData( pCampData );
		}
	}

	if(m_AttrBackUp.m_nOwnerID != GetOwnerID())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_nOwnerID = GetOwnerID();

		msgPublicAttr.setOwnerID( GetOwnerID() );
		//msgDetail.SetOwnerID( GetOwnerID() );
	}

	if(m_AttrBackUp.m_guidCurrentHorse != GetCurrentHorseGUID())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_guidCurrentHorse = GetCurrentHorseGUID();

		msgDetail.SetCurrentPetGUID( GetCurrentHorseGUID() );
	}

	if(m_AttrBackUp.m_Vigor != GetVigor())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_Vigor = GetVigor() ;

		msgDetail.SetVigor( GetVigor() ) ;
	}

	if(m_AttrBackUp.m_MaxVigor != GetMaxVigor())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_MaxVigor = GetMaxVigor() ;

		msgDetail.SetMaxVigor( GetMaxVigor() ) ;
	}

	if(m_AttrBackUp.m_Energy != GetEnergy())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_Energy = GetEnergy() ;

		msgDetail.SetEnergy( GetEnergy() ) ;
	}

	if(m_AttrBackUp.m_MaxEnergy != GetMaxEnergy())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_MaxEnergy = GetMaxEnergy() ;

		msgDetail.SetMaxEnergy( GetMaxEnergy() ) ;
	}

	if(m_AttrBackUp.m_nDataID != GetDataID())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_nDataID = GetDataID();

		msgPublicAttr.setDataID( GetDataID() );
		msgDetail.SetDataID( GetDataID() );
	}
	
	if(m_AttrBackUp.m_nPortraitID != GetPortraitID())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_nPortraitID = GetPortraitID();

		msgPublicAttr.setPortraitID( GetPortraitID() );
		msgDetail.SetPortraitID( GetPortraitID() );
	}
	
	if(m_AttrBackUp.m_nModelID != GetModelID())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_nModelID = GetModelID();

		msgPublicAttr.setModelID( GetModelID() );
		msgDetail.SetModelID( GetModelID() );
	}
	
	if(m_AttrBackUp.m_nMountID != GetMountID())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_nMountID = GetMountID();

		msgPublicAttr.setMountID( GetMountID() );
		msgDetail.SetMountID( GetMountID() );
	}

	//  [7/23/2010 陈军龙]
	if(m_AttrBackUp.m_nMountIndex != GetMountIndex())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_nMountIndex = GetMountIndex();

		msgPublicAttr.setMountIndex( GetMountIndex() );
		msgDetail.SetMountIndex( GetMountIndex() );
	}

	//msgPublicAttr.setMountID( 0 );
	//msgDetail.SetMountID( 0 );

	if(m_AttrBackUp.m_uHairMeshID != __GetHairModel()
		|| m_AttrBackUp.m_uHairColor != __GetHairColor()
		|| m_AttrBackUp.m_uFaceMeshID != __GetFaceModel())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_uHairMeshID = __GetHairModel();
		m_AttrBackUp.m_uFaceMeshID = __GetFaceModel();
		m_AttrBackUp.m_uHairColor = __GetHairColor();

		msgPublicAttr.setFaceMeshID( __GetFaceModel() );
		msgPublicAttr.setHairMeshID( __GetHairModel() );
		msgPublicAttr.setHairMatrixID( __GetHairColor() );
	}

	if(m_AttrBackUp.m_nStealthLevel != GetStealthLevel())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_nStealthLevel = GetStealthLevel();

		msgPublicAttr.setStealthLevel( GetStealthLevel() );
	}

	if(m_AttrBackUp.m_nMoodState != GetMoodState())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_nMoodState = GetMoodState();

		msgPublicAttr.setMoodState( GetMoodState() );
	}

	if(m_AttrBackUp.m_Level != GetLevel())
	{
		bPublicAttrModified = TRUE;
		m_AttrBackUp.m_Level = GetLevel();

		msgPublicAttr.setLevel( GetLevel() );
		msgDetail.SetLevel( GetLevel() );
	}

	if ( m_AttrBackUp.m_bStallIsOpen != m_StallBox.GetStallIsOpen() )
	{
		//摊位状态发生变化
		bPublicAttrModified = TRUE;

		m_AttrBackUp.m_bStallIsOpen = m_StallBox.GetStallIsOpen();
		msgPublicAttr.setIsOpen( m_StallBox.GetStallIsOpen() );
	}

	if ( m_AttrBackUp.m_bStallNameChanged )
	{
		//摊位名发生变化
		m_AttrBackUp.m_bStallNameChanged = FALSE;
		if(m_StallBox.GetStallIsOpen())
		{
			bPublicAttrModified = TRUE;
			msgPublicAttr.setStallName( m_StallBox.GetStallName() );
		}
	}

	// 如果玩家摊位开市标记变化		20100706 BLL
	if ( m_AttrBackUp.m_bStallIsPublic != m_StallBox.GetStallOpenPublic() )
	{
		bPublicAttrModified = TRUE;

		m_AttrBackUp.m_bStallIsOpen = m_StallBox.GetStallOpenPublic();
		msgPublicAttr.setIsOpenPublic( m_StallBox.GetStallOpenPublic());
	}

	if(m_AttrBackUp.m_bLimitMove != IsLimitMove())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_bLimitMove = IsLimitMove();

		//msgPublicAttr.SetLimitMove( IsLimitMove() );
		//msgPublicAttrAndLevel.SetLimitMove( IsLimitMove() );
		//msgProtectedAttr.SetLimitMove( IsLimitMove() );
		msgDetail.SetLimitMove( IsLimitMove() );
	}

	if(m_AttrBackUp.m_bCanActionFlag1 != CanActionFlag1())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_bCanActionFlag1 = CanActionFlag1();

		msgDetail.SetCanActionFlag1(CanActionFlag1());
	}
	
	if(m_AttrBackUp.m_bCanActionFlag2 != CanActionFlag2())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_bCanActionFlag2 = CanActionFlag2();

		msgDetail.SetCanActionFlag2(CanActionFlag1());
	}

	if(m_AttrBackUp.m_Exp != GetExp())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_Exp = GetExp();
		msgDetail.SetExp( GetExp() );
		//当经验值改变时设置目标ID
		msgDetail.SetTargetID( m_iTargetForExp );
	}

	if(m_AttrBackUp.m_Money != GetMoney())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_Money = GetMoney();
		msgDetail.SetMoney( GetMoney() );
		//当财富改变时设置目标ID
		msgDetail.SetTargetID( m_iTargetForExp );
	}

	if(m_AttrBackUp.m_nGoodBadValue != GetGoodBadValue())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nGoodBadValue = GetGoodBadValue();

		msgDetail.SetGoodBadValue( GetGoodBadValue() );
	}

	if(m_AttrBackUp.m_nMenpai != GetMenPai())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nMenpai = GetMenPai();
		
		msgPublicAttr.setJob(GetMenPai());
		msgDetail.SetMenpai(GetMenPai());
	}

	if(m_AttrBackUp.m_nCountry != GetCountry())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nMenpai = GetCountry();

		msgPublicAttr.setCountry(GetCountry());
		msgDetail.SetCountry(GetCountry());
	}

	if(m_AttrBackUp.m_GuildID != GetGuildID())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_GuildID = GetGuildID();

		msgDetail.SetGuild(GetGuildID());
	}

    if (m_AttrBackUp.m_nGP != GetGuildPoint())
    {
        bDetailAttrModified = TRUE;
        m_AttrBackUp.m_nGP = GetGuildPoint();

        msgDetail.SetGuildPoint(GetGuildPoint());
    }

    if (m_AttrBackUp.m_nPK != GetPKValue())
    {
        bDetailAttrModified = TRUE;
        m_AttrBackUp.m_nPK = GetPKValue();

        msgDetail.SetPKValue(GetPKValue());
    }

    if (m_AttrBackUp.m_nHonor != GetHonor())
    {
        bDetailAttrModified = TRUE;
        m_AttrBackUp.m_nHonor = GetHonor();

        msgDetail.SetHonor(GetHonor());
    }

    if (m_AttrBackUp.m_cFamilyID != GetFamilyID())
    {
        bDetailAttrModified = TRUE;
        m_AttrBackUp.m_cFamilyID = GetFamilyID();

        msgDetail.SetFamilyID(GetFamilyID());
    }

    if (m_AttrBackUp.m_cGuildOfficial != GetGuildOffocial())
    {
        bDetailAttrModified = TRUE;
        m_AttrBackUp.m_cGuildOfficial = GetGuildOffocial();

        msgDetail.SetGuildOfficial(GetGuildOffocial());
    }

	if(m_AttrBackUp.m_nStr != GetStr())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nStr = GetStr();

		msgDetail.SetStr(GetStr());
	}

	/*if(m_AttrBackUp.m_nSpr != GetSpr())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nSpr = GetSpr();

		msgDetail.SetSpr(GetSpr());
	}*/

	if(m_AttrBackUp.m_nCon != GetCon())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nCon = GetCon();

		msgDetail.SetCon(GetCon());
	}

	if(m_AttrBackUp.m_nInt != GetInt())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nInt = GetInt();

		msgDetail.SetInt(GetInt());
	}

	if(m_AttrBackUp.m_nDex != GetDex())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nDex = GetDex();

		msgDetail.SetDex(GetDex());
	}

	if(m_AttrBackUp.m_nPointRemain != Get_RemainPoints())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nPointRemain = Get_RemainPoints();

		msgDetail.SetPoint_Remain( Get_RemainPoints() );
	}
	

	if(m_AttrBackUp.m_nSkillPointRemain != Get_RemainSkillPoints())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nSkillPointRemain = Get_RemainSkillPoints();

		msgDetail.SetSkillPoint_Remain( Get_RemainSkillPoints() );
	}

	for(int i=0; i<COMMON_S_NUMBER; ++i)
	{
		if(m_AttrBackUp.m_nS_SkillPoints[i] != Get_S_SkillPoints(i))
		{
			bDetailAttrModified = TRUE;
			m_AttrBackUp.m_nS_SkillPoints[i] = Get_S_SkillPoints(i);

			msgDetail.SetS_SkillPoint(  Get_S_SkillPoints(i),  i );
		}
	}

	if(m_AttrBackUp.m_nReHpSpeed != GetHPRegenerate())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nReHpSpeed = GetHPRegenerate();

		msgDetail.SetHP_ReSpeed( GetHPRegenerate() );
	}

	if(m_AttrBackUp.m_nReMpSpeed !=	GetMPRegenerate())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nReMpSpeed = GetMPRegenerate();

		msgDetail.SetMP_ReSpeed( GetMPRegenerate() );
	}

	if(m_AttrBackUp.m_nReVigorSpeed !=	GetVigorRegeneRate())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nReVigorSpeed = GetVigorRegeneRate();

		msgDetail.SetVigor_ReSpeed( GetVigorRegeneRate() );
	}
	
	if(m_AttrBackUp.m_nAttNear != GetAttackNear())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nAttNear = GetAttackNear();
		msgDetail.SetAtt_Near(GetAttackNear());
	}

	if(m_AttrBackUp.m_nAttFar != GetAttackFar())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nAttFar = GetAttackFar();
		msgDetail.SetAtt_Far(GetAttackFar());
	}


	if(m_AttrBackUp.m_nAttMagicNear != GetAttackMagicNear())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nAttMagicNear = GetAttackMagicNear();
		msgDetail.SetAtt_MagicNear(GetAttackMagicNear());
	}
	if(m_AttrBackUp.m_nAttMagicFar != GetAttackMagicFar())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nAttMagicFar = GetAttackMagicFar();
		msgDetail.SetAtt_MagicFar(GetAttackMagicFar());
	}

	if(m_AttrBackUp.m_nDefNear != GetDefenceNear())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nDefNear = GetDefenceNear();
		msgDetail.SetDef_Near(GetDefenceNear());
	}

	if(m_AttrBackUp.m_nDefFar != GetDefenceFar())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nDefFar = GetDefenceFar();
		msgDetail.SetDef_Far(GetDefenceFar());
	}

	if(m_AttrBackUp.m_nDefMagicNear != GetDefenceMagicNear())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nDefMagicNear = GetDefenceMagicNear();
		msgDetail.SetDef_MagicNear(GetDefenceMagicNear());
	}
	if(m_AttrBackUp.m_nDefMagicFar != GetDefenceMagicFar())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nDefMagicFar = GetDefenceMagicFar();
		msgDetail.SetDef_MagicFar(GetDefenceMagicFar());
	}

	if(m_AttrBackUp.m_nToughness != GetToughness())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nToughness = GetToughness();
		msgDetail.SetToughness(GetToughness());
	}

	if(m_AttrBackUp.m_nHit != GetHit())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nHit = GetHit();

		msgDetail.SetHit(GetHit());
	}

	if(m_AttrBackUp.m_nMiss != GetMiss())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nMiss = GetMiss();

		msgDetail.SetMiss(GetMiss());
	}
	
	if(m_AttrBackUp.m_nCritic != GetCritical())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nCritic = GetCritical();

		msgDetail.SetCritic(GetCritical());
	}

	if (m_AttrBackUp.m_nCriticHurt != GetCriticalHurt())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nCriticHurt = GetCriticalHurt();

		msgDetail.SetCriticHurt(GetCriticalHurt());
	}

	if(m_AttrBackUp.m_nStrikePoint != GetStrikePoint())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nStrikePoint = GetStrikePoint();

		msgDetail.SetStrikePoint( GetStrikePoint() );
	}

	if(m_AttrBackUp.m_nAttSpeed != GetAttackSpeed())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nAttSpeed = GetAttackSpeed();

		msgPublicAttr.setAttackSpeed( (FLOAT)GetAttackSpeed()/100 );
		msgPublicAttr.setAttackSpeed( (FLOAT)GetAttackSpeed() );
		msgDetail.SetAttSpeed( GetAttackSpeed() );
	}

	if(m_AttrBackUp.m_nAttGold != GetAttackGold())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nAttGold = GetAttackGold();

		msgDetail.SetAttGold(GetAttackGold());
	}
	
	if(m_AttrBackUp.m_nDefGold != GetDefenceGold())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nDefGold = GetDefenceGold();

		msgDetail.SetDefGold(GetDefenceGold());
	}	

	if(m_AttrBackUp.m_nAttWood != GetAttackWood())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nAttWood = GetAttackWood();

		msgDetail.SetAttWood(GetAttackWood());
	}

	if(m_AttrBackUp.m_nDefWood != GetDefenceWood())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nDefWood = GetDefenceWood();

		msgDetail.SetDefWood(GetDefenceWood());
	}

	if(m_AttrBackUp.m_nAttWater != GetAttackWater())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nAttWater = GetAttackWater();

		msgDetail.SetAttWater(GetAttackWater());
	}

	if(m_AttrBackUp.m_nDefWater != GetDefenceWater())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nDefWater = GetDefenceWater();

		msgDetail.SetDefWater(GetDefenceWater());
	}

	if(m_AttrBackUp.m_nAttFire != GetAttackFire())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nAttFire = GetAttackFire();

		msgDetail.SetAttFire(GetAttackFire());
	}

	if(m_AttrBackUp.m_nDefFire != GetDefenceFire())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nDefFire = GetDefenceFire();

		msgDetail.SetDefFire(GetDefenceFire());
	}

	if(m_AttrBackUp.m_nAttSoil != GetAttackSoil())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nAttSoil = GetAttackSoil();

		msgDetail.SetAttSoil(GetAttackSoil());
	}

	if(m_AttrBackUp.m_nDefSoil != GetDefenceSoil())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nDefSoil = GetDefenceSoil();

		msgDetail.SetDefSoil(GetDefenceSoil());
	}

	if(m_AttrBackUp.m_nNearReduce != GetNearAttReduce())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nNearReduce = GetNearAttReduce();

		msgDetail.SetNearReduce(GetNearAttReduce());
	}

	if(m_AttrBackUp.m_nFarReduce != GetFarAttReduce())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nFarReduce = GetFarAttReduce();

		msgDetail.SetFarReduce(GetFarAttReduce());
	}
	
	if(m_AttrBackUp.m_nMagicNearReduce != GetMagicNearAttReduce())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nMagicNearReduce = GetMagicNearAttReduce();

		msgDetail.SetMagicNearReduce(GetMagicNearAttReduce());
	}
	if(m_AttrBackUp.m_nMagicFarReduce != GetMagicFarAttReduce())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nMagicFarReduce = GetMagicFarAttReduce();

		msgDetail.SetMagicFarReduce(GetMagicFarAttReduce());
	}

	if(m_AttrBackUp.m_nDreadReduce != GetDreadAttResist())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nDreadReduce = GetDreadAttResist();

		msgDetail.SetDreadReduce(GetDreadAttResist());
	}

	if(m_AttrBackUp.m_nComaReduce != GetComaAttResist())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nComaReduce = GetComaAttResist();

		msgDetail.SetComaReduce(GetComaAttResist());
	}

	if(m_AttrBackUp.m_nHushReduce != GetHushAttResist())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nHushReduce = GetHushAttResist();

		msgDetail.SetHushReduce(GetHushAttResist());
	}

	if(m_AttrBackUp.m_nUnarmReduce != GetUnArmyAttResist())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nUnarmReduce = GetUnArmyAttResist();

		msgDetail.SetUnarmReduce(GetUnArmyAttResist());
	}
	
	if(m_AttrBackUp.m_nAttSpeedResist != GetAttSpeedAttResist())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nAttSpeedResist = GetAttSpeedAttResist();

		msgDetail.SetAttSpeedResist(GetAttSpeedAttResist());
	}
	
	if(m_AttrBackUp.m_nSkillSpeedResist != GetSkillSpeedAttResist())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nSkillSpeedResist = GetSkillSpeedAttResist();

		msgDetail.SetSkillSpeedResist(GetSkillSpeedAttResist());
	}

	if(m_AttrBackUp.m_nMoveSpeedResist != GetMoveSpeedAttResist())
	{
		bDetailAttrModified = TRUE;
		m_AttrBackUp.m_nMoveSpeedResist = GetMoveSpeedAttResist();

		msgDetail.SetMoveSpeedResist(GetMoveSpeedAttResist());
	}

	// 装备刷新
	if ( m_AttrBackUp.m_bMainWeaponModified )
	{
		bEquipmentModified = TRUE;
		msgEquipment.setMainWeapon( GetEquipID( HEQUIP_MAINHAND ));
		Item *pWeaponItem = GetEquipContain()->GetItem(HEQUIP_MAINHAND);
		m_AttrBackUp.m_bMainWeaponModified = FALSE;
	}
	
	if ( m_AttrBackUp.m_bAssiWeaponModified )
	{
		bEquipmentModified = TRUE;
		msgEquipment.setAssiWeapon( GetEquipID( HEQUIP_ASSIHAND ));
		Item *pWeaponItem = GetEquipContain()->GetItem(HEQUIP_ASSIHAND);
		m_AttrBackUp.m_bAssiWeaponModified = FALSE;
	}

	if ( m_AttrBackUp.m_bCapModified )
	{
		bEquipmentModified = TRUE;
		msgEquipment.setCap( GetEquipID( HEQUIP_HEAD ));
		m_AttrBackUp.m_bCapModified = FALSE;
	}
	
	if ( m_AttrBackUp.m_bShoulderModified )
	{
		bEquipmentModified = TRUE;
		msgEquipment.setShoulder( GetEquipID( HEQUIP_SHOULDER ));
		m_AttrBackUp.m_bShoulderModified = FALSE;
	}

	if ( m_AttrBackUp.m_bArmorModified )
	{
		bEquipmentModified = TRUE;
		msgEquipment.setArmour( GetEquipID( HEQUIP_BACK ));
		m_AttrBackUp.m_bArmorModified = FALSE;
	}

	if ( m_AttrBackUp.m_bCuffModified )
	{
		bEquipmentModified = TRUE;
		msgEquipment.setCuff( GetEquipID( HEQUIP_HAND ));
		m_AttrBackUp.m_bCuffModified = FALSE;
	}

	if ( m_AttrBackUp.m_bBootModified )
	{
		bEquipmentModified = TRUE;
		msgEquipment.setFoot( GetEquipID( HEQUIP_FEET ));
		m_AttrBackUp.m_bBootModified = FALSE;
	}

	if ( m_AttrBackUp.m_bSuitModified )
	{
		bEquipmentModified = TRUE;
		msgEquipment.setSuit( GetEquipID( HEQUIP_SUIT ));
		m_AttrBackUp.m_bSuitModified = FALSE;
	}

	if ( m_AttrBackUp.m_bRiderModified )
	{
		bEquipmentModified = TRUE;
		msgEquipment.setRider( GetEquipID( HEQUIP_RIDER ));
		m_AttrBackUp.m_bBootModified = FALSE;
	}


	if ( bPublicAttrModified )
	{
		if ( bPublicAttrModified )
		{
			m_PublicAttrVersion++;
		}

		if ( bEquipmentModified )
		{
			m_EquipVer++;
		}

		HUMANLIST listHuman;
		getScene()->ScanHuman( getZoneID(), MAX_REFESH_OBJ_ZONE_RADIUS, &(listHuman) );

		BOOL bSendPublicMsg				= bPublicAttrModified;
		BYTE i;
		for ( i = 0; i < listHuman.m_Count; i++ )
		{
			Obj_Human *pTargetHuman = listHuman.m_aHuman[i];
			if ( IsCanViewMe( pTargetHuman ) )
			{
				pTargetHuman->GetPlayer()->SendPacket( &msgPublicAttr );
				
				if ( bEquipmentModified )
				{
					pTargetHuman->GetPlayer()->SendPacket( &msgEquipment );
				}
			}
		}
	}

	bDetailAttrModified |= bPublicAttrModified;
	if ( bDetailAttrModified )
	{
		GetPlayer()->SendPacket(&msgDetail); //发送给自己的详细消息
	}
__LEAVE_FUNCTION
}

VOID Obj_Human::SendCharBaseDirectForHP()
{
	__ENTER_FUNCTION

	GCCharBaseAttrib msgPublicAttr;
	GCDetailAttrib msgDetail;
	msgPublicAttr.setObjID( GetID() );
	BYTE yHPPercent = (GetMaxHP())?((BYTE)((GetHP()*100 + GetMaxHP() - 1)/GetMaxHP())):(0);
	msgPublicAttr.setHPPercent( yHPPercent );

	msgDetail.SetObjID( GetID() );
	msgDetail.SetHP( GetHP() );

	if (GetHP() <= 0)	// add by gh 2010/04/09 
	{ 
		BYTE yMPPercent = (GetMaxMP())?((BYTE)((GetMP()*100 + GetMaxMP() - 1)/GetMaxMP())):(0); 
		BYTE yPneumaPercent = (GetMaxPneuma())?((BYTE)((GetPneuma()*100 + GetMaxPneuma() - 1)/GetMaxPneuma())):(0);
		msgPublicAttr.setMPPercent( yMPPercent );	
		msgPublicAttr.setPneumaPercent( yPneumaPercent );
		msgDetail.SetMP( GetMP() );	
		msgDetail.SetPneuma( GetPneuma() );
	} // end of add

	HUMANLIST listHuman;
	getScene()->ScanHuman( getZoneID(), MAX_REFESH_OBJ_ZONE_RADIUS, &(listHuman) );

	BYTE i;
	for ( i = 0; i < listHuman.m_Count; i++ )
	{
		Obj_Human *pTargetHuman = listHuman.m_aHuman[i];
		if ( IsCanViewMe( pTargetHuman ) )
		{
			pTargetHuman->GetPlayer()->SendPacket( &msgPublicAttr );
		}
	}
	//msgDetail.SetHP( GetHP() );
	GetPlayer()->SendPacket(&msgDetail); //发送给自己的详细消息
	//防止消息重复发送
	m_AttrBackUp.m_HP = GetHP();
	__LEAVE_FUNCTION
}

VOID Obj_Human::AskMsg_DetailAttrib(Obj_Human *pAsker)
{
__ENTER_FUNCTION
	if ( pAsker == NULL )
		return;

	Player *pAskPlayer = pAsker->GetPlayer();

	BOOL bMySelfAsk = (pAsker == this)?(TRUE):(FALSE);

	if(NULL==pAskPlayer)
	{
		return;
	}

	GCDetailAttrib	Msg;

	Msg.SetObjID( GetID() );
	Msg.SetGUID( GetGUID());

	Msg.SetLevel(GetLevel());
	Msg.SetMenpai(GetMenPai() ) ;
	Msg.SetModelID(GetModelID());
	Msg.SetMountID( GetMountID() );
	Msg.SetMountIndex( GetMountIndex() );  //  [7/23/2010 陈军龙]
	if ( bMySelfAsk )
	{
		Msg.SetHP(GetHP());
		Msg.SetMAXHP(GetMaxHP());
		Msg.SetMP(GetMP());
		Msg.SetMAXMP(GetMaxMP());
		Msg.SetPneuma(GetPneuma());
		Msg.SetMAXPneuma(GetMaxPneuma());

		Msg.SetExp(GetExp());

		Msg.SetMoney(GetMoney());
		Msg.SetCurrentPetGUID(GetCurrentHorseGUID() ) ;

		Msg.SetMoveSpeed(GetMoveSpeed());

		Msg.SetStr(GetStr());
		//Msg.SetSpr(GetSpr());
		Msg.SetCon(GetCon());
		Msg.SetInt(GetInt());
		Msg.SetDex(GetDex());
		Msg.SetPoint_Remain(Get_RemainPoints());
		Msg.SetSkillPoint_Remain(Get_RemainSkillPoints());
		for(int i=0; i<COMMON_S_NUMBER; ++i)
		{
			Msg.SetS_SkillPoint(  Get_S_SkillPoints(i),  i );
		}
		Msg.SetHP_ReSpeed(GetHPRegenerate());
		Msg.SetMP_ReSpeed(GetMPRegenerate());
		Msg.SetVigor_ReSpeed(GetVigorRegeneRate());
		//Msg.SetAtt_Physics(Attr_VerifyGeneralAttack(GetAttackPhysics()));

		Msg.SetAtt_Near(Attr_VerifyGeneralAttack(GetAttackNear()));
		Msg.SetAtt_Far(Attr_VerifyGeneralAttack(GetAttackFar()));

		Msg.SetAtt_MagicNear(Attr_VerifyGeneralAttack(GetAttackMagicNear()));		
		Msg.SetAtt_MagicFar(Attr_VerifyGeneralAttack(GetAttackMagicFar()));
		//Msg.SetDef_Physics(Attr_VerifyDefence(GetDefencePhysics()));
		Msg.SetDef_Near(Attr_VerifyDefence(GetDefenceNear()));
		Msg.SetDef_Far(Attr_VerifyDefence(GetDefenceFar()));

		Msg.SetDef_MagicNear(Attr_VerifyDefence(GetDefenceMagicNear()));
		Msg.SetDef_MagicFar(Attr_VerifyDefence(GetDefenceMagicFar()));

		Msg.SetToughness(Attr_VerifyToughness(GetToughness()));
		
		Msg.SetHit(Attr_VerifyHitMiss(GetHit()));
		Msg.SetMiss(Attr_VerifyHitMiss(GetMiss()));
		Msg.SetCritic(Attr_VerifyCritical(GetCritical()));
		Msg.SetCriticHurt(Attr_VerifyCritical(GetCriticalHurt()));
		Msg.SetRage(GetRage());
		Msg.SetMaxRage(GetMaxRage());
		Msg.SetRage_ReSpeed(GetRageRegenerate());
		Msg.SetStrikePoint(GetStrikePoint());
		Msg.SetAttSpeed(GetAttackSpeed());

		Msg.SetAttGold(Attr_VerifyTraitAttack(GetAttackGold()));
		Msg.SetDefGold(Attr_VerifyResist(GetDefenceGold()));

		Msg.SetAttWood(Attr_VerifyTraitAttack(GetAttackWood()));
		Msg.SetDefWood(Attr_VerifyResist(GetDefenceWood()));

		Msg.SetAttWater(Attr_VerifyTraitAttack(GetAttackWater()));
		Msg.SetDefWater(Attr_VerifyResist(GetDefenceWater()));

		Msg.SetAttFire(Attr_VerifyTraitAttack(GetAttackFire()));
		Msg.SetDefFire(Attr_VerifyResist(GetDefenceFire()));

		Msg.SetAttSoil(Attr_VerifyTraitAttack(GetAttackSoil()));
		Msg.SetDefSoil(Attr_VerifyResist(GetDefenceSoil()));

		Msg.SetNearReduce(Attr_VerifyGeneralAttack(GetNearAttReduce()));
		Msg.SetFarReduce(Attr_VerifyGeneralAttack(GetFarAttReduce()));
		Msg.SetMagicNearReduce(Attr_VerifyGeneralAttack(GetMagicNearAttReduce()));
		Msg.SetDreadReduce(Attr_VerifyGeneralAttack(GetDreadAttResist()));
		Msg.SetComaReduce(Attr_VerifyGeneralAttack(GetComaAttResist()));
		Msg.SetHushReduce(Attr_VerifyGeneralAttack(GetHushAttResist()));
		Msg.SetUnarmReduce(Attr_VerifyGeneralAttack(GetUnArmyAttResist()));
		Msg.SetAttSpeedResist(Attr_VerifyGeneralAttack(GetAttSpeedAttResist()));
		Msg.SetSkillSpeedResist(Attr_VerifyGeneralAttack(GetSkillSpeedAttResist()));
		Msg.SetMoveSpeedResist(Attr_VerifyGeneralAttack(GetMoveSpeedAttResist()));
	
		Msg.SetLimitMove(IsLimitMove());
		Msg.SetCanActionFlag1(CanActionFlag1());
		Msg.SetCanActionFlag2(CanActionFlag2());
		Msg.SetGuild(GetGuildID());
        Msg.SetGuildPoint(GetGuildPoint());
        Msg.SetFamilyID(GetFamilyID());
        Msg.SetGuildOfficial(GetGuildOffocial());
		//Msg.SetLimitHandle(IsLimitAllHandle());
		//Msg.SetMissionHaveDoneFlags(pHuman->GetMissionHaveDoneFlags());
		//Msg.SetMissionKillObjectFlags(pHuman->GetMissionKillObjectFlags());
		//Msg.SetMissionEnterAreaFlags(pHuman->GetMissionEnterAreaFlags());
		//Msg.SetMissionItemChangedFlags(pHuman->GetMissionItemChangedFlags());
		Msg.SetEnergy( GetEnergy() ) ;
		Msg.SetMaxEnergy( GetMaxEnergy() ) ;
		Msg.SetVigor( GetVigor() ) ;
		Msg.SetMaxVigor( GetMaxVigor() ) ;
	}
	pAskPlayer->SendPacket( &Msg ) ;

	if ( bMySelfAsk )
	{
		// 可以拾取的任务道具
		//GCCanPickMissionItemList msgCanPick;
		//msgCanPick.SetCanPickMissionItemList( (BYTE)(GetCanPickMissionItemCount()), GetCanPickMissionItemList() );
		//pAskPlayer->SendPacket( &msgCanPick );

		//与此同时，发送角色详细效果列表信息给客户端
		GCDetailImpactListUpdate ImpactListUpdate;
		ImpactListUpdate.SetOwnerID(GetID());
		ImpactListUpdate.SetImpactList(Impact_GetImpactList());
		pAskPlayer->SendPacket(&ImpactListUpdate);
		
		//与此同时，发送角色的冷却列表
		GCCooldownUpdate	Msg;
		CooldownListForHuman_T const* pCooldownList = m_DB.GetCooldownDB();
		Assert(pCooldownList);
		CooldownListForHuman_T const& rCooldownList = *pCooldownList;
		for(INT nIdx=0; rCooldownList.LIST_SIZE>nIdx;++nIdx)
		{
			Cooldown_T const& rCooldownInfo = rCooldownList.GetCooldownInfoByIndex(nIdx);
			if(FALSE == rCooldownInfo.IsCooldowned())
			{
				Msg.CooldownModified(rCooldownInfo);
			}
		}
		pAskPlayer->SendPacket( &Msg ) ;
		// 宠物
		for ( INT i = 0; i < HUMAN_PET_MAX_COUNT; ++i )
		{
			Item* pPetFind = GetPetContain()->GetItem(i);
			PET_GUID_t guidFind = pPetFind->GetPetGUID();
			if ( !guidFind.IsNull() )
			{
				RefeshHorseDetailAttrib(guidFind);
			}
		}
	}
	
__LEAVE_FUNCTION
}

VOID Obj_Human::CalculatePetDetailAttrib(Packets::GCDetailAttrib_Pet& rMsg, Item* pPetItem)
{
	__ENTER_FUNCTION
	if(!pPetItem)
	{
		return ;
	}
	if(pPetItem->GetPetGUID().IsNull())
	{
		return;
	}
	
	rMsg.SetGUID( pPetItem->GetPetGUID() );
	rMsg.SetDataID( pPetItem->GetDataID() );
	rMsg.SetName( pPetItem->GetName() );
	rMsg.SetNickName( pPetItem->GetNick() );
	rMsg.SetHP( pPetItem->GetHP() );
	rMsg.SetAIType(pPetItem->GetAIType());
	rMsg.SetSpouseGUID(pPetItem->GetSpouseGUID());
	rMsg.SetLevel(pPetItem->GetLevel());
	rMsg.SetLife(pPetItem->GetLife());
	rMsg.SetGeneration(pPetItem->GetGeneration());
	rMsg.SetHappiness(pPetItem->GetHappiness());
	rMsg.SetExp(pPetItem->GetExp());
	rMsg.SetStrPerception(pPetItem->GetStrengthPer());
	rMsg.SetConPerception(pPetItem->GetConstitutionPer());
	rMsg.SetDexPerception(pPetItem->GetSmartnessPer());
	rMsg.SetIntPerception(pPetItem->GetMindPer());
	rMsg.SetSex( pPetItem->GetSex() );
	if( pPetItem->IsMating() )
	{
		rMsg.SetHorseState( HorseState_Mating );
	}
	rMsg.SetStr( pPetItem->GetLvl1Attr(CATTR_LEVEL1_STR) );
	rMsg.SetCon( pPetItem->GetLvl1Attr(CATTR_LEVEL1_CON) );
	rMsg.SetDex( pPetItem->GetLvl1Attr(CATTR_LEVEL1_DEX) );
	rMsg.SetInt( pPetItem->GetLvl1Attr(CATTR_LEVEL1_INT) );
	rMsg.SetGenGu( pPetItem->GetGenGu() );
	rMsg.SetHPMax( pPetItem->GetBaseMaxHP() );
	rMsg.SetLevel( pPetItem->GetLevel() );
	rMsg.SetSavvy( pPetItem->GetSavvy() );
	
	rMsg.SetModelID(g_HorseAttrTbl.m_aHorseTbl[pPetItem->GetDataID()].m_iBehaveID);
	rMsg.SetMountID(g_HorseAttrTbl.m_aHorseTbl[pPetItem->GetDataID()].m_iMountID);
	rMsg.SetRemainPoint( pPetItem->GetRemainPoint() );

	if( pPetItem->IsCheckup() )
	{
		rMsg.SetGrowRate( (INT)pPetItem->GetGrowRate() );
	}
	else
	{
		rMsg.SetGrowRate( INVALID_ID );
	}

	_PET_SKILL ownSkill;

	for(INT i = 0; i < PET_MAX_SKILL_COUNT; i++)
	{
		ownSkill	= pPetItem->GetSkill(i);
		rMsg.SetSkill(i, &ownSkill);
	}


	INT nBaseParam = 0;
	INT nValue = 0;
	INT nAttribParam = 0;
	FLOAT fLevelRate = 0;
	FLOAT fAttribRate = 0.f;

	INT nLevel = pPetItem->GetLevel();
	FLOAT fGrowRate = pPetItem->GetGrowRate();

	__LEAVE_FUNCTION
}

VOID Obj_Human::RefeshHorseDetailAttrib(PET_GUID_t guidPet)
{
__ENTER_FUNCTION

	if(guidPet.IsNull()) 
	{
		return;
	}

	Item* pPetItem = GetHorseItem(guidPet);
	
	if(!pPetItem)
	{
		return ;
	}

	GCDetailAttrib_Pet msgDetail;
	CalculatePetDetailAttrib(msgDetail, pPetItem);

	GetPlayer()->SendPacket( &msgDetail );
	////与此同时，发送宠物的冷却列表
	//GCCooldownUpdate Msg;
	//Msg.SetPetGuid(guidPet);
	//CooldownListForHuman_T const* pCooldownList = m_DB.GetCooldownDB();
	//Assert(pCooldownList);
	//CooldownListForHuman_T const& rCooldownList = *pCooldownList;
	//for(INT nIdx=0; rCooldownList.LIST_SIZE>nIdx;++nIdx)
	//{
	//	Cooldown_T const& rCooldownInfo = rCooldownList.GetCooldownInfoByIndex(nIdx);
	//	if(FALSE == rCooldownInfo.IsCooldowned())
	//	{
	//		Msg.CooldownModified(rCooldownInfo);
	//	}
	//}
	//GetPlayer()->SendPacket(&Msg); //发送宠物的冷却信息
__LEAVE_FUNCTION
}

VOID Obj_Human::AskMsg_BaseAttrib( Obj_Human *pTargetHuman )
{
__ENTER_FUNCTION

	if ( IsCanViewMe( pTargetHuman ) )
	{
		GCCharBaseAttrib msgAttr;

		BYTE yHPPercent = (GetMaxHP())?((BYTE)((GetHP()*100 + GetMaxHP() - 1)/GetMaxHP())):(0);
		BYTE yMPPercent = (GetMaxMP())?((BYTE)((GetMP()*100 + GetMaxMP() - 1)/GetMaxMP())):(0);
		BYTE yPneumaPercent = (GetMaxPneuma())?((BYTE)((GetPneuma()*100 + GetMaxPneuma() - 1)/GetMaxPneuma())):(0);

		msgAttr.setObjID( GetID() );
		msgAttr.setJob( (BYTE)(GetMenPai()) );
		msgAttr.setCountry( (BYTE)(GetCountry()) );
		msgAttr.setDataID( (WORD)(GetDataID()) );
		msgAttr.setPortraitID( GetPortraitID() );
//		msgAttr.setFaceMatrixID( GetDB()->GetHumanDB()->m_FaceColor );
		msgAttr.setFaceMeshID( GetDB()->GetHumanDB()->m_FaceModel );
		msgAttr.setHairMatrixID( GetDB()->GetHumanDB()->m_HairColor );
		msgAttr.setHairMeshID( GetDB()->GetHumanDB()->m_HairModel );
		msgAttr.setName( GetName() );

		msgAttr.setCountryTitle( GetCurCountryTitleName() );
        msgAttr.setGuildTitle( GetCurGuildTitleName() );
        msgAttr.setNormalTitle( GetCurNormalTitleName() );

		msgAttr.setHPPercent( yHPPercent );
		msgAttr.setMPPercent( yMPPercent );
		msgAttr.setPneumaPercent( yPneumaPercent );
		msgAttr.setRage( GetRage() );
		msgAttr.setStealthLevel( GetStealthLevel() );
		msgAttr.setMoodState( GetMoodState() );
		if ( GetCampData() != NULL )
			msgAttr.setCampData( GetCampData() );
		msgAttr.setMoveSpeed(GetMoveSpeed());
		msgAttr.setModelID(GetModelID());
		msgAttr.setMountID(GetMountID());
		msgAttr.setMountIndex(GetMountIndex());  //  [7/23/2010 陈军龙]
		
		msgAttr.setAttackSpeed((FLOAT)GetAttackSpeed());
		msgAttr.setIsOpen(m_StallBox.GetStallIsOpen());
		if(m_StallBox.GetStallIsOpen())
		{
			msgAttr.setStallName(m_StallBox.GetStallName());
		}

		// 20100706 BLL
		msgAttr.setIsOpenPublic( m_StallBox.GetStallOpenPublic() );

		msgAttr.setLevel( GetLevel() );

		pTargetHuman->GetPlayer()->SendPacket( &msgAttr );
		//在发送属性数据的同时发送角色的效果列表数据
		GCCharImpactListUpdate ImpactListUpdate;
		ImpactListUpdate.SetOwnerID(GetID());
		ImpactListUpdate.SetImpactList(Impact_GetImpactList());
		pTargetHuman->GetPlayer()->SendPacket(&ImpactListUpdate);
		//队伍相关
		TeamInfo* pTeamInfo = GetTeamInfo();
		if ( pTeamInfo->HasTeam() )
		{ // 是普通队员
			GCNotifyTeamInfo msgTeamFlag;
			msgTeamFlag.SetObjID( GetID() );
			msgTeamFlag.SetHaveTeamFlag( TRUE );
            msgTeamFlag.SetTeamLeaderFlag( pTeamInfo->IsLeader() );
            msgTeamFlag.SetTeamFullFlag( pTeamInfo->IsFull() );
		//	msgTeamFlag.SetTeamFollowFlag( __GetTeamFollowFlag() );

			pTargetHuman->GetPlayer()->SendPacket( &msgTeamFlag );

			if ( pTeamInfo->IsLeader() && __GetTeamFollowFlag() )
			{ // 是队长，发送跟随列表给所有客户端，以供路线模拟
				GCTeamFollowList msgFollowList;
				msgFollowList.SetObjID( GetID() );

				for( INT i=1; i<__GetFollowedMembersCount(); ++i )
				{
					msgFollowList.AddFollowMember( __GetFollowedMember(i)->m_GUID );
				}

				pTargetHuman->GetPlayer()->SendPacket( &msgFollowList );
			}
		}
	}

__LEAVE_FUNCTION
}

VOID Obj_Human::RefreshAttrToWorld(UINT uTime)
{
__ENTER_FUNCTION
	if(m_RefreshAttrToWorldTimer.IsSetTimer() == FALSE){
		m_RefreshAttrToWorldTimer.BeginTimer(g_Config.m_ConfigInfo.m_nRefreshAttrToWorldInterval, g_pTimeManager->CurrentTime());
	}
	else{
		if(m_RefreshAttrToWorldTimer.CountingTimer(uTime)){
			BOOL bNeedRefresh = FALSE;
			GWRefreshDataToWorld* pMsg = (GWRefreshDataToWorld*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_REFRESHDATATOWORLD);
				
			// 只有下面的属性发生变化时才设置人物相关数据
			if(GetDB()->GetToWorldEnumAttrDirty(CATTR_DB_COUNTRY)){
				pMsg->SetCountry(GetDB()->GetHumanDB()->m_nCountry);
				GetDB()->SetToWorldEnumAttrDirty(CATTR_DB_COUNTRY, FALSE);
				bNeedRefresh = TRUE;
			}
			if(GetDB()->GetToWorldEnumAttrDirty(CATTR_DB_MENPAI)){
				pMsg->SetMenPai(GetDB()->GetHumanDB()->m_MenPai);
				GetDB()->SetToWorldEnumAttrDirty(CATTR_DB_MENPAI, FALSE);
				bNeedRefresh = TRUE;
			}
			if(GetDB()->GetToWorldEnumAttrDirty(CATTR_DB_LEVEL)){
				pMsg->SetLevel(GetDB()->GetHumanDB()->m_Level);
				GetDB()->SetToWorldEnumAttrDirty(CATTR_DB_LEVEL, FALSE);
				bNeedRefresh = TRUE;
			}
			if(GetDB()->GetToWorldEnumAttrDirty(CATTR_DB_GUILDID)){
				pMsg->SetGuildID(GetDB()->GetHumanDB()->m_GuildID);
				GetDB()->SetToWorldEnumAttrDirty(CATTR_DB_GUILDID, FALSE);
				bNeedRefresh = TRUE;
			}

			if(GetDB()->GetToWorldEnumAttrDirty(CATTR_DB_MONEY)){
				pMsg->SetMoney(GetDB()->GetHumanDB()->m_Money);
				GetDB()->SetToWorldEnumAttrDirty(CATTR_DB_MONEY, FALSE);
				bNeedRefresh = TRUE;
			}

			if(GetDB()->GetToWorldEnumAttrDirty(CATTR_DB_BANK_MONEY)){
				pMsg->SetBankMoney(GetDB()->GetBankDB()->m_Money);
				GetDB()->SetToWorldEnumAttrDirty(CATTR_DB_BANK_MONEY, FALSE);
				bNeedRefresh = TRUE;
			}

			//if(GetDB()->GetToWorldEnumAttrDirty(CATTR_DB_POSTCODE)){
			//	pMsg->SetStr(GetDB()->GetHumanDB()->m_PostCode);
			//	GetDB()->SetToWorldEnumAttrDirty(CATTR_DB_POSTCODE, FALSE);
			//}
		
			if(bNeedRefresh == TRUE){
				pMsg->SetGUID(GetGUID());
				g_pServerManager->SendPacket(pMsg, INVALID_ID);
			}
			else{
				// 如果没有任何改变就释放分配的Packet
				g_pPacketFactoryManager->RemovePacket(pMsg);
			}
		}
	}
__LEAVE_FUNCTION
}

