#include "StdAfx.h"
#include ".\gmgameinterface_script_mount.h"

#include "../dbc/GMDataBase.h"
#include "../Procedure/GameProcedure.h"
#include "../Network/NetManager.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"
#include "LuaPlus.h"
#include "Global.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Object/Manager/ObjectManager.h"
#include "../Action/GMActionSystem.h"
#include "CGManipulatePet.h"
#include "CGSetPetAttrib.h"
#include "../Object/Manager/ObjectManager.h"
#include "../Object/Logic/Character/Obj_PlayerMySelf.h"
#include "../Action/GMActionSystem_Item.h"
#include "../Event/GMEventSystem.h"
#include "../Object/Logic/Item/Obj_Item_Medicine.h"
#include "../Object/Logic/Item/Obj_Item_Mount.h"  //  [7/14/2010 陈军龙]



namespace SCRIPT_SANDBOX
{
	Mount Mount::s_Mount;
	LuaPlus::LuaObject* Mount::s_pMetaTable = NULL;

	/* ------------------------------------------------
	//  查询 坐骑名字      [7/20/2010 陈军龙] 
	参数1	INT		骑宠列表下标
	------------------------------------------------ */
	INT Mount::GetMountName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountName[2] param parameter error");
		}

		INT nMountIndex = args[2].GetInteger();				
		CObject_Item_Mount* itemMount = static_cast<CObject_Item_Mount*>( CDataPool::GetMe()->UserMount_GetItem(nMountIndex) );   

		if (NULL != itemMount)
		{
			if (0 == strcmp(itemMount->GetName(), ""))
			{
				state->PushString("无效名字");
			}
			else
			{
				state->PushString(itemMount->GetName());
			}
		}
		else
		{
			state->PushString("");
		}
		
		return 1;
	}
	/* ------------------------------------------------
	// 查询 骑宠名字
	参数1	INT		骑宠列表下标
	------------------------------------------------ */
	INT	Mount::GetMountNickName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountNickName[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{			
			state->PushString(pPet->m_szNickname.c_str());			
		}
		else
		{
			state->PushString("");
		}

		return 1;
	}



	/* ------------------------------------------------
	// 查询 骑宠级别
	参数1	INT		骑宠列表下标
	------------------------------------------------ */
	INT Mount::GetMountLevel(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountLevel[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nLevel);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠性别
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountGender(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountGender[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nGender);
		}
		else
		{
			state->PushInteger(0);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠代数
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountEraCount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountEraCount[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			DBC_DEFINEHANDLE(s_MountBaseDBC, DBC_MOUNT_BASE_DATA);
			const _DBC_MOUNT_BASE_DATA* pMountBaseData = 
				(const _DBC_MOUNT_BASE_DATA*)s_MountBaseDBC->Search_Index_EQU(pPet->m_nBaseID);

			if (pMountBaseData)
			{
				state->PushInteger(pMountBaseData->nDaiShu);
				return 1;
			}

		}
	
		state->PushInteger(-1);
		

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠是否可交配
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::IsCanBeMating(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::IsCanBeMating[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_bCanBeMating);
		}
		else
		{
			state->PushInteger(FALSE);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 得到 骑宠的GUID
	参数1	MountIndex
	------------------------------------------------ */
	INT Mount::GetMountGUID(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountGUID[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet)
		{		
			CHAR szNum[64] = {0};

			sprintf(szNum, "%d", pPet->m_GUID.m_uHighSection);
			state->PushString(szNum);

			sprintf(szNum, "%d", pPet->m_GUID.m_uLowSection);
			state->PushString(szNum);
			
			return 2;
		}

		state->PushString("0");
		state->PushString("0");

		return 2;
	}

	/* ------------------------------------------------
	// 查询 骑宠配偶
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetSpouseID(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetSpouseID[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			CHAR szID[64] = {0};
			sprintf(szID, "%d", pPet->m_GUID.m_uLowSection);
			state->PushString(szID);
		}
		else
		{
			state->PushString("0");
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠列表
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountList(LuaPlus::LuaState* state)
	{
		INT nNum = 0;
		for (INT i=0; i<HUMAN_PET_MAX_COUNT; ++i)
		{
			const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(i);
			if (pPet && FALSE == pPet->m_GUID.IsNull())
			{
				state->PushString(pPet->m_szName.c_str());
				state->PushInteger(i);
				++nNum;
			}
		}

		return nNum;
	}

	/* ------------------------------------------------
	// 查询 骑宠快乐值
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT	Mount::GetMountHappiness(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountHappiness[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nHappiness);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠成长率
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountGrowRate(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountGrowRate[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nUpRate);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠根骨
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountBasic(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountBasic[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nBasic);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠悟性
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountSavvy(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountSavvy[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nSavvy);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}



	/* ------------------------------------------------
	// 查询 骑宠力量
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountStr(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountStr[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrStr);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠敏捷
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountDex(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountDex[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrDex);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠体力
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT	Mount::GetMountCon(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountCon[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrCon);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠智力
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT	Mount::GetMountInt(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountInt[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrInt);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠潜能
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountPotential(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountPotential[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nPotential);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠力量资质
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountStrApt(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountStrApt[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrStrApt);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠敏捷资质
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountDexApt(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountDexApt[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrDexApt);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠体力资质
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountConApt(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountConApt[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrConApt);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}


	/* ------------------------------------------------
	// 查询 骑宠智力资质
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetMountIntApt(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountIntApt[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrIntApt);
		}
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠UI模型名称
	参数1	INT		骑宠标示
	------------------------------------------------ */
	INT Mount::GetUIModelName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetUIModelName[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const CHAR* pName = CDataPool::GetMe()->Pet_GetPetFakeName(nMountIndex);
		if (pName)
		{
			state->PushString(pName);
		}
		else
		{
			state->PushString("");
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 坐骑UI模型名称  [7/14/2010 陈军龙]
	参数1	INT		坐骑标示
	------------------------------------------------ */
	INT Mount::GetMountUIModelName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountUIModelName[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		const CHAR* pName = CDataPool::GetMe()->Mount_GetMountFakeName(nMountIndex);
		if (pName)
		{
			state->PushString(pName);
		}
		else
		{
			state->PushString("");
		}

		return 1;
	}

	/* ------------------------------------------------
	// 查询 骑宠技能
	参数1	INT		骑宠标示
	参数2	INT		技能下标
	------------------------------------------------ */
	INT Mount::GetMountSkill(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountSkill[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		if (!(args[3].IsInteger()))
		{
			KLThrow("Mount::GetMountSkill[3] param parameter error");
		}
		INT nSkillIndex = args[3].GetInteger();
		if (nSkillIndex < 0 || nSkillIndex >= PET_MAX_SKILL_COUNT)
		{
			KLThrow("GetModelSkill(), nSkillIndex 越界！");
			nSkillIndex = 0;
		}

		const SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPetData)
		{
			const PET_SKILL& skill = pPetData->m_Skill[nSkillIndex];
			state->PushInteger(skill.m_bCanUse);
			
			if (skill.m_pDefine)
			{
				tActionItem* pSkillAction = CActionSystem::GetMe()->GetAction_PetSkillID(pPetData->m_GUID, skill.m_pDefine->m_nID);
				if (pSkillAction)
				{
					state->PushInteger(pSkillAction->GetID());
				}
				else
				{
					state->PushInteger(-1);
				}
			}
			else
			{
				state->PushInteger(-1);
			}
		}
		else
		{
			state->PushInteger(FALSE);
			state->PushInteger(-1);
		}

		return 2;
	}

	/* ------------------------------------------------
	// 查询 坐骑操作   [7/14/2010 陈军龙]
	参数1	INT		坐骑ID
	参数2	INT		Action ID
	------------------------------------------------ */
	INT Mount::GetMountAction(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetMountItem[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();		

		if (nMountIndex < 0 || nMountIndex >= MAX_MOUNT_SIZE)
		{
			KLThrow("GetMountItem(), nMountIndex 越界！");
			nMountIndex = 0;
		}

		const tObject_Item* pMountItem = CDataPool::GetMe()->UserMount_GetItem(nMountIndex);

		if (NULL != pMountItem)
		{
			INT mountID = pMountItem->GetID();
			state->PushInteger(mountID);
			
			if (INVALID_ID != mountID)
			{
				tActionItem* pMountAction = CActionSystem::GetMe()->GetAction_ItemID(mountID);
				if (NULL != pMountAction)
				{
					state->PushInteger(pMountAction->GetID());
				}
				else
				{
					state->PushInteger(-1);
				}
			}
			else
			{
				state->PushInteger(-1);
			}
		}
		else
		{
			state->PushInteger(INVALID_ID);
			state->PushInteger(-1);
		}

		return 2;
	}

	/* ------------------------------------------------
	// 是否骑乘中
	------------------------------------------------ */
	INT Mount::IsMounting(LuaPlus::LuaState* state)
	{
		if (CObjectManager::GetMe()->GetMySelf()->GetCharacterData())
		{
			INT nMount = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID();
			if (nMount > 0)
			{
				state->PushInteger(1);
			}
			else
			{
				state->PushInteger(0);
			}
		}
		else
		{
			state->PushInteger(0);
		}			
		
		return 1;
	}

	/* ------------------------------------------------
	// 上马
	------------------------------------------------ */
	INT Mount::Mounting(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::Mounting[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		// 摆摊中
		if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
		{
			return 0;
		}
		
		const SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			//向服务器发送
			CGManipulatePet Msg;

			Msg.SetGUID(pPet->m_GUID);
			Msg.SetManipulateType( MANIPULATE_CREATEHORSE );
			CNetManager::GetMe()->SendPacket(&Msg);
		}	

		return 0;
	}

	/* ------------------------------------------------
	// 召唤坐骑  [7/14/2010 陈军龙]
	------------------------------------------------ */
	INT Mount::CallMount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::Mounting[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		CGameInterface::GetMe()->PacketItem_CallMount(nMountIndex);

		return 0;
	}

	/* ------------------------------------------------
	// 下马
	------------------------------------------------ */
	INT Mount::Dismount(LuaPlus::LuaState* state)
	{
		if (NULL != CObjectManager::GetMe()->GetMySelf()->GetCharacterData())
		{
			INT nMount = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID();
			if (nMount > 0)
			{
				//向服务器发送
				CGManipulatePet Msg;

				Msg.SetGUID( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID() );
				Msg.SetManipulateType( MANIPULATE_DELETEHORSE );
				CNetManager::GetMe()->SendPacket(&Msg);
			}
		}
		return 0;
	}

	/* ------------------------------------------------
	// 召回坐骑  [7/14/2010 陈军龙]
	------------------------------------------------ */
	INT Mount::ReCallMount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::Mounting[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		CGameInterface::GetMe()->PacketItem_RecallMount(nMountIndex);

		return 0;
	}

	/* ------------------------------------------------
	// 放生
	参数1	GUID m_uHighSection
	参数2	GUID m_uLowSection
	------------------------------------------------ */
	INT Mount::FreeMount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsString()))
		{
			KLThrow("Mount::FreeMount[2] param parameter error");
		}
		STRING szHighSection = args[2].GetString();

		if (!(args[3].IsString()))
		{
			KLThrow("Mount::FreeMount[3] param parameter error");
		}
		STRING szLowSection = args[3].GetString();

		PET_GUID_t GUID;
		GUID.m_uHighSection = atol(szHighSection.c_str());
		GUID.m_uLowSection	= atol(szLowSection.c_str());

		const SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(GUID); 
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			//向服务器发送
			CGManipulatePet Msg;

			Msg.SetGUID(pPet->m_GUID);
			Msg.SetManipulateType( MANIPULATE_FREEHORSE );
			CNetManager::GetMe()->SendPacket(&Msg);
		}

		return 0;
	}

	/* ------------------------------------------------
	// 丢弃坐骑  [7/14/2010 陈军龙]
	------------------------------------------------ */
	INT Mount::DiscardMount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::Mounting[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		CGameInterface::GetMe()->PacketItem_DiscardMount(nMountIndex);

		return 0;
	}

	/* ------------------------------------------------
	// 目标骑宠名字
	------------------------------------------------ */
	INT	Mount::GetTargetMountName(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushString(pPet->m_szName.c_str());
		}
		else
		{
			state->PushString("");
		}
		
		return 1;
	}

	/* ------------------------------------------------
	// 目标骑宠的昵称
	------------------------------------------------ */
	INT Mount::GetTargetMountNickName(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushString(pPet->m_szNickname.c_str());
		}
		else
		{
			state->PushString("");
		}
		return 1;
	}

	/* ------------------------------------------------
	// 目标骑宠级别
	------------------------------------------------ */
	INT	Mount::GetTargetMountLevel(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nLevel);
		}
		else
		{
			state->PushInteger(-1);
		}
		
		return 1;
	}

	/* ------------------------------------------------
	// 目标骑宠性别
	------------------------------------------------ */
	INT	Mount::GetTargetMountGender(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nGender);
		}		
		else
		{
			state->PushInteger(-1);
		}

		return 1;
	}

	/* ------------------------------------------------
	// 目标骑宠ID
	------------------------------------------------ */
	INT	Mount::GetTargetMountID(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_GUID.m_uLowSection);
		}
		else
		{
			state->PushInteger(-1);
		}
		
		return 1;
	}

	/* ------------------------------------------------
	// 目标配偶ID
	------------------------------------------------ */
	INT	Mount::GetTargetSpouseID(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_GUID.m_uLowSection);
		}
		else
		{
			state->PushInteger(-1);
		}
		
		return 1;
	}

	/* ------------------------------------------------
	// 目标力量
	------------------------------------------------ */
	INT	Mount::GetTargetMountStr(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrStr);
		}
		else
		{
			state->PushInteger(-1);
		}
		
		return 1;
	}

	/* ------------------------------------------------
	// 目标体力
	------------------------------------------------ */
	INT	Mount::GetTargetMountCon(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrCon);
		}
		else
		{
			state->PushInteger(-1);
		}
		
		return 1;
	}

	/* ------------------------------------------------
	// 目标敏捷
	------------------------------------------------ */
	INT	Mount::GetTargetMountDex(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrDex);
		}
		else
		{
			state->PushInteger(-1);
		}
		
		return 1;
	}

	/* ------------------------------------------------
	// 目标智力
	------------------------------------------------ */
	INT	Mount::GetTargetMountInt(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrInt);
		}
		else
		{
			state->PushInteger(-1);
		}
		
		return 1;
	}
	/* ------------------------------------------------
	// 目标力量资质
	------------------------------------------------ */
	INT Mount::GetTargetMountStrApt(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrStrApt);
		}
		else
		{
			state->PushInteger(-1);
		}
		return 1;
	}
	/* ------------------------------------------------
	// 目标敏捷资质
	------------------------------------------------ */
	INT Mount::GetTargetMountDexApt(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrDexApt);
		}
		else
		{
			state->PushInteger(-1);
		}
		return 1;
	}
	/* ------------------------------------------------
	// 目标体力资质
	------------------------------------------------ */
	INT	Mount::GetTargetMountConApt(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrConApt);
		}
		else
		{
			state->PushInteger(-1);
		}
		return 1;
	}
	/* ------------------------------------------------
	// 目标智力资质
	------------------------------------------------ */
	INT Mount::GetTargetMountIntApt(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nAttrIntApt);
		}
		else
		{
			state->PushInteger(-1);
		}
		return 1;
	}
	/* ------------------------------------------------
	// 目标根骨
	------------------------------------------------ */
	INT Mount::GetTargetMountBasic(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nBasic);
		}
		else
		{
			state->PushInteger(-1);
		}
		return 1;
	}
	/* ------------------------------------------------
	// 目标悟性
	------------------------------------------------ */
	INT Mount::GetTargetMountSavvy(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nSavvy);
		}
		else
		{
			state->PushInteger(-1);
		}
		return 1;
	}
	/* ------------------------------------------------
	// 目标成长率
	------------------------------------------------ */
	INT Mount::GetTargetMountGrowRate(LuaPlus::LuaState* state)
	{
		const SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_nUpRate);
		}
		else
		{
			state->PushInteger(-1);
		}
		return 1;
	}

	/* ------------------------------------------------
	// 目标UI模型名
	------------------------------------------------ */
	INT Mount::GetTargetUIModelName(LuaPlus::LuaState* state)
	{
		const CHAR* pName = CDataPool::GetMe()->TargetPet_GetPetFakeModel();
		if (pName)
		{
			state->PushString(pName);
		}
		else
		{
			state->PushString("");
		}
		
		return 1;
	}

	/* ------------------------------------------------
	// 加点确认
	参数1	INT		骑宠下标
	参数2	INT		力量
	参数3	INT		敏捷
	参数4	INT		体力
	参数5	INT		智力
	------------------------------------------------ */
	INT Mount::SetAttrAddRequest(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()))		
			KLThrow("Mount::SetAttrAddRequest[2] param parameter error");
		
		INT nMountIndex = args[2].GetInteger();

		if (!(args[3].IsInteger()))
			KLThrow("Mount::SetAttrAddRequest[3] param parameter error");
		
		INT nStrAdd = args[3].GetInteger();
		

		if (!(args[4].IsInteger()))		
			KLThrow("Mount::SetAttrAddRequest[4] param parameter error");
		
		INT nDexAdd = args[4].GetInteger();
		

		if (!(args[5].IsInteger()))		
			KLThrow("Mount::SetAttrAddRequest[5] param parameter error");
		
		INT nConAdd = args[5].GetInteger();
		

		if (!(args[6].IsInteger()))		
			KLThrow("Mount::SetAttrAddRequest[6] param parameter error");

		INT nIntAdd = args[6].GetInteger();

		// 发送加点消息		
		const SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			CGSetPetAttrib Msg;

			Msg.SetGUID(pPet->m_GUID);
			Msg.SetStrIncrement(nStrAdd);
			Msg.SetDexIncrement(nDexAdd);
			Msg.SetConIncrement(nConAdd);
			Msg.SetIntIncrement(nIntAdd);

			CNetManager::GetMe()->SendPacket(&Msg);
		}
		

		return 0;
	}

	/* ------------------------------------------------
	// 请求目标骑宠属性
	------------------------------------------------ */
	INT Mount::TargetMountAttrRequest(LuaPlus::LuaState* state)
	{
		tObject* pObj = CObjectManager::GetMe()->GetMainTarget();

		if (pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_PlayerOther)))
		{
			CObject_PlayerOther* pOtherPlayer = (CObject_PlayerOther*)pObj;
			if (0 < pOtherPlayer->GetCharacterData()->Get_MountID())
			{
				// 发送 请求目标骑宠属性消息
				CGManipulatePet Msg;

				Msg.SetObjID(pOtherPlayer->GetServerID());
				Msg.SetCharObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
				Msg.SetManipulateType(MANIPULATE_ASKOTHERPETINFO);

				CNetManager::GetMe()->SendPacket(&Msg);
			}
		}

		return 0;
	}

	
	/* ------------------------------------------------
	// 目标技能
	参数2	INT		技能下标
	------------------------------------------------ */
	INT	Mount::GetTargetMountSkill(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::GetTargetMountSkill[2] param parameter error");
		}
		INT nSkillIndex = args[2].GetInteger();
		if (nSkillIndex < 0 || nSkillIndex >= PET_MAX_SKILL_COUNT)
		{
			KLThrow("GetTargetMountSkill(), nSkillIndex 越界！");
			nSkillIndex = 0;
		}	

		const SDATA_PET* pPetData = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPetData)
		{
			const PET_SKILL& skill = pPetData->m_Skill[nSkillIndex];
			state->PushInteger(skill.m_bCanUse);

			if (skill.m_pDefine)
			{
				tActionItem* pSkillAction = CActionSystem::GetMe()->GetAction_PetSkillID(pPetData->m_GUID, skill.m_pDefine->m_nID);
				if (pSkillAction)
				{
					state->PushInteger(pSkillAction->GetID());
				}
				else
				{
					state->PushInteger(-1);
				}
			}
			else
			{
				state->PushInteger(-1);
			}
		}
		else
		{
			state->PushInteger(FALSE);
			state->PushInteger(-1);
		}

		return 2;
	}

	/* ------------------------------------------------
	// 得到 当前出战的坐骑
	------------------------------------------------ */
	INT Mount::GetBattleMountIndex(LuaPlus::LuaState* state)
	{
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData() &&
			0 < CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
		{
			const PET_GUID_t& guid = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID();

			for (INT i=0; i<HUMAN_PET_MAX_COUNT; ++i)
			{
				const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(i);
				if (pPet && FALSE == pPet->m_GUID.IsNull() && pPet->m_GUID == guid)
				{
					state->PushInteger(i);
					return 1;
				}
			}
		}

		state->PushInteger(-1);
		
		return 1;
	}	

	/* ------------------------------------------------
	// 得到 当前UI选择的骑宠
	------------------------------------------------ */
	INT Mount::GetSelectMount(LuaPlus::LuaState* state)
	{
		if (FALSE == m_nSelectMount.IsNull())
		{
			for (INT i=0; i<HUMAN_PET_MAX_COUNT; ++i)
			{
				const SDATA_PET* pPet= CDataPool::GetMe()->Pet_GetPet(i);
				if (pPet && FALSE == pPet->m_GUID.IsNull() && pPet->m_GUID == m_nSelectMount)
				{
					state->PushInteger(i);
					return 1;
				}
			}
		}
		state->PushInteger(0);
		return 1;
	}

	/* ------------------------------------------------
	// 设置 当前UI选择的骑宠
	------------------------------------------------ */
	INT Mount::SetSelectMount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::SetSelectMount[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			m_nSelectMount = pPet->m_GUID;
		}
		else
		{
			m_nSelectMount.Reset();
		}

		return 0;
	}

	/* ------------------------------------------------
	// 解除物品锁定状态
	参数1	ActionItemID
	------------------------------------------------ */
	INT	Mount::ItemUnLock(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::ItemUnLock[2] param parameter error");
		}
		INT nActionItemID = args[2].GetInteger();

		// 有效
		tActionItem* pActionItem = CActionSystem::GetMe()->GetAction(nActionItemID);
		if (pActionItem && AOT_ITEM == pActionItem->GetType())
		{
			// 是物品
			CActionItem_Item* pItem = (CActionItem_Item*)pActionItem;
			tObject_Item *pObj = pItem->GetItemImpl();
			if(pObj && pObj->IsLocked())
			{
				CObject_Item_Medicine* pMedicine_Item = (CObject_Item_Medicine*)pObj;

				// 是坐骑物品
				if (pMedicine_Item->GetItemTableType() == COMMON_HORSE_ITEM)
				{
					// 解锁
					pMedicine_Item->SetLock(FALSE);
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
			}			
		}
		return 0;
	}

	/* ------------------------------------------------
	// 使用物品
	参数1	ActionItemID
	参数2	MountIndex
	------------------------------------------------ */
	INT	Mount::UseMountItem(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::UseMountItem[2] param parameter error");
		}
		INT nActionItemID = args[2].GetInteger();

		if (!(args[3].IsInteger()))
		{
			KLThrow("Mount::UseMountItem[3] param parameter error");
		}
		INT nMountIndex = args[3].GetInteger();

		// 骑宠有效
		SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (NULL == pPet)
			return 0;
		
		// 物品有效
		if (-1 == nActionItemID)
			return 0;
		tActionItem* pAction = CActionSystem::GetMe()->GetAction(nActionItemID);
		if (NULL == pAction)
			return 0;
		if (AOT_ITEM != pAction->GetType())
			return 0;

		CActionItem_Item* pActionItem = (CActionItem_Item*)pAction;
		
		// 是坐骑物品
		tObject_Item *pObj = pActionItem->GetItemImpl();
		if(pObj && pObj->GetItemClass() == ICLASS_COMITEM)
		{
			CObject_Item_Medicine* pMedicine_Item = (CObject_Item_Medicine*)pObj;
			if (pMedicine_Item->GetItemTableType() == COMMON_HORSE_ITEM)
			{
				// 发送物品使用消息
				CGManipulatePet Msg;

				Msg.SetGUID( pPet->m_GUID );
				Msg.SetItemID( pMedicine_Item->GetIdTable() );
				Msg.SetTypeData( pMedicine_Item->GetNumber() );
				Msg.SetManipulateType( MANIPULATE_HORSE_ITEM );
				Msg.SetBagIndex(pMedicine_Item->GetPosIndex());

				CNetManager::GetMe()->SendPacket(&Msg);
			}
		}

		return 0;
	}

	/* ------------------------------------------------
	// 申请改变坐骑的名字
	参数1	MountIndex
	参数2	MountName
	------------------------------------------------ */
	INT Mount::ChangeMountName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::ChangeMountName[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		if (!(args[3].IsString()))
		{
			KLThrow("Mount::ChangeMountName[3] param parameter error");
		}
		STRING szNickName = args[3].GetString();

		INT nNum = strlen(szNickName.c_str());
		// 
		if (nNum <= 0)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "名字不能为空");
			return 0;
		}

		// 名字最大长度
		if (nNum >= MAX_NICK_NAME-1)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "名字字数过长");
			return 0;
		}

		SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			// 和当前名不一样
			if (0 != strcmp(szNickName.c_str(), pPet->m_szNickname.c_str()))
			{
				// 发送改名消息
				CGManipulatePet Msg;

				Msg.SetGUID( pPet->m_GUID );
				Msg.SetNickName(szNickName.c_str());
				Msg.SetManipulateType( MANIPULATE_HORSE_NICKNAME );

				CNetManager::GetMe()->SendPacket(&Msg);
			}			
		}

		return 0;
	}

	/* ------------------------------------------------
	// 鉴定骑宠成长率
	参数1	MountIndex
	------------------------------------------------ */
	INT Mount::RequestIdentifyMount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::RequestIdentifyMount[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			if (pPet->m_nUpRate < 0)
			{
				// 发送鉴定消息
				CGManipulatePet Msg;

				Msg.SetGUID( pPet->m_GUID );
				Msg.SetManipulateType( MANIPULATE_HORSE_CHECKUP );

				CNetManager::GetMe()->SendPacket(&Msg);
			}
		}

		return 0;
	}

	/* ------------------------------------------------
	// 是否可繁殖
	参数1	MountIndex
	------------------------------------------------ */
	INT	Mount::CanMating(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::CanMating[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			DBC_DEFINEHANDLE(s_MountBaseDBC, DBC_MOUNT_BASE_DATA);
			const _DBC_MOUNT_BASE_DATA* pMountBaseData = 
							(const _DBC_MOUNT_BASE_DATA*)s_MountBaseDBC->Search_Index_EQU(pPet->m_nBaseID);
			if (pMountBaseData)
			{
				state->PushInteger(pMountBaseData->bMating);
				return 1;
			}			
		}

		state->PushInteger(0);

		return 1;
	}
	/* ------------------------------------------------
	// 查询 加锁状态
	参数1	MountIndex
	------------------------------------------------ */
	INT	Mount::IsLocked(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::IsLocked[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			state->PushInteger(pPet->m_bLocked);

			return 1;
		}

		state->PushInteger(0);

		return 1;
	}


	/* ------------------------------------------------
	// 发送交配锁定或解锁消息
	参数1	MountIndex
	参数2	nLocked		解锁或加锁
	------------------------------------------------ */
	INT	Mount::RequestMyMountMatingLocked(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::RequestMyMountMatingLocked[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		if (!(args[3].IsInteger()))
		{
			KLThrow("Mount::RequestMyMountMatingLocked[3] param parameter error");
		}
		INT nLocked = args[3].GetInteger();

		// 发送解锁消息
		if (0 == nLocked)
		{
			CGManipulatePet Msg;

			Msg.SetManipulateType( MANIPULATE_HORSE_MATING_UNLOCK );

			CNetManager::GetMe()->SendPacket(&Msg);
		}
		// 锁定
		else
		{
			SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
			if (pPet && FALSE == pPet->m_GUID.IsNull())
			{
				CGManipulatePet Msg;

				Msg.SetGUID( pPet->m_GUID );
				Msg.SetManipulateType( MANIPULATE_HORSE_MATING_LOCK );

				CNetManager::GetMe()->SendPacket(&Msg);
			}
		}		

		return 0;
	}

	/* ------------------------------------------------
	// 发送打开繁殖界面消息
	参数1	MountIndex
	------------------------------------------------ */
	INT Mount::RequestTogleMatingPage(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::RequestTogleMatingPage[2] param parameter error");
		}
		INT nNpcSeverID = args[2].GetInteger();

		tObject* pObj = CObjectManager::GetMe()->FindServerObject(nNpcSeverID);
		if (pObj)
		{
			// 发送打开界面消息
			CGManipulatePet Msg;

			Msg.SetCharObjID(nNpcSeverID);
			Msg.SetManipulateType( MANIPULATE_HORSE_OPEN_MATING_UI );

			CNetManager::GetMe()->SendPacket(&Msg);
		}	
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "目标距离太远");
		}

		return 0;
	}

	/* ------------------------------------------------
	// 发送确认或取消交配消息
	参数1	MountIndex
	参数2	nOK			确认或取消
	参数3	nNpcSeverID
	------------------------------------------------ */
	INT Mount::RequestMating(LuaPlus::LuaState* state)
	{
		LuaStack args(state);		

		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::RequestMyMountMatingLocked[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		if (!(args[3].IsInteger()))
		{
			KLThrow("Mount::RequestMating[3] param parameter error");
		}
		INT nOK = args[3].GetInteger();

		if (!(args[4].IsInteger()))
		{
			KLThrow("Mount::RequestMating[4] param parameter error");
		}
		INT nNpcSeverID = args[4].GetInteger();

		if (TRUE == nOK)
		{
			// 发送确认消息
			SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
			if (pPet && FALSE == pPet->m_GUID.IsNull())
			{
				CGManipulatePet Msg;

				Msg.SetGUID( pPet->m_GUID );
				Msg.SetCharObjID(nNpcSeverID);
				Msg.SetManipulateType( MANIPULATE_HORSE_MATING_CONFIRM );

				CNetManager::GetMe()->SendPacket(&Msg);
			}				
		}
		else
		{
			// 发送取消消息
			CGManipulatePet Msg;

			Msg.SetManipulateType( MANIPULATE_HORSE_MATING_CANCEL );

			CNetManager::GetMe()->SendPacket(&Msg);
		}		

		return 0;
	}

	/* ------------------------------------------------
	// 发送领取繁殖坐骑消息
	参数1	nNpcSeverID
	------------------------------------------------ */
	INT Mount::RequestGainMatingMount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);		

		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::RequestGainMatingMount[2] param parameter error");
		}
		INT nNpcSeverID = args[2].GetInteger();

		//  npc有效
		tObject* pObj = CObjectManager::GetMe()->FindServerObject(nNpcSeverID);
		if (pObj)
		{
			/// 发送领取繁殖坐骑消息
			CGManipulatePet Msg;

			Msg.SetCharObjID(nNpcSeverID);
			Msg.SetManipulateType( MANIPULATE_HORSE_MATING_RETRIEVE );

			CNetManager::GetMe()->SendPacket(&Msg);
		}

		return 0;
	}


}










