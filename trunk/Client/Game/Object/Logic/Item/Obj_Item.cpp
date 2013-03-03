
#include "StdAfx.h"
#include "Obj_Item.h"
#include "DBC/GMDataBase.h"
#include "GIDBC_Struct.h"
#include "GIException.h"

#include "Obj_Item_Equip.h"
#include "Obj_Item_Medicine.h"
#include "Obj_Item_Task.h"
#include "Obj_Item_Gem.h"
#include "Obj_Item_StoreMap.h"
#include "Obj_Item_ExtraBag.h"
#include "Obj_Item_Mount.h"  //  [7/8/2010 陈军龙]
#include "Obj_Item_SoulBead.h"
#include "Procedure/GameProcedure.h"


//需要在ui上显示的属性

 const char*  ITEMATTRIBUTE_NAME						 =	"name";					        //物品名  装备名称
 const char*  ITEMATTRIBUTE_ICON						 =	"icon";					        //装备图标
 const char*  ITEMATTRIBUTE_CARD						 =	"card";					        //麻将牌型
 const char*  ITEMATTRIBUTE_LEVEL						 =  "level";	    		        //物品等级 
 const char*  ITEMATTRIBUTE_PROFESSION  				 =	"profession";		            //职业
 const char*  ITEMATTRIBUTE_MAXDUR      				 =	"dur_max";				        //最大耐久
 const char*  ITEMATTRIBUTE_CURDUR      				 =	"dur_cur";                      //当前耐久
 const char*  ITEMATTRIBUTE_PRICE       				 =	"price";                        //价格
 const char*  ITEMATTRIBUTE_STARTLEVEL  				 =	"star_level";                   //星级
 const char*  ITEMATTRIBUTE_PRODUCER 					 =	"producer";		                //作者
 const char*  ITEMATTRIBUTE_GEMSNUM						 =	"gems_num";     				//宝石数量

 //const char*  ITEMATTRIBUTE_REPAIRNUM 					 =	"repair_num";	     	        //修理失败次数

 const char*   ITEMATTRIBUTE_EQUIP_TYPE					 =  "type";							//装备类型 
 const char*   ITEMATTRIBUTE_EQUIP_BINDINFO     		 =  "bindinfo";						//装备绑定信息
 const char*   ITEMATTRIBUTE_EQUIP_BASEATT				 =  "attr_base";					//所有装备基本属性
 const char*   ITEMATTRIBUTE_EQUIP_ADDATT				 =  "attr_add";						//所有装备附加属性
 const char*   ITEMATTRIBUTE_EQUIP_SKILL_ATTR   		 =  "attr_skill";					//所有装备附加技能和影响技能属性
 const char*   ITEMATTRIBUTE_EQUIP_GEMSATT      		 =  "arrt_gems";     				//装备宝石的属性
 const char*   ITEMATTRIBUTE_EQUIP_FOREVERATTRINFO  	 =  "forever_attr";				    //装备的永久（附魔）属性信息
 const char*   ITEMATTRIBUTE_EQUIP_TEMPATTRINFO     	 =  "temp_attr";					//装备的临时（附魔）属性信息
 const char*   ITEMATTRIBUTE_EQUIP_SETINFO 				 =  "setinfo"; 						//装备的套装组合信息
 const char*   ITEMATTRIBUTE_SET_ACCTIVEATTR			 =  "attr_set";						//套装激活的属性
 const char*   ITEMATTRIBUTE_DESC 					     = "desc"	;						//详细解释
 const char*   ITEMATTRIBUTE_DROP_COLOR					 =  "drop_color";					//掉落颜色		


 
INT     bCheck;			//是否需要鉴定
INT     bUnRealItem;    //是否是虚拟道具
INT     bAccessInBack;	//是否可存入银行
INT     bWearOut;		//是否消耗	   



