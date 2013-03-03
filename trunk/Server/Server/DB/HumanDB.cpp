
#include "stdafx.h"
#include "HumanDB.h"
#include "ServerManager.h"
#include "SMUManager.h"
#include "ShareMemManager.h"
#include "DB_Struct.h"

HumanDB::HumanDB( )
{

__ENTER_FUNCTION
	
	BOOL bEnableShareMem = g_pServerManager->GetCurrentServerInfo()->m_EnableShareMem;

	if(bEnableShareMem)
	{
		HumanSMU* pSMU =	g_HumanSMUPool.NewObj();
		Assert(pSMU);
		m_HumanSM		=	pSMU;
		m_AttrRegSM		=	new HUMAN_DB_ATTR_REG;

	}
	else
	{
		m_HumanSM		=	0;
		m_AttrRegSM		=	0;
	}
	
	m_AttrReg		=	new HUMAN_DB_ATTR_REG;
	m_AttrFlag		=	new HUMAN_DB_ATTR_FLAG;
	m_AttrFlag_ToWorld = new HUMAN_DB_ATTR_FLAG;
	m_dbHuman		=	new _HUMAN_DB_LOAD ;
	m_dbEquip		=	new _EQUIP_DB_LOAD ;
	m_dbBag			=	new _BAG_DB_LOAD ;
	m_dbBank		=	new _BANK_DB_LOAD ;
	m_dbSkill		=	new _SKILL_DB_LOAD ;
	m_dbCooldown	=	new _COOLDOWN_DB_LOAD_FOR_HUMAN;
	m_dbXinFa		=	new _XINFA_DB_LOAD ;
	m_dbImpact		=	new _IMPACT_DB_LOAD ;
	m_dbAbility		=	new _ABILITY_DB_LOAD ;
	m_dbMission		=	new _MISSION_DB_LOAD ;
	m_dbSetting		=	new _SETTING_DB_LOAD ;
	m_dbPetList		=	new _PET_DB_LIST_LOAD ;
	m_dbRelation	=	new _RELATION_DB_LOAD ;
	m_dbPrivateInfo	=	new _PRIVATE_INFO_DB_LOAD;
	m_dbMonsterPet	=	new _MONSTER_PET_DB;
	m_dbSouXia		=	new _SOUXIA_DB_LOAD;
	m_dbMount       =   new _MOUNT_DB_LOAD;  // [7/8/2010 ³Â¾üÁú]

	_RegisterDBAttributes();
	
	if(bEnableShareMem)
	{
		_RegisterSMAttributes();
	}

__LEAVE_FUNCTION
}

HumanDB::~HumanDB( )
{
__ENTER_FUNCTION
	
	SAFE_DELETE(m_AttrRegSM);
	SAFE_DELETE(m_AttrReg);
	SAFE_DELETE(m_AttrFlag);
	SAFE_DELETE(m_AttrFlag_ToWorld);
	SAFE_DELETE(m_dbHuman) ;
	SAFE_DELETE(m_dbEquip) ;
	SAFE_DELETE(m_dbBag) ;
	SAFE_DELETE(m_dbBank) ;
	SAFE_DELETE(m_dbSkill) ;
	SAFE_DELETE(m_dbCooldown) ;
	SAFE_DELETE(m_dbXinFa) ;
	SAFE_DELETE(m_dbImpact) ;
	SAFE_DELETE(m_dbAbility) ;
	SAFE_DELETE(m_dbMission) ;
	SAFE_DELETE(m_dbSetting) ;
	SAFE_DELETE(m_dbPetList) ;
	SAFE_DELETE(m_dbRelation) ;
	SAFE_DELETE(m_dbPrivateInfo);
	SAFE_DELETE(m_dbMonsterPet);
	SAFE_DELETE(m_dbSouXia);
	SAFE_DELETE(m_dbMount);  // [7/8/2010 ³Â¾üÁú]

__LEAVE_FUNCTION
}

