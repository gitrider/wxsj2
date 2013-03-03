//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Battle.h
// 模块	:	Script
// 功能	 :  战斗相关函数
// 修改历史:
//	yangfei创建
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_BATTLE_H__
#define __LUAFNTBL_BATTLE_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"

namespace LuaFnTbl
{
	/*
	 *	获得对应ObjID 的玩家的宠物数量
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPetCount
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
     *
	 *	   返回  INT 角色的宠物数量
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPetCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		INT	nCount;


		BEGINHUMANDEFINE("LuaFnGetPetCount")
			nCount = pHuman->GetHorseCount() ;
		Lua_PushNumber(L,nCount);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的DataID(即 怪物表中的号)
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_DataID
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物等级
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_DataID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nDataID;


		BEGINHUMANDEFINE("LuaFnGetPet_DataID")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nDataID = pPetItem->GetDataID();
				Lua_PushNumber(L,nDataID);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的等级
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Level
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物等级
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Level(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nLevel;


		BEGINHUMANDEFINE("LuaFnGetPet_Level")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nLevel = pPetItem->GetLevel();
				Lua_PushNumber(L,nLevel);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的携带等级
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_TakeLevel
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物的携带等级
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_TakeLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nTakeLevel;


		BEGINHUMANDEFINE("LuaFnGetPet_TakeLevel")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nTakeLevel = pPetItem->GetTakeLevel();
				Lua_PushNumber(L,nTakeLevel);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的HP
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_HP
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物HP
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_HP(Lua_State* L)
	{
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		Assert( FALSE && "马没有HP, 此接口已废弃" );
		//LUA_ENTER_FUNCTION
		//sceneId			=	Lua_ValueToNumber(L,1);
		//selfId			=	Lua_ValueToNumber(L,2);	
		//nIndex			=	Lua_ValueToNumber(L,3);	
		//INT	nHP;


		//BEGINHUMANDEFINE("LuaFnGetPet_HP")
		//	PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
		//	Item *pPetItem = pHuman->GetPetItem(guidPet);
		//	if(pPetItem != NULL)
		//	{
		//		nHP = pPetItem->GetHP();
		//		Lua_PushNumber(L,nHP);
		//		return 1;
		//	}
		//ENDHUMANDEFINE
		//RETURNFALSE
		//LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的Max HP
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_MaxHP
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物Max HP
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_MaxHP(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nMaxHP;


		BEGINHUMANDEFINE("LuaFnGetPet_MaxHP")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			nMaxHP = pHuman->GetPetMaxHP(guidPet);
			Lua_PushNumber(L,nMaxHP);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的寿命
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Life
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物寿命
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Life(Lua_State* L)
	{
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		Assert( FALSE && "马没有生命周期，此接口废弃" );
		//LUA_ENTER_FUNCTION
		//
		//sceneId			=	Lua_ValueToNumber(L,1);
		//selfId			=	Lua_ValueToNumber(L,2);	
		//nIndex			=	Lua_ValueToNumber(L,3);	
		//INT	nLife;


		//BEGINHUMANDEFINE("LuaFnGetPet_Life")
		//	PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
		//	Item *pPetItem = pHuman->GetPetItem(guidPet);
		//	if(pPetItem != NULL)
		//	{
		//		nLife = pPetItem->GetLife();
		//		Lua_PushNumber(L,nLife);
		//		return 1;
		//	}
		//ENDHUMANDEFINE
		//RETURNFALSE
		//LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的类型（宝宝 0，变异 1，野生 2）
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Type
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物类型 （宝宝 0，变异 1，野生 2）
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Type(Lua_State* L)
	{
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		Assert( FALSE && "马没有类型，接口废弃" );
		//LUA_ENTER_FUNCTION
		//
		//sceneId			=	Lua_ValueToNumber(L,1);
		//selfId			=	Lua_ValueToNumber(L,2);	
		//nIndex			=	Lua_ValueToNumber(L,3);	
		//INT	nType;


		//BEGINHUMANDEFINE("LuaFnGetPet_Type")
		//	PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
		//	Item *pPetItem = pHuman->GetPetItem(guidPet);
		//	if(pPetItem != NULL)
		//	{
		//		nType = pPetItem->GetPetType();
		//		Lua_PushNumber(L,nType);
		//		return 1;
		//	}
		//ENDHUMANDEFINE
		//RETURNFALSE
		//LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的代数
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Generation
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物代数
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Generation(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nGeneration;


		BEGINHUMANDEFINE("LuaFnGetPet_Generation")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nGeneration = pPetItem->GetGeneration();
				Lua_PushNumber(L,nGeneration);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的快乐度
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Happyness
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物欢乐度
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Happyness(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nHappyness;


		BEGINHUMANDEFINE("LuaFnGetPet_Happyness")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nHappyness = pPetItem->GetHappiness();
				Lua_PushNumber(L,nHappyness);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的力量资质
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_StrPerception
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物力量资质
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_StrPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nStrPerception;

		BEGINHUMANDEFINE("LuaFnGetPet_StrPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nStrPerception = pPetItem->GetStrengthPer();
				Lua_PushNumber(L,nStrPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的敏捷资质
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_ConPerception
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物敏捷资质
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_SmartnessPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nConPerception;


		BEGINHUMANDEFINE("LuaFnGetPet_ConPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nConPerception = pPetItem->GetSmartnessPer();
				Lua_PushNumber(L,nConPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的智力资质
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_DexPerception
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物智力资质
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_MindPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nDexPerception;


		BEGINHUMANDEFINE("LuaFnGetPet_DexPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nDexPerception = pPetItem->GetMindPer();
				Lua_PushNumber(L,nDexPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的体质资质
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_IntPerception
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物体质资质
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_ConstitutionPerception(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nIntPerception;


		BEGINHUMANDEFINE("LuaFnGetPet_IntPerception")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nIntPerception = pPetItem->GetConstitutionPer();
				Lua_PushNumber(L,nIntPerception);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的根骨
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_GenGu
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物根骨
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_GenGu(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nGenGu;


		BEGINHUMANDEFINE("LuaFnGetPet_GenGu")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nGenGu = pPetItem->GetGenGu();
				Lua_PushNumber(L,nGenGu);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的一级属性剩余点数
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_RemainPoint
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物一级属性剩余点数
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_RemainPoint(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nRemainPoint;


		BEGINHUMANDEFINE("LuaFnGetPet_RemainPoint")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nRemainPoint = pPetItem->GetRemainPoint();
				Lua_PushNumber(L,nRemainPoint);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的经验值
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Exp
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  INT 宠物经验值
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Exp(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		INT	nExp;


		BEGINHUMANDEFINE("LuaFnGetPet_Exp")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nExp = pPetItem->GetExp();
				Lua_PushNumber(L,nExp);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物的基础一级战斗属性
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_AttrLevel1
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
	 *			INT			nAttr	  属性(0 力量, 1 灵气, 2 体制, 3 定力, 4 身法)
     *
	 *	   返回  INT 宠物基础一级战斗属性
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_AttrLevel1(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		INT nAttr;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		nAttr			=	Lua_ValueToNumber(L,4);	
		INT	nValue;


		BEGINHUMANDEFINE("LuaFnGetPet_AttrLevel1")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nValue = pPetItem->GetLvl1Attr((CHAR_ATTR_LEVEL1)nAttr);
				Lua_PushNumber(L,nValue);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	获得对应ObjID 的玩家的宠物技能
	 *     开发人员 : jack
	 *     编写时间 : 2006-2-27
	 *     函数名称 : LuaFnGetPet_Skill
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
	 *			INT			nSkillIndex 技能槽索引(0-5),其中第0个是主动技能槽
     *
	 *	   返回  INT 宠物技能
	 *     功能说明 :
	 *
	 */
	INT	LuaFnGetPet_Skill(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		INT nSkillIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	
		nSkillIndex		=	Lua_ValueToNumber(L,4);	
		INT	nSkill;


		BEGINHUMANDEFINE("LuaFnGetPet_Skill")
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			Item *pPetItem = pHuman->GetHorseItem(guidPet);
			if(pPetItem != NULL)
			{
				nSkill = pPetItem->GetSkill(nSkillIndex).m_nSkillID;
				Lua_PushNumber(L,nSkill);
				return 1;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *     开发人员 : jack
	 *     编写时间 : 2006-3-27
	 *     函数名称 : LuaFnCreatePet
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nPetDataID 宠物编号
     *
	 *	   返回  1 成功，0 失败
	 *     功能说明 :
	 *			在给玩家生成一个宠物
	 */
	INT	LuaFnCreatePet(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nPetDataID;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nPetDataID		=	Lua_ValueToNumber(L,3);	

		BEGINHUMANDEFINE("LuaFnCreatePet")
			PET_LOG_PARAM	PetLogParam;
			ORESULT eRet = pHuman->AddPetByDataID(&PetLogParam,nPetDataID ) ;
			if( eRet==OR_OK )
			{
				PetLogParam.CharGUID	=	pHuman->GetGUID();
				PetLogParam.OPType		=	PET_OP_SCRIPT_CREATE;
				PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
				PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SavePetLog(&PetLogParam);

				Lua_PushNumber(L,1);
			}
			else
			{
				Lua_PushNumber(L,0);
			}
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/*
	 *	删除对应ObjID 的玩家的宠物
	 *     开发人员 : jack
	 *     编写时间 : 2006-3-27
	 *     函数名称 : LuaFnDeletePet
	 *     参数说明 :  
	 *			SceneID_t	sceneID   场景编号
	 *          ObjID_t		selfID	  角色编号
	 *			INT			nIndex	  宠物位置
     *
	 *	   返回  1 成功， 0 失败
	 *     功能说明 :
	 *
	 */
	INT	LuaFnDeletePet(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);	
		nIndex			=	Lua_ValueToNumber(L,3);	


		BEGINHUMANDEFINE("LuaFnDeletePet")
			PET_LOG_PARAM	PetLogParam;
			PET_GUID_t guidPet = pHuman->GetPetGUIDByValidIndex(nIndex);
			ORESULT eRet = pHuman->FreePetToNature(&PetLogParam,guidPet) ;
			if( eRet==OR_OK )
			{
			
				PetLogParam.CharGUID	=	pHuman->GetGUID();
				
				PetLogParam.OPType		=	PET_OP_SCRIPT_DELETE;
				PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
				PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SavePetLog(&PetLogParam);
				Lua_PushNumber(L,1);
			}
			else
			{
				Lua_PushNumber(L,0);
			}
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


    /** -----------------------------------------------------------------------
    *     开发人员 : amos
    *     编写时间 : 2008-05-22
    *     函数名称 : LuaFnGetPKLevel
    *     参数说明 :
    *				L
    *					LuaState执行环境指针
    *				sceneId
    *					场景号
    *				selfId
    *					玩家ID
    *				deltaLevel
    *					等级差
    *     功能说明 :
    *				根据等级差获得等级基数
    */
    INT LuaFnGetPKLevel(Lua_State* L)	
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT deltaLevel;

        sceneId		= Lua_ValueToNumber(L,1);
        selfId		= Lua_ValueToNumber(L,2);
        deltaLevel	= Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaFnGetPKLevel")

        for (INT i = 0; i < MAX_PK_NUM; ++i)
        {
            if ( deltaLevel >= g_PKLevelTbl.m_aPKPointInfo[i].m_nMin &&
                 deltaLevel < g_PKLevelTbl.m_aPKPointInfo[i].m_nMax   )
            {
                INT n = g_PKLevelTbl.m_aPKPointInfo[i].m_nValue;

                Lua_PushNumber(L,n);
                break;
            }
        }
        return 1;

        ENDHUMANDEFINE

        LUA_LEAVE_FUNCTION

        return 0 ;
    }

    /** -----------------------------------------------------------------------
    *     开发人员 : amos
    *     编写时间 : 2008-05-22
    *     函数名称 : LuaFnGetPKPoint
    *     参数说明 :
    *				L
    *					LuaState执行环境指针
    *				sceneId
    *					场景号
    *				selfId
    *					玩家ID
    *				PKValue
    *					PK值
    *     功能说明 :
    *				根据PK值差获得PK基数
    */
    INT LuaFnGetPKPoint(Lua_State* L)	
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT PKValue;

        sceneId		= Lua_ValueToNumber(L,1);
        selfId		= Lua_ValueToNumber(L,2);
        PKValue	    = Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaFnGetPKPoint")

            for (INT i = 0; i < MAX_PK_NUM; ++i)
            {
                if ( PKValue >= g_PKPointTbl.m_aPKPointInfo[i].m_nMin &&
                    PKValue < g_PKPointTbl.m_aPKPointInfo[i].m_nMax   )
                {
                    INT n = g_PKPointTbl.m_aPKPointInfo[i].m_nValue;

                    Lua_PushNumber(L,n);
                    break;
                }
            }
            return 1;

            ENDHUMANDEFINE

            LUA_LEAVE_FUNCTION

            return 0 ;
    }


    /** -----------------------------------------------------------------------
    *     开发人员 : amos
    *     编写时间 : 2008-05-22
    *     函数名称 : LuaFnGetPKPunish
    *     参数说明 :
    *				L
    *					LuaState执行环境指针
    *				sceneId
    *					场景号
    *				selfId
    *					玩家ID
    *				PunishValue
    *					惩罚值
    *     功能说明 :
    *				根据惩罚值差获得惩罚值基数
    */
    INT LuaFnGetPKPunish(Lua_State* L)	
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT PunishValue;

        sceneId		= Lua_ValueToNumber(L,1);
        selfId		= Lua_ValueToNumber(L,2);
        PunishValue	    = Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaFnGetPKPunish")

            for (INT i = 0; i < MAX_PK_NUM; ++i)
            {
                if ( PunishValue >= g_PKPunishTbl.m_aPKPointInfo[i].m_nMin &&
                    PunishValue < g_PKPunishTbl.m_aPKPointInfo[i].m_nMax   )
                {
                    INT n = g_PKPunishTbl.m_aPKPointInfo[i].m_nValue;

                    Lua_PushNumber(L,n);
                    break;
                }
            }
            return 1;

            ENDHUMANDEFINE

            LUA_LEAVE_FUNCTION

            return 0 ;
    }


