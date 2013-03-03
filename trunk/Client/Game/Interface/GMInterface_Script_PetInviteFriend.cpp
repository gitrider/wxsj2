
#include "StdAfx.h"
#include "GIException.h"
#include "GITimeSystem.h"
#include "GIEventDefine.h"
#include "GIUISystem.h"
#include "LuaPlus.h"
#include "../../../Common/DB_Struct.h"
#include "../../../Common/GameStruct_Pet.h"
#include "../../../Common/GameStruct_Skill.h"
#include "DBC/GMDataBase.h"
#include "DataPool/GMDataPool.h"
#include "DataPool/GMDP_Struct_Pet.h"
#include "DataPool/GMDP_CharacterData.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Object/Logic/Character/Obj_PlayerNPC.h"
#include "FakeObject\GMFakeObjSystem.h"
#include "Event/GMEventSystem.h"
#include "Procedure/GameProcedure.h"
#include "GMInterface_Script_PetInviteFriend.h"




namespace SCRIPT_SANDBOX
{
	PetInviteFriend PetInviteFriend::s_PetInviteFriend;
	LuaPlus::LuaObject* PetInviteFriend::s_pMetaTable = NULL;

	PetInviteFriend::PetInviteFriend()
	{
		//InitPetList();
	}

	PetInviteFriend::~PetInviteFriend()
	{
		//DestoryPetList();
	}

	//LuaPlus�ӿ�
	//------------------------------------------------------------------------------------------------------------------------------
	INT PetInviteFriend::GetHumanINFO(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);

		//if(!(args[2].IsInteger()))
		//{
		//	KLThrow("LUA: PetInviteFriend GetHumanINFO Wrong Param1");
		//	return -1;
		//}
		//INT plidx = args[2].GetInteger() - 1;	//Lua is begin from 1, but c begin from 0

		//if(!(args[3].IsString()))
		//{
		//	KLThrow("LUA: PetInviteFriend GetHumanINFO Wrong Param2");
		//	return -1;
		//}
		//STRING op = args[3].GetString();

		//INT ret = 1;
		//if(GetPlaceCardItem(plidx))
		//{
		//	if(op == "GUID")
		//	{
		//		CHAR str[32] = {0};
		//		_snprintf(str,sizeof(str)-1,"%X",GetPlaceCardItem(plidx)->GetHumanGUID());
		//		state->PushString(str);

		//		state->PushInteger(GetPlaceCardItem(plidx)->GetHumanGUID());
		//		ret = 2;
		//	}
		//	else if(op == "NAME")
		//	{
		//		state->PushString(GetPlaceCardItem(plidx)->GetHumanName());
		//	}
		//	else if(op == "LEVEL")
		//	{
		//		state->PushInteger(GetPlaceCardItem(plidx)->GetHumanLevel());
		//	}
		//	else if(op == "PROFESSION")
		//	{
		//		state->PushInteger(GetPlaceCardItem(plidx)->GetHumanMenPai());
		//	}
		//	else if(op == "GUILD")
		//	{
		//		state->PushString(GetPlaceCardItem(plidx)->GetHumanGuildName());
		//	}
		//}

