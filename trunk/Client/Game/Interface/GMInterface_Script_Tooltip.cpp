
#include "StdAfx.h"
#include "GMInterface_Script_Tooltip.h"
#include "GIException.h"
#include "LuaPlus.h"
#include "GIActionSystem.h"
#include "action\GMActionSystem_Item.h"
#include "action\GMActionSystem_XinFa.h"
#include "action\GMActionSystem_Skill.h"
#include "object\Logic\item\Obj_Item_Medicine.h"
#include "DBC\GMDataBase.h"
#include "Procedure\GameProcedure.h"

#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool/GMUIDataPool.h"

//
//脚本函数沙箱，这些函数应该由脚本调用
//
namespace SCRIPT_SANDBOX
{

	SuperTooltips SuperTooltips::s_SupperTooltip;
	SuperTooltips SuperTooltips::s_SupperTooltip2;
	LuaPlus::LuaObject* SuperTooltips::s_pMetaTable = NULL;
	SuperTooltips::SuperTooltips()
	{
		m_pActionItem = NULL;
	}

	SuperTooltips::~SuperTooltips()
	{
	}

	//由游戏逻辑设置激活中的ActionItem
	VOID SuperTooltips::SetActionItem(CActionItem* pActionitem)
	{
		m_pActionItem = pActionitem;
		if(m_pActionItem)
		{
			m_nActionId = m_pActionItem->GetID();
		}
		
	}
		
	INT SuperTooltips::GetTitle(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		STRING type( m_pActionItem->GetType_String() );
		if( type == "changesuit" )
		{
			state->PushString("一键换装");
			return 1;
		}

		state->PushString(m_pActionItem->GetName());
		return 1;
	}


	INT SuperTooltips::GetIcon(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetIconName());
		return 1;
	}

	INT SuperTooltips::GetAttributeValue(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		//检查参数
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA: SuperTooltips GetAttributeValue Wrong Param1");
		}

		//通过ActionItem查询逻辑属性
		state->PushString(m_pActionItem->GetAttributeValue(args[2].GetString()).c_str());

		return 1;
	}

	INT	SuperTooltips::GetItemDesc(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		STRING type( m_pActionItem->GetType_String() );
		if( type == "changesuit" )
		{
			CCharacterData* pData = ((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData();
			INT id = pData->Get_EquipSuitId();

			_SUIT_SETTING aSuit = pData->Get_EquipSuit( id );
			STRING name(aSuit.m_SuitName);
			if( !name.empty() )
			{
				CHAR szTemp[100];	
				_snprintf(szTemp, 10, "%s套装", name.c_str());
				state->PushString(szTemp);

			}
			else
			{
				CHAR szTemp[100];	
				_snprintf(szTemp, 10, "这是自定义的第%d套套装", id);
				state->PushString(szTemp);
			}

			return 1;
		}

		//得到详细说明.
		state->PushString(m_pActionItem->GetDesc());
		return 1;
	}

	INT	SuperTooltips::GetItemLevel(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemLevel());
		return 1;
	}

	//查询Tooltip的物品耐久
	INT	SuperTooltips::GetItemDur(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			state->PushInteger(-1);
			return 2;
		}

		state->PushInteger(m_pActionItem->GetItemDur());
		state->PushInteger(m_pActionItem->GetItemMaxDur());
		return 2;
	}

	//查询Tooltip的物品的修理次数
	INT	SuperTooltips::GetItemRepairCount(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemRepairCount());
	
		return 1;
	}

	//
	// 得到最大修理次数
	//INT	SuperTooltips::GetEquipMaxRepair(LuaPlus::LuaState* state)
	//{
	//	// 不利用保存的指针。要重新得到指针。 
	//	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	//	CActionItem_Item* pActionItem_Item = NULL;
	//	pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	//	if(!pActionItem_Item)
	//	{
	//		state->PushInteger(-1);
	//		return 1;
	//	}

	//	state->PushInteger(pActionItem_Item->GetItemMaxRepair());
	//	return 1;
	//
	//}

	//查询Tooltip的物品的绑定信息
	INT	SuperTooltips::GetItemBindInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemBindInfo());
	
		return 1;
	}

	//查询Tooltip的装备的2次绑定信息
	INT	SuperTooltips::GetItemSecLevelBindInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemSecLevelBindInfo());

		return 1;
	}

