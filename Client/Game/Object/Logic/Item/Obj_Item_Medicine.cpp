
#include "StdAfx.h"
#include "Obj_Item_Medicine.h"
#include "GIException.h"
#include "..\character\obj_playermyself.h"
#include "DataPool\GMDataPool.h"
#include "DataPool\GMDP_CharacterData.h"
#include "..\..\Manager\ObjectManager.h"
#include "Procedure\GameProcedure.h"
#include "Global.h"
#include "World\WorldManager.h"
#include "GIGameInterface.h"



CObject_Item_Medicine::CObject_Item_Medicine(INT id)
		: CObject_Item(id)
{
	m_theBaseDef = NULL;
}

CObject_Item_Medicine::~CObject_Item_Medicine()
{
}


VOID CObject_Item_Medicine::AsMedicine(const _DBC_ITEM_MEDIC* pMedicDefine)
{
	KLAssert(pMedicDefine);

	m_theBaseDef = pMedicDefine;
	m_nParticularID = ( ( m_theBaseDef->nClass * 100 + m_theBaseDef->nType ) * 10000 ) + m_theBaseDef->nIndex;

}

//得到图标
LPCTSTR  CObject_Item_Medicine::GetIconName(VOID) const
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szIcon;
	}
	return NULL;
	
}

LPCTSTR	CObject_Item_Medicine::GetDropVisualID(VOID)const
{	
	if (m_theBaseDef)
	{
		return m_theBaseDef->nDropModelId;
	}
	
  return NULL;
}

 

LPCTSTR	CObject_Item_Medicine::GetDropVisColor(VOID)const
{	
	if (m_theBaseDef)
	{
		return m_theBaseDef->szDropVisColor;
	}

	return NULL;
}





//设置详细解释
VOID CObject_Item_Medicine::SetExtraInfo(const _ITEM * pItemInfo)
{
	KLAssert(pItemInfo);

	const MEDIC_INFO& infoMedic = pItemInfo->m_Medic;

	SetNumber(pItemInfo->GetItemCount());

	for ( int i=0; i<MAX_ITEM_PARAM; ++i)
	{
		m_Param[i] = pItemInfo->m_Param[i];
	}
}

LPCTSTR  CObject_Item_Medicine::GetExtraDesc(VOID)
{
	if (m_theBaseDef)
	{
		m_strExtraDesc = m_theBaseDef->szDesc;
		return m_strExtraDesc.c_str();
	}
	return NULL;
}

//克隆详细信息
VOID	CObject_Item_Medicine::Clone(const CObject_Item * pItemSource)
{

	SetNumber(((CObject_Item_Medicine*)pItemSource)->GetNumber());
	CObject_Item::Clone(pItemSource);
}

//得到物品的类别
ITEM_CLASS	CObject_Item_Medicine::GetItemClass(VOID) const 
{
	if (m_theBaseDef)
	{
		return (ITEM_CLASS)m_theBaseDef->nClass; 
	}
	return ITEM_CLASS(-1);
}
// 得到物品是否广播

BOOL     CObject_Item_Medicine::GetBroadcast(VOID)const 
{
	if(m_theBaseDef)
	{
		return (m_theBaseDef->nBroadcast);
	}

	return FALSE;
}

	
// 得到物品买入的价格
INT		CObject_Item_Medicine::GetItemBasePrice()
{
	if(m_theBaseDef)
	{
		// 20100408 ModifyCodeBegin
		// return (m_theBaseDef->nBasePrice * GetNumber());
		return (m_theBaseDef->nBasePrice );
		// 20100408 ModifyCodeEnd
	}
	return 0;
}

// 得到物品卖给npc的价格
INT		CObject_Item_Medicine::GetItemPrice()
{
	if(m_theBaseDef)
	{
		// 20100408 ModifyCodeBegin
		//return (m_theBaseDef->nSalePrice * GetNumber());
		return (m_theBaseDef->nSalePrice );
		// 20100408 ModifyCodeEnd
	}
	return -1;

}

INT				CObject_Item_Medicine::GetLayNum(VOID)
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nPileCount;
	}
	return 1;
}

// 得到物品会引起的技能ID
INT		CObject_Item_Medicine::GetItemSkillID()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nSkillID;
	}
	return -1;
}

INT		CObject_Item_Medicine::GetItemTargetType()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nTargetType;
	}
	return ITEM_TARGET_TYPE_INVALID;
}
//物品等级 例如 材料等级
INT	 CObject_Item_Medicine::GetItemLevelTable(VOID)const
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nLevel;
	}
	return -1;
}

INT		CObject_Item_Medicine::GetNeedLevel()	
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nLevel;
	}
	return -1;

}


// 得到消耗品在表中的类型
INT CObject_Item_Medicine::GetMedicineItemTableType()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}

	return -1;
}


// 得到type信息
INT	CObject_Item_Medicine::GetItemTableType(VOID)
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}
 return  -1;
}

