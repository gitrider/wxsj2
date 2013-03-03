
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
#include "DataPool/GMUIDataPool.h"
#include "DataPool/GMDataPool.h"
#include "datapool/GMDP_Struct_Mail.h"
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
#include "CGTeamInvite.h"
#include "DataPool/GMDP_CharacterData.h"
#include "DataPool/GMDP_Struct_Quest.h"
#include "DataPool/GMDP_Struct_SystemTip.h"
#include "DataPool/GMDataPool.h"
#include "DataPool/GMDP_Struct_Relation.h"
#include "FileDataMgr.h"
#include "Script/GMScriptSystem.h"
#include "DBC\GMDataBase.h"
#include "gamestruct_relation.h"
#include "cgrelation.h"
#include "gmgameinterface_script.h"
#include "network/netmanager.h"
#include "cgmail.h"
#include "cgaskmail.h"
#include "gamestruct.h"
#include "Procedure/GameProcedure.h"
#include "GMInterface_Script_Talk.h"
#include "GIResourceProvider.h"
#include "GIUtil.h"
#include "TransferItem/GMTransferItemSystem.h"

#include "Interface\GMGameInterface_Script.h"



namespace SCRIPT_SANDBOX
{
		
	//===============================================================
	DataPool DataPool::s_DataPool;
	LuaPlus::LuaObject* DataPool::s_pMetaTable = NULL;
	//continue
	INT DataPool::GetMissionContinue_Num(LuaPlus::LuaState* state)
	{
		INT nNum;
		nNum = CUIDataPool::GetMe()->m_pMissionContinueInfo->m_yTextCount;
		state->PushInteger(nNum);

		nNum = CUIDataPool::GetMe()->m_pMissionContinueInfo->m_yBonusCount;
		state->PushInteger(nNum);
		
		return 2;
	}
	INT DataPool::GetMissionContinue_Text(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionContinue_Text");
		}

		INT nIndex = args[2].GetInteger();
		
		state->PushString( CUIDataPool::GetMe()->m_pMissionContinueInfo->m_aText[nIndex].m_szString );

		return 1;
	}
	INT DataPool::GetMissionContinue_Bonus(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionContinue_Bonus");
		}

		INT nIndex = args[2].GetInteger();
		
		SMissionBonus *pBonus = &(CUIDataPool::GetMe()->m_pMissionContinueInfo->m_aBonus[nIndex]);
		if(!pBonus)
		{
			KLThrow("LUA:GetMissionContinue_Bonus Empty Event Item");
		}

		switch( pBonus->m_nType )
		{
		case MISSION_BONUS_TYPE_MONEY:		// ��Ǯ
			state->PushString("money");
			state->PushInteger(0);
			state->PushInteger(pBonus->m_uMoney);//Ϊ�˱��ֵ������ݳ�����ȣ���������Ҫ������
			break;
		case MISSION_BONUS_TYPE_ITEM:		// ��Ʒ
			state->PushString("item");
			state->PushInteger(pBonus->m_ItemBonus.m_uItemID);	//��ƷID
			state->PushInteger(pBonus->m_ItemBonus.m_yCount);	//�ж��ٸ������Ʒ
			break;
		case MISSION_BONUS_TYPE_ITEM_RAND:	// �����Ʒ
			state->PushString("itemrand");
			state->PushInteger(pBonus->m_ItemBonus.m_uItemID);	//��ƷID
			state->PushInteger(pBonus->m_ItemBonus.m_yCount);	//�ж��ٸ������Ʒ
			break;
		case MISSION_BONUS_TYPE_ITEM_RADIO:	// ��ѡ1��Ʒ
			state->PushString("itemradio");
			state->PushInteger(pBonus->m_ItemBonus.m_uItemID);	//��ƷID
			state->PushInteger(pBonus->m_ItemBonus.m_yCount);	//�ж��ٸ������Ʒ
			break;
		default :
			KLThrow("LUA:GetMissionContinue_Bonus Unknown ItemType");
			break;
		}
		return 3;
	}

	//��������DEMAND�������ı���Ϣ����Ҫ����Ʒ��
	INT DataPool::GetMissionDemand_Num(LuaPlus::LuaState* state)
	{
		INT nNum;
		nNum = CUIDataPool::GetMe()->m_pMissionDemandInfo->m_yTextCount;
		state->PushInteger(nNum);

		nNum = CUIDataPool::GetMe()->m_pMissionDemandInfo->m_yDemandCount;
		state->PushInteger(nNum);

		return 2;
	}

	//�ر������ύ����
	INT DataPool::CloseMissionFrame(LuaPlus::LuaState* state)
	{
		// ʹ�����ݳ��м�¼����������������е���Ʒ����
		for(INT i=0; i<3; i++)
		{
			tObject_Item* pItem = CDataPool::GetMe()->MissionBox_GetItem(i);
			if(pItem)
			{
				INT nBagIndex = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(pItem->GetGUID());

				tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(nBagIndex);
				pBagItem->SetLock(FALSE);
			}
		}

		// ��Ҫ�������������������
		CDataPool::GetMe()->MissionBox_Clear();
		CActionSystem::GetMe()->UserMission_Update();

		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		return 0;
	}

	//�������󡪡��ı�
	INT DataPool::GetMissionDemand_Text(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionDemand_Text");
		}

		INT nIndex = args[2].GetInteger();
		
		state->PushString( CUIDataPool::GetMe()->m_pMissionDemandInfo->m_aText[nIndex].m_szString );

		return 1;
	}
	//�������󡪡���Ʒ
	INT DataPool::GetMissionDemand_Item(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionDemand_Item");
		}

		INT nIndex = args[2].GetInteger();
		
		CUIDataPool::QuestDemandItem *pItem = &(CUIDataPool::GetMe()->m_vecQuestDemandItem[nIndex]);
		if(!pItem)
		{
			KLThrow("LUA:GetMissionDemand_Item Empty Event Item");
		}

		state->PushInteger(pItem->pItemImpl->GetID());		//��ƷID
		state->PushInteger(pItem->pDemandItem->m_yCount);		//����

		return 2;
	}

	//������Ϣ�������ı���Ϣ�ͽ�����Ʒ��
	INT DataPool::GetMissionInfo_Num(LuaPlus::LuaState* state)
	{
		INT nNum;
		nNum = CUIDataPool::GetMe()->m_pMissionInfo->m_yTextCount;
		state->PushInteger(nNum);

		nNum = CUIDataPool::GetMe()->m_pMissionInfo->m_yBonusCount;
		state->PushInteger(nNum);

		return 2;
	}


	INT DataPool::GetPlayerMission_SceneID(LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_SceneID");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushInteger(-1);
			return 1;
		}

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushInteger(-1);
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;

		const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_QUEST_LOG );
		const _DBC_QUEST_LOG* pLine = (const _DBC_QUEST_LOG*)((tDataBase*)pdbcbase)->Search_Index_EQU(scriptId);

		//DBC_DEFINEHANDLE( pQuestDBC, DBC_QUEST_LOG );
		//const _DBC_QUEST_LOG* pLine= (const _DBC_QUEST_LOG*)pQuestDBC->Search_LineNum_EQU(scriptId);

		if( !pLine )
		{
			state->PushInteger(-1);
			return 1;
		}
		
		INT id = pLine->nSceneID;
		state->PushInteger(id);

		return 1;
	}
	

	INT DataPool::GetPlayerMission_Level(LuaPlus::LuaState* state)
	{
		//INT nNum;
		//nNum = CUIDataPool::GetMe()->m_pMissionDemandInfo->m_yTextCount;
		//state->PushInteger(nNum);
		
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_Level");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushNumber(0);
			return 1;
		}

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushNumber(0);
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;
		misInfo = &(GetMissionInfo(scriptId));
		
		INT nNum;
		nNum = (INT) misInfo->m_nLevel;
		state->PushInteger(nNum);

		return 1;
	}
	INT	DataPool::GetPlayerMission_Kind(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_Kind");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushNumber(0);
			return 1;
		}

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushNumber(0);
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;
		misInfo = &(GetMissionInfo(scriptId));

		INT nNum;
		nNum = (INT) misInfo->m_nKind;
		state->PushInteger(nNum);

		return 1;
	
	}

	INT	DataPool::GetMissionInfo_Kind(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionInfo_Kind arg error");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= 200)
		{	
			state->PushNumber(0);
			return 1;
		}

		DBC_DEFINEHANDLE(s_pMissionKindDBC, DBC_MISSION_KIND);
	
		//��CREATURE_INFO�в��Ҷ���
		const _DBC_MISSION_KIND* pMissionKind = (const _DBC_MISSION_KIND*)s_pMissionKindDBC->Search_Index_EQU(nIndex);

		if( pMissionKind == NULL )
		{
			KLThrow("LUA:GetMissionInfo_Kind index error");
		}

		state->PushString(pMissionKind->szKindName);

		return 1;
	}


	// �Ƿ���ÿ�ջ���� 20100728 BLL 
	INT	DataPool::GetMissionInfo_IsDayTip(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionInfo_IsDayTip");
		}

		INT  nIndex = args[2].GetInteger();

		BOOL bIsDayTip = FALSE;
		BOOL bIsCanSee = FALSE;

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);

		if(!OwnMission)
		{
			state->PushNumber( 0 );
			return 1;
		}

		CUIDataPool::GetMe()->GetMySelfDetailAttrib()->IsDayTipsMission( OwnMission->m_idMission, bIsDayTip, bIsCanSee );

		if ( bIsDayTip == TRUE )
		{
			state->PushNumber( 1 );
		}
		else
		{
			state->PushNumber( 0 );
		}

		return 1;
	}


	// ��ȡÿ�ջ�ĿɽӴ�������ɴ��� 20100728 BLL 
	INT	DataPool::GetMissionInfo_DayTipCount(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionInfo_DayTipCount");
		}

		INT  nIndex = args[2].GetInteger();

		INT  nValue = 0;
		INT  nSum   = 0;

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);

		if(!OwnMission)
		{
			state->PushNumber( 0 );
			state->PushNumber( 0 );
			return 2;
		}

		nValue = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMissionDoneCount( OwnMission->m_idMission );
		nSum   = DaytipsData::s_DaytipsData.GetMissionCanDoCount( OwnMission->m_idMission );

		state->PushNumber( nValue );
		state->PushNumber( nSum );

		return 2;
	}

	

	INT DataPool::GetPlayerMission_IsElite(LuaPlus::LuaState* state)
	{
		//INT nNum;
		//nNum = CUIDataPool::GetMe()->m_pMissionDemandInfo->m_yTextCount;
		//state->PushInteger(nNum);
		
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_Level");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushNumber(0);
			return 1;
		}

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushNumber(0);
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;
		misInfo = &(GetMissionInfo(scriptId));
		
		INT nNum;
		nNum = (INT) misInfo->m_nElite;
		state->PushInteger(nNum);

		return 1;
	}

	INT DataPool::GetMissionInfo_Text(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionInfo_Text");
		}

		INT nIndex = args[2].GetInteger();
		
		state->PushString( CUIDataPool::GetMe()->m_pMissionInfo->m_aText[nIndex].m_szString );

		return 1;
	}

	//������
	INT DataPool::GetMissionInfo_Bonus(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionInfo_Bonus");
		}

		INT nIndex = args[2].GetInteger();
		if(nIndex <0 || nIndex >= (INT)CUIDataPool::GetMe()->m_vecQuestRewardItem.size()) 
		{
			KLThrow("LUA:GetMissionInfo_Bonus error index[%d]", nIndex);
			return 0;
		}

		CUIDataPool::QuestRewardItem *pBonus = &(CUIDataPool::GetMe()->m_vecQuestRewardItem[nIndex]);
		if(!pBonus)
		{
			KLThrow("LUA:GetMissionInfo_Bonus Empty Event Item");
		}

		switch( pBonus->pItemData->m_nType )
		{
		case MISSION_BONUS_TYPE_MONEY:		// ��Ǯ
			state->PushString("money");
			state->PushInteger(0);//Ϊ�˱������ݳ�����ȣ�
			state->PushInteger( pBonus->pItemData->m_uMoney);
			break;
		case MISSION_BONUS_TYPE_ITEM:		// ��Ʒ
			state->PushString("item");
			state->PushInteger(pBonus->pItemImpl->GetID());		//��ƷID
			state->PushInteger(pBonus->pItemData->m_ItemBonus.m_yCount);	//�ж��ٸ������Ʒ
			break;
		case MISSION_BONUS_TYPE_ITEM_RAND:	// �����Ʒ
			state->PushString("itemrand");
			state->PushInteger(0);			//��ƷID
			state->PushInteger(pBonus->pItemData->m_ItemBonus.m_yCount);	//�ж��ٸ������Ʒ
			break;
		case MISSION_BONUS_TYPE_ITEM_RADIO:	// ��ѡ1��Ʒ
			state->PushString("itemradio");
			state->PushInteger(pBonus->pItemImpl->GetID());	//��ƷID
			state->PushInteger(pBonus->pItemData->m_ItemBonus.m_yCount);	//�ж��ٸ������Ʒ
			break;
		default :
			KLThrow("LUA:GetMissionInfo_Bonus Unknown ItemType");
			break;
		}

		return 3;
	}

	//----------------------------------------------------------------------
	// ȡ��������б�
	INT DataPool::GetScoutMission(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetScoutMission");
		}

		BOOL isScout = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetScoutMission(args[2].GetInteger());
		state->PushInteger(isScout);

		return 1;
	}

	INT DataPool::SetScoutMission(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:SetScoutMission");
		}

		CDetailAttrib_Player* pAttrib = (CDetailAttrib_Player*)CUIDataPool::GetMe()->GetMySelfDetailAttrib();
		pAttrib->SetScoutMission(args[2].GetInteger());

		return 0;
	}

	INT DataPool::GetScoutMission_Num(LuaPlus::LuaState* state)
	{
		CDetailAttrib_Player* pAttrib = (CDetailAttrib_Player*)CUIDataPool::GetMe()->GetMySelfDetailAttrib();
		state->PushInteger(pAttrib->GetSCountNum());
		return 1;
	}
	

	// Npc�Զ���·
	INT DataPool::NpcAutoFindPath(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:AntoFindPath Param 1 Wrong");
		}
		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA:AntoFindPath Param 2 Wrong");
		}
		if( (args[4].IsString())  && args[5].IsInteger())
		{
			// 20100415 AddCodeBegin 
			// ����NPC�Զ�Ѱ·���ܵ��������
			CObjectManager::GetMe()->GetMySelf()->SetIsAutoNPCPath( args[5].GetInteger(), args[4].GetString(), TRUE );
			// 20100415 AddCodeEnd
		}

		CGameProcedure::s_pGameInterface->Player_MoveTo( fVector2(args[2].GetInteger(), args[3].GetInteger()) );

		return 0;
	}


	// �ڳ�����ͼ���ڵ������ƶ�������λ��ʱ�������ǰ��Npc�Զ�Ѱ·״̬���������Զ�Ѱ·�Ĳ���ֵ	20100531 BLL
	INT DataPool::ResetNpcAutoFindPath(LuaPlus::LuaState* state)
	{
		if ( CObjectManager::GetMe()->GetMySelf()->GetIsAutoNPCPath()->m_isAutoPath == TRUE )
		{
			// ����NPC�Զ�Ѱ·���ܵ��������
			CObjectManager::GetMe()->GetMySelf()->SetIsAutoNPCPath( -1, "", FALSE );
		}		

		return 0;
	}


	INT DataPool::NpcOutSceneAutoFindPath(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow("LUA:NpcOutSceneAutoFindPath Param 1 Wrong");
		}
		if ( !(args[3].IsInteger()) )
		{
			KLThrow("LUA:NpcOutSceneAutoFindPath Param 2 Wrong");
		}
		if ( !(args[4].IsInteger()) )
		{
			KLThrow("LUA:NpcOutSceneAutoFindPath Param 3 Wrong");
		}
		INT SceneId = args[2].GetInteger();
		fVector2 pos = fVector2( args[3].GetInteger(), args[4].GetInteger() );
		CGameProcedure::s_pGameInterface->Player_MoveTo(SceneId, pos );

		return 0;
	}

	//----------------------------------------------------------------------
	//Player�����б�------------------------------
	//Player�ĵ�ǰ������
	INT DataPool::GetPlayerMission_Num(LuaPlus::LuaState* state)
	{
		INT nNum = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission_Num();
		state->PushInteger(nNum);
		return 1;
	}
	
	INT DataPool::GetPlayerMission_InUse(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_InUse");
		}

		INT nIndex = args[2].GetInteger();
		if(nIndex <0 || nIndex >= MAX_CHAR_MISSION_NUM) 
		{
			KLThrow("LUA:GetPlayerMission_InUse error index[%d]", nIndex);
			return 0;
		}

		const _OWN_MISSION* pMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if( pMission == NULL )
			state->PushInteger(0);

		if( pMission->m_idScript == INVALID_ID || pMission->m_idMission == INVALID_ID)
			state->PushInteger(0);
		else
			state->PushInteger(1);

		return 1;
	}

	parseLine pFns[] = 
	{
		parseMissionName,
		parseDemandItem,
		parseItemBonus,
		parseRandItemBonus, 
		parseRadioItemBonus, 
		parseMoneyBonus,
		parseExpBonus,
		parseMissionBriefTarget,
		parseMissionState,
		parseMissionLevel,
		parseMissionElite,
		parseMissionKind,
		parseDemandKill,
		parseMissionOKFail,
		parseMissionLimitTime,
		parseMissionRound,
		parseMissionCustom,
		parseMissionBill,
		parseMissionBillName
	};	
	const INT RESERVER = sizeof(pFns)/sizeof(pFns[0]);
	 
	INT DataPool::GetPlayerMission_Description(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_Description 1");
		}

		INT nIndex = args[2].GetInteger();
		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushString("");
			return 1;
		}

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushString("");
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;

		misInfo = &(GetMissionInfo(scriptId));

		// ���������������
		if(!misInfo->m_bFill)
		{
			// �����ļ�
			const STRING& strFileName = CScriptSystem::GetMe()->GetQuestFile(scriptId);
			if(strFileName.empty())
			{
				KLThrow("Invalid script id:%d", scriptId);
			}

			char szFileName[MAX_PATH];
			_snprintf(szFileName, MAX_PATH, "%s", strFileName.c_str()+1);

			char* lpAddress = NULL;
			DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource( szFileName, lpAddress, "General" );
			if( dwSize > 0 )
			{
				CHAR* szDesc[RESERVER];
				for(INT i=0;i<RESERVER;i++)
					szDesc[i] = new CHAR[MAX_PATH];

				BOOL parse = FALSE;
				CHAR* line = strtok( lpAddress, "\r\n" );
				while( line )
				{
					if(!parse)
					{
						if(strstr(line, "MisDescBegin"))
						{
							parse = TRUE;			
						}
					}
					else
					{
						if(strstr(line,"MisDescEnd"))
							break;

						if(strlen(line) >= 2 && (line[0] != '-' || line[1] !='-'))
						{
							for(INT j=0;j<RESERVER;j++)
							{
								if(strstr(line,tokens[j]))
								{
									memset(szDesc[j],0,MAX_PATH);
									pFns[j](&szDesc[j],line,j);
								}
							}
						}
					}
					line = strtok( NULL, "\r\n" );
				}

				CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
			}
			else
			{
				KLThrow("Invalid script file: %s", szFileName);
			}
			misInfo->m_bFill = TRUE;
		}


