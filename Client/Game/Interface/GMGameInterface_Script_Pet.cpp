
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
#include "CGAskDetailAttrib.h"

#include "CGUseItem.h"
#include "CGIssuePetPlacard.h"
#include "GMInterface_Script_Talk.h"



struct SDATA_PET;

namespace SCRIPT_SANDBOX
{
	

	//===============================================================
	Pet Pet::s_Pet;
	LuaPlus::LuaObject* Pet::s_pMetaTable = NULL;
	////得到宠物是否存在
	INT Pet::IsPresent(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet IsPresent parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) )
		{
			KLThrow("LUA:Pet IsPresent parameter error");
			return 0;
		}

		if(this == &s_Pet)
		{
			if( (!My_Pet->m_GUID.IsNull()) && (My_Pet->m_pModelData ))
			{
				state->PushInteger(1);
			}
			else
			{
				state->PushInteger(0);
			}
		}
		else
			KLThrow("Not a valid obj");

		return 1;
	}

	//得到宠物的数量
	INT Pet::GetPet_Count(LuaPlus::LuaState* state)
	{
		state->PushNumber( CDataPool::GetMe()->Pet_GetCount());
		return 1;
	}
	//得到第几只宠物的名字
	INT	Pet::GetPetList_Appoint(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetPetList_Appoint parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();
		SDATA_PET* My_Pet;
		INT count =0;
		for(INT i = 0; i < HUMAN_PET_MAX_COUNT; i++)
		{

			My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
			if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
				continue;
			
			if(count == nPetNum)
			{
				state->PushString( My_Pet->m_szName.c_str());

				if(CDataPool::GetMe()->MyStallBox_IsPetOnStall(My_Pet->m_GUID))
				{
					state->PushInteger(1);
				}
				else
				{
					state->PushInteger(0);
				}
				return 2;
			}
			count++;
		}
		return 0;
	}
	// 得到宠物的名字
	INT	Pet::GetName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetName parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetName parameter error");
			return 0;
		}
		state->PushString( My_Pet->m_szName.c_str());

		return 1;

		//DBC_DEFINEHANDLE(s_pMonsterAttr, DBC_CREATURE_ATT);
		//const _DBC_CREATURE_ATT* pPet = (const _DBC_CREATURE_ATT*)s_pMonsterAttr->Search_Index_EQU(My_Pet->m_nDataID);
		//if(pPet)
		//{
		//	state->PushString(pPet->pName);
		//}
		//else
		//{
		//	state->PushString("未知宠");
		//}
		//return 2;
	}

	//获得宠物的配偶的ID
	INT	Pet::GetConsort(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetConsort parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetConsort parameter error");
			return 0;
		}

		CHAR str[32];
		_snprintf(str, 32, "%.8X",My_Pet->m_SpouseGUID.m_uLowSection);

		state->PushString(str);
		state->PushNumber(My_Pet->m_SpouseGUID.m_uHighSection);
		state->PushNumber(My_Pet->m_SpouseGUID.m_uLowSection);

		return 3;
	}

	//得到宠物的性别
	INT	Pet::GetSex(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetSex parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetSex parameter error");
			return 0;
		}
		state->PushInteger( My_Pet->m_nGender );
		//state->PushString( My_Pet->m_szSex.c_str());
		//state->PushString( "函数没去掉");
		return 1;
	}

	//获得宠物的ID
	INT	Pet::GetID(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetID parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetID parameter error");
			return 0;
		}
		CHAR str[32];
		_snprintf(str, 32, "%.8X",My_Pet->m_GUID.m_uHighSection);

		state->PushString(str);
//		state->PushNumber(My_Pet->m_GUID.m_uHighSection);

		_snprintf(str, 32, "%.8X",My_Pet->m_GUID.m_uLowSection);
		state->PushString(str);