bool CObject_Item_Medicine::IsValidTarget(CObject* pSelectObj, fVector2& fvPos, int& objID, PET_GUID_t& petID)
{
	if(!m_theBaseDef) return false;
	//玩家自己
	CObject_PlayerMySelf* pMySlef = CObjectManager::GetMe()->GetMySelf();

	//无需目标
	if(ITEM_TARGET_TYPE_NONE==m_theBaseDef->nTargetType) 
		return true;

	//需要选择一个场景位置
	if(ITEM_TARGET_TYPE_POS==m_theBaseDef->nTargetType) 
	{
		return CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fvPos)==TRUE;
	}

	//目前不支持的方式
	if(	ITEM_TARGET_TYPE_DIR==m_theBaseDef->nTargetType ||	//方向
		ITEM_TARGET_TYPE_ITEM==m_theBaseDef->nTargetType)	//物品->物品
		return false;

	switch(m_theBaseDef->nTargetType)
	{
	case ITEM_TARGET_TYPE_SELF:	//自已
		{
			objID = pMySlef->GetServerID();
		}
		return true;

	case ITEM_TARGET_TYPE_SELF_PET:	//自己的宠物
		{
			petID = pMySlef->GetCharacterData()->Get_CurrentPetGUID();
			//宠物尚未释放
			if(petID.IsNull())
				return false;

			objID = pMySlef->GetServerID();
			return true;
		}
		break;

	case ITEM_TARGET_TYPE_FRIEND:	//友好的目标
	case ITEM_TARGET_TYPE_ENEMY:	//敌对目标
		{
			//必须需要一个目标
			if(!pSelectObj) return false;
			//必须是角色
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;

			//检查阵营
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			//必须是友好目标/敌对目标
			if( (m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND && 
					(pSelectObj==pMySlef || eCampType==RELATION_FRIEND)) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY && 
					(eCampType==RELATION_ENEMY)))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_FRIEND_PLAYER: // 友好玩家	
	case ITEM_TARGET_TYPE_ENEMY_PLAYER:	//	敌对玩家
		{
			//必须需要一个目标
			if(!pSelectObj) return false;
			//必须是玩家
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_PlayerOther)))
				return false;

			//检查阵营
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			//必须是友好玩家/敌对玩家
			if(	(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND_PLAYER &&
					(pSelectObj==pMySlef || eCampType==RELATION_FRIEND)) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY_PLAYER &&
					(eCampType==RELATION_ENEMY)))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_FRIEND_MONSTER:	//友好怪物
	case ITEM_TARGET_TYPE_ENEMY_MONSTER:	//敌对怪物
		{
			//必须需要一个目标
			if(!pSelectObj) return false;
			//必须是角色
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;
			//必须是NPC
			if(((CObject_Character*)pSelectObj)->GetCharacterType() != 
					CObject_Character::CT_MONSTER) return false;

			//检查阵营
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			//必须是友好NPC
			if(	(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND_MONSTER && 
					eCampType==RELATION_FRIEND) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY_MONSTER && 
					eCampType==RELATION_ENEMY))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_FRIEND_PET:	// 友好宠物
	case ITEM_TARGET_TYPE_ENEMY_PET:	// 敌对宠物	
		{
			// 必须需要一个目标
			if(!pSelectObj) 
				return false;
			// 必须是角色
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;
			// 必须是NPC
			if(((CObject_Character*)pSelectObj)->GetCharacterType() != CObject_Character::CT_MONSTER) 
				return false;
			// 必须是宠物
			if(((CObject_PlayerNPC*)pSelectObj)->GetNpcType() != NPC_TYPE_PET) 
				return false;

			// 检查阵营
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			// 必须是友好宠物
			if(	(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND_MONSTER && 
					eCampType==RELATION_FRIEND) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY_PET && 
					eCampType==RELATION_ENEMY))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_ALL_CHARACTER:	// 所有角色
		{
			// 必须需要一个目标
			if(!pSelectObj) 
				return false;
			// 必须是角色
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;

			return true;
		}
		return false;

	default:
		break;
	}

	return false;
}

bool CObject_Item_Medicine::IsAreaTargetType(void) const
{
	if(!m_theBaseDef) return false;
	//玩家自己
	CObject_PlayerMySelf* pMySlef = CObjectManager::GetMe()->GetMySelf();

	//无需目标
	return (ITEM_TARGET_TYPE_POS==m_theBaseDef->nTargetType);
}

bool CObject_Item_Medicine::IsTargetOne(void) const
{
	if(!m_theBaseDef) return false;

	return (ITEM_TARGET_TYPE_SELF==m_theBaseDef->nTargetType || 
			ITEM_TARGET_TYPE_SELF_PET==m_theBaseDef->nTargetType);
}


	// 得到物品的类型描述 
LPCTSTR	CObject_Item_Medicine::GetItemTableTypeDesc()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szTypeDesc;
	}
	return NULL;
	
}


