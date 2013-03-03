#include "PlayerShopDefine.h"
#include "GameStruct_Pet.h"
#include "ShopInfoLoad.h"

using namespace PLAYER_SHOP;

//////////////////////////////////////////////////////////////////////////
//_PET_DB_LOAD_V0
VOID _PET_DB_LOAD_V0::CleanUp( VOID )
{
	//基本信息
	m_GUID.Reset();
	m_SpouseGUID.Reset();
	m_nDataID			= INVALID_ID;
	memset( m_szName, 0, MAX_CHARACTER_NAME );
	memset( m_szNick, 0, MAX_NICK_NAME );

	m_nLevel			= 0;
	m_nLastProcreateLevel	= -1;
	m_nTakeLevel		= 0;
	m_nAttackType		= 0;
	m_AIType			= _PET_DB_LOAD::PET_AI_INVALID;
	m_nHP				= 0;
	m_nLife				= 0;
	m_byPetType			= _PET_DB_LOAD::PET_TYPE_INVALID;
	m_byGeneration		= 0;
	m_byHappiness			= 0;
	m_nStrPerception	= 0;
	m_nConPerception	= 0;
	m_nDexPerception	= 0;
	m_nSprPerception	= 0;
	m_nIntPerception	= 0;
	m_nGenGu			= 0;
	m_fGrowRate			= 0;
	m_nRemainPoint		= 0;
	m_nExp				= 0;
	m_yFlags			= 0;
	m_nSavvy			= 0;

	for (INT i = 0; i < PET_MAX_SKILL_COUNT; ++i)
	{
		m_SkillList[i].CleanUp();
	}
	m_CooldownList.CleanUp();
}

//_PET_DB_LOAD_V1
VOID _PET_DB_LOAD_V1::CleanUp( VOID )
{
	m_V0.CleanUp();

	for (INT i = 0; i < PET_TITLE_NUM; ++i)
	{
		m_PetTitle[i].CleanUp();
	}
	m_tCurPetTitleID = INVALID_INDEX;
}

//////////////////////////////////////////////////////////////////////////
//ShopInfoLoad
VOID ShopInfoLoad::LoadFrom_V0(PLAYER_SHOP::StallBoxDB_t& stBox,StallBoxDB_t_V0& stBox_V0)
{
__ENTER_FUNCTION
	//不变的部分
	memcpy(stBox.m_bItemIsOnSale,stBox_V0.m_bItemIsOnSale,STALL_BOX_SIZE*sizeof(BOOL));
	memcpy(stBox.m_ItemList,stBox_V0.m_ItemList,STALL_BOX_SIZE*sizeof(_ITEM));
	memcpy(stBox.m_ItemPrice,stBox_V0.m_ItemPrice,STALL_BOX_SIZE*sizeof(UINT));
	stBox.m_nFirstPage = stBox_V0.m_nFirstPage;
	stBox.m_StallStatus = stBox_V0.m_StallStatus;
	stBox.m_StallType = stBox_V0.m_StallType;
	stBox.m_ValidNum = stBox_V0.m_ValidNum;

	//变化的部分
	for(INT i = 0; i < STALL_BOX_SIZE; ++i)
	{
		_PET_DB_LOAD&		petDB		= stBox.m_PetList[i];
		_PET_DB_LOAD_V0&	petDB_V0	= stBox_V0.m_PetList[i];

		PetInfoLoad::LoadFrom_VO(petDB,petDB_V0);
	}
__LEAVE_FUNCTION
}

VOID ShopInfoLoad::LoadFrom_V1(PLAYER_SHOP::StallBoxDB_t& stBox,StallBoxDB_t_V1& stBox_V1)
{
__ENTER_FUNCTION
	//不变的部分
	memcpy(stBox.m_bItemIsOnSale,stBox_V1.m_bItemIsOnSale,STALL_BOX_SIZE*sizeof(BOOL));
	memcpy(stBox.m_ItemPrice,stBox_V1.m_ItemPrice,STALL_BOX_SIZE*sizeof(UINT));
	stBox.m_nFirstPage = stBox_V1.m_nFirstPage;
	stBox.m_StallStatus = stBox_V1.m_StallStatus;
	stBox.m_StallType = stBox_V1.m_StallType;
	stBox.m_ValidNum = stBox_V1.m_ValidNum;

	//变化的部分
	for(INT i = 0; i < STALL_BOX_SIZE; ++i)
	{
		_ITEM&				itemDB		= stBox.m_ItemList[i];
		_ITEM_V1&			itemDB_V1	= stBox_V1.m_ItemList[i];

		ItemInfoLoad::LoadFrom_V1(itemDB,itemDB_V1);
	}
	for(INT i = 0; i < STALL_BOX_SIZE; ++i)
	{
		_PET_DB_LOAD&		petDB		= stBox.m_PetList[i];
		_PET_DB_LOAD_V1&	petDB_V1	= stBox_V1.m_PetList[i];

		PetInfoLoad::LoadFrom_V1(petDB,petDB_V1);
	}
__LEAVE_FUNCTION
}