VOID HumanDB::CleanUp( )
{
__ENTER_FUNCTION

BOOL bEnableShareMem = g_pServerManager->GetCurrentServerInfo()->m_EnableShareMem;

if(!bEnableShareMem)
{
	m_AttrReg->CleanUp();
	m_AttrFlag->CleanUp();
	m_AttrFlag_ToWorld->CleanUp();
	m_dbHuman->CleanUp( ) ;
	m_dbEquip->CleanUp( ) ;
	m_dbBag->CleanUp( ) ;
	m_dbBank->CleanUp( ) ;
	m_dbSkill->CleanUp( ) ;
	m_dbCooldown->CleanUp();
	m_dbXinFa->CleanUp( ) ;
	m_dbImpact->CleanUp( ) ;
	m_dbAbility->CleanUp( ) ;
	m_dbMission->CleanUp( ) ;
	m_dbSetting->CleanUp( ) ;
	m_dbPetList->CleanUp( ) ;
	m_dbRelation->CleanUp( ) ;
	m_dbPrivateInfo->CleanUp();
	m_dbMonsterPet->CleanUp();
	m_dbSouXia->CleanUp();
	m_dbMount->CleanUp();  // [7/8/2010 ³Â¾üÁú]
}

__LEAVE_FUNCTION
}


VOID HumanDB::ValidateShareMem(BOOL bForceAll,BOOL& bUpdateAttr)
{
	 INT iAttr;

	__ENTER_FUNCTION
		
		if(m_HumanSM)
		{
			Lock();
			
			for(iAttr = CATTR_DB_HP;iAttr< CATTR_DB_NUMBER;iAttr++)
			{
				if((IsNoDirectAttr((CHAR_ATTR_DB)iAttr)&& (bUpdateAttr != FALSE)) ||bForceAll)
				{
					memcpy(GetSMEnumAttr((CHAR_ATTR_DB)iAttr),
						GetDBEnumAttr((CHAR_ATTR_DB)iAttr),
						GetDBEnumAttrSize((CHAR_ATTR_DB)iAttr));
					SetDBEnumAttrDirty((CHAR_ATTR_DB)iAttr,FALSE);

				}
				else if(GetDBEnumAttrDirty((CHAR_ATTR_DB)iAttr)||bForceAll)
				{
					memcpy(GetSMEnumAttr((CHAR_ATTR_DB)iAttr),
						   GetDBEnumAttr((CHAR_ATTR_DB)iAttr),
						   GetDBEnumAttrSize((CHAR_ATTR_DB)iAttr));
					SetDBEnumAttrDirty((CHAR_ATTR_DB)iAttr,FALSE);
				}
			}

			UnLock();

			bUpdateAttr = FALSE;
		}



	__LEAVE_FUNCTION
}
BOOL	HumanDB::IsNoDirectAttr(CHAR_ATTR_DB attrType)
{
	__ENTER_FUNCTION
	
		switch(attrType) 
		{
		case CATTR_DB_COOLDOWN:
		case CATTR_DB_IMPACT:
		case CATTR_DB_RELATION:
		case CATTR_DB_PRIVATEINFO:
		case CATTR_DB_AT_POSITION:
			return TRUE;
			break;
		
		default:
			return FALSE;
		}

		return FALSE;
	__LEAVE_FUNCTION
	
	return FALSE;
}

VOID HumanDB::ValidatePlayerID(PlayerID_t PID)
{
	if(m_HumanSM)
	{
		m_HumanSM->SetPlayerID(SM_S_WRITE,PID);
	}
}
VOID HumanDB::RandSet( )
{
__ENTER_FUNCTION

	INT iRand = 10000+rand() ;
	m_dbHuman->m_HP = iRand ;
	sprintf( m_dbHuman->m_Name, "²âÊÔ%d", iRand ) ;


__LEAVE_FUNCTION
}

BOOL HumanDB::InitHumanDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL HumanDB::InitEquipDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL HumanDB::InitBagDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL HumanDB::InitBankDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL HumanDB::InitSkillDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL HumanDB::InitCooldownDB( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}


