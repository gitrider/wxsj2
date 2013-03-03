
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"
#include "Procedure/GamePro_Login.h"
#include "NetWork/NetManager.h"
#include "LuaPlus.h"
#include "Global.h"
#include "Event/GMEventSystem.h"
#include "GIException.h"
#include "Action/GMActionSystem.h"
#include "DataPool/GMDataPool.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Input/GMInputSystem.h"
#include "world/worldmanager.h"
#include "GIDbc_struct.h"
#include "object/Logic/character/obj_playermyself.h"
#include "GIUtil.h"
#include "ScriptCommandStruct.h"
#include "CGEventRequest.h"
#include "..\..\Common\database\WXSJ_DBC.h"
#include "..\..\Common\BuffImpactMgr.h"
#include "GIDBC_Struct.h"
#include "DBC\GMDataBase.h"
#include "GIException.h"
#include "CGSetPetAttrib.h"
#include "CGManipulatePet.h"



struct SDATA_PET;

namespace SCRIPT_SANDBOX
{
	

	//===============================================================
	TargetPet TargetPet::s_TargetPet;
	LuaPlus::LuaObject* TargetPet::s_pMetaTable = NULL;
	////�õ������Ƿ����
	INT TargetPet::IsPresent(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) )
		{
			KLThrow("LUA:Pet IsPresent parameter error");
			return 0;
		}

		if(this == &s_TargetPet)
		{
			if(0 < My_Pet->m_nIsPresent )
			{
				state->PushBoolean(true);
			}
			else
			{
				state->PushBoolean(FALSE);
			}
		}
		else
			KLThrow("Not a valid obj");*/

		return 1;
	}
/*	//�õ����������
	INT TargetPet::GetPet_Count(LuaPlus::LuaState* state)
	{
		state->PushNumber( CDataPool::GetMe()->Pet_GetCount());
		return 1;
	}
	//�õ��ڼ�ֻ���������
	INT	TargetPet::GetPetList_Appoint(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		SDATA_PET* My_Pet;
		INT count =0;
		for(INT i = 0; i < HUMAN_PET_MAX_COUNT; i++)
		{

			My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
			if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) ) 
				continue;
			
			if(count == nPetNum)
			{
				state->PushString( My_Pet->m_szName.c_str());
				return 1;
			}
			count++;
		}
		return 0;
	}
	*/
	// �õ����������
	INT	TargetPet::GetName(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetName parameter error");
			return 0;
		}
		state->PushString( My_Pet->m_szName.c_str());

		DBC_DEFINEHANDLE(s_pMonsterAttr, DBC_CREATURE_ATT);
		const _DBC_CREATURE_ATT* pPet = (const _DBC_CREATURE_ATT*)s_pMonsterAttr->Search_Index_EQU(My_Pet->m_nDataID);

		if(pPet)
		{
			state->PushString(pPet->pName);
		}
		else
		{
			state->PushString("δ֪��");
		}*/
		return 2;
	}

	INT	TargetPet::GetConsort(LuaPlus::LuaState* state)
	{
//		LuaStack args(state);
//		
//		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
//		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
//		{
//			KLThrow("LUA:Pet GetConsort parameter error");
//			return 0;
//		}
//
//		CHAR str[32];
//		sprintf(str,"%X",My_Pet->m_SpouseGUID.m_uLowSection);
//
//		state->PushString(str);
////		state->PushNumber(My_Pet->m_SpouseGUID.m_uHighSection);
////		state->PushNumber(My_Pet->m_SpouseGUID.m_uLowSection);

		return 1;
	}

	INT	TargetPet::GetSex(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);
		//	
		//SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		//if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		//{
		//	KLThrow("LUA:Pet GetSex parameter error");
		//	return 0;
		//}
		////state->PushString( My_Pet->m_szSex.c_str());
		//state->PushString( "����ûȥ��");
		return 1;
	}

	INT	TargetPet::GetID(LuaPlus::LuaState* state)
	{
//		LuaStack args(state);
//		
//		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
//		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
//		{
//			KLThrow("LUA:Pet GetID parameter error");
//			return 0;
//		}
//		CHAR str[32];
//		sprintf(str,"%X",My_Pet->m_GUID.m_uHighSection);
//
//		state->PushString(str);
////		state->PushNumber(My_Pet->m_GUID.m_uHighSection);
//
//		sprintf(str,"%X",My_Pet->m_GUID.m_uLowSection);
//		state->PushString(str);
////		state->PushNumber(My_Pet->m_GUID.m_uLowSection);
//
//		INT sex = My_Pet->m_GUID.m_uLowSection % 2;
//		state->PushInteger(sex);		

		return 3;
	}

	INT	TargetPet::GetLevel(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetLevel parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nLevel);*/
		return 1;
	}

	INT	TargetPet::GetHappy(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetHappy parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nHappiness);*/
		return 1;
	}

	INT	TargetPet::GetType(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetType parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nEraCount);*/
		return 1;
	}

	INT	TargetPet::GetNaturalLife(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
			
		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetNaturalLife parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAge);*/
		return 1;
	}

	INT	TargetPet::GetMaxLife(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		KLAssert(false&&"Pet::GetMaxLife û�г������������ĸ���");
		state->PushNumber( 100 );*/
		return 1;
	}

	INT	TargetPet::GetLoyalgGade(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
			
		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetLoyalgGade parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nHappiness);*/
		return 1;
	}

	INT	TargetPet::GetHP(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetHP parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nHP);*/
		return 1;
	}

	INT	TargetPet::GetMP(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetMP parameter error");
			return 0;
		}
		state->PushNumber( 0);*/
		return 1;
	}

	INT	TargetPet::GetMaxHP(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
			
		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetMaxHP parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nHPMax);*/
		return 1;
	}

	INT	TargetPet::GetMaxMP(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetMaxMP parameter error");
			return 0;
		}
		state->PushNumber( 1);*/
		return 1;
	}

	INT	TargetPet::GetStrAptitude(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetStrAptitude parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrStrApt);*/
		return 1;
	}

	INT	TargetPet::GetPFAptitude(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetPFAptitude parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrConApt);*/
		return 1;
	}

	INT	TargetPet::GetDexAptitude(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetDexAptitude parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrDexApt);*/
		return 1;
	}

