
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
//�ű�����ɳ�䣬��Щ����Ӧ���ɽű�����
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

	//����Ϸ�߼����ü����е�ActionItem
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
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		STRING type( m_pActionItem->GetType_String() );
		if( type == "changesuit" )
		{
			state->PushString("һ����װ");
			return 1;
		}

		state->PushString(m_pActionItem->GetName());
		return 1;
	}


	INT SuperTooltips::GetIcon(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
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
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		//������
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("LUA: SuperTooltips GetAttributeValue Wrong Param1");
		}

		//ͨ��ActionItem��ѯ�߼�����
		state->PushString(m_pActionItem->GetAttributeValue(args[2].GetString()).c_str());

		return 1;
	}

	INT	SuperTooltips::GetItemDesc(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
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
				_snprintf(szTemp, 10, "%s��װ", name.c_str());
				state->PushString(szTemp);

			}
			else
			{
				CHAR szTemp[100];	
				_snprintf(szTemp, 10, "�����Զ���ĵ�%d����װ", id);
				state->PushString(szTemp);
			}

			return 1;
		}

		//�õ���ϸ˵��.
		state->PushString(m_pActionItem->GetDesc());
		return 1;
	}

	INT	SuperTooltips::GetItemLevel(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemLevel());
		return 1;
	}

	//��ѯTooltip����Ʒ�;�
	INT	SuperTooltips::GetItemDur(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
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

	//��ѯTooltip����Ʒ���������
	INT	SuperTooltips::GetItemRepairCount(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
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
	// �õ�����������
	//INT	SuperTooltips::GetEquipMaxRepair(LuaPlus::LuaState* state)
	//{
	//	// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
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

	//��ѯTooltip����Ʒ�İ���Ϣ
	INT	SuperTooltips::GetItemBindInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemBindInfo());
	
		return 1;
	}

	//��ѯTooltip��װ����2�ΰ���Ϣ
	INT	SuperTooltips::GetItemSecLevelBindInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemSecLevelBindInfo());

		return 1;
	}

//	�õ�װ����2�ΰ󶨼ӳ�
	 
	INT	SuperTooltips::GetItemBindAddInfo(LuaPlus::LuaState* state)
	{

		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetItemBindAddInfo());

		return 1;

	}

	//	�õ�װ������ɫ

	INT	SuperTooltips::GetDropVisColor(LuaPlus::LuaState* state)
	{

		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetDropVisColor());

		return 1;

	}

	


	//��ѯTooltip����Ʒ������npc�ļ۸�
	INT	SuperTooltips::GetItemPrice(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushInteger(-1);
			return 1;
		}

		state->PushInteger(m_pActionItem->GetItemPrice());
	
		return 1;
	}

   //��ѯTooltip����Ʒ��ְҵ
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
	//�õ�װ������
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


	//�õ���Ʒ�Ƿ�Ψһ
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
	//�õ�װ��������

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



//�õ�װ����װ��Ϣ

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


//�õ�װ�����ͼ������Ϣ

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




//�õ��������װ��С�������Ϣ

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





//�õ�װ��С�������Ϣ
 
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
//�õ��������װ���󼤻����Ϣ

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


//�õ�װ���󼤻����Ϣ

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

//�õ�װ����װ��Ϣ

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


//�õ�װ����װ�����������Ϣ

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