VOID	HumanDB::_RegisterDBAttributes()
{

#define REG_DB_ATTR(type,var)						_RegDBAttr(type,&(var),sizeof(var));
#define REG_DB_ATTR_SIZE(type,varPtr,size)			_RegDBAttr(type,varPtr,size);



__ENTER_FUNCTION

	REG_DB_ATTR(CATTR_DB_HP,				m_dbHuman->m_HP);
	REG_DB_ATTR(CATTR_DB_MP,				m_dbHuman->m_MP);
	REG_DB_ATTR(CATTR_DB_STRIKEPOINT,		m_dbHuman->m_StrikePoint);
	REG_DB_ATTR(CATTR_DB_VIGOR,				m_dbHuman->m_Vigor);
	REG_DB_ATTR(CATTR_DB_MAX_VIGOR,			m_dbHuman->m_MaxVigor);
	REG_DB_ATTR(CATTR_DB_VIGOR_REGENE_RATE,	m_dbHuman->m_VigorRegeneRate);
	REG_DB_ATTR(CATTR_DB_ENERGY,			m_dbHuman->m_Energy);
	REG_DB_ATTR(CATTR_DB_MAX_ENERGY,		m_dbHuman->m_MaxEnergy);
	REG_DB_ATTR(CATTR_DB_ENERGY_REGENE_RATE,m_dbHuman->m_EnergyRegeneRate);
	REG_DB_ATTR(CATTR_DB_RAGE,				m_dbHuman->m_Rage);
	REG_DB_ATTR(CATTR_DB_LEVEL,				m_dbHuman->m_Level);
	REG_DB_ATTR(CATTR_DB_PKVALUE,			m_dbHuman->m_nPKValue);
	REG_DB_ATTR(CATTR_DB_CURRENTPET,		m_dbHuman->m_guidCurrentPet);
	REG_DB_ATTR(CATTR_DB_EXP,				m_dbHuman->m_Exp);
	REG_DB_ATTR(CATTR_DB_AT_POSITION,		m_dbHuman->m_Position);
	REG_DB_ATTR(CATTR_DB_BK_POSITION,		m_dbHuman->m_BakPosition);
	REG_DB_ATTR(CATTR_DB_AT_SCENE,			m_dbHuman->m_StartScene);
	REG_DB_ATTR(CATTR_DB_BK_SCENE,			m_dbHuman->m_BakScene);
	REG_DB_ATTR(CATTR_DB_STR,				m_dbHuman->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_STR]);
	REG_DB_ATTR(CATTR_DB_CON,				m_dbHuman->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_CON]);
	REG_DB_ATTR(CATTR_DB_INT,				m_dbHuman->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_INT]);
	REG_DB_ATTR(CATTR_DB_DEX,				m_dbHuman->m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_DEX]);
	REG_DB_ATTR(CATTR_DB_REMAINPOINT,		m_dbHuman->m_Level1Points);
	REG_DB_ATTR(CATTR_DB_REMAINSKILLPOINT,	m_dbHuman->m_RemainSkillPoints);
	REG_DB_ATTR_SIZE(CATTR_DB_S_SKILLPOINT, m_dbHuman->m_S_SkillPoints,sizeof(INT)*COMMON_S_NUMBER);
	REG_DB_ATTR(CATTR_DB_HORSE_MATTING_FLAG,m_dbHuman->m_bHorseMating);

	REG_DB_ATTR(CATTR_DB_CAMP,				m_dbHuman->m_CampData);
	REG_DB_ATTR(CATTR_DB_MENPAI,			m_dbHuman->m_MenPai);
	REG_DB_ATTR(CATTR_DB_GUILDID,			m_dbHuman->m_GuildID);
	REG_DB_ATTR(CATTR_DB_FAMILYID,			m_dbHuman->m_FamilyID);
	REG_DB_ATTR(CATTR_DB_GUILDOFFICIAL,		m_dbHuman->m_nGuildOfficial);
	REG_DB_ATTR(CATTR_DB_TEAMID,			m_dbHuman->m_TeamID);
	REG_DB_ATTR(CATTR_DB_GUID,				m_dbHuman->m_GUID);
	REG_DB_ATTR(CATTR_DB_PORTRAITID,		m_dbHuman->m_PortraitID);
	REG_DB_ATTR_SIZE(CATTR_DB_NAME,			m_dbHuman->m_Name,sizeof(CHAR)*MAX_CHARACTER_NAME);
	REG_DB_ATTR(CATTR_DB_SEX,				m_dbHuman->m_Sex);
	REG_DB_ATTR(CATTR_DB_CREATETIME,		m_dbHuman->m_CreateDate);
	REG_DB_ATTR_SIZE(CATTR_DB_TITLE,		m_dbHuman->m_Title,sizeof(CHAR)*MAX_CHARACTER_TITLE);
	REG_DB_ATTR_SIZE(CATTR_DB_PASSWORD,		m_dbHuman->m_Passwd,sizeof(CHAR)*MAX_PWD);
	REG_DB_ATTR(CATTR_DB_PWDDELTIME,		m_dbHuman->m_uPwdDelTime);
	REG_DB_ATTR(CATTR_DB_HAIR_COLOR,		m_dbHuman->m_HairColor);
	REG_DB_ATTR(CATTR_DB_FACE_COLOR,		m_dbHuman->m_FaceColor);
	REG_DB_ATTR(CATTR_DB_HAIR_MODEL,		m_dbHuman->m_HairModel);
	REG_DB_ATTR(CATTR_DB_FACE_MODEL,		m_dbHuman->m_FaceModel);
	REG_DB_ATTR(CATTR_DB_ONLINETIME,		m_dbHuman->m_OnlineTime);
	REG_DB_ATTR(CATTR_DB_LASTLOGINTIME,		m_dbHuman->m_LastLoginTime);
	REG_DB_ATTR(CATTR_DB_LASTLOGOUTTIME,	m_dbHuman->m_LastLogoutTime);
	REG_DB_ATTR(CATTR_DB_DBVERSION,			m_dbHuman->m_DBVersion);
	REG_DB_ATTR(CATTR_DB_MONEY,				m_dbHuman->m_Money);
	REG_DB_ATTR(CATTR_DB_BANK_MONEY,		m_dbBank->m_Money);
	REG_DB_ATTR(CATTR_DB_BANK_OFFSIZE,		m_dbBank->m_CurEndIndex);
    REG_DB_ATTR(CATTR_DB_LEAVEGUILDTIME,    m_dbHuman->m_LeaveGuildTime);
	int iShopIndex = 0;
	for(int iDBShop=CATTR_DB_PLAYER_SHOP_BEGIN;iDBShop<=CATTR_DB_PLAYER_SHOP_END;iDBShop++)
	{
		REG_DB_ATTR((CHAR_ATTR_DB)iDBShop,m_dbHuman->m_ShopGuid[iShopIndex]);
		iShopIndex++;
	}

	int iFavoriteShopIndex = 0;
	for(int iDBFavoriteShop=CATTR_DB_PLAYER_SHOP_FAVORITE_BEGIN;iDBFavoriteShop<=CATTR_DB_PLAYER_SHOP_FAVORITE_END;iDBFavoriteShop++)
	{
		REG_DB_ATTR((CHAR_ATTR_DB)iDBFavoriteShop,m_dbHuman->m_ShopGuid[iFavoriteShopIndex]);
		iFavoriteShopIndex++;
	}

	int iBagIndex = 0;
	for(int iDBBag=CATTR_DB_BAG_BEGIN;iDBBag<=CATTR_DB_BAG_END;iDBBag++)
	{
		REG_DB_ATTR((CHAR_ATTR_DB)iDBBag,m_dbBag->m_pItem[iBagIndex]);
		iBagIndex++;
	}
	int iEquipIndex = 0;
	for(int iDBEquip = CATTR_DB_EQUIP_BEGIN;iDBEquip<=CATTR_DB_EQUIP_END;iDBEquip++)
	{
		REG_DB_ATTR((CHAR_ATTR_DB)iDBEquip,m_dbEquip->m_pItem[iEquipIndex]);
		iEquipIndex++;
	}
	int iBankIndex = 0;
	for(int iDBBank = CATTR_DB_BANK_BEGIN;iDBBank<=CATTR_DB_BANK_END;iDBBank++)
	{
		REG_DB_ATTR((CHAR_ATTR_DB)iDBBank,m_dbBank->m_pItem[iBankIndex]);
		iBankIndex++;
	}
	
	REG_DB_ATTR(CATTR_DB_SKILL,*m_dbSkill);
	REG_DB_ATTR(CATTR_DB_COOLDOWN,*m_dbCooldown);	
	REG_DB_ATTR(CATTR_DB_XINFA,*m_dbXinFa);
	REG_DB_ATTR(CATTR_DB_IMPACT,*m_dbImpact);
	REG_DB_ATTR(CATTR_DB_ABILITY,*m_dbAbility);	
	REG_DB_ATTR(CATTR_DB_MISSION,*m_dbMission);
	REG_DB_ATTR(CATTR_DB_SETTING,*m_dbSetting);

	REG_DB_ATTR(CATTR_DB_PET,*m_dbPetList);

	REG_DB_ATTR(CATTR_DB_RELATION,*m_dbRelation);
	REG_DB_ATTR(CATTR_DB_PRIVATEINFO,*m_dbPrivateInfo);
	REG_DB_ATTR(CATTR_DB_TITLEINFO,m_dbHuman->m_Titles);
	
	REG_DB_ATTR_SIZE(CATTR_DB_RESERVE,m_dbHuman->m_Reserve,sizeof(INT)*MAX_RESERVE);
	
	int iDBSouXiaIndex = 0;
	for(int iDBSouXia=CATTR_DB_SOUXIA_BEGIN; iDBSouXia<=CATTR_DB_SOUXIA_END; iDBSouXia++)
	{
		REG_DB_ATTR((CHAR_ATTR_DB)iDBSouXia, m_dbSouXia->m_SouXiaData[iDBSouXiaIndex]);	// add by gh
		iDBSouXiaIndex++;
	}