#define QUAD 4

		CHAR szBuff[MAX_PATH*QUAD];
		memset(szBuff,0,MAX_PATH*QUAD);
		
		strcpy(szBuff,misInfo->m_misName.c_str());
		strcat(szBuff,misInfo->m_misDesc.c_str());
		char szTemp[MAX_PATH] = {0};

		for(UINT i=0;i<misInfo->m_vecQuestDemandItem.size();i++)
		{
			_MISSION_INFO::QuestDemandItem* pItem = &(misInfo->m_vecQuestDemandItem[i]);
			if(!pItem)
			{
				KLThrow("LUA:GetMissionDemand_Item Empty Event Item");
			}

			strcat(szBuff,"demand : \n");
			memset(szTemp,0,sizeof(szTemp));
			_snprintf(szTemp, MAX_PATH, "%d,%d\n",pItem->pDemandItem->m_uItemID,pItem->pDemandItem->m_yCount);
			strcat(szBuff,szTemp);
		}

		
		for(UINT i=0;i<misInfo->m_vecQuestRewardItem.size();i++)
		{
			//������
			_MISSION_INFO::QuestRewardItem *pBonus = &(misInfo->m_vecQuestRewardItem[i]);
			switch( pBonus->pItemData->m_nType )
			{
			case MISSION_BONUS_TYPE_MONEY:		// ��Ǯ
				strcat(szBuff,"money:\n");
				memset(szTemp,0,sizeof(szTemp));
				_snprintf(szTemp, MAX_PATH, "%d\n",pBonus->pItemData->m_uMoney);
				strcat(szBuff,szTemp);
				break;
			case MISSION_BONUS_TYPE_ITEM:		// ��Ʒ
				strcat(szBuff,"item:\n");
				memset(szTemp,0,sizeof(szTemp));
				_snprintf(szTemp, MAX_PATH, "%d,%d\n",pBonus->pItemData->m_ItemBonus.m_uItemID,pBonus->pItemData->m_ItemBonus.m_yCount);
				strcat(szBuff,szTemp);
				break;
			case MISSION_BONUS_TYPE_ITEM_RAND:	// �����Ʒ
				strcat(szBuff,"itemrand:\n");
				memset(szTemp,0,sizeof(szTemp));
				_snprintf(szTemp, MAX_PATH, "%d,%d\n",pBonus->pItemData->m_ItemBonus.m_uItemID,pBonus->pItemData->m_ItemBonus.m_yCount);
				strcat(szBuff,szTemp);
				break;
			case MISSION_BONUS_TYPE_ITEM_RADIO:	// ��ѡ1��Ʒ
				strcat(szBuff,"itemradio:\n");
				memset(szTemp,0,sizeof(szTemp));
				_snprintf(szTemp, MAX_PATH, "%d,%d\n",pBonus->pItemData->m_ItemBonus.m_uItemID,pBonus->pItemData->m_ItemBonus.m_yCount);
				strcat(szBuff,szTemp);
				break;
			}
		}

		CActionSystem::GetMe()->QuestLogItem_Update();
		state->PushString(szBuff);

		return 1;
	}
/*
	INT DataPool::GetPlayerMissionDemand_Num(LuaPlus::LuaState* state)
	{
		//INT nNum;
		//nNum = CUIDataPool::GetMe()->m_pMissionDemandInfo->m_yTextCount;
		//state->PushInteger(nNum);
		
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionDemand_Item");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushNumber(0);
			return 1;
		}

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushNumber(0);
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;
		misInfo = &(GetMissionInfo(scriptId));
		
		INT nNum;
		nNum = (INT) misInfo->m_vecQuestDemandItem.size();
		state->PushInteger(nNum);

		return 1;
	}
*/	//////////////////////////
	INT DataPool::GetPlayerMissionDemandKill_Num(LuaPlus::LuaState* state)
	{
		//INT nNum;
		//nNum = CUIDataPool::GetMe()->m_pMissionDemandInfo->m_yTextCount;
		//state->PushInteger(nNum);
		
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMissionDemandKill_Num");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushNumber(0);
			return 1;
		}

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushNumber(0);
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;
		misInfo = &(GetMissionInfo(scriptId));
		
		INT nNum;
		nNum = (INT) misInfo->m_vecQuestDemandKill.size();
		state->PushInteger(nNum);

		return 1;
	}
	INT DataPool::GetPlayerMission_Variable(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger() && args[3].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_Variable");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushNumber(0);
			return 1;
		}
		INT nVariable = args[3].GetInteger();
		INT Mission_Variable = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMissionParam(nIndex,nVariable);

		if(!Mission_Variable)
		{
			state->PushNumber(0);
			return 1;
		}

		state->PushInteger(Mission_Variable);

		return 1;
	}
	INT DataPool::GetPlayerMission_DataRound(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!args[2].IsInteger())
		{
			KLThrow("LUA:GetPlayerMission_DataRound");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_DATA_NUM)
		{	
			state->PushNumber(0);
			return 1;
		}
		INT Mission_DataRound = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMissionData(nIndex);

		if(!Mission_DataRound)
		{
			state->PushNumber(0);
			return 1;
		}

		state->PushInteger(Mission_DataRound);

		return 1;
	
	}

	INT DataPool::GetPlayerMission_ItemCountNow(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!args[2].IsInteger())
		{
			KLThrow("LUA:GetPlayerMission_ItemCountNow");
		}

		INT nItemID = args[2].GetInteger();	

		if(nItemID < 0 )
		{	
			state->PushNumber(0);
			return 1;
		}

		INT count = CDataPool::GetMe()->UserBag_CountItemByIDTable(nItemID);

		state->PushInteger(count);

		return 1;
	
	}


	INT DataPool::GetPlayerMission_Display(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger() && args[3].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_Display");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushNumber(0);
			return 1;
		}
		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushNumber(0);
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;
		misInfo = &(GetMissionInfo(scriptId));
		
		INT nNum;
		switch(args[3].GetInteger())
		{
		case 1:
			nNum = (INT) misInfo->m_nOKFail;
			//��������һ�����û�����壬�ò���ֵ Nick
			//nNum = (INT)CUIDataPool::GetMe()->GetMySelfDetailAttrib()->IsMissionHaveDone(OwnMission->m_idMission);

			if(nNum < 1 )
			{
				state->PushInteger(0);
				return 1;
			}
			break;
		case 2:
			nNum = (INT) misInfo->m_nLimitTime;
			if(nNum < 1 )
			{
				state->PushInteger(0);
				return 1;
			}
			break;
		case 3:
			nNum = (INT) misInfo->m_nRound;
			if(nNum < 1 )
			{
				state->PushInteger(-1);
				return 1;
			}
			break;
		case 4:
			nNum = (INT) misInfo->m_nBill;
			if(nNum < 1 )
			{
				state->PushInteger(0);
				return 1;
			}
			break;
		default:
			nNum = 0;
			break;
		}
		state->PushInteger(nNum);
		return 1;
	}

	INT DataPool::GetPlayerMission_RemainTime(LuaPlus::LuaState* state)
	{

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_RemainTime");
		}
		INT nIndex = args[2].GetInteger();

		INT nNum = CDataPool::GetMe()->QuestTimeGroup_Get(nIndex);

		state->PushInteger(nNum);
		return 1;
	}

	INT DataPool::GetPlayerMissionDemand_Item(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionDemand_Item");
		}

		INT nIndex = args[2].GetInteger();	
		
		_MISSION_INFO::QuestDemandItem* pItem = &(misInfo->m_vecQuestDemandItem[nIndex]);
		if(!pItem)
		{
			KLThrow("LUA:GetMissionDemand_Item Empty Event Item");
		}

		state->PushString(pItem->pItemImpl->GetName());			//��Ʒ���֣���ǰ��GetID()
		state->PushInteger(pItem->pItemImpl->GetIdTable());		//��ƷID
		state->PushInteger(pItem->pDemandItem->m_yCount);		//����

		return 3;
	}

	INT DataPool::GetPlayerMissionDemand_NPC(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMissionDemand_NPC");
		}

		INT nIndex = args[2].GetInteger();	
		
		_MISSION_INFO::QuestDemandKill* pNpc = &(misInfo->m_vecQuestDemandKill[nIndex]);
		if(!pNpc)
		{
			KLThrow("LUA:GetPlayerMissionDemand_NPC Empty Event Item");
		}

		state->PushString(pNpc->szNPCName.c_str());				//kill��npc name
		state->PushInteger(pNpc->pDemandKill->m_yCount);		//����

		return 2;
	}
	INT DataPool::GetPlayerMissionCustom(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMissionCustom");
		}

		INT nIndex = args[2].GetInteger();	
		
		_MISSION_INFO::QuestCustom* pCustom = &(misInfo->m_vecQuestCustom[nIndex]);
		if(!pCustom)
		{
			KLThrow("LUA:GetPlayerMissionCustom Empty Event Custom");
		}

		state->PushString(pCustom->szCustomString.c_str());			//�Զ����ַ���
		state->PushInteger(pCustom->nCount);						//����

		return 2;
	}

	INT DataPool::GetPlayerMissionBonus_Item(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionContinue_Bonus");
		}

		INT nIndex = args[2].GetInteger();

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
		case MISSION_BONUS_TYPE_MONEY:		// ��Ǯ
			state->PushString("money");
			state->PushInteger(0);//Ϊ�˱��ֵ������ݳ������
			state->PushInteger(pBonus->m_uMoney);
			return 3;
			break;
		case MISSION_BONUS_TYPE_ITEM:		// ��Ʒ
			state->PushString("item");
			//state->PushInteger(pBonus->m_ItemBonus.m_uItemID);	//��ƷID
			state->PushInteger(pItem->pItemImpl->GetID());	//��ƷID
			state->PushInteger(pBonus->m_ItemBonus.m_yCount);	//�ж��ٸ������Ʒ
			break;
		case MISSION_BONUS_TYPE_ITEM_RAND:	// �����Ʒ
			state->PushString("itemrand");
			//state->PushInteger(pBonus->m_ItemBonus.m_uItemID);	//��ƷID
			state->PushInteger(pItem->pItemImpl->GetID());	//��ƷID
			state->PushInteger(pBonus->m_ItemBonus.m_yCount);	//�ж��ٸ������Ʒ
			break;
		case MISSION_BONUS_TYPE_ITEM_RADIO:	// ��ѡ1��Ʒ
			state->PushString("itemradio");
			//state->PushInteger(pBonus->m_ItemBonus.m_uItemID);	//��ƷID
			state->PushInteger(pItem->pItemImpl->GetID());	//��ƷID
			state->PushInteger(pBonus->m_ItemBonus.m_yCount);	//�ж��ٸ������Ʒ
			break;
		default :
			KLThrow("LUA:GetMissionContinue_Bonus Unknown ItemType");
			break;
		}
		return 3;
	}
	
	INT DataPool::GetPlayerMission_Memo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_Memo IsInteger");
			state->PushString("");
			state->PushString("");
			return 2;
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			KLThrow("LUA::GetPlayerMission_Memo nIndex : [%d]",nIndex);
			state->PushString("");
			state->PushString("");
			return 2;
		}

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushNumber(0);
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;
		misInfo = &(GetMissionInfo(scriptId));
		
		state->PushString(misInfo->m_misName.c_str());
		state->PushString(misInfo->m_misDesc.c_str());
		return 2;
	}

	
	//INT DataPool::GetPlayerMissionDemand_Num(LuaPlus::LuaState* state)
	//{
	//	LuaStack args(state);
	//	if (!(args[2].IsInteger()))
	//	{
	//		KLThrow("LUA:GetPlayerMission_Info");
	//	}
	//	
	//	INT nIndex = args[2].GetInteger();
	//	const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);

	//	state->PushString("mission_info");
	//	return 1;
	//}
	
	INT DataPool::GetPlayerMissionDemand_Num(LuaPlus::LuaState* state)
	{
		//INT nNum;
		//nNum = CUIDataPool::GetMe()->m_pMissionDemandInfo->m_yTextCount;
		//state->PushInteger(nNum);
		
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionDemand_Item");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushNumber(0);
			return 1;
		}

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushNumber(0);
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;
		misInfo = &(GetMissionInfo(scriptId));
		
		INT nNum;
		nNum = (INT) misInfo->m_vecQuestDemandItem.size();
		state->PushInteger(nNum);

		return 1;
	}

	INT DataPool::GetPlayerMissionCustom_Num(LuaPlus::LuaState* state)
	{
		//INT nNum;
		//nNum = CUIDataPool::GetMe()->m_pMissionDemandInfo->m_yTextCount;
		//state->PushInteger(nNum);
		
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMissionCustom_Num");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushNumber(0);
			return 1;
		}

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushNumber(0);
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;
		misInfo = &(GetMissionInfo(scriptId));
		
		INT nNum;
		nNum = (INT) misInfo->m_vecQuestCustom.size();
		state->PushInteger(nNum);

		return 1;
	}

	INT DataPool::GetPlayerMissionBonus_Num(LuaPlus::LuaState* state)
	{
		INT nNum = (INT) misInfo->m_vecQuestRewardItem.size();
		state->PushInteger(nNum);

		return 1;
	}

	INT DataPool::GetPlayerMission_Name(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_Name");
		}

		INT nIndex = args[2].GetInteger();

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(OwnMission)
		{
			CHAR szTemp[MAX_PATH];
			_snprintf(szTemp, MAX_PATH, "Mission:%d", OwnMission->m_idMission);
			state->PushString(szTemp);
		}
		return 1;
	}

	INT DataPool::GetPlayerMission_BillName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetMissionDemand_Item");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
		{	
			state->PushNumber(0);
			return 1;
		}

		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		if(!OwnMission)
		{
			state->PushNumber(0);
			return 1;
		}
		INT scriptId = OwnMission->m_idScript;
		misInfo = &(GetMissionInfo(scriptId));
		
		state->PushString(misInfo->m_misBillName.c_str());

		return 1;
	}

	INT DataPool::GetPlayerMission_Info(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPlayerMission_Info");
		}
		
		INT nIndex = args[2].GetInteger();
		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);

		state->PushString("mission_info");
		return 1;
	}

	INT DataPool::Mission_Abnegate_Popup(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger() && args[3].IsString()))
		{
			KLThrow("LUA:Mission_Abnegate_Popup");
		}

		INT nIndex = args[2].GetInteger();	

		if(nIndex < 0 || nIndex >= MAX_CHAR_MISSION_NUM)
			return 0;
		
		const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(nIndex);
		
		if(!OwnMission)
		{
			state->PushNumber(0);
			return 1;
		}
		
		INT scriptId = OwnMission->m_idScript;

		CGameProcedure::s_pEventSystem->PushEvent( GE_AFFIRM_SHOW, args[3].GetString(), "", scriptId );
		return 0;
	}
	
	INT DataPool::Mission_Abnegate_Special_Quest_Popup(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger() && args[3].IsString()))
		{
			KLThrow("LUA:Mission_Abnegate_Popup");
		}

		INT scriptId = args[2].GetInteger();	
		
		CGameProcedure::s_pEventSystem->PushEvent( GE_AFFIRM_SHOW, args[3].GetString(), "", scriptId );
		return 0;
	}


	INT DataPool::EnumPlayerMission_ItemAction(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if ( !args[2].IsInteger())
		{
			KLThrow("LUA:EnumPlayerMission_ItemAction");
		}

		INT nItemID = args[2].GetInteger();	

		//�õ�item��idclient��ȥȡ����Ӧ��action button.
		tActionItem* pAction = CActionSystem::GetMe()->GetAction_ItemID(nItemID);
		if(!pAction) return 0;
		//action��ID���ظ�����
		state->PushInteger(pAction->GetID());

		return 1;
	}

	INT DataPool::GetNPCEventList_Num(LuaPlus::LuaState* state)
	{
		INT nNum = CUIDataPool::GetMe()->m_pEventList->m_yItemCount;

		state->PushInteger(nNum);

		return 1;
	}

	INT DataPool::GetNPCEventList_Item(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetNPCEventList_Item");
		}

		INT nIndex = args[2].GetInteger();

		const ScriptEventItem* pItem = CUIDataPool::GetMe()->m_pEventList->GetItem( nIndex);
		if(!pItem)
		{
			KLThrow("LUA:GetNPCEventList_Item Empty Event Item");
		}

		std::string str;
		size_t index = -1;

		switch( pItem->m_nType )
		{
		case EVENT_ITEM_TYPE_SCRIPT_ID:
			state->PushString("id");
//			state->PushString(pItem->m_strString.m_szString);
//			if(pItem->m_state == 1 || pItem->m_state == -1)
//			{
//				state->PushInteger("canaccept");
//			}
//			else if(pItem->m_state == 2)
//			{
//				state->PushString("cansubmit");
//			}
			state->PushInteger(2+pItem->m_state);
			state->PushNumber(pItem->m_idScript);
			state->PushNumber(pItem->m_index);
			state->PushString(pItem->m_strString.m_szString);
			break;

		case EVENT_ITEM_TYPE_TEXT:
			state->PushString("text");
//			state->PushString("align");			
			state->PushInteger(2+pItem->m_state);
			state->PushNumber(-1);
			state->PushNumber(-1);

			str = pItem->m_strString.m_szString;
			if(0 == str.find_first_of("&&"))		// string::npos != strPos && 
			{
				index = str.length();
				str = str.substr(2, index - 4);
				index = atoi(str.c_str());

				// ����ȡֵ
				DBC_DEFINEHANDLE(s_pNpcInfoDBC, DBC_MISSION_INFO)
				const _DBC_MISSION_INFO* pNpcInfo = (const _DBC_MISSION_INFO*)(s_pNpcInfoDBC->Search_Index_EQU((INT)index));
				if(pNpcInfo != NULL)
					str = pNpcInfo->szDefault;
			}

			state->PushString(str.c_str());
			break;

		default:
			KLThrow("LUA:GetNPCEventList_Item Unknown ItemType");
			break;
		}

		return 5;
	}

	INT DataPool::GetPrescrList_Num(LuaPlus::LuaState* state)
	{
		INT nNum = (INT)CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr().size();

		
		////��Ӳ��Դ���
		//if ( nNum == 0 )
		//{
		//	for( INT i=0; i<9; ++i )
		//		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Prescr( i, TRUE );
		//	nNum = (INT)CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr().size();
		//}

		state->PushInteger(nNum);

		return 1;
	}

	INT DataPool::GetPrescrList_Item(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetProscrList_Item");
		}	

		INT nIndex = args[2].GetInteger();

		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		if(!pPrescr) 
		{
			KLThrow("LUA:GetPrescrList_Item NULL Item");
		}

		state->PushString( pPrescr->m_pDefine->szName );

		return 1;
	}

	INT DataPool::GetPrescrList_Item_NeedTool(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger()) )
		{
			KLThrow("LUA:GetPrescrList_Item_NeedTool Param1");
		}

		INT nIndex = args[2].GetInteger();
		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		if(!pPrescr) 
		{
			KLThrow("LUA:GetPrescrList_Item_NeedTool NULL Item");
		}
		state->PushInteger( pPrescr->m_pDefine->nFacility);

		return 1;
	}

	INT DataPool::GetPrescrList_Item_Desc(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPrescrList_Item_Desc Param1");
		}	

		INT nIndex = args[2].GetInteger();

		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		if(!pPrescr) 
		{
			KLThrow("LUA:GetPrescrList_Item_Desc NULL Item");
		}

		state->PushString( pPrescr->m_pDefine->szExplain );

		return 1;
	}

	INT DataPool::GetPrescrList_Item_ListPos(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if ( !(args[2].IsInteger() ) )
		{
			KLThrow("LUA:GetPrescrList_Item_ListPos Param1");
		}
		if ( !(args[3].IsInteger() ) )
		{
			KLThrow("LUA:GetPrescrList_Item_ListPos Param2");
		}

		INT nIndex = args[2].GetInteger();
		INT nAbilityID = args[3].GetInteger();

		SPRESCR_MAP::const_iterator it = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr().begin();

		INT pos = 0;
		for ( ; it != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr().end(); ++it )
		{
			if( pos++==nIndex && it->second.m_pDefine->nLifeAbility == nAbilityID )
			{
				state->PushInteger( it->second.m_pDefine->nID );
				state->PushString( it->second.m_pDefine->szName );
				return 2;
			}
		}

		return 0;
		
	}
	
	INT DataPool::GetPrescrList_Item_LifeAbility(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPrescrList_Item_LifeAbility");
		}	

		INT nIndex = args[2].GetInteger();

		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