VOID ShopInfoLoad::LoadFrom_BuyShop_V1(PLAYER_SHOP::PlayerShopBuyShopDB_t& stBuyShop,PlayerShopBuyShopDB_t_V1& stBuyShop_V1)
{
__ENTER_FUNCTION
	//不变的部分
	memcpy(stBuyShop.m_szDesc,stBuyShop_V1.m_szDesc,PLAYER_SHOP_BUY_STALL_DESC_DB*sizeof(CHAR));
	memcpy(stBuyShop.m_Reserve,stBuyShop_V1.m_Reserve,40*sizeof(CHAR));

	//变化的部分
	for(INT i = 0; i < PLAYER_SHOP_BUY_STALL_NUM; ++i)
	{
		PlayerShopBuyShopStallDB_t&		stallDB		= stBuyShop.m_StallList[i];
		PlayerShopBuyShopStallDB_t_V1&	stallDB_V1	= stBuyShop_V1.m_StallList[i];

		//不变的部分
		memcpy(stallDB.m_iNeededItemNum,stallDB_V1.m_iNeededItemNum,PLAYER_SHOP_BUY_STALL_BOX_SIZE*sizeof(INT));
		memcpy(stallDB.m_iNeededItemIdx,stallDB_V1.m_iNeededItemIdx,PLAYER_SHOP_BUY_STALL_BOX_SIZE*sizeof(UINT));
		memcpy(stallDB.m_iPrice,stallDB_V1.m_iPrice,PLAYER_SHOP_BUY_STALL_BOX_SIZE*sizeof(INT));
		memcpy(stallDB.m_InStallNum,stallDB_V1.m_InStallNum,PLAYER_SHOP_BUY_STALL_BOX_SIZE*sizeof(BYTE));
		memcpy(stallDB.m_Reserve,stallDB_V1.m_Reserve,20*sizeof(CHAR));

		//变化的部分
		for(INT j = 0; j < PLAYER_SHOP_BUY_STALL_BOX_SIZE; ++j)
		{
			_ITEM&						itemDB		= stallDB.m_ItemList[j];
			_ITEM_V1&					itemDB_V1	= stallDB_V1.m_ItemList[j];

			ItemInfoLoad::LoadFrom_V1(itemDB,itemDB_V1);
		}
	}
__LEAVE_FUNCTION
}

//////////////////////////////////////////////////////////////////////////
//PetInfoLoad
VOID PetInfoLoad::LoadFrom_VO(_PET_DB_LOAD& petDB,_PET_DB_LOAD_V0& petDB_V0)
{
__ENTER_FUNCTION
	petDB.m_GUID					= petDB_V0.m_GUID;
	petDB.m_SpouseGUID				= petDB_V0.m_SpouseGUID;				
	petDB.m_nDataID					= petDB_V0.m_nDataID;
	strncpy(petDB.m_szName,petDB_V0.m_szName,MAX_CHARACTER_NAME);	
	strncpy(petDB.m_szNick,petDB_V0.m_szNick,MAX_NICK_NAME);
	petDB.m_nLevel					= petDB_V0.m_nLevel;	
	petDB.m_nLastProcreateLevel		= petDB_V0.m_nLastProcreateLevel;
	petDB.m_nTakeLevel				= petDB_V0.m_nTakeLevel;
	petDB.m_nAttackType				= petDB_V0.m_nAttackType;
	petDB.m_AIType					= petDB_V0.m_AIType;
	petDB.m_nHP						= petDB_V0.m_nHP;
	petDB.m_nLife					= petDB_V0.m_nLife;
	petDB.m_byPetType				= petDB_V0.m_byPetType;
	petDB.m_byGeneration			= petDB_V0.m_byGeneration;
	petDB.m_byHappiness				= petDB_V0.m_byHappiness;

	petDB.m_nStrPerception			= petDB_V0.m_nStrPerception;		
	petDB.m_nConPerception			= petDB_V0.m_nConPerception;		
	petDB.m_nDexPerception			= petDB_V0.m_nDexPerception;		
	petDB.m_nSprPerception			= petDB_V0.m_nSprPerception;		
	petDB.m_nIntPerception			= petDB_V0.m_nIntPerception;		
	petDB.m_nGenGu					= petDB_V0.m_nGenGu;		
	petDB.m_fGrowRate				= petDB_V0.m_fGrowRate;
	petDB.m_nRemainPoint			= petDB_V0.m_nRemainPoint;			
	petDB.m_nExp					= petDB_V0.m_nExp;			
	petDB.m_yFlags					= petDB_V0.m_yFlags;

	memcpy(&(petDB.m_BaseAttrLevel1),&(petDB_V0.m_BaseAttrLevel1),sizeof(_ATTR_LEVEL1));
	memcpy(petDB.m_SkillList,petDB_V0.m_SkillList,PET_MAX_SKILL_COUNT*sizeof(_PET_SKILL));

	petDB.m_CooldownList			= petDB_V0.m_CooldownList;
	petDB.m_nSavvy					= petDB_V0.m_nSavvy;
__LEAVE_FUNCTION
}

