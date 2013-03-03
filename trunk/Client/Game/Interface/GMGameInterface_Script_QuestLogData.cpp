
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "LuaPlus.h"
#include "Event/GMEventSystem.h"

#include "NetWork/NetManager.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool/GMUIDataPool.h"
#include "..\DBC\GMDataBase.h"
#include "DataPool/GMDP_Struct_Quest.h"


namespace SCRIPT_SANDBOX
{

	//===============================================================
	QuestLogData QuestLogData::s_QuestLogData;
	LuaPlus::LuaObject* QuestLogData::s_pMetaTable = NULL;

	// 已接任务数量
	INT QuestLogData::DoneMissionNum(LuaPlus::LuaState* state)
	{
		//QuestLogDataMgr::GetMe()->m_QuestLog;
		state->PushInteger( 0 );

		return 1;
	}

	// 可接任务数量
	INT QuestLogData::CanDoMissionNum(LuaPlus::LuaState* state)
	{
		//QuestLogDataMgr::GetMe()->s_allMissions;
		state->PushInteger( 0 );

		return 1;
	}

	// 当前已接任务数量
	INT QuestLogData::DoneMissionNum_CurScene(LuaPlus::LuaState* state)
	{
		QuestLogDataMgr::GetMe()->s_allMissions;
		state->PushInteger( 0 );

		return 1;
	}

	// 当前可接任务数量
	INT QuestLogData::CanDoMissionNum_CurScene(LuaPlus::LuaState* state)
	{
		//QuestLogDataMgr::GetMe()->s_allMissions;
		state->PushInteger( 0 );

		return 1;
	}

	// 得到可接任务列表
	INT QuestLogData::GetMissionList(LuaPlus::LuaState* state)
	{
		const CDetailAttrib_Player* pAttrib = CUIDataPool::GetMe()->GetMySelfDetailAttrib();

		INT i = 0;
		std::vector< _DBC_QUEST_LOG* >::iterator iter = QuestLogDataMgr::GetMe()->s_FilteredMissions.begin();
		for( ; iter != QuestLogDataMgr::GetMe()->s_FilteredMissions.end(); iter++, i++ )
		{
			_DBC_QUEST_LOG* pLog = (*iter);

			// 是否已经做过
			if( pAttrib->IsMissionHaveDone(pLog->nMissionID) )
				continue;

			// 身上是否有这个任务
			INT nIndex = pAttrib->GetMissionIndexByID(pLog->nMissionID);
			if( nIndex != UINT_MAX )
				continue;
	
			//
			// 前置任务是否完成
			//

			if( pLog->nPrevMission1 != -1 )
			{
				if( !pAttrib->IsMissionHaveDone(pLog->nPrevMission1) )
					continue;
			}
			if( pLog->nPrevMission2 != -1 )
			{
				if( !pAttrib->IsMissionHaveDone(pLog->nPrevMission2) )
					continue;
			}
			if( pLog->nPrevMission3 != -1 )
			{
				if( !pAttrib->IsMissionHaveDone(pLog->nPrevMission3) )
					continue;
			}
			if( pLog->nPrevMission4 != -1 )
			{
				if( !pAttrib->IsMissionHaveDone(pLog->nPrevMission4) )
					continue;
			}
			if( pLog->nPrevMission5 != -1 )
			{
				if( !pAttrib->IsMissionHaveDone(pLog->nPrevMission5) )
					continue;
			}
			
			const char* szSceneName = CGameProcedure::s_pWorldManager->GetSceneName(pLog->nSceneID);


			std::vector< STRING > vParam;
			char szTemp[256];
			vParam.push_back( "1" );
			sprintf(szTemp, "%d", i);
			vParam.push_back( szTemp );
			sprintf(szTemp, "%s %s %s", pLog->szName, szSceneName, pLog->szNPCName);
			vParam.push_back( szTemp );
			sprintf(szTemp, "%d,%d", pLog->nNPC_X, pLog->nNPC_Z);
			vParam.push_back( szTemp );
			//DBC_DEFINEHANDLE(s_sceneDefine, DBC_SCENE_DEFINE);
			//const _DBC_SCENE_DEFINE* pSceneDefine = (const _DBC_SCENE_DEFINE*)s_sceneDefine->Search_Index_EQU(pLog->nSceneID);
			//STRING szSceneLevel = "";
			//if (pSceneDefine)
			//{
			//	szSceneLevel = pSceneDefine->szSceneType;
			//}
			sprintf(szTemp, "%d %d", pLog->nSceneID, 1);
			vParam.push_back( szTemp );
			CEventSystem::GetMe()->PushEvent( GE_UPDATE_MISSION, vParam );
		}

		return 0;
	}

