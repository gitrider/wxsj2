#include "stdafx.h"
#include "DBCharFullData.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBItemList.h"
#include "DBSkillList.h"
#include "DBXinFaList.h"
#include "DBAbilityList.h"
#include "DBTaskList.h"
#include "DBRelationList.h"
#include "DBImpactList.h"
#include "DBPrivateInfo.h"
#include "DBTitleInfo.h"
#include "DBCoolDownInfo.h"
#include "DBPetList.h"
#include "FoxEncrypt.h"
#include "DBManager.h"
#include "DBSouXiaList.h"


DBCharFullData::DBCharFullData(ODBCInterface* pInterface)
{
	__ENTER_FUNCTION

	mDBName = CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	m_CharGuid		= INVALID_ID;
	m_FullDataCRC	= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
	m_DebugStep		= 0;

	__LEAVE_FUNCTION
}


BOOL DBCharFullData::Load()
{
	__ENTER_FUNCTION

	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(m_CharGuid==INVALID_ID)
	{
		return FALSE;
	}

	pQuery->Parse(LoadCharFullData,CHAR_TABLE,m_CharGuid);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBCharFullData::Save(VOID* pSource)
{

	__ENTER_FUNCTION

	LONG_DB_QUERY* pQuery = GetLongInterQuery();
	
	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(m_CharGuid==INVALID_ID)
	{
		return FALSE;
	}

	FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pSource);
	Assert(pCharFullData);
	


	if(!StrSafeCheck(pCharFullData->m_Human.m_Title,MAX_CHARACTER_TITLE))
		return FALSE;
	if(!StrSafeCheck(pCharFullData->m_Human.m_Passwd,MAX_PWD))
		return FALSE;
	

	CHAR CharCamp[100];
	memset(CharCamp,0,100);
	Binary2String((CHAR*)(&pCharFullData->m_Human.m_CampData),
				   sizeof(_CAMP_DATA),
				   CharCamp);

	CHAR CharSetting[2048*2];
	memset(CharSetting,0,2048*2);
	Binary2String((CHAR*)(&pCharFullData->m_Setting),
				   sizeof(_SETTING_DB_LOAD),
		           CharSetting);
	CHAR CharShopInfo[512];
	memset(CharShopInfo,0,512);


	_HUMAN_DB_LOAD::_PSHOP_INFO	ShopInfo;
	memcpy(ShopInfo.m_ShopGuid,pCharFullData->m_Human.m_ShopGuid,sizeof(_PLAYERSHOP_GUID)*MAX_SHOP_NUM_PER_PLAYER);
	memcpy(ShopInfo.m_FavoriteList,pCharFullData->m_Human.m_FavoriteList,sizeof(_PLAYERSHOP_GUID)*MAX_FAVORITE_SHOPS);

	Binary2String((CHAR*)&ShopInfo,
				 sizeof(ShopInfo),
				 CharShopInfo);

	
	CHAR CharCarryPet[100];
	memset(CharCarryPet,0,100);
	Binary2String((CHAR*)&pCharFullData->m_Human.m_guidCurrentPet,
				  sizeof(PET_GUID_t),
				 CharCarryPet);

	pQuery->Parse(UpdateCharFullData,
				  CHAR_TABLE,

				  pCharFullData->m_Human.m_bFirstEnterScence,
				  pCharFullData->m_Human.m_Title,
				  pCharFullData->m_Human.m_Sex,
				  pCharFullData->m_Human.m_Level,
				  pCharFullData->m_Human.m_Energy,

				  pCharFullData->m_Human.m_Exp,
				  pCharFullData->m_Human.m_Money,
				  pCharFullData->m_Human.m_Passwd,
				  pCharFullData->m_Human.m_HairColor,
				  pCharFullData->m_Human.m_FaceColor,

				  pCharFullData->m_Human.m_HairModel,
				  pCharFullData->m_Human.m_FaceModel,
				  pCharFullData->m_Human.m_StartScene,
				  Float2Int(pCharFullData->m_Human.m_Position.m_fX*100),
				  Float2Int(pCharFullData->m_Human.m_Position.m_fZ*100),

				  pCharFullData->m_Human.m_LastLoginTime,
				  pCharFullData->m_Human.m_LastLogoutTime,
				  pCharFullData->m_Human.m_DBVersion,
				  CharCamp,
				  pCharFullData->m_Human.m_MenPai,

				  pCharFullData->m_Human.m_nCountry,
				  pCharFullData->m_Human.m_HP,
				  pCharFullData->m_Human.m_MP,
				  pCharFullData->m_Human.m_StrikePoint,
				  pCharFullData->m_Human.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_STR],

				  pCharFullData->m_Human.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_CON],
				  pCharFullData->m_Human.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_INT],
				  pCharFullData->m_Human.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_DEX],
				  pCharFullData->m_Human.m_Level1Points,
				  CharSetting,// tmd 太大了 [6/6/2008 wuwenbin]

				  CharShopInfo,
				  CharCarryPet,
				  pCharFullData->m_Human.m_GuildID,
				  pCharFullData->m_Human.m_TeamID,
				  pCharFullData->m_Human.m_PortraitID,

				  pCharFullData->m_Human.m_EnergyRegeneRate,
				  pCharFullData->m_Human.m_Vigor,
				  pCharFullData->m_Human.m_MaxVigor,
				  pCharFullData->m_Human.m_VigorRegeneRate,
				  pCharFullData->m_Human.m_MaxEnergy,

				  pCharFullData->m_Human.m_uPwdDelTime,
				  pCharFullData->m_Human.m_LeftDieTime,
				  pCharFullData->m_Bank.m_Money,
				  pCharFullData->m_Bank.m_CurEndIndex,
				  pCharFullData->m_Human.m_BakScene,

				  Float2Int(pCharFullData->m_Human.m_BakPosition.m_fX*100),
				  Float2Int(pCharFullData->m_Human.m_BakPosition.m_fZ*100),
				  pCharFullData->m_Human.m_Rage,
				  m_CharGuid,
				  pCharFullData->m_Human.m_DBVersion);

	
	if(!ODBCBase::LongSave(pSource))
		return FALSE;
	

	Assert(mResultCount<=1);
	mInterface->Clear();
	if(mResultCount==0)
	{
		return TRUE;
	}
	
	UINT	dbVersion = pCharFullData->m_Human.m_DBVersion;

	//保存装备
	DBItemList	ItemListObject(mInterface);
	ItemListObject.SetCharGuid(m_CharGuid);
	ItemListObject.SetDBVersion(dbVersion);
	if(!ItemListObject.Save(pCharFullData))
		return FALSE;
	ItemListObject.ParseResult(pCharFullData);

	//保存技能
	DBSkillList	SkillListObject(mInterface);
	SkillListObject.SetCharGuid(m_CharGuid);
	SkillListObject.SetDBVersion(dbVersion);
	if(!SkillListObject.Save(pCharFullData))
		return FALSE;
	SkillListObject.ParseResult(pCharFullData);

	//保存心法
	DBXinFaList	XinFaListObject(mInterface);
	XinFaListObject.SetCharGuid(m_CharGuid);
	XinFaListObject.SetDBVersion(dbVersion);
	if(!XinFaListObject.Save(pCharFullData))
		return FALSE;
	XinFaListObject.ParseResult(pCharFullData);

	//保存生活技能
	DBAbilityList AbilityListObject(mInterface);
	AbilityListObject.SetCharGuid(m_CharGuid);
	AbilityListObject.SetDBVersion(dbVersion);
	if(!AbilityListObject.Save(pCharFullData))
		return FALSE;
	AbilityListObject.ParseResult(pCharFullData);

	//保存任务信息
	DBTaskList	TaskListObject(mInterface);
	TaskListObject.SetCharGuid(m_CharGuid);
	TaskListObject.SetDBVersion(dbVersion);
	if(!TaskListObject.Save(pCharFullData))
		return FALSE;
	TaskListObject.ParseResult(pCharFullData);

	//保存关系列表
	DBRelationList	RelationListObject(mInterface);
	RelationListObject.SetCharGuid(m_CharGuid);
	RelationListObject.SetDBVersion(dbVersion);
	if(!RelationListObject.Save(pCharFullData))
		return FALSE;
	RelationListObject.ParseResult(pCharFullData);

	//保存Impact 列表
	DBImpactList	ImpactListObject(mInterface);
	ImpactListObject.SetCharGuid(m_CharGuid);
	ImpactListObject.SetDBVersion(dbVersion);
	if(!ImpactListObject.Save(pCharFullData))
		return FALSE;
	ImpactListObject.ParseResult(pCharFullData);

	////保存宠物	列表
	//DBPetList	PetListObject(mInterface);
	//PetListObject.SetCharGuid(m_CharGuid);
	//PetListObject.SetDBVersion(dbVersion);
	//if(!PetListObject.Save(pCharFullData))
	//	return FALSE;
	//PetListObject.ParseResult(pCharFullData);

	//私人信息
	DBPrivateInfo	PrivateInfoObject(mInterface);
	PrivateInfoObject.SetCharGuid(m_CharGuid);
	PrivateInfoObject.SetDBVersion(dbVersion);
	if(!PrivateInfoObject.Save(pCharFullData))
		return FALSE;
	PrivateInfoObject.ParseResult(pCharFullData);

	//称号信息
	DBTitleInfo		TitleInfoObject(mInterface);
	TitleInfoObject.SetCharGuid(m_CharGuid);
	TitleInfoObject.SetDBVersion(dbVersion);
	if(!TitleInfoObject.Save(pCharFullData))
		return FALSE;
	TitleInfoObject.ParseResult(pCharFullData);

	//冷却信息
	DBCoolDownInfo		CoolDownInfoObject(mInterface);
	CoolDownInfoObject.SetCharGuid(m_CharGuid);
	CoolDownInfoObject.SetDBVersion(dbVersion);
	if(!CoolDownInfoObject.Save(pCharFullData))
		return FALSE;
	CoolDownInfoObject.ParseResult(pCharFullData);

	//搜侠信息add by gh 2010/06/07
	DBSouXiaList		SouXiaInfoObject(mInterface);
	SouXiaInfoObject.SetCharGuid(m_CharGuid);
	SouXiaInfoObject.SetDBVersion(dbVersion);
	if(!SouXiaInfoObject.Save(pCharFullData))
		return FALSE;
	SouXiaInfoObject.ParseResult(pCharFullData);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBCharFullData::ParseResult(VOID* pResult)
{

	__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{

			FULLUSERDATA* pCharFullData = static_cast<FULLUSERDATA*>(pResult);
			Assert(pCharFullData);

			pCharFullData->CleanUp();
			enum 
			{
				DB_FirstLogin = 1,
				DB_CharGuid,
				DB_CharName,
				DB_Title,
				DB_Sex,

				DB_CreateTime,
				DB_Level,
				DB_Enegry,
				DB_Exp,
				DB_Money,

				DB_Pw,
				DB_HairColor,
				DB_FaceColor,
				DB_HairModel,
				DB_FaceModel,

				DB_Scene,
				DB_XPos,
				DB_ZPos,
				DB_LoginTime,
				DB_LogoutTime,

				DB_Version,
				DB_Camp,
				DB_Menpai,
				DB_Country,
				DB_HP,

				DB_MP,
				DB_StrikePoint,
				DB_Str,
				DB_Con,
				DB_Ipr,

				DB_Dex,
				DB_Points,
				DB_Setting,
				DB_ShopInfo,
				DB_CarryPet,

				DB_GuildID,
				DB_TeamID,
				DB_HeadID,
				DB_eRecover,
				DB_Vigor,

				DB_MaxVigor,
				DB_VRecover,
				DB_EnergyMax,
				DB_PwdelTime,
				DB_DieTime,

				DB_BankMoney,
				DB_BankEnd,
				DB_BackScene,
				DB_BackXPos,
				DB_BackZPos,

				DB_Rage,
			};
			
			Assert(mResultCount<=1);
			Assert(mInterface);
			INT	   ErrorCode;

			//加载基本属性
			for(INT i =0;i<1;i++)
			{
				if(!mInterface->Fetch())
					break;

				//加载角色基本属性
				pCharFullData->m_Human.m_bFirstEnterScence	=	(1 == mInterface->GetInt(DB_FirstLogin,ErrorCode) );
				pCharFullData->m_Human.m_GUID	=	mInterface->GetUInt(DB_CharGuid,ErrorCode);
				mInterface->GetString(DB_CharName,pCharFullData->m_Human.m_Name,MAX_CHARACTER_NAME,ErrorCode);
				mInterface->GetString(DB_Title,pCharFullData->m_Human.m_Title,MAX_CHARACTER_TITLE,ErrorCode);
				pCharFullData->m_Human.m_Sex	=	mInterface->GetUInt(DB_Sex,ErrorCode);

				pCharFullData->m_Human.m_CreateDate	=	mInterface->GetUInt(DB_CreateTime,ErrorCode);
				pCharFullData->m_Human.m_Level	=	mInterface->GetUInt(DB_Level,ErrorCode);
				pCharFullData->m_Human.m_Energy	=	mInterface->GetUInt(DB_Enegry,ErrorCode);
				pCharFullData->m_Human.m_Exp	=	mInterface->GetUInt(DB_Exp,ErrorCode);
				pCharFullData->m_Human.m_Money	=	mInterface->GetUInt(DB_Money,ErrorCode);

				mInterface->GetString(DB_Pw,pCharFullData->m_Human.m_Passwd,MAX_PWD,ErrorCode);
				pCharFullData->m_Human.m_HairColor	=	mInterface->GetUInt(DB_HairColor,ErrorCode);
				pCharFullData->m_Human.m_FaceColor	=	mInterface->GetBYTE(DB_FaceColor,ErrorCode);
				pCharFullData->m_Human.m_HairModel	=	mInterface->GetBYTE(DB_HairModel,ErrorCode);
				pCharFullData->m_Human.m_FaceModel	=	mInterface->GetBYTE(DB_FaceModel,ErrorCode);

				pCharFullData->m_Human.m_StartScene		=	mInterface->GetUInt(DB_Scene,ErrorCode);
				pCharFullData->m_Human.m_Position.m_fX	=	mInterface->GetUInt(DB_XPos,ErrorCode)*0.01f;
				pCharFullData->m_Human.m_Position.m_fZ	=	mInterface->GetUInt(DB_ZPos,ErrorCode)*0.01f;
				pCharFullData->m_Human.m_LastLoginTime	=	mInterface->GetUInt(DB_LoginTime,ErrorCode);
				pCharFullData->m_Human.m_LastLogoutTime	=	mInterface->GetUInt(DB_LogoutTime,ErrorCode);

				pCharFullData->m_Human.m_DBVersion		=	mInterface->GetUInt(DB_Version,ErrorCode);
				mInterface->GetField(DB_Camp, (CHAR*)(&pCharFullData->m_Human.m_CampData), sizeof(_CAMP_DATA), ErrorCode);
				pCharFullData->m_Human.m_MenPai	=	mInterface->GetUInt(DB_Menpai,ErrorCode);
				pCharFullData->m_Human.m_nCountry =	mInterface->GetUInt(DB_Country,ErrorCode);
				pCharFullData->m_Human.m_HP	=	mInterface->GetUInt(DB_HP,ErrorCode);

				pCharFullData->m_Human.m_MP	=	mInterface->GetUInt(DB_MP,ErrorCode);
				pCharFullData->m_Human.m_StrikePoint	=	mInterface->GetUInt(DB_StrikePoint,ErrorCode);
				pCharFullData->m_Human.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_STR]	=	mInterface->GetUInt(DB_Str,ErrorCode);
				pCharFullData->m_Human.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_CON]	=	mInterface->GetUInt(DB_Con,ErrorCode);
				pCharFullData->m_Human.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_INT]	=	mInterface->GetUInt(DB_Ipr,ErrorCode);

				pCharFullData->m_Human.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_DEX]	=	mInterface->GetUInt(DB_Dex,ErrorCode);
				pCharFullData->m_Human.m_Level1Points	=	mInterface->GetUInt(DB_Points,ErrorCode);
				mInterface->GetField(DB_Setting, (CHAR*)&pCharFullData->m_Setting, sizeof(_SETTING_DB_LOAD),ErrorCode);
				_HUMAN_DB_LOAD::_PSHOP_INFO	ShopInfo;
				mInterface->GetField(DB_ShopInfo, (CHAR*)&ShopInfo,sizeof(ShopInfo),ErrorCode);
				memcpy(pCharFullData->m_Human.m_ShopGuid,ShopInfo.m_ShopGuid,sizeof(_PLAYERSHOP_GUID)*MAX_SHOP_NUM_PER_PLAYER);
				memcpy(pCharFullData->m_Human.m_FavoriteList,ShopInfo.m_FavoriteList,sizeof(_PLAYERSHOP_GUID)*MAX_FAVORITE_SHOPS);
				mInterface->GetField(DB_CarryPet,(CHAR*)&pCharFullData->m_Human.m_guidCurrentPet, sizeof(PET_GUID_t),ErrorCode);
				
				pCharFullData->m_Human.m_GuildID	=	mInterface->GetUInt(DB_GuildID,ErrorCode);
				pCharFullData->m_Human.m_TeamID		=	mInterface->GetUInt(DB_TeamID,ErrorCode);
				pCharFullData->m_Human.m_PortraitID	=	mInterface->GetUInt(DB_HeadID,ErrorCode);
				pCharFullData->m_Human.m_EnergyRegeneRate	=	mInterface->GetInt(DB_eRecover,ErrorCode);	
				pCharFullData->m_Human.m_Vigor				=	mInterface->GetInt(DB_Vigor,ErrorCode);					

				pCharFullData->m_Human.m_MaxVigor			=  mInterface->GetInt(DB_MaxVigor,ErrorCode);
				pCharFullData->m_Human.m_VigorRegeneRate	=	mInterface->GetInt(DB_VRecover,ErrorCode);
				pCharFullData->m_Human.m_MaxEnergy	=	mInterface->GetInt(DB_EnergyMax,ErrorCode);
				pCharFullData->m_Human.m_uPwdDelTime	=	mInterface->GetUInt(DB_PwdelTime,ErrorCode);
				pCharFullData->m_Human.m_LeftDieTime	=	mInterface->GetUInt(DB_DieTime,ErrorCode);

				pCharFullData->m_Bank.m_Money	=	mInterface->GetUInt(DB_BankMoney,ErrorCode);
				pCharFullData->m_Bank.m_CurEndIndex	=	mInterface->GetUInt(DB_BankEnd,ErrorCode);
				pCharFullData->m_Human.m_BakScene	=	mInterface->GetUInt(DB_BackScene,ErrorCode);
				pCharFullData->m_Human.m_BakPosition.m_fX = mInterface->GetUInt(DB_BackXPos,ErrorCode)*0.01f;
				pCharFullData->m_Human.m_BakPosition.m_fZ = mInterface->GetUInt(DB_BackZPos,ErrorCode)*0.01f;

				pCharFullData->m_Human.m_Rage			  = mInterface->GetUInt(DB_Rage,ErrorCode);

				mInterface->Clear();

				UINT dbVersion = pCharFullData->m_Human.m_DBVersion;
				
				//加载装备
				DBItemList	ItemListObject(mInterface);
				ItemListObject.SetCharGuid(m_CharGuid);
				ItemListObject.SetDBVersion(dbVersion);
				if(!ItemListObject.Load())
					return FALSE;

				ItemListObject.ParseResult(pCharFullData);

				//加载技能
				DBSkillList	SkillListObject(mInterface);
				SkillListObject.SetCharGuid(m_CharGuid);
				SkillListObject.SetDBVersion(dbVersion);
				if(!SkillListObject.Load())
					return FALSE;
				SkillListObject.ParseResult(pCharFullData);

				//加载心法
				DBXinFaList	XinFaListObject(mInterface);
				XinFaListObject.SetCharGuid(m_CharGuid);
				XinFaListObject.SetDBVersion(dbVersion);
				if(!XinFaListObject.Load())
					return FALSE;
				XinFaListObject.ParseResult(pCharFullData);

				//加载生活技能
				DBAbilityList AbilityListObject(mInterface);
				AbilityListObject.SetCharGuid(m_CharGuid);
				AbilityListObject.SetDBVersion(dbVersion);
				if(!AbilityListObject.Load())
					return FALSE;
				AbilityListObject.ParseResult(pCharFullData);

				//加载任务信息
				DBTaskList	TaskListObject(mInterface);
				TaskListObject.SetCharGuid(m_CharGuid);
				TaskListObject.SetDBVersion(dbVersion);
				if(!TaskListObject.Load())
					return FALSE;

				TaskListObject.ParseResult(pCharFullData);

				//加载关系列表
				DBRelationList	RelationListObject(mInterface);
				RelationListObject.SetCharGuid(m_CharGuid);
				RelationListObject.SetDBVersion(dbVersion);
				if(!RelationListObject.Load())
					return FALSE;
				RelationListObject.ParseResult(pCharFullData);

				//Impact 列表
				DBImpactList	ImpactListObject(mInterface);
				ImpactListObject.SetCharGuid(m_CharGuid);
				ImpactListObject.SetDBVersion(dbVersion);
				if(!ImpactListObject.Load())
					return FALSE;
				ImpactListObject.ParseResult(pCharFullData);
				
				////宠物
				//DBPetList	PetListObject(mInterface);
				//PetListObject.SetCharGuid(m_CharGuid);
				//PetListObject.SetDBVersion(dbVersion);
				//if(!PetListObject.Load())
				//	return FALSE;
				//PetListObject.ParseResult(pCharFullData);
				
				//个人信息
				DBPrivateInfo PrivateInfoObject(mInterface);
				PrivateInfoObject.SetCharGuid(m_CharGuid);
				PrivateInfoObject.SetDBVersion(dbVersion);
				if(!PrivateInfoObject.Load())
					return FALSE;
				PrivateInfoObject.ParseResult(pCharFullData);

				//Title
				DBTitleInfo	TitleInfoObject(mInterface);
				TitleInfoObject.SetCharGuid(m_CharGuid);
				TitleInfoObject.SetDBVersion(dbVersion);
				if(!TitleInfoObject.Load())
					return FALSE;
				TitleInfoObject.ParseResult(pCharFullData);

				//冷却时间
				DBCoolDownInfo	CoolDownInfoObject(mInterface);
				CoolDownInfoObject.SetCharGuid(m_CharGuid);
				CoolDownInfoObject.SetDBVersion(dbVersion);
				if(!CoolDownInfoObject.Load())
					return FALSE;
				CoolDownInfoObject.ParseResult(pCharFullData);

				//捜侠录信息add by gh 2010/06/07
				DBSouXiaList	SouXiaListObject(mInterface);
				SouXiaListObject.SetCharGuid(m_CharGuid);
				SouXiaListObject.SetDBVersion(dbVersion);
				if(!SouXiaListObject.Load())
					return FALSE;
				SouXiaListObject.ParseResult(pCharFullData);
		
			}
		}
		break;
	case DB_SAVE:
		{
			
		}
		break;
	default:
		break;
	}


	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

