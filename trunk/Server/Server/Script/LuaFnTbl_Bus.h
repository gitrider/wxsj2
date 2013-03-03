//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Bus.h
// 模块	:	Script
// 功能	 :  属性相关函数
// 修改历史:
//	wangpojin创建
//-----------------------------------------------------------------------------

#ifndef __LUAFNTBL_BUS_H__
#define __LUAFNTBL_BUS_H__

#include "LuaFnMacro.h"
#include "Obj_Human.h"
#include "Obj_Bus.h"
#include "BusRule.h"
#include "GCOperateResult.h"

#include "GCOperateResult.h"


namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnCreateBus
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busDataID
	*					数据表中的Id
	*				posX
	*					x坐标
	*				posZ
	*					z坐标
	*				dir
	*					方向
	*     功能说明 :
	*				创建一个BUS
	*/
	INT LuaFnCreateBus(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		INT busDataId;
		FLOAT posX;
		FLOAT posZ;
		FLOAT dir;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET0(5)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 5, "LuaFnCreateBus"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busDataId = Lua_ValueToNumber(L,2);
		posX = Lua_ValueToNumber(L,3);
		posZ = Lua_ValueToNumber(L,4);
		dir = Lua_ValueToNumber(L,5);

		BEGINSCENEDEFINE("LuaFnCreateBus")
			WORLD_POS tempPos(posX, posZ);
			ObjID_t nBusObjID = BusRule::CreateBus(sceneId, INVALID_GUID, busDataId, &tempPos, dir);
			Lua_PushNumber(L, nBusObjID);
			return 1;

		ENDSCENEDEFINE

		Lua_PushNumber(L, INVALID_ID);
		return 1;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnCreateBusByPatrolPathId
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busDataID
	*					数据表中的Id
	*				patrolPathId
	*					路径点编号
	*				athwart
	*					是否为逆向而行（相对路径点的逆向）
	*     功能说明 :
	*				创建一个BUS
	*/
	INT LuaFnCreateBusByPatrolPathId(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		INT busDataId;
		INT patrolPathId;
		BOOL athwart;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET0(4)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 4, "LuaFnCreateBusByPatrolPathId"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busDataId = Lua_ValueToNumber(L,2);
		patrolPathId = Lua_ValueToNumber(L,3);
		athwart = Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnCreateBusByPatrolPathId")

			ObjID_t nBusObjID = BusRule::CreateBusByPatrolPathID(sceneId, INVALID_GUID, busDataId, patrolPathId, athwart);
			Lua_PushNumber(L, nBusObjID);
			return 1;

		ENDSCENEDEFINE

		Lua_PushNumber(L, INVALID_ID);
		return 1;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-12-15
	*     函数名称 : LuaFnDeleteBus
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busId
	*					交通工具的ID
	*     功能说明 :
	*				创建一个BUS
	*/
	INT LuaFnDeleteBus(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t busId;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET0(2)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 2, "LuaFnDeleteBus"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnDeleteBus")

			Obj_Bus *pBus = NULL;
			if(busId != INVALID_ID)
			{
				pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(busId));
			}
			if(pBus != NULL && pBus->GetObjType() == Obj::OBJ_TYPE_BUS)
			{
				BusRule::DeleteBus(sceneId, pBus->GetID());
			}

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnBusAddPassenger
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busId
	*					交通工具的ID
	*				passengerId
	*					要上车的角色ID
	*     功能说明 :
	*				加一个乘客到交通工具
	*/
	INT LuaFnBusAddPassenger(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t busId;
		ObjID_t passengerId;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET0(3)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 3, "LuaFnBusAddPassenger"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busId = Lua_ValueToNumber(L,2);
		passengerId = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnBusAddPassenger")

			Obj_Bus *pBus = NULL;
			if(busId != INVALID_ID)
			{
				pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(busId));
			}
			if(pBus != NULL && pBus->GetObjType() == Obj::OBJ_TYPE_BUS)
			{
				ORESULT oResult = BusRule::AddPassenger(sceneId, pBus->GetID(), passengerId);
				if(OR_SUCCEEDED(oResult))
				{
					Lua_PushNumber(L, 1);
					return 1;
				}
				else
				{
					Obj_Human *pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(passengerId));
					if(	pHuman != NULL && 
						pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN && 
						pHuman->IsCanLogic() )
					{
						GCOperateResult msgResult;
						msgResult.setResult(oResult);
						pHuman->GetPlayer()->SendPacket(&msgResult);
					}
					Lua_PushNumber(L, 0);
					return 1;
				}
			}

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnBusRemovePassenger
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busId
	*					交通工具的ID
	*				passengerId
	*					要下车的角色ID
	*     功能说明 :
	*				从交通工具移掉一个乘客
	*/
	INT LuaFnBusRemovePassenger(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t busId;
		ObjID_t passengerId;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET0(3)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 3, "LuaFnBusRemovePassenger"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busId = Lua_ValueToNumber(L,2);
		passengerId = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnBusRemovePassenger")

			Obj_Bus *pBus = NULL;
			if(busId != INVALID_ID)
			{
				pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(busId));
			}
			if(pBus != NULL && pBus->GetObjType() == Obj::OBJ_TYPE_BUS)
			{
				ORESULT oResult = BusRule::RemovePassenger(sceneId, pBus->GetID(), passengerId);
				if(OR_SUCCEEDED(oResult))
				{
					Lua_PushNumber(L, 1);
					return 1;
				}
				else
				{
					Obj_Human *pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(passengerId));
					if(	pHuman != NULL && 
						pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN && 
						pHuman->IsCanLogic() )
					{
						GCOperateResult msgResult;
						msgResult.setResult(oResult);
						pHuman->GetPlayer()->SendPacket(&msgResult);
					}
					Lua_PushNumber(L, 0);
					return 1;
				}
			}

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnBusRemoveAllPassenger
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busId
	*					交通工具的ID
	*     功能说明 :
	*				从交通工具移掉所有乘客
	*/
	INT LuaFnBusRemoveAllPassenger(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t busId;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET0(2)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 2, "LuaFnBusRemoveAllPassenger"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnBusRemoveAllPassenger")

			Obj_Bus *pBus = NULL;
			if(busId != INVALID_ID)
			{
				pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(busId));
			}
			if(pBus != NULL && pBus->GetObjType() == Obj::OBJ_TYPE_BUS)
			{
				BusRule::DebusAllPassenger(sceneId, pBus->GetID());
				Lua_PushNumber(L, 1);
				return 1;
			}

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnBusStart
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busId
	*					交通工具的ID
	*     功能说明 :
	*				开车
	*/
	INT LuaFnBusStart(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t busId;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET0(2)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 2, "LuaFnBusStart"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busId = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnBusStart")

			Obj_Bus *pBus = NULL;
			if(busId != INVALID_ID)
			{
				pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(busId));
			}

			if(pBus != NULL && pBus->GetObjType() == Obj::OBJ_TYPE_BUS)
			{
				BusRule::StartPatrolPath(sceneId, pBus->GetID());
				Lua_PushNumber(L, 1);
				return 1;
			}

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnBusGetObjIDByGUID
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				guid
	*					交通工具的GUID
	*     功能说明 :
	*				得到交通工具的ObjID
	*/
	INT LuaFnBusGetObjIDByGUID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		GUID_t guid;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET0(2)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 2, "LuaFnBusGetObjIDByGUID"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		guid = (GUID_t)(Lua_ValueToNumber(L,2));

		BEGINSCENEDEFINE("LuaFnBusGetObjIDByGUID")

			ObjID_t nObjID = BusRule::GetBusObjIDByGUID(sceneId, guid);
			Lua_PushNumber(L, nObjID);
			return 1;

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnBusAddPassenger_Shuttle
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busId
	*					交通工具的ID
	*				passengerId
	*					要上车的角色ID
	*				npcId
	*					车的售票员
	*				isOutputError
	*					是否输出错误提示
	*     功能说明 :
	*				加一个乘客到往返的交通工具上
	*/
	INT LuaFnBusAddPassenger_Shuttle(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t busId;
		ObjID_t passengerId;
		ObjID_t npcId;
		BOOL isOutputError;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET0(4)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 5, "LuaFnBusAddPassenger_Shuttle"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busId = Lua_ValueToNumber(L,2);
		passengerId = Lua_ValueToNumber(L,3);
		npcId = Lua_ValueToNumber(L,4);
		isOutputError = Lua_ValueToNumber(L,5);

		BEGINSCENEDEFINE("LuaFnBusAddPassenger_Shuttle")

			Obj_Bus *pBus = NULL;
			if(busId != INVALID_ID)
			{
				pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(busId));
			}
			Obj *pNpc = NULL;
			if(npcId != INVALID_ID)
			{
				pNpc = (Obj*)(pScene->GetObjManager()->GetObj(npcId));
			}
			ORESULT oResult = OR_ERROR;
			if(pBus != NULL && pBus->GetObjType() == Obj::OBJ_TYPE_BUS && pNpc != NULL)
			{
				if(!(pBus->IsRunning()))
				{
					FLOAT fDistSq = MyLengthSq(pBus->getWorldPos(), pNpc->getWorldPos());
					if(fDistSq < 400.f)
					{
						oResult = BusRule::AddPassenger(sceneId, pBus->GetID(), passengerId);
						if(OR_SUCCEEDED(oResult))
						{
							Lua_PushNumber(L, OR_OK);
							return 1;
						}
					}
					else
					{
						oResult = OR_BUS_RUNNING;
					}
				}
				else
				{
					oResult = OR_BUS_RUNNING;
				}
			}
			else
			{
				oResult = OR_ERROR;
			}
			if(OR_FAILED(oResult))
			{
				if(isOutputError)
				{
					Obj_Human *pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(passengerId));
					if(	pHuman != NULL && 
						pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN && 
						pHuman->IsCanLogic() )
					{
						GCOperateResult msgResult;
						msgResult.setResult(oResult);
						pHuman->GetPlayer()->SendPacket(&msgResult);
					}
				}
				Lua_PushNumber(L, oResult);
				return 1;
			}

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2007-01-10
	*     函数名称 : LuaFnBusAddPassengerList
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busId
	*					交通工具的ID
	*				npcId
	*					车的售票员
	*				isOutputError
	*					是否输出错误提示
	*				passengerCount
	*					要上车的角色数量
	*				passengerId0~xxx
	*					要上车的角色ID
	*     功能说明 :
	*				加N个乘客到往返的交通工具上
	*/
	INT LuaFnBusAddPassengerList(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t busId;
		ObjID_t npcId;
		BOOL isOutputError;
		INT passengerCount;
		ObjID_t passengerIdList[DEF_BUS_MAX_PASSENGER_COUNT];
		//Steven.Han end   2006-9-18 16:16

		////CHECKPARAM_RET0(4)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 5, "LuaFnBusAddPassengerList"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busId = Lua_ValueToNumber(L,2);
		npcId = Lua_ValueToNumber(L,3);
		isOutputError = Lua_ValueToNumber(L,4);
		passengerCount = Lua_ValueToNumber(L,5);
		if(passengerCount > DEF_BUS_MAX_PASSENGER_COUNT)
		{
			passengerCount = DEF_BUS_MAX_PASSENGER_COUNT;
		}
		INT i;
		for(i = 0; i < passengerCount; i++)
		{
			passengerIdList[i] = Lua_ValueToNumber(L,6+i);
		}

		BEGINSCENEDEFINE("LuaFnBusAddPassengerList")

			Obj_Bus *pBus = NULL;
			if(busId != INVALID_ID)
			{
				pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(busId));
			}
			Obj *pNpc = NULL;
			if(npcId != INVALID_ID)
			{
				pNpc = (Obj*)(pScene->GetObjManager()->GetObj(npcId));
			}
			ORESULT oResult = OR_ERROR;
			if(pBus != NULL && pBus->GetObjType() == Obj::OBJ_TYPE_BUS && pNpc != NULL)
			{
				if(!(pBus->IsRunning()))
				{
					FLOAT fDistSq = MyLengthSq(pBus->getWorldPos(), pNpc->getWorldPos());
					if(fDistSq < 400.f)
					{
						if(pBus->GetMaxPassengerCount() >= passengerCount + pBus->GetPassengerCount())
						{
							for(i = 0; i < passengerCount; i++)
							{
								oResult = BusRule::TestAddPassenger(sceneId, pBus->GetID(), passengerIdList[i]);
								if(OR_FAILED(oResult))
								{
									break;
								}
							}
							if(OR_SUCCEEDED(oResult))
							{
								for(i = 0; i < passengerCount; i++)
								{
									oResult = BusRule::AddPassenger(sceneId, pBus->GetID(), passengerIdList[i]);
									if(OR_FAILED(oResult))
									{
										break;
									}
								}
								if(OR_SUCCEEDED(oResult))
								{
									Lua_PushNumber(L, OR_OK);
									return 1;
								}
								else
								{
									BusRule::DebusAllPassenger(sceneId, pBus->GetID());
								}
							}
						}
						else
						{
							oResult = OR_BUS_PASSENGERFULL;
						}
					}
					else
					{
						oResult = OR_BUS_RUNNING;
					}
				}
				else
				{
					oResult = OR_BUS_RUNNING;
				}
			}
			else
			{
				oResult = OR_ERROR;
			}
			if(OR_FAILED(oResult))
			{
				if(isOutputError)
				{
					for(i = 0; i < passengerCount; i++)
					{
						Obj_Human *pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(passengerIdList[i]));
						if(	pHuman != NULL && 
							pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN && 
							pHuman->IsCanLogic() )
						{
							GCOperateResult msgResult;
							msgResult.setResult(oResult);
							pHuman->GetPlayer()->SendPacket(&msgResult);
						}
					}
				}
				Lua_PushNumber(L, oResult);
				return 1;
			}

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-12-11
	*     函数名称 : LuaFnGetBusScriptParam
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busId
	*					交通工具的objId
	*				paramIndex
	*					参数的索引值
	*     功能说明 :
	*				得到BUS的脚本参数
	*/
	INT LuaFnGetBusScriptParam(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t busId;
		INT paramIndex;
		//Steven.Han end   2006-9-18 16:16

		////CHECKPARAM_RET0(3)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 3, "LuaFnGetBusScriptParam"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busId = (ObjID_t)(Lua_ValueToNumber(L,2));
		paramIndex = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnGetBusScriptParam")

			Obj_Bus *pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(busId));
			if(pBus != NULL && pBus->GetObjType() == Obj::OBJ_TYPE_BUS)
			{
				INT nValue = pBus->GetScriptParam(paramIndex);
				Lua_PushNumber(L, nValue);
				return 1;
			}

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-12-11
	*     函数名称 : LuaFnSetBusScriptParam
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busId
	*					交通工具的objId
	*				paramIndex
	*					参数的索引值
	*				nValue
	*					参数值
	*     功能说明 :
	*				设BUS的脚本参数
	*/
	INT LuaFnSetBusScriptParam(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t busId;
		INT paramIndex;
		INT nValue;
		//Steven.Han end   2006-9-18 16:16

		////CHECKPARAM_RET0(4)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 4, "LuaFnSetBusScriptParam"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busId = (ObjID_t)(Lua_ValueToNumber(L,2));
		paramIndex = Lua_ValueToNumber(L,3);
		nValue = Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnSetBusScriptParam")

			Obj_Bus *pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(busId));
			if(pBus != NULL && pBus->GetObjType() == Obj::OBJ_TYPE_BUS)
			{
				pBus->SetScriptParam(paramIndex, nValue);
				return 0;
			}

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-12-29
	*     函数名称 : LuaFnGetBusPassengerCount
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busId
	*					交通工具的objId
	*     功能说明 :
	*				取乘客数量
	*/
	INT LuaFnGetBusPassengerCount(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t busId;
		//Steven.Han end   2006-9-18 16:16

		////CHECKPARAM_RET0(2)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 2, "LuaFnGetBusPassengerCount"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busId = (ObjID_t)(Lua_ValueToNumber(L,2));

		BEGINSCENEDEFINE("LuaFnGetBusPassengerCount")

			Obj_Bus *pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(busId));
			if(pBus != NULL && pBus->GetObjType() == Obj::OBJ_TYPE_BUS)
			{
				INT nPassengerCount = pBus->GetPassengerCount();
				Lua_PushNumber(L, nPassengerCount);
				return 1;
			}

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpj
	*     编写时间 : 2006-12-29
	*     函数名称 : LuaFnGetBusPassengerIDByIndex
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				busId
	*					交通工具的objId
	*				index
	*					乘客的索引值
	*     功能说明 :
	*				取乘客ObjID
	*/
	INT LuaFnGetBusPassengerIDByIndex(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t busId;
		INT index;
		//Steven.Han end   2006-9-18 16:16

		////CHECKPARAM_RET0(3)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 3, "LuaFnGetBusPassengerIDByIndex"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		busId = (ObjID_t)(Lua_ValueToNumber(L,2));
		index = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnGetBusPassengerIDByIndex")

			Obj_Bus *pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(busId));
			if(pBus != NULL && pBus->GetObjType() == Obj::OBJ_TYPE_BUS)
			{
				ObjID_t nPassengerID = pBus->GetPassengerIDByIndex(index);
				Lua_PushNumber(L, nPassengerID);
				return 1;
			}

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*	  [ QUFEI 2007-08-23 20:50 UPDATE BugID 23699 ]
	*     开发人员 : qufei
	*     编写时间 : 2007-23-08
	*     函数名称 : LuaFnGetBusPassengerID_IsInBus
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					调用者ID
	*     功能说明 :
	*				判断调用者是否在交通工具上
	*/
	INT LuaFnGetBusPassengerID_IsInBus(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		//Steven.Han start 2006-9-18 16:16
		SceneID_t sceneId;
		ObjID_t selfId;
		//Steven.Han end   2006-9-18 16:16

		//CHECKPARAM_RET0(2)

		if(FALSE==LuaVerifyAllNumberParam(L, 1, 2, "LuaFnGetBusPassengerID_IsInBus"))
		{
			RETURNFALSE;
		}

		sceneId = Lua_ValueToNumber(L,1);
		selfId  = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetBusPassengerID_IsInBus")
			BOOL bInBus	= pHuman->IsInBus();
			Lua_PushNumber(L, pHuman->IsInBus());
			return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

}

#endif // __LUAFNTBL_BUS_H__
