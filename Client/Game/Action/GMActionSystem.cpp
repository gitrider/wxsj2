
/**	GMActionSystem.cpp
 *	
 *	功能：	
 *
 *	修改记录：
 *			080421	增加鼠标命令： 好友和交易
 *			
 *			
 */

#include "StdAfx.h"
#include "network\NetManager.h"
#include "GMActionSystem.h"
#include "Event\GMEventSystem.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\GMDataPool.h"
#include "GIException.h"
#include "Procedure\GameProcedure.h"
#include "Interface\GMGameInterface.h"
#include "DataPool\DetailAttrib_Player.h"
#include "GIObjectManager.h"
#include "Script\GMScriptSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Interface\GMGameInterface.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "Object\Logic\Item\Obj_Item_Medicine.h"
#include "SkillDataMgr.h"
#include "GIDBC_Struct.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "GIObjectManager.h"
#include "GIVariable.h"
#include "GIUtil.h"
#include "DBC\GMDataBase.h"
#include "DataPool\GMDP_CharacterData.h"
#include "DataPool\GMDP_Struct_Quest.h"
#include "CGBankAddItem.h"
#include "CGBankRemoveItem.h"
#include "CGBankSwapItem.h"
#include "CGModifySetting.h"
#include "GMActionSystem_Item.h"
#include "GMActionSystem_LifeAbility.h"
#include "GMActionSystem_Skill.h"
#include "GMActionSystem_XinFa.h"
#include "GMActionSystem_PetSkill.h"
#include "Interface\GMInterface_Script_Talk.h"
#include "GMActionSystem_ChatMood.h"
#include "GIInputSystem.h"
#include "Interface\GMInterface_Script_Tooltip.h"
#include "gamedefine2.h"
#include "GMActionSystem_MouseCmd.h"
#include "..\client\cegui\cegui\include\CEGUI.h"
#include "..\client\cegui\cegui\include\CEGUIIme.h"
#include "..\client\cegui\cegui\include\CEGUIPropertyHelper.h"
#include "..\Interface\GMGameInterface_Script_SouXia.h"
#pragma comment(lib, "CEGUIBase")

const int		ActionButtonNum			= 30;
const char*		NAMETYPE_SKILL			= "skill";
const char*		NAMETYPE_LIFESKILL		= "life";
const char*		NAMETYPE_PACKAGEITEM	= "packageitem";
const char*		NAMETYPE_BANKITEM		= "bankitem";
const char*		NAMETYPE_BOOTITEM		= "boothitem";
const char*		NAMETYPE_LOOTITEM		= "lootitem";
const char*		NAMETYPE_XINFA			= "xinfa";
const char*		NAMETYPE_PETSKILL		= "petskill";
const char*		NAMETYPE_VIRTUALITEM	= "virtualitem";
const char*		NAMETYPE_EQUIP			= "equip";
const char*		NAMETYPE_EXCHANGESELF	= "ex_self";
const char*		NAMETYPE_EXCHANGEOTHER	= "ex_other";
const char*		NAMETYPE_MISSIONREFER	= "mission";
const char*		NAMETYPE_APPENDITEM		= "appenditem";
const char*		NAMETYPE_STALLSELF		= "st_self";
const char*		NAMETYPE_STALLOTHER		= "st_other";
const char*		NAMETYPE_OTHEREQUIP		= "targetequip";
const char*		NAMETYPE_PS_SELF		= "ps_self";
const char*		NAMETYPE_PS_OTHER		= "ps_other";
const char*		NAMETYPE_CHATMOOD		= "chatmood";
const char*		NAMETYPE_PS_DROP		= "dropitem";
const char*		NAMETYPE_CHANGE_SUIT	= "changesuit";
const char*		NAMETYPE_TRANSFER_ITEM	= "transferitem";
const char*		NAMETYPE_IB_SHOP		= "ib_shopitem";
const char*		NAMETYPE_MOUNTITEM	    = "mountitem";        // [7/20/2010 陈军龙]

//======================================================================

CActionItem CActionItem::s_InvalidAction;
LuaPlus::LuaObject* CActionItem::s_pMetaTable = NULL;


CActionItem::CActionItem(INT id)
{
	m_ID		= id;
	m_bChecked	= FALSE;
	m_bEnabled	= TRUE;
}

CActionItem::~CActionItem()
{
	ClearRefrence();

	if(CGameProcedure::s_pGameInterface && this==CGameProcedure::s_pGameInterface->Skill_GetActive())
	{
		CGameInterface::GetMe()->Skill_SetActive( /*CActionSystem::GetMe()->GetMyDefaultSkillAtion()*/NULL );
	}

	if(CGameProcedure::s_pActionSystem && this==CActionSystem::GetMe()->GetDefaultAction())
	{
		CActionSystem::GetMe()->SetDefaultAction( /*CActionSystem::GetMe()->GetMyDefaultSkillAtion()*/NULL );
	}
}


VOID CActionItem::ClearRefrence(VOID)
{
	std::set< tActionReference* >::iterator it;
	for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
	{
		(*it)->BeDestroyed();
		//m_setReference.erase(it);
	}
	//m_setReference.clear();
}

VOID CActionItem::UpdateToRefrence(VOID)
{
	std::set< tActionReference* >::iterator it;
	for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
	{
		(*it)->Update(GetID());
	}
}

INT	CActionItem::Lua_GetID(LuaPlus::LuaState* pState)	//得到ID
{
	INT id = GetID();
	pState->PushInteger(id);
	return 1;
}

INT CActionItem::Lua_GetPrice(LuaPlus::LuaState* pState)	//得到价格
{
	pState->PushInteger(GetItemPrice());
	return 1;
}

INT CActionItem::Lua_GetNum(LuaPlus::LuaState* pState)	//得到每个格子里头的数量
{
	pState->PushInteger(GetNum());
	return 1;
}

INT	CActionItem::Lua_GetType(LuaPlus::LuaState* pState)	//得到类型
{
//	AxTrace(0, 1, "Item:%d", CObject_Item::Debug_GetNum());
//	AxTrace(0, 1, "Action:%d", CActionSystem::GetMe()->m_mapItem.size());
	pState->PushString( GetType_String() );
	return 1;
}

INT CActionItem::Lua_GetDesc(LuaPlus::LuaState* pState)
{
	pState->PushString(GetDesc());
	return 1;
}

INT CActionItem::Lua_GetName(LuaPlus::LuaState* pState)
{
	pState->PushString(GetName());
	return 1;
}

//得到在资源表中的ID
INT	CActionItem::Lua_GetDefineID(LuaPlus::LuaState* pState)
{
	pState->PushInteger(GetDefineID());
	return 1;
}

INT CActionItem::Lua_GetOwnerXinfa(LuaPlus::LuaState* pState)
{
	if(GetType() != AOT_SKILL)
	{
		pState->PushNumber(-1);
		return 1;
	}

	//const SCLIENT_SKILL* pSkill = (const SCLIENT_SKILL*)GetImpl();
	//if(!pSkill || pSkill->m_pDefine->m_nClassByUser != A_SKILL_FOR_PLAYER)
	//{
	//	pState->PushNumber(-1);
	//	return 1;
	//}

	//CActionItem* pXinFaActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction_XinFaID(pSkill->m_pDefine->m_nXinFaID);
	//if(!pXinFaActionItem)
	//{
	//	pState->PushNumber(-9999);
	//	return 1;
	//}


	//pState->PushNumber(pXinFaActionItem->GetID());
	pState->PushNumber(0);
	return 1;
}

INT CActionItem::Lua_GetPetSkillOwner(LuaPlus::LuaState* pState)
{
	/*if(GetType() != AOT_PET_SKILL)
	{
		pState->PushNumber(-1);
		return 1;
	}

	const PET_SKILL* pSkill = (const PET_SKILL*)GetImpl();
	if(!pSkill)
	{
		pState->PushNumber(-1);
		return 1;
	}

	pState->PushNumber(pSkill->m_nPetNum);*/
	return 1;
}

INT	CActionItem::Lua_GetOwnerQuest(LuaPlus::LuaState* pState)
{
	if(GetType() != AOT_ITEM)
	{
		pState->PushNumber(-1);
		return 1;
	}

	CActionItem_Item* pItem = (CActionItem_Item*)(tObject_Item*)GetImpl();
	if(!pItem)
	{
		pState->PushNumber(-1);
		return 1;
	}

	pState->PushNumber(pItem->GetQuestIndex());
	return 1;
}

VOID CActionItem::DoActionPrepare(BOOL bCheck)
{
	//显示按钮按下动画
	std::set< tActionReference* >::iterator it;
	for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
	{
		(*it)->SetCheck(bCheck);
	}
}