//�õ�װ����װ�����������Ϣ

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
    //�õ�װ���Ǽ�

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











	// ��ѯTooltip����Ʒ��������
	INT	SuperTooltips::GetItemManufacturer(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ�롣 
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetManufacturer());
	
		return 1;
	}

	// ��ѯTooltip����Ʒ�İ�ɫ���� 
	//INT	SuperTooltips::GetItemBaseWhiteAttrInfo(LuaPlus::LuaState* state)
	//{
	//	// �����ñ����ָ�롣Ҫ���µõ�ָ��
	//	m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

	//	if(!m_pActionItem)
	//	{
	//		state->PushString("");
	//		return 1;
	//	}

	//	state->PushString(m_pActionItem->GetBaseWhiteAttrInfo());
	//	return 1;
	//}


    //�õ�װ����������
	INT	SuperTooltips::GetEquipBaseAttrInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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
	// ��ѯTooltip��װ���ĸ�������    
	INT	SuperTooltips::GetEquipAddAttrInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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



	// ��ѯTooltip��װ���ļ�������    
	INT	SuperTooltips::GetEquipSkillInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	
	// ��ѯTooltip����Ʒ������
	INT	SuperTooltips::GetItemType(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		if(!m_pActionItem)
		{
			state->PushString("");
			return 1;
		}

		state->PushString(m_pActionItem->GetType_String());
		return 1;
	}


	// ��ѯTooltip��װ����Ʒ��(��, ��, ��)  //remove
	//INT	SuperTooltips::GetItemEquipQuantity(LuaPlus::LuaState* state)
	//{
	//	// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	//��ѯTooltip��װ������������
	INT	SuperTooltips::GetItemClassType(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	//��ѯTooltip��װ���ı�ʯ��Ϣ
	//
	//
	//   
	// ��������5��
	//   0[int] - ��ʯ����
	//   1[str] - ��ʯ1��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 2[str] - ��ʯ2��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 3[str] - ��ʯ2��ͼ���ַ��� "" - ��ʾ��ͼ��
	//	 4[str] - ��ʯ�������Ե�����
	INT	SuperTooltips::GetGemInEquipInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			// �õ���ʯ�ĸ���
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
	// �ķ�����
	//
	
	// �õ��ķ��ȼ�
	// ��������1��
	// 0[int] - ��ǰѧ����ķ��ȼ�.
	INT	SuperTooltips::GetXinfaLevel(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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
	// ���ܺ���
	//

	// �õ����ܵȼ�
	// ��������1��
	// 0[int] - ��ǰѧ��ļ��ܵȼ�.
	INT	SuperTooltips::GetSkillLevel(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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
	// �õ��Ƿ�ѧ�Ἴ��
	//

	// �õ��Ƿ�ѧ�Ἴ��
	// ��������1��
	// 0[int] - �Ƿ�ѧ�� 1 - ѧ��, 0 - ûѧ��
	INT	SuperTooltips::GetIsLearnedSkill(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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
	// �õ���������
	//

	// �õ���������
	// ��������1��
	// 0[str] - �õ���������
	INT	SuperTooltips::GetSkillDesc(LuaPlus::LuaState* state)
	{

		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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
	// ��ʯ��Ϣ
	//

	// �õ���ʯ�ȼ�
	// ��������1��
	// 0[int] - ��ʯ�ȼ�
	INT	SuperTooltips::GetGemLevel(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	// �õ���ʯ����
	// ��������1��
	// 0[str] - ��ʯ����
	INT	SuperTooltips::GetGemArribInfo(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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


	//�õ��������������
	INT SuperTooltips::GetFecthHeadInfo_TypeDesc( LuaPlus::LuaState* state )
	{

		LuaStack args(state);
		if( !(args[2].IsInteger()) )
		{
			state->PushString("");
			return 1;
		}

		INT type = args[2].GetInteger();

		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	//�õ����������ֵ
	INT SuperTooltips::GetFecthHeadInfo_Value( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !(args[2].IsInteger()) ) 
		{
			state->PushInteger(0);
		}

		INT type = args[2].GetInteger();

		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	//�õ��������������
	INT SuperTooltips::GetFecthHeadInfo_Num( LuaPlus::LuaState* state )
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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
	// ҩƷ��ʯ��Ϣ
	//

	// �õ�ҩƷ��ʯƷ��
	// ��������1��
	// 0[int] - ҩƷ��ʯƷ��
	INT	SuperTooltips::GetItemQuality(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

//�õ�����ͨװ�����Ǹ߼�װ��
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



	// �õ����ܶ�Ӧ���ķ��ȼ�
	//
	// ��������1��
	// 0[int] - �ķ��ȼ�
	INT	SuperTooltips::GetSkillXinfaLevel(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	// �õ���Ʒ�ڱ��е�����
	// ��������1��
	// 0[int] - ���е����͡�
	INT	SuperTooltips::GetItemTableType(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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


	// �õ��䷽��Ҫ�ļ������֡�
	// ��������1��
	// 0[int] - ���е����͡�
	INT	SuperTooltips::GetPrescrSkillName(LuaPlus::LuaState* state)
	{

		// �����ñ����ָ�롣Ҫ���µõ�ָ��
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		if(pActionItem_Item)
		{
			//CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)pActionItem_Item->GetItemImpl();
			//if(pItem)
			//{
			//	INT nIndex = pItem->m_theBaseDef->nScriptID - 700000;

			//	//�򿪱�ʯ���ݱ�
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


	
	// �õ��䷽��Ҫ�ļ��ܵȼ���
	// ��������1��
	// 0[int] - ���еĵȼ���
	INT	SuperTooltips::GetPrescrNeedSkillLevel(LuaPlus::LuaState* state)
	{

		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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


	// �õ��䷽��Ҫ���ܵĵ�ǰ�ȼ���
	// ��������1��
	// 0[int] - �䷽��Ҫ�ĵ�ǰ�ȼ���
	INT	SuperTooltips::GetPrescrNeedSkillCurLevel(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	// �����������item����ϸ��Ϣ
	//  
	INT	SuperTooltips::SendAskItemInfoMsg(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��



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


	// �õ�װ���Ƿ���� 
	 
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
	// npc�̵��Ƿ�򿪡�
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
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		state->PushInteger(m_pActionItem ? 1 : 0);
		return 1;
	}

	// �õ�������Ϣ
	INT SuperTooltips::GetTypeDesc(LuaPlus::LuaState* state)
	{

		// �����ñ����ָ�롣Ҫ���µõ�ָ��
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;
		if(pActionItem_Item)
		{
			state->PushString(pActionItem_Item->GetTypeDesc());
		}

		return 1;
	}

	//�õ��Ƿ���������Ʒ
	INT SuperTooltips::IsSouXiaItem( LuaPlus::LuaState* state )
	{
		//�����ñ����ָ�룬���µõ�ָ��
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
	
	// �õ���Ʒ�Ƿ��ڱ�����
	INT SuperTooltips::IsItemInPacket(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
		m_pActionItem = (CActionItem*)CActionSystem::GetMe()->GetAction(m_nActionId);
		if(!m_pActionItem)
		{
			state->PushString(0);
			return 1;
		}

		CActionItem_Item* pActionItem_Item = NULL;
		pActionItem_Item =(CActionItem_Item*)m_pActionItem;

		// ʹ�þ���ı�������λ��
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
				// 20100430 ModifyCodeBegin ȡ������ʽ���͸�Ϊȡ��������
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

	// �õ����������ٶ�  [7/21/2010 �¾���]
	INT	SuperTooltips::GetMountAddtionSpeed(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	// �õ��������ʱ��  [7/21/2010 �¾���]
	INT	SuperTooltips::GetMountLife(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	// �õ������ȡʱ��  [7/21/2010 �¾���]
	INT	SuperTooltips::GetMountObtainDate(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	// �õ�����ʣ��ʹ��ʱ��  [7/21/2010 �¾���]
	INT	SuperTooltips::GetMountRemainTime(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	// �õ����ﵱǰ�Ƿ��Ѱ�  [7/21/2010 �¾���]
	INT	SuperTooltips::GetMountIsBinded(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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

	// �õ����������  [7/21/2010 �¾���]
	INT	SuperTooltips::GetMountBindType(LuaPlus::LuaState* state)
	{
		// �����ñ����ָ�롣Ҫ���µõ�ָ��
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