//		state->PushNumber(My_Pet->m_GUID.m_uLowSection);

		INT sex = My_Pet->m_GUID.m_uLowSection % 2;
		state->PushInteger(sex);		

		return 3;
	}

	//得到宠物的等级
	INT	Pet::GetLevel(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetLevel parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetLevel parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nLevel);
		return 1;
	}

	//获得宠物的性格
	INT	Pet::GetAIType(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetAIType parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetAIType parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAIType);
		return 1;
	}

	//得到宠物的快乐度
	INT	Pet::GetHappy(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetHappy parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetHappy parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nHappiness);
		return 1;
	}

	//获得宠物的潜能
	INT	Pet::GetPotential(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetType parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		

		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetPotential parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nPotential);

		return 1;
	}

	//得到宠物是第几代宠
	INT	Pet::GetType(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetType parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetType parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nEraCount);
		return 1;
	}

	//获得宠物的寿命
	INT	Pet::GetNaturalLife(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetNaturalLife parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetNaturalLife parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nLife);
		return 1;
	}

	INT	Pet::GetMaxLife(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		KLAssert(false&&"Pet::GetMaxLife 没有宠物的最大生命的概念");
		state->PushNumber( 100 );
		return 1;
	}

	INT	Pet::GetLoyalgGade(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		/*if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetLoyalgGade parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetLoyalgGade parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nHappiness);*/
		return 1;
	}

	//获得宠物当前的血量
	INT	Pet::GetHP(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetHP parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetHP parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nHP);
		return 1;
	}

	INT	Pet::GetMP(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		/*if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetMP parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetMP parameter error");
			return 0;
		}
		state->PushNumber( 0);*/
		return 1;
	}

	INT	Pet::GetMaxHP(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetMaxHP parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetMaxHP parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nHPMax);
		return 1;
	}

	INT	Pet::GetMaxMP(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		/*if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetMaxMP parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetMaxMP parameter error");
			return 0;
		}
		state->PushNumber( 1);*/
		return 1;
	}

	//得到宠物的力量资质
	INT	Pet::GetStrAptitude(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetStrAptitude parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetStrAptitude parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrStrApt);
		return 1;
	}

	//得到宠物的体力资质
	INT	Pet::GetPFAptitude(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetPFAptitude parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetPFAptitude parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrConApt);
		return 1;
	}

	//得到宠物的身法资质
	INT	Pet::GetDexAptitude(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetDexAptitude parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetDexAptitude parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrDexApt);
		return 1;
	}
/*
	INT	Pet::GetIntAptitude(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetIntAptitude parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetIntAptitude parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrSprApt);
		return 1;
	}
*/

	//得到宠物的定力资质
	INT	Pet::GetStaAptitude(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetStaAptitude parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetStaAptitude parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrIntApt);
		return 1;
	}


	//得到宠物的力量
	INT	Pet::GetStr(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetStr parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetStr parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrStr);
		return 1;
	}
/*
	INT	Pet::GetInt(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetInt parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetInt parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrSpr);
		return 1;
	}
*/

	//得到宠物的身法
	INT	Pet::GetDex(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetDex parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetDex parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrDex);
		return 1;
	}

	//获得宠物的体力
	INT	Pet::GetPF(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetPF parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetPF parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrCon);
		return 1;
	}

	//获得宠物的定力
	INT	Pet::GetSta(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetSta parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetSta parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttrInt);
		return 1;
	}

	//获得宠物的根骨
	INT	Pet::GetBasic(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetBasic parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetBasic parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nBasic);
		return 1;
	}

	/*INT	Pet::GetPotential(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetPotential parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetPotential parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nPot);
		return 1;
	}*/

	//获得宠物当前的经验 和 升级所需要的经验
	INT	Pet::GetExp(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetExp parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetExp parameter error");
			return 0;
		}

		DBC_DEFINEHANDLE(s_pPetLevelup, DBC_PET_LEVELUP);
		const _DBC_PET_LEVELUP* pPetExp = (const _DBC_PET_LEVELUP*)s_pPetLevelup->Search_Index_EQU(My_Pet->m_nLevel);

		state->PushNumber( My_Pet->m_nExp);

		if(pPetExp)
		{
			state->PushInteger(pPetExp->nExp);
		}
		else
		{
			state->PushInteger(INVALID_ID);
		}
		return 2;
	}


	INT	Pet::GetNearAttack(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetNearAttack parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetNearAttack parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAtt_Near);*/
		return 1;
	}
	INT	Pet::GetNearRecovery(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetNearRecovery parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetNearRecovery parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nDef_Near);*/
		return 1;
	}
	INT	Pet::GetFarAttack(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetFarAttack parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetFarAttack parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAtt_Far);*/
		return 1;
	}
	INT	Pet::GetFarRecovery(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetPFarRecovery parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetFarRecovery parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nDef_Far);*/
		return 1;
	}