VOID CActionItem::AddReference(tActionReference* pRef,BOOL bIsInMenuToolbar)
{
	//如果没有,加入
	if(m_setReference.find(pRef) == m_setReference.end()) 
	{
		m_setReference.insert(pRef);
	}
	//刷新Check信息
	pRef->SetCheck(m_bChecked);
	pRef->SetDefault(CActionSystem::GetMe()->GetDefaultAction() == this);

	switch(GetType())
	{
	case AOT_ITEM:
		{
			//物品的每组的个数显示在右下角
			tObject_Item* pItem = (tObject_Item*)GetImpl();
			
			if( pItem->GetTypeOwner() == tObject_Item::IO_BOOTH )
			{
				INT nGroupNumber = GetNum();
				if(nGroupNumber > 1)
				{
					CHAR szTemp[32];
					_snprintf(szTemp, 32, "%d", nGroupNumber);
					pRef->SetCornerChar(tActionReference::ANP_BOTRIGHT, szTemp);
				}
				else
				{
					pRef->SetCornerChar(tActionReference::ANP_BOTRIGHT, "");
				}

				//有限物品的最大数量显示在左上角
				INT nMaxNumber = pItem->GetMax();
				if(nMaxNumber > 1)
				{
					CHAR szTemp[32];
					_snprintf(szTemp, 32, "%d", nMaxNumber);
					pRef->Enable();
					pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, szTemp);
				}
				else if(nMaxNumber == 0)
				{
					pRef->Disable();
					pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, "");
				}
				else
				{
					pRef->Enable();
					pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, "");
				}
			}
			else if( pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET 
				|| pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_BANK 
				|| pItem->GetTypeOwner() == tObject_Item::IO_MYEXBOX 
				|| pItem->GetTypeOwner() == tObject_Item::IO_OTHEREXBOX
				|| pItem->GetTypeOwner() == tObject_Item::IO_MISSIONBOX
				|| pItem->GetTypeOwner() == tObject_Item::IO_MYSTALLBOX
				|| pItem->GetTypeOwner() == tObject_Item::IO_OTSTALLBOX
				|| pItem->GetTypeOwner() == tObject_Item::IO_QUESTVIRTUALITEM
				|| pItem->GetTypeOwner() == tObject_Item::IO_ITEMBOX
				|| pItem->GetTypeOwner() == tObject_Item::IO_PS_SELFBOX
				|| pItem->GetTypeOwner() == tObject_Item::IO_PS_OTHERBOX
                )
			{
				INT nGroupNumber = GetNum();
				//AxTrace(0, 0, "%s:GroupNumber=%d", pItem->GetName(), nGroupNumber);
				if(bIsInMenuToolbar == TRUE)
				{
					CHAR szTemp[32];
					_snprintf(szTemp, 32, "%d", CDataPool::GetMe()->UserBag_CountItemByIDTable(pItem->GetIdTable()));
					pRef->SetCornerChar(tActionReference::ANP_BOTRIGHT, szTemp);
				}
				else if(nGroupNumber > 1)
				{
					CHAR szTemp[32];
					_snprintf(szTemp, 32, "%d", nGroupNumber);
					pRef->SetCornerChar(tActionReference::ANP_BOTRIGHT, szTemp);
				}
				else
				{
					pRef->SetCornerChar(tActionReference::ANP_BOTRIGHT, "");
				}
			}
		}
		break;

	default:
		pRef->SetCornerChar(tActionReference::ANP_BOTRIGHT, "");
		break;
	}


	UpdateCoolDown();
}

VOID CActionItem::RemoveRefrence(tActionReference* pRef)
{
	if(m_setReference.find(pRef) == m_setReference.end()) return;

	m_setReference.erase(pRef);

	pRef->SetCheck(false);
	pRef->SetDefault(false);
}

VOID CActionItem::SetCheckState(BOOL bCheck)
{
	m_bChecked = bCheck;
	std::set< tActionReference* >::iterator it;

	for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
	{
		(*it)->SetCheck(bCheck);
	}
}

VOID CActionItem::SetDefaultState(BOOL bCheck)
{
	std::set< tActionReference* >::iterator it;

	for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
	{
		(*it)->SetDefault(bCheck);
	}
}

VOID CActionItem::UpdateCoolDown(VOID)
{
	std::set< tActionReference* >::iterator it;
	INT nCoolDownID = GetCoolDownID();

	//对于没有冷却的Action
	if(-1 == nCoolDownID)
	{
		for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
		{
			(*it)->EnterCoolDown(-1, 0.0f);
		}
		return;
	}

	//取得冷却组数据
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
			CDataPool::GetMe()->CoolDownGroup_Get(nCoolDownID);

	INT nTimeNow   = pCoolDownGroup->nTime;
	INT nTimeTotal = pCoolDownGroup->nTotalTime;

	for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
	{
		if(nTimeNow <= 0 || nTimeTotal <= 0)
		{
			(*it)->EnterCoolDown(-1, 0.0f);
		}
		else
		{
			(*it)->EnterCoolDown(nTimeTotal, 1.0f - (FLOAT)nTimeNow/(FLOAT)nTimeTotal);
		}
	}
}

VOID CActionItem::UpdateCommonCoolDown(VOID)
{
	//药品也要走cooldown
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetImpl();
	if(!pItem)
	{
		return;
	}

    //针对技能Action
	if(GetType() != AOT_SKILL /*&& pItem->GetItemClass() != ICLASS_COMITEM*/ ) return;

	//取得冷却组数据
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
			CDataPool::GetMe()->CoolDownGroup_Get(GetCoolDownID());
	if(!pCoolDownGroup) return;

	//如果处于冷却中
	if(pCoolDownGroup->nTime > 0) return;
	
	INT nCommonCoolDown = CDataPool::GetMe()->CommonCoolDown_Get();

	//通知UI，进入公共冷却
	std::set< tActionReference* >::iterator it;
	for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
	{
		(*it)->EnterCoolDown(nCommonCoolDown, 1.0f - (FLOAT)nCommonCoolDown/(FLOAT)COMMONCOOLDOWN_TIME );
	}
}

//显示tooltips
VOID CActionItem::NotifyTooltipsShow(int btn_left, int btn_top, int btn_right, int btn_bottom)
{
	//生成数据
	SCRIPT_SANDBOX::SuperTooltips::s_SupperTooltip.SetActionItem(this);

	//产生UI事件
	std::vector< STRING > vParam;
	vParam.push_back("1");
	vParam.push_back(GetType_String());
	
	//POINT ptMouse = CGameProcedure::s_pInputSystem->MouseGetPos();

	char szTemp[32];
	_snprintf(szTemp, 32, "%d", btn_left);
	vParam.push_back(szTemp);
	_snprintf(szTemp, 32, "%d", btn_top);
	vParam.push_back(szTemp);
	_snprintf(szTemp, 32, "%d", btn_right);
	vParam.push_back(szTemp);
	_snprintf(szTemp, 32, "%d", btn_bottom);
	vParam.push_back(szTemp);

	CEventSystem::GetMe()->PushEvent(GE_SUPERTOOLTIP, vParam);
}

//隐藏Tooltip
VOID CActionItem::NotifyTooltipsHide(VOID)
{
	//生成数据
	SCRIPT_SANDBOX::SuperTooltips::s_SupperTooltip.SetActionItem(NULL);

	//隐藏Tooltip
	CEventSystem::GetMe()->PushEvent(GE_SUPERTOOLTIP, "0");
}


// 返回物品的等级
INT	CActionItem::GetItemLevel()
{
	// -1 表示物品没有等级.
	return -1;
}

// 得到物品的耐久
INT	CActionItem::GetItemDur()
{
	// -1 表示物品没有这个属性
	return -1;
}

// 得到物品的最大耐久
INT	CActionItem::GetItemMaxDur()
{
	// -1 表示物品没有这个属性
	return -1;
}



// 得到物品的修理次数
INT	CActionItem::GetItemRepairCount()
{
	// -1 表示物品没这个属性.
	return -1;
}

// 得到物品的绑定信息 
INT	CActionItem::GetItemBindInfo()
{
	// -1 表示物品没这个属性
	return -1;
}


// 得到装备的2次绑定信息 
INT	CActionItem::GetItemSecLevelBindInfo()

{
	// -1 表示物品没这个属性
	return -1;
}
//	得到装备的2次绑定加成
LPCTSTR	CActionItem::GetItemBindAddInfo()
 {

	 return _T("");
 }

 
//	得到掉落颜色
 LPCTSTR	CActionItem::GetDropVisColor()
 {

	 return _T("");
 }


// 得到物品卖给npc的价格
INT	CActionItem::GetItemPrice()
{
	// -1 表示物品没这个属性
	return -1;
}

//得到物品职业
INT CActionItem::GetItemProfession()

 {
	 // -1 表示没有这个属性
	 return -1;
 }



//得到装备牌型
INT CActionItem::GetEquipCard()

{
	// 0 表示没有这个属性
	return 0;
}


 //得到物品是否唯一
 INT CActionItem::GetItemRuleOnly()
 {

	 return -1;
 }


// 得到物品的制作人
LPCTSTR	CActionItem::GetManufacturer()
{
	return _T("");
}
 INT	CActionItem::GetEquipHoleNum()
{
	return 0;
}

//得到装备的套装信息
LPCTSTR		CActionItem::GetEquipSetInfo()

{
	return _T("");
}

//得到装备牌型激活的信息 
LPCTSTR		CActionItem::GetEquipSpritePaiInfo()

{
	return _T("");
}


//得到其他玩家装备小激活的信息
LPCTSTR		CActionItem::GetOtherEquipSpriteChainInfo()

{
	return _T("");
}


//得到装备小激活的信息
LPCTSTR		CActionItem::GetEquipSpriteChainInfo()

{
	return _T("");
}


//得到其他玩家装备大激活的信息
 
LPCTSTR		CActionItem::GetOtherEquipSpriteAllActiveInfo()

{
	return _T("");
}



//得到装备大激活的信息

LPCTSTR		CActionItem::GetEquipSpriteAllActiveInfo()

{
	return _T("");
}


//得到装备的套装信息
LPCTSTR		CActionItem::GetOtherEquipSetInfo()

{
	return _T("");
}




//得到装备套装激活的属性信息
LPCTSTR		CActionItem::GetEquipSetActiveAttr()

{
	return _T("");
}

//得到装备套装激活的属性信息
LPCTSTR		CActionItem::GetOtherEquipSetActiveAttr()

{
	return _T("");
}

INT	CActionItem::GetStartLevel()
{
	return 0;
}


// 得到白色属性              
LPCTSTR	CActionItem::GetBaseWhiteAttrInfo()
{
	return _T("");
}

// 得到action所引用的item 的类型
INT	CActionItem::GetItemType()
{
	return -1;
}


// 得到类型描述
LPCTSTR	CActionItem::GetTypeDesc()
{
	return "无类型描述";
}


//==================================================================