		//return ret;
		return 0;
	}

	INT PetInviteFriend::GetInviteMsg(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);

		//if(!(args[2].IsInteger()))
		//{
		//	KLThrow("LUA: PetInviteFriend GetInviteMsg Wrong Param1");
		//	return -1;
		//}
		//INT plidx = args[2].GetInteger() - 1;	//Lua is begin from 1, but c begin from 0

		//if(GetPlaceCardItem(plidx))
		//{
		//	state->PushString(GetPlaceCardItem(plidx)->GetMessage());
		//}

		return 1;
	}

	INT PetInviteFriend::GetInviteNum(LuaPlus::LuaState* state)
	{
		//state->PushInteger((INT)m_PetList.size());
		/*for(INT i = 0; i < (INT)m_PetList.size(); ++i)
		{
			if(INVALID_ID == GetPetListData(i)->m_nIsPresent)
			{
				state->PushInteger(i);
				return 1;
			}
		}
		state->PushInteger((INT)m_PetList.size());*/

		return 1;
	}

	INT PetInviteFriend::SetPetModel(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);

		//if(!(args[2].IsInteger()))
		//{
		//	KLThrow("LUA: PetInviteFriend SetPetModel Wrong Param1");
		//	return -1;
		//}
		//INT lidx = args[2].GetInteger() - 1;	//Lua is begin from 1, but c begin from 0

		//if(GetPetListData(lidx))
		//{
		//	if(INVALID_ID != GetPetListData(lidx)->m_nIsPresent)
		//	{
		//		SetPetListFakeObj(lidx, GetPetListData(lidx)->m_nDataID);
		//	}
		//}

		return 0;
	}

	INT PetInviteFriend::ShowTargetPet(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);

		//if(!(args[2].IsInteger()))
		//{
		//	KLThrow("LUA: PetInviteFriend ShowTargetPet Wrong Param1");
		//	return -1;
		//}
		//INT lidx = args[2].GetInteger() - 1;	//Lua is begin from 1, but c begin from 0

		//if(GetPetListData(lidx))
		//{
		//	if(INVALID_ID != GetPetListData(lidx)->m_nIsPresent)
		//	{
		//		CDataPool::GetMe()->Pet_CopyToTarget(GetPetListData(lidx));
		//		CEventSystem::GetMe()->PushEvent(GE_UPDATE_PETINVITEFRIEND, "target");	//֪ͨTargetPet��ʾ����
		//	}
		//}

		return 0;
	}

	INT PetInviteFriend::GetPetINFO(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);

		//if(!(args[2].IsInteger()))
		//{
		//	KLThrow("LUA: PetInviteFriend ShowTargetPet Wrong Param1");
		//	return -1;
		//}
		//INT lidx = args[2].GetInteger() - 1;	//Lua is begin from 1, but c begin from 0

		//if(!(args[3].IsString()))
		//{
		//	KLThrow("LUA: PetInviteFriend GetHumanINFO Wrong Param2");
		//	return -1;
		//}
		//STRING op = args[3].GetString();

		//if(GetPetListData(lidx))
		//{
  //          if(op == "NAME")
		//	{
		//		state->PushString(GetPetListData(lidx)->m_szName.c_str());
		//	}
		//}

		return 1;
	}

	//����ӿ�
	//------------------------------------------------------------------------------------------------------------------------------
	VOID PetInviteFriend::ConvertPlaceCard2PetFriend()
	{
		/*CleanUpPetList();
		for(UINT i = 0; i < m_PetList.size(); ++i)
		{
			if(GetPetListData(i) && GetPlaceCardItem(i))
			{
				SDATA_PET* pOut = GetPetListData(i);
				const _PET_DETAIL_ATTRIB* pIn = GetPlaceCardItem(i)->GetPetAttr();

				PET_DETAIL_ATTRIB2SDATA_PAT(pIn,pOut);
			}
		}*/
	}

	//���ýӿ�
	//------------------------------------------------------------------------------------------------------------------------------
	VOID PetInviteFriend::PET_DETAIL_ATTRIB2SDATA_PAT(const _PET_DETAIL_ATTRIB* pIn, SDATA_PET* pOut)
	{
//		if(NULL == pIn || NULL == pOut)
//			return;
//
//		//���������
//		pOut->CleanUp();
//		pOut->m_nIsPresent = 2;
//
//		//��������ת��
//		pOut->m_GUID			= pIn->m_GUID;				// ID
//		pOut->m_idServer		= pIn->m_ObjID;				// ����Obj���͵�ObjID
//		pOut->m_nDataID			= pIn->m_nDataID;			// Data ID,��������
//		pOut->m_szName			= pIn->m_szName;			// ����
//		pOut->m_nAIType			= pIn->m_nAIType;			// �Ը�
//		pOut->m_SpouseGUID		= pIn->m_SpouseGUID;		// ��ż��GUID
//		pOut->m_nLevel			= pIn->m_nLevel;			// �ȼ�
//		pOut->m_nExp			= pIn->m_nExp;				// ����
//		pOut->m_nHP				= pIn->m_nHP;				// Ѫ��ǰֵ
//		pOut->m_nHPMax			= pIn->m_nHPMax;			// Ѫ���ֵ
//
//		pOut->m_nAge			= pIn->m_nLife;				// ��ǰ����
//		pOut->m_nEraCount		= pIn->m_byGeneration;		// ������
//		pOut->m_nHappiness		= pIn->m_byHappiness;		// ���ֶ�
//
//		pOut->m_nAtt_Near		= pIn->m_nAtt_Near;
//		pOut->m_nDef_Near		= pIn->m_nDef_Near;
//		pOut->m_nAtt_Far		= pIn->m_nAtt_Far;
//		pOut->m_nDef_Far		= pIn->m_nDef_Far;
//
////		pOut->m_nAttPhysics		= pIn->m_nAtt_Physics;		// ��������
//		pOut->m_nAttMagic		= pIn->m_nAtt_Magic;		// ħ��������
////		pOut->m_nDefPhysics		= pIn->m_nDef_Physics;		// ���������
//		pOut->m_nDefMagic		= pIn->m_nDef_Magic;		// ħ��������
//
//		pOut->m_nHit			= pIn->m_nHit;				// ������
//		pOut->m_nMiss			= pIn->m_nMiss;				// ������
//		pOut->m_nCritical		= pIn->m_nCritical;			// ������
//
//		pOut->m_nModelID		= pIn->m_nModelID;			// ����
//		pOut->m_nMountID		= pIn->m_nMountID;			// ����ID
//
//		pOut->m_nAttrStrApt		= pIn->m_StrPerception;		// ��������
//		pOut->m_nAttrConApt		= pIn->m_ConPerception;		// ��������
//		pOut->m_nAttrDexApt		= pIn->m_DexPerception;		// ������
////		pOut->m_nAttrSprApt		= pIn->m_SprPerception;		// ��������
//		pOut->m_nAttrIntApt		= pIn->m_IntPerception;		// ��������
//
//		pOut->m_nAttrStr		= pIn->m_Str;				// ����
//		pOut->m_nAttrCon		= pIn->m_Con;				// ����
//		pOut->m_nAttrDex		= pIn->m_Dex;				// ��
////		pOut->m_nAttrSpr		= pIn->m_Spr;				// ����
//		pOut->m_nAttrInt		= pIn->m_Int;				// ����
//		pOut->m_nBasic			= pIn->m_GenGu;				// ����
//
//		pOut->m_nPot			= pIn->m_nRemainPoint;		// Ǳ�ܵ�
//
//		//����ת��
//		INT minSkill = (PET_MAX_SKILL_COUNT > MAX_PET_SKILL_COUNT)?MAX_PET_SKILL_COUNT:PET_MAX_SKILL_COUNT;
//		for(INT i=0; i< minSkill; ++i)
//		{
//			PET_SKILL* theSkill = pOut->m_aSkill[i];
//
//			DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
//			const _DBC_SKILL_DATA* pDefine = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(pIn->m_aSkill[i].m_nSkillID);
//
//			if(!theSkill)
//			{
//				PET_SKILL* newSkill = new PET_SKILL;
//
//				newSkill->m_pDefine = pDefine;
//				newSkill->m_nPetNum = MENPAI_PETSKILLSTUDY_PETNUM-(i+1);	//no need to set if only shown.
//				newSkill->m_nPosIndex = i;
//				newSkill->m_bCanUse = TRUE;
//
//				pOut->m_aSkill[i] = newSkill;
//			}
//			else
//			{
//				theSkill->m_pDefine = pDefine;
//				theSkill->m_nPetNum = MENPAI_PETSKILLSTUDY_PETNUM-(i+1);	//no need to set if only shown.
//				theSkill->m_bCanUse = TRUE;
//				theSkill->m_nPosIndex = i;
//			}
//		}
	}

	VOID PetInviteFriend::InitPetList()
	{
		/*static BOOL bFirst = FALSE;

		if(bFirst)
			return;

		m_PetList.resize(0);

		for(INT i=0; i < MAX_PETPLACARD_LIST_ITEM_NUM; ++i)
		{
			PET_FRIEND *pf = new PET_FRIEND;
			pf->m_Pet.CleanUp();

			CHAR szFakeName[32] = {0};
			_snprintf(szFakeName,32-1,"My_PetInviteFriend%02d", i+1);
			pf->m_Avatar = (CObject_PlayerNPC*)(CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerNPC", szFakeName, "Camera_Main"));
			
			m_PetList.push_back(pf);
		}

		bFirst = TRUE;*/
	}

	VOID PetInviteFriend::CleanUpPetList()
	{
		//for(UINT i=0; i < m_PetList.size(); ++i)
		//{
		//	//m_PetList.at(i).m_Avatar �ɽ���ÿ����ʾʱ���ԭ��������
		//	m_PetList.at(i)->m_Pet.CleanUp();
		//}
	}

	VOID PetInviteFriend::DestoryPetList()
	{
		/*for(UINT i=0; i < m_PetList.size(); ++i)
		{
			SAFE_DELETE( m_PetList[i]);
		}

		m_PetList.resize(0);*/
	}

	VOID PetInviteFriend::SetPetListFakeObj(INT idx, INT rdx)
	{
		/*if(idx >= (INT)m_PetList.size() || idx < 0)
			return;

		m_PetList.at(idx)->m_Avatar->GetCharacterData()->Set_RaceID(rdx);
		m_PetList.at(idx)->m_Avatar->SetFaceDir(0.0f);*/
	}

	SDATA_PET* PetInviteFriend::GetPetListData(INT idx)
	{
		/*if(idx >= (INT)m_PetList.size() || idx < 0)
			return NULL;

		return &(m_PetList.at(idx)->m_Pet);*/
		return NULL;
	}

	const _PET_PLACARD_ITEM* PetInviteFriend::GetPlaceCardItem(INT idx)
	{
		return CDataPool::GetMe()->PetPlacard_GetItem(idx);
	}
}