/*		const SPRESCR_MAP& mapPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr();
		SPRESCR_MAP::const_iterator it;
		INT count;
		for(it=mapPrescr.begin(); it!=mapPrescr.end(); it++,count++)
		{
			if(count == nIndex) break;
		}
*/		if(!pPrescr) 
		{
			KLThrow("LUA:GetPrescrList_Item_LifeAbility NULL Item");
		}
//		state->PushInteger( (&(it->second))->m_pDefine->nID)
		state->PushInteger( pPrescr->m_pDefine->nLifeAbility );

		return 1;
	}
	INT DataPool::GetPrescrList_Item_Result(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetPrescrList_Item_Result Args error");
		}	

		INT nIndex = args[2].GetInteger();

		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		if(!pPrescr) 
		{
			KLThrow("LUA:GetPrescrList_Item_Result NULL Item");
		}

		state->PushInteger( pPrescr->m_pDefine->nResultID );
		state->PushInteger( pPrescr->m_pDefine->nResultNum );


		return 2;
	}

	INT	DataPool::GetBoothList_Num(LuaPlus::LuaState* state) // �õ��ж��ٸ�����
	{
		INT nNum = CDataPool::GetMe()->Booth_GetNumber();
		state->PushInteger( nNum );
		return 1;
	}
	INT	DataPool::GetBoothGood_ID( LuaPlus::LuaState* state ) // �õ������id 
	{
		// �������ڻ����ϵ����к�
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBoothGood_ID");
		}	
		INT nIndex = args[ 2 ].GetInteger();
		if( nIndex >= (INT)CDataPool::GetMe()->Booth_GetNumber() )
		{
			KLThrow("LUA:GetBoothGood_ID");
			return 0;
		}
		tObject_Item* pItem = CDataPool::GetMe()->Booth_GetItem( nIndex );
		state->PushInteger( pItem->GetID() );
		return 1;
	}
	INT	DataPool::GetBoothGood_Name(LuaPlus::LuaState* state) // �õ����������
	{
		// �������ڻ����ϵ����к�
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBoothGood_Name");
		}	
		INT nIndex = args[ 2 ].GetInteger();
		if( nIndex >= (INT)CDataPool::GetMe()->Booth_GetNumber() )
		{
			KLThrow("LUA:GetBoothGood_Name");
			return 0;
		}
		tObject_Item* pItem = CDataPool::GetMe()->Booth_GetItem( nIndex );
		state->PushString( pItem->GetName() );
		return 1;
	}
	INT	DataPool::GetBoothGood_Price(LuaPlus::LuaState* state)// �õ�����ļ۸�
	{
		// ��û������
		state->PushString( 0 );
		return 1;
	}


INT DataPool::GetPrescrList_Item_Requirement(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[2].IsInteger()) || !(args[3].IsInteger()))
	{
		KLThrow("LUA:GetPrescrList_Item_Requirement");
	}	

	INT nIndex = args[2].GetInteger();
	INT nMaterial = args[3].GetInteger();

	const SCLIENT_PRESCR* pPrescr = 
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
	if(!pPrescr || !pPrescr->m_pDefine) 
	{
		KLThrow("LUA:GetPrescrList_Item_Requirement NULL Item");
	}
/*	const SCLIENT_LIFEABILITY* pLifeAbility = 
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(pPrescr->m_pDefine->nLifeAbility);
	if(!pLifeAbility)
	{
		KLThrow("LUA:GetPrescrList_Item_Requirement NULL Item");
	}*/
	switch (nMaterial)
	{
	case 1:
		state->PushInteger( pPrescr->m_pDefine->nStuff1ID );
		state->PushInteger( pPrescr->m_pDefine->nStuff1num );
		return 2;
		break;
	case 2:
		state->PushInteger( pPrescr->m_pDefine->nStuff2ID );
		state->PushInteger( pPrescr->m_pDefine->nStuff2num );
		return 2;
		break;
	case 3:
		state->PushInteger( pPrescr->m_pDefine->nStuff3ID );
		state->PushInteger( pPrescr->m_pDefine->nStuff3num );
		return 2;
		break;
	case 4:
		state->PushInteger( pPrescr->m_pDefine->nStuff4ID );
		state->PushInteger( pPrescr->m_pDefine->nStuff4num );
		return 2;
		break;
	case 5:
		state->PushInteger( pPrescr->m_pDefine->nStuff5ID );
		state->PushInteger( pPrescr->m_pDefine->nStuff5num );
		return 2;
		break;
	default :
		return 0;
		break;
	}
}


//---------------------------------------------------------------------------------------------------------------
// ������.

// �Ƿ������״̬��
INT DataPool::GetMyTeamFlag(LuaPlus::LuaState* state)
{
	BOOL bFlag = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_HaveTeamFlag();
	state->PushInteger(bFlag);
	return 1;
}

// �õ���ǰ��������
INT DataPool::GetMyTeamMemberCount(LuaPlus::LuaState* state)
{
	BOOL bFlag = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_HaveTeamFlag();
	if (TRUE == bFlag)
	{
		INT nNum = CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount();
		state->PushInteger(nNum);
		return 1;
	}
	state->PushInteger(0);
	return 1;
}

// �Լ������(��npc)��ľ���
INT DataPool::GetDistanceBySeverID(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("GetDistanceBySeverID(), args[2] error!!!");
	}	

	INT nSeverID = args[2].GetInteger();

	tObject* pObj = CObjectManager::GetMe()->FindServerObject(nSeverID);
	// ��Ұ��
	if (pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
	{
		CObject_Character* pChar = (CObject_Character*)pObj;

		const fVector3& myPos	 = CObjectManager::GetMe()->GetMySelf()->GetPosition();
		const fVector3& otherPos = pChar->GetPosition();

		FLOAT fDist = KLU_GetDist(myPos, otherPos);

		state->PushNumber(fDist);
		return 1;
	}

	// ��Ұ��
	state->PushInteger(-1);	
	return 1;
}

// �õ���һ�����ѵ�severid
INT DataPool::GetAnotherTeamMemberSeverID(LuaPlus::LuaState* state)
{
	BOOL bFlag = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_HaveTeamFlag();
	if (TRUE == bFlag)
	{
		CTeamOrGroup* pTeamInfo = CUIDataPool::GetMe()->GetTeamOrGroup();
		if (pTeamInfo)
		{
			INT nNum = pTeamInfo->GetTeamMemberCount();
			GUID_t myGUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();
			for (INT i=0; i<nNum; ++i)
			{
				const TeamMemberInfo* pInfo = pTeamInfo->GetMemberByIndex(i);
				if (pInfo && pInfo->m_GUID != myGUID)
				{
					state->PushInteger(pInfo->m_OjbID);
					return 1;
				}
			}
		}
		
	}
	
	state->PushNumber(-1);
	return 1;
}

// �õ�clientid
INT DataPool::GetClientIDBySeverID(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("GetDistanceBySeverID(), args[2] error!!!");
	}	

	INT nSeverID = args[2].GetInteger();

	tObject* pObj = CObjectManager::GetMe()->FindServerObject(nSeverID);
	if (pObj)
	{
		state->PushInteger(pObj->GetID());
		return 1;
	}

	state->PushInteger(-1);
	return 1;

}


// ���ڶ����е��Լ�����Ϣ, ��ͨ��������Ϣ����, ÿ�δ򿪽����ʱ��Ҫˢ���Լ�����Ϣ
INT	DataPool::SetSelfInfo(LuaPlus::LuaState* state)
{
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		CUIDataPool::GetMe()->GetTeamOrGroup()->FillMyInfo();
		CUIDataPool::GetMe()->GetTeamOrGroup()->SetModelLook();
	}

	return 0;
}


// �ӳ��򿪶�����Ϣʱ, ѡ�е�����
INT	DataPool::SetSelectTeamMember_Leader(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA:SetSelectTeamMember_Leader");
	}	

	INT iIndex = args[2].GetInteger();

	if(CUIDataPool::GetMe())
	{
		CUIDataPool::GetMe()->m_iSelectedTeamMember_Leader = iIndex;
	}
	
	return 0;
}

// �õ���һ��ʾ���Ƿ��Ƕӳ�
INT	DataPool::IsTeamLeader(LuaPlus::LuaState* state)		
{
	INT iIsLeader = 0;
	const TeamMemberInfo* pMemberInfo = NULL;

        //laim need_fix
	iIsLeader = 1;
	state->PushInteger(iIsLeader);
	return 1;
	/*
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		pMemberInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByUIIndex(1);
		if(pMemberInfo)
		{
			if(pMemberInfo->m_GUID == CUIDataPool::GetMe()->GetTeamOrGroup()->GetLeaderGUID())
			{
				iIsLeader = 1;
			}

		}
	}

	state->PushInteger(iIsLeader);
	return 1;
*/
}

BOOL DataPool::GetMyTeamLeaderFlag(LuaPlus::LuaState* state)
{
	if(CUIDataPool::GetMe()->GetTeamOrGroup()->HasTeam())
	{
		if( CUIDataPool::GetMe()->GetTeamOrGroup()->GetLeaderGUID()
			== CObjectManager::GetMe()->GetMySelf()->GetServerGUID()
		 )
		{
			state->PushInteger(1);
			return 1;
		}
	}

	state->PushInteger(0);
	return 1;
}

INT	DataPool::GetTeamMemUIModelNameByIndex(LuaPlus::LuaState* state)		// �õ���Ա��uiģ������
{

	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA:GetTeamMemUIModelNameByIndex");
	}	
	INT iIndex = args[2].GetInteger();

	const TeamMemberInfo* pMemberInfo = NULL;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{

		pMemberInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex(iIndex);
		if(pMemberInfo)
		{
		
			//pMemberInfo->m_UIModel.GetUIModelName()
			// ��Աuiģ������
			state->PushString(pMemberInfo->m_UIModel.m_szBufModel);
			return 1;
		}

	}
	
	// ��Աuiģ������
	state->PushString("");
	return 1;

}


INT	DataPool::SelectAsTargetByUIIndex(LuaPlus::LuaState* state)	// ͨ������ͷ��, ѡ�ж�����Ϊtarget
{

	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA:SelectAsTargetByUIIndex");
	}	
	INT iUIIndex = args[2].GetInteger();

	INT iObjServerId = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberServerIdByUIIndex(iUIIndex);
	GUID_t	GUID     = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberGUIDByUIIndex(iUIIndex);
	if(-1 != iObjServerId)
	{
		CGameInterface::GetMe()->Object_SelectAsMainTarget(iObjServerId, 0);
	}


	if(-1 != GUID)
	{
		// ���õ�ǰѡ�е��Ƕ���
		CObjectManager::GetMe()->SetCurSelTeam(GUID);
		
		// ���õ�ǰѡ�ж����Ƿ��ڳ����С�
		INT	iPos = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberUIIndex(GUID);
		if(CUIDataPool::GetMe()->GetTeamOrGroup()->IsInScene(iPos))
		{
			CObjectManager::GetMe()->SetPresent(TRUE);
		}
		else
		{
			CObjectManager::GetMe()->SetPresent(FALSE);
		}

		CObject* pFindObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(iObjServerId));
		if((-1 == iObjServerId)||(NULL == pFindObj))
		{
			if(CObjectManager::GetMe()->GetMainTarget())
			{
				CObjectManager::GetMe()->UnSelMainTarget();
			}

			CGameProcedure::s_pEventSystem->PushEvent(GE_MAINTARGET_OPEND);
		}

	}
	return 0;
}

INT	DataPool::GetTeamMemInfoByIndex(LuaPlus::LuaState* state)		// �õ���Ա����Ϣ
{

	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA:GetTeamMemInfoByIndex");
	}	

	INT iIndex = args[2].GetInteger();
	const TeamMemberInfo* pMemberInfo = NULL;

	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{

		pMemberInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex(iIndex);
		if(pMemberInfo)
		{
			// ��Ա����
			state->PushString(pMemberInfo->m_szNick);
		
			// 2.ְҵ
			state->PushInteger(pMemberInfo->m_uFamily);
			
			// 3.�ȼ�
			state->PushInteger(pMemberInfo->m_uLevel);

			// 4.������Ϣ
			state->PushInteger(pMemberInfo->m_bDead);

			// 5.������Ϣ
			state->PushInteger(pMemberInfo->m_bDeadLink);

			// 6.�Ա�
			state->PushInteger(pMemberInfo->m_uDataID);

			return 6;
		
		}

	}

	// ����ò�����Ա����Ϣ, ����Ĭ��ֵ.
	if(NULL == pMemberInfo)
	{

		// ��Ա����
		state->PushString("???");

		// 2.����
		state->PushInteger(0);
		
		// 3.�ȼ�
		state->PushInteger(0);
		
		// 4.������Ϣ
		state->PushInteger(0);

		// 5.������Ϣ
		state->PushInteger(0);

		state->PushInteger(0);
		return 6;
	}
	
	return 0;

}


INT	DataPool::GetTeamMemUIModelName(LuaPlus::LuaState* state)		// �õ���Աuiģ������
{

	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA:GetTeamMemUIModelName");
	}	

	INT iUIIndex = args[2].GetInteger();
	const TeamMemberInfo* pMemberInfo = NULL;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{

		pMemberInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex(iUIIndex);
		if(pMemberInfo)
		{
			
			// ��Աuiģ������
			//state->PushString(pMemberInfo->m_UIModel.GetUIModelName());
			state->PushString(pMemberInfo->m_UIModel.m_szBufModel);
			return 1;

		}
	
	}

	// ��Աuiģ������
	state->PushString("");
	return 1;
}

	// ��Ա���Լ��Ƿ���ͬһ��������
INT	DataPool::IsTeamMemberInScene(LuaPlus::LuaState* state)	
{

	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA:IsTeamMemberInScene");
	}	

	INT iIndex = args[2].GetInteger();	
	
	INT iIsInScene = 0;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		iIsInScene = CUIDataPool::GetMe()->GetTeamOrGroup()->IsInScene(iIndex);
	}
		
	state->PushInteger(iIsInScene);
	return 1;
}

INT	DataPool::GetSelfGUID(LuaPlus::LuaState* state)
{
	
	LuaStack args(state);
	state->PushInteger( CObjectManager::GetMe()->GetMySelf()->GetGUID() );

	return 1;
}

INT		DataPool::GetTeamMemGUIDByUIIndex(LuaPlus::LuaState* state)
{
	//CObjectManager::GetMe()->GetMySelf()->GetGUID();
	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA:GetTeamMemGUIDByUIIndex");
		return 0;
	}	
	INT iUIIndex = args[2].GetInteger();
	const TeamMemberInfo* pMemberInfo = NULL;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		pMemberInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByUIIndex(iUIIndex);
		state->PushInteger( pMemberInfo->m_GUID );
	}

	return 1;
}

INT	DataPool::GetTeamMemPosByUIIndex(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if ( !(args[2].IsInteger()) )
	{
		KLThrow( "LUA:GetTeamMemPosByUIIndex" );
	}
	INT iUIIndex = args[2].GetInteger();
	const TeamMemberInfo* pMemberInfo = NULL;

	if( CUIDataPool::GetMe()->GetTeamOrGroup() )
	{

		pMemberInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByUIIndex(iUIIndex);
		if(pMemberInfo)
		{
			state->PushInteger( pMemberInfo->m_SceneID );				//�õ���Ա���ڵĳ���ID
			state->PushInteger( pMemberInfo->m_WorldPos.m_fX );		//�õ���Ա���ڵ�λ����Ϣ
			state->PushInteger( pMemberInfo->m_WorldPos.m_fZ );
		}
	}

	return 3;
}

INT	DataPool::GetTeamMemInfo(LuaPlus::LuaState* state)		// �õ���Ա����Ϣ
{

	LuaStack args(state);
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA:GetTeamMemInfo");
	}	

	INT iUIIndex = args[2].GetInteger();
	const TeamMemberInfo* pMemberInfo = NULL;

	if( CUIDataPool::GetMe()->GetTeamOrGroup() )
	{

		pMemberInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByUIIndex(iUIIndex);
		if(pMemberInfo)
		{
			// 1 ��Ա����
			state->PushString(pMemberInfo->m_szNick);
			

			//----------------------------------------------------------------------------------------------------------------------------
			// �õ�����ͷ��
			// �õ���Һ��Լ���ͷ����Ϣ
		
			DBC_DEFINEHANDLE(s_pFaceImageDBC, DBC_CHARACTER_FACE)	// char_face.tab
			//�����ﶨ���
			const _DBC_CHAR_FACE* pFaceInfo = NULL;
			pFaceInfo = (const _DBC_CHAR_FACE*)( s_pFaceImageDBC->Search_Index_EQU( pMemberInfo->m_nPortrait ) );
			
			// 2 �õ�ͷ������
			if(pFaceInfo)
			{
				state->PushString(pFaceInfo->pImageSetName);
			}
			else
			{
				int nRaceID = (int)pMemberInfo->m_uDataID;
				nRaceID = nRaceID % 2;
				const _DBC_CHAR_FACE* pFaceImage = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_First_Column_Equ( 2, nRaceID ));
				if (pFaceImage)
				{
					state->PushString(pFaceImage->pImageSetName);
				}
				else
				{
					state->PushString("");
				}
			}

			//
			//-----------------------------------------------------------------------------------------------------------------------------

			// 3 �õ�hp
			if(pMemberInfo->m_SceneID != CWorldManager::GetMe()->GetActiveSceneID())
			{
				state->PushInteger(-1);
			}
			else
			{
				state->PushInteger(pMemberInfo->m_nHP);
			}

			// 4 �õ����hp
			if(0 == pMemberInfo->m_dwMaxHP)
			{
				state->PushInteger(1);
			}
			else
			{
				state->PushInteger(pMemberInfo->m_dwMaxHP);
			}

			// 5 �õ�mp
			state->PushInteger(pMemberInfo->m_nMP);

			// 6 �õ����mp
			if(0 == pMemberInfo->m_dwMaxMP)
			{
			
				state->PushInteger(1);//

			}
			else
			{
				
				state->PushInteger(pMemberInfo->m_dwMaxMP);//
			}


			// 7 ����
			state->PushInteger(pMemberInfo->m_uFamily);
			
			// 8 �ȼ�
			state->PushInteger(pMemberInfo->m_uLevel);

			// 9.ŭ��
			state->PushInteger(pMemberInfo->m_nAnger);

			// 10.����
			state->PushInteger(pMemberInfo->m_bDeadLink);

			// 11.����
			state->PushInteger(pMemberInfo->m_bDead);

			// 12.�Ա�
			state->PushInteger(pMemberInfo->m_uDataID);

		
		
		}

	}

	// ����ò�����Ա����Ϣ, ����Ĭ��ֵ.
	if(NULL == pMemberInfo)
	{

		// ��Ա����
		state->PushString("??????");

		// �õ�ͷ��id
		state->PushInteger(1);

		// �õ�hp
		state->PushInteger(0);

		// �õ����hp
		state->PushInteger(100);

		// �õ�mp
		state->PushInteger(0);

		// �õ����mp
		state->PushInteger(100);


		
		// .����
		state->PushInteger(0);
		
		// .�ȼ�
		state->PushInteger(0);

		// .ŭ��
		state->PushInteger(0);

		// .����
		state->PushInteger(0);

		// .����
		state->PushInteger(0);

		// .�Ա�
		state->PushInteger(0);

	}

	
	return 12;
}

INT	DataPool::GetTeamMemCount(LuaPlus::LuaState* state)		// �õ���Ա�ĸ���
{

	// �õ���Ա�ĸ���
	INT iMemberCount = 0;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		iMemberCount = CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount();
	}

	state->PushInteger(iMemberCount);
	return 1;
}

//��Щ�����������Ϣ����ʹ��.  ����ӿ�.   

// �õ��������ĸ���.
INT	DataPool::GetInviteTeamCount(LuaPlus::LuaState* state)				
{
	// �õ���Ա�ĸ���
	INT iTeamCount = 0;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		iTeamCount = CUIDataPool::GetMe()->GetTeamOrGroup()->GetInviteTeamCount();
	}

	state->PushInteger(iTeamCount);
	return 1;

}

// �õ�ĳ���������Ķ�Ա�ĸ���.
INT	DataPool::GetInviteTeamMemberCount(LuaPlus::LuaState* state)				
{
	// �õ���Ա�ĸ���
	LuaStack args(state);
	if (!args[2].IsInteger())
	{
		KLThrow("LUA:GetInviteTeamMemberCount");
	}	

	const InviteTeam* pInviteTeam = NULL;
	TeamCacheInfo*    pMemberInfo = NULL;

	INT iTeamIndex   = args[2].GetInteger();
	INT iMemberCount = 0;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		
		pInviteTeam = CUIDataPool::GetMe()->GetTeamOrGroup()->GetInviteTeamByIndex(iTeamIndex);
		if(pInviteTeam)
		{
			iMemberCount = (int)pInviteTeam->m_InvitersInfo.size();
			state->PushInteger(iMemberCount);
			return 1;
		}

	}

	// û�ҵ����ض�Ա����0
	state->PushInteger(0);
	return 1;

}



// �õ�����uiģ������
INT DataPool::GetInviteTeamMemberUIModelName(LuaPlus::LuaState* state)							
{
	const InviteTeam* pInviteTeam = NULL;
	TeamCacheInfo*    pMemberInfo = NULL;

	LuaStack args(state);
	if ((!(args[2].IsInteger()))||(!(args[3].IsInteger())))
	{
		KLThrow("LUA:GetInviteTeamMemberUIModelName");
	}	

	INT iTeamIndex   = args[2].GetInteger();
	INT iMemberIndex = args[3].GetInteger();

	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		
		pInviteTeam = CUIDataPool::GetMe()->GetTeamOrGroup()->GetInviteTeamByIndex(iTeamIndex);
		if(pInviteTeam)
		{
			if(iMemberIndex < (int)pInviteTeam->m_InvitersInfo.size())
			{//��Ա�����ںϷ��ķ�Χ��.

				// ��¼��ǰ��������guid
				CUIDataPool::GetMe()->m_IdSource = pInviteTeam->m_InviterGUID;

				// ��ǰ�ܾ����������
				CUIDataPool::GetMe()->m_iCurRejectTeamIndex = iTeamIndex;
				
				// ��Ա��Ϣ
				pMemberInfo =  pInviteTeam->m_InvitersInfo[iMemberIndex];
				if(pMemberInfo)
				{
					// �ǳ�����
					//state->PushString(pMemberInfo->m_UIModel.GetUIModelName());
					state->PushString(pMemberInfo->m_UIModel.m_szBufModel);
					return 1;

				}
			}
		}
	}

	// �ǳ�����
	state->PushString("");
	return 1;
}


// �õ��������Ϣ.		
INT DataPool::GetInviteTeamMemberInfo(LuaPlus::LuaState* state)							
{
	const InviteTeam* pInviteTeam = NULL;
	TeamCacheInfo*    pMemberInfo = NULL;

	LuaStack args(state);
	if ((!(args[2].IsInteger()))||(!(args[3].IsInteger())))
	{
		KLThrow("LUA:GetInviteTeamMemberInfo");
	}	

	INT iTeamIndex   = args[2].GetInteger();
	INT iMemberIndex = args[3].GetInteger();

	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		
		pInviteTeam = CUIDataPool::GetMe()->GetTeamOrGroup()->GetInviteTeamByIndex(iTeamIndex);
		if(pInviteTeam)
		{
			if(iMemberIndex < (int)pInviteTeam->m_InvitersInfo.size())
			{//��Ա�����ںϷ��ķ�Χ��.

				// ��¼��ǰ��������guid
				//CUIDataPool::GetMe()->m_IdSource = pInviteTeam->m_InviterGUID;

				// ��ǰ�ܾ����������
				CUIDataPool::GetMe()->m_iCurRejectTeamIndex = iTeamIndex;
				
				// ��Ա��Ϣ
				pMemberInfo =  pInviteTeam->m_InvitersInfo[iMemberIndex];
				if(pMemberInfo)
				{
					// 1 �ǳ�����
					state->PushString(pMemberInfo->m_szNick);
					// 2 �õ�ְҵ
					state->PushInteger(pMemberInfo->m_uFamily);
					// 3 �õ��ȼ�
					state->PushInteger(pMemberInfo->m_uLevel);
					// 10, �Ա�
					state->PushInteger(pMemberInfo->m_uDataID);

					// ���µõ�װ����Ϣ
					// 4 �õ�ñ��
					state->PushInteger(pMemberInfo->m_CapID);
					// 6 �õ�����
					state->PushInteger(pMemberInfo->m_ArmourID);
					// ��
					state->PushInteger(pMemberInfo->m_ShoulderID);
					// 7 �õ�����
					state->PushInteger(pMemberInfo->m_CuffID);
					// 8 �õ���
					state->PushInteger(pMemberInfo->m_FootID);
					// 9 �õ�����
					state->PushInteger(pMemberInfo->m_WeaponID);
					state->PushInteger(pMemberInfo->m_AssihandID);

					return 11;

				}// if(pMemberInfo)
			}

		}

	}


	// ����ò�����ϸ��Ϣ, ����Ĭ����Ϣ

	// �ǳ�����
	state->PushString("???");

	// �õ�����
	state->PushInteger(0);

	// �õ��ȼ�
	state->PushInteger(0);

	// ���µõ�װ����Ϣ
	// �õ�ñ��
	state->PushInteger(0);
	// �õ�ͷ
	state->PushInteger(0);
	// �õ�����
	state->PushInteger(0);
	// �õ�����
	state->PushInteger(0);
	// �õ���
	state->PushInteger(0);
	// �õ�����
	state->PushInteger(0);
	// �õ��Ա�
	state->PushInteger(0);

	return 10;

}


//��Щ�����������Ϣ����ʹ��.  ����ӿ�. 
INT		DataPool::GetApplyMemberCount(LuaPlus::LuaState* state)		// �õ������߸���.
{

	// �õ���Ա�ĸ���
	INT iApplyCount = 0;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{

		iApplyCount = CUIDataPool::GetMe()->GetTeamOrGroup()->GetProposerCount();

	}

	state->PushInteger(iApplyCount);
	return 1;
}//

// �õ������ߵ�uiģ������
INT DataPool::GetApplyMemberUIModelName(LuaPlus::LuaState* state)
{
	// ���������õ�ĳ��������
	const TeamCacheInfo* pApplyInfo = NULL;

	LuaStack args(state);
	if (!args[2].IsInteger())
	{
		KLThrow("LUA:GetApplyMemberUIModelName");
	}	

	INT iApplyIndex  = args[2].GetInteger();
	if( CUIDataPool::GetMe()->GetTeamOrGroup() )
	{
		pApplyInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetProposerByIndex(iApplyIndex);
		if( pApplyInfo )
		{
			//state->PushString(pApplyInfo->m_UIModel.GetUIModelName());
			state->PushString(pApplyInfo->m_UIModel.m_szBufModel);
			return 1;
		}
	}
	
	// uiģ������
	state->PushString("");

	return 1;
}

// �õ������ߵ���Ϣ.
INT DataPool::GetApplyMemberInfo(LuaPlus::LuaState* state)
{
	// ���������õ�ĳ��������
	const TeamCacheInfo* pApplyInfo = NULL;

	LuaStack args(state);
	if (!args[2].IsInteger())
	{
		KLThrow("LUA:GetApplyMemberInfo");
	}	

	INT iApplyIndex  = args[2].GetInteger();
	if( CUIDataPool::GetMe()->GetTeamOrGroup() )
	{
		pApplyInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetProposerByIndex( iApplyIndex );
		if( pApplyInfo )
		{
			// 1 �ǳ�����
			state->PushString(pApplyInfo->m_szNick);
			// 2 �õ�ְҵ
			state->PushInteger(pApplyInfo->m_uFamily);
			// 3 �õ��ȼ�
			state->PushInteger(pApplyInfo->m_uLevel);

			// ���µõ�װ����Ϣ
			// 4 �õ�ñ��
			state->PushInteger(pApplyInfo->m_CapID);
			// 5 �õ�ͷ
//			state->PushInteger(0);
			// 6 �õ�����
			state->PushInteger(pApplyInfo->m_ArmourID);
			// ��
			state->PushInteger(pApplyInfo->m_ShoulderID);
			// 7 �õ�����
			state->PushInteger(pApplyInfo->m_CuffID);
			// 8 �õ���
			state->PushInteger(pApplyInfo->m_FootID);
			// 9 �õ�����
			state->PushInteger(pApplyInfo->m_WeaponID);
			state->PushInteger(pApplyInfo->m_AssihandID);
			// 10 �Ա�
			state->PushInteger(pApplyInfo->m_uDataID);

            return 11;
		}

	}

	// ����ò�����ϸ��Ϣ, ����Ĭ����Ϣ
	// �ǳ�����
	state->PushString("???");

	// �õ�����
	state->PushInteger(0);

	// �õ��ȼ�
	state->PushInteger(0);

	// ���µõ�װ����Ϣ
	// �õ�ñ��
	state->PushInteger(0);
	// �õ�ͷ
	state->PushInteger(0);
	// �õ�����
	state->PushInteger(0);
	// �õ�����
	state->PushInteger(0);
	// �õ���
	state->PushInteger(0);
	// �õ�����
	state->PushInteger(0);

	// �Ա�
	state->PushInteger(0);

	return 10;

}

// ������ӶԻ���򿪱��.
INT DataPool::SetTeamFrameOpenFlag(LuaPlus::LuaState* state)	// ���ö���Ի���򿪱��
{
	// ���ö�����Ϣ�Ի���Ĵ򿪷�ʽ.
	LuaStack args(state);
	if (!args[2].IsInteger())
	{
		KLThrow("LUA:SetTeamFrameOpenFlag");
	}	
	INT iOpenType = args[2].GetInteger();
	CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_NOTIFY_APPLY, iOpenType);
	return 0;
}

// ���õ�ǰѡ���������.
INT DataPool::SetCurSelApply(LuaPlus::LuaState* state)
{
	// ���������õ�ĳ��������
	const TeamCacheInfo*	pApplyInfo = NULL;

	LuaStack args(state);
	if(!args[2].IsInteger())
	{
		KLThrow("LUA:SetCurSelApply");
	}	

	INT iApplyIndex  = args[2].GetInteger();
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		CUIDataPool::GetMe()->m_iSelectApply_Apply = iApplyIndex;

		pApplyInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetProposerByIndex(iApplyIndex);
		if(pApplyInfo)
		{
			CUIDataPool::GetMe()->SetSourceGUID(pApplyInfo->m_GUID);
		}
	}

	return 0;
}

// ���õ�ǰѡ���������.
INT     DataPool::EraseApply(LuaPlus::LuaState* state)
{

	// ���������õ�ĳ��������
	const TeamCacheInfo*	pApplyInfo = NULL;

	LuaStack args(state);
	if (!args[2].IsInteger())
	{
		KLThrow("LUA:EraseApply");
	}	

	INT iApplyIndex  = args[2].GetInteger();
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		CUIDataPool::GetMe()->GetTeamOrGroup()->EraseProposer(iApplyIndex);
	}

	return 0;
}

// ������е�������.
INT	DataPool::ClearAllApply(LuaPlus::LuaState* state)
{
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		CUIDataPool::GetMe()->GetTeamOrGroup()->ClearProposer();
	}

	return 0;
}