/*
	INT	Pet::GetPhysicsAttack(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetPhysicsAttack parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetPhysicsAttack parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttPhysics);
		return 1;
	}
*/

	INT	Pet::GetMagicAttack(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetMagicAttack parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetMagicAttack parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nAttMagic);*/
		return 1;
	}

/*
	INT	Pet::GetPhysicsRecovery(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetPhysicsRecovery parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetPhysicsRecovery parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nDefPhysics);
		return 1;
	}
*/

	INT	Pet::GetMagicRecovery(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetMagicRecovery parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetMagicRecovery parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nDefMagic);*/
		return 1;
	}

	//获得宠物的命中率
	INT	Pet::GetShootProbability(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetShootProbability parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetShootProbability parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nHit);
		return 1;
	}

	//获得宠物的闪避率
	INT	Pet::GetMiss(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetMiss parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetMiss parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nMiss);
		return 1;
	}

	INT	Pet::SetModel(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet SetModel parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet SetModel parameter error");
			return 0;
		}
		CDataPool::GetMe()->Pet_SetPetModel(nPetNum);*/
		return 0;
	}

	// 宠物加属性点。
	INT Pet::Add_Attribute(LuaPlus::LuaState* state)
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
		INT nStr = args[CATTR_LEVEL1_STR+3].GetInteger();
//		INT	nInt = args[CATTR_LEVEL1_SPR+3].GetInteger();
		INT nInt = 0;
		INT	nDex = args[CATTR_LEVEL1_CON+3].GetInteger();
		INT	nPF = args[CATTR_LEVEL1_INT+3].GetInteger();
		INT	nSta = args[CATTR_LEVEL1_DEX+3].GetInteger();

		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet Add_Attribute parameter error");
			return 0;
		}
		if( (nStr + nInt + nDex + nPF +nSta == 0) || My_Pet->m_nPotential == 0)
			return 0;
		if(nStr + nInt + nDex + nPF +nSta > My_Pet->m_nPotential )
			return 0;
		//向服务器发送

		CGSetPetAttrib Msg;
//		Msg.SetIndex(nPetNum);
		Msg.SetGUID(My_Pet->m_GUID);
		if(nStr > 0) Msg.SetStrIncrement(nStr);
		if(nSta > 0) Msg.SetIntIncrement(nSta);
		if(nDex > 0) Msg.SetDexIncrement(nDex);
		if(nPF  > 0) Msg.SetConIncrement(nPF);