/*
	INT	TargetPet::GetIntAptitude(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetIntAptitude parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrIntApt);
		return 1;
	}
*/

/*
	INT	TargetPet::GetStaAptitude(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetStaAptitude parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrSprApt);
		return 1;
	}
*/

	INT	TargetPet::GetStr(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetStr parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrStr);*/
		return 1;
	}


	INT	TargetPet::GetInt(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetInt parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrCon);*/
		return 1;
	}

	INT	TargetPet::GetDex(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetDex parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrDex);*/
		return 1;
	}
/*
	INT	TargetPet::GetPF(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetPF parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrSpr);
		return 1;
	}
*/

	INT	TargetPet::GetSta(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetSta parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrInt);*/
		return 1;
	}

	INT	TargetPet::GetBasic(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetBasic parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nBasic);*/
		return 1;
	}

	INT	TargetPet::GetPotential(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetPotential parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nPot);*/
		return 1;
	}
	INT	TargetPet::GetExp(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetExp parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nExp);*/
		return 1;
	}


	INT	TargetPet::GetNearAttack(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetNearAttack parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAtt_Near);*/
		return 1;
	}
	INT	TargetPet::GetNearRecovery(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetNearRecovery parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nDef_Near);*/
		return 1;
	}
	INT	TargetPet::GetFarAttack(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetFarAttack parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAtt_Far);*/
		return 1;
	}
	INT	TargetPet::GetFarRecovery(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetFarRecovery parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nDef_Far);*/
		return 1;
	}

/*
	INT	TargetPet::GetPhysicsAttack(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetPhysicsAttack parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttPhysics);
		return 1;
	}
*/

	INT	TargetPet::GetMagicAttack(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetMagicAttack parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttMagic);*/
		return 1;
	}

