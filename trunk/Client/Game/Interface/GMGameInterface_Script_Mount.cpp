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
#include "../Object/Logic/Item/Obj_Item_Mount.h"  //  [7/14/2010 �¾���]



namespace SCRIPT_SANDBOX
{
	Mount Mount::s_Mount;
	LuaPlus::LuaObject* Mount::s_pMetaTable = NULL;

	/* ------------------------------------------------
	//  ��ѯ ��������      [7/20/2010 �¾���] 
	����1	INT		����б��±�
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
				state->PushString("��Ч����");
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
	// ��ѯ �������
	����1	INT		����б��±�
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
	// ��ѯ ��輶��
	����1	INT		����б��±�
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
	// ��ѯ ����Ա�
	����1	INT		����ʾ
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
	// ��ѯ ������
	����1	INT		����ʾ
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
	// ��ѯ ����Ƿ�ɽ���
	����1	INT		����ʾ
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
	// �õ� ����GUID
	����1	MountIndex
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
	// ��ѯ �����ż
	����1	INT		����ʾ
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
	// ��ѯ ����б�
	����1	INT		����ʾ
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
	// ��ѯ ������ֵ
	����1	INT		����ʾ
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
	// ��ѯ ���ɳ���
	����1	INT		����ʾ
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
	// ��ѯ ������
	����1	INT		����ʾ
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
	// ��ѯ �������
	����1	INT		����ʾ
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
	// ��ѯ �������
	����1	INT		����ʾ
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
	// ��ѯ �������
	����1	INT		����ʾ
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
	// ��ѯ �������
	����1	INT		����ʾ
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
	// ��ѯ �������
	����1	INT		����ʾ
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
	// ��ѯ ���Ǳ��
	����1	INT		����ʾ
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
	// ��ѯ �����������
	����1	INT		����ʾ
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
	// ��ѯ �����������
	����1	INT		����ʾ
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
	// ��ѯ �����������
	����1	INT		����ʾ
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
	// ��ѯ �����������
	����1	INT		����ʾ
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
	// ��ѯ ���UIģ������
	����1	INT		����ʾ
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
	// ��ѯ ����UIģ������  [7/14/2010 �¾���]
	����1	INT		�����ʾ
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
	// ��ѯ ��輼��
	����1	INT		����ʾ
	����2	INT		�����±�
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
			KLThrow("GetModelSkill(), nSkillIndex Խ�磡");
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
	// ��ѯ �������   [7/14/2010 �¾���]
	����1	INT		����ID
	����2	INT		Action ID
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
			KLThrow("GetMountItem(), nMountIndex Խ�磡");
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
	// �Ƿ������
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
	// ����
	------------------------------------------------ */
	INT Mount::Mounting(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::Mounting[2] param parameter error");
		}
		INT nMountIndex = args[2].GetInteger();

		// ��̯��
		if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
		{
			return 0;
		}
		
		const SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			//�����������
			CGManipulatePet Msg;

