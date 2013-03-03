
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
#include "Obj_Item_Mount.h"  //  [7/8/2010 �¾���]
#include "Obj_Item_SoulBead.h"
#include "Procedure/GameProcedure.h"


//��Ҫ��ui����ʾ������

 const char*  ITEMATTRIBUTE_NAME						 =	"name";					        //��Ʒ��  װ������
 const char*  ITEMATTRIBUTE_ICON						 =	"icon";					        //װ��ͼ��
 const char*  ITEMATTRIBUTE_CARD						 =	"card";					        //�齫����
 const char*  ITEMATTRIBUTE_LEVEL						 =  "level";	    		        //��Ʒ�ȼ� 
 const char*  ITEMATTRIBUTE_PROFESSION  				 =	"profession";		            //ְҵ
 const char*  ITEMATTRIBUTE_MAXDUR      				 =	"dur_max";				        //����;�
 const char*  ITEMATTRIBUTE_CURDUR      				 =	"dur_cur";                      //��ǰ�;�
 const char*  ITEMATTRIBUTE_PRICE       				 =	"price";                        //�۸�
 const char*  ITEMATTRIBUTE_STARTLEVEL  				 =	"star_level";                   //�Ǽ�
 const char*  ITEMATTRIBUTE_PRODUCER 					 =	"producer";		                //����
 const char*  ITEMATTRIBUTE_GEMSNUM						 =	"gems_num";     				//��ʯ����

 //const char*  ITEMATTRIBUTE_REPAIRNUM 					 =	"repair_num";	     	        //����ʧ�ܴ���

 const char*   ITEMATTRIBUTE_EQUIP_TYPE					 =  "type";							//װ������ 
 const char*   ITEMATTRIBUTE_EQUIP_BINDINFO     		 =  "bindinfo";						//װ������Ϣ
 const char*   ITEMATTRIBUTE_EQUIP_BASEATT				 =  "attr_base";					//����װ����������
 const char*   ITEMATTRIBUTE_EQUIP_ADDATT				 =  "attr_add";						//����װ����������
 const char*   ITEMATTRIBUTE_EQUIP_SKILL_ATTR   		 =  "attr_skill";					//����װ�����Ӽ��ܺ�Ӱ�켼������
 const char*   ITEMATTRIBUTE_EQUIP_GEMSATT      		 =  "arrt_gems";     				//װ����ʯ������
 const char*   ITEMATTRIBUTE_EQUIP_FOREVERATTRINFO  	 =  "forever_attr";				    //װ�������ã���ħ��������Ϣ
 const char*   ITEMATTRIBUTE_EQUIP_TEMPATTRINFO     	 =  "temp_attr";					//װ������ʱ����ħ��������Ϣ
 const char*   ITEMATTRIBUTE_EQUIP_SETINFO 				 =  "setinfo"; 						//װ������װ�����Ϣ
 const char*   ITEMATTRIBUTE_SET_ACCTIVEATTR			 =  "attr_set";						//��װ���������
 const char*   ITEMATTRIBUTE_DESC 					     = "desc"	;						//��ϸ����
 const char*   ITEMATTRIBUTE_DROP_COLOR					 =  "drop_color";					//������ɫ		


 