KL_IMPLEMENT_DYNAMIC(CActionSystem, GETCLASS(tActionSystem));
CActionSystem* CActionSystem::s_pMe = NULL;


CActionSystem::CActionSystem()
{
	s_pMe = this;
	m_pDefaultAction = NULL;
	m_pSetoutAction =NULL;

	m_nCurShortcutKeyState = 0;
	m_NotCancleIden = false;
	//m_nCurShortcutKeyFState = 0;
}

CActionSystem::~CActionSystem()
{
}


VOID CActionSystem::Initial(VOID*)
{
	// 加载主菜单配置
	const INT s_MainMenuBarNum	= ActionButtonNum;

	for(INT i=0; i<s_MainMenuBarNum; i++)
	{
		ACTIONBAR_ITEM newItem;
		newItem.idActionItem = INVALID_UID;
		newItem.typeItem = AOT_EMPTY;
		m_barMain.push_back(newItem);
	}

	UserLifeAbilityStudy_Init();

	// 注册关心的事件
	// 技能开始使用
	CEventSystem::GetMe()->RegisterEventHandle("ON_SKILL_ACTIVE", _OnSkillActive);

	// 创建鼠标操作Action - 修理
	CActionItem_MouseCmd_Repair* pRepairAction = new CActionItem_MouseCmd_Repair(_CreateID());
	m_mapAllAction[pRepairAction->GetID()] = pRepairAction;

	// 创建鼠标操作Action - 鉴定
	CActionItem_MouseCmd_Identify* pIdenAction = new CActionItem_MouseCmd_Identify(_CreateID());
	m_mapAllAction[pIdenAction->GetID()] = pIdenAction;

	CActionItem_ChangeSuit* pSuitAction = new CActionItem_ChangeSuit(_CreateID());
	m_mapAllAction[pSuitAction->GetID()] = pSuitAction;

	// 好友
	CActionItem_MouseCmd_Friend* pFriendAction = new CActionItem_MouseCmd_Friend(_CreateID());
	m_mapAllAction[pFriendAction->GetID()] = pFriendAction;

//	// 交易
//	CActionItem_MouseCmd_Exchange* pExchangeAction = new CActionItem_MouseCmd_Exchange(_CreateID());
//	m_mapAllAction[pExchangeAction->GetID()] = pExchangeAction;

}

VOID CActionSystem::Release(VOID)
{
	std::map< INT, CActionItem* >::iterator it;
	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
	{
		if(it->second) 
			delete (it->second);

		it->second = NULL;
	}

	m_mapAllAction.clear();


	//LIFEABILITYSTUDY_INFO_ITER iter = m_AllAbilityStudyInfo.begin();
	//for (; iter != m_AllAbilityStudyInfo.end(); ++iter )
	//{
	//	if( iter->pAction )
	//		delete (iter->pAction);
	//	iter->pAction = NULL;
	//}
	//m_AllAbilityStudyInfo.clear();

}

VOID CActionSystem::SaveAction(VOID)
{
	for(INT i=0; i<(INT)m_barMain.size(); i++)
	{
		ACTIONBAR_ITEM& barItem = m_barMain[i];

		tActionItem* pActionItem = NULL;
		CGModifySetting msg;
		_OWN_SETTING* pSend_Value = msg.GetValue();

		switch(barItem.typeItem)
		{
			case AOT_PET_SKILL:
				pActionItem = GetAction_PetSkillID(barItem.m_GUID, barItem.idImpl);
				
//				if(m_bIsInitalDone == TRUE)
//				{
					msg.SetType(i+SETTING_TYPE_K0);
					pSend_Value->m_SettingType = barItem.typeItem;
					//pSend_Value->m_SettingData = (barItem.idImpl << 16) + barItem.idPetIndex;
					pSend_Value->m_SettingData = (barItem.m_GUID.m_uLowSection << 16) + barItem.idImpl;

					CNetManager::GetMe()->SendPacket( &msg );
//				}

				break;
			case AOT_SKILL:
				pActionItem = GetAction_SkillID(barItem.idImpl);

//				if(m_bIsInitalDone == TRUE)
//				{
					msg.SetType(i+SETTING_TYPE_K0);
					pSend_Value->m_SettingType = barItem.typeItem;
					pSend_Value->m_SettingData = barItem.idImpl;

					CNetManager::GetMe()->SendPacket( &msg );
//				}
				break;

			case AOT_SKILL_LIFEABILITY:
				break;
			case AOT_ITEM:
				pActionItem = GetAction_ItemIDTable_FromMyPacket(barItem.idImpl);
				if(pActionItem)
				{
					barItem.idItemCount = CDataPool::GetMe()->UserBag_CountItemByIDTable(barItem.idImpl);
				}
				else
				{
					barItem.idImpl = INVALID_ID;					
				}

//				if(m_bIsInitalDone == TRUE)
//				{
					msg.SetType(i+SETTING_TYPE_K0);
					pSend_Value->m_SettingType = barItem.typeItem;
					pSend_Value->m_SettingData = barItem.idImpl;

					CNetManager::GetMe()->SendPacket( &msg );
//				}

				break;
		/*	case AOT_XINFA:
				break;*/

			case AOT_EMPTY:
//				if(m_bIsInitalDone == TRUE)
//				{
					msg.SetType(i+SETTING_TYPE_K0);
					pSend_Value->m_SettingType = barItem.typeItem;
					pSend_Value->m_SettingData = barItem.idImpl;
					//给服务器发送CGMODIFYSETTING

					CNetManager::GetMe()->SendPacket( &msg );
//				}
				break;

			default:
				break;
			}
	}


}

VOID CActionSystem::Tick(VOID)
{
}

VOID CActionSystem::UserSkill_Update(VOID)
{
	CleanInvalidAction();

	// 技能列表
	const SSKILL_MAP& mapSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill();

	SSKILL_MAP::const_iterator it;
	for( it=mapSkill.begin(); it!=mapSkill.end(); it++ )
	{
		UpdateAction_FromSkill(&(it->second));
	}
}

tActionItem* CActionSystem::GetMyDefaultSkillAtionBySkillId(INT nSkillId)
{
	// 查找该技能的ActionItem
	CActionItem_Skill* pSkillAction = (CActionItem_Skill*)GetAction_SkillID(nSkillId);
	return pSkillAction;
}

tActionItem* CActionSystem::GetMyDefaultSkillAtion()
{
	// 角色的技能列表中，第一个技能为默认技能
	INT nSkillID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_DefaultSkillID();
	// 查找该技能的ActionItem
	CActionItem_Skill* pSkillAction = (CActionItem_Skill*)GetAction_SkillID(nSkillID);
	return pSkillAction;
}

VOID CActionSystem::UserPetSkill_Update(INT nPetNum)
{
	// 清除无效操作
	CleanInvalidAction();

	// 更新坐骑技能操作
	const SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nPetNum);
	if (pPet)
	{
		// 技能列表
		for(INT i=0; i < MAX_PET_SKILL_COUNT; i++)
		{
			const PET_SKILL& skill = pPet->m_Skill[i];			
			if(skill.m_pDefine)
			{
				UpdateAction_FromPetSkill(pPet->m_GUID, &skill, i);
			}
		}
	}
	// 更新快捷栏
	UpdateToolBarForPetSkill();

	// 更新坐骑界面
	CEventSystem::GetMe()->PushEvent(GE_MOUNT_ATTR_UPDATE);
}

VOID CActionSystem::UserTargetPetSkill_Update(VOID)
{
	CleanInvalidAction();

	const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
	if (pPet)
	{
		//技能列表
		for(INT i=0; i < MAX_PET_SKILL_COUNT; i++)
		{
			const PET_SKILL& skill = pPet->m_Skill[i];
			if(skill.m_pDefine)
			{
				UpdateAction_FromPetSkill(pPet->m_GUID, &skill, i);
			}
		}
	}
}

VOID CActionSystem::UserPetSkillStudy_Update(VOID)
{
	/*CleanInvalidAction();
	
	INT i=0;
	const PET_SKILL* pSk = CDataPool::GetMe()->PetSkillStudy_MenPaiList_Get(i);

	while(pSk)
	{
		if(!pSk || !(pSk->m_pDefine)) break;
		UpdateAction_FromPetSkillStudy(pSk);
		pSk = CDataPool::GetMe()->PetSkillStudy_MenPaiList_Get(++i);
	}*/
}