    /** -----------------------------------------------------------------------
    *     开发人员 : amos
    *     编写时间 : 2008-5-21
    *     函数名称 : LuaAddHonor
    *     参数说明 : 
    *     功能说明 :
    *				SceneID
    *					场景编号
    *				selfId
    *					调用者ID,无符号整型
    *				nHonor 
    *					增加荣誉
    *				
    */
    INT	LuaAddHonor(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT nHonor ;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        nHonor     		=	Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaAddHonor")

        pHuman->SetHonor(pHuman->GetHonor() + nHonor);


        ENDHUMANDEFINE
        return	0;

        LUA_LEAVE_FUNCTION
        return  0;
    }

    /** -----------------------------------------------------------------------
    *     开发人员 : amos
    *     编写时间 : 2008-5-21
    *     函数名称 : LuaSubHonor
    *     参数说明 : 
    *     功能说明 :
    *				SceneID
    *					场景编号
    *				selfId
    *					调用者ID,无符号整型
    *				nHonor 
    *					荣誉
    *				
    */
    INT	LuaSubHonor(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT nHonor ;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        nHonor     		=	Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaSubHonor")
            
            INT nTotal  = pHuman->GetHonor() - nHonor;
            if (nTotal < 0)
                nTotal = 0;

            pHuman->SetHonor(nTotal);


        ENDHUMANDEFINE
            return	0;

        LUA_LEAVE_FUNCTION
            return  0;
    }
}

#endif