INT     bCheck;			//�Ƿ���Ҫ����
INT     bUnRealItem;    //�Ƿ����������
INT     bAccessInBack;	//�Ƿ�ɴ�������
INT     bWearOut;		//�Ƿ�����	   



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

	//����Ʒ�����
	DBC_DEFINEHANDLE(s_pItemRule, DBC_ITEM_RULE);


	//--------------------------------------------------------------------------------------------------------------
	BYTE nItemClass = GetSerialClass(idTable);
	switch( nItemClass )
	{
		//����WEAPON������DEFENCE������ADORN   1
	case ICLASS_EQUIP: 
		{
			//�����ݱ� equip_algo.tab װ��������
			DBC_DEFINEHANDLE(s_pItem_Equip, DBC_ITEM_EQUIP);
			//������¼
			const  _DBC_ITEM_EQUIP* pGEquip = ( const _DBC_ITEM_EQUIP*)s_pItem_Equip->Search_Index_EQU((UINT)idTable);
			if(!pGEquip) return NULL;

			pNewItem = new CObject_Item_Equip(_CreateID());
			((CObject_Item_Equip*)pNewItem)->AsEquip(   pGEquip );

			//��Ʒ����
			INITIAL_RULE(pGEquip);
		}
		break;

	case ICLASS_MATERIAL: //ԭ�� 2

	case ICLASS_COMITEM:  //ҩƷ 3
	case ICLASS_IDENT:	  //����ȯ��4

		{
			//�����ݱ�
			DBC_DEFINEHANDLE(s_pItem_Medic, DBC_ITEM_MEDIC);
			//������¼
			const _DBC_ITEM_MEDIC* pMedic = (const _DBC_ITEM_MEDIC*)s_pItem_Medic->Search_Index_EQU((UINT)idTable);
			if(!pMedic) return NULL;

			pNewItem = new CObject_Item_Medicine(_CreateID());
			((CObject_Item_Medicine*)pNewItem)->AsMedicine(pMedic);

			//��Ʒ����
			INITIAL_RULE(pMedic);
			
		}
		break;
	case ICLASS_TASKITEM: //������Ʒ 5
		{
			//�����ݱ�
			DBC_DEFINEHANDLE(s_pItem_Task, DBC_ITEM_TASK);
			//������¼
			const _DBC_ITEM_TASK* pTask = (const _DBC_ITEM_TASK*)s_pItem_Task->Search_Index_EQU((UINT)idTable);
			if(!pTask) return NULL;

			pNewItem = new CObject_Item_Task(_CreateID());
			((CObject_Item_Task*)pNewItem)->AsTask(pTask);

			//��Ʒ����
			INITIAL_RULE(pTask);


		}
		break;

	case ICLASS_GEM: //��ʯ 6
		{
			//�����ݱ�
			DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
			//������¼
			const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU((UINT)idTable);
			if(!pGem) return NULL;

			pNewItem = new CObject_Item_Gem(_CreateID());
			((CObject_Item_Gem*)pNewItem)->AsGem(pGem);

			//��Ʒ����
			INITIAL_RULE(pGem);
		}
		break;

	case ICLASS_STOREMAP:   //�ر�ͼ  7
		{
			//�����ݱ�
			DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_STOREMAP);
			//������¼
			const _DBC_ITEM_STOREMAP* pStoreMap = (const _DBC_ITEM_STOREMAP*)s_pItem_StoreMap->Search_Index_EQU((UINT)idTable);
			if(!pStoreMap) return NULL;

			pNewItem = new CObject_Item_StoreMap(_CreateID());
			((CObject_Item_StoreMap*)pNewItem)->AsStoreMap(pStoreMap);

			//��Ʒ����
			INITIAL_RULE(pStoreMap);
		}
		break;

	case ICLASS_TALISMAN : //����  8
		return NULL;
		break;

	case ICLASS_GUILDITEM : //�����Ʒ 9
		return NULL;
		break;

	case ICLASS_EXTRABAG:		//��չ����
		{
			//�����ݱ�
			DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_EXTRABAG);
			//������¼
			const _DBC_ITEM_EXTRABAG* pStoreMap = (const _DBC_ITEM_EXTRABAG*)s_pItem_StoreMap->Search_Index_EQU((UINT)idTable);
			if(!pStoreMap) return NULL;

			pNewItem = new CObject_Item_ExtraBag(_CreateID());
			((CObject_Item_ExtraBag*)pNewItem)->AsExtraBag(pStoreMap);

			//��Ʒ����
			INITIAL_RULE(pStoreMap);
		}
		break;

	case ICLASS_SOUL_BEAD:
		{
			//�����ݱ�
			DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_SOULBEAD);
			//������¼
			const _DBC_ITEM_SOULBEAD* pStoreMap = (const _DBC_ITEM_SOULBEAD*)s_pItem_StoreMap->Search_Index_EQU((UINT)idTable);
			if(!pStoreMap) return NULL;

			pNewItem = new CObject_Item_SoulBead(_CreateID());
			((CObject_Item_SoulBead*)pNewItem)->AsSoulBead(pStoreMap);

			//��Ʒ����
			INITIAL_RULE(pStoreMap);
		}
		break;
    // ����  [7/8/2010 �¾���]
	case ICLASS_MOUNT: 
		{
		//�����ݱ�
		DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_MOUNT_ATT);
		//������¼
		const _DBC_ITEM_MOUNT_ATT* pMountMap = (const _DBC_ITEM_MOUNT_ATT*)s_pItem_StoreMap->Search_Index_EQU((UINT)idTable);
		if(!pMountMap) return NULL;

		pNewItem = new CObject_Item_Mount(_CreateID());
		((CObject_Item_Mount*)pNewItem)->AsMount(pMountMap);

		//��Ʒ����
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
	//�����ٵ�����������
	//...
}