VOID CActionSystem::UserLifeAbilityStudy_Init()
{
	std::map<STRING, INT> dbc_file;
	dbc_file.clear();
	dbc_file.insert( make_pair( "caikuang.tab",DBC_CAIKUANG_LEVELINFO ) );
	dbc_file.insert( make_pair( "shouge.tab",DBC_SHOUGE_LEVELINFO ) );
	dbc_file.insert( make_pair( "buzhuo.tab",DBC_BUZHUO_LEVELINFO ) );
	dbc_file.insert( make_pair( "zhuzao.tab",DBC_ZHUZAO_LEVELINFO ) );
	dbc_file.insert( make_pair( "zhiyi.tab",DBC_ZHIYI_LEVELINFO ) );
	dbc_file.insert( make_pair( "gongjiang.tab",DBC_GONGJIANG_LEVELINFO ) );
	dbc_file.insert( make_pair( "pengren.tab",DBC_PENGREN_LEVELINFO ) );


	const tDataBase* pLifeAbilityDBC = CDataBaseSystem::GetMe()->GetDataBase( DBC_LIFEABILITY_DEFINE );
	for ( INT i=1; i<=pLifeAbilityDBC->GetRecordsNum(); ++i )
	{
		const _DBC_LIFEABILITY_DEFINE* pData = (const _DBC_LIFEABILITY_DEFINE*)pLifeAbilityDBC->Search_Index_EQU(i);
		if( !pData )continue;
		INT idDataBase= dbc_file[pData->szLevelupTable];
		const tDataBase* pAbilityLevel = CDataBaseSystem::GetMe()->GetDataBase( idDataBase );
		for ( INT j=1; j<=pAbilityLevel->GetRecordsNum(); ++j )
		{
			//AbilityStudyInfo abilityInfo;
			//abilityInfo.nAbilityId = i;
			//abilityInfo.nLevel = j;

			SCLIENT_LIFEABILITY *newLifeAbility = new SCLIENT_LIFEABILITY;
			newLifeAbility->m_pDefine = pData;
			newLifeAbility->m_nPosIndex = (INT)m_mapAllAction.size();
			newLifeAbility->m_nLevel = j;

			CActionItem_LifeAbility* pLifeAbilityAction = new CActionItem_LifeAbility(_CreateID());
			pLifeAbilityAction->Update_LifeAbility(newLifeAbility);
			pLifeAbilityAction->Set_LifeStudy(TRUE);
			pLifeAbilityAction->Set_Sclient_lifeability( newLifeAbility );
			m_mapAllAction[pLifeAbilityAction->GetID()] = pLifeAbilityAction;
			//abilityInfo.pAction = pLifeAbilityAction;

			//m_AllAbilityStudyInfo.push_back(abilityInfo);
			//m_AllAbilityStudyInfo.insert( make_pair(abilityInfo,(CActionItem*)pLifeAbilityAction) );

		}

	}
	//const _DBC_LIFEABILITY_DEFINE* pDefine = (const _DBC_LIFEABILITY_DEFINE*)s_pLifeAbilityDBC->Search_Index_EQU(nID);
}

tActionItem* CActionSystem::UserLifeAbilityStudy_Get( INT nAbilityID, INT nLevel )
{
	//if( m_AllAbilityStudyInfo.empty() )
	//	UserLifeAbilityStudy_Set();

	//LIFEABILITYSTUDY_INFO_ITER iter = m_AllAbilityStudyInfo.begin();

	//for ( ; iter != m_AllAbilityStudyInfo.end(); ++iter )
	//{
	//	if( (*iter).nAbilityId == nAbilityID && (*iter).nLevel == nLevel )
	//	{
	//		return (*iter).pAction->GetID();
	//	}
	//}

	//return -1;

	register std::map< INT, CActionItem* >::iterator it;

	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
	{
		CActionItem* pActionItem = (CActionItem*) it->second;

		//如果是生活技能
		if(	pActionItem && 
			pActionItem->GetType() == AOT_SKILL_LIFEABILITY )
		{
			const SCLIENT_LIFEABILITY* pAbility = (const SCLIENT_LIFEABILITY*)pActionItem->GetImpl();
			if(pAbility && pAbility->m_pDefine->nID == nAbilityID && 
				((CActionItem_LifeAbility*)pActionItem)->Get_LifeLevel() == nLevel &&
				((CActionItem_LifeAbility*)pActionItem)->Get_LifeStudy()				
				) return pActionItem;
		}
	}

	return NULL;
}


VOID CActionSystem::UserLifeAbility_Update(VOID)
{
	CleanInvalidAction();

	//技能
	const SLIFEABILITY_MAP& mapLifeAbility = 
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility();

	SLIFEABILITY_MAP::const_iterator it;
	for(it=mapLifeAbility.begin(); it!=mapLifeAbility.end(); it++)
	{
		UpdateAction_FromLifeAbility(&(it->second));
	}
	CGameProcedure::s_pEventSystem->PushEvent(GE_UPDATE_LIFESKILL_PAGE);
}

//VOID CActionSystem::XinFa_Update(VOID)
//{
//	CleanInvalidAction();
//
//	//心法// $$
//	/*const SXINFA_MAP& mapXinFa = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_XinFa();
//
//	SXINFA_MAP::const_iterator it;
//	for(it=mapXinFa.begin(); it!=mapXinFa.end(); it++)
//	{
//		UpdateAction_FromXinFa(&(it->second));
//	}*/
//}

VOID CActionSystem::CleanInvalidAction(BOOL bForceCleanItemBox)
{
	register std::map< INT, CActionItem* >::iterator it;
	for(it=GetMe()->m_mapAllAction.begin(); it!=GetMe()->m_mapAllAction.end();)
	{
		std::map< INT, CActionItem* >::iterator itNext = it;
		itNext++;

		CActionItem* pActionItem = it->second;
		VOID* pImpl = pActionItem->GetImpl();
		if(!pImpl) 
		{
			//销毁-
			EraseActionItem(it->second);
		}

		it = itNext;
	}
}

VOID CActionSystem::UserBag_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();

	for(INT i=0; i<MAX_BAG_SIZE; i++)
	{
		//取得物品
		tObject_Item*	pItem = CDataPool::GetMe()->UserBag_GetItem(i);
		if(pItem)
		{
			//设置新的物品数据
			GetMe()->UpdateAction_FromItem(pItem);
		}
	}
}

// 通过数据池中的数据, 创建出来装备action item
VOID CActionSystem::UserEquip_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();

	// 装备点的最大个数.
	for(INT i=0; i<HEQUIP_NUMBER; i++)
	{
		//取得玩家身上的装备
		tObject_Item*	pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)i);
		if(pItem)
		{
			//创建新的装备的action item
			GetMe()->UpdateAction_FromItem(pItem);
		}
	}
}

// 通过数据池中的数据, 创建出来坐骑action item    [7/8/2010 陈军龙] 
VOID CActionSystem::UserMount_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();

	// 坐骑的最大个数.
	for(INT i=0; i<MAX_MOUNT_SIZE; i++)
	{
		//取得玩家的坐骑
		tObject_Item*	pItem = CDataPool::GetMe()->UserMount_GetItem(i);
		if(pItem)
		{
			//创建新的坐骑的action item
			GetMe()->UpdateAction_FromItem(pItem);
		}
	}
}

// 通过数据池中的数据, 创建出来装备action item
VOID CActionSystem::OtherPlayerEquip_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();

	// 装备点的最大个数.
	for(INT i=0; i<HEQUIP_NUMBER; i++)
	{
		//取得玩家身上的装备
		tObject_Item*	pItem = CDataPool::GetMe()->OtherPlayerEquip_GetItem((HUMAN_EQUIP)i);
		if(pItem)
		{
			//创建新的装备的action item
			GetMe()->UpdateAction_FromItem(pItem);
		}
	}
}


//摆摊（出售）
VOID CActionSystem::UserStallSale_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();

	for(INT i=0; i<STALL_BOX_SIZE; i++)
	{
		//取得物品
		tObject_Item*	pItem = CDataPool::GetMe()->MyStallBox_GetItem(i);
		if(pItem)
		{
			//设置新的物品数据
			UpdateAction_FromItem(pItem);
		}
	}
}

//摆摊（购买）
VOID CActionSystem::UserStallBuy_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();

	for(INT i=0; i<STALL_BOX_SIZE; i++)
	{
		//取得物品
		tObject_Item*	pItem = CDataPool::GetMe()->OtStallBox_GetItem(i);
		if(pItem)
		{
			//设置新的物品数据
			UpdateAction_FromItem(pItem);
		}
	}
}

VOID CActionSystem::SetActionSystemKeyState( INT keyState, BOOL isFKey )
{
	if( isFKey );
		//m_nCurShortcutKeyFState = keyState;
	else
		m_nCurShortcutKeyState = keyState;
}


//聊天动作
VOID CActionSystem::UserChatMood_Update(VOID)
{
	GetMe()->CleanInvalidAction();

	INT i = 0;
	do {
		TALK_ACT_STRUCT* pActStruct = SCRIPT_SANDBOX::Talk::s_Talk.GetTalkActStructByID(i++);
		if(pActStruct)
		{
			UpdateAction_FromChatMood(pActStruct);
		}
		else
		{
			break;
		}
	} while(TRUE);
}

//物品传输系统
VOID CActionSystem::TransferSystem_Update(tObject_Item* pItem, BOOL bDelete /* = FALSE */)
{
	if(!pItem) return;
	CActionItem* pActionItem = (CActionItem*)(GetAction_ItemID(pItem->GetID()));

	//销毁
	if(bDelete && pActionItem)
	{
		EraseActionItem(pActionItem);
	}

	//更新
	if(!bDelete)
	{
		//设置新的物品数据
		UpdateAction_FromItem(pItem);
	}
}

VOID CActionSystem::UserBank_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();

	for(INT i=0; i<MAX_BANK_SIZE; i++)
	{
		//取得物品
		tObject_Item*	pItem = CDataPool::GetMe()->UserBank_GetItem(i);
		if(pItem)
		{
			//设置新的物品数据
			UpdateAction_FromItem(pItem);
		}
	}
}

VOID CActionSystem::UserExchangeSelf_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();

	for(INT i=0; i<EXCHANGE_BOX_SIZE; i++)
	{
		//取得物品
		tObject_Item*	pItem = CDataPool::GetMe()->MyExBox_GetItem(i);
		if(pItem)
		{
			//设置新的物品数据
			UpdateAction_FromItem(pItem);
		}
	}
}

VOID CActionSystem::UserExchangeOther_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();

	for(INT i=0; i<EXCHANGE_BOX_SIZE; i++)
	{
		//取得物品
		tObject_Item*	pItem = CDataPool::GetMe()->OtExBox_GetItem(i);
		if(pItem)
		{
			//设置新的物品数据
			UpdateAction_FromItem(pItem);
		}
	}
}
// 任务物品提交界面的更新
VOID CActionSystem::UserMission_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();
	for(INT i=0; i<MISSION_BOX_SIZE; i++)
	{
		//取得物品
		tObject_Item*	pItem = CDataPool::GetMe()->MissionBox_GetItem(i);
		if(pItem)
		{
			//设置新的物品数据
			UpdateAction_FromItem(pItem);
		}
	}
}