GUID_t	DBCharFullData::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBCharFullData::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

VOID	DBCharFullData::SetCRC(UINT uCRC)
{
	m_FullDataCRC = uCRC;
}
UINT	DBCharFullData::GetCRC()
{
	return m_FullDataCRC;
}

INT FullDataMagicInt1(FULLUSERDATA* pData)
{
	Assert(pData);

	GUID_t	guid		= pData->m_Human.m_GUID;
	UINT	money		= pData->m_Human.m_Money;
	INT		bankmoney	= pData->m_Bank.m_Money;
	INT		yuanbao		= 0;//pData->m_Human.m_iYuanBao;
	UINT	logouttime	= pData->m_Human.m_LastLogoutTime;
	return 0;
	//return (INT)CRC_Get(guid,money,bankmoney,yuanbao,logouttime);
}

INT FullDataMagicInt2(FULLUSERDATA* pData)
{
	Assert(pData);

	GUID_t	guid		= pData->m_Human.m_GUID;
	UINT	money		= pData->m_Human.m_Money;
	INT		bankmoney	= pData->m_Bank.m_Money;
	INT		yuanbao		= 0;//pData->m_Human.m_iYuanBao;
	INT		level		= pData->m_Human.m_Level;
	return 0;
	//return (INT)CRC_Get(guid,money,bankmoney,yuanbao,level);
}