BOOL CObject_Item::CheckUseInPackage(CObject_Item* pItem, STRING& szMsg)
{
	struct ID_RANGE{
		INT		idBegin;
		INT		idEnd;
	};

	static ID_RANGE invalidRange[]={
		{ITEM_PET_SKILL_STUDY_BEGIN,	ITEM_PET_SKILL_STUDY_END},			//������
		{ITEM_PET_RETURN_BABAY_BEGIN-1, ITEM_PET_RETURN_BABAY_END+1},		//��ͯ
		{ITEM_PET_ADD_LIFE_BEGIN,		ITEM_PET_ADD_LIFE_END},				//�ӳ�����
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
	//��¡GUID
	USHORT idWorld		= 0;
	USHORT idServer		= 0;
	UINT idSerail		= 0;
	pItemSource->GetGUID(idWorld, idServer, idSerail);
	SetGUID(idWorld, idServer, idSerail);
}


//��Ʒ������֤
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
			//return "����Ʒ�����Զ���";
		}
		break;
	case RULE_OVERLAY:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_Overlay");
			return strTemp;
			//return "����Ʒ�����Ե���";
		}
		break;

	case RULE_QUICK:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_In_ShortCut");
			return strTemp;
			//return "����Ʒ�����Է�������";
		}
		break;

	case RULE_SALE:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_Sell");
			return strTemp;
			//return "����Ʒ����������";
		}
		break;

	case RULE_EXCHANGE:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_Exchange");
			return strTemp;
			//return "����Ʒ�����Խ���";
		}
		break;

	case RULE_USE:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_Use");
			return strTemp;
			//return "����Ʒ������ʹ��";
		}
		break;

	case RULE_PICKBIND:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Bind");
			return strTemp;
			//return "����Ʒ�Ѱ�";
		}
		break;

	case RULE_EQUIPBIND:
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Bind");
			return strTemp;
			//return "����Ʒ�Ѱ�";
		}
		break;

	case RULE_UNIQUE:
		{
			return "";
		}
		break;
	case RULE_IDENT://����Ʒ����Ҫ����	
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Not_Need_Ident");
			return strTemp;
		}
		break;
	case RULE_DUMMYITEM://����Ʒ�����������	
		{
			return "";
		}
		break;
	case RULE_BANK://����Ʒ�����Դ�������
		{
			strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_InBank");
			return strTemp;
		}
		break;
	case RULE_CONSUME://����Ʒ������
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

// �õ����ʹ�������Ʒ��Ҫ�ĵȼ�
INT	CObject_Item::GetNeedLevel()
{
	// ��ʾû���������
	return -1;
}

// �õ���Ʒ�;ö�
INT	CObject_Item::GetItemDur()
{
	
	return -1;
}

// �õ���Ʒ����;ö�
INT	CObject_Item::GetItemMaxDur()
{

	return -1;
}

// �õ���Ʒ��������� 
INT	CObject_Item::GetItemRepairCount()
{
	// -1 ��ʾ��������Ϣ
	return -1;
}

// �õ���Ʒ�İ���Ϣ 
INT	CObject_Item::GetItemBindInfo()
{
	// -1 �ް���Ϣ
	return -1;
}

// �õ���Ʒ��2������Ϣ 
INT	CObject_Item::GetItemSecLevelBindInfo()
{
	// -1 �ް���Ϣ
	return -1;
}



//	�õ�װ����2�ΰ󶨼ӳ�

LPCTSTR	CObject_Item::GetItemBindAddInfo()
{
	// -1 �ް���Ϣ
	return _T("");
}


// 20100408 AddCodeBegin
// �õ���Ʒ����ļ۸�
 INT CObject_Item::GetItemBasePrice()
{
	 return -1;
}
// 20100408 AddCodeEnd


// �õ���Ʒ����npc�ļ۸�
INT	CObject_Item::GetItemPrice()
{
	// -1 �޼۸���Ϣ
	 return -1;
}


//�õ���Ʒְҵ
INT CObject_Item::GetItemProfession()
{
  // -1 ��Чְҵ
	
	return -1;
}

//�õ���Ʒ�Ƿ�Ψһ
INT	CObject_Item::GetItemRuleOnly()					  
{       
	return m_Rule.bUnique;

}
// �õ���Ʒ��������
LPCTSTR	CObject_Item::GetManufacturer()
{
	return _T("");
}

// �õ���ɫ���� 
LPCTSTR	CObject_Item::GetBaseWhiteAttrInfo()
{
	return _T("");
}


//�õ�����ʱ��ģ��id
 LPCTSTR	 CObject_Item::GetDropVisualID(VOID)const
{
	return NULL;
}

//�õ�����ʱ����ɫ
LPCTSTR	 CObject_Item::GetDropVisColor(VOID)const
{
	return NULL;
}


// �õ������Ʒ�ĵȼ�
INT	 CObject_Item::GetItemLevelTable(VOID)const
{

	return -1;

}
 
// ������Ʒ����״̬  
void CObject_Item::SetItemSaveStatus(SAVE_ITEM_STATUS nSaveStatus)
{
	m_nItemSaveStatus = nSaveStatus;
}

//������Ʒ����״̬  
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

// �õ���Ʒ���������� 
LPCTSTR	CObject_Item::GetItemTableTypeDesc()
{
	return "����������";
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