	INT QuestLogData::GetMissionInfoByMissionID(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( args[2].IsInteger() )
		{
			INT missionId = args[2].GetInteger();
			INT i=0;
			std::vector< _DBC_QUEST_LOG* >::iterator iter = QuestLogDataMgr::GetMe()->s_FilteredMissions.begin();
			for( ; iter != QuestLogDataMgr::GetMe()->s_FilteredMissions.end(); iter++, i++ )
			{
				_DBC_QUEST_LOG* pLog = (*iter);
				
				if ( pLog->nMissionID == missionId )
				{
					state->PushString(pLog->szName);
					state->PushString(pLog->szNPCName);
					state->PushInteger(pLog->nSceneID);
					state->PushInteger(pLog->nNPC_X);
					state->PushInteger(pLog->nNPC_Z);
					state->PushInteger(pLog->nMinLevel);
					state->PushInteger(pLog->nMaxLevel);
					state->PushInteger(pLog->nPrevMission1);
					state->PushInteger(pLog->nPrevMission2);
					state->PushInteger(pLog->nPrevMission3);
					state->PushInteger(pLog->nPrevMission4);
					state->PushInteger(pLog->nPrevMission5);
					return 12;
				}
			}
		}
		return 0;
		
	}

	INT QuestLogData::GetMissionInfoByClientIndex(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( args[2].IsInteger() )
		{
			INT nIndex = args[2].GetInteger();
			INT i=0;
			std::vector< _DBC_QUEST_LOG* >::iterator iter = QuestLogDataMgr::GetMe()->s_FilteredMissions.begin();
			for( ; iter != QuestLogDataMgr::GetMe()->s_FilteredMissions.end(); iter++, i++ )
			{
				_DBC_QUEST_LOG* pLog = (*iter);

				if ( i== nIndex )
				{
					state->PushString(pLog->szName);
					state->PushString(pLog->szNPCName);
					state->PushInteger(pLog->nSceneID);
					state->PushInteger(pLog->nNPC_X);
					state->PushInteger(pLog->nNPC_Z);
					
					_MISSION_INFO& mInfo = GetMissionInfo(pLog->nScriptID);
					state->PushString(mInfo.m_misDesc.c_str());
					
					return 6;
				}
			}
		}
		return 0;

	}

	INT QuestLogData::GetMissionRewordNumByIndex(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( args[2].IsInteger() )
		{
			INT nIndex = args[2].GetInteger();
			INT i=0;
			std::vector< _DBC_QUEST_LOG* >::iterator iter = QuestLogDataMgr::GetMe()->s_FilteredMissions.begin();
			for( ; iter != QuestLogDataMgr::GetMe()->s_FilteredMissions.end(); iter++, i++ )
			{
				_DBC_QUEST_LOG* pLog = (*iter);

				if ( i== nIndex )
				{
					_MISSION_INFO& mInfo = GetMissionInfo(pLog->nScriptID);
					state->PushInteger(mInfo.m_vecQuestRewardItem.size());

					return 1;
				}
			}
		}
		state->PushInteger(0);
		return 1;

	}

	INT QuestLogData::GetMissionRewordInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if ( args[2].IsInteger()  && args[3].IsInteger() )
		{
			INT nIndex = args[2].GetInteger();
			INT i=0;
			std::vector< _DBC_QUEST_LOG* >::iterator iter = QuestLogDataMgr::GetMe()->s_FilteredMissions.begin();
			for( ; iter != QuestLogDataMgr::GetMe()->s_FilteredMissions.end(); iter++, i++ )
			{
				_DBC_QUEST_LOG* pLog = (*iter);

				if ( i== nIndex )
				{
					_MISSION_INFO& mInfo = GetMissionInfo(pLog->nScriptID);
					_MISSION_INFO::QuestRewardItem* pItem = &(misInfo->m_vecQuestRewardItem[nIndex]);
					if(!pItem)
					{
						KLThrow("LUA:GetMissionDemand_Item Empty Event Item");
					}
					SMissionBonus *pBonus = pItem->pItemData;
					if(!pBonus)
					{
						KLThrow("LUA:GetMissionContinue_Bonus Empty Event Item");
					}

					switch( pBonus->m_nType )
					{
					case MISSION_BONUS_TYPE_MONEY:		// 金钱
						state->PushString("money");
						state->PushInteger(0);//为了保持到处数据长度相等
						state->PushInteger(pBonus->m_uMoney);
						return 3;
						break;
					case MISSION_BONUS_TYPE_ITEM:		// 物品
						state->PushString("item");
						//state->PushInteger(pBonus->m_ItemBonus.m_uItemID);	//物品ID
						state->PushInteger(pItem->pItemImpl->GetID());	//物品ID
						state->PushInteger(pBonus->m_ItemBonus.m_yCount);	//有多少个这个物品
						break;
					case MISSION_BONUS_TYPE_ITEM_RAND:	// 随机物品
						state->PushString("itemrand");
						//state->PushInteger(pBonus->m_ItemBonus.m_uItemID);	//物品ID
						state->PushInteger(pItem->pItemImpl->GetID());	//物品ID
						state->PushInteger(pBonus->m_ItemBonus.m_yCount);	//有多少个这个物品
						break;
					case MISSION_BONUS_TYPE_ITEM_RADIO:	// 多选1物品
						state->PushString("itemradio");
						//state->PushInteger(pBonus->m_ItemBonus.m_uItemID);	//物品ID
						state->PushInteger(pItem->pItemImpl->GetID());	//物品ID
						state->PushInteger(pBonus->m_ItemBonus.m_yCount);	//有多少个这个物品
						break;
					default :
						KLThrow("LUA:GetMissionContinue_Bonus Unknown ItemType");
						break;
					}
					return 3;
				}
			}
		}
		return 0;

	}

};