__LEAVE_FUNCTION

}

VOID	HumanDB::_RegisterSMAttributes()
{

#define REG_SM_ATTR(type,var)					_RegSMAttr(type,&(var),sizeof(var));
#define REG_SM_ATTR_SIZE(type,varPtr,size)		_RegSMAttr(type,varPtr,size);

	__ENTER_FUNCTION
		
	Lock();
	REG_SM_ATTR(CATTR_DB_HP,				m_HumanSM->m_HumanSM.m_HP);
	REG_SM_ATTR(CATTR_DB_MP,				m_HumanSM->m_HumanSM.m_MP);
	REG_SM_ATTR(CATTR_DB_STRIKEPOINT,		m_HumanSM->m_HumanSM.m_StrikePoint);
	REG_SM_ATTR(CATTR_DB_VIGOR,				m_HumanSM->m_HumanSM.m_Vigor);
	REG_SM_ATTR(CATTR_DB_MAX_VIGOR,			m_HumanSM->m_HumanSM.m_MaxVigor);
	REG_SM_ATTR(CATTR_DB_VIGOR_REGENE_RATE,	m_HumanSM->m_HumanSM.m_VigorRegeneRate);
	REG_SM_ATTR(CATTR_DB_ENERGY,			m_HumanSM->m_HumanSM.m_Energy);
	REG_SM_ATTR(CATTR_DB_MAX_ENERGY,		m_HumanSM->m_HumanSM.m_MaxEnergy);
	REG_SM_ATTR(CATTR_DB_ENERGY_REGENE_RATE,m_HumanSM->m_HumanSM.m_EnergyRegeneRate);
	REG_SM_ATTR(CATTR_DB_RAGE,				m_HumanSM->m_HumanSM.m_Rage);
	REG_SM_ATTR(CATTR_DB_LEVEL,				m_HumanSM->m_HumanSM.m_Level);
	REG_SM_ATTR(CATTR_DB_PKVALUE,			m_HumanSM->m_HumanSM.m_nPKValue);
	REG_SM_ATTR(CATTR_DB_CURRENTPET,		m_HumanSM->m_HumanSM.m_guidCurrentPet);
	REG_SM_ATTR(CATTR_DB_EXP,				m_HumanSM->m_HumanSM.m_Exp);
	REG_SM_ATTR(CATTR_DB_AT_POSITION,		m_HumanSM->m_HumanSM.m_Position);
	REG_SM_ATTR(CATTR_DB_BK_POSITION,		m_HumanSM->m_HumanSM.m_BakPosition);
	REG_SM_ATTR(CATTR_DB_AT_SCENE,			m_HumanSM->m_HumanSM.m_StartScene);
	REG_SM_ATTR(CATTR_DB_BK_SCENE,			m_HumanSM->m_HumanSM.m_BakScene);
	REG_SM_ATTR(CATTR_DB_STR,				m_HumanSM->m_HumanSM.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_STR]);
	REG_SM_ATTR(CATTR_DB_CON,				m_HumanSM->m_HumanSM.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_CON]);
	REG_SM_ATTR(CATTR_DB_INT,				m_HumanSM->m_HumanSM.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_INT]);
	REG_SM_ATTR(CATTR_DB_DEX,				m_HumanSM->m_HumanSM.m_BaseAttrLevel1.m_pAttr[CATTR_LEVEL1_DEX]);
	REG_SM_ATTR(CATTR_DB_REMAINPOINT,		m_HumanSM->m_HumanSM.m_Level1Points);
	REG_SM_ATTR_SIZE(CATTR_DB_S_SKILLPOINT,	m_dbHuman->m_S_SkillPoints,sizeof(INT)*COMMON_S_NUMBER);
	REG_SM_ATTR(CATTR_DB_HORSE_MATTING_FLAG,m_dbHuman->m_bHorseMating);

	REG_SM_ATTR(CATTR_DB_REMAINSKILLPOINT,  m_HumanSM->m_HumanSM.m_RemainSkillPoints);
	REG_SM_ATTR(CATTR_DB_CAMP,				m_HumanSM->m_HumanSM.m_CampData);
	REG_SM_ATTR(CATTR_DB_MENPAI,			m_HumanSM->m_HumanSM.m_MenPai);
	REG_SM_ATTR(CATTR_DB_GUILDID,			m_HumanSM->m_HumanSM.m_GuildID);
	REG_SM_ATTR(CATTR_DB_FAMILYID,			m_HumanSM->m_HumanSM.m_FamilyID);
	REG_SM_ATTR(CATTR_DB_GUILDOFFICIAL,		m_HumanSM->m_HumanSM.m_nGuildOfficial);
	REG_SM_ATTR(CATTR_DB_TEAMID,			m_HumanSM->m_HumanSM.m_TeamID);
	REG_SM_ATTR(CATTR_DB_GUID,				m_HumanSM->m_HumanSM.m_GUID);
	REG_SM_ATTR(CATTR_DB_PORTRAITID,		m_HumanSM->m_HumanSM.m_PortraitID);
	REG_SM_ATTR_SIZE(CATTR_DB_NAME,			m_HumanSM->m_HumanSM.m_Name,sizeof(CHAR)*MAX_CHARACTER_NAME);
	REG_SM_ATTR(CATTR_DB_SEX,				m_HumanSM->m_HumanSM.m_Sex);
	REG_SM_ATTR(CATTR_DB_CREATETIME,		m_HumanSM->m_HumanSM.m_CreateDate);
	REG_SM_ATTR_SIZE(CATTR_DB_TITLE,		m_HumanSM->m_HumanSM.m_Title,sizeof(CHAR)*MAX_CHARACTER_TITLE);
	REG_SM_ATTR_SIZE(CATTR_DB_PASSWORD,		m_HumanSM->m_HumanSM.m_Passwd,sizeof(CHAR)*MAX_PWD);
	REG_SM_ATTR(CATTR_DB_PWDDELTIME,		m_HumanSM->m_HumanSM.m_uPwdDelTime);
	REG_SM_ATTR(CATTR_DB_HAIR_COLOR,		m_HumanSM->m_HumanSM.m_HairColor);
	REG_SM_ATTR(CATTR_DB_FACE_COLOR,		m_HumanSM->m_HumanSM.m_FaceColor);
	REG_SM_ATTR(CATTR_DB_HAIR_MODEL,		m_HumanSM->m_HumanSM.m_HairModel);
	REG_SM_ATTR(CATTR_DB_FACE_MODEL,		m_HumanSM->m_HumanSM.m_FaceModel);
	REG_SM_ATTR(CATTR_DB_ONLINETIME,		m_HumanSM->m_HumanSM.m_OnlineTime);
	REG_SM_ATTR(CATTR_DB_LASTLOGINTIME,		m_HumanSM->m_HumanSM.m_LastLoginTime);
	REG_SM_ATTR(CATTR_DB_LASTLOGOUTTIME,	m_HumanSM->m_HumanSM.m_LastLogoutTime);
	REG_SM_ATTR(CATTR_DB_DBVERSION,			m_HumanSM->m_HumanSM.m_DBVersion);
	REG_SM_ATTR(CATTR_DB_MONEY,				m_HumanSM->m_HumanSM.m_Money);
	REG_SM_ATTR(CATTR_DB_BANK_MONEY,		m_HumanSM->m_BankSM.m_Money);
	REG_SM_ATTR(CATTR_DB_BANK_OFFSIZE,		m_HumanSM->m_BankSM.m_CurEndIndex);
    REG_SM_ATTR(CATTR_DB_LEAVEGUILDTIME,    m_HumanSM->m_HumanSM.m_LeaveGuildTime);
	int iShopIndex = 0;
	for(int iDBShop=CATTR_DB_PLAYER_SHOP_BEGIN;iDBShop<=CATTR_DB_PLAYER_SHOP_END;iDBShop++)
	{
		//REG_SM_ATTR((CHAR_ATTR_DB)iDBShop,m_dbHuman->m_ShopGuid[iShopIndex]);
		REG_SM_ATTR((CHAR_ATTR_DB)iDBShop,m_HumanSM->m_HumanSM.m_ShopGuid[iShopIndex]);
		iShopIndex++;
	}
	int iFavoriteShopIndex = 0;
	for(int iDBFavoriteShop=CATTR_DB_PLAYER_SHOP_FAVORITE_BEGIN;iDBFavoriteShop<=CATTR_DB_PLAYER_SHOP_FAVORITE_END;iDBFavoriteShop++)
	{
		//REG_SM_ATTR((CHAR_ATTR_DB)iDBFavoriteShop,m_dbHuman->m_ShopGuid[iFavoriteShopIndex]);
		REG_SM_ATTR((CHAR_ATTR_DB)iDBFavoriteShop,m_HumanSM->m_HumanSM.m_ShopGuid[iFavoriteShopIndex]);
		iFavoriteShopIndex++;
	}
	int iBagIndex = 0;
	for(int iDBBag=CATTR_DB_BAG_BEGIN;iDBBag<=CATTR_DB_BAG_END;iDBBag++)
	{
		REG_SM_ATTR((CHAR_ATTR_DB)iDBBag,m_HumanSM->m_BagSM.m_pItem[iBagIndex]);
		iBagIndex++;
	}
	int iEquipIndex = 0;
	for(int iDBEquip = CATTR_DB_EQUIP_BEGIN;iDBEquip<=CATTR_DB_EQUIP_END;iDBEquip++)
	{
		REG_SM_ATTR((CHAR_ATTR_DB)iDBEquip,m_HumanSM->m_EquipSM.m_pItem[iEquipIndex]);
		iEquipIndex++;
	}
	int iBankIndex = 0;
	for(int iDBBank = CATTR_DB_BANK_BEGIN;iDBBank<=CATTR_DB_BANK_END;iDBBank++)
	{
		REG_SM_ATTR((CHAR_ATTR_DB)iDBBank,m_HumanSM->m_BankSM.m_pItem[iBankIndex]);
		iBankIndex++;
	}
	REG_SM_ATTR(CATTR_DB_SKILL,		m_HumanSM->m_SkillSM);
	REG_SM_ATTR(CATTR_DB_COOLDOWN,	m_HumanSM->m_CooldownSM);	
	REG_SM_ATTR(CATTR_DB_XINFA,		m_HumanSM->m_XinFaSM);
	REG_SM_ATTR(CATTR_DB_IMPACT,	m_HumanSM->m_ImpactSM);
	REG_SM_ATTR(CATTR_DB_ABILITY,	m_HumanSM->m_AbilitySM);	
	REG_SM_ATTR(CATTR_DB_MISSION,	m_HumanSM->m_MissionSM);
	REG_SM_ATTR(CATTR_DB_SETTING,	m_HumanSM->m_SettingSM);

	REG_SM_ATTR(CATTR_DB_PET,		m_HumanSM->m_PetListSM);

	REG_SM_ATTR(CATTR_DB_RELATION,  m_HumanSM->m_RelationSM);
	REG_SM_ATTR(CATTR_DB_PRIVATEINFO,m_HumanSM->m_PrivateInfoSM);
	REG_SM_ATTR(CATTR_DB_TITLEINFO,m_HumanSM->m_HumanSM.m_Titles);
	REG_SM_ATTR_SIZE(CATTR_DB_RESERVE,m_HumanSM->m_HumanSM.m_Reserve,sizeof(INT)*MAX_RESERVE);
	
	int iDBSouXiaIndex = 0; // add by gh
	for(int iDBSouXia=CATTR_DB_SOUXIA_BEGIN; iDBSouXia<=CATTR_DB_SOUXIA_END; iDBSouXia++)
	{
		REG_SM_ATTR((CHAR_ATTR_DB)iDBSouXia, m_HumanSM->m_SouXiaSM.m_SouXiaData[iDBSouXiaIndex]);	// add by gh
		iDBSouXiaIndex++;
	}
	UnLock();
	__LEAVE_FUNCTION
}