#define INITIAL_RULE(ppItem) \
	const _DBC_ITEM_RULE* pItemRule = (const _DBC_ITEM_RULE*)s_pItemRule->Search_Index_EQU((UINT)ppItem->nRule);\
	if(pItemRule == NULL) return NULL;\
	pNewItem->m_Rule.bCanDrop		=	pItemRule->bCanDrop;\
	pNewItem->m_Rule.bCanOverLay	=	pItemRule->bCanOverLay;\
	pNewItem->m_Rule.bCanQuick		=	pItemRule->bCanQuick;\
	pNewItem->m_Rule.bCanSale		=	pItemRule->bCanSale;\
	pNewItem->m_Rule.bCanExchange	=	pItemRule->bCanExchange;\
	pNewItem->m_Rule.bCanUse		=	pItemRule->bCanUse;\
	pNewItem->m_Rule.bPickBind		=	pItemRule->bPickBind;\
	pNewItem->m_Rule.bEquipBind		=	pItemRule->bEquipBind;\
	pNewItem->m_Rule.bUnique		=	pItemRule->bUnique;\
	pNewItem->m_Rule.bCheck         =   pItemRule->bCheck;\
	pNewItem->m_Rule.bUnRealItem    =	pItemRule->bUnRealItem;\
	pNewItem->m_Rule.bAccessInBack  =	pItemRule->bAccessInBack;\
	pNewItem->m_Rule.bWearOut       =   pItemRule->bWearOut;\



std::map< UINT, CObject_Item* > CObject_Item::s_mapAllItem;


CObject_Item::CObject_Item(UINT id)
{
	m_idClient			= id;
	m_ID.m_idUnion		= 0;

	m_typeOwner			= IO_UNKNOWN;
	m_idOwner			= INVALID_UID;
	m_nPosIndex			= 0;
	m_idTable			= INVALID_UID;
	m_nMax				= -1;

	m_bLocked			= FALSE;

	m_nItemSaveStatus   = NO_MORE_INFO;

	m_bByProtect		= FALSE;

	m_Param[0] = -1;
	m_Param[1] = -1;
	m_Param[2] = -1;

	m_nNum = 0;
}

CObject_Item::~CObject_Item()
{
}