//		if(nInt > 0) Msg.SetSprIncrement(nInt);

		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}

	//宠物改名字
	INT	Pet::Change_Name(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !(args[2].IsInteger()) || !(args[3].IsString()) )
		{
			KLThrow("LUA:Pet Change_Name parameter error");
		}

		INT nPetNum = args[2].GetInteger();
		LPCTSTR szName = args[3].GetString();
		STRING szTemp="";

		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet Change_Name parameter error");
			return 0;
		}
		if(CGameProcedure::s_pUISystem->CheckStringFilter(szName, FT_NAME) == FALSE)
		{
			
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Talk_Info_InvalidContent");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			return 0;

		}
		else if(CGameProcedure::s_pUISystem->CheckStringCode(szName,szTemp) == FALSE)
		{

			return 0;

		}
		else if(KLU_CheckStringValid(szName) == FALSE)
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			return 0;
		}
		//向服务器发送
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

	//宠物参战
	INT	Pet::Go_Fight(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			KLThrow("LUA:Pet Go_Fight parameter error");
		}

		INT nPetNum = args[2].GetInteger();

		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet Go_Fight parameter error");
			return 0;
		}
		// 如果自己正在摆摊，不能出战
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_You_Stalling");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
			return 0;
		}
		//向服务器发送
		CGManipulatePet Msg;

		Msg.SetGUID(My_Pet->m_GUID);
		Msg.SetManipulateType( MANIPULATE_CREATEPET );
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}

	//宠物放生确认
	INT	Pet::Free_Confirm(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			KLThrow("LUA:Pet Free_Confirm parameter error");
		}

		INT nPetNum = args[2].GetInteger();

		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet Free_Confirm parameter error");
			return 0;
		}

		CEventSystem::GetMe()->PushEvent(GE_PET_FREE_CONFIRM,nPetNum);

		return 0;
	}

	//宠物放生
	INT	Pet::Go_Free(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			KLThrow("LUA:Pet Go_Free parameter error");
		}

		INT nPetNum = args[2].GetInteger();

		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet Go_Free parameter error");
			return 0;
		}
		if( CGameProcedure::s_pUISystem->IsWindowShow("ProgressBar") )
		{
			STRING strMsg = COLORMSGFUNC("GMGameInterface_Script_Pet_CantFree");
			//ADDTALKMSG(strMsg);
			ADDNEWDEBUGMSG(strMsg);
			return 0;
			
		}
		// 如果自己正在摆摊，不能放生
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_You_Stalling");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
			return 0;
		}

		//向服务器发送
		CGManipulatePet Msg;

		Msg.SetGUID(My_Pet->m_GUID);
		Msg.SetManipulateType( MANIPULATE_FREEPET );
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}

	INT Pet::Feed(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			KLThrow("LUA:Pet Feed parameter error");
		}

		INT nPetNum = args[2].GetInteger();

		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet Feed parameter error");
			return 0;
		}

		INT idxPackage = -1;
		CObject_Item* pFind = (CObject_Item*)CDataPool::GetMe()->Pet_GetLowestLevel_Food_From_Package(My_Pet->m_GUID,&idxPackage);
		if(!pFind || idxPackage < 0) 
		{
			STRING strMsg = COLORMSGFUNC("GMGameInterface_Script_Pet_NoFeedFood");
			//ADDTALKMSG(strMsg);
			ADDNEWDEBUGMSG(strMsg);
			return 0;
		}

		Packets::CGUseItem pi;
		pi.SetBagIndex(idxPackage);
		pi.SetTargetPetGUID(My_Pet->m_GUID);

		CNetManager::GetMe()->SendPacket(&pi);

		return 0;
	}

	INT Pet::Dome(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);

		//if ( !args[2].IsInteger() )
		//{
		//	KLThrow("LUA:Pet Dome parameter error");
		//}

		//INT nPetNum = args[2].GetInteger();

		//SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		//if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		//{
		//	KLThrow("LUA:Pet Dome parameter error");
		//	return 0;
		//}

		//INT idxPackage = -1;
		//CObject_Item* pFind = (CObject_Item*)CDataPool::GetMe()->Pet_GetLowestLevel_Dome_From_Package(My_Pet->m_GUID,&idxPackage);

		//if(My_Pet->m_nHappiness == 100)
		//{
		//	STRING strMsg = COLORMSGFUNC("GMGameInterface_Script_Pet_NoNeedDemo");
		//	//ADDTALKMSG(strMsg);
		//	ADDNEWDEBUGMSG(strMsg);
		//	return 0;
		//}

		//if(!pFind || idxPackage < 0)
		//{
		//	STRING strMsg = COLORMSGFUNC("GMGameInterface_Script_Pet_NoDemoFood");
		//	//ADDTALKMSG(strMsg);
		//	ADDNEWDEBUGMSG(strMsg);
		//	return 0;
		//}

		//Packets::CGUseItem pi;
		//pi.SetBagIndex(idxPackage);
		//pi.SetTargetPetGUID(My_Pet->m_GUID);

		//CNetManager::GetMe()->SendPacket(&pi);

		return 0;
	}

	//宠物休息
	INT	Pet::Go_Relax(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( !args[2].IsInteger() )
		{
			KLThrow("LUA:Pet Go_Relax parameter error");
		}

		INT nPetNum = args[2].GetInteger();

		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet Go_Relax parameter error");
			return 0;
		}
		//向服务器发送
		CGManipulatePet Msg;

		Msg.SetGUID(My_Pet->m_GUID);
		Msg.SetManipulateType(MANIPULATE_DELETEPET);
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}

	//宠物是否处于参战状态
	INT	Pet::GetIsFighting(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetIsFighting parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetIsFighting parameter error");
			return 0;
		}
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID() == My_Pet->m_GUID )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		return 1;
	}

	//获得宠物的会心率
	INT Pet::GetDeathWarrant(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetDeathWarrant parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetBasic parameter error");
			return 0;
		}
		state->PushNumber( My_Pet->m_nCritical);
		return 1;
	}

	//获得宠物的第N个技能的ID
	INT Pet::GetSkillID(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetSkillID parameter1 error");
			return 0;
		}
		if (!(args[3].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetSkillID parameter2 error");
			return 0;
		}

		INT nPetNum = args[2].GetInteger();		
		INT nSkillNum = args[3].GetInteger();

		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet GetSkillID parameter error");
			return 0;
		}

		state->PushInteger(My_Pet->m_Skill[nSkillNum].m_pDefine->m_nID );

		return 1;
	}


	INT Pet::Select_Pet(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetDeathWarrant parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet Select_Pet parameter error");
			return 0;
		}
		CEventSystem::GetMe()->PushEvent(GE_REPLY_MISSION_PET, nPetNum);*/
		return 1;
		
	}
	//得到操作对象,得到宠物的技能ID
	INT	Pet::Lua_EnumPetSkill(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: EnumPetSkill Wrong Param1");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA: EnumPetSkill Wrong Param2");
		}
		if (!args[4].IsString())
		{
			KLThrow("LUA: EnumPetSkill Wrong Param3");
		}


		INT nPetIndex = args[2].GetInteger();
		INT nPetSkill = args[3].GetInteger();
		LPCSTR szFilter = args[4].GetString();

		tActionItem* pActionItem = CActionSystem::GetMe()->EnumPetSkill(nPetIndex, nPetSkill, szFilter);

		if(pActionItem) 
		{
			LuaObject objReturn = state->BoxPointer(pActionItem);
			objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
			objReturn.PushStack();
		}
		else
		{
			LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
			objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
			objReturn.PushStack();
		}

		return 1;
	}



	//for petskillstudy
	//设置宠物技能学习的模型
	INT	Pet::SetSkillStudyModel(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet SetSkillStudyModel parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet SetSkillStudyModel parameter error");
			return 0;
		}
		CDataPool::GetMe()->PetStudySkill_SetPetModel(My_Pet->m_pModelData->m_nModelID);
		return 0;
	}

	//显示目标宠物
	INT Pet::ShowTargetPet(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet ShowTargetPet parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();	

		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || ( !My_Pet->m_pModelData ) ) 
		{
			KLThrow("LUA:Pet ShowTargetPet parameter error");
			return 0;
		}

		CEventSystem::GetMe()->PushEvent(GE_UPDATE_TARGETPET_PAGE, nPetNum);
		return 0;
	}

	INT Pet::SkillStudy_Do(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);

		//if(!(args[2].IsInteger()))
		//{
		//	KLThrow("LUA:Pet SkillStudy_Do parameter1 error");
		//	return 0;
		//}
		//INT uitype = args[2].GetInteger();

		//if(!(args[3].IsInteger()))
		//{
		//	KLThrow("LUA:Pet SkillStudy_Do parameter2 error");
		//	return 0;
		//}
		//INT lid = args[3].GetInteger();	//选中的宠物

		//INT ret = 0;
		//switch(uitype)
		//{
		//case 1:	//普通技能学习
		//case 2:	//还童
		//case 3: //延长寿命
		//	{
		//		if(!(args[4].IsInteger()))
		//		{
		//			KLThrow("LUA:Pet SkillStudy_Do parameter3 error");
		//			break;
		//		}
		//		INT pid = args[4].GetInteger();	//保存在Lua中的对应玩家背包的索引

		//		if(CDataPool::GetMe()->Pet_GetPet(lid) && CDataPool::GetMe()->UserBag_GetItem(pid))
		//		{
		//			PET_GUID_t pg = CDataPool::GetMe()->Pet_GetPet(lid)->m_GUID;
		//			
		//			Packets::CGUseItem pi;
		//			pi.SetBagIndex(pid);
		//			pi.SetTargetPetGUID(pg);

		//			CNetManager::GetMe()->SendPacket(&pi);
		//		}
		//	}
		//	break;
		////4由客户端和服务端的脚本完成
		//case 4:	//宠物驯养
		//	break;
		//case 5:	//宠物征友
		//	{
		//		if(!(args[4].IsString()))
		//		{
		//			KLThrow("LUA:Pet SkillStudy_Do parameter3 error");
		//			break;
		//		}
		//		STRING sU = args[4].GetString();

		//		if(!(args[5].IsInteger()))
		//		{
		//			KLThrow("LUA:Pet SkillStudy_Do parameter4 error");
		//			break;
		//		}
		//		INT npcId = args[5].GetInteger();

		//		if(sU.size() < 2 || npcId < 0)
		//		{
		//			state->PushInteger(0);
		//			ret = 1;
		//			break;
		//		}

		//		if(CDataPool::GetMe()->Pet_GetPet(lid))
		//		{
		//			PET_GUID_t pg = CDataPool::GetMe()->Pet_GetPet(lid)->m_GUID;

		//			//发送征友信息包
		//			Packets::CGIssuePetPlacard pi;
		//			pi.SetNpcID((ObjID_t)npcId);
		//			pi.SetGUID(pg);
		//			pi.SetMessage(sU.c_str());

		//			CNetManager::GetMe()->SendPacket(&pi);
		//		}
		//	}
		//	break;
		//default:
		//	KLThrow("LUA:Pet SkillStudy_Do parameter1 error");
		//	break;
		//}

		//return ret;
		return 0;
	}

	INT Pet::SkillStudyUnlock(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA:Pet SkillStudyUnlock parameter1 error");
			return 0;
		}

		INT pid = args[2].GetInteger();
		if(CDataPool::GetMe()->UserBag_GetItem(pid) && CDataPool::GetMe()->UserBag_GetItem(pid)->IsLocked())
		{
			CDataPool::GetMe()->UserBag_GetItem(pid)->SetLock(FALSE);
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}*/
		return 0;
	}

	INT Pet::SkillStudy_MenPaiSkill_Created(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		if(!(args[2].IsInteger()) || !(args[3].IsInteger()) || !(args[4].IsInteger()))
		{
			KLThrow("LUA:Pet SkillStudy_MenPaiSkill_Created parameter error");
			return 0;
		}

		for(INT i=0; i<3; ++i)
		{
			INT sk = args[2+i].GetInteger();
			CDataPool::GetMe()->PetSkillStudy_MenPaiList_AddSkill(sk);
		}

		CActionSystem::GetMe()->UserPetSkillStudy_Update();*/

		return 0;
	}

	INT Pet::SkillStudy_MenPaiSkill_Clear(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		CDataPool::GetMe()->PetSkillStudy_MenPaiList_Clear();
		CActionSystem::GetMe()->UserPetSkillStudy_Update();*/

		return 0;
	}

	INT Pet::GetGUID(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);

		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet GetGUID parameter error");
			return 0;
		}
		INT nPetNum = args[2].GetInteger();		
		
		SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(nPetNum);
		if( (!My_Pet) || (INVALID_ID == My_Pet->m_nIsPresent) )
		{
			KLThrow("LUA:Pet GetGUID parameter error");
			return 0;
		}

		state->PushInteger(My_Pet->m_GUID.m_uHighSection);
		state->PushInteger(My_Pet->m_GUID.m_uLowSection);*/

		return 2;
	}

	INT Pet::ShowPetList(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		if (!(args[2].IsInteger()) ) 
		{
			KLThrow("LUA:Pet ShowPetList parameter error");
			return 0;
		}
		INT nShow = args[2].GetInteger();

		if(nShow > 0)
		{
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST);
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_CLOSE_PET_LIST);
		}*/

		return 0;
	}

	INT	Pet::ShowMyPetContexMenu(LuaPlus::LuaState* state)
	{
		//PET_GUID_t pg = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID();
		//if(CDataPool::GetMe()->Pet_GetPet(pg))
		//{
		//	tObject* pObj = CObjectManager::GetMe()->FindServerObject(CDataPool::GetMe()->Pet_GetPet(pg)->m_idServer);
		//	if(pObj)
		//	{
		//		CGameInterface::GetMe()->Object_SelectAsMainTarget(CDataPool::GetMe()->Pet_GetPet(pg)->m_idServer, 0);
		//		
		//		//得到鼠标位置
		//		POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();

		//		CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
		//		CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);
		//		
		//		std::vector< STRING > vParam;

		//		vParam.push_back("my_pet");
		//		vParam.push_back("-1");
		//		vParam.push_back(szXPos);
		//		vParam.push_back(szYPos);
		//		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
		//	}
		//}
		return 0;
	}

	INT	Pet::HandlePetMenuItem(LuaPlus::LuaState* state)
	{
		//LuaStack args(state);

		//if(!(args[2].IsString()))
		//{
		//	KLThrow("LUA:Pet HandlePetMenuItem parameter error");
		//	return 0;
		//}

		//STRING strOrder = args[2].GetString();

		//CObject* pObj = NULL;
		//PET_GUID_t pg;

		//if(!(args[3].IsInteger()))
		//{
		//	//没有第二个参数，直接查找当前目标
		//	pObj = (CObject*)CObjectManager::GetMe()->GetMainTarget();
		//	pg = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID();
		//}
		//else
		//{
		//	INT petIdx = args[3].GetInteger();
		//	//有参数，对这个目标宠物进行
		//	if(CDataPool::GetMe()->Pet_GetPet(petIdx) && INVALID_ID != CDataPool::GetMe()->Pet_GetPet(petIdx)->m_idServer)
		//	{
		//		pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(CDataPool::GetMe()->Pet_GetPet(petIdx)->m_idServer);
		//		pg = CDataPool::GetMe()->Pet_GetPet(petIdx)->m_GUID;
		//	}
		//}

		//if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_PlayerNPC)))
		//{
		//	CObject_PlayerNPC* pNpcObj = (CObject_PlayerNPC*)pObj;

		//	//当前目标是自己的宠
		//	if(CDataPool::GetMe()->Pet_GetPet(pg) && pObj->GetServerID() == CDataPool::GetMe()->Pet_GetPet(pg)->m_idServer)
		//	{
		//		if(strOrder == "feed") //喂养
		//		{
		//			INT idxPackage = -1;
		//			CObject_Item* pFind = (CObject_Item*)CDataPool::GetMe()->Pet_GetLowestLevel_Food_From_Package(pg,&idxPackage);
		//			if(!pFind || idxPackage < 0) 
		//			{
		//				STRING strMsg = COLORMSGFUNC("GMGameInterface_Script_Pet_NoFeedFood");
		//				//ADDTALKMSG(strMsg);
		//				ADDNEWDEBUGMSG(strMsg);
		//				return 0;
		//			}

		//			Packets::CGUseItem pi;
		//			pi.SetBagIndex(idxPackage);
		//			pi.SetTargetPetGUID(pg);

		//			CNetManager::GetMe()->SendPacket(&pi);
		//		}
		//		else if(strOrder == "dome")	//驯养
		//		{
		//			INT idxPackage = -1;
		//			CObject_Item* pFind = (CObject_Item*)CDataPool::GetMe()->Pet_GetLowestLevel_Dome_From_Package(pg,&idxPackage);

		//			SDATA_PET* My_Pet= CDataPool::GetMe()->Pet_GetPet(pg);
		//			if(My_Pet && My_Pet->m_nHappiness == 100)
		//			{
		//				STRING strMsg = COLORMSGFUNC("GMGameInterface_Script_Pet_NoNeedDemo");
		//				//ADDTALKMSG(strMsg);
		//				ADDNEWDEBUGMSG(strMsg);
		//				return 0;
		//			}

		//			if(!pFind || idxPackage < 0)
		//			{
		//				STRING strMsg = COLORMSGFUNC("GMGameInterface_Script_Pet_NoDemoFood");
		//				//ADDTALKMSG(strMsg);
		//				ADDNEWDEBUGMSG(strMsg);
		//				return 0;
		//			}

		//			Packets::CGUseItem pi;
		//			pi.SetBagIndex(idxPackage);
		//			pi.SetTargetPetGUID(pg);

		//			CNetManager::GetMe()->SendPacket(&pi);
		//		}
		//		else if(strOrder == "relax") // 休息
		//		{
		//			// 向服务器发送
		//			CGManipulatePet Msg;

		//			Msg.SetGUID(pg);
		//			Msg.SetManipulateType(MANIPULATE_DELETEPET);
		//			CNetManager::GetMe()->SendPacket(&Msg);
		//		}
		//	}
		//	// 当前目标是宠物
		//	else if(pNpcObj->GetNpcType() == NPC_TYPE_PET)
		//	{
		//		if(strOrder == "detail") // 查看详细信息
		//		{
		//			// 发送查询宠物详细信息的Packet.
		//			CGManipulatePet pia;
		//			pia.SetManipulateType(MANIPULATE_ASKOTHERPETINFO);
		//			pia.SetObjID((ObjID_t)(pNpcObj->GetServerID()));

		//			CNetManager::GetMe()->SendPacket(&pia);
		//		}
		//	}
		//}

		return 0;
	}

	INT	Pet::GetPetPortraitByIndex(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA:Pet GetPetPortraitByIndex parameter error");
			return -1;
		}

		INT petNum = args[2].GetInteger();
		SDATA_PET* pData = CDataPool::GetMe()->Pet_GetPet(petNum);

		if(pData && pData->m_nDataID != INVALID_ID)
		{
			DBC_DEFINEHANDLE(s_pCreatureDBC, DBC_CREATURE_ATT);
			const _DBC_CREATURE_ATT* pLine = (const _DBC_CREATURE_ATT*)s_pCreatureDBC->Search_Index_EQU(pData->m_nDataID);

			if(pLine)
			{
				state->PushString(pLine->szIconName);
				return 1;
			}
		}*/

		state->PushString("");
		return 1;
	}

	INT	Pet::GetPetImpactNum(LuaPlus::LuaState* state)
	{
	/*	LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA:Pet GetPetImpactNum parameter error");
			return -1;
		}

		INT petNum = args[2].GetInteger();
		if(CDataPool::GetMe()->Pet_GetPet(petNum))
		{
			SDATA_PET* pData = CDataPool::GetMe()->Pet_GetPet(petNum);
			CObject_PlayerNPC* pNpcObj = NULL;

			if(pData)
			{
				pNpcObj = (CObject_PlayerNPC*)CObjectManager::GetMe()->FindServerObject(pData->m_idServer);
			}

			if(pNpcObj && pNpcObj->GetImpactEffectMap())
			{
				state->PushInteger((INT)pNpcObj->GetImpactEffectMap()->size());
				return 1;
			}
		}

		state->PushInteger(0);*/
		return 1;
	}

	INT Pet::GetPetImpactIconNameByIndex(LuaPlus::LuaState* state)
	{
		/*LuaStack args(state);
		
		if(!(args[2].IsInteger()) || !(args[3].IsInteger()))
		{
			KLThrow("LUA:Pet GetPetImpactIconNameByIndex parameter error");
			return -1;
		}

		INT petNum = args[2].GetInteger();
		INT impactIdx = args[3].GetInteger();

		if(CDataPool::GetMe()->Pet_GetPet(petNum))
		{
			SDATA_PET* pData = CDataPool::GetMe()->Pet_GetPet(petNum);
			CObject_PlayerNPC* pNpcObj = (CObject_PlayerNPC*)CObjectManager::GetMe()->FindServerObject(pData->m_idServer);

			if(pNpcObj && pNpcObj->GetImpactEffectMap())
			{
				CImpactEffectMap* pImpactMap = (CImpactEffectMap*)pNpcObj->GetImpactEffectMap();
				if(impactIdx < (INT)pImpactMap->size())
				{
					CImpactEffectMap::iterator it = pImpactMap->begin();
					for(int i = 0; i < impactIdx; i++, it++);

					_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( it->second->GetImpactID() );
					if(pBuffImpact) 
					{
						state->PushString(pBuffImpact->m_szIconName);
						state->PushString(pBuffImpact->m_pszInfo);
					}
				}
			}
		}*/

		return 2;
	}

	INT	Pet::GetSkillPassive(LuaPlus::LuaState* state)
	{
//		LuaStack args(state);
//
//		if ( !(args[2].IsInteger()) || (!(args[3].IsInteger())) ) 
//		{
//			KLThrow("LUA:Pet GetSkillPassive parameter error");
//			return 0;
//		}
//		INT nPetNum = args[2].GetInteger();	
//		INT nPetSkill =  args[3].GetInteger();	
//		
//		if(nPetSkill < 0 || nPetSkill >= PET_MAX_SKILL_COUNT)
//		{
//			state->PushNumber(-1);
////			KLThrow("LUA:Pet GetSkillPassive parameter PetSkill error");
//			return 1;
//		}
//
//		const PET_SKILL* pPet_Skill = CDataPool::GetMe()->Pet_GetSkill(nPetNum,nPetSkill);
//		
//		if(!pPet_Skill || (!pPet_Skill->m_pDefine))
//		{
//			state->PushNumber(-1);
//			return 1;
//		}
//
//		state->PushNumber( pPet_Skill->m_pDefine->m_nOperateModeForPetSkill != PET_SKILL_OPERATE_NEEDOWNER);
		return 1;
	}
}