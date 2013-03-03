//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Attr.h
// 模块	:	Script
// 功能	 :  属性相关函数
// 修改历史:
//	yangfei创建
//-----------------------------------------------------------------------------

#ifndef __LUAFNTBL_ATTR_H__
#define __LUAFNTBL_ATTR_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCNotifyChangeScene.h"
#include "GCDetailAttrib_Pet.h"
#include "GCChat.h"
#include "AI_Human.h"
#include "HumanItemLogic.h"
#include "PetManager.h"
#include "Obj_Pet.h"
#include "ItemOperator.h"
#include "AI_Monster.h"

#include "GCDetailSkillList.h"
#include "GamePlayer.h"

namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetPos
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
    *					调用该函数场景ID,short类型
	*				selfId
	*					调用者ID,无符号整型
	*				x
	*					需要warp到的新位置的X坐标值
	*				z
	*					需要warp到的新位置的Z坐标值
	*     功能说明 :
	*				角色的同场景转移
	*				设置角色(玩家和怪物)跳转到当前地图上的某点
	*/
	INT LuaFnSetPos( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		FLOAT x;
		FLOAT z;
		
		if(!Lua_IsNumber(L,1) || !Lua_IsNumber(L,2) || !Lua_IsNumber(L,3) || !Lua_IsNumber(L,4))
		{
			RETURNFALSE
		}

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		x = Lua_ValueToNumber(L,3);
		z = Lua_ValueToNumber(L,4);

		BEGINCHARCTERDEFINE("LuaFnSetPos")

		WORLD_POS Old = *pCharacter->getWorldPos() ;
		WORLD_POS New(x,z);

		pScene->GetMap()->VerifyPos( &New ) ;
		if( !pScene->GetMap()->IsCanGo( New ) )
		{
			return 0;
		}

		((Obj_Human*)(pCharacter))->GetHumanAI()->PushCommand_Idle( ) ;
		pCharacter->Teleport( &New ) ;

		return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetRevPos
	*     参数说明 : 
	*				L
	*				LuaState执行环境指针
	*				sceneId
	*				调用该函数场景ID,short
	*				selfId
	*				调用者ID,无符号整型
	*				x
	*					位置x坐标
	*				z
	*					位置z坐标
	*     功能说明 :
	*				设置玩家的重生点（再次登入当前世界的位置）
	*				这个函数有待细究
	*/
	INT LuaFnSetRevPos( Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		FLOAT x;
		FLOAT z;
	
		sceneId = Lua_ValueToNumber(L,1);
		selfId	= Lua_ValueToNumber(L,2);
		x		= Lua_ValueToNumber(L,3);
		z		= Lua_ValueToNumber(L,4);

		BEGINCHARCTERDEFINE("LuaFnSetRevPos")
			if( pCharacter->GetObjType()!=Obj::OBJ_TYPE_MONSTER )
				return 0 ;
			Obj_Monster* pMonster = (Obj_Monster*)pCharacter ;
			WORLD_POS pos(x,z) ;
			pMonster->SetRespawnPos(&pos) ;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnNewWorld
	*     参数说明 : 
	*				L
	*				LuaState执行环境指针
	*				sceneId
	*				调用该函数场景ID,short
	*				selfId
	*				调用者ID,无符号整型
	*				targetSceneId
	*				将要跳转场景ID,short
	*				x
	*					位置x坐标
	*				z
	*					位置z坐标
	*     功能说明 :
	*				用于切换场景
	*				把selfId的玩家从sceneId标志的场景跳转到targetSceneId标志的场景的(x,z)位置
	*				示例:NewWorld(sceneId,selfId,4,100,102)
	*/
	INT LuaFnNewWorld(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		SceneID_t targetSceneId;
		FLOAT x;
		FLOAT z;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetSceneId = Lua_ValueToNumber(L,3);
		x = Lua_ValueToNumber(L,4);
		z = Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnNewWorld")
		if( pHuman->getScene()->SceneID() == targetSceneId )
		{
			return 0 ;
		}

		WORLD_POS New(x,z);
		pHuman->ChangeScene( pHuman->getScene()->SceneID(), targetSceneId, New, 9 ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetWorldPos
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
    *				得到角色(character)当前坐标
	*				返回x和z的FLOAT类型值
	*				示例: x,z = GetWorldPos(sceneId,selfId) 
	*/
	INT LuaFnGetWorldPos(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetWorldPos")
		Lua_PushNumber(L,(int)(pCharacter->getWorldPos()->m_fX));
		Lua_PushNumber(L,(int)(pCharacter->getWorldPos()->m_fZ));

		return 2;
		ENDCHARCTERDEFINE

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 陈军
	*     编写时间 : 2010-6-7
	*     函数名称 : LuaFnGetSceneNPCPos
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				uDataID
	*					无符号整型
	*     功能说明 :
    *				得到角色(character)当前坐标
	*				返回x和z的FLOAT类型值
	*				示例: x,z = GetWorldPos(sceneId,npcId) 
	*/
	INT LuaFnGetSceneNPCPos(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		UINT uDataID;

		sceneId = Lua_ValueToNumber(L,1);
		uDataID = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetSceneNPCPos")

		ObjID_t	MonsterID = pScene->GetMonsterManager()->FindMonsterByDataID(uDataID);

		if(MonsterID != INVALID_ID)
		{
			Obj_Monster* pMonster = pScene->GetMonsterManager()->GetMonster(MonsterID);
			if(pMonster)
			{
				Lua_PushNumber(L,(int)pMonster->getWorldPos()->m_fX);
				Lua_PushNumber(L,(int)pMonster->getWorldPos()->m_fZ);
				return 2;
			}
		}
		
		ENDSCENEDEFINE

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetPatrolId
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				设置Obj_Monster的巡逻路径
	*				示例:SetPatrolId(sceneId,selfId,0)
	*				patrolId的有效值是 
	*				0 到 该sceneId所指场景_patrolpoint.ini文件PATROLNUMBER(不包括)之间的值
	*/
	INT LuaFnSetPatrolId(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT patrolId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		patrolId = Lua_ValueToNumber(L,3);

		BEGINMONSTERDEFINE("LuaFnSetPatrolId")
		if(patrolId >= 0)
		{
			pMonster->SetPatrolId(patrolId);
			pMonster->GetMonsterAI()->StartPatrol(TRUE);
		}
		else
		{
			pMonster->GetMonsterAI()->StopPatrol();
			pMonster->setDir( pMonster->GetRespawnDir() );
			pMonster->SetPatrolId(INVALID_ID);
		}
		return 0;
		ENDMONSTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnIsCaptain
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				判断当前玩家是否为队长，返回值为0：不是队长；返回值为1：队长。
	*				没有测试
	*/
	INT LuaFnIsCaptain( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnIsCaptain")
		if(pHuman->GetTeamInfo()->Leader()->m_GUID == pHuman->GetGUID())
		{
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
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddXinFa
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				xinfaId
	*					心法编号
	*     功能说明 :
	*				给当前玩家加某个心法
	*				没有测试
	*/
	INT LuaFnAddXinFa( Lua_State* L)	
	{
		
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT xinfaId;
	
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		xinfaId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnAddXinFa")

		BOOL bRet = ((Obj_Human*)pCharacter)->Skill_SetXinFaLevel(xinfaId,1);
		Lua_PushNumber(L,bRet);
		if(bRet)
		{
			Obj_Human* pHuman	=	(Obj_Human*)pCharacter;
			Assert(pHuman);
			
				XINFA_LOG_PARAM XinFaLogParam;
				XinFaLogParam.CharGUID	=	pHuman->GetGUID();
				XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
				XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				XinFaLogParam.OPType	=	XINFA_OP_SCRIPT_STUDY;
				XinFaLogParam.Count		=	pHuman->Skill_GetXinFaList().m_Count;
				XinFaLogParam.XinFaID	=	xinfaId;
				XinFaLogParam.Level		=	1;
				SaveXinFaLog(&XinFaLogParam);
		}
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号
	*     功能说明 :
	*				给当前玩家加某个技能
	*				没有测试
	*/
	INT LuaFnAddSkill( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT skillId;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnAddSkill")	
		BOOL bRet = ((Obj_Human*)pCharacter)->Skill_ModifySkill(skillId,1);
		Lua_PushNumber(L,bRet);
		
		if(bRet)
		{
			Obj_Human* pHuman = (Obj_Human*)pCharacter;
			Assert(pHuman);

			SKILL_LOG_PARAM SkillLogParam;
			SkillLogParam.CharGUID	=	pHuman->GetGUID();
			SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
			SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SkillLogParam.OPType	=	SKILL_OP_SCRIPT_STUDY;
			SkillLogParam.Count		=	pHuman->Skill_GetSkillList().m_Count;
			SkillLogParam.SkillID	=	skillId;
			SkillLogParam.Level		=	1;

			SaveSkillLog(&SkillLogParam);
		}

		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDelXinFa
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				xinfaId
	*					心法编号
	*     功能说明 :
	*				删除当前玩家的某个心法
	*				没有测试
	*/
	INT LuaFnDelXinFa( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT xinfaId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		xinfaId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnDelXinFa")	

			BOOL bRet  =((Obj_Human*)pCharacter)->Skill_SetXinFaLevel(xinfaId,-1);	
			Lua_PushNumber(L,bRet);
			if(bRet)
			{
				Obj_Human* pHuman	=	(Obj_Human*)pCharacter;
				Assert(pHuman);
				XINFA_LOG_PARAM XinFaLogParam;
				XinFaLogParam.CharGUID	=	pHuman->GetGUID();
				XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
				XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				XinFaLogParam.OPType	=	XINFA_OP_SCRIPT_ABANDON;
				XinFaLogParam.Count		=	pHuman->Skill_GetXinFaList().m_Count;
				XinFaLogParam.XinFaID	=	xinfaId;
				XinFaLogParam.Level		=	-1;
				SaveXinFaLog(&XinFaLogParam);
			}

			return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDelSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号				
	*     功能说明 :
	*				删除当前玩家的某个技能
	*				没有测试
	*/
	INT LuaFnDelSkill( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT skillId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnDelSkill")
		
		BOOL bRet = ((Obj_Human*)pCharacter)->Skill_ModifySkill(skillId,-1);
		Lua_PushNumber(L,bRet);
		
		if(bRet)
		{
			Obj_Human* pHuman = (Obj_Human*)pCharacter;
			Assert(pHuman);

			SKILL_LOG_PARAM SkillLogParam;
			SkillLogParam.CharGUID	=	pHuman->GetGUID();
			SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
			SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SkillLogParam.OPType	=	SKILL_OP_SCRIPT_STUDY;
			SkillLogParam.Count		=	pHuman->Skill_GetSkillList().m_Count;
			SkillLogParam.SkillID	=	skillId;
			SkillLogParam.Level		=	-1;
			SaveSkillLog(&SkillLogParam);
		}

		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnHaveXinFa
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				xinfaId
	*					心法编号				
	*     功能说明 :
	*				判断当前玩家是否有某个心法。未学过该技能返回-1，否则返回心法等级
	*				没有测试
	*/
	INT LuaFnHaveXinFa( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT xinfaId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		xinfaId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnHaveXinFa")
		if(FALSE==((Obj_Human*)pCharacter)->Skill_GetXinFaLevel(xinfaId))
		{
			Lua_PushNumber(L,((Obj_Human*)pCharacter)->Skill_GetXinFaLevel(xinfaId));//gaoqi: 这里是不是有问题?
			return	1;
		}
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnHaveSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号				
	*     功能说明 :
	*				判断当前玩家是否有某个技能。未学过该技能返回-1，否则返回技能等级。//gaoqi? 技能没有等级
	*/
	INT LuaFnHaveSkill( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT skillId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnHaveSkill")
		if(pCharacter->Skill_HaveSkill(skillId))
		{
			Lua_PushNumber(L,1);//gaoqi: 这里是不是有问题?
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnUseSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号				
	*     功能说明 :
	*				有待与策划商榷
	*/
	INT LuaFnUseSkill( Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		INT skillId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnUseSkill")
			AssertEx(FALSE,"此函数有问题") ;
			if(1)
			{
				Lua_PushNumber(L,1);//gaoqi: 这里是不是有问题?
				return 1;
			}
			ENDCHARCTERDEFINE
				RETURNFALSE

	LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 五勇鸷
	*     编写时间 : 2008-3-20
	*     函数名称 : LuaFnGetCurCountry
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的当前国家
	*				没有测试
	*/
	INT LuaFnGetCurCountry( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetCurCountry")
			Lua_PushNumber(L,pHuman->GetCountry());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}



	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetCurCamp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的当前阵营
	*				没有测试
	*/
	INT LuaFnGetCurCamp( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetCurCamp")
		Lua_PushNumber(L,pHuman->GetCampID());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetCurCamp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				campIndex
	*					阵营编号,INT类型
	*     功能说明 :
	*				修改玩家阵营,函数体尚未填写
	*/
	INT LuaFnSetCurCamp ( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT campIndex ;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		campIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetCurCamp")
		pHuman->SetBaseCampID(campIndex);//高骐：杨飞找我聊一聊
		return 0;
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-3-10	19:42
	 *	函数名称：	LuaFnGetGUID
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到调用者的 GUID
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetGUID( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetGUID")	
		Lua_PushNumber(L, ((Obj_Human*)pCharacter)->GetGUID());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetExp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的当前战斗经验值
	*/
	INT LuaFnGetExp( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetExp")	
		Lua_PushNumber(L, ((Obj_Human*)pCharacter)->GetExp());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddExp
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				exp
	*					经验值增加,INT
	*     功能说明 :
	*				增加玩家的战斗经验值
	*/
	INT LuaFnAddExp( Lua_State* L)	
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT exp;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		exp = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnAddExp")
		pHuman->SetAlterExp( exp ) ;
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetHp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的生命值
	*				返回玩家的HP值
	*/
	INT LuaFnGetHp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetHp")	
		Lua_PushNumber(L, pCharacter->GetHP());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE
		
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnRestoreHp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*		
	*				percent
	*					恢复的百分比,为0时走原来的逻辑，非0时直接SetMP
	*     功能说明 :
	*				恢复角色的生命全满
	*				把角色的生命值设为MaxHP*percent/100值
	*/
	INT LuaFnRestoreHp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT percent;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		percent	= Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnRestoreHp")

		// 20100414 AddCodeBegin 满血时不需要恢复，直接返回，返回后给出不需要恢复的提示
		if ( pCharacter->GetHP() == pCharacter->GetMaxHP() )
		{
			return 1;
		}
		// 20100414 AddCodeEnd

		if ( percent == 0 )
			pCharacter->HealthIncrement(pCharacter->GetMaxHP());
		else
		{
			INT Increment = ( pCharacter->GetMaxHP()*percent ) / 100;
			pCharacter->HealthIncrement( Increment-pCharacter->GetHP() );
		}

		return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得角色的内力值
	*				返回角色的MP值
	*/
	INT LuaFnGetMp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetMp")
		Lua_PushNumber(L, pCharacter->GetMP());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnRestoreMp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				percent
	*					恢复的百分比
	*     功能说明 :
	*				恢复玩家的内力为MaxMP*percent/100
	*/
	INT LuaFnRestoreMp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT percent;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		percent =  Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnRestoreMp")
		
		// 20100414 AddCodeBegin 满魔时不需要恢复，直接返回，返回后给出不需要恢复的提示
		if ( pCharacter->GetMP() == pCharacter->GetMaxHP() )
		{
			return 1;
		}
		// 20100414 AddCodeEnd

		if ( percent == 0 )
			percent = 100;
		pHuman->SetMP( (pHuman->GetMaxMP() *percent)/100 );

		GCDetailHealsAndDamages msg;
		msg.SetReceiverID(pHuman->GetID());
		msg.SetSenderID(INVALID_ID);
		msg.SetSenderLogicCount(0);

		INT Increment = ( pCharacter->GetMaxMP()*percent ) / 100;
		msg.SetMpModification(Increment-pCharacter->GetMP());
		pHuman->GetPlayer()->SendPacket(&msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetCon
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的体力值
	*/
	INT LuaFnGetCon(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetCon")
		Lua_PushNumber(L,pHuman->GetCon());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetSex
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的性别
	*				返回值为0：女
	*				返回值为1：男
	*/
	INT LuaFnGetSex(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetSex")
		Lua_PushNumber(L,pHuman->GetSex());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetName
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得角色(Obj_Character)名字，返回一个字符串
	*				角色包括玩家和Obj_Monster
	*				示例: npcName = GetName(sceneId,npcId)
	*/
	INT LuaFnGetName(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		
		BEGINCHARCTERDEFINE("LuaFnGetName")
		Lua_PushString(L,pCharacter->GetName());
		return 1;
		ENDCHARCTERDEFINE
		Lua_PushString(L,"");
		return 1;
		
		LUA_LEAVE_FUNCTION

		Lua_PushString(L,"");
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMoney
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的现金
	*				返回玩家拥有的金钱数
	*/
	INT LuaFnGetMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMoney")
		Lua_PushNumber(L,pHuman->GetMoney());	
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnCostMoney
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				payment
	*					金钱消耗
	*     功能说明 :
	*				扣除玩家金钱
	*				成功返回1
	*				以后是否考虑返回修改后的玩家金钱数?
	*				目前payment应该为正值
	*				失败返回-1
	*/
	INT LuaFnCostMoney(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT payment;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		payment = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnCostMoney")
		if( pHuman->GetMoney() >= payment)
		{
			pHuman->SetMoney( pHuman->GetMoney() - payment);
			

			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.OPType	=	MONEY_SCRIPT_COST;	
			MoneyLogParam.Count		=	payment;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);

			Lua_PushNumber(L,1);	
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}				

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddMoney
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				profit
	*					金钱增加数目
	*     功能说明 :
	*				增加玩家金钱
	*				返回1,成功
	*				以后是否考虑返回修改后的玩家金钱数?
	*				返回-1,失败
	*/
	INT LuaFnAddMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT profit;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		profit = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnAddMoney")

		//处理“金钱获得”道具的影响
		_ITEM_EFFECT* pIE = pHuman->ItemEffect(IATTRIBUTE_MONEY_GET);
		Assert(pIE);
		if(pIE->IsActive())
		{
			profit = Float2Int(profit * (100 + pIE->m_Attr.m_Value) /100.0f);
		}

		pHuman->SetMoney( pHuman->GetMoney() + profit);

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_SCRIPT_COST;	
		MoneyLogParam.Count		=	profit;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		Lua_PushNumber(L,1);	
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetLevel
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				取得玩家当前等级
	*				Level的合法范围在?-?
	*				返回-1,失败
	*/
	INT LuaFnGetLevel(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetLevel")	
		Lua_PushNumber(L,pCharacter->GetLevel());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	

	/** -----------------------------------------------------------------------
	*     开发人员 : 　
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnSetLevel
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				level
	*					角色级别,INT
	*     功能说明 :
	*				设置玩家玩家等级
	*				Level的合法范围在1-60
	*				返回-1,失败
	*/
	INT LuaFnSetLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT level;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		level = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetLevel")	
		
		AssertEx(level > 0 && level <= DEFAULT_WASHPOINT_LEVEL,"level wrong");
		if( level <=0 || level > DEFAULT_WASHPOINT_LEVEL)
		{
			RETURNFALSE
		}
		
		pHuman->SetLevel(level);

		Lua_PushNumber(L,1);
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-3-7	16:36
	 *	函数名称：	LuaFnCanLevelUp
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	判断该玩家是否可以升级
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnCanLevelUp(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnCanLevelUp")
			if( pHuman->GetExp() >= (UINT)g_LevelExpTbl.Get(pHuman->GetLevel() - 1) )
			{
				Lua_PushNumber(L, TRUE);
			}
			else
			{
				Lua_PushNumber(L, FALSE);
			}

			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-28
	*     函数名称 : LuaFnGetItemTableIndexByIndex
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemIndex
	*					物品在身上的位置索引
	*     功能说明 :
	*				取当前位置的物品种类
	*				
	*/
	INT LuaFnGetItemTableIndexByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT itemIndex;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetItemTableIndexByIndex")
			
		Item*	pItem = 	HumanItemLogic::GetBagItem(pHuman, itemIndex);
		INT itemType = pItem->GetItemTableIndex();

		Lua_PushNumber(L, itemType);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-3-30	14:47
	 *	函数名称：	LuaFnGetMaterialStartBagPos
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到背包中材料的起始位置
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetMaterialStartBagPos(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		/*sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetMaterialStartBagPos")
			Lua_PushNumber(L, MAT_CONTAINER_OFFSET);
			return 1;
		ENDHUMANDEFINE*/
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-3-30	14:47
	 *	函数名称：	LuaFnGetMaterialEndBagPos
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到背包中材料的结束位置
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetMaterialEndBagPos(Lua_State* L)	
	{
	/*LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetMaterialEndBagPos")
			Lua_PushNumber(L, TASK_CONTAINER_OFFSET-1);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION*/

		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-3-30	19:30
	 *	函数名称：	LuaFnGetItemCountInBagPos
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				uBagPos
	 *					背包中位置
	 *
	 *	功能说明：	得到背包某格中物品的数量
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetItemCountInBagPos(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT uBagPos;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		uBagPos = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnGetItemCountInBagPos")

			Item* pItem = HumanItemLogic::GetBagItem(pHuman, uBagPos);
			if (pItem != NULL)
			{
				Lua_PushNumber(L, pItem->GetLayedNum());
				return 1;
			}
			return 0;

		ENDHUMANDEFINE
			RETURNFALSE

			LUA_LEAVE_FUNCTION

			RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*     功能说明 :
	*				判断当前玩家有多少个道具
	*				只从玩家的装备和物品包中查找
	*/
	INT LuaFnGetItemCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT itemTypeSn;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemTypeSn = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetItemCount")
		INT count =0;
				
		count = HumanItemLogic::CalcEquipItemCount(pHuman,itemTypeSn);
		
		count += HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn);

		Lua_PushNumber(L,count);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}















	/** -----------------------------------------------------------------------
	*     开发人员 : 王能鸷
	*     编写时间 : 2008-4-22
	*     函数名称 : LuaFnGetPKValue
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				得到玩家当前PK值
	*/
	INT LuaFnGetPKValue(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetPKValue")
		Lua_PushNumber(L,pHuman->GetPKValue());
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}




	/** -----------------------------------------------------------------------
	*     开发人员 : 王能鸷
	*     编写时间 : 2008-4-22
	*     函数名称 : LuaFnSetPKValue
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				Val:
	*					玩家新的PK值
	*     功能说明 :
	*				设置玩家当前PK值
	*/
	INT LuaFnSetPKValue(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT Val;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		Val    = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetPKValue")
		
		pHuman->SetPKValue(Val);

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0;
	}















	/** -----------------------------------------------------------------------
	*     开发人员 : yf
	*     编写时间 : 2005-1-22
	*     函数名称 : LuaFnHaveItemInBag
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*     功能说明 :
	*				判断当前玩家包中是否拥有某个道具
	*				如果成功,返回大于0
	*				如果失败,返回-1
	*/
	INT LuaFnHaveItemInBag ( Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT itemTypeSn;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemTypeSn = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnHaveItemInBag")
		if(HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn))
		{
			Lua_PushNumber(L,1);
			return 1;			
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;			
		}

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnHaveItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*     功能说明 :
	*				判断当前玩家是否拥有某个道具
	*				如果成功,返回大于0
	*				如果失败,返回-1
	*/
	INT LuaFnHaveItem ( Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		INT itemTypeSn;
	
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemTypeSn = Lua_ValueToNumber(L,3);
		
		BEGINHUMANDEFINE("LuaFnHaveItem")
		//INT count = 0;
	
		//先从装备栏查找
		for(INT i=0;i<HEQUIP_NUMBER;i++)
		{
			if( pHuman->GetDB()->GetEquipItem((HUMAN_EQUIP)i)->m_ItemIndex == itemTypeSn)
			{
				Lua_PushNumber(L,1);
				return 1;	
			}
		}
	
		if(HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn))
		{
			Lua_PushNumber(L,1);
			return 1;			
		}
		
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2008-4-28
	*     函数名称 : LuaFnUseItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*     功能说明 :
	*				玩家使用某个道具
	*				如果成功,返回大于0
	*				如果失败,返回-1
	*/
	INT LuaFnUseItem ( Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT itemTypeSn;

			sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemTypeSn = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnUseItem")
			//INT count = 0;
		if (GetSerialClass(itemTypeSn) == ICLASS_EQUIP)
		{
			INT BagIndex = HumanItemLogic::GetBagItemPosByType(pHuman, itemTypeSn);
			if (BagIndex == INVALID_INDEX)
			{
				Lua_PushNumber(L,0);
				return 1;
			}
			
			Item* pUseItem = HumanItemLogic::GetBagItem(pHuman, BagIndex);
			Assert(pUseItem);
			if (pUseItem->IsEmpty())
			{
				Lua_PushNumber(L,0);
				return 1;
			}

			INT nPos = pHuman->UseEquip(BagIndex);
			if (nPos == INVALID_INDEX)
			{
				Lua_PushNumber(L,0);
				return 1;
			}
		}
			
		ENDHUMANDEFINE
			RETURNFALSE

			LUA_LEAVE_FUNCTION
			RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDelItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*				delCount
	*					删除数目
	*     功能说明 :
	*				删除当前玩家的某个道具	
	*				不仅是指任务道具	
	*				不对已装备的道具删除
	*				删除成功,返回1
	*				删除失败,返回-1
	*/
	INT LuaFnDelItem ( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT itemIndex;
		INT delCount;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		itemIndex = Lua_ValueToNumber(L,3);
		delCount = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDelItem")
		//TSerialHelper help(itemIndex);
		//_ITEM_TYPE	itemType =	help.GetItemTypeStruct();

		//pHuman->EraseItem(itemType,delCount);
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	=	ITEM_SCRIPT_DELETE;
		if( HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,itemIndex,delCount)==TRUE )
		{
			Lua_PushNumber(L,1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnEraseItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				bagIndex
	*					背包的格子
	*     功能说明 :
	*				删除当前玩家背包某一格的道具
	*				删除成功,返回1
	*				删除失败,返回-1
	*/
	INT LuaFnEraseItem ( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		uint bagIndex;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		bagIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnEraseItem")
		
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	=	ITEM_SCRIPT_DELETE;
		if( HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman,bagIndex) == TRUE )
		{
			SaveItemLog(&ItemLogParam);
			Lua_PushNumber(L,1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMenPai
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获取玩家当前所属门派ID
	*/
	INT LuaFnGetMenPai(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMenPai")
		Lua_PushNumber(L,pHuman->GetMenPai());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-13
	*     函数名称 : LuaFnSetMenPai
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				menpaiId
	*					门派ID,INT
	*     功能说明 :
	*				设置玩家当前所属门派为menpaiId
	*/
	INT LuaFnSetMenPai(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT menpaiId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		menpaiId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetMenPai")
		
		AssertEx(menpaiId < MATTRIBUTE_NUMBER && menpaiId >= MATTRIBUTE_SHAOLIN_GWS ,"门派设置越界");
		if ( menpaiId >= MATTRIBUTE_NUMBER && menpaiId < MATTRIBUTE_SHAOLIN_GWS)
		{
			RETURNFALSE
		}

		pHuman->SetMenPai(menpaiId);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : yangei
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMenPaiMaster
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获取玩家所在帮会的帮主名称（字符串）
	*				函数体没有实现
	*/
	INT LuaFnGetMenPaiMaster(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMenPaiMaster")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMenPaiFigure
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				返回玩家在帮会中的阶级：0帮众，1队长，2长老，4帮主
	*				函数体没有实现
	*/
	INT LuaFnGetMenPaiFigure(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMenPaiFigure")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMenPaiTitle
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				返回玩家在帮会中的头衔，返回头衔中不包括帮会名
	*				函数体没有实现
	*/
	INT LuaFnGetMenPaiTitle(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMenPaiTitle")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDestroyMenPai
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				menpaiId
	*					帮会编号
	*     功能说明 :
	*				解散帮会
	*				函数体没有实现
	*/
	INT LuaFnDestroyMenPai(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT menpaiId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		menpaiId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDestroyMenPai")
		//INT value = pHuman->GetTask(misId); 
		//Lua_PushNumber(L,value);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetNpcCurCamp
	*     参数说明 : 
	*     功能说明 :
	*				改变NPC的阵营,函数体尚未填写
	*/
	INT LuaFnSetNpcCurCamp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		INT npcIndex;
		INT campId ;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnRestoreNpcCamp
	*     参数说明 : 
	*     功能说明 :
	*				恢复NPC原有的阵营,函数体尚未填写
	*/
	INT LuaFnRestoreNpcCamp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		INT npcindex;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-1-23
	*     函数名称 : LuaFnSetDamage
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					自己的ObjID
	*				targetId
	*					目标的ObjID
	*				damage
	*					需要设置的伤害
	*     功能说明 :
	*				设置某个obj的伤害
	*/
	INT LuaFnSetDamage(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		INT damage;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);
		damage = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetDamage")
			Obj* pTarObj = pScene->GetObjManager()->GetObj(targetId) ;
			if( pTarObj && IsCharacterObj(pTarObj->GetObjType()) )
			{
				Obj_Character* pTarChar = (Obj_Character*)pTarObj ;	
				pTarChar->HealthIncrement(-damage, INVALID_ID, pHuman);
			}
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : gaoqi
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGmKillObj
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					自己的ObjID
	*				targetId
	*					目标的ObjID
	*     功能说明 :
	*				直接杀死指定的obj;
	*/
	INT LuaFnGmKillObj(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGmKillObj")
			Obj* pTarObj = pScene->GetObjManager()->GetObj(targetId) ;
			if( pTarObj && IsCharacterObj(pTarObj->GetObjType()) )
			{
				Obj_Character* pTarChar = (Obj_Character*)pTarObj ;	
				pTarChar->GM_ForceDie(pHuman);
			}
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-1-23
	*     函数名称 : LuaFnFindMonsterByGUID
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				guid
	*					需要寻找的怪物GUID
	*     功能说明 :
	*				寻找场景中的某个怪物,根据GUID
	*/
	INT LuaFnFindMonsterByGUID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		GUID_t guid;

		sceneId = Lua_ValueToNumber(L,1);
		guid = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnFindMonsterByGUID")
			ObjID_t objid = pScene->GetMonsterManager()->FindMonsterByGUID(guid) ;
			Lua_PushNumber(L,objid);
		return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanMaxVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家活力最大值
	*/
	INT LuaFnGetHumanMaxVigor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanMaxVigor")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetMaxVigor());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanMaxEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家精力最大值
	*/
	INT LuaFnGetHumanMaxEnergy(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanMaxEnergy")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetMaxEnergy());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家活力值
	*/
	INT LuaFnGetHumanVigor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanVigor")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetVigor());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家精力值
	*/
	INT LuaFnGetHumanEnergy(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanEnergy")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetEnergy());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanMaxVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家活力最大值
	*/
	INT LuaFnSetHumanMaxVigor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanMaxVigor")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetMaxVigor(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanMaxEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家精力最大值
	*/
	INT LuaFnSetHumanMaxEnergy(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanMaxEnergy")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetMaxEnergy(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家活力
	*/
	INT LuaFnSetHumanVigor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanVigor")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetVigor(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家精力
	*/
	INT LuaFnSetHumanEnergy(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanEnergy")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetEnergy(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanVigorRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家活力恢复速度
	*/
	INT LuaFnGetHumanVigorRegeneRate(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanVigorRegeneRate")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetVigorRegeneRate());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanEnergyRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家精力恢复速度
	*/
	INT LuaFnGetHumanEnergyRegeneRate(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHumanEnergyRegeneRate")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			Lua_PushNumber(L,pHuman->GetEnergyRegeneRate());
			return 1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanVigorRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家活力恢复速度
	*/
	INT LuaFnSetHumanVigorRegeneRate(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanVigorRegeneRate")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetVigorRegeneRate(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanEnergyRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家精力恢复速度
	*/
	INT LuaFnSetHumanEnergyRegeneRate(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t humanId;
		INT value;

		sceneId = Lua_ValueToNumber(L,1);
		humanId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnSetHumanEnergyRegeneRate")
		Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
		if ( pHuman != NULL )
		{
			pHuman->SetEnergyRegeneRate(value);
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanHairColor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家头发颜色
	*/
	INT LuaFnSetHumanHairColor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT value;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		value	= Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetHumanHairColor")
			pHuman->__SetHairColor(value);
			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanHairColor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家头发颜色
	*/
	INT LuaFnGetHumanHairColor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetHumanHairColor")
			UINT uValue = pHuman->__GetHairColor();
			Lua_PushNumber(L,uValue);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanHairModel
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				uHairStyle
	*					设值的值
	*     功能说明 :
	*				设置玩家头发模型
	*/
	INT LuaFnSetHumanHairModel(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT uHairStyle;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		uHairStyle = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnSetHumanHairModel")
			pHuman->__SetHairModel(uHairStyle);
			Lua_PushNumber(L,1);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-5-15	14:36
	 *	函数名称：	LuaFnChangeHumanHairModel
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				uHairStyle
	 *					发型值
	 *
	 *	功能说明：	将玩家的发型换成 uHairStyle
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnChangeHumanHairModel(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT uHairStyle;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		uHairStyle = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnChangeHumanHairModel")
			if ( uHairStyle >= g_HairStyleTbl.m_uCount )
			{
				Assert( uHairStyle < g_HairStyleTbl.m_uCount );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			_HAIR_STYLE_TYPE_* pHairStyleType;
			pHairStyleType = &(g_HairStyleTbl.m_HairStyle[uHairStyle]);
			if ( pHairStyleType->m_nIndex != uHairStyle )
			{
				AssertEx( pHairStyleType->m_nIndex == uHairStyle, "Invalid Hair Style, Please Check the Table!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			if ( pHairStyleType->m_nUseScope < 2 )
			{
				AssertEx( pHairStyleType->m_nUseScope >= 2, "This Hair Style Can't be Used in this Scenario!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			if ( pHairStyleType->m_nRaceID != pHuman->GetSex() )
			{
				AssertEx( pHairStyleType->m_nRaceID == pHuman->GetSex(), "This Hair Style is NOT Suit this Player's Gender!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			INT nCostCount;
			nCostCount = pHairStyleType->m_nItemCostCount;
			if ( nCostCount > 0 )
			{
				if ( nCostCount > HumanItemLogic::CalcBagItemCount(pHuman, pHairStyleType->m_uItemSerial) )
				{
					Lua_PushNumber(L, FALSE);
					return 1;
				}
				
				ITEM_LOG_PARAM ItemLogParam;
				ItemLogParam.OpType	= ITEM_SCRIPT_DELETE;
				HumanItemLogic::EraseBagItem(&ItemLogParam, pHuman, pHairStyleType->m_uItemSerial, nCostCount);
			}

			pHuman->__SetHairModel(uHairStyle);
			Lua_PushNumber(L, TRUE);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanHairModel
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家头发模型
	*/
	INT LuaFnGetHumanHairModel(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetHumanHairModel")
			UINT uValue = pHuman->__GetHairModel();
			Lua_PushNumber(L,uValue);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-5-15	14:26
	 *	函数名称：	LuaFnGetHumanGoodBadValue
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到 selfId 的善恶值
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnGetHumanGoodBadValue(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetHumanGoodBadValue")
			INT nValue = pHuman->GetGoodBadValue();
			Lua_PushNumber(L, nValue);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-5-15	14:26
	 *	函数名称：	LuaFnSetHumanGoodBadValue
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				value
	 *					善恶值
	 *
	 *	功能说明：	设置 selfId 的善恶值
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnSetHumanGoodBadValue(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
	
	  SceneID_t sceneId;
	  ObjID_t selfId;
	  INT value;

		sceneId = Lua_ValueToNumber(L, 1);
		selfId = Lua_ValueToNumber(L, 2);
		value = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnSetHumanGoodBadValue")
			pHuman->SetGoodBadValue(value);
			return 0;
		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

    
    /****************************************************************************
    *	开发人员：	amos
    *	编写时间：	2008-6-23	14:26
    *	函数名称：	LuaFnSetExchangStatus
    *	参数说明：
    *				sceneId
    *					场景编号
    *				selfId
    *					调用者 ID，无符号整型
    *				
    *					
    *
    *	功能说明：	清空交易状态
    *	修改记录：
    *****************************************************************************/
    INT LuaFnSetExchangStatus(Lua_State* L)	
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;

        sceneId = Lua_ValueToNumber(L, 1);
        selfId = Lua_ValueToNumber(L, 2);

        BEGINHUMANDEFINE("LuaFnSetExchangStatus")
            pHuman->m_ExchangBox.CleanUp();
        return 0;
        ENDHUMANDEFINE

            LUA_LEAVE_FUNCTION

            return 0;
    }


}

#endif