// 玩家商店的更新
VOID CActionSystem::UserPS_Self_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();
	for(INT k=0; k<10; k++)
	{
		for(INT i=0; i<STALL_BOX_SIZE; i++)
		{
			//取得物品
			tObject_Item*	pItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE,k,i);  
			if(pItem)
			{
				//设置新的物品数据
				UpdateAction_FromItem(pItem);
			}
		}
	}
}

// 玩家商店的更新
VOID CActionSystem::UserPS_Other_Update(VOID)
{
	//清空已经不存在的Action
	GetMe()->CleanInvalidAction();
	for(INT k=0; k<10; k++)
	{
		for(INT i=0; i<STALL_BOX_SIZE; i++)
		{
			//取得物品
			tObject_Item*	pItem = CDataPool::GetMe()->PlayerShop_GetItem(FALSE,k,i);  
			if(pItem)
			{
				//设置新的物品数据
				UpdateAction_FromItem(pItem);
			}
		}
	}
}

VOID CActionSystem::ItemBox_Update(VOID)
{
	GetMe()->CleanInvalidAction();
	//创建新的物品
	for(INT i=0; i<MAX_BOXITEM_NUMBER; i++)
	{
		//取得物品
		tObject_Item* pItem = CDataPool::GetMe()->ItemBox_GetItem(i);
		if(!pItem) continue;

		UpdateAction_FromItem(pItem);
	}
}

VOID CActionSystem::ItemBox_Created(INT nObjId)
{
	//清空原有ItemBox中的所有物品Action
	GetMe()->CleanInvalidAction(true);
	//创建新的物品
	for(INT i=0; i<MAX_BOXITEM_NUMBER; i++)
	{
		//取得物品
		tObject_Item* pItem = CDataPool::GetMe()->ItemBox_GetItem(i);
		if(!pItem) continue;

		UpdateAction_FromItem(pItem);
	}

	//产生事件打开UI箱子
	CGameProcedure::s_pEventSystem->PushEvent(GE_LOOT_OPENED, nObjId);
}

INT CActionSystem::CreateObjectItemByTableId(tObject_Item* pItem)
{
	if (pItem)
	{	
		return UpdateAction_FromItem(pItem);
	}
	return -1;
}




VOID CActionSystem::_OnSkillActive(const EVENT* pEvent, UINT dwOwnerData)
{
	//取得技能id
	INT idSkill = atoi(pEvent->vArg[0].c_str());
	//技能Action
	CActionItem_Skill* pSkillItem = (CActionItem_Skill*)GetMe()->GetAction(idSkill);
	if(!pSkillItem) return;

	//检查是否是能够自动进行的技能
	pSkillItem->AutoKeepOn();
}

VOID CActionSystem::UpdateAction_FromSkill(const SCLIENT_SKILL* pSkill)
{
	if (NULL == pSkill)
	{
		KLThrow("UpdateAction_FromSkill(), pSkill = NULL");
		return;
	}
	if (NULL == pSkill->m_pDefine)
		return;
		
	// 检查时候已经有该操作
	CActionItem_Skill* pSkillAction = (CActionItem_Skill*)GetAction_SkillID(pSkill->m_pDefine->m_nID);
	
	//AxTrace(0, 0, "Skill:%d", pSkill->m_pDefine->m_dwID);
	// 如果没有,创建
	if( NULL == pSkillAction )
	{
		pSkillAction = new CActionItem_Skill(_CreateID());
		pSkillAction->Update_Skill(pSkill);
		// 加入
		m_mapAllAction[pSkillAction->GetID()] = pSkillAction;
		UpdateToolBar();		
	}
	else
	{
		pSkillAction->Update_Skill(pSkill);
	}

}

VOID CActionSystem::UpdateAction_FromPetSkill(const PET_GUID_t& PetGUID, const PET_SKILL* pSkill, INT nSkillIndex)
{
	if (NULL == pSkill || NULL == pSkill->m_pDefine)
	{
		KLThrow("UpdateAction_FromPetSkill(), pSkill = NULL");
		return;
	}
	
	//检查时候已经有该操作
	CActionItem_PetSkill* pSkillAction = (CActionItem_PetSkill*)GetAction_PetSkillID(PetGUID, pSkill->m_pDefine->m_nID);
	
	//创建
	if(NULL == pSkillAction)
	{
		pSkillAction = new CActionItem_PetSkill(_CreateID());
		pSkillAction->Update_PetSkill(PetGUID, pSkill, nSkillIndex);
		//加入
		m_mapAllAction[pSkillAction->GetID()] = pSkillAction;		
	}
	// 更改
	else
	{
		pSkillAction->Update_PetSkill(PetGUID, pSkill, nSkillIndex);
	}
}

VOID CActionSystem::UpdateAction_FromPetSkillStudy(const PET_SKILL* pSk)
{
	//KLAssert(pSk);

	////检查时候已经有该操作

	//CActionItem_PetSkill* pSkillAction = (CActionItem_PetSkill*)GetAction_PetSkillID(pSk->m_nPetNum,pSk->m_pDefine->m_nID);
	//
	////AxTrace(0, 0, "Skill:%d", pSkill->m_pDefine->m_dwID);
	////如果没有,创建
	//if(!pSkillAction)
	//{
	//	pSkillAction = new CActionItem_PetSkill(_CreateID());
	//	pSkillAction->Update_PetSkill(pSk);
	//	//加入
	//	m_mapAllAction[pSkillAction->GetID()] = pSkillAction;
	//}
	//else
	//{
	//	pSkillAction->Update_PetSkill(pSk);
	//}
}

VOID CActionSystem::UpdateAction_FromLifeAbility(const SCLIENT_LIFEABILITY* pLifeAbility)
{
	KLAssert(pLifeAbility);

	//如果没有学会的技能，退出
	if(pLifeAbility->m_nLevel <= 0) return;

	if(pLifeAbility->m_pDefine == NULL)
		return;

	//检查时候已经有该操作
	CActionItem_LifeAbility* pLifeAbilityAction = (CActionItem_LifeAbility*)GetAction_LifeAbilityID(pLifeAbility->m_pDefine->nID);

	//如果没有,创建
	if(!pLifeAbilityAction)
	{
		pLifeAbilityAction = new CActionItem_LifeAbility(_CreateID());
		pLifeAbilityAction->Update_LifeAbility(pLifeAbility);
		//加入
		m_mapAllAction[pLifeAbilityAction->GetID()] = pLifeAbilityAction;
//		UpdateToolBar();
	}
}

INT CActionSystem::UpdateAction_FromItem(tObject_Item* pItem)
{
	KLAssert(pItem);

	//检查时候已经有该操作
	CActionItem_Item* pItemAction = (CActionItem_Item*)GetAction_ItemID(pItem->GetID());

	//如果没有,创建
	if(!pItemAction)
	{
		pItemAction = new CActionItem_Item(_CreateID());
		pItemAction->Update_Item(pItem);
		
		//加入链表
		m_mapAllAction[pItemAction->GetID()] = pItemAction;
		UpdateToolBar();
		return pItemAction->GetID();
	}
	//如果有，更新
	else
	{
		pItemAction->Update_Item(pItem);
		for(INT i=0; i<(INT)m_barMain.size(); i++)
		{
			ACTIONBAR_ITEM& barItem = m_barMain[i];

			if(barItem.idImpl == pItem->GetIdTable())
			{
				MainMenuBar_Set(i,pItemAction->GetID());
			}
		}
	}
	return -1;
}
// $$
//VOID CActionSystem::UpdateAction_FromXinFa(const SCLIENT_XINFA* pXinFaDef)
//{
//	//检查时候已经有该操作
//	CActionItem_XinFa* pXinFaAction = (CActionItem_XinFa*)GetAction_XinFaID(pXinFaDef->m_pDefine->nID);
//
//	//如果没有,创建
//	if(!pXinFaAction)
//	{
//		pXinFaAction = new CActionItem_XinFa(_CreateID());
//		pXinFaAction->Update_XinFa(pXinFaDef);
//
//		//加入链表
//		m_mapAllAction[pXinFaAction->GetID()] = pXinFaAction;
////		UpdateToolBar();
//	}
//	//如果有，更新
//	else
//	{
//		pXinFaAction->Update_XinFa(pXinFaDef);
//	}
//
//}

VOID CActionSystem::UpdateAction_FromChatMood(const TALK_ACT_STRUCT* pActStruct)
{
	CActionItem_ChatMood* pChatMood = (CActionItem_ChatMood*)GetAction_ChatMoodID(pActStruct->m_actIdx);

	if(!pChatMood)
	{
		pChatMood = new CActionItem_ChatMood(_CreateID());
		pChatMood->Update_ChatMood(pActStruct);

		m_mapAllAction[pChatMood->GetID()] = pChatMood;
		UpdateToolBar();
	}
	else
	{
		pChatMood->Update_ChatMood(pActStruct);
		UpdateToolBar();
	}
}

// 为一键换装更新
VOID CActionSystem::UpdateAction_FromChangeSuit(STRING& iconName, INT itemId, INT suitId)
{
	CActionItem_ChangeSuit* pItem = (CActionItem_ChangeSuit*)EnumAction( 0, "changesuit" );
	if( pItem == NULL )
		return;

	pItem->UpdateSuit(iconName, suitId);

	// 更新UI
	for(INT i=0; i<(INT)m_barMain.size(); i++)
	{
		ACTIONBAR_ITEM& barItem = m_barMain[i];

		if(	barItem.typeItem == AOT_CHANGE_SUIT )
		{
			INT idAction = pItem->GetID();
			barItem.idActionItem = idAction;

			CHAR szTemp[32];
			std::vector< STRING > vParam;
			vParam.push_back("main");

			_snprintf(szTemp, 32, "%d", i+1);	// Lua数组以1起始
			vParam.push_back(szTemp);

			_snprintf(szTemp, 32, "%d", idAction);
			vParam.push_back(szTemp);

			_snprintf(szTemp, 32, "%d", INVALID_ID);
			vParam.push_back(szTemp);

			CEventSystem::GetMe()->PushEvent(GE_CHANGE_BAR, vParam);
		}
	}
}