//	得到装备的2次绑定加成
	 
	INT	SuperTooltips::GetItemBindAddInfo(LuaPlus::LuaState* state)
	{

		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetItemBindAddInfo());

		return 1;

	}

	//	得到装备的颜色

	INT	SuperTooltips::GetDropVisColor(LuaPlus::LuaState* state)
	{

		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetDropVisColor());

		return 1;

	}

	


	//查询Tooltip的物品的卖给npc的价格
	INT	SuperTooltips::GetItemPrice(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemPrice());
	
		return 1;
	}

   //查询Tooltip的物品的职业
	INT  SuperTooltips::GetItemProfession(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemProfession());

		return 1;

	}
	//得到装备牌型
	INT  SuperTooltips::GetEquipCard(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetEquipCard());

		return 1;

	}


	//得到物品是否唯一
	INT  SuperTooltips::GetItemRuleOnly(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemRuleOnly());

		return 1;


	}
	//得到装备孔数量

	INT		SuperTooltips::GetEquipHoleNum(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetEquipHoleNum());
		return 1;

	}



//得到装备套装信息

INT		SuperTooltips::GetEquipSetInfo(LuaPlus::LuaState* state)
{
	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	CActionItem_Item* pActionItem_Item = NULL;
	pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	if(!pActionItem_Item)
	{
		state->PushString("");
		return 1;
	}

	state->PushString(pActionItem_Item->GetEquipSetInfo());
	return 1;

}


//得到装备牌型激活的信息

INT		SuperTooltips::GetEquipSpritePaiInfo(LuaPlus::LuaState* state)
{
	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	CActionItem_Item* pActionItem_Item = NULL;
	pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	if(!pActionItem_Item)
	{
		state->PushString("");
		return 1;
	}

	state->PushString(pActionItem_Item->GetEquipSpritePaiInfo());
	return 1;

}




//得到其他玩家装备小激活的信息

INT		SuperTooltips::GetOtherEquipSpriteChainInfo(LuaPlus::LuaState* state)
{
	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	CActionItem_Item* pActionItem_Item = NULL;
	pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	if(!pActionItem_Item)
	{
		state->PushString("");
		return 1;
	}

	state->PushString(pActionItem_Item->GetOtherEquipSpriteChainInfo());
	return 1;

}





//得到装备小激活的信息
 
INT		SuperTooltips::GetEquipSpriteChainInfo(LuaPlus::LuaState* state)
{
	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	CActionItem_Item* pActionItem_Item = NULL;
	pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	if(!pActionItem_Item)
	{
		state->PushString("");
		return 1;
	}

	state->PushString(pActionItem_Item->GetEquipSpriteChainInfo());
	return 1;

}
//得到其他玩家装备大激活的信息

INT		SuperTooltips::GetOtherEquipSpriteAllActiveInfo(LuaPlus::LuaState* state)
{
	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	CActionItem_Item* pActionItem_Item = NULL;
	pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	if(!pActionItem_Item)
	{
		state->PushString("");
		return 1;
	}

	state->PushString(pActionItem_Item->GetOtherEquipSpriteAllActiveInfo());
	return 1;

}


//得到装备大激活的信息

INT		SuperTooltips::GetEquipSpriteAllActiveInfo(LuaPlus::LuaState* state)
{
	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	CActionItem_Item* pActionItem_Item = NULL;
	pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	if(!pActionItem_Item)
	{
		state->PushString("");
		return 1;
	}

	state->PushString(pActionItem_Item->GetEquipSpriteAllActiveInfo());
	return 1;

}

//得到装备套装信息

INT		SuperTooltips::GetOtherEquipSetInfo(LuaPlus::LuaState* state)
{
	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	CActionItem_Item* pActionItem_Item = NULL;
	pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	if(!pActionItem_Item)
	{
		state->PushString("");
		return 1;
	}

	state->PushString(pActionItem_Item->GetOtherEquipSetInfo());
	return 1;

}