// ������.
//-------------------------------------------------------------------------------------------------------------

// 1 �ڼ�ҳ
// 2 �ڼ�����Ʒ
INT DataPool::GetPacketItem( LuaPlus::LuaState* state )
{
#define MAX_ITEM_PER_PAGE MAX_BAG_SIZE / 3
	
	LuaStack args( state );
	
	if( !args[ 2 ].IsInteger() ) 
		return 0;
	if( !args[ 3 ].IsInteger() ) 
		return 0;
	
	INT nPage = args[ 2 ].GetInteger();
	INT nIndex = args[ 3 ].GetInteger();
	INT nFirst = nPage * MAX_ITEM_PER_PAGE;
	
	tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem( nFirst + nIndex );
	if( pItem && pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET )
	{
		state->PushInteger( pItem->GetID() );
		state->PushInteger( pItem->GetNumber() );
	}
	else
	{
		state->PushInteger( -1 );
		state->PushInteger( 0 );
	}

	return 2;
}

// 1 �ڼ�ҳ
// 2 �ڼ�����Ʒ
INT DataPool::GetPacketItemFroId( LuaPlus::LuaState* state )
{
	LuaStack args( state );
	if( !args[ 2 ].IsInteger() ) return 0;
	 
	INT nIndex = args[ 2 ].GetInteger();

	tObject_Item* pItem =  CDataPool::GetMe()->UserBag_GetItem(  nIndex );
	

	if( pItem && pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET )
	{
		state->PushInteger( pItem->GetIdTable() );
		state->PushInteger( pItem->GetNumber() );
	}
	else
	{
		state->PushInteger( -1 );
		state->PushInteger( -1 );
	}
	return 2;
}

//�õ���������ĳid��Ʒ��������
INT DataPool::GetPacketItemAllNum( LuaPlus::LuaState* state )
{
	LuaStack args( state );
	if( !args[ 2 ].IsInteger() ) return 0;

	INT nIItemId = args[ 2 ].GetInteger();
	tObject_Item* pItem  = NULL;
	INT  ItemNum = 0;

	for ( INT i =0 ;i <MAX_BAG_SIZE ;++i)
	{

		pItem =  CDataPool::GetMe()->UserBag_GetItem(  i );

		if ( pItem && pItem->GetIdTable() == nIItemId )
		{
			ItemNum += pItem->GetNumber();

		}

	}

	state->PushInteger( ItemNum );

	return 1;
}


// �õ��ڼ�ҳ�м�����Ʒ
INT DataPool::GetPacketItemMaxNumber( LuaPlus::LuaState* state )
{
	LuaStack args( state );
	if( !args[ 1 ].IsInteger() ) return 0;
	state->PushInteger( MAX_BAG_SIZE / 3 );

	return 1;
}

 // �õ�ĳһ��ĺ�������
// in: 1   �����������
// out: 1  ����Ҫ������ĺ��ѵ�����
INT DataPool::GetFriendNumber( LuaPlus::LuaState* state )
{
	LuaStack args( state );
	if( !args[ 2 ].IsInteger() ) 
		return 0;

	int nCount = CDataPool::GetMe()->GetRelation()->GetListCount( RELATION_GROUP( args[ 2 ].GetInteger() ) );
	state->PushInteger( nCount );
	
	return 1;
}

//�õ�ĳһ��ĺ��ѵľ�������
// in:	1, ָ������
//		2, ָ�������к�
//		3, ��Ҫ����������
INT DataPool::GetFriend( LuaPlus::LuaState* state )
{
	LuaStack args( state );
	const SDATA_RELATION_MEMBER *pInfo = NULL;
	std::string  str;

	if(args[2].IsString() && 0 == strcmp(args[2].GetString(),"chat"))
	{
		pInfo = CDataPool::GetMe()->GetRelation()->GetPlayerCheckInfo();
		str = args[3].GetString();
	}
	else
	{
		pInfo = CDataPool::GetMe()->GetRelation()->GetRelationInfo( RELATION_GROUP( args[ 2 ].GetInteger() ), args[ 3 ].GetInteger() );
		str = args[ 4 ].GetString();
	}


	if( pInfo )
	{
		if( str == "ID" )// GUID
		{
			state->PushInteger( pInfo->m_GUID );
		}
		else if( str == "ID_TEXT" )
		{
			char szInfo[ 16 ];
			sprintf( szInfo,"%08X", DWORD(pInfo->m_GUID ) );
			state->PushString( szInfo );
		}
		// ����
		else if( str == "NAME" ) 
		{
			state->PushString( pInfo->m_szName );
		}
		// ��ϵ����
		else if( str == "RELATION_TYPE" ) 
		{
			state->PushInteger( pInfo->m_RelationType );
		}
		// ��ϵ����
		else if( str == "RELATION_TEXT" ) 
		{

			//switch( pInfo->m_RelationType )
			//{
			//case RELATION_TYPE_NONE:
			//	{
			//		STRING strTemp = "";
			//		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_DataPool_Info_None");
			//		state->PushString( strTemp.c_str() );
			//		//state->PushString( "��" );
			//	}
			//	break;
			//case RELATION_TYPE_FRIEND:
			//	{
			//		STRING strTemp = "";
			//		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_DataPool_Info_Good_Friend");
			//		state->PushString( strTemp.c_str() );
			//		//state->PushString( "����" );
			//	}
			//	break;
			//case RELATION_TYPE_BROTHER:						//���
			//	{
			//		STRING strTemp = "";
			//		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_DataPool_Info_JieBai");
			//		state->PushString( strTemp.c_str() );
			//		//state->PushString( "���" );
			//	}
			//	break;
			//case RELATION_TYPE_MARRY:
			//	{
			//		STRING strTemp = "";
			//		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_DataPool_Info_Marry");
			//		state->PushString( strTemp.c_str() );
			//		//state->PushString( "���" );
			//	}
			//	
			//	break;
			//case RELATION_TYPE_BLACKNAME:
			//	{
			//		STRING strTemp = "";
			//		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_DataPool_Info_Black_Name_Table");
			//		state->PushString( strTemp.c_str() );
			//	
			//	}
			//	//state->PushString( "������" );
			//	break;
			//case RELATION_TYPE_TEMPFRIEND:
			//	{
			//		STRING strTemp = "";
			//		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_DataPool_Info_Temp_Good_Friend");
			//		state->PushString( strTemp.c_str() );
			//	
			//	}
			//	//state->PushString( "��ʱ����" );
			//	break;
			//case RELATION_TYPE_STRANGER:
			//	{
			//		STRING strTemp = "";
			//		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_DataPool_Info_Stranger");
			//		state->PushString( strTemp.c_str() );
			//	
			//	}
			//	//state->PushString( "İ����" );
			//	break;
			//case RELATION_TYPE_MASTER:
			//	{
			//		STRING strTemp = "";
			//		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_DataPool_Info_Teacher");
			//		state->PushString( strTemp.c_str() );
			//	
			//	}
			//	//state->PushString( "ʦ��" );
			//	break;
			//case RELATION_TYPE_PRENTICE:
			//	{
			//		STRING strTemp = "";
			//		strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_DataPool_Info_Student");
			//		state->PushString( strTemp.c_str() );
			//	
			//	}
			//	//state->PushString( "ͽ��" );
			//	break;
			//default:
			//	break;
			//}//

			state->PushString( pInfo->m_szRelation );

		}
		// ���Ѷ�
		else if( str == "FRIENDSHIP" ) 
		{							 
			state->PushInteger( pInfo->m_nFriendPoint );
		}							 
		// ��ɫ�ȼ�
		else if( str == "LEVEL" )	 
		{							 
			state->PushInteger( pInfo->m_nLevel );
		}							  
		// ְҵ���� MENPAI_ATTRIBUTE
		else if( str == "MENPAI_TYPE" )	  
		{
			state->PushInteger( pInfo->m_nMenPai );
		}
		else if( str == "MENPAI_TEXT" )	  // ���� MENPAI_ATTRIBUTE               		
		{																		 		
			switch( pInfo->m_nMenPai )											 		
			{
			// ������
			case MATTRIBUTE_SHAOLIN_GWS:
			case MATTRIBUTE_SHAOLIN_TLH:
				{
					state->PushString( "������" );
				}
				break;
			// ؤ��
			case MATTRIBUTE_GAIBANG_GGB:
			case MATTRIBUTE_GAIBANG_ZGB:
				{
					state->PushString( "ؤ��" );
				}
				break;
			// �䵱��
			case MATTRIBUTE_WUDANG_JWD:
			case MATTRIBUTE_WUDANG_QWD:
				{
					state->PushString( "�䵱��" );
				}
				break;
			// ����
			case MATTRIBUTE_TANGMEN_DT: 
			case MATTRIBUTE_TANGMEN_XJT:
				{
					state->PushString( "����" );
				}
				break;
			// ��ң��
			case MATTRIBUTE_XIAOYAO_XXY: 
			case MATTRIBUTE_XIAOYAO_ZXY:
				{
					state->PushString( "��ң��" );
				}
				break;
			// ҩ����
			case MATTRIBUTE_YAOWANGGU_KYW:
			case MATTRIBUTE_YAOWANGGU_YYW:
				{
					state->PushString( "ҩ����" );
				}
				break;
			// �һ���
			case MATTRIBUTE_TAOHUAGU_DTH:
			case MATTRIBUTE_TAOHUAGU_YTH:
				{
					state->PushString( "�һ���" );
				}
				break;
			// ��ü��
			case MATTRIBUTE_EMEI_CEM:
			case MATTRIBUTE_EMEI_FEM:
				{
					state->PushString( "��ü��" );
				}
				break;
			// ��ְҵ
			case MATTRIBUTE_WUMENPAI: 
				{
					//STRING strTemp = "";
					//strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_DataPool_Info_WuZhiYe");
					//state->PushString( strTemp.c_str() );
					state->PushString( "������" );
				}
				break;

			default:
				break;
			}

			return 1;
			
		}
		// ͷ��
		else if( str == "PORTRAIT" )  
		{
			DBC_DEFINEHANDLE(s_pFaceImageDBC, DBC_CHARACTER_FACE)	// char_face.tab
			
			const _DBC_CHAR_FACE* pFaceImage = NULL;
			pFaceImage = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_Index_EQU( pInfo->m_nPortrait ) );
			
			if( pFaceImage )
				state->PushString( pFaceImage->pImageSetName );
			else
				state->PushString( "1" );				
		}
		// ���ID
		else if( str == "GUID_ID" )	   
		{
			state->PushInteger( pInfo->m_GuildID );
		}							 
		// �������
		else if( str == "GUID_NAME" )  
		{
			state->PushString( pInfo->m_szGuildName );
		}
		// �Ƿ�����
		else if( str == "ONLINE" )
		{
			if( pInfo->m_bOnlineFlag )
				state->PushBoolean( true );
			else
				state->PushBoolean( false );
		}
		// ����
		else if( str == "MOOD" )
		{
			state->PushString( pInfo->m_szMood );
		}
		// �ƺ�
		else if( str == "TITLE" )
		{
			state->PushString( pInfo->m_szTitle );
		}
		// ���ڳ���
		else if( str == "SCENE" ) 
		{
			state->PushString( pInfo->m_szLocation );
		}
		// ����������0 ��ʾδ��ӣ�
		else if( str == "TEAM_NUMBER" )
		{
			state->PushString( pInfo->m_szTeamDesc );
		}
		else 
		{
			Assert( false );
			return 0;
		}
	}
	
	return 1;
}

// �ƶ�����������
INT DataPool::ThrowToBlackList( LuaPlus::LuaState* state )
{
	LuaStack args( state );

	if(!(args[2].IsInteger()))
	{
		KLThrow("LUA: Friend ThrowToBlackList Wrong Param1");
		return 0;
	}
	if(!(args[3].IsInteger()))
	{
		KLThrow("LUA: Friend ThrowToBlackList Wrong Param2");
		return 0;
	}

	INT igroup	= args[ 2 ].GetInteger();
	INT idx		= args[ 3 ].GetInteger();

	const SDATA_RELATION_MEMBER *pMember = CDataPool::GetMe()->GetRelation()->GetRelationInfo( ( RELATION_GROUP )igroup, idx );
	if ( pMember == NULL ) return 0;

	CGRelation Msg;

	Msg.GetRelation()->CleanUp();
	Msg.GetRelation()->m_Type = REQ_TRANSITION;
	
	RELATION_GUID_UCHAR_UCHAR* pFriend = &(Msg.GetRelation()->m_RelationGUIDUCHARUCHAR);
	if( pFriend == NULL ) return 0;
	
	pFriend->CleanUp();
	pFriend->SetTargetGUID( pMember->m_GUID );
	pFriend->SetRelationType( RELATION_TYPE_BLACKNAME );
	pFriend->SetGroup( RELATION_GROUP_BLACK );
	
	CNetManager::GetMe()->SendPacket( &Msg );
	
	return 0;
}

//�ƶ��������б���
INT DataPool::ThrowToFriendList( LuaPlus::LuaState* state )
{
	//Ϊ�˿���չ���������д�ɿ��Խ������ֲ�����GUID��(GROUP,INDEX)
	LuaStack args( state );

	if(!(args[2].IsInteger()))
	{
		KLThrow("LUA: Friend ThrowToFriendList Wrong Param1");
		return 0;
	}
	
	CGRelation Msg;

	Msg.GetRelation()->CleanUp();
	Msg.GetRelation()->m_Type = REQ_TRANSITION;

	RELATION_GUID_UCHAR_UCHAR* pFriend = &(Msg.GetRelation()->m_RelationGUIDUCHARUCHAR);
	if( pFriend == NULL ) return 0;

	pFriend->CleanUp();

	
	//���������ֵ���򴫽�������group ��index
	if ( (args[2].IsInteger()) && (args[3].IsInteger()) )
	{
		INT igroup	= args[ 2 ].GetInteger();
		INT idx		= args[ 3 ].GetInteger();
		const SDATA_RELATION_MEMBER *pMember = CDataPool::GetMe()->GetRelation()->GetRelationInfo( ( RELATION_GROUP )igroup, idx );
		if ( pMember == NULL ) return 0;

		pFriend->SetTargetGUID( pMember->m_GUID );

	}
	//�����������ֻ��args[2]���򴫽������Ǳ��ƶ���ҵ�guid
	else
	{
		pFriend->SetTargetGUID( args[ 2 ].GetInteger() );
	}

	pFriend->SetRelationType( RELATION_TYPE_FRIEND );
	pFriend->SetGroup( RELATION_GROUP_FRIEND_ALL );

	CNetManager::GetMe()->SendPacket( &Msg );

	return 0;
}


// �ŵ��������б�
INT DataPool::ThrowToEnemyList( LuaPlus::LuaState* state )
{
	//LuaStack args( state );

	//if(!(args[2].IsInteger()))
	//{
	//	KLThrow("LUA: Friend ThrowToEnemyList Wrong Param1");
	//	return 0;
	//}
	//if(!(args[3].IsInteger()))
	//{
	//	KLThrow("LUA: Friend ThrowToEnemyList Wrong Param2");
	//	return 0;
	//}

	//INT igroup	= args[ 2 ].GetInteger();
	//INT idx		= args[ 3 ].GetInteger();

	//const SDATA_RELATION_MEMBER *pMember = CDataPool::GetMe()->GetRelation()->GetRelationInfo( ( RELATION_GROUP )igroup, idx );

	//CGRelation Msg;

	//Msg.GetRelation()->CleanUp();
	//Msg.GetRelation()->m_Type = REQ_TRANSITION;		// ת����Ϊ
	//
	//RELATION_GUID_UCHAR_UCHAR* pFriend = &(Msg.GetRelation()->m_RelationGUIDUCHARUCHAR);
	//
	//pFriend->CleanUp();
	//pFriend->SetTargetGUID( pMember->m_GUID );
	//pFriend->SetRelationType( RELATION_TYPE_ENEMY );
	//pFriend->SetGroup( RELATION_GROUP_ENEMY );
	//
	//CNetManager::GetMe()->SendPacket( &Msg );
	
	return 0;
}