VOID CActionSystem::EraseActionItem(CActionItem* pItem)
{
	if(!pItem) return;
	//从总表中删除
	m_mapAllAction.erase(m_mapAllAction.find(pItem->GetID()));

	delete pItem;
    pItem = NULL;
	//...不能再调用pItem任何代码
}

INT CActionSystem::_CreateID(VOID) const
{
	static INT idNow = 1;
	return idNow++;
}

INT CActionSystem::GetActionNum(LPCTSTR szType, INT PosIndex)
{
	INT nNum = 0;

	if(_stricmp(szType, NAMETYPE_PACKAGEITEM) == 0) 
		return MAX_BAG_SIZE;
	else if(_stricmp(szType, NAMETYPE_LOOTITEM) == 0) 
		return MAX_BOXITEM_NUMBER;
	else if(_stricmp(szType, NAMETYPE_BANKITEM) == 0)
	{
		std::map< INT, CActionItem* >::iterator it;
		for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
		{
			if(_stricmp(it->second->GetType_String(), szType) == 0) 
				if( PosIndex == -1 || (PosIndex != -1 && PosIndex == it->second->GetPosIndex()) )
				nNum++;
		}

		return nNum;
	}
	else if(_stricmp(szType, NAMETYPE_EXCHANGESELF) == 0)
	{
		std::map< INT, CActionItem* >::iterator it;
		for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
		{
			if(_stricmp(it->second->GetType_String(), szType) == 0) 
				if( PosIndex == -1 || (PosIndex != -1 && PosIndex == it->second->GetPosIndex()) )
				nNum++;
		}

		return nNum;
	}
	else if(_stricmp(szType, NAMETYPE_STALLSELF) == 0)
	{
		std::map< INT, CActionItem* >::iterator it;
		for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
		{
			if(_stricmp(it->second->GetType_String(), szType) == 0) 
				if( PosIndex == -1 || (PosIndex != -1 && PosIndex == it->second->GetPosIndex()) )
				nNum++;
		}

		return nNum;
	}
	else if(_stricmp(szType, NAMETYPE_STALLOTHER) == 0)
	{
		std::map< INT, CActionItem* >::iterator it;
		for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
		{
			if(_stricmp(it->second->GetType_String(), szType) == 0) 
				if( PosIndex == -1 || (PosIndex != -1 && PosIndex == it->second->GetPosIndex()) )
				nNum++;
		}

		return nNum;
	}
	else if(_stricmp(szType, NAMETYPE_STALLOTHER) == 0)
	{
		std::map< INT, CActionItem* >::iterator it;
		for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
		{
			if(_stricmp(it->second->GetType_String(), szType) == 0) 
				if( PosIndex == -1 || (PosIndex != -1 && PosIndex == it->second->GetPosIndex()) )
				nNum++;
		}

		return nNum;
	}
	else  if (_stricmp(szType, NAMETYPE_IB_SHOP) == 0)
	{
		std::map< INT, CActionItem* >::iterator it;
		for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
		{
			if(_stricmp(it->second->GetType_String(), szType) == 0) 
				if( PosIndex == -1 || (PosIndex != -1 && PosIndex == it->second->GetPosIndex()) )
					nNum++;
		}

		return nNum;
	}
	else
	{
		std::map< INT, CActionItem* >::iterator it;
		for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
		{
			if(_stricmp(it->second->GetType_String(), szType) == 0) 
				if( PosIndex == -1 || (PosIndex != -1 && PosIndex == it->second->GetPosIndex()) )
				nNum++;
		}

		return nNum;
	}
}


/////////////////////////////////////////////////////////////////////////
// Desc：调用关系 Nick
// 从CActionItem以及其子类中的NotifyDragDropDragged中调用设置快捷栏
//////////////////////////////////////////////////////////////////////////
VOID CActionSystem::MainMenuBar_Set(INT nIndex, INT idAction)
{
	if( nIndex < 0 || nIndex > ActionButtonNum )
		return;

	//if( idAction != -1 )
	//	SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.SetMainMenuBarSouXiaSkillNum(idAction, nIndex );

	if(idAction == INVALID_ID)
	{
		m_barMain[nIndex].typeItem = AOT_EMPTY;
		m_barMain[nIndex].idImpl = INVALID_ID;
		m_barMain[nIndex].idActionItem = INVALID_UID;

		UpdateToolBar();
	}
	
	// 取得 CActionItem 并填充与其对应的 ACTIONBAR_ITEM 结构
	// 在这之前 ACTIONBAR_ITEM 中是初始化数据
	CActionItem* pItem = (CActionItem*)GetAction(idAction);
	if(pItem)
	{
		switch(pItem->GetType())
		{
		case AOT_SKILL:
			{
				const SCLIENT_SKILL* pSkill = (const SCLIENT_SKILL*)pItem->GetImpl();
				if(!pSkill) break;

				m_barMain[nIndex].typeItem = AOT_SKILL;
				m_barMain[nIndex].idImpl = pSkill->m_pDefine->m_nID;
				m_barMain[nIndex].idActionItem = INVALID_UID;
			}
			break;
		case AOT_ITEM:
			{
				const CObject_Item* pItemInfo = (const CObject_Item*)(const tObject_Item*)pItem->GetImpl();
				if(!pItemInfo) break;

				m_barMain[nIndex].typeItem = AOT_ITEM;
				m_barMain[nIndex].idImpl = pItemInfo->GetIdTable();
				m_barMain[nIndex].idActionItem = INVALID_UID;
			}
			break;
		case AOT_PET_SKILL:
			{
				const PET_SKILL* pPetSkill = (const PET_SKILL*)pItem->GetImpl();
				if(!pPetSkill) 
					break;

				CActionItem_PetSkill* pPetItem = (CActionItem_PetSkill*)pItem;

				m_barMain[nIndex].typeItem		= AOT_PET_SKILL;
				m_barMain[nIndex].idImpl		= pPetSkill->m_pDefine->m_nID;
				m_barMain[nIndex].m_GUID		= pPetItem->GetMountGUID();
				m_barMain[nIndex].idActionItem	= INVALID_UID;
			}
			break;
		case AOT_CHATMOOD:
			{
				const TALK_ACT_STRUCT* pActStruct = (const TALK_ACT_STRUCT*)pItem->GetImpl();
				if(!pActStruct) break;

				m_barMain[nIndex].typeItem = AOT_CHATMOOD;
				m_barMain[nIndex].idImpl = pActStruct->m_actIdx;
				m_barMain[nIndex].idActionItem = INVALID_UID;
			}
			break;
		case AOT_CHANGE_SUIT:
			{
				m_barMain[nIndex].typeItem = AOT_CHANGE_SUIT;
				m_barMain[nIndex].idImpl = pItem->GetDefineID();
				m_barMain[nIndex].idActionItem = INVALID_UID;
			}
			break;

		default:
			break;
		}

		UpdateToolBar();
	}

	// 发送 ACTIONBAR_ITEM 到服务器并保存
	// ACTIONBAR_ITEM 即是快捷栏数据
	ACTIONBAR_ITEM& barItem = m_barMain[nIndex];

	tActionItem* pActionItem = NULL;
	CGModifySetting msg;
	_OWN_SETTING* pSend_Value = msg.GetValue();

	switch(barItem.typeItem)
	{
	case AOT_PET_SKILL:
			pActionItem = GetAction_PetSkillID(barItem.m_GUID, barItem.idImpl);
			msg.SetType(nIndex+SETTING_TYPE_K0);
			pSend_Value->m_SettingType = barItem.typeItem;
			pSend_Value->m_SettingData = (barItem.m_GUID.m_uLowSection << 16) + barItem.idImpl;


			CNetManager::GetMe()->SendPacket( &msg );
			break;
						

	case AOT_SKILL:
		pActionItem = GetAction_SkillID(barItem.idImpl);
		msg.SetType(nIndex+SETTING_TYPE_K0);
		pSend_Value->m_SettingType = barItem.typeItem;
		pSend_Value->m_SettingData = barItem.idImpl;

		CNetManager::GetMe()->SendPacket( &msg );
		break;

	case AOT_SKILL_LIFEABILITY:
		break;

	case AOT_ITEM:
		pActionItem = GetAction_ItemIDTable_FromMyPacket(barItem.idImpl);
		if(pActionItem)
		{
			barItem.idItemCount = CDataPool::GetMe()->UserBag_CountItemByIDTable(barItem.idImpl);
		}
		else
		{
			barItem.idImpl = INVALID_ID;					
		}

		msg.SetType(nIndex+SETTING_TYPE_K0);
		pSend_Value->m_SettingType = barItem.typeItem;
		pSend_Value->m_SettingData = barItem.idImpl;

		CNetManager::GetMe()->SendPacket( &msg );
		break;

	case AOT_EMPTY:
		msg.SetType(nIndex+SETTING_TYPE_K0);
		pSend_Value->m_SettingType = barItem.typeItem;
		pSend_Value->m_SettingData = barItem.idImpl;

		CNetManager::GetMe()->SendPacket( &msg );
		break;

	case AOT_CHANGE_SUIT:
		msg.SetType(nIndex+SETTING_TYPE_K0);
		pSend_Value->m_SettingType = barItem.typeItem;
		pSend_Value->m_SettingData = barItem.idImpl;

		CNetManager::GetMe()->SendPacket( &msg );
		break;

	default:
		break;
	}
}