/*
	INT	TargetPet::GetPhysicsRecovery(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetPhysicsRecovery parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nDefPhysics);
		return 1;
	}
*/

	INT	TargetPet::GetMagicRecovery(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetMagicRecovery parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nDefMagic);*/
		return 1;
	}

	INT	TargetPet::GetShootProbability(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetShootProbability parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nHit);*/
		return 1;
	}

	INT	TargetPet::GetMiss(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetMiss parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nMiss);*/
		return 1;
	}

	INT	TargetPet::SetModel(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet SetModel parameter error");
			return 0;
		}
		CDataPool::GetMe()->TargetPet_SetPetModel(My_Pet->m_nDataID);*/
		return 0;
	}

	INT	TargetPet::CopyMyPet(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		if( !args[2].IsInteger() )
		{
			KLThrow("LUA:Pet CopyMyPet parameter error");
			return 0;			
		}

		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(args[2].GetInteger());
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet CopyMyPet parameter error");
			return 0;
		}
		CDataPool::GetMe()->Pet_CopyToTarget(My_Pet);*/
		return 0;
	}
/*
	// ��������Ե㡣
	INT TargetPet::Add_Attribute(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ||
			!(args[3].IsInteger()) ||
			!(args[4].IsInteger()) ||
			!(args[5].IsInteger()) ||
			!(args[6].IsInteger()) ||
			!(args[7].IsInteger()) )
		{
			KLThrow("LUA:Pet Add_Attribute parameter error");
		}

		INT nPetNum = args[2].GetInteger();
		INT nStr = args[3].GetInteger();
		INT	nInt = args[4].GetInteger();
		INT	nDex = args[5].GetInteger();
		INT	nPF	 = args[6].GetInteger();
		INT	nSta = args[7].GetInteger();

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet Add_Attribute parameter error");
			return 0;
		}
		if( (nStr + nInt + nDex + nPF +nSta == 0) || My_Pet->m_nPot == 0)
			return 0;
		if(nStr + nInt + nDex + nPF +nSta > My_Pet->m_nPot)
			return 0;
		//�����������

		CGSetPetAttrib Msg;
//		Msg.SetIndex(nPetNum);
		Msg.SetGUID(My_Pet->m_GUID);
		if(nStr > 0) Msg.SetStrIncrement(nStr);
		if(nInt > 0) Msg.SetIntIncrement(nInt);
		if(nDex > 0) Msg.SetDexIncrement(nDex);
		if(nPF  > 0) Msg.SetConIncrement(nPF);
		if(nSta > 0) Msg.SetSprIncrement(nSta);

		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}

	//���������
	INT	TargetPet::Change_Name(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !(args[2].IsInteger()) || !(args[3].IsString()) )
		{
			KLThrow("LUA:Pet Change_Name parameter error");
		}

		INT nPetNum = args[2].GetInteger();
		LPCTSTR szName = args[3].GetString();

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet Change_Name parameter error");
			return 0;
		}
		//�����������
		CGSetPetAttrib Msg;
		if(strcmp(szName, My_Pet->m_szName.c_str()) != 0)
		{
			Msg.SetGUID(My_Pet->m_GUID);
//			Msg.SetIndex(nPetNum);		
			Msg.SetName((CHAR*)szName);
			CNetManager::GetMe()->SendPacket(&Msg);
		}
		return 0;
	}

	INT	TargetPet::Go_Fight(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			KLThrow("LUA:Pet Go_Fight parameter error");
		}

		INT nPetNum = args[2].GetInteger();

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet Go_Fight parameter error");
			return 0;
		}
		//�����������
		CGManipulatePet Msg;

		Msg.SetGUID(My_Pet->m_GUID);
		Msg.SetManipulateType( MANIPULATE_CREATEPET );
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}

	INT	TargetPet::Go_Relax(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			KLThrow("LUA:Pet Go_Relax parameter error");
		}

		INT nPetNum = args[2].GetInteger();

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet Go_Relax parameter error");
			return 0;
		}
		//�����������
		CGManipulatePet Msg;

		Msg.SetGUID(My_Pet->m_GUID);
		Msg.SetManipulateType(MANIPULATE_DELETEPET);
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}
*/
	INT	TargetPet::GetIsFighting(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetIsFighting parameter error");
			return 0;
		}
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID() == My_Pet->m_GUID )
		{
			state->PushBoolean(true);
		}
		else
		{
			state->PushBoolean(false);
		}*/

		return 1;
	}
	INT TargetPet::GetDeathWarrant(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetShootProbability parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nCritical );*/
		return 1;
	}

	INT	TargetPet::GetAIType(LuaPlus::LuaState* state)
	{

		/*SDATA_PET* My_Pet= CDataPool::GetMe()->TargetPet_GetPet();
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:TargetPet GetAIType parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAIType);*/
		return 1;
	}

}