// �ƶ����ƶ���
INT DataPool::ThrowToList( LuaPlus::LuaState* state )
{
	LuaStack args( state );
	
	if( !args[ 2 ].IsInteger() )
	{
		KLThrow("LUA: Friend ThrowToList Wrong Param1");
		return 0;
	}
	if( !args[ 3 ].IsInteger() )
	{
		KLThrow("LUA: Friend ThrowToList Wrong Param2");
		return 0;
	}
	if( !args[ 4 ].IsInteger() )
	{
		KLThrow("LUA: Friend ThrowToList Wrong Param3");
		return 0;
	}

	INT igroup	= args[ 2].GetInteger();
	INT idx		= args[ 3 ].GetInteger();
	INT iSetGroup = args[ 4 ].GetInteger();


	const SDATA_RELATION_MEMBER *pMember = CDataPool::GetMe()->GetRelation()->GetRelationInfo( ( RELATION_GROUP )igroup, idx );

	CGRelation Msg;
	
	Msg.GetRelation()->CleanUp();
	Msg.GetRelation()->m_Type = REQ_TRANSITION;
	
	RELATION_GUID_UCHAR_UCHAR* pFriend = &(Msg.GetRelation()->m_RelationGUIDUCHARUCHAR);
	
	pFriend->CleanUp();
	pFriend->SetTargetGUID( pMember->m_GUID );
	pFriend->SetRelationType( pMember->m_RelationType ); // ת�����������飬��ϵ���Ͳ��䣬����˵����ݣ�fancy
	pFriend->SetGroup( ( RELATION_GROUP )iSetGroup );
	
	CNetManager::GetMe()->SendPacket( &Msg );
	
	return 0;
}
// ���һ�����ѵ�ĳһ��
// int: 1 ����Ҫ��ӵĺ��ѵ�ID
INT DataPool::AddFriend( LuaPlus::LuaState* state )
{
	LuaStack args( state );

	if( !args[ 2 ].IsInteger() )
	{
		KLThrow("LUA: Friend AddFriend Wrong Param1");
		return 0;
	}

	int nGroup = -1;

	CGRelation Msg;
	
	Msg.GetRelation()->CleanUp();
	Msg.GetRelation()->m_Type = REQ_ADDFRIEND;
	REQUEST_ADD_RELATION_WITH_GROUP* pFriend = &(Msg.GetRelation()->m_AddRelationWithGroup);
	pFriend->CleanUp();

	if( args[ 2 ].GetInteger() == RELATION_GROUP_FRIEND_ALL ) // ���ֱ��һ�����֣����Զ������е��б���ӣ�
	{
		nGroup = 0;
	}
	// ��ʱ����
	else if( args[ 2 ].GetInteger() == RELATION_GROUP_TEMPFRIEND )
	{
		SDATA_RELATION_MEMBER pMember;
		
		if( args[ 3 ].IsString() )
		{
			_snprintf( pMember.m_szName, 30, "%s", args[ 3 ].GetString() );
		}
		else
		{
			// �������Ҳ�����ͳһ��Ӫ�ĲŻ����
			CObject_Character* pCharObj = (CObject_Character*)CObjectManager::GetMe()->GetMainTarget();
			ENUM_RELATION sCamp = CGameInterface::GetMe()->GetCampType( pCharObj, CObjectManager::GetMe()->GetMySelf() );
			if( g_theKernel.IsKindOf( pCharObj->GetClass(), GETCLASS(CObject_PlayerOther) ) == FALSE )
				return 0;
			
			if( sCamp != RELATION_FRIEND )
				return 0;
			
			_snprintf( pMember.m_szName, 30, "%s", pCharObj->GetCharacterData()->Get_Name() );
		}

		// ��������ϵ���б����Ƿ��Ѿ�����ң�����У���ִ���κβ���
		if ( CDataPool::GetMe()->GetRelation()->IsTempFriendName( pMember.m_szName ) )
		{
			return 0;
		}

		// �����ʱ�����������
		int nCount = CDataPool::GetMe()->GetRelation()->GetListCount( RELATION_GROUP_TEMPFRIEND );
		// �����ʱ��������������10�ˣ�����ɾ��������뵽��ʱ���ѵ����
		if(nCount >= 10)
		{
			CDataPool::GetMe()->GetRelation()->RemoveRelation( RELATION_GROUP( RELATION_GROUP_TEMPFRIEND ), 0 );
		}

		pMember.m_RelationType = RELATION_TYPE_TEMPFRIEND;
		if( CDataPool::GetMe()->GetRelation()->AddRelation( RELATION_GROUP_TEMPFRIEND, &pMember ) )
		{

			// �ؽ���ʷ��Ϣ
			CDataPool::GetMe()->GetRelation()->RebuildMailHistory( RELATION_GROUP_TEMPFRIEND, pMember.m_szName );

			/*STRING strTemp = "";
			CHAR szText[_MAX_PATH];
			strTemp = NOPARAMMSGFUNC("GCRelationHandler_Info_Add_Firend_List");
			_snprintf(szText, _MAX_PATH, strTemp.c_str(), pMember.m_szName);
			
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
			
			ADDTALKMSG(szText);*/
		}

		CEventSystem::GetMe()->PushEvent(GE_UPDATE_FRIEND);

		return 0;
	}
	// ��������
//	else if( args[ 2 ].GetInteger() == RELATION_GROUP_BLACK )
//	{
		/*CGRelation Msg;
		Msg.GetRelation()->CleanUp();
		Msg.GetRelation()->m_Type = REQ_ADDTOBLACKLIST;
		REQUEST_ADD_RELATION* pBlackName = &(Msg.GetRelation()->m_AddRelation);
		pBlackName->CleanUp();

		if( args[ 3 ].IsInteger() ) 
		{
			pBlackName->SetTargetGUID( args[ 3 ].GetInteger() );
		}
		else if( args[ 3 ].IsString() )
		{
			pBlackName->SetTargetName( args[ 3 ].GetString() );
		}
		else
		{
			CObject_Character* pCharObj = (CObject_Character*)CObjectManager::GetMe()->GetMainTarget();
			if( pCharObj == CObjectManager::GetMe()->GetMySelf() ) // ������Լ����Ͳ���
				return 0;
			
			if( g_theKernel.IsKindOf( pCharObj->GetClass(), GETCLASS(CObject_PlayerOther) ) ) // ��������
			{
				ENUM_RELATION sCamp = CGameInterface::GetMe()->GetCampType( 
							pCharObj, 
							(CObject*)CObjectManager::GetMe()->GetMySelf() );
				
				if( sCamp != RELATION_FRIEND ) // �����ͬһ��Ӫ��
					return 0;
			}
			else
			{
				return 0;
			}

			pBlackName->SetTargetName( pCharObj->GetCharacterData()->Get_Name() );
		}

		pBlackName->SetRelationType(RELATION_TYPE_BLACKNAME);
		
		CNetManager::GetMe()->SendPacket( &Msg );*/
		
	//	return 0;
//	}
	// ������
	else if( args[ 2 ].GetInteger() == RELATION_GROUP_ENEMY )
	{
		CGRelation Msg;
		
		Msg.GetRelation()->CleanUp();
		Msg.GetRelation()->m_Type = REQ_ADDTOENEMY;

		REQUEST_ADD_RELATION* pBlackName = &(Msg.GetRelation()->m_AddRelation);
		pBlackName->CleanUp();

		if( args[ 3 ].IsInteger() ) 
		{
			pBlackName->SetTargetGUID( args[ 3 ].GetInteger() );
		}
		else if( args[ 3 ].IsString() )
		{
			pBlackName->SetTargetName( args[ 3 ].GetString() );
		}
		else
		{
			CObject_Character* pCharObj = (CObject_Character*)CObjectManager::GetMe()->GetMainTarget();
			
			// ������Լ����Ͳ���
			if( pCharObj == CObjectManager::GetMe()->GetMySelf() ) 
				return 0;

			// ��������
			if( g_theKernel.IsKindOf( pCharObj->GetClass(), GETCLASS(CObject_PlayerOther) ) ) 
			{
				// �жϹ�ϵ���ж� �� �Ѻã�
				ENUM_RELATION sCamp = CGameInterface::GetMe()->GetCampType( pCharObj, (CObject*)CObjectManager::GetMe()->GetMySelf() );
				
				if( sCamp != RELATION_FRIEND ) // �����ͬһ��Ӫ��
					return 0;
			}
			else
			{
				return 0;
			}

			pBlackName->SetTargetName( pCharObj->GetCharacterData()->Get_Name() );
		}

		pBlackName->SetRelationType( RELATION_TYPE_ENEMY );
		
		CNetManager::GetMe()->SendPacket( &Msg );
	
		return 0;
	}


	if( args[ 3 ].IsInteger() ) 
	{
		CObject_Character* pCharObj = CObjectManager::GetMe()->GetMySelf();
		if( pCharObj->GetCharacterData()->Get_Guild() == args[ 3 ].GetInteger() )
			return 0;
		
		pFriend->SetTargetGUID( args[ 3 ].GetInteger() );
	}
	else if( args[ 3 ].IsString() )
	{
		pFriend->SetTargetName( args[ 3 ].GetString() );
		
		CObject_Character* pCharObj = CObjectManager::GetMe()->GetMySelf() ;
		if( strcmp( pCharObj->GetCharacterData()->Get_Name(), args[ 3 ].GetString() ) == 0 )
			return 0;
	}
	else
	{
		CObject_Character* pCharObj = (CObject_Character*)CObjectManager::GetMe()->GetMainTarget();
		
		if( pCharObj == NULL )
			return 0;
		
		if( pCharObj == CObjectManager::GetMe()->GetMySelf() ) // ������Լ����Ͳ���
			return 0;
		
		if( g_theKernel.IsKindOf( pCharObj->GetClass(), GETCLASS(CObject_PlayerOther) ) ) // ��������
		{
			ENUM_RELATION sCamp = CGameInterface::GetMe()->GetCampType( 
						pCharObj, 
						(CObject*)CObjectManager::GetMe()->GetMySelf() );
			
			if( sCamp != RELATION_FRIEND ) // �����ͬһ��Ӫ��
				return 0;
		}
		else
		{
			return 0;
		}

		pFriend->SetTargetName( pCharObj->GetCharacterData()->Get_Name() );
	}

	pFriend->SetGroup( nGroup );
	pFriend->SetRelationType( RELATION_TYPE_FRIEND );

	CNetManager::GetMe()->SendPacket( &Msg );

	return 0;
}


// ��������ͨ���Ҽ������Ҷ����Ϊ����
INT DataPool::AddFriendByModel( LuaPlus::LuaState* state )
{
	CObject_Character* pCharObj = (CObject_Character*)CObjectManager::GetMe()->GetMainTarget();

	if( pCharObj == NULL )
		return 0;

	if( pCharObj == CObjectManager::GetMe()->GetMySelf() ) // ������Լ����Ͳ���
		return 0;

	if( g_theKernel.IsKindOf( pCharObj->GetClass(), GETCLASS(CObject_PlayerOther) ) ) // ��������
	{
		ENUM_RELATION sCamp = CGameInterface::GetMe()->GetCampType( 
			pCharObj, 
			(CObject*)CObjectManager::GetMe()->GetMySelf() );

		if( sCamp != RELATION_FRIEND ) // �����ͬһ��Ӫ��
			return 0;
	}
	else
	{
		return 0;
	}

	CGRelation Msg;
	Msg.GetRelation()->CleanUp();

	//	��ⱻ�ӵĺ����Ƿ��ں�������
	const SDATA_RELATION_MEMBER *pInfo = NULL;
	int nCount = CDataPool::GetMe()->GetRelation()->GetListCount( RELATION_GROUP_BLACK );
	for (int i=0; i<nCount; i++)
	{
		pInfo = CDataPool::GetMe()->GetRelation()->GetRelationInfo( RELATION_GROUP_BLACK, i );

		// ������ӵ����ں������о�ͨ���ƶ��ķ�ʽ�ѱ����˷ŵ�������
		if ( 0 == strcmp(pInfo->m_szName,pCharObj->GetCharacterData()->Get_Name()) )
		{
			Msg.GetRelation()->m_Type = REQ_TRANSITION;
			RELATION_GUID_UCHAR_UCHAR* pFriend = &(Msg.GetRelation()->m_RelationGUIDUCHARUCHAR);
			if( pFriend == NULL ) return 0;

			pFriend->CleanUp();
			pFriend->SetTargetGUID( pInfo->m_GUID );//��Ȼ���Լ��ĺ������У���һ��֪������˵�GUID������

			pFriend->SetRelationType( RELATION_TYPE_FRIEND );
			pFriend->SetGroup( RELATION_GROUP_FRIEND_ALL );

			CNetManager::GetMe()->SendPacket( &Msg );

			return 0;
		}
	}

	Msg.GetRelation()->m_Type = REQ_ADDFRIEND;
	REQUEST_ADD_RELATION_WITH_GROUP* pFriend = &(Msg.GetRelation()->m_AddRelationWithGroup);
	pFriend->CleanUp();

	pFriend->SetTargetName( pCharObj->GetCharacterData()->Get_Name() );
	pFriend->SetGroup( RELATION_GROUP_FRIEND_ALL );
	pFriend->SetRelationType( RELATION_TYPE_FRIEND );

	CNetManager::GetMe()->SendPacket( &Msg );

	return 0;
}



// ȷ�����������ҵ������б�
INT DataPool::SureAddFriend( LuaPlus::LuaState* state )
{
	int nGroup = 0;

	CGRelation Msg;
	Msg.GetRelation()->CleanUp();
	Msg.GetRelation()->m_Type = REQ_SUREADDFRIEND;
	REQUEST_ADD_RELATION_WITH_GROUP* pFriend = &(Msg.GetRelation()->m_AddRelationWithGroup);
	pFriend->CleanUp();
	pFriend->SetTargetName( CDataPool::GetMe()->Relation_GetInviteName() );
	pFriend->SetGroup( nGroup );
	pFriend->SetRelationType( RELATION_TYPE_FRIEND );
	
	CNetManager::GetMe()->SendPacket( &Msg );

	return 0;
}

// �ܾ���Ҽ�Ϊ��������
INT DataPool::RejectAddFriend( LuaPlus::LuaState* state )
{
	CGRelation Msg;
	Msg.GetRelation()->CleanUp();
	Msg.GetRelation()->m_Type = REQ_REJECTADDFRIEND;
	REQUEST_ADD_RELATION_WITH_GROUP* pFriend = &(Msg.GetRelation()->m_AddRelationWithGroup);
	pFriend->CleanUp();
	pFriend->SetTargetName( CDataPool::GetMe()->Relation_GetInviteName() );
	pFriend->SetGroup( 0 );
	pFriend->SetRelationType( RELATION_TYPE_FRIEND );

	CNetManager::GetMe()->SendPacket( &Msg );

	return 0;
}

//	�ж�����Ƿ����Լ������ϵ��
INT DataPool::IsTempFriendByName( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[2].IsString() ) return 0;
	
	const SDATA_RELATION_MEMBER *pInfo = NULL;
	int nCount = CDataPool::GetMe()->GetRelation()->GetListCount( RELATION_GROUP_TEMPFRIEND );
	for (int i=0; i<nCount; i++)
	{
		pInfo = CDataPool::GetMe()->GetRelation()->GetRelationInfo( RELATION_GROUP_TEMPFRIEND, i );
		if ( 0 == strcmp(pInfo->m_szName,args[2].GetString()) )
		{
			state->PushInteger( 1 );

			return 1;
		}
	}
	
	return 0;
}


// �ж�����Ƿ����Լ��ĺ������б���
INT DataPool::IsInBlackListByName( LuaPlus::LuaState* state )
{
	LuaStack args(state);

	if ( !args[2].IsString() )
	{
		KLThrow("LUA: DataPool::IsInBlackListByName Wrong Param1");
		return 0;
	}
	const SDATA_RELATION_MEMBER *pInfo = NULL;
	int nCount = CDataPool::GetMe()->GetRelation()->GetListCount( RELATION_GROUP_BLACK );
	for (int i=0; i<nCount; i++)
	{
		pInfo = CDataPool::GetMe()->GetRelation()->GetRelationInfo( RELATION_GROUP_BLACK, i );
		if ( 0 == strcmp(pInfo->m_szName,args[2].GetString()) )
		{
			state->PushInteger( 1 );
			return 1;
		}
	}

	return 0;
}


// ͨ��GUID�ж�����Ƿ����Լ��ĺ������б���
INT DataPool::IsInBlackListByGuid( LuaPlus::LuaState* state )
{
	LuaStack args(state);

	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA: DataPool::IsInBlackListByGuid Wrong Param1");
		return 0;
	}
	const SDATA_RELATION_MEMBER *pInfo = NULL;
	int nCount = CDataPool::GetMe()->GetRelation()->GetListCount( RELATION_GROUP_BLACK );
	for (int i=0; i<nCount; i++)
	{
		pInfo = CDataPool::GetMe()->GetRelation()->GetRelationInfo( RELATION_GROUP_BLACK, i );
		if ( pInfo->m_GUID == args[2].GetInteger() )
		{
			state->PushInteger( 1 );
			return 1;
		}
	}

	return 0;
}

// �ж�����Ƿ����Լ��ĳ����б���
INT DataPool::IsInEnemyListByName( LuaPlus::LuaState* state )
{
	LuaStack args(state);

	if ( !args[2].IsString() )
	{
		KLThrow("LUA: DataPool::IsInEnemyListByName Wrong Param1");
		return 0;
	}
	const SDATA_RELATION_MEMBER *pInfo = NULL;
	int nCount = CDataPool::GetMe()->GetRelation()->GetListCount( RELATION_GROUP_ENEMY );
	for (int i=0; i<nCount; i++)
	{
		pInfo = CDataPool::GetMe()->GetRelation()->GetRelationInfo( RELATION_GROUP_ENEMY, i );
		if ( 0 == strcmp(pInfo->m_szName,args[2].GetString()) )
		{
			state->PushInteger( 1 );
			return 1;
		}
	}

	return 0;
}

// ��ϵͳ���ɾ����������
INT DataPool::AskDelFriend( LuaPlus::LuaState* state )
{
	LuaStack args( state );
	
	if(!(args[ 2 ].IsInteger()))
	{
		KLThrow("LUA: Friend::AskDelFriend Wrong Param1");
		return 0;
	}
	if(!(args[ 3 ].IsInteger()))
	{
		KLThrow("LUA: Friend::AskDelFriend Wrong Param2");
		return 0;
	}

	// �������ʱ����,ֱ�Ӵ���
	if ( args[2].GetInteger() == RELATION_GROUP_TEMPFRIEND )
	{
		CDataPool::GetMe()->GetRelation()->RemoveRelation( RELATION_GROUP( args[ 2 ].GetInteger() ), args[ 3 ].GetInteger() );
		CGameProcedure::s_pEventSystem->PushEvent( GE_UPDATE_FRIEND );
		return 0;
	}
	// ��������������ֱ���������ȷ��
	else
	{
		CEventSystem::GetMe()->PushEvent( GE_FRIEND_DELETE_FRIEND, args[ 2 ].GetInteger(), args[ 3 ].GetInteger() );
	}
	return 0;
}


 // ɾ��һ������
// int: 1 ����Ҫɾ���ĺ��ѵ�ID
INT DataPool::DelFriend( LuaPlus::LuaState* state )
{
	LuaStack args( state );

	if(!(args[ 2 ].IsInteger()))
	{
		KLThrow("LUA: Friend::DelFriend Wrong Param1");
		return 0;
	}
	if(!(args[ 3 ].IsInteger()))
	{
		KLThrow("LUA: Friend::DelFriend Wrong Param2");
		return 0;
	}


	const SDATA_RELATION_MEMBER *pInfo = 
		CDataPool::GetMe()->GetRelation()->GetRelationInfo( RELATION_GROUP( args[ 2 ].GetInteger() ), args[ 3 ].GetInteger() );

	if ( pInfo == NULL)
	{
		return 0;
	}

	// ɾ��������
	if( args[ 2 ].GetInteger() == RELATION_GROUP_BLACK )
	{
		/*CGRelation Msg;

		Msg.GetRelation()->CleanUp();
		Msg.GetRelation()->m_Type = REQ_DELFROMBLACKLIST;	

		RELATION_GUID* pBlackName = &(Msg.GetRelation()->m_RelationGUID);
		pBlackName->CleanUp();
		pBlackName->SetTargetGUID( pInfo->m_GUID );
		
		CNetManager::GetMe()->SendPacket( &Msg );*/
	}
	// ɾ������
	else if( args[ 2 ].GetInteger() == RELATION_GROUP_ENEMY )
	{
		CGRelation Msg;

		Msg.GetRelation()->CleanUp();
		Msg.GetRelation()->m_Type = REQ_DELFROMENEMY;

		RELATION_GUID* pEnemyName = &(Msg.GetRelation()->m_RelationGUID);

		pEnemyName->CleanUp();
		pEnemyName->SetTargetGUID( pInfo->m_GUID );

		CNetManager::GetMe()->SendPacket( &Msg );
	}
	// ɾ������
	else
	{
		CGRelation Msg;
		
		Msg.GetRelation()->CleanUp();
		Msg.GetRelation()->m_Type = REQ_DELFRIEND;

		REQUEST_DEL_FRIEND* pFriend = &(Msg.GetRelation()->m_DelFriend);
		pFriend->CleanUp();
		pFriend->SetGUID( pInfo->m_GUID );
		pFriend->SetName( pInfo->m_szName );

		CNetManager::GetMe()->SendPacket( &Msg );
	}

	return 0;
}

INT DataPool::GetFriendByName( LuaPlus::LuaState* state )
{
	LuaStack args( state );
	if( !args[2].IsString() ) 
		return 0;

	int nGroup = -1, nIndex = -1;
	CDataPool::GetMe()->GetRelation()->GetRelationByName( args[2].GetString(), nGroup , nIndex );
	
	state->PushInteger( nGroup );
	state->PushInteger( nIndex );
	
	return 2;
}

INT DataPool::LuaShowFriendInfo( LuaPlus::LuaState* state )
{
	LuaStack args( state );

	if( !( args[2].IsInteger() ) )
	{
		KLThrow("LUA: LuaShowFriendInfo Wrong Param1");
	}

	INT nIndex = args[2].GetInteger();

	CGameProcedure::s_pEventSystem->PushEvent( GE_SHOW_FRIEND_INFO, nIndex );

	return 0;
}

INT DataPool::ShowFriendInfo( LuaPlus::LuaState* state )
{
	LuaStack args( state );
	int nGroup,nIndex;
	if( args[2].IsInteger() ) // ������ַ���
	{
		nGroup = args[2].GetInteger();
		nIndex = args[ 3 ].GetInteger();
	}
	else if( args[2].IsString() )
	{
		std::string  str = args[ 2 ].GetString();
		CDataPool::GetMe()->GetRelation()->GetRelationByName( str.c_str(), nGroup , nIndex );
	}
	else
		return 0;
	
	CGRelation Msg;
	Msg.GetRelation()->CleanUp();
	Msg.GetRelation()->m_Type = REQ_RELATIONINFO;
	REQUEST_RELATION_INFO* pFriend = &(Msg.GetRelation()->m_RequestInfo);
	pFriend->CleanUp();
	const SDATA_RELATION_MEMBER* pInfo = CDataPool::GetMe()->GetRelation()->GetRelationInfo( ( RELATION_GROUP )nGroup, nIndex );

	if ( pInfo->m_GUID != INVALID_ID )
	{ // �� GUID ���� GUID��fancy
		pFriend->SetTargetGUID( pInfo->m_GUID );
	}
	else
	{ // ������ֻ�����������ߺ��ѵ���Ϣ��fancy
		pFriend->SetTargetName( pInfo->m_szName );
	}

	CNetManager::GetMe()->SendPacket( &Msg );
	
	CGameProcedure::s_pEventSystem->PushEvent( GE_TOGLE_FRIEND_INFO, nGroup, nIndex );
	return 0;
}


INT	DataPool::ShowChatInfo(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if(!(args[2].IsString()))
	{
		KLThrow("LUA: DataPool ShowChatInfo Wrong Param1");
		return -1;
	}

	STRING strUser = args[2].GetString();
	if(strUser.empty()) return 0;

	CGRelation pk;
	//0.clear
	pk.GetRelation()->CleanUp();
	REQUEST_RELATION_INFO* pInfo = &(pk.GetRelation()->m_RequestInfo);
	pInfo->CleanUp();
	//1.set pk
	pk.GetRelation()->m_Type = REQ_RELATIONINFO;
	pInfo->SetTargetName(strUser.c_str());
	//2.send pk
	CNetManager::GetMe()->SendPacket(&pk);

	return 0;
}

INT	DataPool::GetMailNumber( LuaPlus::LuaState* state )
{
	state->PushInteger( CDataPool::GetMe()->GetMailPool()->GetMailLeftCount() );

	return 1;
}

INT DataPool::OpenMailEdit(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	CEventSystem::GetMe()->PushEvent( GE_OPEN_EMAIL_WRITE, args[ 2 ].GetString() );
	
	return 0;
}
INT DataPool::OpenMailBrowser(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent( GE_OPEN_EMAIL );

	return 0;
}

INT DataPool::OpenFriendList(LuaPlus::LuaState* state)
{
//	CEventSystem::GetMe()->PushEvent( GE_TOGLE_FRIEND, "2" );
	CEventSystem::GetMe()->PushEvent( GE_UNITE_GF_SHOW, 0 );

	return 0;
}
INT	DataPool::GetNextMail(LuaPlus::LuaState* state )
{
	CGAskMail mail;

	mail.SetAskType( ASK_TYPE_MAIL );
	CNetManager::GetMe()->SendPacket( &mail );

	//CEventSystem::GetMe()->PushEvent( GE_HAVE_MAIL );
	
	return 0;
}
INT DataPool::SendMail( LuaPlus::LuaState* state )
{
	LuaStack args(state);

	MAIL  mail;
	
	mail.m_GUID = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();
	time( &(mail.m_uCreateTime) );
	_snprintf( mail.m_szDestName, 30, "%s", args[ 2 ].GetString() ); 
	_snprintf( mail.m_szContex, 256, "%s", args[ 3 ].GetString() );
	_snprintf( mail.m_szSourName, 30, "%s", CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name() );

	MailAnalyze analyze;
	STRING strContexpIn = mail.m_szContex;
	STRING strContexpOut = analyze.ReplaceItemElementInfo1( strContexpIn );
	const CHAR* contex = strContexpOut.c_str();
	strcpy( mail.m_szContex, contex );
	
	// ����Ǹ��Լ����ʼ� ��Ҳ����
	if( strcmp( mail.m_szDestName,mail.m_szSourName ) == 0 )
		return 0;
	
	if( strlen( mail.m_szContex ) == 1 )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("NULL_MAIL");
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return 0;
	}

	if(!CGameProcedure::s_pUISystem->CheckStringFilter(mail.m_szContex, FT_CHAT))
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("INVALID_STRING_IN_NAME");
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return 0;
	}

	////��֤�̵����ƵĺϷ���
	//if(!KLU_CheckStringValid(mail.m_szContex))
	//{
	//	STRING strTemp = "";
	//	strTemp = NOCOLORMSGFUNC("INVALID_STRING_IN_NAME");
	//	CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
	//	return 0;
	//}

	// ��������˲����Լ��ĺ��ѣ��������ʱ��������
	INT nGroup, nIndex;
	Relation* pRelation = CDataPool::GetMe()->GetRelation();

	RELATION_TYPE RelationType = pRelation->GetRelationByName( args[ 2 ].GetString(), nGroup , nIndex );
	if( RelationType == RELATION_TYPE_BLACKNAME )
	{ // ����Ǻ�����
		return 0;
	}
	else if( RelationType == RELATION_TYPE_STRANGER )
	{ 
		// ����Ҫ�������Ϊ��ʱ���ѣ���Ϊ�ڽű������Ѿ������


		//// ��ʱ�����Ƿ���ʱ����
		//SDATA_RELATION_MEMBER Member;
		//strncpy( Member.m_szName, args[ 2 ].GetString(), sizeof(Member.m_szName)-1 );
		//Member.m_RelationType = RELATION_TYPE_TEMPFRIEND;
		//if( pRelation->AddRelation(RELATION_GROUP_TEMPFRIEND, &Member) )
		//{ 
		//	// �ؽ���ʷ��Ϣ
		//	pRelation->RebuildMailHistory( RELATION_GROUP_TEMPFRIEND, args[ 2 ].GetString() );
		//}
	}

	//��֤�̵����ƵĺϷ���

	mail.m_nPortrait = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_PortraitID();
	mail.m_ContexSize = BYTE( strlen( mail.m_szContex ) );
	mail.m_SourSize = BYTE( strlen( mail.m_szSourName ) );
	mail.m_DestSize = BYTE( strlen( mail.m_szDestName ) );
	mail.m_uFlag	= 0;

	CGMail Msg;
	Msg.SetMail( &mail );
	CNetManager::GetMe()->SendPacket( &Msg );

	nIndex = CDataPool::GetMe()->GetMailPool()->AddMail( &mail );

	if( nIndex >= 0 )
	{
		CDataPool::GetMe()->GetRelation()->AddMail( mail.m_szDestName, nIndex );
	}

	return 0;
}
INT DataPool::GetMail( LuaPlus::LuaState* state )
{
	LuaStack args( state );

	if( !args[ 2 ].IsInteger() ) 
		return 0;
	if( !args[ 3 ].IsString() ) 
		return 0;
	const SMail* pMail = CDataPool::GetMe()->GetMailPool()->GetMail( args[ 2 ].GetInteger() );
	std::string  str = args[ 3 ].GetString();
	if( str == "CONTEX" )// GUID
	{
		state->PushString( pMail->m_szContex );
	}
	else if( str == "TIME" )
	{
		state->PushString( pMail->m_szCreateTime );
	}
	else if( str == "SENDER" )
	{
		state->PushString( pMail->m_szSourName );
	}
	else if( str == "RECIVER" )
	{
		state->PushString( pMail->m_szDestName );
	}
	else if( str == "PORTRAIT" )
	{
		DBC_DEFINEHANDLE(s_pFaceImageDBC, DBC_CHARACTER_FACE)	// char_face.tab
		const _DBC_CHAR_FACE* pFaceImage = NULL;
		pFaceImage = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_Index_EQU( pMail->m_nPortrait ) );

		if( pFaceImage )
			state->PushString( pFaceImage->pImageSetName );
		else
			state->PushString( "" );
	}
	else
		state->PushString( "-1" );
	return 1;
}
// ���͸���������˵������ı��ˡ�
INT DataPool::SetMood( LuaPlus::LuaState* state )
{
	LuaStack args( state );
	if( !args[ 2 ].IsString() )
	{
		KLThrow("LUA:SetMood Param1 error");
		return 0;
	}

	if( !CGameProcedure::s_pUISystem->CheckStringFilter(args[ 2 ].GetString(), FT_CHAT) )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("INVALID_STRING_IN_NAME");
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return 0;
	}
	//��֤�̵����ƵĺϷ���
	if(!KLU_CheckStringValid(args[ 2 ].GetString()))
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return 0;
	}


	CGRelation Msg;
	
	Msg.GetRelation()->CleanUp();
	Msg.GetRelation()->m_Type = REQ_MODIFYMOOD;
	REQUEST_MODIFY_MOOD* pFriend = &(Msg.GetRelation()->m_ModifyMood);
	pFriend->CleanUp();
	
	//��֤�̵����ƵĺϷ���
	
	pFriend->SetMood( args[ 2 ].GetString() );
	
	CNetManager::GetMe()->SendPacket( &Msg );

	return 0;
}

// �������
INT DataPool::GetMood( LuaPlus::LuaState* state )
{
	state->PushString( CDataPool::GetMe()->GetRelation()->GetMood() );
	//CEventSystem::GetMe()->PushEvent( GE_MOOD_SET, args[ 2 ].GetString() );

	return 1;
}

// �޸�����
INT DataPool::FriendSeting( LuaPlus::LuaState* state )
{
	CEventSystem::GetMe()->PushEvent( GE_FRIEND_SET );
	return 0;
}


INT DataPool::OpenHistroy( LuaPlus::LuaState* state )
{
	LuaStack args( state );
	
	if( !args[ 2 ].IsInteger() ) 
	{
		KLThrow("LUA:OpenHistroy Param1 error");
		return 0;
	}

	if( !args[ 3 ].IsInteger() ) 
	{
		KLThrow("LUA:OpenHistroy Param2 error");
		return 0;
	}

	CEventSystem::GetMe()->PushEvent( GE_OPEN_HISTROY, args[ 2 ].GetInteger(), args[ 3 ].GetInteger() );
	
	return 0;
}

INT DataPool::AddChatHistoryMessage(LuaPlus::LuaState* state )
{
	//INT msgID,LPCTSTR szWinName, LPCTSTR szMessage, INT nOp = -1, UINT nDisappearTime = 0
	LuaStack args( state );

	if( !args[ 2 ].IsInteger() ) 
	{
		KLThrow("LUA:AddChatHistoryMessage Param1 error");
		return 0;
	}
	if( !args[ 3 ].IsString() ) 
	{
		KLThrow("LUA:AddChatHistoryMessage Param2 error");
		return 0;
	}
	if( !args[ 4 ].IsString() ) 
	{
		KLThrow("LUA:AddChatHistoryMessage Param3 error");
		return 0;
	}
	INT msgID = args[2].GetInteger();
	LPCTSTR szWinName = args[3].GetString();
	LPCTSTR szMessage = args[4].GetString();
	INT nOp = -1;
	UINT nDisappearTime = 0;
	if( args[ 5].IsInteger() ) 
	{
		nOp = args[5].GetInteger();	
	}
	if( !args[ 6 ].IsInteger() ) 
	{
		nDisappearTime = args[6].GetInteger();
	}
	
	//�����Ϣ������
	if(CGameProcedure::s_pUISystem)
	{
		CGameProcedure::s_pUISystem->addChatHistoryMessage( msgID, szWinName,szMessage, nOp, nDisappearTime );
	}

	return 0;
}

INT DataPool::Change_MyHairColor(LuaPlus::LuaState* state)
{

	LuaStack args(state);

	if (!args[2].IsInteger()	||
		(!args[3].IsInteger())	||
		(!args[4].IsInteger())	||
		(!args[5].IsInteger()) )
	{
		KLThrow("LUA:Change_MyHairColor Param error");
	}

	INT nRed	= args[2].GetInteger();
	INT nGreen  = args[3].GetInteger();
	INT nBlue	= args[4].GetInteger();
	INT nAlpha  = args[5].GetInteger();

	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_HairColor( (nRed<<24) | (nGreen <<16) | (nBlue<<8) | (nAlpha));

	UINT Original_Color = MAKE_COLOR(nRed,nGreen,nBlue,nAlpha);

	INT	Lumination = CGameProcedure::s_pUISystem->OgreColor_Lumination(Original_Color);

	state->PushInteger(Lumination);
	return 1;
}

INT DataPool::Change_RectifyColor(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!args[2].IsInteger()	||
		(!args[3].IsInteger())	||
		(!args[4].IsInteger())	||
		(!args[5].IsInteger()) )
	{
		KLThrow("LUA:Change_MyHairColor Param error");
	}	

	INT nRed	= args[2].GetInteger();
	INT nGreen  = args[3].GetInteger();
	INT nBlue	= args[4].GetInteger();
	INT nAlpha  = args[5].GetInteger();
	UINT Original_Color = MAKE_COLOR(nRed,nGreen,nBlue,nAlpha);

	INT	Lumination = CGameProcedure::s_pUISystem->OgreColor_Lumination(Original_Color);

	if(Lumination > 126)
	{
		UINT Ogre_Color = CGameProcedure::s_pUISystem->Lumination_OgreColor(Original_Color,127);
		state->PushInteger(COLOR_R(Ogre_Color));
		state->PushInteger(COLOR_G(Ogre_Color));
		state->PushInteger(COLOR_B(Ogre_Color));
		state->PushInteger(COLOR_A(Ogre_Color));
		state->PushInteger(Ogre_Color);
	}
	else
	{
		state->PushInteger(INVALID_ID);
		state->PushInteger(INVALID_ID);
		state->PushInteger(INVALID_ID);
		state->PushInteger(INVALID_ID);
		state->PushInteger(Original_Color);
	}
	return 5;

	//UINT Change_Color = CGameProcedure::s_pUISystem->OgreColor_CEGUIColor_Limit_Lumination(Original_Color,126);

	//if(Change_Color != Original_Color)
	//{
	//	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_HairColor( Original_Color );
	//	state->PushBoolean(false);
	//}
	//else
	//{
	//	state->PushBoolean(true);
	//}
	//return 1;

}

