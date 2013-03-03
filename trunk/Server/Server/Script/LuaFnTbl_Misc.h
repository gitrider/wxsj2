//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Misc.h
// ģ��	:	Script
// ����	 :  ������غ���
// �޸���ʷ:
//	yangfei����
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
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetTimer
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 selfId
	*					������ID,�޷�������
	*				 scriptId
	*					ִ�нű�ID,short����
	*				 funcName
	*					��ʱ���ص���������,�ַ�������
	*				 tickTime
	*					��ʱ�����ü��ʱ��
	*     ����˵�� :
	*				 ����Ҵ򿪼�ʱ��,ʱ�䵽ʱ���Զ�����OnTimer����	
	*				 ��ұ������ϲ�û�м�ʱ�����ű�������Ҫ��ʱ��ʱ��ϵͳ���롰���á�����ϵͳ����
	*                ���أ�1,��ʾ����
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
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnStopTimer
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				sceneTimerIndex
	*					��ʱ���ڳ�����ʱ�����е������±�,��SetTimerʱ����		
	*     ����˵�� :
    *				�رյ�ǰ��ҵļ�ʱ��	//ÿ��������������԰�������ʱ��
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
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-24
	*     �������� : LuaFnCheckTimer
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 TimeIndex
	*					��ʱ������
	*     ����˵�� : �ж�TimeIndex�ż�ʱ���Ƿ��Ѿ���ʹ����
	*                ����1,��ʾ��ʹ�ã�����0����ʾû��ʹ��; ����-1����ʾ�쳣
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
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetGameTime
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
    *				��ȡ��ǰ�������ʱ�䣬��λΪ��
	*				�õ�ǰʱ���ȥ����ʱ��
	*				û�в���
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
	*     ������Ա : �¾�
	*     ��дʱ�� : 2010-7-30
	*     �������� : LuaFnGetTickCount
	*     ����˵�� : 
	*				L
	*				LuaStateִ�л���ָ��
	*     ����˵�� :
	*/
	INT LuaFnGetTickCount(Lua_State* L)	
	{	
		Lua_PushNumber(L, g_pTimeManager->CurrentTime());

		return 1;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-1-10
	*     �������� : LuaFnGetCurrentTime
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*     ����˵�� :
	*				��ȡ��ǰʱ�䣬��λΪ��
	*/
	INT LuaFnGetCurrentTime(Lua_State* L)	
	{	
		Lua_PushNumber(L, g_pTimeManager->GetANSITime());
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : �����
	*     ��дʱ�� : 2008-4-21
	*     �������� : LuaFnIsInStall
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				����Ƿ��ڰ�̯״̬
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
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddGlobalCountNews
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				newstr
	*					��������,�ַ���ָ������
	*				count
	*					���ʹ���
	*     ����˵�� :
    *				��ȫ��������������Ź��棬���淢��ָ������
	*				û�в���
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
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnPrintNum
	*     ����˵�� : 
	*				num
	*				��Ϣ�����ʾ����
	*     ����˵�� :
	*				������Ϣ��,���Խű���
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
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnPrintStr
	*     ����˵�� : 
	*				strText
	*				��Ϣ�����ʾ�ַ���
	*     ����˵�� :
	*				������Ϣ��,���Խű���
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
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnCallScriptFunction
	*     ����˵�� : 
	*     ����˵�� :
	*					���ÿɱ����������ű�����
	*/
	INT LuaFnCallScriptFunction(Lua_State* L,...) 
	{
		LUA_ENTER_FUNCTION
		
		INT argc = Lua_GetTopIndex(L);

		AssertEx(argc>=3,"��������С��3");

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
			tsnprintf(buf,32,"�ļ���%d",scriptId);
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
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnShowNpcEventList
	*     ����˵�� : 
	*     ����˵�� :
	*				���ÿɱ����������ű�����
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
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-19
	*     �������� : LuaFnIsInDist
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					�Է���ID���,ObjID
	*				dist
	*					self��target����
	*     ����˵�� :
	*				�ж�selfId��targetId֮��ľ����Ƿ���dist��(����)
	*				Ŀǰ���ڻ�������,�ж���Һͱ�������֮��ľ���
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
	*     ������Ա : �����
	*     ��дʱ�� : 2008-3-17
	*     �������� : LuaFnAddEnemy
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				EnemyId
	*					����ID,�޷�������
	*     ����˵�� :
	*				��ӳ���
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

		//	//�õ�����ָ��
		//	Obj_Human* pEnemy = pScene->GetHumanManager()->GetHuman(EnemyId);		
		//	AssertEx(pEnemy,"Obj_Character��Obj_Humanָ��ת������,LuaFnAddEnemy");
		//		
		//	// ����Ŀǰ��ϵ�����Ѿ��и���ң����������ѻ��Ǻ�������
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
		//		{ // ��ת��
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
		//		{ // ��ȷ��ϵ
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
	*     ������Ա : �����
	*     ��дʱ�� : 2008-3-17
	*     �������� : LuaFnGetObjType
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					��������ID,�޷�������
	*     ����˵�� :
	*				�õ�������ָ��OBJ������
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
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-15
	*     �������� : LuaFnGetDist
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					�Է���ID���,ObjID
	*     ����˵�� :
	*				�õ�selfId��targetId֮��ľ���
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
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnGuid2ObjId
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				guid
	*					GUID,GUID_t���ͣ�����������ҵ�GUIDת��
	*     ����˵�� :
	*				��Guidת��ΪobjId
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
	*     ������Ա : jack
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnObjId2Guid
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� : ����selfId��GUID
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
	//*     ������Ա : 
	//*     ��дʱ�� : 2005-10-21
	//*     �������� : LuaFnRemoveMonster
	//*     ����˵�� : 
	//*     ����˵�� :
	//*				SceneID
	//*					�������
	//*				selfId
	//*					������ID,�޷�������
	//*     ����˵�� :
	//*				ɾ��Obj_Monster,��Obj_Monster�ӵ�ͼɾ��
	//*/
	//INT LuaFnRemoveMonster(Lua_State* L)
	//{
	//	LUA_ENTER_FUNCTION
	
	//	SceneID_t sceneId;
	//	ObjID_t selfId;

	//	sceneId			=	Lua_ValueToNumber(L,1);
	//	selfId			=	Lua_ValueToNumber(L,2);

	//	AssertEx( FALSE, "�˽ӿ��Ѿ���������ʹ��LuaFnDeleteMonster" ) ;

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
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-2
	*     �������� : LuaFnAwardTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleIndex
	*					�ƺ�ID
	*				TitleId
	*					�ƺ�ID
	*     ����˵�� :
	*				ΪselfId����һ���ƺ�
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
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-2
	*     �������� : LuaFnDeleteTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleIndex
	*					�ƺ�ID
	*     ����˵�� :
	*				ɾ��ĳ���ƺ�
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
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnDispatchAllTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				���´��˵����гƺ�
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
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnGetTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleIndex
	*					�ƺ�ID
	*     ����˵�� :
	*				��ô��˵�ĳ���ƺ�
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
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnGetDyTitlesNum
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ô��˵Ķ�̬�ƺŸ���
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
                //ModifyBy AMos ���ܲ�����ע����
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
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnAddDyTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleId
	*					�ƺ�ID
	*     ����˵�� :
	*				�����µĶ�̬�ƺţ�������˷��ڵ�һ��
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
                //ModifyBy AMos ��̬�ƺŹ��ܲ�����ע����
				//if(pHuman->GetDyTitle(i) == 0)
				//{
				//	pHuman->SetDyTitle(i, TitleId); 
				//	break;
				//}
			}
			if(i == MAX_DYNAMIC_TITLE_SIZE)
			{//���ǵ�һ��title
                //ModifyBy AMos ��̬�ƺŹ��ܲ�����ע����
				//pHuman->SetDyTitle(0, TitleId); 
			}

		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardNickTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					ǩ�������ʵ��ǳ�
	*     ����˵�� :
	*				���ô��˵�ǩ�������ʵ��ǳ�
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
            //ModifyBy AMos ���ܲ�����ע����
			//pHuman->SetNickName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardSpouseTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					��ż��
	*     ����˵�� :
	*				������ż��
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
            //ModifyBy AMos ���ܲ�����ע����
			//pHuman->SetSpouseName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardJieBaiTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					��ݳƺ�
	*     ����˵�� :
	*				���ý�ݳƺ�
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
            //ModifyBy AMos ���ܲ�����ע����
			//pHuman->SetJieBaiName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}



	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardBangPaiTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					������
	*     ����˵�� :
	*				���ô��˵İ��ɳƺ�
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
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardShiTuTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					������
	*     ����˵�� :
	*				���ô��˵İ��ɳƺ�
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
            //ModifyBy AMos ���ܲ�����ע����
			//pHuman->SetShiTuName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardShangDianTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					������
	*     ����˵�� :
	*				���ô��˵İ��ɳƺ�
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
            //ModifyBy AMos ���ܲ�����ע����
			//pHuman->SetShangDianName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-8
	*     �������� : LuaFnEnableBankRentIndex
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				RentIndex
	*					�򿪵�������
	*     ����˵�� :
	*				�򿪴��˵�������
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
	*     ������Ա : sunyu
	*     ��дʱ�� : 2006-1-12
	*     �������� : LuaFnGetBankRentIndex
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��õ�ǰ�������Ѵ򿪵�����
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
	*     ������Ա : sunyu
	*     ��дʱ�� : 2005-11-8
	*     �������� : LuaFnBankBegin
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				֪ͨ�ͻ��˿�ʼ�������н���
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
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-14
    *     �������� : LuaFnExpandBankSize
    *     ����˵�� : 
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *               nSize
    *                   ��չ�ĸ�����
    *     ����˵�� :
    *				��չ���и�����
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
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-14
    *     �������� : LuaFnGetBankSize
    *     ����˵�� : 
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *     ����˵�� :
    *				���и�����
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
	*     ������Ա : wangpojin
	*     ��дʱ�� : 2005-11-14
	*     �������� : LuaSetPlayerDefaultReliveInfo
	*     ����˵�� : 
	*				sceneId
	*					�������
	*				playerId
	*					���볡�������ID
	*				pszHP
	*					HP������(%10=HP�ָ�10/100��10=HP�ָ�10��)
	*				pszMP
	*					MP������(%10=HP�ָ�10/100��10=HP�ָ�10��)
	*				pszSP
	*					SP������(%10=HP�ָ�10/100��10=HP�ָ�10��)
	*				defaultSceneID
	*					����ĳ���ID
	*				defaultPosX
	*					����X
	*				defaultPosZ
	*					����Z
	*     ����˵�� :
	*				��ҵ�ȱʡ������ص���������
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
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-20
	*     �������� : LuaGetDayTime
	*     ����˵�� : 
	*					��
	*     ����˵�� :
	*				ȡ�õ�ǰʱ������������磺1140 ��ʾ2001��ĵ�140��
	*										  6099 ��ʾ2006��ĵ�99��
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
	*     ������Ա : jack
	*     ��дʱ�� : 2006-3-23
	*     �������� : LuaGetObjCreateTime
	*     ����˵�� : 
	*				sceneId
    *					���øú�������ID,short����
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				ȡ��ObjID Ϊ selfId��obj�Ĵ���ʱ��
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
	*     ������Ա : sunyu
	*     ��дʱ�� : 2006-3-28
	*     �������� : LuaFnUpdateAttr
	*     ����˵�� : 
	*				sceneId
	*					���øú�������ID,short����
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				ͨ���ʼ�ϵͳ������������
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
				//��¼
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
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-4-26	17:08
	 *	�������ƣ�	LuaFnIsPasswordSetup
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				flag
	 *					��ǣ��������δ���ö���������Ч����� 1�����õ��������ö������룬0 ����Ҫ����
	 *
	 *	����˵����	�ж�����Ƿ����ö������룬0 Ϊδ���ã�1 Ϊ������
	 *	�޸ļ�¼��
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
				{ // ���ýӿڷ���Ϣ OR_NEED_SETMINORPASSWORD
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
	 *	������Ա��	����
	 *	��дʱ�䣺	2006-4-26	17:21
	 *	�������ƣ�	LuaFnIsPasswordUnlocked
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				flag
	 *					��ǣ��������δ��������������Ч����� 1�����õ����߽����������룬0 ��ǿ�ƽ���
	 *
	 *	����˵����	�ж�����Ƿ�����������룬0 Ϊδ������1 Ϊ�ѽ���
	 *	�޸ļ�¼��
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
				{ // ���ýӿڷ���Ϣ OR_NEED_UNLOCKMINORPASSWORD
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
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnStartupSystemTimer
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*     ����˵�� :
    *				����ϵͳ��ʱ���¼�
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
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnStopSystemTimer
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*     ����˵�� :
    *				ֹͣϵͳ��ʱ���¼�
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
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnSetSystemTimerTick
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*				tick
	*					����ʱ��
	*     ����˵�� :
    *				����ϵͳ��ʱ������ʱ��
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
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnSetSystemTimerParam
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*				paramIndex
	*					��ʱ��������index
	*				val
	*					��Ӧλ�õ�ֵ
	*     ����˵�� :
    *				����ϵͳ��ʱ������
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
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnGetSystemTimerParam
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*				paramIndex
	*					��ʱ��������index
	*     ����˵�� :
    *				�õ�ϵͳ��ʱ������
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
	*     ������Ա : �����
	*     ��дʱ�� : 2008-5-20
	*     �������� : LuaFnCheckSystemTimerValidity
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				TimerIndex
	*					ϵͳ��ʱ���������index
	*     ����˵�� :
    *				���ϵͳ��ʱ���Ƿ����
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
			//δ����
			Lua_PushNumber( L,1 );
			return 1;		
		}		

		//����
		Lua_PushNumber( L,0 );
		return 1;

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-27
    *     �������� : LuaAddActionPoint
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nAP 
    *					�ж���
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

        pHuman->SendMsg_RefeshAP(nAP, 0);// 0 ����

        ENDHUMANDEFINE
        return	0;

        LUA_LEAVE_FUNCTION
        return  0;
    }


    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-27
    *     �������� : LuaSubActionPoint
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
    *				nAP 
    *					�ж���
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

            pHuman->SendMsg_RefeshAP(nAP, 1);// 1 ����

        ENDHUMANDEFINE
            return	0;

        LUA_LEAVE_FUNCTION
            return  0;
    }

    /** -----------------------------------------------------------------------
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-28
    *     �������� : LuaCountryDeletaBegin
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
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
    *     ������Ա : amos
    *     ��дʱ�� : 2008-5-28
    *     �������� : LuaCountryVoteBegin
    *     ����˵�� : 
    *     ����˵�� :
    *				SceneID
    *					�������
    *				selfId
    *					������ID,�޷�������
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