VOID*	HumanDB::GetDBEnumAttr(CHAR_ATTR_DB eOffSet)
{
	__ENTER_FUNCTION
	 
		Assert(eOffSet<CATTR_DB_NUMBER);
		Assert(eOffSet>=0);
		Assert(m_AttrReg);

		return m_AttrReg->m_AttrPtr[eOffSet];

	__LEAVE_FUNCTION

		return NULL;
}

VOID*	HumanDB::GetSMEnumAttr(CHAR_ATTR_DB eOffSet)
{
	__ENTER_FUNCTION

	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(m_AttrRegSM);

	return m_AttrRegSM->m_AttrPtr[eOffSet];

	__LEAVE_FUNCTION

		return NULL;
}

INT		HumanDB::GetDBEnumAttrSize(CHAR_ATTR_DB eOffSet)
{
	__ENTER_FUNCTION
	
		Assert(eOffSet<CATTR_DB_NUMBER);
		Assert(eOffSet>=0);
		Assert(m_AttrFlag);

		return m_AttrReg->m_AttrSize[eOffSet];
	
	__LEAVE_FUNCTION

		return 0;
}

VOID	HumanDB::SetDBEnumAttrDirty(CHAR_ATTR_DB eOffSet,BOOL bFlag)
{
	__ENTER_FUNCTION
		
		Assert(eOffSet<CATTR_DB_NUMBER);
		Assert(eOffSet>=0);
		Assert(m_AttrFlag);

		m_AttrFlag->SetBitFlag(eOffSet,bFlag);

		//////////////////////////////////////////////////////////////////////////
		// add by wuwenbin
		if(TRUE == bFlag){
			SetToWorldEnumAttrDirty(eOffSet, bFlag);
		}
		//////////////////////////////////////////////////////////////////////////
		
	__LEAVE_FUNCTION
}

