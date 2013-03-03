//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Mission.h
// 模块	:	Script
// 功能	 :	任务相关导出函数
// 修改历史:2008-2-1 王勇鸷
//	yangfei创建
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_MISSION_H__
#define __LUAFNTBL_MISSION_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCScriptCommand.h"
#include "SceneManager.h"
#include "ItemHelper.h"
#include "ItemManager.h"
#include "HumanItemLogic.h"
#include "MonsterManager.h"
#include "Obj_Monster.h"
#include "GCMissionResult.h"
#include "GCUICommand.h"
#include "GameTable.h"
#include "LogDefine.h"
#include "DayTips.h"


namespace LuaFnTbl
{


	enum
	{
		EA_Number,
		EA_State,
		EA_Index,
		EA_Count
	};

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnBeginEvent
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				和EndEvent配合使用
	*				为下一步的相关操作做准备
	*/
	INT LuaFnBeginEvent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;

		sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginEvent")
		pScene->mMisBuf.ResetMisBuf();

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0;
	}

	
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				msg
	*					提示内容
	*     功能说明 :
	*				添加字符串提示
	*/
	INT LuaFnAddText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* msg;

		sceneId = Lua_ValueToNumber(L,1);		
		msg = Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnAddText")
		//size_t len = min(_MisBuf::MISSIONCHARBUFLEN -1,strlen(msg));	
		//strncpy(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf,msg,len);
		Q_strncpyz(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf,msg,_MisBuf::MISSIONCHARBUFLEN);//优化
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Str;		

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddNumber
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				id
	*					数目,INT
	*     功能说明 :
	*				添加数目提示
	*/
	INT LuaFnAddNumber(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT id;

		sceneId = Lua_ValueToNumber(L,1);
		id = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnAddNumber")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber = id;
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Int;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddNumText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				id
	*					第几个提示,INT
	*				msg
	*					提示内容
	*				state 
	*					是否可以交任务和接任务
	*				index
	*					策划用整形数
	*     功能说明 :
	*				添加数目和字符串提示
	*				state和index可以不添
	*/
	INT LuaFnAddNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		INT id;
		const CHAR* msg;
		INT state;
		INT index;

		sceneId = Lua_ValueToNumber(L,1);
		id = Lua_ValueToNumber(L,2);
		msg = Lua_ValueToString(L,3);
		
		//
		if(Lua_GetTopIndex(L) < 4)
			state = -1;//可以接的任务
		else
			state = Lua_ValueToNumber(L,4);
		
		if(Lua_GetTopIndex(L) < 5)
			index = -1;//可以接的任务
		else
			index = Lua_ValueToNumber(L,5);

		BEGINSCENEDEFINE("LuaFnAddNumText")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Number * sizeof(INT));
		*pNumber = id;
		INT* pState  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_State * sizeof(INT));
		*pState = state;
		INT* pIndex  = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Index * sizeof(INT));
		*pIndex = index;
		
		//size_t len = min(_MisBuf::MISSIONCHARBUFLEN -1,strlen(msg));	
		//strncpy(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + sizeof(INT),msg,strlen(msg));
		Q_strncpyz(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf + EA_Count * sizeof(INT),msg,_MisBuf::MISSIONCHARBUFLEN - EA_Count * sizeof(INT));
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_IntStr;	

		return 0;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : yf
	*     编写时间 : 2005-12-9
	*     函数名称 : LuaFnSetNumText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				number
	*					待设置的值,short
	*     功能说明 :
	*				设置脚本交换用数据
	*/
	INT LuaFnSetNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		INT number;

		number = Lua_ValueToNumber(L,1);
		lua_setglobal(L,"global_numtext");

		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : yf
	*     编写时间 : 2005-12-9
	*     函数名称 : LuaFnGetNumText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				number
	*					待设置的值,short
	*     功能说明 :
	*				取出脚本交换用数据
	*/
	INT LuaFnGetNumText(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		lua_getglobal(L,"global_numtext");
		INT number = Lua_ValueToNumber(L,1);
		Lua_PushNumber(L,number);
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 :五勇鸷
	*     编写时间 : 2008-1-31
	*     函数名称 : LuaFnGetOneMissionInfo
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				nPhase
	*					任务所处阶段
	*				MissionType
	*					任务类型 0：杀怪  1：寻物  2：送信	
	*				MisIndex
	*					任务索引 为-1时需要随机产生，不为-1时MisIndex为上一次随机数
	*     功能说明 :
	*				得到循环任务中目的NPC或ITEM的信息：ID、NAME、COUNT、所在场景名与ID、位置、描述
	*				得到循环任务索引，需要传出给脚本使用
	*/

	INT LuaFnGetOneMissionInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		INT nPhase;
		INT MissionType;
		INT MisIndex;

		nPhase = Lua_ValueToNumber(L,1);
		MissionType = Lua_ValueToNumber(L,2);
		MisIndex = Lua_ValueToNumber(L,3);

		if ( MisIndex == -1 )
		{
			MisIndex = rand()%g_XunHuanMissionAttrTbl.m_SubMissionInfo[nPhase][MissionType].m_SubMissionCnt ;
			AssertEx(MisIndex>=0 && MisIndex<g_XunHuanMissionAttrTbl.m_SubMissionInfo[nPhase][MissionType].m_SubMissionCnt,"子任务ID出错 ");
			MisIndex += g_XunHuanMissionAttrTbl.m_SubMissionInfo[nPhase][MissionType].M_SubMissionPos;	
		}

		Lua_PushNumber(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_ObjId) ;	
		Lua_PushString(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_ObjName);
		Lua_PushNumber(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_ObjCount) ;	

		Lua_PushString(L, g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_SceneName);
		Lua_PushNumber(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_ScendId);
		Lua_PushNumber(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_PosX);			//位置
		Lua_PushNumber(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_PosZ);			//位置
		Lua_PushString(L,g_XunHuanMissionAttrTbl.m_TableExt[nPhase][MisIndex].m_DialogText);
		Lua_PushNumber(L,MisIndex);
		return 9;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}



	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddMoneyBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				count
	*					奖励金钱数目
	*     功能说明 :
	*				添加Money
	*/
	INT LuaFnAddMoneyBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT count;

		sceneId = Lua_ValueToNumber(L,1);
		count = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnAddMoneyBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber = count;
		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Money;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItemBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					奖励物品的ItemType的9位编号
	*				count
	*					奖励数目
	*     功能说明 :
	*				添加count个编号为itemType道具
	*/
	INT LuaFnAddItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT itemTypeSn;
		INT count;
			
		sceneId = Lua_ValueToNumber(L,1);
		itemTypeSn = Lua_ValueToNumber(L,2);
		count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItemBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		*pNumber++ = itemTypeSn;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Item;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItemDemand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					需求物品的ItemType的9位编号
	*				count
	*					需求数目

	*     功能说明 :
	*				需求count个编号为itemType道具
	*/
	INT LuaFnAddItemDemand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT itemTypeSn;
		INT count;
			
		sceneId = Lua_ValueToNumber(L,1);;
		itemTypeSn = Lua_ValueToNumber(L,2);
		count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItemDemand")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);
		
		*pNumber++ = itemTypeSn ;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_Item;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddRandItemBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*					提示奖励随机道具
	*/
	INT LuaFnAddRandItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;

		sceneId = Lua_ValueToNumber(L,1);
			
		BEGINSCENEDEFINE("LuaFnAddRandItemBonus")

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_RandItem;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddRadioItemBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					物品的ItemType9位编号
	*				count
	*					奖励多选一物品的数目
	*     功能说明 :
	*				添加多选一道具
	*/
	INT LuaFnAddRadioItemBonus(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT itemTypeSn;
		INT count;
		
		sceneId = Lua_ValueToNumber(L,1);
		itemTypeSn = Lua_ValueToNumber(L,2);
		count = Lua_ValueToNumber(L,3);
	
		BEGINSCENEDEFINE("LuaFnAddRadioItemBonus")
		INT* pNumber = (INT*)(pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum].buf);

		*pNumber++ = itemTypeSn;
		*pNumber = count;

		pScene->mMisBuf.Buff[pScene->mMisBuf.mUseNum++].mType = _MisBuf::_MisItemBuf::BufType_RadioItem;

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnEndEvent
	*     参数说明 : 
	*     功能说明 :
	*				和BeginEvent配合使用
	*				之后需调用某个Dispatch***函数
	*/
	INT LuaFnEndEvent(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
			return 0;

		LUA_LEAVE_FUNCTION
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchEventList
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*     功能说明 :
	*				向客户端返回该Npc可触发事件列表
	*				可触发事件由该npc的挂的事件OnEnumerate函数判断
	*				如果玩家满足任务条件,通过AddNumText添加
	*				BeginEvent
	*				AddText
	*				可能: AddNumText
	*				EndEvent
	*/
	INT LuaFnDispatchEventList(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchEventList")
		ScriptParam_EventList paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptEventItem itemEvent;
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				itemEvent.SetText( &scriptStr);
				paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Int)
			{
				//ScriptString scriptStr;
				//scriptStr = pScene->mMisBuf.Buff[i].buf;
				//INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				//ScriptEventItem itemEvent;
				//itemEvent.SetScriptID(*number,&scriptStr);
				//paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_IntStr)
			{
				INT number = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Number * sizeof(INT));
				INT state = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_State * sizeof(INT));
				INT index = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Index * sizeof(INT));
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf + EA_Count * sizeof(INT);
				ScriptEventItem itemEvent;
				itemEvent.SetScriptID(index,state,number,&scriptStr);
				paramEventList.AddItem( &itemEvent );
			}

		}

		Packets::GCScriptCommand Msg;
		Msg.SetEventListResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchRegieEventList
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*     功能说明 :
	*				向客户端返回该Npc可触发事件列表
	*				可触发事件由该npc的挂的事件OnEnumerate函数判断
	*				如果玩家满足任务条件,通过AddNumText添加
	*				BeginEvent
	*				AddText
	*				可能: AddNumText
	*				EndEvent
	*/
	INT LuaFnDispatchRegieEventList(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchRegieEventList")
		ScriptParam_EventList paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptEventItem itemEvent;
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				itemEvent.SetText( &scriptStr);
				paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Int)
			{
				//ScriptString scriptStr;
				//scriptStr = pScene->mMisBuf.Buff[i].buf;
				//INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				//ScriptEventItem itemEvent;
				//itemEvent.SetScriptID(*number,&scriptStr);
				//paramEventList.AddItem( &itemEvent );
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_IntStr)
			{
				INT number = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Number * sizeof(INT));
				INT state = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_State * sizeof(INT));
				INT index = *(INT*)(pScene->mMisBuf.Buff[i].buf + EA_Index * sizeof(INT));
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf + EA_Count * sizeof(INT);
				ScriptEventItem itemEvent;
				itemEvent.SetScriptID(index,state,number,&scriptStr);
				paramEventList.AddItem( &itemEvent );
			}

		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionRegieCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionInfo
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				scriptId
	*					脚本编号
	*				misId
	*					任务编号
	*     功能说明 :
	*				向客户端返回任务信息处理
	*				
	*/
	INT LuaFnDispatchMissionInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		ScriptID_t scriptId;
		MissionID_t misId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);
		scriptId = Lua_ValueToNumber(L,4);
		misId = Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnDispatchMissionInfo")
		ScriptParam_MissionInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idMission = misId;
		paramEventList.m_idScript = scriptId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionBonus bonus;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItem(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RandItem)
			{
				SMissionBonus bonus;
				bonus.SetItemRand();
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RadioItem)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItemRadio(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Money)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetMoney(*number);
				paramEventList.AddBonus(&bonus);
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionDemandInfo
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				scriptId
	*					脚本编号
	*				misId
	*					任务编号
	*				done
	*					是否完成
	*     功能说明 :
	*				向客户端返回任务需求处理
	*				done如果为TRUE,客户端将激活任务界面的完成按钮
	*				之前需使用的函数依次为
	*				BeginEvent
	*				AddText,描述性字符串
	*				AddItemDemand,需求物品
	*				EndEvent
	*/
	INT LuaFnDispatchMissionDemandInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		ScriptID_t scriptId;
		MissionID_t misId;
		INT done;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);
		scriptId = Lua_ValueToNumber(L,4);
		misId = Lua_ValueToNumber(L,5);
		done = (Lua_ValueToNumber(L,6));

		BEGINHUMANDEFINE("LuaFnDispatchMissionDemandInfo")
		ScriptParam_MissionDemandInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = misId;
		paramEventList.m_bDone = done;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionDemandItem demandItem;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				demandItem.m_uItemID = number[0];
				demandItem.m_yCount = (BYTE)(number[1]);
				paramEventList.AddDemandItem(&demandItem);
			}
		}


		Packets::GCScriptCommand Msg;
		Msg.SetMissionDemandInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionContinueInfo
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				scriptId
	*					脚本编号
	*				misId
	*					任务编号
	*     功能说明 :
	*				向客户端返回任务继续处理
	*/
	INT LuaFnDispatchMissionContinueInfo(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		ScriptID_t scriptId;
		MissionID_t misId;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		targetId = Lua_ValueToNumber(L,3);
		scriptId = Lua_ValueToNumber(L,4);
		misId = Lua_ValueToNumber(L,5);
		
		BEGINHUMANDEFINE("LuaFnDispatchMissionContinueInfo")
		ScriptParam_MissionContinueInfo paramEventList;
		paramEventList.Reset();
		paramEventList.m_idNPC = targetId;
		paramEventList.m_idScript = scriptId;
		paramEventList.m_idMission = misId;

		for(INT i= 0;i < pScene->mMisBuf.mUseNum;i++)
		{
			if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Str)
			{
				ScriptString scriptStr;
				scriptStr = pScene->mMisBuf.Buff[i].buf;
				paramEventList.AddText( &scriptStr);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Item)
			{
				SMissionBonus bonus;

				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItem(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RandItem)
			{
				SMissionBonus bonus;
				bonus.SetItemRand();
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_RadioItem)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetItemRadio(number[0],number[1]);
				paramEventList.AddBonus(&bonus);
			}
			else if(pScene->mMisBuf.Buff[i].mType == _MisBuf::_MisItemBuf::BufType_Money)
			{
				SMissionBonus bonus;
				INT* number = (INT*)(pScene->mMisBuf.Buff[i].buf);
				bonus.SetMoney(*number);
				paramEventList.AddBonus(&bonus);
			}
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionContinueInfoResponseCmd( &paramEventList );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionTips
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				向客户端返回任务相关提示信息
	*				
	*/
	INT LuaFnDispatchMissionTips(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnDispatchMissionTips")
		ScriptParam_MissionTips paramMissionTips;
		paramMissionTips.Reset();
		if(pScene->mMisBuf.Buff[0].mType == _MisBuf::_MisItemBuf::BufType_Str)
		{
			paramMissionTips.m_strText = pScene->mMisBuf.Buff[0].buf;
		}

		Packets::GCScriptCommand Msg;
		Msg.SetMissionTipsCmd( &paramMissionTips );

		Player* pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &Msg ) ;

		return 0;	
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-28
	*     函数名称 : LuaFnDispatchMissionResult
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				向客户端返回任务验证结果
	*				
	*/
	INT LuaFnDispatchMissionResult(Lua_State* L)
	{

		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT IsFinished;

		sceneId		= Lua_ValueToNumber(L,1);
		selfId		= Lua_ValueToNumber(L,2);
		IsFinished	= Lua_ValueToNumber(L,3);
		BEGINHUMANDEFINE("LuaFnDispatchMissionResult")

		Packets::GCMissionResult Msg;
		Msg.setIsFinished(IsFinished);
		pHuman->GetPlayer()->SendPacket(&Msg);

		return 0;	

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnMissionCom
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				任务完成
	*				之后需DelMission
	*/
	INT LuaFnMissionCom(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;

		sceneId = Lua_ValueToNumber(L,1);	
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnMissionCom")
		pHuman->SetMissionHaveDone(misId,TRUE);

		return 0;
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				获得当前玩家某一任务号的值	//做过,正在做,未做
	*/
	INT LuaFnGetMission(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;

		sceneId = Lua_ValueToNumber(L,1);	
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMission")
		Lua_PushNumber(L,pHuman->IsHaveMission(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMissionIndexByID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				设置当前玩家某一任务号的值
	*/
	INT LuaFnGetMissionIndexByID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMissionIndexByID")
		Lua_PushNumber(L,pHuman->GetMissionIndexByID(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION	
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMissionParam
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misIndex
	*					任务位置索引
	*				paramIndex
	*					该任务字段编号
	*     功能说明 :
	*				 取任务参数
	*/
	INT LuaFnGetMissionParam(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		INT misIndex;
		INT paramIndex;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misIndex = Lua_ValueToNumber(L,3);
		paramIndex = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetMissionParam")
		if( misIndex>=0 )
		{
			Lua_PushNumber( L, pHuman->GetMissionParam( misIndex, paramIndex ) );
			return 1;
		}
		else
		{
			Lua_PushNumber( L, 0 );
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
	*     函数名称 : LuaFnSetMissionByIndex
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				paramIndex
	*					该任务字段编号
	*				value
	*					设paramIndex字段的值
	*     功能说明 :
	*				 设置任务参数
	*/
	INT LuaFnSetMissionByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		INT misIndex;
		INT paramIndex;
		INT value;
			
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misIndex = Lua_ValueToNumber(L,3);
		paramIndex = Lua_ValueToNumber(L,4);
		value= Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetMissionByIndex")
		if( misIndex>=0 )
		{
			pHuman->SetMissionParam(misIndex,paramIndex,value);
			return 0;
		}
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-18
	*     函数名称 : LuaFnGetMissionCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				 得到已接任务的数量
	*				 没有任务时返回0
	*/
	INT LuaFnGetMissionCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMissionCount")
			Lua_PushNumber(L,pHuman->GetConstMissionList()->m_Count);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : yf
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				scriptId
	*					调用脚本编号
	*				killObjEvent
	*					是否回调OnKillObject
	*				enterAreaEvent
	*					是否回调OnEnterArea
	*				itemChangeEvent
	*					是否回调OnItemChange
	*     功能说明 :
	*				在玩家身上添加misId编号的任务
	*				成功返回1
	*/
	INT LuaFnAddMission( Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;
		ScriptID_t scriptId;
		BOOL killObjEvent;
		BOOL enterAreaEvent;
		BOOL itemChangedEvent;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);
		scriptId = Lua_ValueToNumber(L,4);
		killObjEvent = Lua_ValueToNumber(L,5) > 0 ? 1 : 0;
		enterAreaEvent = Lua_ValueToNumber(L,6)> 0 ? 1 : 0;
		itemChangedEvent = Lua_ValueToNumber(L,7)> 0 ? 1 : 0;

		BEGINHUMANDEFINE("LuaFnAddMission")
		INT ret = pHuman->AddMission( misId, scriptId, killObjEvent, enterAreaEvent, itemChangedEvent );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-3-13
	*     函数名称 : LuaFnAddMissionEx
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				scriptId
	*					调用脚本编号
	*     功能说明 :
	*				在玩家身上添加misId编号的任务
	*				成功返回1
	*/
	INT LuaFnAddMissionEx( Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;
		ScriptID_t scriptId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);
		scriptId = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnAddMissionEx")
		INT ret = pHuman->AddMissionEx( misId, scriptId );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-3-13
	*     函数名称 : LuaFnSetMissionEvent
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				eventId
	*					调用脚本编号，0 KillObj, 1 EnterArea, 2 ItemChanged, 3 PetChanged
	*     功能说明 :
	*				修改玩家身上misId编号的任务相应事件
	*				成功返回1
	*/
	INT LuaFnSetMissionEvent( Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;
		INT eventId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);
		eventId = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetMissionEvent")
		INT ret = pHuman->SetMissionEvent( misId, eventId );
		ret = ( (ret==OR_OK) ? TRUE : FALSE ) ;

		Lua_PushNumber( L, ret );
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDelMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				删除任务
	*/
	INT LuaFnDelMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);
		
		BEGINHUMANDEFINE("LuaFnDelMission")
		pHuman->DelMission(misId);

		Lua_PushNumber( L, 1 );
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : yf
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnIsMissionHaveDone
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				是否做过此任务
	*				返回值1为成功
	*				返回值0为失败
	*/
	INT LuaFnIsMissionHaveDone(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnIsMissionHaveDone")
		Lua_PushNumber(L,pHuman->IsMissionHaveDone(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : IsHaveMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				玩家是否正在做此任务
	*				返回值大于0为成功
	*				
	*/
	INT LuaFnIsHaveMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		MissionID_t misId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		misId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnIsHaveMission")
		Lua_PushNumber(L,pHuman->IsHaveMission(misId));
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnBeginAddItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				开始添加道具操作
	*				与EndAddItem配对使用
	*/
	INT LuaFnBeginAddItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;

			sceneId = Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginAddItem")
			pScene->mItemList.Init();
			return 0;
		ENDSCENEDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)	
	*				count
	*					添加数目
	*     功能说明 :
	*				在玩家物品栏中增加count个itemType编号的道具
	*/
	INT LuaFnAddItem( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT itemTypeSn;
		INT count;

		sceneId = Lua_ValueToNumber(L,1);
		itemTypeSn = Lua_ValueToNumber(L,2);
		count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddItem")
		TSerialHelper help(itemTypeSn);
		_ITEM_TYPE	itemType =	help.GetItemTypeStruct();


		if(!itemType.isNull())
		{
			pScene->mItemList.AddType(itemType,count);
			
			return 0;
		}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : yf
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnEndAddItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				结束添加道具操作
	*				与BeginAddItem配对使用
	*				返回1表示有空间放置添加物品
	*/
	INT LuaFnEndAddItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnEndAddItem")
			Lua_PushNumber(L,HumanItemLogic::CanBagReceiveItemList(pHuman,pScene->mItemList));
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItemListToHuman
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				把物品列表中的所有物品放置到玩家包中
	*				在调用BeginAddItem和EndAddItem之后使用
	*
	*	  备注:
	*				此函数会回调响应OnItemChanged事件的脚本
	*/
	INT LuaFnAddItemListToHuman(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnAddItemListToHuman")
			//pHuman->ReceiveItemList(pScene->mItemList);

		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_SCRIPT;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
	
		BOOL bRet =	HumanItemLogic::ReceiveBagItemList(&ItemLogParam,pHuman,pScene->mItemList);

		if(bRet) SaveItemLog(&ItemLogParam);

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnGetMissionData
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					数据块索引
	*     功能说明 :
	*				得到Human全局数据块中的nIndex个数据值
	*				GetMissionData(L,18,12,3)
	*/
	INT LuaFnGetMissionData(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		nIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetMissionData")

		if( nIndex<0 || nIndex>=MAX_CHAR_MISSION_DATA_NUM )
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,pHuman->GetMissionData(nIndex));
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnSetMissionData
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					数据块索引
	*				nData
	*					数据值
	*     功能说明 :
	*				设置Human的nIndex个数据块的值为nData
	*				在调用BeginAddItem和EndAddItem之后使用
	*/
	INT LuaFnSetMissionData(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		INT nData;
		
		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		nIndex = Lua_ValueToNumber(L,3);
		nData = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetMissionData")
			if( nIndex<0 || nIndex>=MAX_CHAR_MISSION_DATA_NUM )
			{
				return 0;
			}
			pHuman->SetMissionData(nIndex,nData);
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnAddMonsterDropItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				MonsterID
	*					怪物编号
	*				HumanID
	*					角色编号
	*				ItemIndex
	*					掉落的物品类型
	*				
	*     功能说明 :
	*			在怪物身上添加一个掉落任务物品,并表明所有者
	*/
	INT LuaFnAddMonsterDropItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t MonsterID;
		ObjID_t HumanID;
		UINT ItemIndex;

		sceneId			=	Lua_ValueToNumber(L,1);
		MonsterID		=	Lua_ValueToNumber(L,2);
		HumanID			=	Lua_ValueToNumber(L,3);
		ItemIndex		=	Lua_ValueToNumber(L,4);
		
		BEGINSCENEDEFINE("LuaFnAddMonsterDropItem")
		Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			return 0;
		}
		Assert(ItemIndex>0);
		Assert(ItemIndex!=INVALID_ID);
		//pMonster->GetOwnerList().AddItem(ItemIndex,HumanID);

		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnGetMonsterOwnerCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				MonsterID
	*					怪物编号
	*				
	*     功能说明 :
	*			获取怪物所有者数量
	*/
	INT LuaFnGetMonsterOwnerCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t MonsterID;

		sceneId			=	Lua_ValueToNumber(L,1);
		MonsterID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetMonsterOwnerCount")
		Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			return 0;
		}
		
		Lua_PushNumber(L,pMonster->GetOwnerList().OwnerCount);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** ---------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnGetMonsterOwnerID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				MonsterID
	*					怪物编号
	*				OwnerOffSet
	*					所有者偏移量
	*     功能说明 :
	*			获取怪物所有者编号
	*			返回所有者id
	*/
	INT LuaFnGetMonsterOwnerID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t MonsterID;
		UINT OwnerOffSet;

		sceneId			=	Lua_ValueToNumber(L,1);
		MonsterID		=	Lua_ValueToNumber(L,2);
		OwnerOffSet		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnGetMonsterOwnerID")
		Obj_Monster*	pMonster	=	
			pScene->GetMonsterManager()->GetMonster(MonsterID);
		if(!pMonster)
		{
			Assert(FALSE);
			RETURNFALSE
		}	
		
		Assert(OwnerOffSet<pMonster->GetOwnerList().OwnerCount);
		//Lua_PushNumber(L,pMonster->GetOwnerList().OwnerDropList[OwnerOffSet].HumanID);
		return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** ---------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnBeginUICommand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*			开始构造UICommand数据体
	*			无返回
	*/
	INT LuaFnBeginUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;

		sceneId			=	Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnBeginUICommand")
			pScene->m_XParam.CleanUp() ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	/** ---------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnEndUICommand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	INT LuaFnEndUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;

		sceneId			=	Lua_ValueToNumber(L,1);

		BEGINSCENEDEFINE("LuaFnEndUICommand")
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnDispatchUICommand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nUIIndex
	*					窗口索引值
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	INT LuaFnDispatchUICommand(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nUIIndex;

		sceneId	= Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		nUIIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDispatchUICommand")
			GCUICommand Msg ;
			Msg.SetParam(&pScene->m_XParam) ;
			Msg.SetUIIndex(nUIIndex) ;
			pHuman->GetPlayer()->SendPacket(&Msg) ;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	
	
	/** ---------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnUICommand_AddInt
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				nValue
	*					向数据体添加的数据,整数
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	INT LuaFnUICommand_AddInt(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT nValue;

		sceneId			=	Lua_ValueToNumber(L,1);
		nValue			=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnUICommand_AddInt")
			pScene->m_XParam.AddIntValue(nValue) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnUICommand_AddString
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				szValue
	*					向数据体添加的数据,字符串
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	INT LuaFnUICommand_AddString(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		const CHAR* szValue;

		sceneId			=	Lua_ValueToNumber(L,1);
		szValue			=	Lua_ValueToString(L,2);

		BEGINSCENEDEFINE("LuaFnUICommand_AddString")
			pScene->m_XParam.AddStrValue(szValue) ;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-2-10
	*     函数名称 : LuaFnGetQuestionsRecord
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				nIndex
	*					问题号，整数
	*     功能说明 :
	*			读取题库信息
	*			返回值：问题内容(字符串),
	*					选项0(字符串),
	*					选项1(字符串),
	*					选项2(字符串),
	*					选项3(字符串),
	*					选项4(字符串),
	*					选项5(字符串),
	*					答案0(整数),
	*					答案1(整数),
	*					答案2(整数),
	*					答案3(整数),
	*					答案4(整数),
	*					答案5(整数),
	*/
	INT LuaFnGetQuestionsRecord(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		INT nIndex;

		nIndex			=	Lua_ValueToNumber(L,1);

		INT i ;
		for( i=0; i<MAX_QUESTIONS; i++ )
		{
			if( nIndex==g_QuestionLib.m_aQuestions[i].m_nIndex )
				break ;
		}
		if( i>=MAX_QUESTIONS )
		{
			AssertEx(FALSE,"此问题没有在题库中发现，请检查！") ;
			return 0 ;
		}

		_QUESTION_DATA* pData = &(g_QuestionLib.m_aQuestions[i]) ;

		Lua_PushString(L,pData->m_szContex) ;
		Lua_PushString(L,pData->m_szOption0) ;
		Lua_PushString(L,pData->m_szOption1) ;
		Lua_PushString(L,pData->m_szOption2) ;
		Lua_PushString(L,pData->m_szOption3) ;
		Lua_PushString(L,pData->m_szOption4) ;
		Lua_PushString(L,pData->m_szOption5) ;
		Lua_PushNumber(L,pData->m_Key0);
		Lua_PushNumber(L,pData->m_Key1);
		Lua_PushNumber(L,pData->m_Key2);
		Lua_PushNumber(L,pData->m_Key3);
		Lua_PushNumber(L,pData->m_Key4);
		Lua_PushNumber(L,pData->m_Key5);
		return 13;

		LUA_LEAVE_FUNCTION

		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushString(L,"") ;
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		Lua_PushNumber(L,0);
		return 13;
	}	

	/** ---------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-4-19
	*     函数名称 : LuaFnSetMissionDataBit
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					MissionData的编号索引
	*				nBits
	*					偏移的位数（0－31）
	*				nValue
	*					数据, 0或非0
	*     功能说明 :
	*			无返回
	*/
	INT LuaFnSetMissionDataBit(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		INT nBits;
		INT nValue;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		nIndex			=	Lua_ValueToNumber(L,3);
		nBits			=	Lua_ValueToNumber(L,4);
		nValue			=	Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetMissionDataBit")
			INT nDataValue = pHuman->GetMissionData(nIndex) ;
			if( nValue )
			{
				nDataValue |= (1<<nBits) ;
			}
			else
			{
				nDataValue &= (~(1<<nBits)) ;
			}
			pHuman->SetMissionData(nIndex,nDataValue) ;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}	

	/** ---------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-4-19
	*     函数名称 : LuaFnGetMissionDataBit
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					MissionData的编号索引
	*				nBits
	*					偏移的位数（0－31）
	*     功能说明 :
	*			0 或者 1
	*/
	INT LuaFnGetMissionDataBit(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT nIndex;
		INT nBits;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		nIndex			=	Lua_ValueToNumber(L,3);
		nBits			=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetMissionDataBit")
			INT nDataValue = pHuman->GetMissionData(nIndex) ;
			INT nRet=nDataValue&(1<<nBits) ;
			//nRet = nRet >> nBits; // 返回nRet为1或0
			Lua_PushNumber(L,nRet);
			return 1 ;
		ENDHUMANDEFINE

		Lua_PushNumber(L,0);
		return 1 ;

		LUA_LEAVE_FUNCTION
	}	

	/** ---------------------------------------------------
	*     开发人员 : liulu
	*     编写时间 : 2010-07-22
	*     函数名称 : LuaFnIsMissionInTime
	*					
	*     功能说明 :
	*			返回每日活动任务是否在活动进行时间内
	*			接每日活动时用此函数判断
	*	
	*/
	INT LuaFnIsMissionInTime(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
	
		INT nMissionID;

		nMissionID = Lua_ValueToNumber( L, 1 );	

		INT nRet;

		nRet = DayTips::GetMe()->IsMissionInTime( nMissionID );

		Lua_PushNumber( L, nRet );
		return 1 ;

		LUA_LEAVE_FUNCTION
	}

	/** ---------------------------------------------------
	*     开发人员 : liulu
	*     编写时间 : 2010-07-22
	*     函数名称 : LuaFnMissionCommonCheck
	*					
	*     功能说明 :
	*			返回玩家是否满足接每日活动的基本条件
	*			接每日活动时用此函数判断
	*
	*/
	INT LuaFnMissionCommonCheck(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT  nMissionID;
		BOOL bRet;

		SceneID_t sceneId;
		ObjID_t selfId;

		nMissionID = Lua_ValueToNumber( L, 1 );	
		sceneId    = Lua_ValueToNumber( L, 2 );	
		selfId     = Lua_ValueToNumber( L, 3 );	

		BEGINHUMANDEFINE( "LuaFnMissionCommonCheck" )		

		bRet = DayTips::GetMe()->CommonCheck( nMissionID, pHuman );

		ENDHUMANDEFINE

		if ( bRet == TRUE )
		{
			Lua_PushNumber( L, 1 );
		}
		else
			Lua_PushNumber( L, 0 );
			
		return 1;

		LUA_LEAVE_FUNCTION
	}

	/** ---------------------------------------------------
	*     开发人员 : liulu
	*     编写时间 : 2010-07-23
	*     函数名称 : LuaFnIsTodayMission
	*					
	*     功能说明 :
	*			返回每日活动是否是今日可见任务
	*/
	INT LuaFnIsTodayMission(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT  nMissionID;
		BOOL bRet;

		nMissionID = Lua_ValueToNumber( L, 1 );	

		bRet = DayTips::GetMe()->IsTodayMission( nMissionID );		

		if ( bRet == TRUE )
		{
			Lua_PushNumber( L, 1 );
		}
		else
			Lua_PushNumber( L, 0 );

		return 1;

		LUA_LEAVE_FUNCTION		
	}

	/** ---------------------------------------------------
	*     开发人员 : liulu
	*     编写时间 : 2010-07-23
	*     函数名称 : LuaFnSetTimeParam								
	*					
	*     功能说明 :
	*			保存最后一次完成活动的时间
	*			下标在脚本ScriptGlobal.lua里配置
	*
	*/
	INT LuaFnSetTimeParam(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId;
		ObjID_t   selfId;

		INT  nIndex;

		nIndex     = Lua_ValueToNumber( L, 1 );	
		sceneId    = Lua_ValueToNumber( L, 2 );
		selfId     = Lua_ValueToNumber( L, 3 );

		BEGINHUMANDEFINE("LuaFnSetTimeParam")

			if( nIndex >= 0 )
			{
				INT nValue;

				nValue = DayTips::GetMe()->GetTodayFormatDate();

				pHuman->SetMissionData( nIndex, nValue );
				return 0;
			}

		ENDHUMANDEFINE
		 LUA_LEAVE_FUNCTION
	}

	/** ---------------------------------------------------
	*     开发人员 : liulu
	*     编写时间 : 2010-07-30
	*     函数名称 : LuaFnGetTodayDate								
	*					
	*     功能说明 :
	*			获取当前日期，按年月日转换后的格式
	*			
	*/
	INT LuaFnGetTodayDate(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT nValue;

		nValue = DayTips::GetMe()->GetTodayFormatDate();

		Lua_PushNumber( L, nValue );

		return 1;

		LUA_LEAVE_FUNCTION
	}


}

#endif