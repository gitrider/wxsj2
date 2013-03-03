//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_DoAction.h
// 模块	:	Script
// 功能	 :  让指定角色做指定行为的相关函数
// 修改历史:
//	Gaoqi创建
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_DOACTION_H__
#define __LUAFNTBL_DOACTION_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "SkillInfo.h"
#include "GameStruct_Skill.h"
#include "StdImpact000.h"
#include "ScriptLogic.h"
#include "Impact_Core.h"
#include "ActionDelegator.h"

using namespace Combat_Module;
using namespace Impact_Module;
using namespace Script_Module;
using namespace Action_Module;

namespace LuaFnTbl
{

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-5-17	11:29
	 *	函数名称：	LuaFnDoAction
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *				nActionID
	 *					动作编号
	 *				TargetID
	 *					目标编号（没有目标填 -1 就可以了）
	 *	功能说明：	对目标做动作
	 *	修改记录：
	*****************************************************************************/
	INT	LuaFnDoAction(Lua_State* L) 
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		SkillID_t nActionID;
		ObjID_t TargetID;

		sceneId		=	Lua_ValueToNumber(L,1);
		selfId		=	Lua_ValueToNumber(L,2);
		nActionID	=	Lua_ValueToNumber(L,3);
		TargetID	=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDoAction")

			pHuman->DoAction( nActionID, TargetID );
			return 0;

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-1-10
	*     函数名称 : LuaFnUnitDoEmoteAction
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nSkillID
	*					技能编号
	*				nTargetID
	*					目标编号
	*     功能说明 :
	*/
	INT	LuaFnUnitDoEmoteAction(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		SkillID_t nSkillID;
		ObjID_t nTargetID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);
		nSkillID	=	Lua_ValueToNumber(L,3);
		nTargetID	=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnUnitDoEmoteAction")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			GetGlobalActionDelegator().RegisterEmoteAction(rMe, nSkillID);
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-1-10
	*     函数名称 : LuaFnUnitUseSkill
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nSkillID
	*					技能编号
	*				nTargetID
	*					目标编号
	*				fPos_X
	*					目标位置的X坐标
	*				fPos_Z
	*					目标位置的Z坐标
	*				fDir
	*					目标方向
	*				bPassSkillCheck
	*					是否忽略技能的条件检测和物品消耗
	*     功能说明 :
	*/
	INT	LuaFnUnitUseSkill(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		SkillID_t nSkillID;
		ObjID_t nTargetID;
		FLOAT fPos_X;
		FLOAT fPos_Z;
		FLOAT fDir;
		BOOL bPassSkillCheck;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);
		nSkillID	=	Lua_ValueToNumber(L,3);
		nTargetID	=	Lua_ValueToNumber(L,4);
		fPos_X		=	Lua_ValueToNumber(L,5);
		fPos_Z		=	Lua_ValueToNumber(L,6);
		fDir		=	Lua_ValueToNumber(L,7);
		bPassSkillCheck = 	((0==Lua_ValueToNumber(L,8))?FALSE:TRUE);