VOID PetInfoLoad::LoadFrom_V1(_PET_DB_LOAD& petDB,_PET_DB_LOAD_V1& petDB_V1)
{
__ENTER_FUNCTION
	LoadFrom_VO(petDB,petDB_V1.m_V0);

	memcpy(petDB.m_PetTitle,petDB_V1.m_PetTitle,PET_TITLE_NUM*sizeof(_PET_TITLE));
	petDB.m_tCurPetTitleID			= petDB_V1.m_tCurPetTitleID;
__LEAVE_FUNCTION
}

//////////////////////////////////////////////////////////////////////////
//ItemInfoLoad
VOID ItemInfoLoad::LoadFrom_V1(_ITEM& itemDB,_ITEM_V1& itemDB_V1)
{
__ENTER_FUNCTION
	itemDB.m_ItemGUID	= itemDB_V1.m_ItemGUID;
	itemDB.m_ItemIndex	= itemDB_V1.m_ItemIndex;
	itemDB.m_RulerID	= itemDB_V1.m_RulerID;
	itemDB.m_nsBind		= itemDB_V1.m_nsBind;

	memcpy(itemDB.m_Creator,itemDB_V1.m_Creator,MAX_ITEM_CREATOR_NAME*sizeof(CHAR));
	memcpy(itemDB.m_Param,itemDB_V1.m_Param,MAX_ITEM_PARAM*sizeof(INT));

	switch(itemDB.ItemClass())
	{
	case ICLASS_EQUIP://12
		{
			memcpy(&(itemDB.m_Equip),&(itemDB_V1.m_Equip),sizeof(EQUIP_INFO));
		}
		break;
	case ICLASS_GEM://7
		{
			memcpy(&(itemDB.m_Gem),&(itemDB_V1.m_Gem),sizeof(GEM_INFO));
		}
		break;
	case ICLASS_COMITEM:
	case ICLASS_MATERIAL:
	case ICLASS_TASKITEM://24
		{
			memcpy(&(itemDB.m_Medic),&(itemDB_V1.m_Medic),sizeof(MEDIC_INFO));
		}
		break;
	case ICLASS_STOREMAP://20
		{
			memcpy(&(itemDB.m_StoreMap),&(itemDB_V1.m_StoreMap),sizeof(STORE_MAP_INFO));
		}
		break;
	default:
		break;
	}
__LEAVE_FUNCTION
}

//////////////////////////////////////////////////////////////////////////
//PetProcreateInfoLoad
VOID PetProcreateInfoLoad::LoadFrom_V0(PetProcreateItem& petPro,PetProcreateItem_VO& petPro_V0)
{
__ENTER_FUNCTION
	petPro.m_cState					= petPro_V0.m_cState;								
	petPro.m_guidHumanFirst			= petPro_V0.m_guidHumanFirst;
	petPro.m_guidHumanSecond		= petPro_V0.m_guidHumanSecond;
	strncpy(petPro.m_szFirstHumanName,petPro_V0.m_szFirstHumanName,MAX_CHARACTER_NAME);
	strncpy(petPro.m_szSecondHumanName,petPro_V0.m_szSecondHumanName,MAX_CHARACTER_NAME);
	PetInfoLoad::LoadFrom_VO(petPro.m_dbPetFirst,petPro_V0.m_dbPetFirst);
	PetInfoLoad::LoadFrom_VO(petPro.m_dbPetSecond,petPro_V0.m_dbPetSecond);
	petPro.m_uProcreateInterval		= petPro_V0.m_uProcreateInterval;	
	petPro.m_uWaitTakeOutTime		= petPro_V0.m_uWaitTakeOutTime;		
	petPro.m_uPrevMailToHumanTime	= petPro_V0.m_uPrevMailToHumanTime;	
	petPro.m_uStateInterval			= petPro_V0.m_uStateInterval;		
__LEAVE_FUNCTION
}

VOID PetProcreateInfoLoad::LoadFrom_V1(PetProcreateItem& petPro,PetProcreateItem_V1& petPro_V1)
{
__ENTER_FUNCTION
	petPro.m_cState					= petPro_V1.m_cState;								
	petPro.m_guidHumanFirst			= petPro_V1.m_guidHumanFirst;
	petPro.m_guidHumanSecond		= petPro_V1.m_guidHumanSecond;
	strncpy(petPro.m_szFirstHumanName,petPro_V1.m_szFirstHumanName,MAX_CHARACTER_NAME);
	strncpy(petPro.m_szSecondHumanName,petPro_V1.m_szSecondHumanName,MAX_CHARACTER_NAME);
	PetInfoLoad::LoadFrom_V1(petPro.m_dbPetFirst,petPro_V1.m_dbPetFirst);
	PetInfoLoad::LoadFrom_V1(petPro.m_dbPetSecond,petPro_V1.m_dbPetSecond);
	petPro.m_uProcreateInterval		= petPro_V1.m_uProcreateInterval;	
	petPro.m_uWaitTakeOutTime		= petPro_V1.m_uWaitTakeOutTime;		
	petPro.m_uPrevMailToHumanTime	= petPro_V1.m_uPrevMailToHumanTime;	
	petPro.m_uStateInterval			= petPro_V1.m_uStateInterval;		
__LEAVE_FUNCTION
}