CObject_Item* CObject_Item::NewItem(UINT idTable)
{
	CObject_Item* pNewItem = NULL;

	//打开物品规则表
	DBC_DEFINEHANDLE(s_pItemRule, DBC_ITEM_RULE);


	//--------------------------------------------------------------------------------------------------------------
	BYTE nItemClass = GetSerialClass(idTable);
	switch( nItemClass )
	{
		//武器WEAPON、防具DEFENCE、饰物ADORN   1
	case ICLASS_EQUIP: 
		{
			//打开数据表 equip_algo.tab 装备的属性
			DBC_DEFINEHANDLE(s_pItem_Equip, DBC_ITEM_EQUIP);
			//搜索纪录
			const  _DBC_ITEM_EQUIP* pGEquip = ( const _DBC_ITEM_EQUIP*)s_pItem_Equip->Search_Index_EQU((UINT)idTable);
			if(!pGEquip) return NULL;

			pNewItem = new CObject_Item_Equip(_CreateID());
			((CObject_Item_Equip*)pNewItem)->AsEquip(   pGEquip );

			//物品规则
			INITIAL_RULE(pGEquip);
		}
		break;

	case ICLASS_MATERIAL: //原料 2

	case ICLASS_COMITEM:  //药品 3
	case ICLASS_IDENT:	  //鉴定券轴4

		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_Medic, DBC_ITEM_MEDIC);
			//搜索纪录
			const _DBC_ITEM_MEDIC* pMedic = (const _DBC_ITEM_MEDIC*)s_pItem_Medic->Search_Index_EQU((UINT)idTable);
			if(!pMedic) return NULL;

			pNewItem = new CObject_Item_Medicine(_CreateID());
			((CObject_Item_Medicine*)pNewItem)->AsMedicine(pMedic);

			//物品规则
			INITIAL_RULE(pMedic);
			
		}
		break;
	case ICLASS_TASKITEM: //任务物品 5
		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_Task, DBC_ITEM_TASK);
			//搜索纪录
			const _DBC_ITEM_TASK* pTask = (const _DBC_ITEM_TASK*)s_pItem_Task->Search_Index_EQU((UINT)idTable);
			if(!pTask) return NULL;

			pNewItem = new CObject_Item_Task(_CreateID());
			((CObject_Item_Task*)pNewItem)->AsTask(pTask);

			//物品规则
			INITIAL_RULE(pTask);


		}
		break;

	case ICLASS_GEM: //宝石 6
		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
			//搜索纪录
			const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU((UINT)idTable);
			if(!pGem) return NULL;

			pNewItem = new CObject_Item_Gem(_CreateID());
			((CObject_Item_Gem*)pNewItem)->AsGem(pGem);

			//物品规则
			INITIAL_RULE(pGem);
		}
		break;

	case ICLASS_STOREMAP:   //藏宝图  7
		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_STOREMAP);
			//搜索纪录
			const _DBC_ITEM_STOREMAP* pStoreMap = (const _DBC_ITEM_STOREMAP*)s_pItem_StoreMap->Search_Index_EQU((UINT)idTable);
			if(!pStoreMap) return NULL;

			pNewItem = new CObject_Item_StoreMap(_CreateID());
			((CObject_Item_StoreMap*)pNewItem)->AsStoreMap(pStoreMap);

			//物品规则
			INITIAL_RULE(pStoreMap);
		}
		break;

	case ICLASS_TALISMAN : //法宝  8
		return NULL;
		break;

	case ICLASS_GUILDITEM : //帮会物品 9
		return NULL;
		break;

	case ICLASS_EXTRABAG:		//扩展背包
		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_EXTRABAG);
			//搜索纪录
			const _DBC_ITEM_EXTRABAG* pStoreMap = (const _DBC_ITEM_EXTRABAG*)s_pItem_StoreMap->Search_Index_EQU((UINT)idTable);
			if(!pStoreMap) return NULL;

			pNewItem = new CObject_Item_ExtraBag(_CreateID());
			((CObject_Item_ExtraBag*)pNewItem)->AsExtraBag(pStoreMap);

			//物品规则
			INITIAL_RULE(pStoreMap);
		}
		break;

	case ICLASS_SOUL_BEAD:
		{
			//打开数据表
			DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_SOULBEAD);
			//搜索纪录
			const _DBC_ITEM_SOULBEAD* pStoreMap = (const _DBC_ITEM_SOULBEAD*)s_pItem_StoreMap->Search_Index_EQU((UINT)idTable);
			if(!pStoreMap) return NULL;

			pNewItem = new CObject_Item_SoulBead(_CreateID());
			((CObject_Item_SoulBead*)pNewItem)->AsSoulBead(pStoreMap);

			//物品规则
			INITIAL_RULE(pStoreMap);
		}
		break;
    // 坐骑  [7/8/2010 陈军龙]
	case ICLASS_MOUNT: 
		{
		//打开数据表
		DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_MOUNT_ATT);
		//搜索纪录
		const _DBC_ITEM_MOUNT_ATT* pMountMap = (const _DBC_ITEM_MOUNT_ATT*)s_pItem_StoreMap->Search_Index_EQU((UINT)idTable);
		if(!pMountMap) return NULL;

		pNewItem = new CObject_Item_Mount(_CreateID());
		((CObject_Item_Mount*)pNewItem)->AsMount(pMountMap);

		//物品规则
		INITIAL_RULE(pMountMap);
		}
		break;

	default:
		{
			return NULL;
			KLThrow("Invalid ItemID:%d", idTable);
		}
		break;
	}	


	pNewItem->m_idTable = idTable;
	s_mapAllItem[pNewItem->GetID()] = pNewItem;

	return pNewItem;
}

VOID CObject_Item::DestroyItem(CObject_Item* pItem)
{
	if(pItem)
	{
		std::map< UINT, CObject_Item* >::iterator it = s_mapAllItem.find(pItem->GetID());
		if(it != s_mapAllItem.end()) s_mapAllItem.erase(it);
		delete pItem;
	}
	return;
	//不能再调用其他代码
	//...
}

BOOL CObject_Item::CheckUseInPackage(CObject_Item* pItem, STRING& szMsg)
{
	struct ID_RANGE{
		INT		idBegin;
		INT		idEnd;
	};

	static ID_RANGE invalidRange[]={
		{ITEM_PET_SKILL_STUDY_BEGIN,	ITEM_PET_SKILL_STUDY_END},			//技能书
		{ITEM_PET_RETURN_BABAY_BEGIN-1, ITEM_PET_RETURN_BABAY_END+1},		//还童
		{ITEM_PET_ADD_LIFE_BEGIN,		ITEM_PET_ADD_LIFE_END},				//延长寿命
	};

    if(!pItem) return FALSE;
	for(INT i=0; i<sizeof(invalidRange)/sizeof(ID_RANGE); ++i)
	{
		if(pItem->GetIdTable() > invalidRange[i].idBegin && pItem->GetIdTable() < invalidRange[i].idEnd)
		{
			szMsg = pItem->RuleFailMessage(RULE_USE);
			return FALSE;
		}
	}

	return TRUE;
}