		BEGINSCENEDEFINE("LuaFnUnitUseSkill")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			WORLD_POS pos;
			pos.m_fX = fPos_X;
			pos.m_fZ = fPos_Z;
			rMe.Skill_ExecFromScript(nSkillID, nTargetID, pos, fDir, bPassSkillCheck);			
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnSendSpecificImpactToUnit
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nSenderID
	*					效果创造者ID,无符号整型
	*				nTargetID
	*					目标角色ID,无符号整型
	*				nImpactDataIndex
	*					效果数据索引,无符号整型
	*				nDelayTime
	*					延迟时间
	*     功能说明 :
	*				
	*/
	INT	LuaFnSendSpecificImpactToUnit(Lua_State* L)
	{
		__ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		ObjID_t nSenderID;
		ObjID_t nTargetID;
		INT nImpactDataIndex;
		INT nDelayTime;

		sceneId				=	Lua_ValueToNumber(L,1);
		nSelfID				=	Lua_ValueToNumber(L,2);
		nSenderID			=	Lua_ValueToNumber(L,3);
		nTargetID			=	Lua_ValueToNumber(L,4);
		nImpactDataIndex	=	Lua_ValueToNumber(L,5);
		nDelayTime			=	Lua_ValueToNumber(L,6);

		BEGINSCENEDEFINE("LuaFnSendSpecificImpactToUnit")
			Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pSelf)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pSelf->GetObjType()))
			{
				RETURNFALSE
			}
			Obj* pSender = (Obj*) (pScene->GetObjManager()->GetObj(nSenderID));
			if(NULL==pSender)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pSender->GetObjType()))
			{
				RETURNFALSE
			}
			Obj* pTarget = (Obj*) (pScene->GetObjManager()->GetObj(nTargetID));
			if(NULL==pTarget)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pTarget->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rTar = *(Obj_Character*)pTarget;
			BOOL bRet = g_ImpactCore.SendImpactToUnit(rTar, nImpactDataIndex, nSenderID, nDelayTime);
			Lua_PushNumber(L, bRet);
			return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		__LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnCancelSpecificImpact
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nImpactDataIndex
	*					效果数据索引, 无符号整型；在标准效果表里查询
	*     功能说明 :
	*				
	*/
	INT	LuaFnCancelSpecificImpact(Lua_State* L)
	{
		__ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		INT nImpactDataIndex;

		sceneId				=	Lua_ValueToNumber(L,1);
		nSelfID				=	Lua_ValueToNumber(L,2);
		nImpactDataIndex	=	Lua_ValueToNumber(L,3);
		
		BEGINSCENEDEFINE("LuaFnCancelSpecificImpact")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			INT nRet = rMe.Impact_DispelSpecificImpactByDataIndex(nImpactDataIndex, 1);
			Lua_PushNumber(L, nRet);
			return 1;

		ENDSCENEDEFINE
		RETURNFALSE

		__LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnVerifyUsedItem
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnVerifyUsedItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnVerifyUsedItem")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			if(Obj::OBJ_TYPE_HUMAN!=pObj->GetObjType())
			{
				RETURNFALSE
			}
			Obj_Human& rMe = *(static_cast<Obj_Human*>(pObj));
			BOOL bRet = rMe.VerifyItem();
			//return the result
			Lua_PushNumber(L, bRet);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnDepletingUsedItem
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnDepletingUsedItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnDepletingUsedItem")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			BOOL bRet = rMe.DepletingUsedItem();
			//return the result
			Lua_PushNumber(L, bRet);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetObjID
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetTargetObjID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetObjID")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetObj());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetPosition_X
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetTargetPosition_X(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetPosition_X")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPosition().m_fX);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetPosition_Z
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetTargetPosition_Z(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetPosition_Z")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPosition().m_fZ);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetBagSlot
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetTargetBagSlot(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetBagSlot")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetBagSlot());
			return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-4-11	19:29
	 *	函数名称：	LuaFnGetBagIndexOfUsedItem
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	?
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetBagIndexOfUsedItem( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetBagIndexOfUsedItem")
			TargetingAndDepletingParams_T& rParams = pHuman->GetTargetingAndDepletingParams();
			
			Lua_PushNumber(L, rParams.GetBagIndexOfDepletedItem());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-8-16	12:23
	 *	函数名称：	LuaFnGetItemIndexOfUsedItem
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	取被使用的物品的物品号
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetItemIndexOfUsedItem( Lua_State* L )
	{
	LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t selfId;
	    //Steven.Han end   2006-9-18 16:16
		//CHECKPARAM_RET0(2)
		if( FALSE == LuaVerifyAllNumberParam( L, 1, 2, "LuaFnGetItemIndexOfUsedItem" ) )
		{
			RETURNFALSE
		}

		sceneId = Lua_ValueToNumber( L, 1 );
		selfId = Lua_ValueToNumber( L, 2 );

		BEGINHUMANDEFINE("LuaFnGetItemIndexOfUsedItem")
			TargetingAndDepletingParams_T& rParams = pHuman->GetTargetingAndDepletingParams();
			
			Lua_PushNumber( L, rParams.GetItemIndexOfDepletedItem() );
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetLowSectionOfTargetPetGuid
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetLowSectionOfTargetPetGuid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetLowSectionOfTargetPetGuid")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPetGuid().GetLowSection());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetHighSectionOfTargetPetGuid
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetHighSectionOfTargetPetGuid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHighSectionOfTargetPetGuid")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPetGuid().GetHighSection());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnIsScriptDynamicParamIndexLegal
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nIndex
	*					参数索引
	*     功能说明 :
	*				
	*/
	INT	LuaFnIsScriptDynamicParamIndexLegal(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		INT nIndex;

		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);
		nIndex		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnIsScriptDynamicParamIndexLegal")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.IsDynamicParamIndexLegal(nIndex));
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetScriptDynamicParamByIndex
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nIndex
	*					参数索引
	*     功能说明 :
	*				
	*/
	INT	LuaFnGetScriptDynamicParamByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		INT nIndex;
		
		sceneId		=	Lua_ValueToNumber(L,1);
		nSelfID		=	Lua_ValueToNumber(L,2);
		nIndex		=	Lua_ValueToNumber(L,3);
		INT nRet = 0;
		BEGINSCENEDEFINE("LuaFnGetScriptDynamicParamByIndex")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(TRUE==rParams.IsDynamicParamIndexLegal(nIndex))
			{
				//return the specific param
				Lua_PushNumber(L, rParams.GetDynamicParamByIndex(nIndex));
				return 1 ;
			}
			else
			{
				Lua_PushNumber(L, -1);
				return 1 ;
			}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 高骐
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnSetScriptDynamicParamByIndex
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nIndex
	*					参数索引
	*				nValue
	*					参数值
	*     功能说明 :
	*				
	*/
	INT	LuaFnSetScriptDynamicParamByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t nSelfID;
		INT nIndex;
		INT nValue;

		sceneId	=	Lua_ValueToNumber(L,1);
		nSelfID	=	Lua_ValueToNumber(L,2);
		nIndex	=	Lua_ValueToNumber(L,3);
		nValue	=	Lua_ValueToNumber(L,4);
		BEGINSCENEDEFINE("LuaFnSetScriptDynamicParamByIndex")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			rParams.SetDynamicParamByIndex(nIndex, nValue);
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
}

#endif
