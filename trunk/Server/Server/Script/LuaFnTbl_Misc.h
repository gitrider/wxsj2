//-----------------------------------------------------------------------------
// 文件名 : LuaFnTbl_Misc.h
// 模块	:	Script
// 功能	 :  杂项相关函数
// 修改历史:
//	yangfei创建
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_MISC_H__
#define __LUAFNTBL_MISC_H__

#include "LuaFnMacro.h"
#include "TimeManager.h"
#include "GWChat.h"
#include "CGChat.h"
#include "FileDataMgr.h"
#include "GUIDManager.h"
#include "GCDelObject.h"
#include "Scene.h"
#include "ServerManager.h"
#include "SceneTimer.h"
#include "PacketFactoryManager.h"
#include "GCCharAllTitles.h"
#include "GCBankBegin.h"
#include "GWRelation.h"
#include "GCRelation.h"
#include "GCExpandBankSize.h"
#include "GCBankSize.h"
#include "GameTable.h"
#include "GWCountryDelateRequest.h"
#include "GWCountryVoteRequest.h"

extern INT	g_Command_AlwaysLoadScript ;

namespace LuaFnTbl
{
/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetTimer
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 selfId
	*					调用者ID,无符号整型
	*				 scriptId
	*					执行脚本ID,short类型
	*				 funcName
	*					计时器回调调函数名,字符串类型
	*				 tickTime
	*					计时器调用间隔时间
	*     功能说明 :
	*				 给玩家打开计时器,时间到时将自动调用OnTimer函数	
	*				 玩家本身身上并没有计时器，脚本程序需要计时器时向系统申请“租用”，由系统分配
	*                返回－1,表示错误
	*/
	INT LuaFnSetTimer( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ScriptID_t scriptId;
		const CHAR* funcName;
		INT tickTime;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		scriptId = Lua_ValueToNumber(L,3);
		funcName = Lua_ValueToString(L,4);
		tickTime = Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetTimer")
		if(pHuman->GetUseTimerCount() < pHuman->GetMaxTimerCount())
		{
			INT sceneTimerIndex = pScene->GetSceneTimer()->GetTimer();
			AssertEx(sceneTimerIndex>=0 && sceneTimerIndex < g_Config.m_ConfigInfo.m_MaxTimerCount,"No Timer resource for allocate");

			if(sceneTimerIndex>=0 && sceneTimerIndex < g_Config.m_ConfigInfo.m_MaxTimerCount)
			{
				pHuman->AddTimer(sceneTimerIndex);
				pScene->GetSceneTimer()->StartTheTimer(sceneTimerIndex,selfId,scriptId,funcName,tickTime);

				Lua_PushNumber(L,sceneTimerIndex);
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
	*     函数名称 : LuaFnStopTimer
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				sceneTimerIndex
	*					计时器在场景计时器池中的数组下标,有SetTimer时返回		
	*     功能说明 :
    *				关闭当前玩家的计时器	//每个玩家身上最多可以绑定三个计时器
	*/
	INT LuaFnStopTimer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT sceneTimerIndex;

		sceneId = Lua_ValueToNumber(L,1);
		sceneTimerIndex = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnStopTimer")
		pScene->GetSceneTimer()->FreeTimer(sceneTimerIndex);
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-24
	*     函数名称 : LuaFnCheckTimer
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 TimeIndex
	*					计时器索引
	*     功能说明 : 判断TimeIndex号计时器是否已经在使用中
	*                返回1,表示在使用；返回0，表示没有使用; 返回-1，表示异常
	*/
	INT LuaFnCheckTimer( Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT TimeIndex;

		sceneId = Lua_ValueToNumber(L,1);
		TimeIndex = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnCheckTimer")
		INT nRet = pScene->GetSceneTimer()->CheckTimer(TimeIndex) ;
		Lua_PushNumber(L,nRet);
		return 1;
		ENDSCENEDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetGameTime
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
    *				获取当前玩家在线时间，单位为秒
	*				用当前时间减去上线时间
	*				没有测试
	*/
	INT LuaFnGetGameTime(Lua_State* L)	
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetGameTime")
		Lua_PushNumber(L,g_pTimeManager->CurrentTime() / 1000  - pHuman->GetDB()->GetHumanDB()->m_LastLoginTime);
		return 1;
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 陈军
	*     编写时间 : 2010-7-30
	*     函数名称 : LuaFnGetTickCount
	*     参数说明 : 
	*				L
	*				LuaState执行环境指针
	*     功能说明 :
	*/
	INT LuaFnGetTickCount(Lua_State* L)	
	{	
		Lua_PushNumber(L, g_pTimeManager->CurrentTime());

		return 1;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-1-10
	*     函数名称 : LuaFnGetCurrentTime
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*     功能说明 :
	*				获取当前时间，单位为秒
	*/
	INT LuaFnGetCurrentTime(Lua_State* L)	
	{	
		Lua_PushNumber(L, g_pTimeManager->GetANSITime());
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 王勇鸷
	*     编写时间 : 2008-4-21
	*     函数名称 : LuaFnIsInStall
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				玩家是否处于摆摊状态
	*/
	INT LuaFnIsInStall(Lua_State* L)	
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId = Lua_ValueToNumber(L,1);
		selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnIsInStall")

		if ( pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN )
			Lua_PushNumber(L,1);
		else 
			Lua_PushNumber(L,0);

		return 1;
		
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		return 0 ;

	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddGlobalCountNews
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				newstr
	*					公告内容,字符串指针类型
	*				count
	*					发送次数
	*     功能说明 :
    *				向全组服务器发送新闻公告，公告发送指定次数
	*				没有测试
	*/
	INT LuaFnAddGlobalCountNews(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		CHAR* newstr;
		INT count;

		sceneId = Lua_ValueToNumber(L,1);
		newstr = const_cast<CHAR*>(Lua_ValueToString(L,2));
		count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddGlobalCountNews")

		GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

		pChatPacket->SetSourGUID( INVALID_ID ) ;
		pChatPacket->SetChatType( CHAT_TYPE_SYSTEM ) ;
		pChatPacket->SetContexSize( count ) ;
		pChatPacket->SetContex( newstr ) ;

		g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;

		return 0;

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnPrintNum
	*     参数说明 : 
	*				num
	*				消息框的提示数字
	*     功能说明 :
	*				弹出消息框,测试脚本用
	*/
	INT LuaFnPrintNum(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		INT num;

		num = Lua_ValueToNumber(L,1);

		CHAR msg[1024];
		memset(msg,0,1024) ;
		sprintf(msg,"%d\n",num);
		MyMessageBox(msg);
		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnPrintStr
	*     参数说明 : 
	*				strText
	*				消息框的提示字符串
	*     功能说明 :
	*				弹出消息框,测试脚本用
	*/
	INT LuaFnPrintStr(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		const CHAR* strText;

		strText = Lua_ValueToString(L,1);

		CHAR msg[1024];
		memset(msg,0,1024);
		sprintf(msg,"%s\n",strText);
		MyMessageBox(msg);
		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	

	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnCallScriptFunction
	*     参数说明 : 
	*     功能说明 :
	*					调用可变参数的任意脚本函数
	*/
	INT LuaFnCallScriptFunction(Lua_State* L,...) 
	{
		LUA_ENTER_FUNCTION
		
		INT argc = Lua_GetTopIndex(L);

		AssertEx(argc>=3,"参数个数小于3");

		ScriptID_t scriptId = Lua_ValueToNumber(L,1);
		const CHAR* funcname = Lua_ValueToString(L,2);
		INT sceneId = Lua_ValueToNumber(L,3);

		Scene* pScene = g_pSceneManager->GetScene(sceneId);
		Assert( pScene && "CallScriptFunction pScene = null" );

		if(pScene)
		{
			INT nTopIndex = 0;
			pScene->GetLuaInterface()->mLua.SafeCallBegin( &nTopIndex ) ;
			
			BOOL bLoad = FALSE ;
			SFileData* pSFileData = (SFileData*)(pScene->GetLuaInterface()->m_ScriptTable.Get(scriptId)) ;
			if( pSFileData==NULL )
			{
				pSFileData = pScene->GetLuaInterface()->GetOwner()->GetScriptFileMgr()->GetFileData(scriptId);
				Assert( pSFileData != NULL && "CallScriptFunction scriptId not found" );
				pScene->GetLuaInterface()->m_ScriptTable.Add( scriptId, pSFileData ) ;
				bLoad = TRUE ;
			}

#ifdef _DEBUG
			CHAR buf[32];
			tsnprintf(buf,32,"文件号%d",scriptId);
			AssertEx( pSFileData && "CallScriptFunction pFileData = null" ,buf);
#endif
			const CHAR* filename = pSFileData->GetFileName();
			AssertEx( filename && "CallScriptFunction filename = null",filename );

			if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
			BOOL bRet=TRUE ;
			if( bLoad )
			{
				bRet = pScene->GetLuaInterface()->mLua.Load( const_cast<CHAR*>(filename) ) ;
				AssertEx(bRet,filename);
				if( bRet == FALSE )
					return 0;
			}

			if ( pSFileData != NULL )
			{
				if ( filename != NULL )
				{
					if ( TRUE )
					{
						CHAR argv[10];
						memset(argv,0,10);
						
						char funcnameX[128];
						memset(funcnameX,0,128);
						sprintf(funcnameX,"x%.6d_%s",scriptId,funcname);
						char* fnname = funcnameX;

						#if _DEBUG
							lua_pushstring(L,filename);
							lua_setglobal(L,FILENAMEKEY);
						#endif

						if(!(pScene->GetLuaInterface()->PrefixCall(L,&fnname)))
						{
							Lua_PushNumber(L,-1);
							return 1;
						}

						LUA_ENTER_FUNCTION

						Lua_GetGlobal(L, fnname); 

						INT nBeginParamIndex = 3;
						for(INT i=nBeginParamIndex;i<=argc;i++)
						{
							switch ( Lua_GetValueType( L, i ) )
							{
							case LUA_TUSERDATA:
								argv[i-nBeginParamIndex] = 'p';
								//Lua_PushUserTag(L,Lua_ValueToUserData(L,i));
								break;
							case LUA_TNIL:
								argv[i-nBeginParamIndex] = 'n';
								Lua_PushNil(L);
								break;
							case LUA_TNUMBER:
								argv[i-nBeginParamIndex] = 'd';
								Lua_PushNumber(L,Lua_ValueToNumber(L,i));
								break;
							case LUA_TSTRING:
								argv[i-nBeginParamIndex] = 's';
								Lua_PushString(L,Lua_ValueToString(L,i));
								break;
							case LUA_TTABLE:
								argv[i-nBeginParamIndex] = 't';
								//Lua_PushNumber(L,Lua_ValueToString(L,i));
								break;
							case LUA_TFUNCTION:
							default:
								argv[i-nBeginParamIndex] = 'f';
								Lua_PushCFunction(L,Lua_ValueToCFunction(L,i));
								break;
							}
						}

						INT nResults = -1;
						INT ret = Lua_Call(L, argc - nBeginParamIndex + 1, nResults);

						if (ret != 0)
						{
							Log::SaveLog(SERVER_ERRORFILE,"Lua_Call %s %s!",filename,fnname);
						}

						int argnow = Lua_GetTopIndex(L);
						nResults = argnow - argc;//(INT)Lua_ValueToNumber(L,argc);		
						for(int i=1;i<=nResults;i++)
						{
							Lua_PushNumber(L,lua_tonumber(L,i +argc));
						}
						
						pScene->GetLuaInterface()->mLua.SafeCallEnd( argnow ) ;
						return nResults;

						LUA_LEAVE_FUNCTION
					}
				}
			}
		}
		
		Lua_PushNumber(L,-1);
		return 1;	

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,-1);
		return 1;	
	}


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnShowNpcEventList
	*     参数说明 : 
	*     功能说明 :
	*				调用可变参数的任意脚本函数
	*/
	INT LuaFnShowNpcEventList(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		targetId		=	Lua_ValueToNumber(L,3);	
		
		BEGINSCENEDEFINE("LuaFnShowNpcEventList")

		Obj* pSelfObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
		Obj* pTargetObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));

		if(pSelfObj && pTargetObj)
		{
			if(pSelfObj->GetObjType()==Obj::OBJ_TYPE_HUMAN&&
					pTargetObj->GetObjType() == Obj::OBJ_TYPE_MONSTER)
			{
				Obj_Monster* pMonster = (Obj_Monster*) pTargetObj;
				NPC_QUEST* pQuest = g_NpcQuestTbl.Get(pMonster->GetDataID());
				if(pQuest!=NULL)
				{
					for(int i = 0;i<pQuest->m_EventCount;i++)
					{
						pScene->GetLuaInterface()->ExeScript_DDD(pQuest->m_EventList[i],
																"OnEnumerate",
																sceneId,
																selfId,
																targetId);
					}
				}

			}
		}

		ENDSCENEDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-19
	*     函数名称 : LuaFnIsInDist
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					对方的ID编号,ObjID
	*				dist
	*					self与target距离
	*     功能说明 :
	*				判断selfId与targetId之间的距离是否在dist内(包括)
	*				目前用于护送任务,判断玩家和被护送者之间的距离
	*/
	INT LuaFnIsInDist(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		FLOAT dist;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		targetId		=	Lua_ValueToNumber(L,3);
		dist			=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnIsInDist")

		Obj* pSelfObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
		Obj* pTargetObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));

		if(pSelfObj && pTargetObj)
		{
			FLOAT fTDist = MySqrt(pSelfObj->getWorldPos(),pTargetObj->getWorldPos());
			if(fTDist <= dist)
			{
				Lua_PushNumber(L,TRUE);
				return 1;
			}
			else
			{
				Lua_PushNumber(L,FALSE);
				return 1;
			}
		}
	
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	
	/** -----------------------------------------------------------------------
	*     开发人员 : 王勇鸷
	*     编写时间 : 2008-3-17
	*     函数名称 : LuaFnAddEnemy
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				EnemyId
	*					仇人ID,无符号整型
	*     功能说明 :
	*				添加仇人
	*/	
	INT LuaFnAddEnemy(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t EnemyId;

		//sceneId			=	Lua_ValueToNumber(L,1);
		//selfId			=	Lua_ValueToNumber(L,2);
		//EnemyId			=	Lua_ValueToNumber(L,3);

		//BEGINHUMANDEFINE("LuaFnAddEnemy")
		//{
		//	HumanRelation* pMyRelation = pHuman->GetHumanRelation();
		//	GCRelation Msg;
		//	Msg.GetRelation()->CleanUp();
		//	INT nErr = 0;

		//	if( EnemyId == selfId )
		//	{
		//		g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self into blacklist.",
		//			pHuman->GetName() );
		//		return PACKET_EXE_CONTINUE;
		//	}

		//	//得到仇人指针
		//	Obj_Human* pEnemy = pScene->GetHumanManager()->GetHuman(EnemyId);		
		//	AssertEx(pEnemy,"Obj_Character到Obj_Human指针转换出错,LuaFnAddEnemy");
		//		
		//	// 看看目前关系表中已经有该玩家（不管是朋友还是黑名单）
		//	RELATION_TYPE RelationType;

		//	RelationType = pMyRelation->GetRelationType( pEnemy->GetGUID() );
		//	if (RelationType == RELATION_TYPE_NONE)
  //          {
		//		RelationType = RELATION_TYPE_STRANGER;
		//	}

		//	switch( RelationType )
		//	{
		//	case RELATION_TYPE_BLACKNAME:
		//	case RELATION_TYPE_MARRY:
		//	case RELATION_TYPE_MASTER:
		//	case RELATION_TYPE_PRENTICE:
		//	case RELATION_TYPE_BROTHER:
		//	case RELATION_TYPE_FRIEND:
		//		{ // 用转换
		//			if( pMyRelation->RelationTransition( RELATION_TYPE_ENEMY,pEnemy->GetGUID() ) != TRUE )
		//			{
		//				nErr = RET_ERR_RELATIONUNKNOWN;
		//			}
		//			else
		//			{
		//				Msg.GetRelation()->m_Type = RET_TRANSITION;

		//				RELATION_GUID_UCHAR_UCHAR& RelationInfo = Msg.GetRelation()->m_RelationGUIDUCHARUCHAR;
		//				RelationInfo.CleanUp();
		//				RelationInfo.SetTargetGUID( pEnemy->GetGUID() );
		//				RelationInfo.SetRelationType( RELATION_TYPE_ENEMY );
		//				RelationInfo.SetGroup( RELATION_GROUP_ENEMY );
		//				g_pLog->FastSaveLog( LOG_FILE_1, "LuaFnAddEnemy: Name=%s add GUID=%X to black list.",
		//				pHuman->GetName(), pEnemy->GetGUID() );
		//			}
		//		}
		//		break;
		//	case RELATION_TYPE_STRANGER:
		//		{ // 正确关系
		//			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
		//			GW_RELATION* pRelation = pMsg->GetRelation();
		//			pRelation->CleanUp();
		//			pRelation->m_Type = REQ_ADDTOENEMY;
		//			pRelation->SetGUID( pHuman->GetGUID() );
		//			REQUEST_ADD_RELATION& Relation = pRelation->m_AddRelation;
		//			Relation.CleanUp();
		//			Relation.SetTargetGUID( pEnemy->GetGUID() );
		//			Relation.SetTargetName( pEnemy->GetName() );
		//			Relation.SetRelationType( RELATION_TYPE_ENEMY );

		//			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		//			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add Name=%s to black list.",
		//				pHuman->GetName(), pEnemy->GetName() );

		//			return PACKET_EXE_CONTINUE;
		//		}
		//		break;
		//	default:
		//		Assert(FALSE);
		//		break;
		//	}
		//	if ( nErr )
		//	{
		//		Msg.GetRelation()->m_Type = nErr;

		//		g_pLog->FastSaveLog( LOG_FILE_1, "LuaFnAddEnemy: GUID=%X Error code %d.",
		//			pHuman->GetGUID(), nErr );
		//	}

		//	pHuman->GetPlayer()->SendPacket(&Msg);
	
		//}

		//ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0;
	}
	
	/** -----------------------------------------------------------------------
	*     开发人员 : 王勇鸷
	*     编写时间 : 2008-3-17
	*     函数名称 : LuaFnGetObjType
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					被调查者ID,无符号整型
	*     功能说明 :
	*				得到场景中指定OBJ的类型
	*/	
	INT LuaFnGetObjType(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetObjType")

			Lua_PushNumber( L,pCharacter->GetObjType() );
			return 1;

		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION	
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-15
	*     函数名称 : LuaFnGetDist
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					对方的ID编号,ObjID
	*     功能说明 :
	*				得到selfId和targetId之间的距离
	*/
	INT LuaFnGetDist(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		FLOAT dist;

		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		targetId		=	Lua_ValueToNumber(L,3);
		dist			=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnGetDist")

			Obj* pSelfObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
			Obj* pTargetObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));

			if(pSelfObj && pTargetObj)
			{
				FLOAT fTDist = MySqrt(pSelfObj->getWorldPos(),pTargetObj->getWorldPos());
				
				Lua_PushNumber(L,fTDist);
				return 1;
			}


		RETURNFALSE
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnGuid2ObjId
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				guid
	*					GUID,GUID_t类型，仅适用于玩家的GUID转换
	*     功能说明 :
	*				把Guid转换为objId
	*/
	INT LuaFnGuid2ObjId(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		GUID_t guid;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		guid		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGuid2ObjId")
		Obj_Character* pCharacter = (Obj_Character*)(g_pGUIDManager->Get(guid)) ;
		if( pCharacter )
		{
			if( pCharacter->GetObjType()==Obj::OBJ_TYPE_HUMAN )
			{
				INT objId = pCharacter->GetID();
				Lua_PushNumber(L,objId);
				return 1;
			}
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnObjId2Guid
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 : 返回selfId的GUID
	*/
	INT LuaFnObjId2Guid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnObjId2Guid")
		if( pHuman )
		{
			GUID_t guid = pHuman->GetGUID() ;
			Lua_PushNumber(L,guid);
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	///** -----------------------------------------------------------------------
	//*     开发人员 : 
	//*     编写时间 : 2005-10-21
	//*     函数名称 : LuaFnRemoveMonster
	//*     参数说明 : 
	//*     功能说明 :
	//*				SceneID
	//*					场景编号
	//*				selfId
	//*					调用者ID,无符号整型
	//*     功能说明 :
	//*				删除Obj_Monster,把Obj_Monster从地图删除
	//*/
	//INT LuaFnRemoveMonster(Lua_State* L)
	//{
	//	LUA_ENTER_FUNCTION
	
	//	SceneID_t sceneId;
	//	ObjID_t selfId;

	//	sceneId			=	Lua_ValueToNumber(L,1);
	//	selfId			=	Lua_ValueToNumber(L,2);

	//	AssertEx( FALSE, "此接口已经废弃，请使用LuaFnDeleteMonster" ) ;

	//	BEGINCHARCTERDEFINE("LuaFnRemoveMonster")
	//	pCharacter->SetActiveFlag( FALSE );
	//	pCharacter->CleanUp();
	//	pCharacter->getScene()->DeleteObject( pCharacter );

	//	return 0;
	//	ENDCHARCTERDEFINE

	//	LUA_LEAVE_FUNCTION
	//	return 0 ;
	//}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-2
	*     函数名称 : LuaFnAwardTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleIndex
	*					称号ID
	*				TitleId
	*					称号ID
	*     功能说明 :
	*				为selfId授予一个称号
	*/
	INT	LuaFnAwardTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT TitleIndex;
		ObjID_t TitleId;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		TitleIndex		=	Lua_ValueToNumber(L,3);
		TitleId			=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnAwardTitle")
			switch(TitleIndex)
			{
            case _TITLE::GUOJIA_TITLE:
				pHuman->SetCurCountryTitle(TitleId);
				break;
            case _TITLE::BANGPAI_TITLE:
				pHuman->SetCurGuildTitle(TitleId);
				break;
			case _TITLE::WANFA_TITLE:
				pHuman->SetCurNormalTitle(TitleId);
				break;
			default:
                Assert(0);
                g_pLog->FastSaveLog( LOG_FILE_1, "LuaFnAwardTitle: TitleIndex Error(%d), GamePlayer GUID=(%d)",
                    TitleIndex, pHuman->GetGUID());                
				break;
			}

		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-2
	*     函数名称 : LuaFnDeleteTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleIndex
	*					称号ID
	*     功能说明 :
	*				删除某个称号
	*/
	INT	LuaFnDeleteTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT TitleIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		TitleIndex		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDeleteTitle")

        pHuman->RemoveTitle(TitleIndex);

		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnDispatchAllTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				更新此人的所有称号
	*/
	INT	LuaFnDispatchAllTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnDispatchAllTitle")
		//PACKET_ALL_TITLES_MSG
        pHuman->UpdateTitlesToClient();

		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}



	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnGetTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleIndex
	*					称号ID
	*     功能说明 :
	*				获得此人的某个称号
	*/
	INT	LuaFnGetTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT TitleIndex ;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		TitleIndex		=	Lua_ValueToNumber(L,3);
		INT	TitleId		=	0;
		BEGINHUMANDEFINE("LuaFnGetTitle")
            switch(TitleIndex)
        {
            case _TITLE::GUOJIA_TITLE:
                TitleId = pHuman->GetCurCountryTitle();
                break;
            case _TITLE::BANGPAI_TITLE:
                TitleId = pHuman->GetCurGuildTitle();
                break;
            case _TITLE::WANFA_TITLE:
                TitleId = pHuman->GetCurNormalTitle();
                break;
            default:
                Assert(0);
                g_pLog->FastSaveLog( LOG_FILE_1, "LuaFnGetTitle: TitleIndex Error(%d), GamePlayer GUID=(%d)",
                    TitleIndex, pHuman->GetGUID());                
                break;
        }

		Lua_PushNumber(L,TitleId);
		return	1;

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnGetDyTitlesNum
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得此人的动态称号个数
	*/
	INT	LuaFnGetDyTitlesNum( Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId ;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		INT	titlenum	=	0;
		BEGINHUMANDEFINE("LuaFnGetDyTitlesNum")
			for(int i = 0; i<MAX_DYNAMIC_TITLE_SIZE; i++ )
			{
                //ModifyBy AMos 功能不明先注释了
				//if(pHuman->GetDyTitle(i))
				//{
				//	titlenum++;
				//}
			}
			Lua_PushNumber(L,titlenum);
			return	1;
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnAddDyTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleId
	*					称号ID
	*     功能说明 :
	*				加入新的动态称号，如果满了放在第一个
	*/
	INT	LuaFnAddDyTitle( Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t TitleId ;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		TitleId			=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnAddDyTitle")
			int i = 0;
			for(i = 0; i<MAX_DYNAMIC_TITLE_SIZE; i++ )
			{
                //ModifyBy AMos 动态称号功能不明先注释了
				//if(pHuman->GetDyTitle(i) == 0)
				//{
				//	pHuman->SetDyTitle(i, TitleId); 
				//	break;
				//}
			}
			if(i == MAX_DYNAMIC_TITLE_SIZE)
			{//覆盖第一个title
                //ModifyBy AMos 动态称号功能不明先注释了
				//pHuman->SetDyTitle(0, TitleId); 
			}

		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardNickTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					签名档性质的昵称
	*     功能说明 :
	*				设置此人的签名档性质的昵称
	*/
	INT LuaFnAwardNickTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		const CHAR* strText;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardNickTitle")
            //ModifyBy AMos 功能不明先注释了
			//pHuman->SetNickName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardSpouseTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					配偶名
	*     功能说明 :
	*				设置配偶名
	*/
	INT LuaFnAwardSpouseTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		const CHAR* strText;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardSpouseTitle")
            //ModifyBy AMos 功能不明先注释了
			//pHuman->SetSpouseName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardJieBaiTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					结拜称号
	*     功能说明 :
	*				设置结拜称号
	*/
	INT LuaFnAwardJieBaiTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		const CHAR* strText;
		
			sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardJieBaiTitle")
            //ModifyBy AMos 功能不明先注释了
			//pHuman->SetJieBaiName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}



	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardBangPaiTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					帮派名
	*     功能说明 :
	*				设置此人的帮派称号
	*/
	INT LuaFnAwardBangPaiTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		const CHAR* strText;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardBangPaiTitle")
			pHuman->SetGuildTitleName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE
		
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardShiTuTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					帮派名
	*     功能说明 :
	*				设置此人的帮派称号
	*/
	INT LuaFnAwardShiTuTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		const CHAR* strText;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardShiTuTitle")
            //ModifyBy AMos 功能不明先注释了
			//pHuman->SetShiTuName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardShangDianTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					帮派名
	*     功能说明 :
	*				设置此人的帮派称号
	*/
	INT LuaFnAwardShangDianTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		const CHAR* strText;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardShangDianTitle")
            //ModifyBy AMos 功能不明先注释了
			//pHuman->SetShangDianName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-8
	*     函数名称 : LuaFnEnableBankRentIndex
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				RentIndex
	*					打开的租赁箱
	*     功能说明 :
	*				打开此人的租赁箱
	*/
	INT LuaFnEnableBankRentIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		INT RentIndex;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		RentIndex		=	Lua_ValueToNumber(L,3);

		if(RentIndex<0&&RentIndex>5)
			return 0;
		BEGINHUMANDEFINE("LuaFnEnableBankRentIndex")
			//pHuman->SetBangPaiName(strText, (BYTE)strlen(strText));
			INT	CurEndIndex = pHuman->__GetBankEndIndex();
		switch(RentIndex)
		{
		case 1:
			{
			}
			break;
		case 2:
			{
				if(CurEndIndex <= RENTBOX2_START_INDEX)
					pHuman->__SetBankEndIndex(RENTBOX3_START_INDEX);
			}
			break;
		case 3:
			{
				if(CurEndIndex <= RENTBOX3_START_INDEX)
					pHuman->__SetBankEndIndex(RENTBOX4_START_INDEX);
			}
			break;
		case 4:
			{
				if(CurEndIndex <= RENTBOX4_START_INDEX)
					pHuman->__SetBankEndIndex(RENTBOX5_START_INDEX);
			}
			break;
		case 5:
			{
				if(CurEndIndex <= RENTBOX5_START_INDEX)
					pHuman->__SetBankEndIndex(MAX_BANK_SIZE);
			}
			break;
		default:
			{
			}
			break;
		}
		ENDHUMANDEFINE
		return 0;
		LUA_LEAVE_FUNCTION

			return 0 ;

	}

	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2006-1-12
	*     函数名称 : LuaFnGetBankRentIndex
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得当前租赁箱已打开的索引
	*/
	INT LuaFnGetBankRentIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId ;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetBankRentIndex")
		INT	CurEndIndex = pHuman->__GetBankEndIndex();
		Lua_PushNumber(L, CurEndIndex);
		return 1;
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2005-11-8
	*     函数名称 : LuaFnBankBegin
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				通知客户端开始进行银行交易
	*/
	INT LuaFnBankBegin(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		ObjID_t targetId;
		
		sceneId			=	Lua_ValueToNumber(L,1);
		selfId			=	Lua_ValueToNumber(L,2);
		targetId		=	Lua_ValueToNumber(L,3);
		BEGINHUMANDEFINE("LuaFnBankBegin")
		GCBankBegin Msg;
		Player* pPlayer = pHuman->GetPlayer();
        if (pPlayer)
        {
            Msg.SetNPCObjID(targetId);
            if (pHuman->__IsPasswordProtect(MINORPASSWD_INFO::PROTECT_BANK))
                Msg.SetBankPwdFlag(TRUE);
            else
                Msg.SetBankPwdFlag(FALSE);

            pPlayer->SendPacket( &Msg ) ;
        }

		ENDHUMANDEFINE

		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}


    /** -----------------------------------------------------------------------
    *     开发人员 : amos
    *     编写时间 : 2008-5-14
    *     函数名称 : LuaFnExpandBankSize
    *     参数说明 : 
    *				SceneID
    *					场景编号
    *				selfId
    *					调用者ID,无符号整型
    *               nSize
    *                   扩展的格子数
    *     功能说明 :
    *				扩展银行格子数
    */
    INT LuaFnExpandBankSize(Lua_State* L)
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT nSize;
        
        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        nSize	    	=	Lua_ValueToNumber(L,3);
        BEGINHUMANDEFINE("LuaFnExpandBankSize")

        GCExpandBankSize Msg;
        Player* pPlayer = pHuman->GetPlayer();

        INT nCurSize = pHuman->__GetBankSize();
        if ((nCurSize+nSize) <= MAX_BANK_SIZE)
        {
            pHuman->__ExpandBankSize(nSize);
            if (pPlayer)
            {
                Msg.SetBankSize(nSize);
                pPlayer->SendPacket( &Msg ) ;
            }
        }
        else
        {
            Log::SaveLog(SERVER_ERRORFILE,"Lua_Call Error! BankSize=%d > MAX_BANK_SIZE", (nCurSize+nSize));
        }

        ENDHUMANDEFINE

            return	0;
        LUA_LEAVE_FUNCTION
            return 0 ;
    }


    /** -----------------------------------------------------------------------
    *     开发人员 : amos
    *     编写时间 : 2008-5-14
    *     函数名称 : LuaFnGetBankSize
    *     参数说明 : 
    *				SceneID
    *					场景编号
    *				selfId
    *					调用者ID,无符号整型
    *     功能说明 :
    *				银行格子数
    */
    INT LuaFnGetBankSize(Lua_State* L)
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        
        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        BEGINHUMANDEFINE("LuaFnGetBankSize")

        GCBankSize Msg;
        Player* pPlayer = pHuman->GetPlayer();
        INT nSize = pHuman->__GetBankSize();
        if (pPlayer)
        {
            Msg.SetBankSize(nSize);
            pPlayer->SendPacket( &Msg ) ;
        }

        ENDHUMANDEFINE

            return	0;
        LUA_LEAVE_FUNCTION
            return 0 ;
    }

	/** -----------------------------------------------------------------------
	*     开发人员 : wangpojin
	*     编写时间 : 2005-11-14
	*     函数名称 : LuaSetPlayerDefaultReliveInfo
	*     参数说明 : 
	*				sceneId
	*					场景编号
	*				playerId
	*					进入场景的玩家ID
	*				pszHP
	*					HP的数据(%10=HP恢复10/100，10=HP恢复10点)
	*				pszMP
	*					MP的数据(%10=HP恢复10/100，10=HP恢复10点)
	*				pszSP
	*					SP的数据(%10=HP恢复10/100，10=HP恢复10点)
	*				defaultSceneID
	*					复活的场景ID
	*				defaultPosX
	*					坐标X
	*				defaultPosZ
	*					坐标Z
	*     功能说明 :
	*				玩家的缺省复活相关的数据设置
	*/
	INT LuaSetPlayerDefaultReliveInfo(Lua_State* L )
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		const char *pszHP;
		const char *pszMP;
		const char *pszSP;
		SceneID_t defaultSceneID;
		FLOAT defaultPosX;
		FLOAT defaultPosZ;
		
		sceneId			= Lua_ValueToNumber( L, 1 );
		selfId		= Lua_ValueToNumber( L, 2 );
		pszHP			= Lua_ValueToString( L, 3 );
		pszMP			= Lua_ValueToString( L, 4 );
		pszSP			= Lua_ValueToString( L, 5 );
		defaultSceneID	= Lua_ValueToNumber( L, 6 );
		defaultPosX		= (FLOAT)(Lua_ValueToNumber( L, 7 ));
		defaultPosZ		= (FLOAT)(Lua_ValueToNumber( L, 8 ));

		BEGINHUMANDEFINE("LuaSetPlayerDefaultReliveInfo")

		Obj_Human::_RELIVE_INFO infoRelive;
		infoRelive.m_bHPUsePercent = (pszHP[0] == '%')?(TRUE):(FALSE);
		infoRelive.m_bMPUsePercent = (pszMP[0] == '%')?(TRUE):(FALSE);
		infoRelive.m_bSPUsePercent = (pszSP[0] == '%')?(TRUE):(FALSE);
		if ( infoRelive.m_bHPUsePercent )
		{
			infoRelive.m_nHPPercent	= atoi( &pszHP[1] );
		}
		else
		{
			infoRelive.m_nHP		= atoi( pszHP );
		}

		if ( infoRelive.m_bMPUsePercent )
		{
			infoRelive.m_nMPPercent	= atoi( &pszMP[1] );
		}
		else
		{
			infoRelive.m_nMP		= atoi( pszMP );
		}

		if ( infoRelive.m_bSPUsePercent )
		{
			infoRelive.m_nSPPercent	= atoi( &pszSP[1] );
		}
		else
		{
			infoRelive.m_nSP		= atoi( pszSP );
		}

		infoRelive.m_SceneID		= defaultSceneID;
		infoRelive.m_Pos.m_fX		= defaultPosX;
		infoRelive.m_Pos.m_fZ		= defaultPosZ;

		pHuman->SetReliveInfo( FALSE, &infoRelive );

		ENDHUMANDEFINE
		return	0;

	LUA_LEAVE_FUNCTION

		return 0 ;

	}

	/** -----------------------------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-3-20
	*     函数名称 : LuaGetDayTime
	*     参数说明 : 
	*					无
	*     功能说明 :
	*				取得当前时间的天数，例如：1140 表示2001年的第140天
	*										  6099 表示2006年的第99天
	*/
	INT LuaGetDayTime(Lua_State* L )
	{
	LUA_ENTER_FUNCTION

		UINT uRet = g_pTimeManager->GetDayTime() ;

		Lua_PushNumber(L, uRet) ;
		return	1;

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : jack
	*     编写时间 : 2006-3-23
	*     函数名称 : LuaGetObjCreateTime
	*     参数说明 : 
	*				sceneId
    *					调用该函数场景ID,short类型
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				取得ObjID 为 selfId的obj的创建时间
	*					
	*/
	INT LuaGetObjCreateTime(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;

		sceneId		= Lua_ValueToNumber( L, 1 );
		selfId		= Lua_ValueToNumber( L, 2 );

		BEGINSCENEDEFINE("LuaGetObjCreateTime")
		Obj* pObj = (Obj*)(pScene->GetObjManager()->GetObj(selfId));
		if( pObj )
		{
			UINT uRet = pObj->GetCreateTime() ;
			Lua_PushNumber(L, uRet) ;
			return	1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     开发人员 : sunyu
	*     编写时间 : 2006-3-28
	*     函数名称 : LuaFnUpdateAttr
	*     参数说明 : 
	*				sceneId
	*					调用该函数场景ID,short类型
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				通过邮件系统更新人物属性
	*					
	*/
	INT LuaFnUpdateAttr(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ObjID_t selfId;
		UINT param0;
		UINT param1;
		UINT param2;
		UINT param3 ;

		sceneId		= Lua_ValueToNumber( L, 1 );
		selfId		= Lua_ValueToNumber( L, 2 );
		param0		= Lua_ValueToNumber( L, 3 );	 
		param1		= Lua_ValueToNumber( L, 4 );
		param2		= Lua_ValueToNumber( L, 5 );
		param3		= Lua_ValueToNumber( L, 6 );

		BEGINHUMANDEFINE("LuaFnUpdateAttr")
		switch(param1)
		{
		case MAIL_ATTR_GUILD:
			{
				pHuman->SetGuildID(param2);
				if(param2 == INVALID_ID)
				{
					pHuman->SetGuildTitleName("",0);
					pHuman->UpdateTitlesToClient();
				}
			}
			break;
		case MAIL_ATTR_MONEY:
			{
				pHuman->SetMoney(pHuman->GetMoney()+(INT)param2);
				//记录
				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	param3;
				MoneyLogParam.TargetGUID	=	pHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_BUY_PLAYERSHOP_OUTCOME;	
				MoneyLogParam.Count		=	param2;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);
			}
		default:
			break;
		}

		ENDHUMANDEFINE
		return 0 ;
		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-4-26	17:08
	 *	函数名称：	LuaFnIsPasswordSetup
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				flag
	 *					标记，仅当玩家未设置二级密码有效，如果 1，则让调用者设置二级密码，0 则不需要设置
	 *
	 *	功能说明：	判断玩家是否设置二级密码，0 为未设置，1 为已设置
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnIsPasswordSetup(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		BOOL flag; 

		sceneId		= Lua_ValueToNumber( L, 1 );
		selfId		= Lua_ValueToNumber( L, 2 );
		flag		= Lua_ValueToNumber( L, 3 );	 

		BEGINHUMANDEFINE("LuaFnIsPasswordSetup")

			if( pHuman->__IsPasswordSetup() )
			{
				Lua_PushNumber(L, TRUE);
			}
			else
			{
				if( flag )
				{ // 调用接口发消息 OR_NEED_SETMINORPASSWORD
					pHuman->SendOperateResultMsg( OR_NEED_SETMINORPASSWORD );
				}

				Lua_PushNumber(L, FALSE);
			}

			return 1;

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	开发人员：	胡繁
	 *	编写时间：	2006-4-26	17:21
	 *	函数名称：	LuaFnIsPasswordUnlocked
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				flag
	 *					标记，仅当玩家未解锁二级密码有效，如果 1，则让调用者解锁二级密码，0 则不强制解锁
	 *
	 *	功能说明：	判断玩家是否解锁二级密码，0 为未解锁，1 为已解锁
	 *	修改记录：
	*****************************************************************************/
	INT LuaFnIsPasswordUnlocked(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId;
		ObjID_t selfId;
		BOOL flag ;
		

		sceneId		= Lua_ValueToNumber( L, 1 );
		selfId		= Lua_ValueToNumber( L, 2 );
		flag		= Lua_ValueToNumber( L, 3 );	 

		BEGINHUMANDEFINE("LuaFnIsPasswordUnlocked")

			if( pHuman->__IsPasswordUnlock() )
			{
				Lua_PushNumber(L, TRUE);
			}
			else
			{
				if( flag )
				{ // 调用接口发消息 OR_NEED_UNLOCKMINORPASSWORD
					pHuman->SendOperateResultMsg( OR_NEED_UNLOCKMINORPASSWORD );
				}

				Lua_PushNumber(L, FALSE);
			}

			return 1;

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 王勇鸷
	*     编写时间 : 2008-5-20
	*     函数名称 : LuaFnStartupSystemTimer
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				TimerIndex
	*					系统定时器表里面的index
	*     功能说明 :
    *				启动系统定时器事件
	*/
	INT LuaFnStartupSystemTimer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT TimerIndex;

		sceneId = Lua_ValueToNumber(L,1);
		TimerIndex = Lua_ValueToNumber(L,2);

		Assert( TimerIndex<MAX_TIMER_DOING_NUM );

		BEGINSCENEDEFINE("LuaFnStartupSystemTimer")

		pScene->GetTimerDoingManager()->SetActive(TRUE,TimerIndex);

		ENDSCENEDEFINE

		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 王勇鸷
	*     编写时间 : 2008-5-20
	*     函数名称 : LuaFnStopSystemTimer
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				TimerIndex
	*					系统定时器表里面的index
	*     功能说明 :
    *				停止系统定时器事件
	*/
	INT LuaFnStopSystemTimer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT TimerIndex;

		sceneId = Lua_ValueToNumber(L,1);
		TimerIndex = Lua_ValueToNumber(L,2);
		
		Assert( TimerIndex<MAX_TIMER_DOING_NUM );

		BEGINSCENEDEFINE("LuaFnStopSystemTimer")

		pScene->GetTimerDoingManager()->SetActive(FALSE,TimerIndex);

		ENDSCENEDEFINE

		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 王勇鸷
	*     编写时间 : 2008-5-20
	*     函数名称 : LuaFnSetSystemTimerTick
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				TimerIndex
	*					系统定时器表里面的index
	*				tick
	*					心跳时间
	*     功能说明 :
    *				设置系统定时器心跳时间
	*/
	INT LuaFnSetSystemTimerTick(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		ScriptID_t scriptId;
		const CHAR* funcName;
		INT TimerIndex;
		INT tick;

		sceneId = Lua_ValueToNumber(L,1);
		scriptId = Lua_ValueToNumber(L,2);
		funcName = Lua_ValueToString(L,3);
		TimerIndex = Lua_ValueToNumber(L,4);
		tick = Lua_ValueToNumber(L,5);

		Assert( TimerIndex<MAX_TIMER_DOING_NUM );

		BEGINSCENEDEFINE("LuaFnSetSystemTimerTick")

		if ( pScene->GetTimerDoingManager()->GetActive(TimerIndex) == TRUE )
		{
			pScene->GetSceneTimer()->StartSystemTimer(TimerIndex,scriptId,funcName,tick);

		}

		ENDSCENEDEFINE
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 王勇鸷
	*     编写时间 : 2008-5-20
	*     函数名称 : LuaFnSetSystemTimerParam
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				TimerIndex
	*					系统定时器表里面的index
	*				paramIndex
	*					计时器参数的index
	*				val
	*					对应位置的值
	*     功能说明 :
    *				设置系统定时器参数
	*/
	INT LuaFnSetSystemTimerParam(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT TimerIndex;
		INT paramIndex;
		INT val;

		sceneId = Lua_ValueToNumber(L,1);
		TimerIndex = Lua_ValueToNumber(L,2);
		paramIndex = Lua_ValueToNumber(L,3);
		val = Lua_ValueToNumber(L,4);

		Assert( TimerIndex<MAX_TIMER_DOING_NUM && paramIndex<MAX_TIEMR_PARAM_NUM );

		BEGINSCENEDEFINE("LuaFnSetSystemTimerParam")

		if ( pScene->GetTimerDoingManager()->GetActive(TimerIndex) == TRUE )
		{	
			pScene->GetTimerDoingManager()->SetTimerParam(val,TimerIndex,paramIndex);
		}

		ENDSCENEDEFINE

		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	
	/** -----------------------------------------------------------------------
	*     开发人员 : 王勇鸷
	*     编写时间 : 2008-5-20
	*     函数名称 : LuaFnGetSystemTimerParam
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				TimerIndex
	*					系统定时器表里面的index
	*				paramIndex
	*					计时器参数的index
	*     功能说明 :
    *				得到系统定时器参数
	*/
	INT LuaFnGetSystemTimerParam(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT TimerIndex;
		INT paramIndex;

		sceneId = Lua_ValueToNumber(L,1);
		TimerIndex = Lua_ValueToNumber(L,2);
		paramIndex = Lua_ValueToNumber(L,3);

		Assert( TimerIndex<MAX_TIMER_DOING_NUM && paramIndex<MAX_TIEMR_PARAM_NUM );

		BEGINSCENEDEFINE("LuaFnGetSystemTimerParam")
		
		Lua_PushNumber( L,pScene->GetTimerDoingManager()->GetTimerParam(TimerIndex,paramIndex) );

		ENDSCENEDEFINE
		return 1;

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     开发人员 : 王勇鸷
	*     编写时间 : 2008-5-20
	*     函数名称 : LuaFnCheckSystemTimerValidity
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				TimerIndex
	*					系统定时器表里面的index
	*     功能说明 :
    *				检查系统定时器是否过期
	*/
	INT LuaFnCheckSystemTimerValidity(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId;
		INT TimerIndex;

		sceneId = Lua_ValueToNumber(L,1);
		TimerIndex = Lua_ValueToNumber(L,2);

		Assert( TimerIndex<MAX_TIMER_DOING_NUM );
		
		BEGINSCENEDEFINE("LuaFnCheckSystemTimerValidity")

		if ( pScene->GetTimerDoingManager()->CheckSystemTimerValidity(TimerIndex) == TRUE ) 
		{
			//未过期
			Lua_PushNumber( L,1 );
			return 1;		
		}		

		//过期
		Lua_PushNumber( L,0 );
		return 1;

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

    /** -----------------------------------------------------------------------
    *     开发人员 : amos
    *     编写时间 : 2008-5-27
    *     函数名称 : LuaAddActionPoint
    *     参数说明 : 
    *     功能说明 :
    *				SceneID
    *					场景编号
    *				selfId
    *					调用者ID,无符号整型
    *				nAP 
    *					行动力
    *				
    */
    INT	LuaAddActionPoint(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT nAP ;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        nAP      		=	Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaAddActionPoint")

        pHuman->SendMsg_RefeshAP(nAP, 0);// 0 增加

        ENDHUMANDEFINE
        return	0;

        LUA_LEAVE_FUNCTION
        return  0;
    }


    /** -----------------------------------------------------------------------
    *     开发人员 : amos
    *     编写时间 : 2008-5-27
    *     函数名称 : LuaSubActionPoint
    *     参数说明 : 
    *     功能说明 :
    *				SceneID
    *					场景编号
    *				selfId
    *					调用者ID,无符号整型
    *				nAP 
    *					行动力
    *				
    */
    INT	LuaSubActionPoint(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;
        INT nAP ;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);
        nAP      		=	Lua_ValueToNumber(L,3);

        BEGINHUMANDEFINE("LuaSubActionPoint")

            pHuman->SendMsg_RefeshAP(nAP, 1);// 1 减少

        ENDHUMANDEFINE
            return	0;

        LUA_LEAVE_FUNCTION
            return  0;
    }

    /** -----------------------------------------------------------------------
    *     开发人员 : amos
    *     编写时间 : 2008-5-28
    *     函数名称 : LuaCountryDeletaBegin
    *     参数说明 : 
    *     功能说明 :
    *				SceneID
    *					场景编号
    *				selfId
    *					调用者ID,无符号整型
    *				
    */
    INT	LuaCountryDeletaBegin(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);

        BEGINHUMANDEFINE("LuaCountryDeletaBegin")


        GWCountryDelateRequest *pMsg = (GWCountryDelateRequest *)g_pPacketFactoryManager->CreatePacket(PACKET_GW_COUNTRY_DELATE_REQUEST);
        pMsg->SetGuid(pHuman->GetGUID());
        g_pServerManager->SendPacket( pMsg, INVALID_ID );

        ENDHUMANDEFINE
            return	0;

        LUA_LEAVE_FUNCTION
            return  0;
    }

    /** -----------------------------------------------------------------------
    *     开发人员 : amos
    *     编写时间 : 2008-5-28
    *     函数名称 : LuaCountryVoteBegin
    *     参数说明 : 
    *     功能说明 :
    *				SceneID
    *					场景编号
    *				selfId
    *					调用者ID,无符号整型
    *				
    */
    INT	LuaCountryVoteBegin(Lua_State* L) 
    {
        LUA_ENTER_FUNCTION
        
        SceneID_t sceneId;
        ObjID_t selfId;

        sceneId			=	Lua_ValueToNumber(L,1);
        selfId			=	Lua_ValueToNumber(L,2);

        BEGINHUMANDEFINE("LuaCountryVoteBegin")

        GWCountryVoteRequest *pMsg = (GWCountryVoteRequest *)g_pPacketFactoryManager->CreatePacket(PACKET_GW_COUNTRY_VOTE_REQUEST);
        pMsg->SetGuid(pHuman->GetGUID());
        g_pServerManager->SendPacket( pMsg, INVALID_ID );

        ENDHUMANDEFINE
            return	0;

        LUA_LEAVE_FUNCTION
            return  0;
    }


}

#endif