VOID  CObject_Item::ChangeClientID(VOID)
{ 
	s_mapAllItem.erase(m_idClient);
	m_idClient = _CreateID(); 
	s_mapAllItem[m_idClient] = this;
}

UINT CObject_Item::_CreateID(VOID)
{
	static UINT s_ID = 1;
	return s_ID++;
}

tObject_Item* CObject_Item::FindItem(UINT id)
{
	std::map< UINT, CObject_Item* >::iterator it = s_mapAllItem.find(id);
	if(it == s_mapAllItem.end()) return NULL;
	return it->second;
}

//=============================================================================

VOID CObject_Item::SetGUID(USHORT idWorld, USHORT idServer, UINT idSerial)
{
	m_ID.m_idOrg.m_idWorld	= idWorld;
	m_ID.m_idOrg.m_idServer	= idServer;
	m_ID.m_idOrg.m_uSerial	= idSerial;

}

VOID CObject_Item::GetGUID(USHORT& idWorld, USHORT& idServer, UINT& idSerail ) const
{
	idWorld		= m_ID.m_idOrg.m_idWorld;
	idServer	= m_ID.m_idOrg.m_idServer;
	idSerail	= m_ID.m_idOrg.m_uSerial;
}

unsigned __int64 CObject_Item::GetGUID(VOID) const
{
	return m_ID.m_idUnion;
}

STRING CObject_Item::GetAttributeValue(LPCTSTR szValueName)
{
	if(!szValueName) return "";

	if(_stricmp(szValueName, ITEMATTRIBUTE_NAME) == 0)
	{
		return GetName();
	}
	else
	{
		return "";
	}
}

VOID	CObject_Item::Clone(const tObject_Item* pItemSource)
{
	//克隆GUID
	USHORT idWorld		= 0;
	USHORT idServer		= 0;
	UINT idSerail		= 0;
	pItemSource->GetGUID(idWorld, idServer, idSerail);
	SetGUID(idWorld, idServer, idSerail);
}


//物品规则验证
BOOL	CObject_Item::Rule(INT ruletype)
{
	switch(ruletype)
	{
	case RULE_DROP:
		{
			return m_Rule.bCanDrop;
		}
		break;
	case RULE_OVERLAY:
		{
			return m_Rule.bCanOverLay;
		}
		break;

	case RULE_QUICK:
		{
			return m_Rule.bCanQuick;
		}
		break;

	case RULE_SALE:
		{
			return m_Rule.bCanSale;
		}
		break;

	case RULE_EXCHANGE:
		{
			return m_Rule.bCanExchange;
		}
		break;

	case RULE_USE:
		{
			return m_Rule.bCanUse;
		}
		break;

	case RULE_PICKBIND:
		{
			return m_Rule.bPickBind;
		}
		break;

	case RULE_EQUIPBIND:
		{
			return m_Rule.bEquipBind;
		}
		break;

	case RULE_UNIQUE:
		{
			return m_Rule.bUnique;
		}
		break;
	default:
		KLAssert(0);
		break;
	}
	return TRUE;
}

STRING	CObject_Item::RuleFailMessage(INT ruletype)
{
	STRING strTemp = "";

	switch(ruletype)
	{
	case RULE_DROP:
		{
			
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_Discard");
			return strTemp;
			//return "此物品不可以丢弃";
		}
		break;
	case RULE_OVERLAY:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_Overlay");
			return strTemp;
			//return "此物品不可以叠加";
		}
		break;

	case RULE_QUICK:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_In_ShortCut");
			return strTemp;
			//return "此物品不可以放入快捷栏";
		}
		break;

	case RULE_SALE:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_Sell");
			return strTemp;
			//return "此物品不可以卖出";
		}
		break;

	case RULE_EXCHANGE:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_Exchange");
			return strTemp;
			//return "此物品不可以交易";
		}
		break;

	case RULE_USE:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_Use");
			return strTemp;
			//return "此物品不可以使用";
		}
		break;

	case RULE_PICKBIND:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Bind");
			return strTemp;
			//return "此物品已绑定";
		}
		break;

	case RULE_EQUIPBIND:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Bind");
			return strTemp;
			//return "此物品已绑定";
		}
		break;

	case RULE_UNIQUE:
		{
			return "";
		}
		break;
	case RULE_IDENT://此物品不需要鉴定	
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Not_Need_Ident");
			return strTemp;
		}
		break;
	case RULE_DUMMYITEM://此物品不是虚拟道具	
		{
			return "";
		}
		break;
	case RULE_BANK://此物品不可以存入银行
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_InBank");
			return strTemp;
		}
		break;
	case RULE_CONSUME://此物品不消耗
		{
			return "";

		}
		break;
	default:
		KLAssert(0);
		break;
	}
	return "";
}