//得到装备套装激活的属性信息

INT		SuperTooltips::GetEquipSetActiveAttr(LuaPlus::LuaState* state)
{
	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	CActionItem_Item* pActionItem_Item = NULL;
	pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	if(!pActionItem_Item)
	{
		state->PushString("");
		return 1;
	}

	state->PushString(pActionItem_Item->GetEquipSetActiveAttr());
	return 1;

}


//得到装备套装激活的属性信息

INT		SuperTooltips::GetOtherEquipSetActiveAttr(LuaPlus::LuaState* state)
{
	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	CActionItem_Item* pActionItem_Item = NULL;
	pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	if(!pActionItem_Item)
	{
		state->PushString("");
		return 1;
	}

	state->PushString(pActionItem_Item->GetOtherEquipSetActiveAttr());
	return 1;

}
    //得到装备星级

	INT		SuperTooltips::GetStartLevel(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetStartLevel());
		return 1;

	}











	// 查询Tooltip的物品的制造者
	INT	SuperTooltips::GetItemManufacturer(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针。 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetManufacturer());
	
		return 1;
	}

	// 查询Tooltip的物品的白色属性 
	//INT	SuperTooltips::GetItemBaseWhiteAttrInfo(LuaPlus::LuaState* state)
	//{
	//	// 不利用保存的指针。要重新得到指针
	//	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	//	if(!m_pActionItem)
	//	{
	//		state->PushString("");
	//		return 1;
	//	}

	//	state->PushString(m_pActionItem->GetBaseWhiteAttrInfo());
	//	return 1;
	//}


    //得到装备基本属性
	INT	SuperTooltips::GetEquipBaseAttrInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item || pActionItem_Item->GetEquipIdentify() == CObject_Item_Equip::EQUIP_ATTRIB_UNIDENTIFY)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(pActionItem_Item->GetEquipBaseAttrInfo());
		return 1;


	}
	// 查询Tooltip的装备的附加属性    
	INT	SuperTooltips::GetEquipAddAttrInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(pActionItem_Item->GetEquipAddAttrInfo());
		return 1;
	}



	// 查询Tooltip的装备的技能属性    
	INT	SuperTooltips::GetEquipSkillInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(pActionItem_Item->GetEquipSkillInfo());
		return 1;
	}

	
	// 查询Tooltip的物品的类型
	INT	SuperTooltips::GetItemType(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetType_String());
		return 1;
	}


	// 查询Tooltip的装备的品质(白, 蓝, 绿)  //remove
	//INT	SuperTooltips::GetItemEquipQuantity(LuaPlus::LuaState* state)
	//{
	//	// 不利用保存的指针。要重新得到指针
	//	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
	//
	//	CActionItem_Item* pActionItem_Item = NULL;
	//	pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	//	if(!pActionItem_Item)
	//	{
	//		state->PushInteger(0);
	//		return 1;
	//	}

	//	state->PushInteger(pActionItem_Item->GetEquipQuantity());
	//	return 1;

	//}

	//查询Tooltip的装备的类型属性
	INT	SuperTooltips::GetItemClassType(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetItemType());
		return 1;
	}

	//查询Tooltip的装备的宝石信息
	//
	//
	//   
	// 传出参数5个
	//   0[int] - 宝石个数
	//   1[str] - 宝石1的图标字符串 "" - 表示无图标
	//	 2[str] - 宝石2的图标字符串 "" - 表示无图标
	//	 3[str] - 宝石2的图标字符串 "" - 表示无图标
	//	 4[str] - 宝石附加属性的描述
	INT	SuperTooltips::GetGemInEquipInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			// 得到宝石的个数
			STRING strInfo = _T("");
			INT iCount = pActionItem_Item->GetGemInEquipCount();

			state->PushInteger(iCount);//1

			for(int i = 0; i < DBC_ITEM__EQUIP_GEM_MAXNUM; i++)
			{
				strInfo = _T("");
				pActionItem_Item->GetGemIcon(i, strInfo);
				state->PushString(strInfo.c_str());//7
			}

			strInfo = _T("");
			pActionItem_Item->GetGemInEquipDesc(strInfo);
			state->PushString(strInfo.c_str()); //1
		}
		else//
		{

			state->PushInteger(0);
			state->PushString("");
			state->PushString("");
			state->PushString("");
			state->PushString("");
			state->PushString("");
			state->PushString("");
			state->PushString("");
			state->PushString("");

		}

		return 9;
	}

	INT SuperTooltips::GetNeedGemInfo(LuaPlus::LuaState* state)
	{
		state->PushString("");
		return 1;
	}


	

	//----------------------------------------------------------------------------------------------------------------------
	// 心法函数
	//
	
	// 得到心法等级
	// 传出参数1个
	// 0[int] - 当前学会的心法等级.
	INT	SuperTooltips::GetXinfaLevel(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}

		/*CActionItem_XinFa* pXinfaAction = (CActionItem_XinFa*)m_pActionItem;
		if(pXinfaAction)
		{
			state->PushInteger(pXinfaAction->GetXinfaLevel());
			return 1;
		}*/

		state->PushInteger(0);
		return 1;
		
	}


	//----------------------------------------------------------------------------------------------------------------------
	// 技能函数
	//

	// 得到技能等级
	// 传出参数1个
	// 0[int] - 当前学会的技能等级.
	INT	SuperTooltips::GetSkillLevel(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			state->PushInteger(pAction->GetSkillLevel());
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}


	//----------------------------------------------------------------------------------------------------------------------
	// 得到是否学会技能
	//

	// 得到是否学会技能
	// 传出参数1个
	// 0[int] - 是否学会 1 - 学会, 0 - 没学会
	INT	SuperTooltips::GetIsLearnedSkill(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			state->PushInteger(pAction->GetIsLearnedSkill());
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}

	//----------------------------------------------------------------------------------------------------------------------
	// 得到技能描述
	//

	// 得到技能描述
	// 传出参数1个
	// 0[str] - 得到技能描述
	INT	SuperTooltips::GetSkillDesc(LuaPlus::LuaState* state)
	{

		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;

		if(pAction)
		{
			state->PushString(pAction->GetDesc());
			return 1;
		}

		state->PushString(_T(""));
		return 1;
	}

	INT	SuperTooltips::IsPassiveSkill(LuaPlus::LuaState* state)
	{

		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;

		if(pAction)
		{
			state->PushInteger(pAction->IsPassiveSkill());
			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}

		
    //----------------------------------------------------------------------------------------------------------------------
	//
	// 宝石信息
	//

	// 得到宝石等级
	// 传出参数1个
	// 0[int] - 宝石等级
	INT	SuperTooltips::GetGemLevel(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
			state->PushInteger(pActionItem_Item->GetGemLevel());
			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}

	// 得到宝石属性
	// 传出参数1个
	// 0[str] - 宝石属性
	INT	SuperTooltips::GetGemArribInfo(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
			state->PushString(pActionItem_Item->GetGemArribInfo());
			return 1;
		}

		state->PushString("");
		return 1;
	}


	//得到魂珠的属性类型
	INT SuperTooltips::GetFecthHeadInfo_TypeDesc( LuaPlus::LuaState* state )
	{

		LuaStack args(state);
		if( !(args[2].IsInteger()) )
		{
			state->PushString("");
			return 1;
		}

		INT type = args[2].GetInteger();

		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{

			state->PushString(pActionItem_Item->GetFecthHeadInfo_TypeDesc(type) );
			return 1;
		}

		state->PushString("");
		return 1;
	}

	//得到魂珠的属性值
	INT SuperTooltips::GetFecthHeadInfo_Value( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !(args[2].IsInteger()) ) 
		{
			state->PushInteger(0);
		}

		INT type = args[2].GetInteger();

		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{

			state->PushInteger(pActionItem_Item->GetFecthHeadInfo_Value(type) );
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}

	//得到魂珠的属性数量
	INT SuperTooltips::GetFecthHeadInfo_Num( LuaPlus::LuaState* state )
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if ( pActionItem_Item )
		{
			state->PushInteger( pActionItem_Item->GetFecthHeadInfo_Num() );
			return 1;
		}
		state->PushInteger(0);
		return 1;
	}


	//----------------------------------------------------------------------------------------------------------------------
	//
	// 药品或宝石信息
	//

	// 得到药品或宝石品质
	// 传出参数1个
	// 0[int] - 药品或宝石品质
	INT	SuperTooltips::GetItemQuality(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
		//	state->PushInteger(pActionItem_Item->GetItemQuality());  //remove
			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}