/////////////////////////////////////////////////////////////////////////
// Desc：调用关系 Nick
// 服务器保存了快捷栏的信息， 在GCRetSettingHandler中调用填充
//////////////////////////////////////////////////////////////////////////
VOID CActionSystem::MainMenuBar_SetID(INT nIndex, INT idType,INT idData)
{
	if( nIndex < 0 || nIndex > ActionButtonNum )
		return;

	if(idType == INVALID_ID)
	{
		m_barMain[nIndex].typeItem = AOT_EMPTY;
		m_barMain[nIndex].idImpl = INVALID_ID;
		m_barMain[nIndex].idActionItem = INVALID_ID;

		UpdateToolBar();
		return;
	}

	switch(idType)
	{
	case AOT_SKILL:
		{
			m_barMain[nIndex].typeItem = AOT_SKILL;
			m_barMain[nIndex].idImpl = idData;
			m_barMain[nIndex].idActionItem = INVALID_UID;
		}
		break;
	case AOT_ITEM:
		{
			m_barMain[nIndex].typeItem = AOT_ITEM;
			m_barMain[nIndex].idImpl = idData;
			m_barMain[nIndex].idActionItem = INVALID_UID;
		}
		break;
	/*case AOT_PET_SKILL:
		{
			m_barMain[nIndex].typeItem = AOT_PET_SKILL;
			m_barMain[nIndex].idImpl = idData & 0xFFFF;
			m_barMain[nIndex].idActionItem = INVALID_UID;
			m_barMain[nIndex].idPetIndex = (idData >> 16) & 0xFFFF;
		}
		break;*/

		break;
	case AOT_CHANGE_SUIT:
		{
			m_barMain[nIndex].typeItem = AOT_CHANGE_SUIT;
			m_barMain[nIndex].idImpl = idData;
			m_barMain[nIndex].idActionItem = INVALID_UID;
		}
		break;

	default:
		break;
	}
	
	UpdateToolBar();
}


INT CActionSystem::MainMenuBar_Get(INT nIndex)
{
	if(nIndex < 0 || nIndex > ActionButtonNum) return -1;

	return m_barMain[nIndex].idActionItem;
}

VOID CActionSystem::UserBag_Set(INT nIndex, INT idAction)
{
	if(nIndex < 0 || nIndex > ActionButtonNum) return;

	CActionItem* pItem = (CActionItem*)GetAction(idAction);
	if(!pItem) return;

	switch(pItem->GetType())
	{
	case AOT_ITEM:
		{
			CDataPool::GetMe()->UserBag_SetItem(nIndex, (tObject_Item*)pItem->GetImpl());
		}
		break;

	default:
		break;
	}
}

VOID CActionSystem::UpdateToolBar(VOID)
{
	// 保存MainMenuBar上修改过的快捷键
	STRING szTemp = "";
	STRING szRightTemp = "";
	INT nPet_Num =-1;

	///////////////////////////////////////////////
	//MainMenu Bar
	for(INT i=0; i<(INT)m_barMain.size(); i++)
	{
		ACTIONBAR_ITEM& barItem = m_barMain[i];

		//如果尚未引用
		if(	barItem.idActionItem == INVALID_UID || barItem.typeItem == AOT_PET_SKILL || 
			NULL==GetAction(barItem.idActionItem) )
		{
			//查询是否可以引用
			tActionItem* pActionItem = NULL;

			switch(barItem.typeItem)
			{
			case AOT_PET_SKILL:
				pActionItem = GetAction_PetSkillID(barItem.m_GUID, barItem.idImpl);

				// 不是当前宠物技能时，清除
				/*CActionItem_PetSkill* pPetSkillItem = (CActionItem_PetSkill*)pActionItem;
				if(pPetSkillItem->GetMountGUID() != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID() )
				{
				MainMenuBar_Set(i,INVALID_ID);
				continue;
				}*/									
						
				break;
			
			case AOT_SKILL:
				pActionItem = GetAction_SkillID(barItem.idImpl);
				break;

			case AOT_SKILL_LIFEABILITY:
				break;

			case AOT_ITEM:
				pActionItem = GetAction_ItemIDTable_FromMyPacket(barItem.idImpl);
				if(pActionItem)
				{
					barItem.idItemCount = CDataPool::GetMe()->UserBag_CountItemByIDTable(barItem.idImpl);
				}
				else
				{
					barItem.idImpl = INVALID_ID;					
				}
				break;

			case AOT_EMPTY:
				break;
			case AOT_CHATMOOD:
				pActionItem = GetAction_ChatMoodID(barItem.idImpl);
				break;
			case AOT_CHANGE_SUIT:
				pActionItem = EnumAction(0, "changesuit");
				break;
			default:
				break;
			}

			INT idAction;
			
			if(pActionItem)
			{
				barItem.idActionItem = pActionItem->GetID();
				idAction = pActionItem->GetID();
			}
			else
			{
				idAction = INVALID_ID;
			}

			//可以引用了，给UI发送事件
			CHAR szTemp[32];
			std::vector< STRING > vParam;
			vParam.push_back("main");

			_snprintf(szTemp, 32, "%d", i+1);	//Lua数组以1起始
			vParam.push_back(szTemp);

			_snprintf(szTemp, 32, "%d", idAction);
			vParam.push_back(szTemp);

			if(idAction != INVALID_ID && barItem.typeItem == AOT_PET_SKILL)
			{
				//_snprintf(szTemp, 32, "%d", pActionItem->GetNum());//以后策划要是变想法了，就把这句恢复回来。
				_snprintf(szTemp, 32, "%d", INVALID_ID);
				vParam.push_back(szTemp);
			}
			else
			{
				_snprintf(szTemp, 32, "%d", INVALID_ID);
				vParam.push_back(szTemp);
			}

			CEventSystem::GetMe()->PushEvent(GE_CHANGE_BAR, vParam);
		}
	}
}

VOID CActionSystem::UpdateToolBarForPetSkill(VOID)
{
	CleanInvalidAction();

	for(INT i=0; i<(INT)m_barMain.size(); i++)
	{
		ACTIONBAR_ITEM& barItem = m_barMain[i];

		// 是宠物技能
		if(	barItem.typeItem == AOT_PET_SKILL)
		{
			//查询是否可以引用
			tActionItem* pActionItem = NULL;

			pActionItem = GetAction_PetSkillID(barItem.m_GUID, barItem.idImpl);
			if(!pActionItem)
			{
				// 按钮无效，清除
				MainMenuBar_Set(i,INVALID_ID);
				continue;
			}

			// 不是当前宠物技能时，清除
			/*CActionItem_PetSkill* pPetSkillItem = (CActionItem_PetSkill*)pActionItem;
			if(pPetSkillItem->GetMountGUID() != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID() )
			{
				MainMenuBar_Set(i,INVALID_ID);
				continue;
			}*/			
			
			INT idAction;
			
			if(pActionItem)
			{
				barItem.idActionItem = pActionItem->GetID();
				idAction = pActionItem->GetID();
			}
			else
			{
				idAction = INVALID_ID;
				MainMenuBar_Set(i,INVALID_ID);
			}

			//可以引用了，给UI发送事件
			CHAR szTemp[32];
			std::vector< STRING > vParam;
			vParam.push_back("main");

			_snprintf(szTemp, 32, "%d", i+1);	//Lua数组以1起始
			vParam.push_back(szTemp);

			_snprintf(szTemp, 32, "%d", idAction);
			vParam.push_back(szTemp);
			
			if(idAction != INVALID_ID)
			{
//				_snprintf(szTemp, 32, "%d", pActionItem->GetNum());
// 以后策划要是变想法了，就把上面这句恢复回来。把下面那句注释起来。
				_snprintf(szTemp, 32, "%d", INVALID_ID);
			}
			else
			{
				_snprintf(szTemp, 32, "%d", INVALID_ID);
				vParam.push_back(szTemp);
			}

			CEventSystem::GetMe()->PushEvent(GE_CHANGE_BAR, vParam);
		}

	}
	return;
}
//枚举所有Action
tActionItem* CActionSystem::EnumAction(INT nPosIndex, LPCTSTR szFilter)
{
	std::map< INT, CActionItem* >::iterator it;

	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
	{
		if(_stricmp(it->second->GetType_String(), szFilter) == 0) 
		{
			if(it->second->GetPosIndex() == nPosIndex)
			{
				return it->second;
			}
		}		
	}

	return NULL;
}

tActionItem* CActionSystem::EnumPetSkill(INT nPetIndex, INT nPetSkill, LPCTSTR szFilter)
{
	GetMe()->CleanInvalidAction();
	UserTargetPetSkill_Update();

	std::map< INT, CActionItem* >::iterator it;
	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
	{
		if(_stricmp(it->second->GetType_String(), szFilter) == 0) 
		{
			if(it->second->GetPosIndex() == nPetSkill && it->second->GetNum() == nPetIndex)  
				return it->second;
		}		
	}

	return NULL;
}

VOID CActionSystem::MainMenuBar_DoAction(INT nIndex, BOOL bPrepare)
{	
	if(nIndex<0 || nIndex>=s_MainMenuBarNum) return;

	// 计算快捷键对应的m_barMain位置 Nick 2008.1.12
	if( nIndex < 10 )
		nIndex += m_nCurShortcutKeyState * 10;
	//else
	//	nIndex += m_nCurShortcutKeyFState * 6 + 12;

	ACTIONBAR_ITEM& item = m_barMain[nIndex];

	//尚未设定
	CActionItem* pItem = (CActionItem*)GetAction(item.idActionItem);
	if(!pItem) return;

	if(bPrepare)
	{
		void* pCustomData = NULL;
		switch(item.typeItem)
		{
		case AOT_CHANGE_SUIT:
			pCustomData = (void*)&item.idImpl;
			break;
		}
		// 特殊操作处理,各个子类可以定义自己的操作
		pItem->SpecialOperation(pCustomData);

		//显示Check
		pItem->DoActionPrepare(true);
	}
	else
	{

		//如果有交易，摆摊，玩家商店界面开着，不能执行
		// 如果交易界面开着
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("CActionSystem_Cannt_DoIt");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你不能这样做");

			return ;
		}
		//如果摆摊界面开着
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallSale"))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("CActionSystem_Cannt_DoIt");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你不能这样做");

			return ;
		}
		//如果玩家商店界面开着
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag"))
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("CActionSystem_Cannt_DoIt");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你不能这样做");

			return ;
		}

		//隐藏Check
		pItem->DoActionPrepare(FALSE);
		//执行动作
		pItem->DoAction();
	}
}