INT DataPool::Change_GetColorLumination(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!args[2].IsInteger()	||
		(!args[3].IsInteger())	||
		(!args[4].IsInteger())	||
		(!args[5].IsInteger()) )
	{
		KLThrow("LUA:Change_MyHairColor Param error");
	}	

	INT nRed	= args[2].GetInteger();
	INT nGreen  = args[3].GetInteger();
	INT nBlue	= args[4].GetInteger();
	INT nAlpha  = args[5].GetInteger();
	INT	nLumination = args[6].GetInteger();

	UINT Original_Color = MAKE_COLOR(nRed,nGreen,nBlue,nAlpha);

	UINT Ogre_Color = CGameProcedure::s_pUISystem->Lumination_OgreColor(Original_Color,nLumination);
	state->PushInteger(COLOR_R(Ogre_Color));
	state->PushInteger(COLOR_G(Ogre_Color));
	state->PushInteger(COLOR_B(Ogre_Color));
	state->PushInteger(COLOR_A(Ogre_Color));


	return 4;

	//UINT Change_Color = CGameProcedure::s_pUISystem->OgreColor_CEGUIColor_Limit_Lumination(Original_Color,126);

	//if(Change_Color != Original_Color)
	//{
	//	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_HairColor( Original_Color );
	//	state->PushBoolean(false);
	//}
	//else
	//{
	//	state->PushBoolean(true);
	//}
	//return 1;

}

INT DataPool::Change_MyHairStyle(LuaPlus::LuaState* state)
{

	LuaStack args(state);

	if (!args[2].IsInteger() )
	{
		KLThrow("LUA:Change_MyHairStyle Param error");
	}	

	INT nStyle = args[2].GetInteger();

	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_HairMesh(nStyle);

	return 0;
}

INT DataPool::Change_MyHairStyle_Item(LuaPlus::LuaState* state)
{

	LuaStack args(state);

	if (!args[2].IsInteger() )
		
	{
		KLThrow("LUA:Change_MyHairStyle Param error");
	}

	INT nIndex = args[2].GetInteger();

	//�򿪷������ı�
	DBC_DEFINEHANDLE(s_pHair_Style, DBC_HAIR_STYLE);
	const _DBC_HAIR_STYLE* pHair_Style = (const _DBC_HAIR_STYLE*)s_pHair_Style->Search_Index_EQU(nIndex);
	INT nRaceID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_RaceID();

	if(!pHair_Style || nRaceID != pHair_Style->nRaceID)
	{
		state->PushInteger(INVALID_ID);
		state->PushInteger(INVALID_ID);
		return 2;
	}
	
	state->PushInteger(pHair_Style->nItemTableID);
	state->PushInteger(pHair_Style->nItemCount);

	return 2;
}

INT DataPool::Get_MyHairColor(LuaPlus::LuaState* state)
{

	LuaStack args(state);

	INT nColor=CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_HairColor();

	INT nRed = (nColor >> 24) & 0xFF; 
	INT nGreen = (nColor >> 16) & 0xFF; 
	INT nBlue = (nColor >> 8) & 0xFF; 
	INT nAlpha = (nColor) & 0xFF; 

	state->PushInteger(nRed);
	state->PushInteger(nGreen);
	state->PushInteger(nBlue);
	state->PushInteger(nAlpha);

	return 4;
}

INT DataPool::Get_MyHairStyle(LuaPlus::LuaState* state)
{

	LuaStack args(state);

	INT nStyle = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_HairMesh();
	state->PushInteger(nStyle);

	return 1;
}

INT	DataPool::IsCanDoAction( LuaPlus::LuaState* state )
{
		//�������̵꿪�ţ�
	if( CGameProcedure::s_pUISystem && 
		(	CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag") ||
			CGameProcedure::s_pUISystem->IsWindowShow("StallSale")  ||
			CGameProcedure::s_pUISystem->IsWindowShow("MissionReply") ||
			CGameProcedure::s_pUISystem->IsWindowShow("Bank") ||
			CGameProcedure::s_pUISystem->IsWindowShow("Exchange") ||
			CGameProcedure::s_pUISystem->IsWindowShow("Shop") 
		)
	  )
	{
		state->PushBoolean(false);	
	}
	else
	{
		state->PushBoolean(true);
	}
	return 1;
}

INT DataPool::Check_StringCode( LuaPlus::LuaState* state )
{
	LuaStack args(state);

	if (!args[2].IsString() )
	{
		KLThrow("LUA:Check_StringCode Param error");
	}

	LPCTSTR szMsg = args[2].GetString();
	STRING szTemp = "";

	state->PushInteger(CGameProcedure::s_pUISystem->CheckStringCode(szMsg,szTemp));

	return 1;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main target ������ʾ������Ϣ�� 
// ��ʾ����
INT DataPool::TargetFrame_Update_Name_Team(LuaPlus::LuaState* state)
{
	LPCSTR pName = CUIDataPool::GetMe()->GetTeamNameByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
	
	state->PushString(pName);
	return 1;
}

// ��ʾhp
INT DataPool::TargetFrame_Update_HP_Team(LuaPlus::LuaState* state)
{
	int iHP = CUIDataPool::GetMe()->GetTeamHPByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
	state->PushNumber( iHP );
	return 1;
}

// ��ʾmp
INT	DataPool::TargetFrame_Update_MP_Team(LuaPlus::LuaState* state)
{
	int iMP = CUIDataPool::GetMe()->GetTeamMPByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
	state->PushNumber( iMP );
	return 1;
}

// ��ʾŭ��
INT DataPool::TargetFrame_Update_Rage_Team(LuaPlus::LuaState* state)
{
	int iRange = CUIDataPool::GetMe()->GetTeamRangeByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
	state->PushNumber( iRange );
	return 1;
}

// ��ʾ�ȼ�
INT DataPool::TargetFrame_Update_Level_Team(LuaPlus::LuaState* state)
{
	int iLevel = CUIDataPool::GetMe()->GetTeamLevelByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
	state->PushNumber( iLevel );
	return 1;
}

// ��ʾͷ�����֡�
INT DataPool::TargetFrame_Update_Icon_Team(LuaPlus::LuaState* state)
{
	LPCSTR strIcon = CUIDataPool::GetMe()->GetTeamIconByGUID( CObjectManager::GetMe()->GetCurSelTeam() );
	state->PushString( strIcon );
	return 1;
}

//��ʱʵ�ֺ������������й���
INT DataPool::TargetFrame_Update_BuffNum_Team(LuaPlus::LuaState* state)
{
	return 0;
}
int DataPool::TargetFrame_Update_BuffInfo_Team(LuaPlus::LuaState* state)
{
	return 0;
}

/*
// �Ͽ����ӵĴ���
INT DataPool::NetClose(LuaPlus::LuaState* state)
{
	PostMessage(g_hMainWnd, WM_CLOSE, 0, 0);
	return 0;
}
*/
INT DataPool::ToggleShowPetList(LuaPlus::LuaState* state)
{
	
	CEventSystem::GetMe()->PushEvent(GE_TOGGLE_PETLIST);
	return 0;
}
INT DataPool::GetNPCIDByServerID(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!args[2].IsInteger() )
	{
		KLThrow("LUA:GetNPCIDByServerID Param error");
	}

	INT nServerID = args[2].GetInteger();
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(nServerID);
	if(pObj)
	{
		state->PushInteger(pObj->GetID());
	}
	else
	{
		state->PushInteger(INVALID_ID);
	}
	return 1;
}

INT DataPool::GetUIColor(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!args[2].IsInteger() )
	{
		KLThrow("LUA:GetUIColor Param error");
	}

	INT nIndex = args[2].GetInteger();

	DBC_DEFINEHANDLE(s_pSystemColorDBC, DBC_SYSTEM_COLOR);
	const _DBC_SYSTEM_COLOR* pNameColor = 0;

	pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(nIndex);

	if(pNameColor)
	{
		state->PushString(pNameColor->strFontColor);
		state->PushString(pNameColor->strExtenColor);
	}
	else
	{
		state->PushString("-1");
		state->PushString("-1");
	}
	return 2;
}


//================
// ����ϵͳ��ʾ


//INT m_SysTipCurrIndex;

// ����¼�
INT DataPool::SystemTip_AddEventInfo( LuaPlus::LuaState* state )
{
	LuaStack args(state);

	if( !args[2].IsInteger() )
	{
		KLThrow("SystemTip_AddEventInfo Param1 error");
	}
	if( !args[3].IsInteger() )
	{
		KLThrow("SystemTip_AddEventInfo Param2 error");
	}

//	SYSTIP_TYPE	stType	= (SYSTIP_TYPE)( args[2].GetInteger() );		// ���ͣ� ��11��
//	INT nIndex	= args[3].GetInteger();		// ĳһ�����е�����

/*
	// ���ͺϷ�
	if( stType < 0 || stType >= STT_NUM )
		return 0;

	// ϵͳ�¼�
	SystemTipEvent ste;


	// �����������ж��Ƿ��Ѿ������
	MAP_SYSTIP::iterator it;
	it = m_mapSysTip.find( stType );

	// �ҵ��ˣ� �Ѿ������ˣ�
	if( it != m_mapSysTip.end( ) )
	{
		it->second.push_back( ste );	// VECTOR_SYSTIP
	}
	// �����ڴ���һ���µ�����
	else
	{
		std::vector< SystemTipEvent > vSysTip;
		vSysTip.push_back( ste );

//		m_mapSysTip[ stType ] = vSysTip;
		m_mapSysTip.insert( std::make_pair( stType, vSysTip ) );
	}


	// ��������
	m_SysTipCurrIndex = m_mapSysTip.size();

	// ����һ�������ϵ�����
//	state->PushInteger( uiIndex );
*/

	return 0;
}
// ɾ���¼�
INT DataPool::SystemTip_DelEventInfo( LuaPlus::LuaState* state )
{
	LuaStack args(state);

	if( !args[2].IsInteger() )
	{
		KLThrow("SystemTip_AddEventInfo Param1 error");
	}

	// �����ϵ������� ָʾҪ����ĸ���ʾ
	INT nIndex	= args[2].GetInteger();

/*
	// �����Ϸ�
	if( nIndex < 0 || nIndex >= STT_NUM )
		return 0;


	MAP_SYSTIP::iterator itNext;

	for( MAP_SYSTIP::iterator it = m_mapSysTip.begin(); it != m_mapSysTip.end(); it = itNext )
	{
		itNext = it;
		itNext++;

		m_mapSysTip.erase( it );
	}


	MAP_SYSTIP::iterator it = m_mapSysTip.find( (SYSTIP_TYPE)nIndex );
	if( it != m_mapSysTip.end() )
	{
		m_mapSysTip.erase( it );
	}
*/

	return 0;
}


// ��ȡĳһ�¼�����Ϣ
INT DataPool::SystemTip_GetEventInfo( LuaPlus::LuaState* state )
{
	LuaStack args(state);

	if( !args[2].IsInteger() )
	{
		KLThrow("SystemTip_GetEventInfo Param1 error");
	}
	if( !args[3].IsString() )
	{
		KLThrow("SystemTip_GetEventInfo Param2 error");
	}

	INT		nIndex	= args[2].GetInteger();
	STRING	szFilter= args[3].GetString();


	SystemTipEvent*	ste = CDataPool::GetMe()->SystemTip_GetEventInfo( nIndex );
	if( ste )
	{
		if( szFilter == "type" )
		{
			state->PushInteger( (INT)(ste->m_eType) );
		}
		else if( szFilter == "desc" )
		{
			state->PushString( ste->m_strTipDesc.c_str() );
		}
		else if( szFilter == "param" )
		{
			state->PushInteger( ste->m_nParameter );
		}
	}

	return 1;
}

// ����ĳһ�¼���״̬
INT DataPool::SystemTip_SetEventState( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[2].IsInteger() )
	{
		KLThrow("SystemTip_SetEventState Param1 error");
	}

	INT nIndex	= args[2].GetInteger();

	CDataPool::GetMe()->SystemTip_DelEventInfo( nIndex );

	return 0;
}

INT DataPool::SystemTip_GetEventNum( LuaPlus::LuaState* state )
{
	INT num = CDataPool::GetMe()->SystemTip_GetUIIndex();
	state->PushInteger( num );

	return 1;
}

/**���¼���window�Ľű�
*/
INT DataPool::Lua_ReloadWindowScript(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//������
	if (!(args[2].IsString()))
	{
		KLThrow("LUA: Macro::Lua_ReloadWindowScript Wrong Param1");
	}

	STRING window_name = args[2].GetString();
	CGameProcedure::s_pUISystem->ReloadWindowScript(window_name);
	return 0;
}

STRING MailAnalyze::ReplaceItemElementInfo1(const STRING &strSrc)
{
	const CHAR KeyStart		= '#';

	const STRING MsgBegin	= "#{_INFOMSG";
	const CHAR ContentsEnd = '}';

	/*
	1.ת��ǰ:	�뿴���#{_INFOID123}�������
	2.����CTransferItemSystem��ת������123��elementת��Ϊһ���Ϸ��ִ���
	3.ת����:	�뿴���#{_INFOMSG������˵��}�������
	*/

	STRING strOut;

	STRING::size_type sB = 0;
	STRING::size_type sE = strSrc.find_first_of(KeyStart);
	STRING::size_type sLen = strSrc.size();

	do
	{
		if( sE == STRING::npos )
		{
			//copy last
			strOut += strSrc.substr( sB );
			break;
		}

		//copy normal
		strOut += strSrc.substr( sB, sE- sB );

		//get op
		STRING strOp = strSrc.substr( sE+1, 1 );

		if ( strOp == "{" )	//OK, check magic #{} string.
		{
			//item element is valid. ex: #{_INFOID123}
			STRING strItemElement = strSrc.substr( sE+2, 7 );
			if ( strItemElement == "_INFOID" )
			{
				//get itemId
				//todo_yangjun	��Ҫ��ϸ���ʣ�µ��ַ��Ƿ���һ��������INFOID��Ϣ
				STRING::size_type sIDEnd = strSrc.find(ContentsEnd, sE+2+7);
				if(sE+2+7 >= sLen)	// fix dead loop if str is "xxx#{_INFOID" [9/25/2006]
				{
					//skip invalid #{
					sE += 2;
					goto LengthOver;
				}
				STRING strId = strSrc.substr(sE+2+7, sIDEnd-sE-2-7);
				INT itemId = atoi(strId.c_str());

				if(CTransferItemSystem::GetMe() && CTransferItemSystem::GetMe()->IsElementExist(itemId) && CGameProcedure::s_pUISystem)
				{//ok, valid item element found.

					//1. head
					strOut += MsgBegin;

					//2. content
					strOut += CTransferItemSystem::GetMe()->GetElementContents(itemId);

					//3. tail
					strOut += ContentsEnd;
				}

				//step to new point.
				sE = sIDEnd + 1;
			}
			else
			{
				//all other things is not allowed.
				strOut += strSrc.substr(sE,2);
				sE += 2;
			}
		}
		else
		{
			//single #
			strOut += strSrc.substr(sE,1);
			sE += 1;
		}
LengthOver:
		if(sE >= sLen) break;

		//save new begin point
		sB = sE;

		//find next KeyStart
		sE = strSrc.find(KeyStart, sB);

	}while(TRUE);

	return strOut;
}

STRING MailAnalyze::CreateItemElementInfo1(const STRING &strSrc)
{
	const CHAR		KeyStart		= '#';

	const STRING	MsgBegin		= "#{_INFOMSG";
	const CHAR		ContentsEnd		= '}';

	const STRING	IdBegin			= "#{_INFOID";
	const STRING	IdEnd			= "}";

	const STRING	ErrSign			= "<ERRTRANS>";

	/*
	1.ת��ǰ:	�뿴���#{_INFOMSG������˵��}�������
	2.����CTransferSystem��ת����������һ���µ�Element������Ϊ123��
	3.ת����:	�뿴���#{_INFOID123}�������
	*/

	STRING strOut;

	STRING::size_type sB = 0;
	STRING::size_type sE = strSrc.find_first_of(KeyStart);
	STRING::size_type sLen = strSrc.size();

	do
	{	
		if(sE == STRING::npos)
		{
			//copy last
			strOut += strSrc.substr(sB);
			break;
		}

		//copy normal
		strOut += strSrc.substr(sB, sE-sB);

		//get op
		STRING strOp = strSrc.substr(sE+1, 1);

		if(strOp == "{")	//ok, check magic #{} string.
		{
			//item element is valid. ex: #{_INFOMSG������˵��}
			if(sE+2 >= sLen) break;
			STRING strItemElement = strSrc.substr(sE+2, 8);
			if(strItemElement == "_INFOMSG")
			{
				STRING strContents = strSrc.substr( sE+2+8, sLen-sE-2-8 );
				//STRING::size_type sIDEnd = strContents.find_first_of(IdEnd);
				STRING::size_type sIDEnd = strSrc.find(IdEnd, sE);
				STRING::size_type nContentsLen = sIDEnd - sE - 2- 8; //strSrc.find(ContentsEnd, sE+2+8);
				if(sIDEnd >= sLen) break;
				if(strSrc.at(sIDEnd) != ContentsEnd)
				{
					//todo_yangjun ��Ҫ��ϸ����Ϊ�β��ǽ�β��ʶ��ԭ��
					sIDEnd = strSrc.find(ContentsEnd,sIDEnd);
				}
				if(sIDEnd == STRING::npos) break;

				strContents = strSrc.substr(sE+2+8, nContentsLen);


				if(CTransferItemSystem::GetMe())
				{//ok, valid item element found.

					//2. content
					CTransferItemSystem::Element* pE = CTransferItemSystem::GetMe()->ConvertFromString("", strContents);
					if(pE)
					{
						//1. head
						strOut += IdBegin;

						CHAR itemId[64] = {0};
						_snprintf(itemId, 63, "%d", pE->nID);
						strOut += itemId;

						//4. tail
						strOut += IdEnd;
					}
					else
					{
						strOut += ErrSign;
					}
				}

				//step to new point.
				sE = sIDEnd + 1;
			}
			else
			{
				//all other things
				strOut += strSrc.substr(sE, 2);
				sE += 2;
			}
		}
		else
		{
			//single #
			strOut += strSrc.substr(sE, 1);
			sE += 1;
		}

		if(sE >= sLen) break;

		//save new begin point
		sB = sE;

		//find next KeyStart
		sE = strSrc.find(KeyStart, sB);

	}while(TRUE);

	return strOut;
}



}