//得到是普通装备还是高级装备
INT SuperTooltips::GetItemEquipQua(LuaPlus::LuaState* state)
 {
	 m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	 CActionItem_Item* pActionItem_Item = NULL;
	 pActionItem_Item =(CActionItem_Item*)m_pActionItem;

	 if(pActionItem_Item)
	 {
		 state->PushInteger(pActionItem_Item->GetItemEquipQua());  
		 return 1;
	 }

	 state->PushInteger(-1);
	 return 1;


 }



	// 得到技能对应的心法等级
	//
	// 传出参数1个
	// 0[int] - 心法等级
	INT	SuperTooltips::GetSkillXinfaLevel(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		/*m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Skill* pAction = NULL;

		pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			state->PushInteger(pAction->GetSkillXinfaLevel());
			return 1;
		}*/

		state->PushInteger(0);
		return 1;

	}

	// 得到物品在表中的类型
	// 传出参数1个
	// 0[int] - 表中的类型。
	INT	SuperTooltips::GetItemTableType(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			
			state->PushInteger(pActionItem_Item->GetMedicineItemTableType());
			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}


	// 得到配方需要的技能名字。
	// 传出参数1个
	// 0[int] - 表中的类型。
	INT	SuperTooltips::GetPrescrSkillName(LuaPlus::LuaState* state)
	{

		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			//CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)pActionItem_Item->GetItemImpl();
			//if(pItem)
			//{
			//	INT nIndex = pItem->m_theBaseDef->nScriptID - 700000;

			//	//打开宝石数据表
			//	DBC_DEFINEHANDLE(s_pItem_Compound, DBC_LIFEABILITY_ITEMCOMPOSE);
			//	_DBC_LIFEABILITY_ITEMCOMPOSE* pData = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_pItem_Compound->Search_Index_EQU(nIndex);

			//	if(pData)
			//	{
			//		state->PushString(pData->szName);
			//		return 1;
			//	}
			//	//const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
			//	//if(pPrescr)
			//	//{
			//	//	STRING strTemp = "";
			//	//	strTemp = NOCOLORMSGFUNC("Obj_Item_Medicine_Info_Learned");
			//	//	m_strExtraDesc += strTemp;
			//	//}//

			//}//

			LPCTSTR pNeedSkillName = pActionItem_Item->GetPrescrNeedSkillName();
			if(pNeedSkillName)
			{
				state->PushString(pNeedSkillName);
				return 1;
			}
		}//


		state->PushString("");
		return 1;
	}


	
	// 得到配方需要的技能等级。
	// 传出参数1个
	// 0[int] - 表中的等级。
	INT	SuperTooltips::GetPrescrNeedSkillLevel(LuaPlus::LuaState* state)
	{

		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			int iNeedLevel = pActionItem_Item->GetPrescrNeedSkillLevel();
			state->PushInteger(iNeedLevel);
			return 1;
			
		}//


		state->PushInteger(-1);
		return 1;
	}


	// 得到配方需要技能的当前等级。
	// 传出参数1个
	// 0[int] - 配方需要的当前等级。
	INT	SuperTooltips::GetPrescrNeedSkillCurLevel(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			int iNeedCurLevel = pActionItem_Item->GetPrescrNeedSkillCurLevel();
				
			state->PushInteger(iNeedCurLevel);
			return 1;
			
		}//

		state->PushInteger(0);
		return 1;
	}

	// 向服务器请求item的详细信息
	//  
	INT	SuperTooltips::SendAskItemInfoMsg(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针



		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(m_pActionItem->GetType() != AOT_ITEM) return 0;

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			pActionItem_Item->SendAskItemInfoMsg();
			
		}

		return 0;
	}


	// 得到装备是否鉴定 
	 
	INT	SuperTooltips::IsEquipIdentify(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;
		if(pActionItem_Item)
		{
			INT nIdentify = pActionItem_Item->GetEquipIdentify();
			state->PushInteger(nIdentify);
		}
		else
		{
			
			state->PushInteger(0);
		}

		return 1;
	}


	
	//
	// npc商店是否打开。
	// 
	INT	SuperTooltips::IsNpcShopOpen(LuaPlus::LuaState* state)
	{

		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		return 1;
	}

	INT SuperTooltips::IsPresent(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		state->PushInteger(m_pActionItem ? 1 : 0);
		return 1;
	}

	// 得到描述信息
	INT SuperTooltips::GetTypeDesc(LuaPlus::LuaState* state)
	{

		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;
		if(pActionItem_Item)
		{
			state->PushString(pActionItem_Item->GetTypeDesc());
		}

		return 1;
	}

	//得到是否是搜侠物品
	INT SuperTooltips::IsSouXiaItem( LuaPlus::LuaState* state )
	{
		//不利用保存的指针，重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if ( m_pActionItem )
		{
			tObject_Item* pItem = (tObject_Item*)( m_pActionItem->GetImpl() );
			if ( pItem )
			{				
				if ( pItem->GetItemClass()==ICLASS_EQUIP && pItem->GetItemTableType()==EQUIP_SOUXIA )
				{
					state->PushInteger(1);
					return 1;
				}
			}
		}
		state->PushInteger(0);
		return 1;
	}

	INT SuperTooltips::GetItemIsOnStall(LuaPlus::LuaState* state)
	{
		state->PushInteger(0);
		return 1;
	}

	INT SuperTooltips::GetItemPriceOnStall(LuaPlus::LuaState* state)
	{
		state->PushInteger(100);
		return 1;
	}
	
	// 得到物品是否在背包中
	INT SuperTooltips::IsItemInPacket(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushString(0);
			return 1;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// 使用卷轴的背包索引位置
		CObject_Item* pItem = (CObject_Item*)pActionItem_Item->GetItemImpl();
		if(NULL == pItem)
		{
			state->PushInteger(0);
			return 1;
		}

		if(tObject_Item::IO_MYSELF_PACKET == pItem->GetTypeOwner())
		{
			state->PushInteger(1);
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}


	INT SuperTooltips::GetSkillMaxLevel(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			const SCLIENT_SKILL* pSkill = (const SCLIENT_SKILL*)pAction->GetImpl();
			if(pSkill) 
			{
				state->PushInteger(pSkill->GetSkillData()->m_nMaxLevel);
				return 1;
			}
		}
		state->PushInteger(100);
		return 1;
	}

	INT SuperTooltips::GetSkillType(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			const SCLIENT_SKILL* pSkill = (SCLIENT_SKILL*)pAction->GetImpl();
			if(pSkill) 
			{
				// 20100430 ModifyCodeBegin 取技能招式类型改为取技能类型
				state->PushInteger(pSkill->GetSkillData()->m_nType);
				// 20100430 ModifyCodeEnd
				return 1;
			}
		}
		
		state->PushInteger(1);
		return 1;
	}
	INT SuperTooltips::GetSkillCoolDown(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem )
		{
			state->PushInteger(0);
			return 1;
		}
		state->PushInteger(m_pActionItem->GetCooldownTime());
		return 1;
	}
	INT SuperTooltips::GetSkillAttackRange(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			const SCLIENT_SKILL* pSkill = (SCLIENT_SKILL*)pAction->GetImpl();
			if(pSkill) 
			{
				state->PushInteger(pSkill->GetSkillData()->m_fMinAttackRange);
				state->PushInteger(pSkill->GetSkillData()->m_fMaxAttackRange);
				return 2;
			}
		}

		state->PushInteger(0);
		state->PushInteger(10);
		return 2;
	}

	INT SuperTooltips::GetSkillGatherTime(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			const SCLIENT_SKILL* pSkill = (SCLIENT_SKILL*)pAction->GetImpl();
			if(pSkill) 
			{
				state->PushInteger(atoi(pSkill->GetSkillData()->m_lpszJiQiTime));
				return 1;
			}
		}

		state->PushInteger(1);
		return 1;
	}

	INT SuperTooltips::GetSkillLeadTime(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}

		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		if(pAction)
		{
			const SCLIENT_SKILL* pSkill = (SCLIENT_SKILL*)pAction->GetImpl();
			if(pSkill) 
			{
				state->PushInteger(atoi(pSkill->GetSkillData()->m_lpszDirectTime));
				return 1;
			}
		}

		state->PushInteger(1);
		return 1;
	}

	INT SuperTooltips::GetNextLvSkillDesc(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}
		CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		state->PushString( pAction->GetNextLevelDesc() );
		return 1;
	}

	INT SuperTooltips::GetNextLvSkillGatherTime(LuaPlus::LuaState* state)
	{
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(0);
			return 1;
		}
		state->PushInteger(0);
		//CActionItem_Skill* pAction = (CActionItem_Skill*)m_pActionItem;
		return 1;
	}

	INT SuperTooltips::GetNextLvSkillLeadTime(LuaPlus::LuaState* state)
	{
		state->PushInteger(1);
		return 1;
	}

	INT SuperTooltips::GetSkillLvUpLevelQua(LuaPlus::LuaState* state)
	{
		state->PushInteger(1);
		state->PushInteger(1);
		state->PushInteger(1);
		return 3;
	}

	INT SuperTooltips::GetSkillLvUpPointQua(LuaPlus::LuaState* state)
	{
		state->PushInteger(1);
		state->PushInteger(1);
		state->PushInteger(1);
		return 3;
	}

	INT SuperTooltips::GetSkillLvUpFahterSkillQua(LuaPlus::LuaState* state)
	{
		state->PushInteger(1);
		state->PushString("");
		state->PushInteger(1);
		state->PushInteger(1);
		return 4;
	}

	INT SuperTooltips::GetSkillLvUpMoneyQua(LuaPlus::LuaState* state)
	{
		state->PushInteger(1);
		state->PushInteger(1);
		state->PushInteger(1);
		return 3;
	}

	INT SuperTooltips::GetSkillLvUpXpQua(LuaPlus::LuaState* state)
	{
		state->PushInteger(1);
		state->PushInteger(1);
		state->PushInteger(1);
		return 3;
	}

	INT SuperTooltips::GetSkillLvUpItemQua(LuaPlus::LuaState* state)
	{
		state->PushInteger(1);
		state->PushString("");
		state->PushInteger(1);
		state->PushInteger(1);
		return 4;
	}

	// 得到坐骑增加速度  [7/21/2010 陈军龙]
	INT	SuperTooltips::GetMountAddtionSpeed(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetMountAddtionSpeed());
		return 1;
	}

	// 得到坐骑存在时限  [7/21/2010 陈军龙]
	INT	SuperTooltips::GetMountLife(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetMountLife());
		return 1;
	}

	// 得到坐骑获取时间  [7/21/2010 陈军龙]
	INT	SuperTooltips::GetMountObtainDate(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetMountObtainDate());
		return 1;
	}

	// 得到坐骑剩余使用时间  [7/21/2010 陈军龙]
	INT	SuperTooltips::GetMountRemainTime(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushInteger(0);
			return 1;
		}

		state->PushInteger(pActionItem_Item->GetMountRemainTime());
		return 1;
	}

	// 得到坐骑当前是否已绑定  [7/21/2010 陈军龙]
	INT	SuperTooltips::GetMountIsBinded(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushBoolean(FALSE);
			return 1;
		}

		state->PushBoolean(pActionItem_Item->GetMountIsBinded());
		return 1;
	}

	// 得到坐骑绑定类型  [7/21/2010 陈军龙]
	INT	SuperTooltips::GetMountBindType(LuaPlus::LuaState* state)
	{
		// 不利用保存的指针。要重新得到指针
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(!pActionItem_Item)
		{
			state->PushBoolean(FALSE);
			return 1;
		}

		state->PushBoolean(pActionItem_Item->GetMountBindType());
		return 1;
	}
}