// 得到玩家使用这个物品需要的等级
INT	CObject_Item::GetNeedLevel()
{
	// 表示没有这个属性
	return -1;
}

// 得到物品耐久度
INT	CObject_Item::GetItemDur()
{
	
	return -1;
}

// 得到物品最大耐久度
INT	CObject_Item::GetItemMaxDur()
{

	return -1;
}

// 得到物品的修理次数 
INT	CObject_Item::GetItemRepairCount()
{
	// -1 表示无修理信息
	return -1;
}

// 得到物品的绑定信息 
INT	CObject_Item::GetItemBindInfo()
{
	// -1 无绑定信息
	return -1;
}

// 得到物品的2级绑定信息 
INT	CObject_Item::GetItemSecLevelBindInfo()
{
	// -1 无绑定信息
	return -1;
}



//	得到装备的2次绑定加成

LPCTSTR	CObject_Item::GetItemBindAddInfo()
{
	// -1 无绑定信息
	return _T("");
}


// 20100408 AddCodeBegin
// 得到物品买入的价格
 INT CObject_Item::GetItemBasePrice()
{
	 return -1;
}
// 20100408 AddCodeEnd


// 得到物品卖给npc的价格
INT	CObject_Item::GetItemPrice()
{
	// -1 无价格信息
	 return -1;
}


//得到物品职业
INT CObject_Item::GetItemProfession()
{
  // -1 无效职业
	
	return -1;
}

//得到物品是否唯一
INT	CObject_Item::GetItemRuleOnly()					  
{       
	return m_Rule.bUnique;

}
// 得到物品的制作人
LPCTSTR	CObject_Item::GetManufacturer()
{
	return _T("");
}

// 得到白色属性 
LPCTSTR	CObject_Item::GetBaseWhiteAttrInfo()
{
	return _T("");
}


//得到掉落时的模型id
 LPCTSTR	 CObject_Item::GetDropVisualID(VOID)const
{
	return NULL;
}

//得到掉落时的颜色
LPCTSTR	 CObject_Item::GetDropVisColor(VOID)const
{
	return NULL;
}


// 得到这个物品的等级
INT	 CObject_Item::GetItemLevelTable(VOID)const
{

	return -1;

}
 
// 设置物品保存状态  
void CObject_Item::SetItemSaveStatus(SAVE_ITEM_STATUS nSaveStatus)
{
	m_nItemSaveStatus = nSaveStatus;
}

//设置物品保存状态  
BOOL CObject_Item::IsItemHasDetailInfo()
{
	if(GET_MORE_INFO == m_nItemSaveStatus)
	{
		return FALSE;
	}

	return TRUE;
}

INT	CObject_Item::GetItemTargetType()
{
	return -1;
}

INT	CObject_Item::GetItemTableQuality(VOID)
{
	return -1;
}

INT	CObject_Item::GetItemTableType(VOID)
{
	return -1;
}

// 得到物品的类型描述 
LPCTSTR	CObject_Item::GetItemTableTypeDesc()
{
	return "无类型描述";
}

LPCTSTR CObject_Item::GetExtranInfo()
{
	CHAR    out[MAX_ITEM_STRING_LENGTH] = {0};
	CHAR*	pOut = out;
	CHAR* pBuff = pOut;
	INT OutLength = 0;
	*((UINT*)pBuff)	=	m_idTable;
	pBuff+=sizeof(UINT);

	OutLength	+=	sizeof(INT);
	CHAR		p[255];
	memset(p,0,255);
	Binary2String(pOut,OutLength,p);
	strcpy( pOut, "i");
	strcat( pOut, p );
	OutLength = (INT)strlen(pOut);

	return pOut;
}