tActionItem* CActionSystem::GetAction(INT id)
{
	std::map< INT, CActionItem* >::iterator it;

	it = m_mapAllAction.find(id);
	if( it != m_mapAllAction.end() )
		return it->second;

	return NULL;
}

tActionItem* CActionSystem::GetAction_SkillID(INT id)
{
	register std::map< INT, CActionItem* >::iterator it;
	
	for( it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++ )
	{
		CActionItem* pActionItem = (CActionItem*) it->second;

		// 如果是技能
		if(	pActionItem && pActionItem->GetType() == AOT_SKILL)
		{
			const SCLIENT_SKILL* pSkill = (const SCLIENT_SKILL*)pActionItem->GetImpl();
			if( !pSkill ) 
				continue;

			if( pSkill->m_pDefine->m_nID == id )
				return pActionItem;
		}
	}

	return NULL;
}

tActionItem* CActionSystem::GetAction_PetSkillID(const PET_GUID_t& petGUID, INT idPetSkill)
{
	register std::map< INT, CActionItem* >::iterator it;
	
	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
	{
		CActionItem* pActionItem = (CActionItem*)it->second;

		//如果是骑宠技能
		if(	pActionItem && pActionItem->GetType() == AOT_PET_SKILL)
		{
			// 同一个骑宠
			CActionItem_PetSkill* pPetSkillItem = (CActionItem_PetSkill*)pActionItem;
			if (pPetSkillItem->GetMountGUID() == petGUID)
			{
				const PET_SKILL* pSkill = (const PET_SKILL*)pActionItem->GetImpl();
				if (pSkill && pSkill->m_pDefine)
				{
					if (pSkill->m_pDefine->m_nID == idPetSkill)
						return pActionItem;
				}
			}
		}
	}

	return NULL;
}

tActionItem* CActionSystem::GetAction_ItemID(INT id)
{
	register std::map< INT, CActionItem* >::iterator it;
	
	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
	{
		CActionItem* pActionItem = (CActionItem*) it->second;

		//如果是物品
		if(	pActionItem && 
			pActionItem->GetType() == AOT_ITEM )
		{
			tObject_Item* pItem = (tObject_Item* )pActionItem->GetImpl();
			if(pItem && pItem->GetID() == id) return pActionItem;
		}
	}

	return NULL;
}

tActionItem* CActionSystem::GetAction_ItemIDTable_FromMyPacket(INT id)
{
	register std::map< INT, CActionItem* >::iterator it;
	
	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
	{
		CActionItem* pActionItem = (CActionItem*) it->second;

		//如果是物品
		if(	pActionItem && 
			pActionItem->GetType() == AOT_ITEM )
		{
			tObject_Item* pItem = (tObject_Item* )pActionItem->GetImpl();
			if(pItem && pItem->GetIdTable() == id && pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET) return pActionItem;
		}
	}

	return NULL;
}

tActionItem* CActionSystem::GetAction_LifeAbilityID(INT idAbility)
{
	register std::map< INT, CActionItem* >::iterator it;
	
	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
	{
		CActionItem* pActionItem = (CActionItem*) it->second;

		//如果是生活技能
		if(	pActionItem && 
			pActionItem->GetType() == AOT_SKILL_LIFEABILITY )
		{
			const SCLIENT_LIFEABILITY* pAbility = (const SCLIENT_LIFEABILITY*)pActionItem->GetImpl();
			if(pAbility && pAbility->m_pDefine->nID == idAbility&&
				((CActionItem_LifeAbility*)pActionItem)->Get_LifeStudy() == FALSE	) return pActionItem;
		}
	}

	return NULL;
}
// $$
//tActionItem* CActionSystem::GetAction_XinFaID(INT idXinFa)
//{
//	register std::map< INT, CActionItem* >::iterator it;
//	
//	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
//	{
//		CActionItem* pActionItem = (CActionItem*) it->second;
//
//		//如果是生活技能
//		if(	pActionItem && 
//			pActionItem->GetType() == AOT_XINFA )
//		{
//			const SCLIENT_XINFA* pXinFa = (const SCLIENT_XINFA*)pActionItem->GetImpl();
//			if(pXinFa && pXinFa->m_pDefine->nID == idXinFa) return pActionItem;
//		}
//	}
//
//	return NULL;
//}

tActionItem* CActionSystem::GetAction_ChatMoodID(INT idAct)
{
	register std::map< INT, CActionItem* >::iterator it;

	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
	{
		CActionItem* pActionItem = (CActionItem*) it->second;

		//如果是聊天动作
		if(	pActionItem && 
			pActionItem->GetType() == AOT_CHATMOOD )
		{
			const TALK_ACT_STRUCT* pActStruct = (const TALK_ACT_STRUCT*)pActionItem->GetImpl();
			if(pActStruct && pActStruct->m_actIdx == idAct) return pActionItem;
		}
	}

	return NULL;
}



VOID CActionSystem::UpdateCoolDown(INT nCoolDownID)
{
	register std::map< INT, CActionItem* >::iterator it;
	
	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
	{
		CActionItem* pActionItem = (CActionItem*) it->second;

		//如果是相应冷却组的Action
		if(	pActionItem && pActionItem->GetCoolDownID() == nCoolDownID )
		{
			pActionItem->UpdateCoolDown();
		}
	}
}

VOID CActionSystem::UpdateCommonCoolDown(INT nSkillID)
{

	register std::map< INT, CActionItem* >::iterator it;

	INT nCommonCoolDown = CDataPool::GetMe()->CommonCoolDown_Get();
	if( nCommonCoolDown > 0 ) return;
	CDataPool::GetMe()->CommonCoolDown_Update();

	for(it=m_mapAllAction.begin(); it!=m_mapAllAction.end(); it++)
	{
		CActionItem* pActionItem = (CActionItem*) it->second;
		
		//如果是相应冷却组的Action
		if(	pActionItem && (pActionItem->GetCoolDownID() != INVALID_ID) )
		{
			if(INVALID_ID != nSkillID && pActionItem->GetDefineID() == nSkillID && pActionItem->GetType_String() == NAMETYPE_SKILL ) continue;
			pActionItem->UpdateCommonCoolDown();
		}
	}
}


VOID CActionSystem::Booth_Update(VOID)
{
	GetMe()->CleanInvalidAction();

	//创建新的物品
	for(INT i=0; i<MAX_BOOTH_NUMBER; i++)
	{
		//取得物品
		tObject_Item* pItem = CDataPool::GetMe()->Booth_GetItem(i);
		if(!pItem) continue;

		GetMe()->UpdateAction_FromItem(pItem);
	}

	//回购商品刷新
	for(INT i=0; i<MAX_BOOTH_SOLD_NUMBER ; i++)
	{
		//取得物品
		tObject_Item* pItem = CDataPool::GetMe()->Booth_GetSoldItem(i);
		if(!pItem) continue;

		GetMe()->UpdateAction_FromItem(pItem);
	}
}

//add by gh 2010/07/08
VOID CActionSystem::IBShop_Update(VOID)
{
	GetMe()->CleanInvalidAction();

	//创建新的物品
	for(INT i=0; i<MAX_IBSHOP_NUMBER; i++)
	{
		//取得物品
		tObject_Item* pItem = CDataPool::GetMe()->IBShop_GetItem(i);
		if(!pItem) continue;

		GetMe()->UpdateAction_FromItem(pItem);
	}
}
//end of add
VOID CActionSystem::QuestLogItem_Update(VOID)
{
	GetMe()->CleanInvalidAction();
	_MISSION_INFO* misInfo = NULL;
	
	//创建新的物品
	for(INT i=0; i < MAX_CHAR_MISSION_NUM; i++)
	{
		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(i);

		if(!OwnMission)	continue;

		INT scriptId = OwnMission->m_idScript;

		misInfo = &(GetMissionInfo(scriptId));
		if((!misInfo) || (!misInfo->m_bFill)) continue;
		INT max = (INT)misInfo->m_vecQuestRewardItem.size();
		for(INT j=0; j< max; j++)
		{			
			//取得物品
			_MISSION_INFO::QuestRewardItem misItem = misInfo->m_vecQuestRewardItem[j];
			if(		misItem.pItemData->m_nType == MISSION_BONUS_TYPE_ITEM 
				||	misItem.pItemData->m_nType == MISSION_BONUS_TYPE_ITEM_RAND
				||	misItem.pItemData->m_nType == MISSION_BONUS_TYPE_ITEM_RADIO )
			{
				tObject_Item* pItem = misItem.pItemImpl;
				if(!pItem) continue;
				UpdateAction_FromItem(pItem);
			}
		}
	}
	return;
}

tActionItem* CActionSystem::GetTooltipsFocus(void)
{
	return SCRIPT_SANDBOX::SuperTooltips::s_SupperTooltip.GetActionItem();
}

VOID CActionSystem::SetDefaultAction(tActionItem* pDefAction) 
{ 
	if(m_pDefaultAction != pDefAction && m_pDefaultAction)
	{
		((CActionItem*)m_pDefaultAction)->SetDefaultState(FALSE);
	}

	m_pDefaultAction = pDefAction;

	if(m_pDefaultAction)
	{
		((CActionItem*)m_pDefaultAction)->SetDefaultState(TRUE);
	}
}