			Msg.SetGUID(pPet->m_GUID);
			Msg.SetManipulateType( MANIPULATE_CREATEHORSE );
			CNetManager::GetMe()->SendPacket(&Msg);
		}	

		return 0;
	}

	/* ------------------------------------------------
	// �ٻ�����  [7/14/2010 �¾���]
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
	// ����
	------------------------------------------------ */
	INT Mount::Dismount(LuaPlus::LuaState* state)
	{
		if (NULL != CObjectManager::GetMe()->GetMySelf()->GetCharacterData())
		{
			INT nMount = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID();
			if (nMount > 0)
			{
				//�����������
				CGManipulatePet Msg;

				Msg.SetGUID( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID() );
				Msg.SetManipulateType( MANIPULATE_DELETEHORSE );
				CNetManager::GetMe()->SendPacket(&Msg);
			}
		}
		return 0;
	}

	/* ------------------------------------------------
	// �ٻ�����  [7/14/2010 �¾���]
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
	// ����
	����1	GUID m_uHighSection
	����2	GUID m_uLowSection
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
			//�����������
			CGManipulatePet Msg;

			Msg.SetGUID(pPet->m_GUID);
			Msg.SetManipulateType( MANIPULATE_FREEHORSE );
			CNetManager::GetMe()->SendPacket(&Msg);
		}

		return 0;
	}

	/* ------------------------------------------------
	// ��������  [7/14/2010 �¾���]
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
	// Ŀ���������
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
	// Ŀ�������ǳ�
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
	// Ŀ����輶��
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
	// Ŀ������Ա�
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
	// Ŀ�����ID
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
	// Ŀ����żID
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
	// Ŀ������
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
	// Ŀ������
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
	// Ŀ������
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
	// Ŀ������
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
	// Ŀ����������
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
	// Ŀ����������
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
	// Ŀ����������
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
	// Ŀ����������
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
	// Ŀ�����
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
	// Ŀ������
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
	// Ŀ��ɳ���
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
	// Ŀ��UIģ����
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
	// �ӵ�ȷ��
	����1	INT		����±�
	����2	INT		����
	����3	INT		����
	����4	INT		����
	����5	INT		����
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

		// ���ͼӵ���Ϣ		
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
	// ����Ŀ���������
	------------------------------------------------ */
	INT Mount::TargetMountAttrRequest(LuaPlus::LuaState* state)
	{
		tObject* pObj = CObjectManager::GetMe()->GetMainTarget();

		if (pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_PlayerOther)))
		{
			CObject_PlayerOther* pOtherPlayer = (CObject_PlayerOther*)pObj;
			if (0 < pOtherPlayer->GetCharacterData()->Get_MountID())
			{
				// ���� ����Ŀ�����������Ϣ
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
	// Ŀ�꼼��
	����2	INT		�����±�
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
			KLThrow("GetTargetMountSkill(), nSkillIndex Խ�磡");
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
	// �õ� ��ǰ��ս������
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
	// �õ� ��ǰUIѡ������
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
	// ���� ��ǰUIѡ������
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
	// �����Ʒ����״̬
	����1	ActionItemID
	------------------------------------------------ */
	INT	Mount::ItemUnLock(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::ItemUnLock[2] param parameter error");
		}
		INT nActionItemID = args[2].GetInteger();

		// ��Ч
		tActionItem* pActionItem = CActionSystem::GetMe()->GetAction(nActionItemID);
		if (pActionItem && AOT_ITEM == pActionItem->GetType())
		{
			// ����Ʒ
			CActionItem_Item* pItem = (CActionItem_Item*)pActionItem;
			tObject_Item *pObj = pItem->GetItemImpl();
			if(pObj && pObj->IsLocked())
			{
				CObject_Item_Medicine* pMedicine_Item = (CObject_Item_Medicine*)pObj;

				// ��������Ʒ
				if (pMedicine_Item->GetItemTableType() == COMMON_HORSE_ITEM)
				{
					// ����
					pMedicine_Item->SetLock(FALSE);
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
			}			
		}
		return 0;
	}

	/* ------------------------------------------------
	// ʹ����Ʒ
	����1	ActionItemID
	����2	MountIndex
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

		// �����Ч
		SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (NULL == pPet)
			return 0;
		
		// ��Ʒ��Ч
		if (-1 == nActionItemID)
			return 0;
		tActionItem* pAction = CActionSystem::GetMe()->GetAction(nActionItemID);
		if (NULL == pAction)
			return 0;
		if (AOT_ITEM != pAction->GetType())
			return 0;

		CActionItem_Item* pActionItem = (CActionItem_Item*)pAction;
		
		// ��������Ʒ
		tObject_Item *pObj = pActionItem->GetItemImpl();
		if(pObj && pObj->GetItemClass() == ICLASS_COMITEM)
		{
			CObject_Item_Medicine* pMedicine_Item = (CObject_Item_Medicine*)pObj;
			if (pMedicine_Item->GetItemTableType() == COMMON_HORSE_ITEM)
			{
				// ������Ʒʹ����Ϣ
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
	// ����ı����������
	����1	MountIndex
	����2	MountName
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
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���ֲ���Ϊ��");
			return 0;
		}

		// ������󳤶�
		if (nNum >= MAX_NICK_NAME-1)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "������������");
			return 0;
		}

		SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(nMountIndex);
		if (pPet && FALSE == pPet->m_GUID.IsNull())
		{
			// �͵�ǰ����һ��
			if (0 != strcmp(szNickName.c_str(), pPet->m_szNickname.c_str()))
			{
				// ���͸�����Ϣ
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
	// �������ɳ���
	����1	MountIndex
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
				// ���ͼ�����Ϣ
				CGManipulatePet Msg;

				Msg.SetGUID( pPet->m_GUID );
				Msg.SetManipulateType( MANIPULATE_HORSE_CHECKUP );

				CNetManager::GetMe()->SendPacket(&Msg);
			}
		}

		return 0;
	}

	/* ------------------------------------------------
	// �Ƿ�ɷ�ֳ
	����1	MountIndex
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
	// ��ѯ ����״̬
	����1	MountIndex
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
	// ���ͽ��������������Ϣ
	����1	MountIndex
	����2	nLocked		���������
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

		// ���ͽ�����Ϣ
		if (0 == nLocked)
		{
			CGManipulatePet Msg;

			Msg.SetManipulateType( MANIPULATE_HORSE_MATING_UNLOCK );

			CNetManager::GetMe()->SendPacket(&Msg);
		}
		// ����
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
	// ���ʹ򿪷�ֳ������Ϣ
	����1	MountIndex
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
			// ���ʹ򿪽�����Ϣ
			CGManipulatePet Msg;

			Msg.SetCharObjID(nNpcSeverID);
			Msg.SetManipulateType( MANIPULATE_HORSE_OPEN_MATING_UI );

			CNetManager::GetMe()->SendPacket(&Msg);
		}	
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "Ŀ�����̫Զ");
		}

		return 0;
	}

	/* ------------------------------------------------
	// ����ȷ�ϻ�ȡ��������Ϣ
	����1	MountIndex
	����2	nOK			ȷ�ϻ�ȡ��
	����3	nNpcSeverID
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
			// ����ȷ����Ϣ
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
			// ����ȡ����Ϣ
			CGManipulatePet Msg;

			Msg.SetManipulateType( MANIPULATE_HORSE_MATING_CANCEL );

			CNetManager::GetMe()->SendPacket(&Msg);
		}		

		return 0;
	}

	/* ------------------------------------------------
	// ������ȡ��ֳ������Ϣ
	����1	nNpcSeverID
	------------------------------------------------ */
	INT Mount::RequestGainMatingMount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);		

		if (!(args[2].IsInteger()))
		{
			KLThrow("Mount::RequestGainMatingMount[2] param parameter error");
		}
		INT nNpcSeverID = args[2].GetInteger();

		//  npc��Ч
		tObject* pObj = CObjectManager::GetMe()->FindServerObject(nNpcSeverID);
		if (pObj)
		{
			/// ������ȡ��ֳ������Ϣ
			CGManipulatePet Msg;

			Msg.SetCharObjID(nNpcSeverID);
			Msg.SetManipulateType( MANIPULATE_HORSE_MATING_RETRIEVE );

			CNetManager::GetMe()->SendPacket(&Msg);
		}

		return 0;
	}


}