BOOL	HumanDB::GetDBEnumAttrDirty(CHAR_ATTR_DB eOffSet)
{
	__ENTER_FUNCTION

	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(m_AttrFlag);
	
	
	return m_AttrFlag->IsUpdated(eOffSet);

	__LEAVE_FUNCTION

	return FALSE;
}

VOID HumanDB::SetToWorldEnumAttrDirty(CHAR_ATTR_DB eOffSet,BOOL bFlag/* = TRUE*/)
{
	__ENTER_FUNCTION

	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(m_AttrFlag_ToWorld);

	m_AttrFlag_ToWorld->SetBitFlag(eOffSet,bFlag);

	__LEAVE_FUNCTION
}

BOOL HumanDB::GetToWorldEnumAttrDirty(CHAR_ATTR_DB eOffSet)
{
	__ENTER_FUNCTION

	Assert(eOffSet<CATTR_DB_NUMBER);
	Assert(eOffSet>=0);
	Assert(m_AttrFlag_ToWorld);


	return m_AttrFlag_ToWorld->IsUpdated(eOffSet);

	__LEAVE_FUNCTION

	return FALSE;
}

VOID	HumanDB::SetFreeOwnFlag(INT Flag)
{
	__ENTER_FUNCTION
		
		if(m_HumanSM)
		{
			m_HumanSM->SetUseStatus(Flag,SM_S_WRITE);
		}

	__LEAVE_FUNCTION
}

INT	HumanDB::GetFreeOwnFlag()
{
	__ENTER_FUNCTION
		
		if(m_HumanSM)
		{
			return m_HumanSM->GetUseStatus(SM_S_WRITE);
		}
		
		return -1;

	__LEAVE_FUNCTION

		return -1;
}