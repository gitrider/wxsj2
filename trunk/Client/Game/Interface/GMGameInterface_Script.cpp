
/**	GMGameInterface_Script.cpp
 *	
 *	功能：	
 *
 *	修改记录：
 *			080306	Lua_ShowGuildInfoDlg
 *			
 */

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
#include "CGAskDetailEquipList.h"
#include "CGEquipSuitExchange.h"
#include "DBC\GMDataBase.h"
#include "DataPool\GMDP_CharacterData.h"
#include "GMInterface_Script_Tooltip.h"
#include "world/scene.h"
#include "Procedure/GamePro_Enter.h"
#include "Procedure\GameProcedure.h"
#include "Procedure\GamePro_main.h"
#include "GIUISystem.h"
#include "Sound\GMSoundSystem.h"
#include "CGDiscardItem.h"
#include "CGDiscardEquip.h"
#include "CGCharIdle.h"
#include "Object\Logic\Item\Obj_Item_Equip.h"
#include "Object\ObjectCommandDef.h"
#include "CGExecuteScript.h"
#include "GIGfxSystem.h"
#include "CGMinorPasswd.h"								// 发送设置2级密码
#include "Action\GMActionSystem_MouseCmd.h"
#include "../Object/Manager/FakeObjectEntityManager.h"
#include "CGTopList.h"
#include "CGModifySetting.h"
#include "CGEquipRefining.h"
#include "CGEquipAdsorbMagic.h"
#include "CGEquipCancelMagic.h"

#include "Engine/EngineInterface.h"



namespace SCRIPT_SANDBOX
{
	const FLOAT PI_2 = 2 * __PI;
//登陆界面中，用于点击确定后的操作(-- for debug)
INT Lua_ServerLogin(LuaPlus::LuaState* state)
{
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcLogIn)
	{
		CSoundSystemFMod::_PlayUISoundFunc(68);
		if(CGameProcedure::s_pProcLogIn->GetStatus() == CGamePro_Login::LOGIN_DEBUG_SETTING)
		{
			CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_DISCONNECT);
		}
	}
	return 0;
}

//要求退出程序
INT Lua_QuitApplication(LuaPlus::LuaState* state)
{
	PostMessage(g_hMainWnd, WM_CLOSE, 0XC0DE, 0XC0DE);
	return 0;
}

//要求打开/关闭技能书
INT Lua_ToggleSkillBook(LuaPlus::LuaState* state)
{
	if(CGameProcedure::s_pUISystem && 
		(CGameProcedure::s_pUISystem->IsWindowShow("LifeSkill") || 
		 CGameProcedure::s_pUISystem->IsWindowShow("ActionSkill") ||
		 CGameProcedure::s_pUISystem->IsWindowShow("CommonSkill") ) ) 
		CEventSystem::GetMe()->PushEvent(GE_CLOSE_SKILL_BOOK);
	else
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_COMMONSKILL_PAGE);
	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH,10);
	return 0;
}

//要求打开技能书
INT Lua_OpenSkillBook(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_SKILL_BOOK);
	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH,10);
	return 0;
}
//要求打开/关闭宠物界面
INT Lua_TogglePetPage(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_PET_PAGE);
	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_SHOW, "");
	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH,10);
	return 0;
}
//要求打开生活技能界面
INT Lua_OpenLifePage(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_LIFE_PAGE);
//	CEventSystem::GetMe()->PushEvent(GE_UPDATE_TARGETPET_PAGE,0);

	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH,10);
	return 0;
}

//要求打开普通技能界面
INT Lua_OpenCommonSkillPage(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_COMMONSKILL_PAGE);
	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH,10);
	return 0;
}
INT Lua_UpdateMinimap( LuaPlus::LuaState* state )
{
	CGameProcedure::m_bNeedFreshMinimap = true;
	return 0;
}
INT Lua_OpenMinimap( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsString() ) return 0;
	if( strcmp( args[ 1 ].GetString(), "Minimap" ) == 0 ) // 打开小地图
	{
		CEventSystem::GetMe()->PushEvent( GE_OPEN_MINIMAP, "1" );
		
	}
	else if( strcmp( args[ 1 ].GetString(), "MinimapExp" ) == 0) // 打开扩展小地图
	{
		CEventSystem::GetMe()->PushEvent( GE_OPEN_MINIMAPEXP, "1" ); 
	}
	CGameProcedure::m_bNeedFreshMinimap = true;
	CGameProcedure::m_bWaitNeedFreshMinimap = true;
	return 0;
}
// 关闭打开大地图
INT Lua_ToggleLargeMap( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsString() ) 
		CEventSystem::GetMe()->PushEvent( GE_TOGLE_LARGEMAP, "2" );
		return 0;
	CEventSystem::GetMe()->PushEvent( GE_TOGLE_LARGEMAP, args[ 1 ].GetString() );
	CGameProcedure::m_bNeedFreshMinimap = true;
	CGameProcedure::m_bWaitNeedFreshMinimap = true;
	return 0;
}
// 关闭打开场景地图
INT Lua_ToggleSceneMap( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	std::vector< STRING > vParam;
	vParam.push_back( CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine()->szSceneMap );
	if( !args[ 1 ].IsString() ) 
		vParam.push_back( "1" );
	else
		vParam.push_back( args[ 1 ].GetString() );
	
	vParam.push_back("tab");
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_SCENEMAP, vParam );
	CGameProcedure::m_bNeedFreshMinimap = true;
	CGameProcedure::m_bWaitNeedFreshMinimap = true;
	return 0;
}

// 关闭大地图
INT Lua_CloseLargeMap( LuaPlus::LuaState* state )
{
	CEventSystem::GetMe()->PushEvent( GE_TOGLE_CLOSELARGEMAP );
	return 0;
}

// 关闭场景地图
INT Lua_CloseSceneMap( LuaPlus::LuaState* state )
{
	CEventSystem::GetMe()->PushEvent( GE_TOGLE_CLOSESCENEMAP );
	return 0;
}


// 更新世界地图列表
INT Lua_UpdateLargeMapList(LuaPlus::LuaState* state )
{	
	const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_WORLDPOS_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();
	for( int i = 0; i < nNum; i ++ )
	{
		std::vector< STRING > vParam;
		const _DBC_WORLDPOS_DEFINE* pLine = (const _DBC_WORLDPOS_DEFINE*)((tDataBase*)pdbcbase)->Search_LineNum_EQU(i);
		if( pLine )
		{
			//if( pLine->nXPos == 0 ||pLine->nYPos == 0 )
			//	continue;
			
			char arg0[128],arg1[128],arg2[128];
			std::string utf8Arg0, utf8Arg1, utf8Arg2, utf8Arg3, utf8Arg4;

			sprintf(arg0, "%d", pLine->nID );
			sprintf(arg1, "%s", pLine->szSceneName );
			sprintf(arg2, "%d,%d", pLine->nSceneType, pLine->nMapLevel );
			//sprintf(arg3,);

			//StringConvert::mbcs_to_utf8(arg0, utf8Arg0);
			vParam.push_back( arg0 );

			//StringConvert::mbcs_to_utf8(arg1, utf8Arg1);
			vParam.push_back( arg1 );

			//StringConvert::mbcs_to_utf8(arg2, utf8Arg2);
			vParam.push_back( arg2 );

			//StringConvert::mbcs_to_utf8(arg3, utf8Arg3);
			//vParam.push_back( utf8Arg3 );
			STRING a = vParam[0];
			STRING b = vParam[1];
			CEventSystem::GetMe()->PushEvent( GE_UPDATE_LARGEMAP_LIST, vParam );
		}
	}	
	
	return 0;
}

INT Lua_LargeMapListNum( LuaPlus::LuaState* state )
{

	const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_WORLDPOS_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();
	
	state->PushInteger(nNum);

	return 1;
}

INT Lua_SceneMapListNum( LuaPlus::LuaState* state )
{

	const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_SCENENPC_DENFINE );
	int nNum = pdbcbase->GetRecordsNum();
	//int nSceneId = CGameProcedure::s_pProcEnter->GetSceneID();
	int nSceneId = CGameProcedure::s_pProcEnter->GetSceneID();

	state->PushInteger(nNum);
	state->PushInteger( nSceneId );
	
	return 2;
}

INT Lua_LargeMapList( LuaPlus::LuaState* state )
{
	
	LuaStack args(state);
	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: EnumActionForEquip Wrong Param");
	}
	INT nIndex = args[1].GetInteger();

	const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_WORLDPOS_DEFINE );
	const _DBC_WORLDPOS_DEFINE* pLine = (const _DBC_WORLDPOS_DEFINE*)((tDataBase*)pdbcbase)->Search_LineNum_EQU(nIndex);
	if( pLine )
	{
		//if( pLine->nXPos == 0 ||pLine->nYPos == 0 )
		//	return 0;

		char arg0[128],arg1[128],arg2[128];

		sprintf(arg0, "%d", pLine->nID );
		sprintf(arg1, "%s", pLine->szSceneName );
		sprintf(arg2, "%d,%d", pLine->nSceneType, pLine->nMapLevel );

		state->PushString( arg0 );
		state->PushString( arg1 );
		state->PushString( arg2 );

	}

	return 3;
}

INT Lua_SceneMapList( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: EnumActionForEquip Wrong Param");
	}
	INT nIndex = args[1].GetInteger();

	const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_SCENENPC_DENFINE );
	const _DBC_SCENENPC_DENFINE* pLine = (const _DBC_SCENENPC_DENFINE*)((tDataBase*)pdbcbase)->Search_LineNum_EQU(nIndex);
	if( pLine )
	{
		//if( pLine->nXPos == 0 ||pLine->nYPos == 0 )
		//	return 0;

		char arg0[128],arg1[128],arg2[128], arg3[128];

		sprintf(arg0, "%d", pLine->nID );
		sprintf(arg1, "%s,%s", pLine->szNpcName, pLine->szNpcTypeName );
		sprintf(arg2, "%d,%d", pLine->nNpcType, pLine->nSceneID );
		sprintf(arg3, "%d %d,%d",pLine->nIsTalkNpc, pLine->nXPos, pLine->nYPos );

		state->PushString( arg0 );
		state->PushString( arg1 );
		state->PushString( arg2 );
		state->PushString( arg3 );

	}

	return 4;
}

// 更新场景地图列表
INT Lua_UpdateSceneMapList(LuaPlus::LuaState* state )
{
	
	return 0;
}


// 打开包裹
INT Lua_ToggleContainer(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_CONTAINER);
	//CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH,70);
	return 0;
}

// 打开任务
INT Lua_ToggleMission(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_MISSION);
	return 0;
}

// 关闭所有二级菜单
INT Lua_CloseSecondMenu(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: Lua_CloseSecondMenu Wrong Param");
	}

	// 0:关闭所有菜单
	// 1:关闭左边菜单
	// 2:关闭右边菜单
	INT dir = args[1].GetInteger();

	// 要关闭的窗口的 id
	INT ban = args[2].GetInteger();

	CEventSystem::GetMe()->PushEvent(GE_COLSE_SECOND_MENU, dir, ban);

	return 0;
}

INT Lua_PushDebugMessage(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsString()))
	{
		KLThrow("LUA: Lua_PushDebugMessage Wrong Param");
	}
	STRING Msg = args[1].GetString();

	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,args[1].GetString());

	return 0;
}

INT	Lua_UpdateChangeSuit(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[1].IsString())) return 0;
	if (!(args[2].IsInteger())) return 0;
	if (!(args[3].IsInteger())) return 0;

	STRING iconName = args[1].GetString();
	int itemId = args[2].GetInteger();
	int suitId = args[3].GetInteger();

	CActionSystem::GetMe()->UpdateAction_FromChangeSuit(iconName, itemId, suitId);

	return 1;
}

INT Lua_Get_XParam_INT(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: Lua_Get_XParam_INT Wrong Param");
	}
	INT ret = (INT)CDataPool::GetMe()->X_PARAM_Get()->GetIntValue(args[1].GetInteger());

	state->PushInteger(ret);

	return 1;
}

INT Lua_Get_XParam_STR(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: Lua_Get_XParam_STR Wrong Param");
	}
	char* ret = CDataPool::GetMe()->X_PARAM_Get()->GetStrValue(args[1].GetInteger());
	if(!ret) return 0;

	state->PushString(ret);

	return 1;
}

INT Lua_Set_XSCRIPT_Function_Name(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsString()))
	{
		KLThrow("LUA: Lua_Set_XSCRIPT_Function_Name Wrong Param");
	}

	char* str_temp = (char*)args[1].GetString();

	if(!str_temp) return 0;
	CDataPool::GetMe()->X_XCRIPT_Get()->SetFunName(str_temp);

	return 0;
}

INT Lua_Set_XSCRIPT_Parameter(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger() && args[2].IsInteger()))
	{
		KLThrow("LUA: Lua_Set_XSCRIPT_Parameter Wrong Param");
	}
	CDataPool::GetMe()->X_XCRIPT_Get()->SetParam(args[1].GetInteger(),args[2].GetInteger());

	return 0;
}

INT Lua_Set_XSCRIPT_ScriptID(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: Lua_Set_XSCRIPT_ScriptID Wrong Param");
	}
	CDataPool::GetMe()->X_XCRIPT_Get()->SetScriptID(args[1].GetInteger());

	return 0;
}

INT Lua_Set_XSCRIPT_ParamCount(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: Lua_Set_XSCRIPT_ParamCount Wrong Param");
	}
	CDataPool::GetMe()->X_XCRIPT_Get()->SetParamCount(args[1].GetInteger());

	return 0;
}

INT Lua_Clear_XSCRIPT(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	CDataPool::GetMe()->X_XCRIPT_Get()->CleanUp();

	return 0;
}

INT Lua_Send_XSCRIPT(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	CGExecuteScript msg;

	X_SCRIPT* pXScript_DataPool = CDataPool::GetMe()->X_XCRIPT_Get();
	X_SCRIPT* pXScript_Send = msg.GetScript();

	memcpy(pXScript_Send,pXScript_DataPool,sizeof(X_SCRIPT));
	
	CNetManager::GetMe()->SendPacket( &msg );
	
	return 0;
}

/*
//导出数据池内从服务器发送来的老师的门派信息
INT Lua_GetMenpaiInfo(LuaPlus::LuaState* state)
{
	INT idMenpai = (INT)CDataPool::GetMe()->JoinMenpai_GetMenpaiInfo();
	state->PushNumber( idMenpai );

	//有可能将要扩展导出的内容
	return 1;
}
//加入门派
INT Lua_MenpaiFrameJoin(LuaPlus::LuaState* state)
{
	INT idMenpai = (INT)CDataPool::GetMe()->JoinMenpai_GetMenpaiInfo();

	CDataPool::GetMe()->SendJoinMenpaiEvent();
	return 0;
}
*/

// 设置UI摄像机
INT Lua_Set_UICamera(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].GetNumber()))
	{
		KLThrow("LUA: Lua_Set_UICamera Wrong Param[1]");
	}
	FLOAT fHeight = args[1].GetNumber();

	if (!(args[2].GetNumber()))
	{
		KLThrow("LUA: Lua_Set_UICamera Wrong Param[2]");
	}
	FLOAT fDistance = args[2].GetNumber();

	if (!(args[3].GetNumber()))
	{
		KLThrow("LUA: Lua_Set_UICamera Wrong Param[3]");
	}
	FLOAT fPitch = args[3].GetNumber();

//	CFakeObjectEntityManager::GetMe()->SetFakeCameraParameter(fHeight, fDistance, fPitch);

	return 0;
}


//导出心法等级
INT Lua_GetXinfaLevel(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: Lua_GetXinfaLevel Wrong Param");
	}
	INT nXinfaID = args[1].GetInteger();

	/*const SCLIENT_XINFA* pXinFa = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_XinFa(nXinfaID);
	if(!pXinFa) 
	{
		state->PushInteger(-1);
	}
	else
	{
		state->PushInteger(pXinFa->m_nLevel);
	}*/

	return 1;
}

//导出心法升级需要的金钱,传入的参数为要学习的心法等级
INT Lua_GetUplevelXinfaSpendMoney(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: Lua_GetUplevelXinfaSpendMoney Wrong Param");
	}

	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA: Lua_GetUplevelXinfaSpendMoney Wrong Param");
	}

	INT nXinfaId = args[1].GetInteger();
	INT nXinfaLevel = args[2].GetInteger();

	////加载心法升级消耗表
	//DBC_DEFINEHANDLE(pSpendDBC, DBC_XINFA_STUDYSPEND);
	//
	//INT nNum = (INT)pSpendDBC->GetRecordsNum();
	//for(INT i=0; i<nNum; i++)
	//{
	//	// xf_learnspend.tab
	//	const _DBC_XINFA_STUDYSPEND* pSpend = (const _DBC_XINFA_STUDYSPEND*)pSpendDBC->Search_LineNum_EQU(i);
	//	if(pSpend->nStudyLevel != nXinfaLevel) 
	//		continue;
	//	INT nNeedMoney	= pSpend->StudySpend[nXinfaId].dwSpendMoney;

	//	//const _DBC_XINFA_STUDYSPEND* pSpend = (const _DBC_XINFA_STUDYSPEND*)pSpendDBC->Search_LineNum_EQU(i);
	//	//if(pSpend->nStudyLevel != nXinfaLevel) 
	//	//	continue;
	//	state->PushNumber( nNeedMoney );

	//	// 同时输出转换为金币、银币、铜币的格式输出
	//	INT nMoney = nNeedMoney;
	//	INT nGoldCoin;	
	//	INT nSilverCoin;
	//	INT nCopperCoin;

	//	nCopperCoin = nMoney % 100;

	//	if( nMoney >= 100 )
	//	{
	//		nSilverCoin = ((nMoney-nCopperCoin)/100) % 100;
	//	}
	//	else
	//	{
	//		nSilverCoin = 0;
	//	}

	//	if ( nMoney >= 10000 )
	//	{
	//		nGoldCoin = (((nMoney-nCopperCoin)/100)-nSilverCoin)/100;
	//	}
	//	else
	//		nGoldCoin = 0;

	//	state->PushInteger(nGoldCoin);
	//	state->PushInteger(nSilverCoin);
	//	state->PushInteger(nCopperCoin);

	//	break;
	//}
	return 4;
}
//导出心法升级需要的经验值,传入的参数为要学习的心法等级
INT Lua_GetUplevelXinfaSpendExp(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: Lua_GetUplevelXinfaSpendExp Wrong Param");
	}
	if (!(args[2].IsInteger()))
	{
		KLThrow("LUA: Lua_GetUplevelXinfaSpendExp Wrong Param");
	}
	INT nXinfaId = args[1].GetInteger();
	INT nXinfaLevel = args[2].GetInteger();

	//// 加载心法升级消耗表 xf_learnspend.tab
	//DBC_DEFINEHANDLE(pSpendDBC, DBC_XINFA_STUDYSPEND);
	//INT nNum = (INT)pSpendDBC->GetRecordsNum();
	//for(INT i=0; i<nNum; i++)
	//{
	//	const _DBC_XINFA_STUDYSPEND* pSpend = (const _DBC_XINFA_STUDYSPEND*)pSpendDBC->Search_LineNum_EQU(i);
	//	if(pSpend->nStudyLevel != nXinfaLevel) 
	//		continue;
	//	INT nNeedExp	= pSpend->StudySpend[nXinfaId].dwSpendExperience;
	//	state->PushNumber( nNeedExp );
	//}
	return 1;
}

//学习技能界面
INT lua_SkillsStudyFrame_study(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: lua_SkillsStudyFrame_study Wrong Param");
	}

	//$$ CDataPool::GetMe()->SendStudyXinfaEvent(args[1].GetInteger());

	return 0;
}

INT	Lua_GetActionNum(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsString()))
	{
		KLThrow("LUA: GetActionNum Wrong Param");
	}

	char* strType = (char*)args[1].GetString();

	int posIndex = -1;
	if ( args[2].IsInteger() )
	{
		posIndex = args[2].GetInteger();
	}

	state->PushNumber( CActionSystem::GetMe()->GetActionNum(strType, posIndex) );

	return 1;
}

INT Lua_IsWindowShow(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsString()))
	{
		KLThrow("LUA: Lua_IsWindowShow Wrong Param");
	}
	BOOL ret = CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow(args[1].GetString());
	state->PushBoolean( ret != FALSE );

	return 1;
}

//得到操作对象
INT	Lua_EnumAction(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: EnumAction Wrong Param1");
	}
	if (!(args[2].IsString()))
	{
		KLThrow("LUA: EnumAction Wrong Param2");
	}

	INT nIndex = args[1].GetInteger();
	const char* szFilter = args[2].GetString();

	tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex, szFilter);

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




//得到操作对象
INT	Lua_EnumActionForEquip(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: EnumActionForEquip Wrong Param");
	}
	 
	INT nIndex = args[1].GetInteger();

	tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(nIndex, NAMETYPE_EQUIP);

	if(pActionItem) 
	{
		LuaObject objReturn = state->BoxPointer(pActionItem);
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();

		if(((tObject_Item*)pActionItem->GetImpl()))
		{
			if(((tObject_Item*)pActionItem->GetImpl())->IsLocked())
			{
					state->PushInteger(1);
			}

			else
			{
					state->PushInteger(0);
			}

		}else
		{
			state->PushInteger(-1);

		}

	}
	else
	{
		LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();
		state->PushInteger(-1);
	}

	return 2;
}





INT	Lua_GetAction_SkillID(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: GetAction_SkillID Wrong Param1");
	}

	INT nIndex = args[1].GetInteger();

	tActionItem* pActionItem = CActionSystem::GetMe()->GetAction_SkillID(nIndex);

	if(pActionItem) 
	{
		LuaObject objReturn = state->BoxPointer(pActionItem);
		objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
		objReturn.PushStack();
	}
	else
	{
		state->PushNil();
	}

	return 1;
}

INT	Lua_GetAction_ItemID(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: GetAction_ItemID Wrong Param1");
	}

	INT nIndex = args[1].GetInteger();

	tActionItem* pActionItem = CActionSystem::GetMe()->GetAction_ItemID(nIndex);

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

INT	Lua_ShowContexMenu(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsString()))
	{
		KLThrow("LUA:ShowContexMenu Wrong Param");
	}

	LPCTSTR szMenuName = args[1].GetString();

	POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();

	std::vector< STRING > vParam;

	//arg0 - MenuNema
	vParam.push_back(szMenuName);

	vParam.push_back("-1");

	//arg2, arg3 MouseX, MouseY
	CHAR szTemp[32];

	_snprintf(szTemp, 32, "%d", ptMouse.x);
	vParam.push_back(szTemp);

	_snprintf(szTemp, 32, "%d", ptMouse.y);
	vParam.push_back(szTemp);

	CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);

	return 0;
}

INT Lua_QuestFrameOptionClicked(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsNumber()))
	{
		KLThrow("LUA:QuestFrameOptionClicked Wrong Param1");
	}

	if (!(args[2].IsString()))
	{
		KLThrow("LUA:QuestFrameOptionClicked Wrong Param2");
	}

	//if (!(args[3].IsNumber()))
	//{
	//	KLThrow("LUA:QuestFrameOptionClicked Wrong Param3");
	//}

	INT nIndex = (INT)args[1].GetNumber();
	INT nExIndex2 = (INT)args[3].GetNumber();

	// 界面显示
	if( nIndex == 0 )
	{
		STRING nExIndex1( args[2].GetString() );
		CEventSystem::GetMe()->PushEvent( nExIndex1, nExIndex2);
	}
	// 服务器处理
	else if( nIndex == -1 )
	{
		INT nExIndex1 = (INT)args[2].GetNumber();
		CUIDataPool::GetMe()->SendSelectEvent(nIndex,nExIndex1,nExIndex2);
	}

	//CGameProcedure::s_pGfxSystem->PushDebugString("CGEventRequest");

	return 0;
}

//接受任务
INT Lua_QuestFrameAcceptClicked(LuaPlus::LuaState* state)
{
	CUIDataPool::GetMe()->SendAcceptEvent();
	return 0;
}
//拒绝任务
INT Lua_QuestFrameRefuseClicked(LuaPlus::LuaState* state)
{
	CUIDataPool::GetMe()->SendRefuseEvent();
	return 0;
}
//继续任务
INT Lua_QuestFrameMissionContinue(LuaPlus::LuaState* state)
{
	CUIDataPool::GetMe()->SendContinueEvent();
	return 0;
}
//放弃任务
INT Lua_QuestFrameMissionAbnegate(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if ( !(args[1].IsInteger()) ) return 0;
	CUIDataPool::GetMe()->SendAbandonEvent(args[1].GetInteger());
	return 0;
}
//完成任务
INT Lua_QuestFrameMissionComplete(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if (!(args[1].IsNumber()))
	{
		KLThrow("LUA:QuestFrameMissionComplete Wrong Param");
	}

	INT nIndex = (INT)args[1].GetNumber();

	//为了对应服务器从1开始编号
	CUIDataPool::GetMe()->SendCompleteEvent( nIndex );
	return 0;
}


// 根据ID得到场景名字
INT Lua_GetSceneNameFromID( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA:QuestFrameMissionComplete Wrong Param");
	}

	int id = args[1].GetInteger();
	const tDataBase* pdbcbase = CDataBaseSystem::GetMe()->GetDataBase( DBC_SCENE_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();
	for( int i = 0; i < nNum; i ++ )
	{
		const _DBC_SCENE_DEFINE* pLine = (const _DBC_SCENE_DEFINE*)((tDataBase*)pdbcbase)->Search_First_Column_Equ(_DBC_SCENE_DEFINE::SCENE_SERVER_ID_COLUMN, i);
		if( pLine->nServerID == id )
		{
			state->PushString(pLine->szName);
			return 1;
		}
	}
	return 0;
}

// 得到当前场景得尺寸
INT Lua_GetSceneSize( LuaPlus::LuaState* state)
{
	const _DBC_SCENE_DEFINE* pSceneDef = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine();
	state->PushNumber( pSceneDef->nXSize );
	state->PushNumber( pSceneDef->nZSize );
	return 2;
}
INT Lua_GetSceneID( LuaPlus::LuaState* state)
{
	state->PushInteger( CWorldManager::GetMe()->GetActiveSceneID() );
	return 1;
}
// 得到当前小地图得尺寸
INT Lua_GetMinimapSize( LuaPlus::LuaState* state)
{

	state->PushNumber( 256 );
	state->PushNumber( 256 );
	return 2;
}
INT Lua_ResetCamera( LuaPlus::LuaState* state )
{
	//FLOAT fSub = max( FLOAT( nMousePosXDiff /10.0f ), 1.0f) / 100;
	float fCurrentDirect = float( CGameProcedure::s_pGfxSystem->Camera_GetDirection() );
	if( fCurrentDirect < 0 )
	{
		float ftmp = float( int( fCurrentDirect / PI_2 ) );
		ftmp *= PI_2;
		fCurrentDirect = fCurrentDirect - ftmp + PI_2;
	}
	else 
	{
		fCurrentDirect = fCurrentDirect - float( int( fCurrentDirect / PI_2 ) * PI_2 );
	}
	CGameProcedure::s_pGfxSystem->Camera_Direction( fCurrentDirect );
	if( fCurrentDirect < __PI  )
		CGamePro_Main::m_nResetCamera = 1;
	else 
		CGamePro_Main::m_nResetCamera = -1;
	return 0;
}

//要求打开/关闭技能书
INT Lua_ToggleComposeWnd(LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_TOGLE_COMPOSE_WINDOW);
	return 0;
}
//得到某一种配方的数量
INT Lua_GetMethodNumber( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	// 第一个值为配方类型，第二个为配方序列号
	if (!(args[1].IsInteger()))
	{
		KLThrow("LUA: EnumAction Wrong Param");
	}

	INT nType = args[1].GetInteger();		//  配方类型

	return 0;
}
//枚举配方数据
INT	Lua_EnumMethod(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	// 第一个值为配方类型，第二个为配方序列号
	if (!(args[1].IsInteger()))
	{
		//INT t1 = args[1].GetType();
		//INT t2 = args[1].GetType();
		KLThrow("LUA: EnumAction Wrong Param");
	}

	INT nType = args[1].GetInteger();		//  配方类型
	INT nIndex = args[ 2 ].GetInteger();	//  配方序列好
	return 3;
}

//发送聊天信息 
/*
1. 频道
2. 聊天类型，比如是私了还是喊叫
3. 队伍的id,如果有队伍的话
4. 聊天的内容
5. 聊天的对象，把话发给谁。
*/
INT Lua_SendChatMessage( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsString() ) return 0; // 消息字符串 
	if( !args[ 2 ].IsString() ) return 0; // 目标名字字符串
	if( !args[ 3 ].IsInteger() ) return 0; //频道id
	if( !args[ 4 ].IsInteger() ) return 0; // 消息类型
	if( !args[ 5 ].IsInteger() ) return 0;	// 团队id

	CGameProcedure::s_pGameInterface->SendChatCommand( args[ 3 ].GetInteger(),
		args[ 4 ].GetInteger(),
		args[ 5 ].GetInteger(),
		args[ 1 ].GetString(),
		args[ 2 ].GetString() );
	return 0;
}

INT Lua_SendGMCommand(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsString() ) return 0; // 消息字符串 

	CGameProcedure::s_pGameInterface->GM_SendCommand(args[1].GetString());
	return 0;
}

INT Lua_GetCurChatProperty( LuaPlus::LuaState* state )
{
	state->PushInteger( 0 );
	state->PushInteger( 0 );
	state->PushInteger( 0 );
	state->PushString( "" );
	return 4;
}

INT Lua_AxTrace(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	
	if(!(args[1].IsInteger()) || !(args[2].IsInteger()) || !(args[3].IsString()))
		return 0;

	AxTrace(args[1].GetInteger(), args[2].GetInteger(), "%s", args[3].GetString());

	return 0;
}

INT Lua_ComposeItem_Begin(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsInteger()) || !(args[2].IsInteger()) )
	{
		KLThrow("LUA: ComposeItem Wrong Param");

	}
	if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get(CObject_Character::LOGIC_BASE) == CObject_Character::CHARACTER_LOGIC_ABILITY_ACTION )
		return 0;
	INT Make_Count = args[2].GetInteger();

	if(Make_Count < 1)
		return 0;

//	INT	nAbility		= args[1].GetInteger();
	INT	nPrescription	= args[1].GetInteger();
//	ObjID_t	n_guidPlatform	= args[3].GetInteger();
//	AxTrace(nAbility, nPrescription, "%s", n_guidPlatform);
	CGameProcedure::s_pGameInterface->Player_UseLifeAbility( nPrescription, Make_Count);

	return 0;
}

INT Lua_ComposeItem_Cancel(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	//if (! (args[1].IsInteger()) )
	//{
	//	KLThrow("LUA: ComposeItem_Cancel Wrong Param");
	//}
//	INT	nAbility		= args[1].GetInteger();
//	INT	nPrescription	= args[1].GetInteger();
//	ObjID_t	n_guidPlatform	= args[3].GetInteger();
//	AxTrace(nAbility, nPrescription, "%s", n_guidPlatform);

	//anyway，hide prograssbar first.
	CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_HIDE);
	CObject_PlayerMySelf *pMySelf = CObjectManager::GetMe()->GetMySelf();
	// removed by wangpojin 
	// 不充许这么操作
	//SCommand_Object pCmd;
	//pCmd.m_wID = OC_IDLE;
	//pCmd.m_abParam[2] = TRUE;
	////when the cancel button is clicked, the logic of client enter idle first.
	////then send the idle command to server.
	////it is not the best measure,but i can't get the better way.
	//pMySelf->DoCommand(&pCmd);
	//向服务器发送发呆消息
	CGCharIdle msg;
	CNetManager::GetMe()->SendPacket( &msg );

	return 0;
}


INT Lua_OpenBooth( LuaPlus::LuaState* state) 
{
	//测试使用
	return 0;
}



INT Lua_CloseBooth( LuaPlus::LuaState* state)
{
	CGameProcedure::s_pGameInterface->Booth_Close();
	return 0;
}

INT Lua_RepairAll( LuaPlus::LuaState* state)
{
	//检查是否有需要修理的物品
	BOOL bNeed = FALSE;
	for(INT i = 0; i<HEQUIP_NUMBER; i++)
	{
		CObject_Item_Equip* pItem = (CObject_Item_Equip*)(CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)i));
		if(!pItem)
			continue;
		if(pItem->GetItemDur() < pItem->GetItemMaxDur())
		{
			bNeed = TRUE;
			break;
		}
	}

	//检查背包
	for(INT i = 0; i<MAX_BAG_SIZE; i++)
	{
		CObject_Item* pItem = (CObject_Item*)CDataPool::GetMe()->UserBag_GetItem(i);
		if(!pItem || pItem->GetItemClass() != ICLASS_EQUIP)
			continue;
		if(pItem->GetItemDur() < pItem->GetItemMaxDur())
		{
			bNeed = TRUE;
			break;
		}
	}
       

	//需要修理的时候再通知服务器端修改
	if(bNeed)
	{
		CGameProcedure::s_pGameInterface->Booth_Repair(true,0,false);
	}
	else
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
		if(bLog)	
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Info_Too_Far_To_Open_Stall");
			ADDNEWDEBUGMSG(strTemp);
			//ADDNEWDEBUGMSG(STRING("没有物品需要修理"));
		}
	}

	return 0;
}

INT Lua_RepairOne( LuaPlus::LuaState* state)
{
	//设置鼠标为修理状态
	CGameProcedure::s_pGameInterface->Booth_EnterRepair();
	return 0;
}

//设置鼠标为默认状态
INT Lua_CancelRepairState( LuaPlus::LuaState* state)
{ 
	CGameProcedure::s_pGameInterface->Skill_CancelAction();
	return 0;
}


//打开关闭称号界面
INT Lua_OpenSelfEquip( LuaPlus::LuaState* state) 
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) 
	{
		return 0;
	}

	CEventSystem::GetMe()->PushEvent(GE_OPEN_EQUIP, args[1].GetInteger());

	return 0;
}
//关闭称号界面
INT Lua_CloseTitleList( LuaPlus::LuaState* state) 
{
	CEventSystem::GetMe()->PushEvent(GE_CLOSE_AGNAME);

	return 0;
}


//打开称号界面
INT Lua_OpenTitleList( LuaPlus::LuaState* state) 
{
	CEventSystem::GetMe()->PushEvent(GE_OPEN_AGNAME);

	return 0;
}


INT Lua_GetCurrentSceneName( LuaPlus::LuaState* state )
{
	const _DBC_SCENE_DEFINE* pSceneDef = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine();
	state->PushString( pSceneDef->szName );
	return 1;
}

//获得商店的修理和收购类型
INT Lua_GetShopType( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[1].IsString() )
	{
		KLThrow("LUA: Lua_GetShopType[1] param parameter error");
	}
	std::string str = args[1].GetString();

	if(str == "Repair")
	{
		state->PushInteger(CDataPool::GetMe()->Booth_GetRepairType());
	}
	else if( str == "buy" )
	{
		state->PushInteger(CDataPool::GetMe()->Booth_GetBuyType());
	}

	return 1;
}

INT Lua_GetOperationType( LuaPlus::LuaState* state )
{
	state->PushInteger(CDataPool::GetMe()->Booth_GetRepairType());
	return 1;
}

INT Lua_OpenCharacter( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;
	CEventSystem::GetMe()->PushEvent( GE_OPEN_CHARACTOR, args[ 1 ].GetInteger() );
	return 0;
}

INT Lua_OpenEquip( LuaPlus::LuaState* state )
{
	CEventSystem::GetMe()->PushEvent(GE_OPEN_EQUIP);
	CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_PLAYER_ATTR_PAGE);	
	return 0;
}

// 打开个人装备 个人属性 个人称号  一键换装 骑乘属性  个人资料
INT		Lua_OpenEquipAll( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;
	CEventSystem::GetMe()->PushEvent(GE_EQUIP_ALL_OPEN,args[ 1 ].GetInteger()); //0表示 装备分界面 1表示宠物分界面

	return 0;
}

// 关闭个人装备 个人属性 个人称号  一键换装 骑乘属性  个人资料
INT		Lua_CloseEquipAll( LuaPlus::LuaState* state )
{
	 
	CEventSystem::GetMe()->PushEvent(GE_EQUIP_ALL_CLOSE); 

	return 0;
}
 

// 打开关闭一键换装界面
INT Lua_OpenQuickChangeEquip( LuaPlus::LuaState* state )
{
	CEventSystem::GetMe()->PushEvent(GE_QUICK_CHANGE_EQUIP);

	return 0;
}

INT Lua_OpenMainFuncBtn( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;

	CEventSystem::GetMe()->PushEvent(GE_OPEN_MAIN_FUNC_BTN, args[1].GetInteger());

	return 0;
}


INT Lua_GetActionInfo( LuaPlus::LuaState* state )
 
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;

	int idx = args[ 1 ].GetInteger();

	CGEquipSuitExchange msg;
	msg.setEquipSuitNum(idx);
	CNetManager::GetMe()->SendPacket( &msg );

	return 0;
}

 
 //玩家配方数量
INT  Lua_EnumPackFormulationNum(LuaPlus::LuaState* state)
{
	//Product_info.tab
	DBC_DEFINEHANDLE(s_pItem_Equip, DBC_EQUIP_PRODUCT_INFO);
	//搜索纪录	
	int t =s_pItem_Equip->GetRecordsNum();

	state->PushInteger(s_pItem_Equip->GetRecordsNum());

	return 1;
}


//枚举玩家可用配方配方 信息 --- 判断玩家是否符合当前这个配方条件
INT     Lua_EnumPackProCanUse(LuaPlus::LuaState* state) 
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;  

	//Product_info.tab
	DBC_DEFINEHANDLE(s_pItem_Equip, DBC_EQUIP_PRODUCT_INFO);
	//搜索纪录
	const _DBC_EQUIP_PRODUCT_INFO* pGEquipProduct  = (const _DBC_EQUIP_PRODUCT_INFO*)s_pItem_Equip->Search_Index_EQU(args[ 1 ].GetInteger());
    if (pGEquipProduct)
    {
		  INT materialId , materialNum ,materialLevel  ;
          tObject_Item *pMaterial ;

          //判断一种材料 玩家是否有 〈id 等级 数量 〉


		 INT  bagMatNum = 0; //玩家材料数量
		 INT  matIndex = 1; //材料索引
		 INT  flagID = 0;     //MAX_CREATE_MATERIAL_NUM

		do
		{	 
			if (matIndex > MAX_EQUIP_PROMATL_NUM)
			{
				break;
			}

            flagID = 0;
          
			
			materialId	  =   pGEquipProduct->szMat[matIndex].nMaterial;
			materialNum	  =   pGEquipProduct->szMat[matIndex].nMaterialNum;
			materialLevel =   pGEquipProduct->szMat[matIndex].nMatLowestLv;			


			matIndex++; 
			
			if( (materialId == -1)  )
			{
				flagID  = 1;
				continue;				
			}

				
				for ( UINT i = 0; i < MAX_BAG_SIZE; ++i)
				{
					pMaterial = CDataPool::GetMe()->UserBag_GetItem(i);

					if (pMaterial)
					{
						if(pMaterial->GetItemClass() == ICLASS_MATERIAL)
						{  				
							for (UINT j = materialLevel ;j<=  MAX_MATERIAL_LEVEL ; ++j)  // 最少等级--6级 
							{
								

								if (pMaterial->GetIdTable() == GenMaterialIndex(materialId,j ) )
								{		 

									bagMatNum +=pMaterial->GetNumber();

								}	

							}

						}	
					}

				}
		
	 

		} while( flagID ||( bagMatNum >= materialNum && matIndex <= MAX_EQUIP_PROMATL_NUM ) );

		if( matIndex >= 4 )

		{
			state->PushInteger(1); //1表示玩家符合这种配方的条件

		}else
		{
			state->PushInteger(0);
		}
	}else
	{
		state->PushInteger(-1);
	}

	return 1;


}


///分析装备拆解后的材料数量和等级
INT     Lua_EquipDismantle_Material_Info(LuaPlus::LuaState* state)

{
	
	tObject_Item *pItem = NULL;

	INT idTable = -1;

	if ( CDataPool::GetMe()->Dismantle_GetItemConta() == tObject_Item::IO_MYSELF_PACKET)
	{
		pItem  = 	CDataPool::GetMe()->UserBag_GetItem(  CDataPool::GetMe()->Dismantle_GetItemPos()  );
	
	}else if ( CDataPool::GetMe()->Dismantle_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP)
	{

		pItem  = 	CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP(  CDataPool::GetMe()->Dismantle_GetItemPos() ) );
	}	
		
	if ( !pItem )
	{
		for (INT i = 0 ;i< MAX_EQUIP_PROMATL_NUM; ++i)
		{
			state->PushString("");  
			state->PushInteger(-1);
			state->PushInteger(-1);

		}
		state->PushInteger(-1);
		return MAX_EQUIP_PROMATL_NUM * 3 + 1;

	}

	 		
	 idTable = pItem->GetIdTable();
 
		//Product_info.tab
	 DBC_DEFINEHANDLE(s_pItem_Equip, DBC_EQUIP_PRODUCT_INFO);

		const _DBC_EQUIP_PRODUCT_INFO* pGEquipProduct  = NULL;
		INT i;
		for (i = 1;i<= (int)s_pItem_Equip->GetRecordsNum();i++) 
		{
			pGEquipProduct  = (const _DBC_EQUIP_PRODUCT_INFO*)s_pItem_Equip->Search_Index_EQU(i);
			if (pGEquipProduct)
			{					
				if (pGEquipProduct->nProductId ==  idTable)
				{
					CDataPool::GetMe()->Dismantle_SetIdTable( pGEquipProduct->nFormulaId );
					break;
				}

			}	
		}

		if (  (i > (int)s_pItem_Equip->GetRecordsNum()))
		{
			pGEquipProduct = NULL;
			
			STRING strTemp = "该装备无法拆解";
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

			for (INT i = 0 ;i< MAX_EQUIP_PROMATL_NUM; ++i)
			{
				state->PushString("");  
				state->PushInteger(-1);
				state->PushInteger(-1);
			}

			if (pItem)
			{
				pItem->SetLock(FALSE);
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
			}
		}else
		{
			
		
				//拆解获得材料分   = 装备分*拆解比例/(100*制造材料数量)
				//拆解获得材料个数 = 打造材料数量*拆解比例/100
				INT score =	 ( (CObject_Item_Equip *) pItem )->GetEquipScore();

				for (INT i = 0 ;i< MAX_EQUIP_PROMATL_NUM; ++i)
				{
					if(pGEquipProduct->szRwMat[i].nReWorkMaterial!= -1)
					{			
						CDataPool::GetMe()->Dismantle_RecordMatNum();  //材料数量	
						CDataPool::GetMe()->Dismantle_SetMatNum( pGEquipProduct->szMat[i].nMaterialNum,i);
					}		
					 
				}

				INT  MatScore ; //拆解获得材料分  

				DBC_DEFINEHANDLE(s_pItem_Mat, DBC_EQUIP_MATERIAL_INFO);
				const _DBC_EQUIP_MATERIAL_INFO *pItem_Mat = NULL , *pItem_Mat_Max =NULL ;

				pItem_Mat_Max = (const _DBC_EQUIP_MATERIAL_INFO*)s_pItem_Mat->Search_Index_EQU(MAX_MATERIAL_LEVEL);
		      
				for (INT i = 0 ;i< CDataPool::GetMe()->Dismantle_GetRecordMatNum(); ++i)
				{

					MatScore = score *  (pGEquipProduct->szMat[i].nMaterialLoss)/100 / (CDataPool::GetMe()->Dismantle_GetMatNum(i)) ;

					for (INT j = 1 ;j <=  MAX_MATERIAL_LEVEL ; ++j)
					{

						pItem_Mat	= (const _DBC_EQUIP_MATERIAL_INFO*)s_pItem_Mat->Search_Index_EQU(j);

						if ( MatScore < pItem_Mat->nMaterialValues   )
						{		 

							CDataPool::GetMe()->Dismantle_SetMatGrade( j,i );   // 拆分后每个材料的等级
							break;

						}else if(MatScore > pItem_Mat_Max->nMaterialValues )
						{
							CDataPool::GetMe()->Dismantle_SetMatGrade( MAX_MATERIAL_LEVEL,i );   // 拆分后每个材料的等级
							break;

						}

					}


				}

				INT Id = 0;
				 
				for (INT i = 0 ;i< CDataPool::GetMe()->Dismantle_GetRecordMatNum(); ++i)

					{


						CDataPool::GetMe()->Dismantle_SetMatNum( pGEquipProduct->szMat[i].nMaterialNum *pGEquipProduct->szMat[i].nMaterialLoss/100 ,i);

						Id =  GenMaterialIndex(pGEquipProduct->szMat[i].nMaterial ,  CDataPool::GetMe()->Dismantle_GetMatGrade(i) );

						CDataPool::GetMe()->Dismantle_SetMatId(Id,i); //材料的id
					}

					//打开数据表
					DBC_DEFINEHANDLE(s_pItem_Medic, DBC_ITEM_MEDIC);
					//搜索纪录
					const _DBC_ITEM_MEDIC* pMedic =  NULL;
			

					for (INT i = 0 ;i< MAX_EQUIP_PROMATL_NUM; ++i)
					{
						pMedic = (const _DBC_ITEM_MEDIC*) ( s_pItem_Medic->Search_Index_EQU( CDataPool::GetMe()->Dismantle_GetMatId(i))  );
						if(pMedic)  	
						{
							state->PushString(pMedic->szName );  
							state->PushInteger(CDataPool::GetMe()->Dismantle_GetMatNum(i));
							state->PushInteger(pMedic->nID);
						}else

						{
							state->PushString("");  
							state->PushInteger(-1);
							state->PushInteger(-1);

						}

					}


				 

		  
		}

		if (pGEquipProduct)
		{
			state->PushInteger(pGEquipProduct->nProductId);
		}
		else
		{
			state->PushInteger(-1);
		}

		return   MAX_EQUIP_PROMATL_NUM * 3 + 1;

}

INT     Lua_GetReWorkMatInfo(LuaPlus::LuaState* state)

{
	//Product_info.tab
	DBC_DEFINEHANDLE(s_pItem_Equip, DBC_EQUIP_PRODUCT_INFO);

    const _DBC_EQUIP_PRODUCT_INFO* pGEquipProduct  = NULL;

	tObject_Item *pItem = NULL;

	INT idTable = -1;

	if ( CDataPool::GetMe()->Rework_GetItemConta() == tObject_Item::IO_MYSELF_PACKET)
	{
		pItem  = 	CDataPool::GetMe()->UserBag_GetItem(  CDataPool::GetMe()->Rework_GetItemPos()  );
		if ( pItem )
		{ 
			idTable = pItem->GetIdTable();

		}

	}else if ( CDataPool::GetMe()->Rework_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP)
	{

		pItem  = 	CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP(  CDataPool::GetMe()->Rework_GetItemPos() ) );
		if ( pItem )
		{
			idTable = pItem->GetIdTable();

		}

	}
	
    
	DBC_DEFINEHANDLE(s_pItem_Material, DBC_ITEM_MEDIC);

	const _DBC_ITEM_MEDIC* pGEquipMaterial = NULL ;

	 INT id = -1;
	 INT i;
	 for ( i = 1;i<= (int)s_pItem_Equip->GetRecordsNum();i++ )
	 {
		pGEquipProduct  = (const _DBC_EQUIP_PRODUCT_INFO*)s_pItem_Equip->Search_Index_EQU(i);
		if (pGEquipProduct)
		{					
			if (pGEquipProduct->nProductId ==  idTable)
			{
			    CDataPool::GetMe()->Rework_SetIdTable( pGEquipProduct->nFormulaId );
				break;
			}

		}	
	 }
       if (  (i > (int)s_pItem_Equip->GetRecordsNum()))
       {
		   for (INT i = 0 ;i< MAX_EQUIP_RWMATL_NUM; ++i)
		   {
			   state->PushString("");  
			   state->PushInteger(-1);
			   state->PushInteger(-1);	
  
		   }

		   STRING strTemp = "该装备无法改造";
		   CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

       }else
	   {
				for (INT i = 0 ;i< MAX_EQUIP_RWMATL_NUM; ++i)
				{

					if(pGEquipProduct->szRwMat[i].nReWorkMaterial!= -1)
					{
						id = GenMaterialIndex(pGEquipProduct->szRwMat[i].nReWorkMaterial,pGEquipProduct->szRwMat[i].nReWorkMatLowestLv);
						
						CDataPool::GetMe()->Rework_SetMatId(pGEquipProduct->szMat[i].nMaterial,i);
						CDataPool::GetMe()->Rework_SetMatGrade(pGEquipProduct->szMat[i].nMatLowestLv,i);
						CDataPool::GetMe()->Rework_SetMatNum(pGEquipProduct->szMat[i].nMaterialNum,i);
						CDataPool::GetMe()->Rework_RecordMatNum();

					}else
					{
						id =-1;
					}

					pGEquipMaterial = (const _DBC_ITEM_MEDIC*)s_pItem_Material->Search_Index_EQU(id);

					if (pGEquipMaterial)
					{
						state->PushString(pGEquipMaterial->szName);    
						state->PushInteger(pGEquipProduct->szRwMat[i].nReWorkMatNum);
						state->PushInteger(pGEquipProduct->szRwMat[i].nReWorkMatLowestLv);		
					}
					else
					{
						state->PushString("");  
						state->PushInteger(-1);
						state->PushInteger(-1);		

					}


				}
		}

		if (pGEquipProduct)
		{
			state->PushInteger(pGEquipProduct->nProductId);
		}
		else
		{
			state->PushInteger(-1);
		}

 return MAX_EQUIP_RWMATL_NUM*3 + 1;

}




INT     Lua_Open_Rework_Equip(LuaPlus::LuaState* state)

{
	LuaStack args(state); 
	if (!(args[1].IsInteger()) )
		{
			KLThrow("LUA: Open_Rework_Equip Wrong Param1");
		}
	if ( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: Open_Rework_Equip Wrong Param2");
		}

		INT	cont = args[1].GetInteger(); //0 表示背包 1表示装备栏
		INT	pos = args[2].GetInteger();

		tObject_Item* pItemOld = NULL;

       if (CDataPool::GetMe()->Rework_GetItemConta() == tObject_Item::IO_MYSELF_PACKET)
       {
		  pItemOld = CDataPool::GetMe()->UserBag_GetItem(	CDataPool::GetMe()->Rework_GetItemPos( ));
		   
       }
	   else if (CDataPool::GetMe()->Rework_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP )
	   {
		 pItemOld =  CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP (	CDataPool::GetMe()->Rework_GetItemPos( )));
	   }

		if (pItemOld)
		{
			pItemOld->SetLock(false);
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}


		// 背包里物品改造
		if( cont == 0 )
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem( pos );

			if( pItem->GetItemClass() == ICLASS_EQUIP ) 
			{
               
				CDataPool::GetMe()->Rework_CleanUP();
				CDataPool::GetMe()->Rework_SetItemConta(tObject_Item::IO_MYSELF_PACKET);
				CDataPool::GetMe()->Rework_SetItemPos( pos);
				pItem->SetLock(TRUE);
				CEventSystem::GetMe()->PushEvent( GE_OPEN_EQUIP_REWORK );
				//通知背包锁定状态
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				 
			}else
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("只有装备可以改造");
				STRING strTemp = "只有装备可以改造";
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			}

		}
		// 装备里物品改造
		else if( cont == 1 )
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem( (HUMAN_EQUIP)pos );
			if( pItem )
			{
				CDataPool::GetMe()->Rework_SetItemConta(tObject_Item::IO_MYSELF_EQUIP);
				CDataPool::GetMe()->Rework_SetItemPos( pos);
				pItem->SetLock(TRUE);
				CEventSystem::GetMe()->PushEvent( GE_OPEN_EQUIP_REWORK );
				//通知装备锁定状态
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);

			}
		}	

		return 0;
}




 
//枚举配方名字和索引号  用于打造和合成装备界面
INT     Lua_EnumPackFormulation(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;  

	//Product_info.tab
	DBC_DEFINEHANDLE(s_pItem_Equip, DBC_EQUIP_PRODUCT_INFO);
	//搜索纪录
	const _DBC_EQUIP_PRODUCT_INFO* pGEquipProduct  = (const _DBC_EQUIP_PRODUCT_INFO*)s_pItem_Equip->Search_Index_EQU(args[ 1 ].GetInteger());

		 if(pGEquipProduct)  
		 {	
			state->PushInteger(pGEquipProduct->nFormulaId);
			state->PushString(pGEquipProduct->szFormulaName);
			 
		 }	 
		 else

		 {
			  state->PushInteger(-1);
			  state->PushString("");
		 }


	return 2;
}



//取得玩家 是否有相应等级的材料  用于改造                         
 
INT  Lua_GetPackForMatOnGradeReWork(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;    //材料几的等级


	if( !args[ 2 ].IsInteger() ) return 0;  //材料的等级

    

	tObject_Item  *pItem = NULL;

	if (args[ 2 ].GetInteger()<= 0 )
	{
		state->PushInteger(-1);
		return 1;
	}



	INT  Id  =  CDataPool::GetMe()->Rework_GetMatId(args[ 1 ].GetInteger());

	Id = GenMaterialIndex(Id,  args[ 2 ].GetInteger());

	INT   Num = 0;

	for (INT i = 0 ;i< MAX_BAG_SIZE; ++i)
	{
		pItem = CDataPool::GetMe()->UserBag_GetItem( i );

		if (pItem)
		{
			if ( pItem->GetIdTable() ==  Id  )
			{

				Num += pItem->GetNumber();

			}
		}		

	}


	if (Num >=  CDataPool::GetMe()->Rework_GetMatNum( args[ 1 ].GetInteger()))

		state->PushInteger(1);
	else
		state->PushInteger(0);

	return 1;


}

 //取得玩家 是否有相应等级的材料                          打造和合成装备 
INT  Lua_GetPackForMatOnGrade(LuaPlus::LuaState* state)

{

	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;    //材料几的等级

 
	if( !args[ 2 ].IsInteger() ) return 0;  //材料的等级


	tObject_Item  *pItem = NULL;



	INT  Id  =  CDataPool::GetMe()->SynEquip_GetMatId(args[ 1 ].GetInteger());

	Id = GenMaterialIndex(Id,  args[ 2 ].GetInteger());

	INT   Num = 0;

   	for (INT i = 0 ;i< MAX_BAG_SIZE; ++i)
	{
		pItem = CDataPool::GetMe()->UserBag_GetItem( i );

		if (pItem)
		{
			if ( pItem->GetIdTable() ==  Id  )
			{

				Num += pItem->GetNumber();

			}
		}		
		
	}

	state->PushInteger(Id);
	
	 if (Num >=  CDataPool::GetMe()->SynEquip_GetMatNum( args[ 1 ].GetInteger()))

		state->PushInteger(1);
	 else
		state->PushInteger(0);

	 return 2;
}
//取得玩家 相应等级的材料icon                  
INT  Lua_GetPackForMatIconOnGrade(LuaPlus::LuaState* state)

{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;    //材料几的等级


	if( !args[ 2 ].IsInteger() ) return 0;  //材料的等级

	tObject_Item  *pItem = NULL;


	INT  Id  =  CDataPool::GetMe()->SynEquip_GetMatId(args[ 1 ].GetInteger());

	Id = GenMaterialIndex(Id,  args[ 2 ].GetInteger());


	DBC_DEFINEHANDLE(s_Mat, DBC_ITEM_MEDIC);
	//搜索纪录
	const _DBC_ITEM_MEDIC* ps_Mat = (const _DBC_ITEM_MEDIC*)s_Mat->Search_Index_EQU((UINT)Id);
	if(ps_Mat)
	{
			
		state->PushString(ps_Mat->szIcon);
	}else
	{
		state->PushString("");
	}



	return 1;

}












//取得打造和合成装备 配方的材料〈按编号取得〉 
INT  Lua_GetPackFormulation(LuaPlus::LuaState* state)
{
     
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;  


	CDataPool::GetMe()->SynEquip_CleanUP();
	//Product_info.tab
	DBC_DEFINEHANDLE(s_pItem_Equip, DBC_EQUIP_PRODUCT_INFO);
	//搜索纪录
	const _DBC_EQUIP_PRODUCT_INFO* pGEquipProduct = (const _DBC_EQUIP_PRODUCT_INFO*)s_pItem_Equip->Search_Index_EQU(args[ 1 ].GetInteger() );
	INT   id = 0;
	CDataPool::GetMe()->SynEquip_SetIdTable(args[ 1 ].GetInteger());

    if ( pGEquipProduct )
	{
		
		DBC_DEFINEHANDLE(s_pItem_Material, DBC_ITEM_MEDIC);

		const _DBC_ITEM_MEDIC* pGEquipMaterial = NULL ;


		for (INT i = 0 ;i< MAX_EQUIP_PROMATL_NUM; ++i)
		{

			if(pGEquipProduct->szMat[i].nMaterial != -1)
			{
				id = GenMaterialIndex(pGEquipProduct->szMat[i].nMaterial,pGEquipProduct->szMat[i].nMatLowestLv);

				CDataPool::GetMe()->SynEquip_SetMatId(pGEquipProduct->szMat[i].nMaterial,i);
				CDataPool::GetMe()->SynEquip_SetMatGrade(pGEquipProduct->szMat[i].nMatLowestLv,i);
				CDataPool::GetMe()->SynEquip_SetMatNum(pGEquipProduct->szMat[i].nMaterialNum,i);
				CDataPool::GetMe()->SynEquip_RecordMatNum();

			}else
			{
				id = -1;
			}


			pGEquipMaterial = (const _DBC_ITEM_MEDIC*)s_pItem_Material->Search_Index_EQU(id);

			if (pGEquipMaterial)
			{
				state->PushString(pGEquipMaterial->szName);    
				state->PushInteger(pGEquipProduct->szMat[i].nMaterialNum);
				state->PushInteger(id);
				state->PushInteger(pGEquipProduct->szMat[i].nMatLowestLv);		
			}
			else
			{
				state->PushString("");  
				state->PushInteger(-1);
				state->PushInteger(-1);  
				state->PushInteger(-1);		

			}

		}


	}else
	{
		for (INT i = 0 ;i< MAX_EQUIP_PROMATL_NUM; ++i)
		{
			state->PushString("");  
			state->PushInteger(-1);
			state->PushInteger(-1);  
			state->PushInteger(-1);	
		}

	}
	 return MAX_CREATE_MATERIAL_NUM*4;

}

// 弹出确认全部修理界面 
INT Lua_PopRepairSureWnd( LuaPlus::LuaState* state) 
{
	CEventSystem::GetMe()->PushEvent(GE_OPEN_REPAIR_SURE,1);  
	return 0;
}


// 装备孔的数目。 
INT Lua_GetEquipHoleNumForAddHole( LuaPlus::LuaState* state) 

{

	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) return 0;  //  1表示背包 2表示装备栏
 
	if( !args[ 2 ].IsInteger() ) return 0;

	INT pos = args[ 1 ].GetInteger();

	INT posType = args[ 2 ].GetInteger();
          
	if (posType == 1 )
	{
		 CObject_Item_Equip * pItem = (CObject_Item_Equip * )(CDataPool::GetMe()->UserBag_GetItem(pos));
		 if ( pItem )
		 {
			    
				state->PushInteger(pItem->GetEquipHoleNum());
		 }

	}
 
   else if ( posType == 2 )
   {
	   CObject_Item_Equip * pItem = (CObject_Item_Equip * )(CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP(pos)));
	   if ( pItem )
	   {

		   state->PushInteger(pItem->GetEquipHoleNum());
	   }

   }

	return 1;

}


// 装备打孔条件。 
INT		Lua_GetAddHoleCondition( LuaPlus::LuaState* state)

{

	CObject_Item_Equip * pItem = NULL;
	if ( CDataPool::GetMe()->AddHole_GetItemConta() == tObject_Item::IO_MYSELF_PACKET )
	{
		pItem = (CObject_Item_Equip * )(  CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->AddHole_GetItemPos()) );

	}

	else if ( CDataPool::GetMe()->AddHole_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP)
	{
		pItem = (CObject_Item_Equip * )(CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP( CDataPool::GetMe()->AddHole_GetItemPos())));
	}



	if ( pItem ) 
	{
		if(pItem->GetEquipHoleNum() >= pItem->GetEquipHoleMaxNum())

		{
			STRING strTemp = "此装备的孔已达到最大数量";
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushString("");
			state->PushString("");

		}else
		{
			//打开打孔表 Equip_HoleReqInfo.tab  
			DBC_DEFINEHANDLE(s_Add_Hole, DBC_EQUIP_HOLE_INFO);
			//搜索纪录
			const _DBC_EQUIP_HOLE_INFO* p_Add_Hole = (const _DBC_EQUIP_HOLE_INFO*)s_Add_Hole->Search_Index_EQU((UINT) (pItem->GetEquipHoleNum() +1));
			if(!p_Add_Hole) return NULL;

			state->PushInteger(p_Add_Hole->nItemNeed);
			state->PushInteger(p_Add_Hole->nItemNeedNum);
			state->PushInteger(p_Add_Hole->nMoneyNeed);


			CDataPool::GetMe()->AddHole_SetNeedItemNum(p_Add_Hole->nItemNeedNum);
			CDataPool::GetMe()->AddHole_SetNeedItemId(p_Add_Hole->nItemNeed);
			CDataPool::GetMe()->AddHole_SetNeedMoney(p_Add_Hole->nMoneyNeed);


			DBC_DEFINEHANDLE(s_NeedItem, DBC_ITEM_MEDIC);
			//搜索纪录
			const _DBC_ITEM_MEDIC* p_NeedItem = (const _DBC_ITEM_MEDIC*)s_NeedItem->Search_Index_EQU((UINT)(p_Add_Hole->nItemNeed) );
			if(!p_NeedItem) return NULL;

			string name = p_NeedItem->szName;
			string IconName = p_NeedItem->szIcon;
			state->PushString( name.c_str());
			state->PushString( IconName.c_str());

		}

	}else

	{

		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushString("");
		state->PushString("");
		CDataPool::GetMe()->AddHole_CleanUP();

	}



	return 5;


}


// 打星条件信息
INT   Lua_AddStart_Equip_Info(LuaPlus::LuaState* state)  
{
	//LuaStack args(state);
	////检查参数
	//if (!(args[1].IsInteger()))       // 宝石的数目 
	//{
	//	KLThrow("LUA: PlayerPackage::AddStart_Equip_Info Wrong Param1");
	//}

	CObject_Item_Equip * pItem = NULL;
	if ( CDataPool::GetMe()->AddStar_GetItemConta() == tObject_Item::IO_MYSELF_PACKET )
	{
		pItem = (CObject_Item_Equip * )( CDataPool::GetMe()->UserBag_GetItem(  CDataPool::GetMe()->AddStar_GetItemPos() ) );

	}

	else if ( CDataPool::GetMe()->AddStar_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP)
	{
		pItem = (CObject_Item_Equip * )( CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP( CDataPool::GetMe()->AddStar_GetItemPos())));
	}



	if ( pItem ) 
	{
		// 装备打星
		//Equip_levelup.tab

		DBC_DEFINEHANDLE(s_Add_Star, DBC_EQUIP_ADD_START);
		//搜索纪录
		const _DBC_EQUIP_ADD_START* p_Add_Star = (const _DBC_EQUIP_ADD_START*)s_Add_Star->Search_Index_EQU((UINT) (pItem->GetStartLevel() +1));
		if(!p_Add_Star)
		{
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(0);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushString("");
			state->PushString("");
			state->PushInteger(-1);
			state->PushInteger(-1);

			return 22;
		}


		tObject_Item* pItemBagGem = NULL;

		INT nItemNum  = 0;
		INT nGemID = pItem->GetNeedLevel() >= 60 ? p_Add_Star->m_HighLevelGemID : p_Add_Star->m_LowLevelGemID ;
		for ( INT i =0 ;i<  MAX_BAG_SIZE ;++i)
		{
			pItemBagGem =  CDataPool::GetMe()->UserBag_GetItem(  i );
			if ( pItemBagGem)
			{
				if (  pItemBagGem->GetIdTable() == nGemID)
				{

					nItemNum += pItemBagGem->GetNumber();
				}
			}

		}

		state->PushInteger(nItemNum); //玩家的宝石数目
		state->PushInteger(p_Add_Star->nProOneGem); //成功几率
		state->PushInteger(p_Add_Star->nProTwoGem); //成功几率
		state->PushInteger(p_Add_Star->nProThrGem); //成功几率
		state->PushInteger(p_Add_Star->nProFourGem); //成功几率
		state->PushInteger(p_Add_Star->nNeedMoney);	//需要金钱
	

		CDataPool::GetMe()->AddStar_SetGemId(nGemID);  //宝石id
		CDataPool::GetMe()->AddStar_SetMoney(p_Add_Star->nNeedMoney); //所需金钱	
	 	


		string Attr =    pItem->GetEquipBaseAttrInfo(); //基本属性

		INT  nexAtt = 0;
		FLOAT IntensifyInfo =  p_Add_Star->nIntensifyInfo;

		if(	pItem->GetEquipBaseDefine().pDefineEquip)
		{

		  //if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackNear >0 )
		  {
			  nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackNear *(1 + IntensifyInfo /100.0f);
			  state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackNear < 0 ? 0 : pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackNear); 
			  state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
		  }
		// else if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackFar >0 )
		  {
			  nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackFar *(1 + IntensifyInfo /100.0f);
			  state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackFar < 0 ? 0 : pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackFar); 
			  state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
		  }
		// else  if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackMagic >0 )
		  {
			  nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackMagic *(1 + IntensifyInfo /100.0f);
			  state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackMagic <0 ? 0 : pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackMagic); 
				  state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
		  }
		 //else if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseDEefNear >0 )
		  {
			  nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseDEefNear *(1 + IntensifyInfo /100.0f);
			  state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseDEefNear < 0 ? 0 : pItem->GetEquipBaseDefine().pDefineEquip->nBaseDEefNear); 
				  state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
		  }
		//else  if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefFar >0 )
		  {
			  nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefFar *(1 + IntensifyInfo /100.0f);
			  state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefFar < 0 ? 0 : pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefFar); 
				 state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
		  }
		 //else if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefMagic >0 )
		  {
			  nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefMagic *(1 + IntensifyInfo /100.0f);
			  state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefMagic < 0 ? 0: pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefMagic); 
				 state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
		  }
		//else
		//  {
		//	  state->PushInteger(-1); 
		//	  state->PushInteger(-1); //强化后的属性
		//  }
			
		}else
		{
			state->PushInteger(-1); 
			state->PushInteger(-1); //强化后的属性
			state->PushInteger(-1); 
			state->PushInteger(-1); //强化后的属性
			state->PushInteger(-1); 
			state->PushInteger(-1); //强化后的属性
			state->PushInteger(-1); 
			state->PushInteger(-1); //强化后的属性
			state->PushInteger(-1); 
			state->PushInteger(-1); //强化后的属性
			state->PushInteger(-1); 
			state->PushInteger(-1); //强化后的属性
		}

		 CDataPool::GetMe()->AddStar_SetIntensifyInfo(nexAtt);//强化后属性

		 tActionItem* pAction = CActionSystem::GetMe()->GetAction_ItemID(pItem->GetID());
		 INT actionID = -1;
		 if (pAction)
		 {
			actionID = pAction->GetID();
		 }
		 
		

		DBC_DEFINEHANDLE(s_NeedItem, DBC_ITEM_MEDIC);
		//搜索纪录
		const _DBC_ITEM_MEDIC* p_NeedItem = (const _DBC_ITEM_MEDIC*)s_NeedItem->Search_Index_EQU((UINT)(nGemID) );
		if(!p_NeedItem) return NULL;

		string name = p_NeedItem->szName;
		string icon =p_NeedItem->szIcon;

		state->PushString(Attr.c_str());
		state->PushString( name.c_str());  //宝石名字
		state->PushInteger( nGemID );  //宝石icon
		state->PushInteger(actionID);



	}else

	{
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(0);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushString("");
		state->PushString("");
		state->PushInteger(-1);
		state->PushInteger(-1);

		CDataPool::GetMe()->AddStar_CleanUP();

	}
	return  22;


}

INT   Lua_AddStartAutoMatic_Attr_Info(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) 
	{
		KLThrow("Lua_AddStartAutoMatic_Attr_Info Wrong Arg1");
		return 0;
	}
	INT wantStarLevel = args[ 1 ].GetInteger();

	CObject_Item_Equip * pItem = NULL;
	if ( CDataPool::GetMe()->AddStar_GetItemConta() == tObject_Item::IO_MYSELF_PACKET )
	{
		pItem = (CObject_Item_Equip * )( CDataPool::GetMe()->UserBag_GetItem(  CDataPool::GetMe()->AddStar_GetItemPos() ) );

	}

	else if ( CDataPool::GetMe()->AddStar_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP)
	{
		pItem = (CObject_Item_Equip * )( CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP( CDataPool::GetMe()->AddStar_GetItemPos())));
	}
	
	if(pItem)
	{
		// 装备打星
		//Equip_levelup.tab

		DBC_DEFINEHANDLE(s_Add_Star, DBC_EQUIP_ADD_START);
		//搜索纪录
		const _DBC_EQUIP_ADD_START* p_Add_Star = (const _DBC_EQUIP_ADD_START*)s_Add_Star->Search_Index_EQU((UINT) (wantStarLevel));
		if(!p_Add_Star)
		{
			state->PushInteger(0);
			state->PushInteger(0);
			state->PushInteger(0);
			state->PushInteger(0);
			state->PushInteger(0);
			state->PushInteger(0);
			state->PushInteger(0);
			state->PushInteger(0);
			state->PushInteger(0);
			state->PushInteger(0);
			state->PushInteger(0);
			state->PushInteger(0);
			state->PushInteger(0);
			
			return 12;
		}


		tObject_Item* pItemBagGem = NULL;

		string Attr =    pItem->GetEquipBaseAttrInfo(); //基本属性

		INT  nexAtt = 0;
		FLOAT IntensifyInfo =  p_Add_Star->nIntensifyInfo;

		if(	pItem->GetEquipBaseDefine().pDefineEquip)
		{

			//if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackNear >0 )
			{
				nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackNear *(1 + IntensifyInfo /100.0f);
				state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackNear < 0 ? 0 : pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackNear); 
				state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
			}
			// else if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackFar >0 )
			{
				nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackFar *(1 + IntensifyInfo /100.0f);
				state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackFar < 0 ? 0 : pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackFar); 
				state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
			}
			// else  if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackMagic >0 )
			{
				nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackMagic *(1 + IntensifyInfo /100.0f);
				state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackMagic <0 ? 0 : pItem->GetEquipBaseDefine().pDefineEquip->nBaseAttackMagic); 
				state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
			}
			//else if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseDEefNear >0 )
			{
				nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseDEefNear *(1 + IntensifyInfo /100.0f);
				state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseDEefNear < 0 ? 0 : pItem->GetEquipBaseDefine().pDefineEquip->nBaseDEefNear); 
				state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
			}
			//else  if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefFar >0 )
			{
				nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefFar *(1 + IntensifyInfo /100.0f);
				state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefFar < 0 ? 0 : pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefFar); 
				state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
			}
			//else if(	pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefMagic >0 )
			{
				nexAtt = pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefMagic *(1 + IntensifyInfo /100.0f);
				state->PushInteger(pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefMagic < 0 ? 0: pItem->GetEquipBaseDefine().pDefineEquip->nBaseDefMagic); 
				state->PushInteger(nexAtt < 0 ? 0 : nexAtt); //强化后的属性
			}
			//else
			//  {
			//	  state->PushInteger(-1); 
			//	  state->PushInteger(-1); //强化后的属性
			//  }

		}else
		{
			state->PushInteger(0); 
			state->PushInteger(0); //强化后的属性
			state->PushInteger(0); 
			state->PushInteger(0); //强化后的属性
			state->PushInteger(0); 
			state->PushInteger(0); //强化后的属性
			state->PushInteger(0); 
			state->PushInteger(0); //强化后的属性
			state->PushInteger(0); 
			state->PushInteger(0); //强化后的属性
			state->PushInteger(0); 
			state->PushInteger(0); //强化后的属性
		}
	}
	else
	{
		state->PushInteger(0); 
		state->PushInteger(0); //强化后的属性
		state->PushInteger(0); 
		state->PushInteger(0); //强化后的属性
		state->PushInteger(0); 
		state->PushInteger(0); //强化后的属性
		state->PushInteger(0); 
		state->PushInteger(0); //强化后的属性
		state->PushInteger(0); 
		state->PushInteger(0); //强化后的属性
		state->PushInteger(0); 
		state->PushInteger(0); //强化后的属性
	}
	return 12;
}

INT   Lua_Get_Equip_StarInfo(LuaPlus::LuaState* state)
{
	CObject_Item_Equip * pItem = NULL;
	if ( CDataPool::GetMe()->AddStar_GetItemConta() == tObject_Item::IO_MYSELF_PACKET )
	{
		pItem = (CObject_Item_Equip * )( CDataPool::GetMe()->UserBag_GetItem(  CDataPool::GetMe()->AddStar_GetItemPos() ) );

	}

	else if ( CDataPool::GetMe()->AddStar_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP)
	{
		pItem = (CObject_Item_Equip * )( CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP( CDataPool::GetMe()->AddStar_GetItemPos())));
	}

	if (pItem)
	{
		state->PushInteger(pItem->GetStartLevel());
	}
	else
	{
		state->PushInteger(1);
	}
	return 1;
}

//装备炼化
INT	Lua_Equip_Refining( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) 
	{
		KLThrow("Lua_Equip_Refining Wrong Arg1");
		return 0;
	}

	if( !args[ 2 ].IsInteger() ) 
	{
		KLThrow("Lua_Equip_Refining Wrong Arg2");
		return 0;
	}

	INT ItemBagPos = args[1].GetInteger();
	INT DisBagPos = args[2].GetInteger();

	CGEquipRefining msg;
	msg.SetEquipItemPos(ItemBagPos);
	msg.SetRefiningItemPos(DisBagPos);
	CNetManager::GetMe()->SendPacket( &msg );

	return 0;
}

//装备附魔
INT	Lua_Equip_Fumo( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) 
	{
		KLThrow("Lua_Equip_Fumo Wrong Arg1");
		return 0;
	}

	if( !args[ 2 ].IsInteger() ) 
	{
		KLThrow("Lua_Equip_Fumo Wrong Arg2");
		return 0;
	}

	INT ItemBagPos = args[1].GetInteger();
	INT DisBagPos = args[2].GetInteger();

	CGEquipAdsorbMagic msg;
	msg.SetEquipPosType(ENUM_BAG);
	msg.SetEquipPos(ItemBagPos);
	msg.SetBeadPos(DisBagPos);
	CNetManager::GetMe()->SendPacket( &msg );

	return 0;
}

//装备退魔
INT	Lua_Equip_Tuimo( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() ) 
	{
		KLThrow("Lua_Equip_Tuimo Wrong Arg1");
		return 0;
	}

	if( !args[ 2 ].IsInteger() ) 
	{
		KLThrow("Lua_Equip_Tuimo Wrong Arg2");
		return 0;
	}

	INT ItemBagPos = args[1].GetInteger();
	INT DisBagPos = args[2].GetInteger();

	CGEquipCancelMagic msg;
	msg.SetEquipPos( ItemBagPos );
	msg.SetGemPos( DisBagPos );
	CNetManager::GetMe()->SendPacket( &msg );

	return 0;
}
 
// 特修条件信息
INT   Lua_SpecialRepair_Info(LuaPlus::LuaState* state)  
{
	
	CObject_Item_Equip * pItem = NULL;
	if ( CDataPool::GetMe()->SpRepair_GetItemConta() == tObject_Item::IO_MYSELF_PACKET )
	{
		pItem = (CObject_Item_Equip * )( CDataPool::GetMe()->UserBag_GetItem(  CDataPool::GetMe()->SpRepair_GetItemPos() ) );

	}

	else if ( CDataPool::GetMe()->SpRepair_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP)
	{
		pItem = (CObject_Item_Equip * )( CDataPool::GetMe()->UserEquip_GetItem( HUMAN_EQUIP( CDataPool::GetMe()->SpRepair_GetItemPos())));
	}



	if ( pItem ) 
	{
		tObject_Item* pItemBagGem = NULL;

		INT nItemNum  = 0;
		for ( INT i =0 ;i<  MAX_BAG_SIZE ;++i)
		{
			pItemBagGem =  CDataPool::GetMe()->UserBag_GetItem(  i );
			if ( pItemBagGem)
			{
				if (  pItemBagGem->GetIdTable() == CDataPool::GetMe()->SpRepair_GetNeedItemId())
				{

					nItemNum += pItemBagGem->GetNumber();
				}
			}

		}


		DBC_DEFINEHANDLE(s_NeedItem, DBC_ITEM_MEDIC);
		//搜索纪录
		const _DBC_ITEM_MEDIC* p_NeedItem = (const _DBC_ITEM_MEDIC*)s_NeedItem->Search_Index_EQU((UINT)(  CDataPool::GetMe()->SpRepair_GetNeedItemId()) );
		if(!p_NeedItem)
			
		{
			
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushInteger(-1);
			state->PushString("");
			state->PushInteger(-1);
			return 6;

		}

		string name = p_NeedItem->szName;


		state->PushInteger(nItemNum); //玩家的材料数目
		state->PushInteger(pItem->GetMaxDur());//当前耐久上限
		state->PushInteger(pItem->GetMaxDurFromTable());//耐久上限

		state->PushInteger( CDataPool::GetMe()->SpRepair_GetIntensifyInfo()	);//强化耐久值
		state->PushString( name.c_str());  //材料名字
		state->PushInteger( CDataPool::GetMe()->SpRepair_GetNeedItemId() );//强化耐久值

	}else

	{
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushInteger(-1);
		state->PushString("");
		state->PushInteger(-1);
	

		CDataPool::GetMe()->SpRepair_CleanUP();

	}
	return 6;


}

//更新装备 骑乘 换装 资料 称号 属性ui位置

INT   Lua_UpdataEquipAllPos(LuaPlus::LuaState* state) 
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() )
	{
		return 0;
	}
	if( !args[ 2 ].IsInteger() )
	{
		return 0;
	}
	INT I = args[ 1 ].GetInteger();
	INT J = args[ 2 ].GetInteger();

	CGameProcedure::s_pEventSystem->PushEvent( GE_UPDATA_EQUIP_ALL_POS,args[ 1 ].GetInteger(),args[ 2 ].GetInteger());	

	

	return 0;



}


//关闭装备 骑乘 换装 资料 称号 属性ui

INT   Lua_EquipAllCloseUI(LuaPlus::LuaState* state)
{
	
	if( CGameProcedure::s_pUISystem->IsWindowShow("QuickChangeEquip") )
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_QUICK_CHANGE_EQUIP);	
	}
	if( CGameProcedure::s_pUISystem->IsWindowShow("Ride") )
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_MOUNT_PAGE);	
	}
	if( CGameProcedure::s_pUISystem->IsWindowShow("SelfEquip") )
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_OPEN_EQUIP);	
		
		// 20100419 AddCodeBegin 点击人物窗口的关闭按钮，通知主菜单置窗口关闭标记
		CGameProcedure::s_pEventSystem->PushEvent( GE_SET_ROLE_OPENCLOSESIGN, 0 );
		// 20100419 AddCodeEnd
	}
  
	if( CGameProcedure::s_pUISystem->IsWindowShow("Selfattribute") )
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_PLAYER_ATTR_PAGE);	
	}

	if( CGameProcedure::s_pUISystem->IsWindowShow("TitleInfo") )
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_SHOW_TITLE);	
	}

	 

	return 0;
}
//open装备 骑乘 换装 资料 称号 属性ui

INT   Lua_EquipAllOpenUI(LuaPlus::LuaState* state)
{

	LuaStack args(state);
	if( !args[ 1 ].IsInteger() )
	{
		return 0;
	}


	switch(args[ 1 ].GetInteger() )

	{
	case 0:
		{
			if( CGameProcedure::s_pUISystem->IsWindowShow("TitleInfo") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_SHOW_TITLE);	
			}
			if( CGameProcedure::s_pUISystem->IsWindowShow("QuickChangeEquip") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_QUICK_CHANGE_EQUIP);	
			}
			if( CGameProcedure::s_pUISystem->IsWindowShow("Ride") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_MOUNT_PAGE);	
			}

			if( !CGameProcedure::s_pUISystem->IsWindowShow("SelfEquip") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_OPEN_EQUIP);

				// 20100419 DeleteCodeBegin GE_OPEN_EQUIP事件响应里已经发送了GE_TOGLE_PLAYER_ATTR_PAGE事件
				// CGameProcedure::s_pEventSystem->PushEvent( GE_TOGLE_PLAYER_ATTR_PAGE );
				// 20100419 DeleteCodeEnd
			}
		}
		break;

	case 1:

		{
			if( !CGameProcedure::s_pUISystem->IsWindowShow("Selfattribute") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_TOGLE_PLAYER_ATTR_PAGE);	
			}
			else
			{
				CGameProcedure::s_pUISystem->CloseWindow("Selfattribute");
			}

		}
		break;

	case 2:
		{
			if( CGameProcedure::s_pUISystem->IsWindowShow("SelfEquip") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_OPEN_EQUIP);	
			}
			if( CGameProcedure::s_pUISystem->IsWindowShow("TitleInfo") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_SHOW_TITLE);	
			}
			if( CGameProcedure::s_pUISystem->IsWindowShow("Selfattribute") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_PLAYER_ATTR_PAGE);	
			}
			if( CGameProcedure::s_pUISystem->IsWindowShow("Ride") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_MOUNT_PAGE );	
			}
			if( !CGameProcedure::s_pUISystem->IsWindowShow("QuickChangeEquip") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_QUICK_CHANGE_EQUIP);	
			}
			
		}
		break;

	case 3:
		{
			if( CGameProcedure::s_pUISystem->IsWindowShow("SelfEquip") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_OPEN_EQUIP);	
			}
			if( CGameProcedure::s_pUISystem->IsWindowShow("TitleInfo") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_SHOW_TITLE);	
			}
			if( CGameProcedure::s_pUISystem->IsWindowShow("Selfattribute") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_PLAYER_ATTR_PAGE);	
			}
			if( CGameProcedure::s_pUISystem->IsWindowShow("QuickChangeEquip") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_QUICK_CHANGE_EQUIP);	
			}
			if(! CGameProcedure::s_pUISystem->IsWindowShow("Ride") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_TOGLE_MOUNT_PAGE);	
			}
		}
		break;


	case 4:
		{

			if( CGameProcedure::s_pUISystem->IsWindowShow("SelfEquip") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_OPEN_EQUIP);	
			}
			if( CGameProcedure::s_pUISystem->IsWindowShow("Ride") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_MOUNT_PAGE);	
			}
			if( CGameProcedure::s_pUISystem->IsWindowShow("Selfattribute") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_CLOSE_PLAYER_ATTR_PAGE);	
			}
			if( CGameProcedure::s_pUISystem->IsWindowShow("QuickChangeEquip") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_QUICK_CHANGE_EQUIP);	
			}
			if(! CGameProcedure::s_pUISystem->IsWindowShow("TitleInfo") )
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_SHOW_TITLE);	
			}
		}
		break;

	case 5:
		{

		}
		break;


	}

	return 0;
}


//触发快捷键之后，判断是否可以进行一键换装
INT Lua_IsEquipExChange(LuaPlus::LuaState* state)
{
	LuaStack args(state);

	//鼠标焦点在输入窗口上
	if( CGameProcedure::s_pUISystem->HasInputFocus() )
	{
		state->PushInteger(0);
		return 1;
	}

	state->PushInteger(1);
	return 1;
}

//请求升级
INT Lua_AskLevelUp(LuaPlus::LuaState* state)
{
	CGameInterface::GetMe()->Player_AskLevelUp();
	return 0;
}



//---------------------------------------------------------------------------------------------------------
// 组队相关.

// 队员或队长的功能菜单.
INT Lua_ShowTeamFuncMenu(LuaPlus::LuaState* state)	
{
	
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() )
	{
		return 0;
	}

	// 记录当前选择的队友.
	CUIDataPool::GetMe()->m_iSelectedTeamMember = args[ 1 ].GetInteger();
	
	// 得到自己的guid
	INT MyGUID   = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();

	// 得到队长的guid
	UINT LeaderId = CUIDataPool::GetMe()->GetTeamOrGroup()->GetLeaderGUID();

	//得到鼠标位置
	POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();
	CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
	CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);
	CHAR szIndex[ 32 ]; 
	_snprintf(szIndex, 32, "%d", CUIDataPool::GetMe()->m_iSelectedTeamMember );
	std::vector< STRING > strParamInfo;
	if(MyGUID == LeaderId)
	{

		strParamInfo.push_back("Team_Leader");
		strParamInfo.push_back("-1");
		strParamInfo.push_back(szXPos);
		strParamInfo.push_back(szYPos);
		strParamInfo.push_back(szIndex);
		((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_SHOW_CONTEXMENU, strParamInfo);
		//如果自己是队长. 打开队长界面.
		//CEventSystem::GetMe()->PushEvent( GE_SHOW_TEAM_FUNC_MENU_LEADER, args[ 1 ].GetInteger() );
		//CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, "Team_Leader");
	}
	else
	{
		strParamInfo.push_back("Team_Member");
		strParamInfo.push_back("-1");
		strParamInfo.push_back(szXPos);
		strParamInfo.push_back(szYPos);
		strParamInfo.push_back(szIndex);
		((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_SHOW_CONTEXMENU, strParamInfo);
		// 如果自己是队员, 打开队员界面.
		//CEventSystem::GetMe()->PushEvent( GE_SHOW_TEAM_FUNC_MENU_MEMBER, args[ 1 ].GetInteger() );
		//CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, "Team_Member");//
	}

	return 0;
}

// 设置当前选择的队员
INT Lua_SetCurSelMember(LuaPlus::LuaState* state)	
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() )
	{
		return 0;
	}

	// 记录当前选择的队友.

	CUIDataPool::GetMe()->m_iSelectedTeamMember = args[ 1 ].GetInteger();
	return 0;
}




// 显示队伍信息对话框.
INT Lua_ShowTeamInfoDlg(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() )
	{
		return 0;
	}

	int iOpenType = 0;

	// 得到打开队友信息的类型.
	// 0 : 打开队伍信息
	// 1 : 有人申请加入队伍
	// 2 : 有人邀请你加入队伍
	iOpenType = args[ 1 ].GetInteger();

	CEventSystem::GetMe()->PushEvent(GE_TEAM_OPEN_TEAMINFO_DLG, iOpenType);
	return 0;
}

// 闪烁按钮.
INT Lua_FlashTeamButton(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if( !args[ 1 ].IsInteger() )
	{
		return 0;
	}

	int iOpenType = 0;

	// 得到打开队友信息的类型.
	// 0 : 打开队伍信息
	// 1 : 有人申请加入队伍
	// 2 : 有人邀请你加入队伍
	iOpenType = args[ 1 ].GetInteger();
	CEventSystem::GetMe()->PushEvent(GE_TEAM_NOTIFY_APPLY, iOpenType);
	return 0;
}

// 组队相关.
//--------------------------------------------------------------------------------------------------------

// 设置一个全局变量（）
INT Lua_SetGlobalInteger(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsNumber()) )
	{
		KLThrow("LUA: Lua_SetGlobalInteger Wrong Param");
	}

	if (! (args[2].IsString()) )
	{
		KLThrow("LUA: Lua_SetGlobalInteger Wrong Param");
	}

	int nNumber = args[1].GetInteger();
	std::string str = args[2].GetString();

	lua_pushnumber(state->GetCState(),nNumber);
	lua_setglobal(state->GetCState(),str.c_str());

	return 0;
}

// 获得一个全局变量
INT Lua_GetGlobalInteger(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		KLThrow("LUA: Lua_GetGlobalInteger Wrong Param");
	}
	std::string str = args[1].GetString();
	
	lua_getglobal(state->GetCState(),str.c_str());
	INT nNumber = (INT)lua_tonumber(state->GetCState(),-1);

	state->PushNumber(nNumber);
	return 1;
}

// 要装备的详细信息
INT Lua_AskEquipDetial( LuaPlus::LuaState* state)
{
	//详细装备(用于在数据池中存储物品详细属性)
	if ( CObjectManager::GetMe()->GetMySelf() != NULL )
	{
		CGAskDetailEquipList msgAskMyDetailEquip;
		msgAskMyDetailEquip.setObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		msgAskMyDetailEquip.SetAskMode(ASK_EQUIP_ALL);
		CNetManager::GetMe()->SendPacket(&msgAskMyDetailEquip);
	}

	return 0;
}

// 销毁物品
INT Lua_DiscardItem(LuaPlus::LuaState* state)
{
	//销毁物品,根据不同的物品来源，需要发送不同的销毁消息
	INT nTypeOwner	=	CDataPool::GetMe()->DisCard_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->DisCard_GetItemPos();
	switch( nTypeOwner ) 
	{
	case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(!pItem) break;
			//验证是否可以丢弃
			if(!pItem->Rule(tObject_Item::RULE_DROP))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_DROP).c_str());
				if(pItem)//add by gh for unlock 2010/05/28 不能丢弃的规则
				{
					pItem->SetLock(FALSE);
				}
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				break;
			}

			CGDiscardItem msg;
			msg.SetOpt(CGDiscardItem::FromBag);
			msg.SetBagIndex(nItemPos);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;
	case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
		{
			break;
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(!pItem) break;
			//验证是否可以丢弃
			if(!pItem->Rule(tObject_Item::RULE_DROP))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_DROP).c_str());
				if(pItem)//add by gh for unlock 2010/05/28
				{
					pItem->SetLock(FALSE);
				}
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				break;
			}

			CGDiscardEquip msg;
			msg.SetEquipPoint(nItemPos);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;
	case tObject_Item::IO_MYSELF_BANK:					// 玩家的银行
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

			if(!pItem) break;
			//验证是否可以丢弃
			if(!pItem->Rule(tObject_Item::RULE_DROP))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_DROP).c_str());
				if(pItem)//add by gh for unlock 2010/05/28
				{
					pItem->SetLock(FALSE);
				}
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				break;
			}

			CGDiscardItem msg;
			msg.SetOpt(CGDiscardItem::FromBank);
			msg.SetBagIndex(nItemPos);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;
	default:
		break;
	}

	return 0;
}

// 取消物品的锁定状态
INT Lua_DiscardItemCancelLocked( LuaPlus::LuaState* state)
{
	INT nTypeOwner	=	CDataPool::GetMe()->DisCard_GetItemConta();
	INT	nItemPos	=	CDataPool::GetMe()->DisCard_GetItemPos();

	switch( nTypeOwner ) 
	{
	// 来自玩家的背包
	case tObject_Item::IO_MYSELF_PACKET:				
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
		}
		break;

	// 来自玩家身上的装备
	case tObject_Item::IO_MYSELF_EQUIP:					
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
		}
		break;

	// 玩家的银行
	case tObject_Item::IO_MYSELF_BANK:					
		{
			tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

			if(pItem)
			{
				pItem->SetLock(FALSE);
			}
		}
		break;

	default:
		break;
	}

	CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	return 0;
}

// 播放声音
INT Lua_PlaySound(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsNumber()) )
	{
		KLThrow("LUA: Lua_SetGlobalInteger Wrong Param");
	}

	int nSoundIndex = args[1].GetInteger();
	CSoundSystemFMod::_PlayUISoundFunc(nSoundIndex);
	return 0;
}

INT	Lua_CloseAllWindow( LuaPlus::LuaState* state)
{
	CGameProcedure::s_pUISystem->CloseAllWindow();
	return 0;
}

//// 打开或关闭某个特定的窗口(_WX@7)
//INT Lua_CloseOneWindow( LuaPlus::LuaState* state)
//{
//	LuaStack args(state);
//	if ( !(args[1].IsString()) )
//	{
//		return 0;
//	}
//	if ( !(args[2].IsInteger()) )
//	{
//		return 0;
//	}
//
//	std::string name = args[1].GetString();
//	INT open = args[2].GetInteger();
//
//	if ( open == 0 )
//	{
//		CEGUI::WindowManager::getSingleton().getWindow(name.c_str())->isVisible( false );
//	}
//	else
//	{
//		CEGUI::WindowManager::getSingleton().getWindow(name.c_str())->isVisible( true );
//	}
//
//	return 0;
//}

// target窗口弹出的菜单
INT	Lua_OpenTargetMenu( LuaPlus::LuaState* state)
{
	INT iObjectId = 0;
	
	if(NULL == CObjectManager::GetMe()->GetMainTarget())
	{
		return 0;
	}

	iObjectId = CObjectManager::GetMe()->GetMainTarget()->GetServerID();
	CGameProcedure::s_pGameInterface->Object_ShowContexMenu(iObjectId);
	return 0;
}


INT Lua_GetMousePos( LuaPlus::LuaState* state)
{
	POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();
	state->PushInteger( ptMouse.x );
	state->PushInteger( ptMouse.y );
	return 2;
}

// 显示系统提示对话框。 
INT Lua_ShowSystemTipInfo( LuaPlus::LuaState* state)
{
	
	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		KLThrow("LUA: Lua_ShowSystemTipInfo Wrong Param");
	}

	const char* pInfo =  args[1].GetString();
	CEventSystem::GetMe()->PushEvent(GE_OPEN_SYSTEM_TIP_INFO_DLG, pInfo);
	return 0;
}

/*
// 打开更改二级密码对话框。 
INT Lua_OpenChangeMinorPasswordDlg( LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG);
	return 0;
}

// 打开解锁密码。 
INT Lua_OpenUnLockeMinorPasswordDlg( LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG);
	return 0;
}

// 打开设置密码界面。 
INT Lua_OpenSetMinorPasswordDlg( LuaPlus::LuaState* state)
{
	CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_SET_PASSWORD_DLG);
	return 0;
}
*/


// 设置密码。 
INT Lua_SendSetMinorPassword( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		KLThrow("LUA: Lua_SendSetMinorPassword Wrong Param");
	}

	const char* pPassword =  args[1].GetString();

	CGMinorPasswd Msg;
	
	CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
	
	pMinorPasswd->CleanUp();
	pMinorPasswd->m_Type = MREQT_SETPASSWD;		// 设置二级密码
	pMinorPasswd->m_OnePasswd.CleanUp();
	pMinorPasswd->m_OnePasswd.SetPasswd( pPassword );

	CNetManager::GetMe()->SendPacket(&Msg);

	return 0;
}

// 修改密码。 
INT Lua_SendModifyMinorPassword( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		KLThrow("LUA: Lua_SendModifyMinorPassword Wrong Param");
	}

	const char* pPassword =  args[1].GetString();

	CGMinorPasswd Msg;
	CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
	pMinorPasswd->CleanUp();
	pMinorPasswd->m_Type = MREQT_MODIFYPASSWD;		// 修改二级密码
	pMinorPasswd->m_OnePasswd.CleanUp();
	pMinorPasswd->m_OnePasswd.SetPasswd( pPassword );

	CNetManager::GetMe()->SendPacket(&Msg);

	return 0;
}

// 解锁密码。 
INT Lua_UnLockMinorPassword( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		KLThrow("LUA: Lua_UnLockMinorPassword Wrong Param");
	}

	const char* pPassword =  args[1].GetString();

	CGMinorPasswd Msg;
	CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();

	pMinorPasswd->CleanUp();
	pMinorPasswd->m_Type = MREQT_UNLOCKPASSWD;		// 二级密码解锁
	pMinorPasswd->m_OnePasswd.CleanUp();
	pMinorPasswd->m_OnePasswd.SetPasswd( pPassword );

	CNetManager::GetMe()->SendPacket(&Msg);

	return 0;
}

// 强制解锁密码。 
INT Lua_ForceUnLockMinorPassword( LuaPlus::LuaState* state)
{
	// 活力值判断
//	if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Vigor() < 100 )
//	{
//		STRING strTemp = "";
//		strTemp = NOCOLORMSGFUNC("MinorPassword_Unlock_Password_Vigor_Not_Enough");
//		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, strTemp.c_str());	
//		return 0;
//	}//


	CGMinorPasswd Msg;
	CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();

	pMinorPasswd->CleanUp();
	pMinorPasswd->m_Type = MREQT_DELETEPASSWD;		// 强制解除二级密码
	pMinorPasswd->m_OnePasswd.CleanUp();

	CNetManager::GetMe()->SendPacket(&Msg);

	return 0;
}


// 修改密码。 
INT Lua_ModifyMinorPassword( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! (args[1].IsString()) )
	{
		KLThrow("LUA: Lua_UnLockMinorPassword Wrong Param");
	}

	const char* pOldPassword =  args[1].GetString();

	if (! (args[2].IsString()) )
	{
		KLThrow("LUA: Lua_UnLockMinorPassword Wrong Param");
	}

	const char* pNewPassword =  args[2].GetString();

	CGMinorPasswd Msg;
	CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
	pMinorPasswd->CleanUp();

	if( strcmp(pNewPassword, "") == 0 )
		pMinorPasswd->m_Type = MREQT_CLEANPASSWD;		// 清空二级密码
	else
		pMinorPasswd->m_Type = MREQT_MODIFYPASSWD;		// 修改二级密码

	pMinorPasswd->m_ModifyPasswd.CleanUp();

	// 设置要修改的密码
	pMinorPasswd->m_ModifyPasswd.SetOldPasswd(pOldPassword);
	pMinorPasswd->m_ModifyPasswd.SetNewPasswd(pNewPassword);

	CNetManager::GetMe()->SendPacket(&Msg);

	return 0;
}

INT Lua_GetCurrentTime( LuaPlus::LuaState* state)
{
	SYSTEMTIME systime;
	::GetLocalTime( &systime );
	
	state->PushInteger( int( systime.wHour ) );
	state->PushInteger( int( systime.wMinute ) );
	state->PushInteger( int( systime.wMilliseconds ) );
	return 3;
}

// 安全时间
INT Lua_SetPasswordProtectTime( LuaPlus::LuaState* state)
{
	LuaStack args(state);

	if( !(args[1].IsInteger()) )
	{
		KLThrow("LUA: Lua_SetProtectTime 1 Wrong Param");
	}
	if( !(args[2].IsInteger()) )
	{
		KLThrow("LUA: Lua_SetProtectTime 2 Wrong Param");
	}

	INT nHour =  args[1].GetInteger();
	INT nMinute =  args[2].GetInteger();


	CGMinorPasswd Msg;
	
	CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
	
	pMinorPasswd->CleanUp();

	pMinorPasswd->m_Type = MREQT_SETPROTECTTIME;		// 设置上线保护时间
	pMinorPasswd->m_cTime = nMinute;

	CNetManager::GetMe()->SendPacket( &Msg );

	return 0;
}


/*
// 检测输入的密码
INT Lua_CheckInputPassword( LuaPlus::LuaState* state )
{
	LuaStack args(state);

	if(! (args[1].IsString()) )
	{
		KLThrow("LUA: Lua_SetPasswordProtectTime 1 Wrong Param");
	}
	if( !(args[2].IsInteger()) )
	{
		KLThrow("LUA: Lua_SetPasswordProtectTime 2 Wrong Param");
	}

	const char* pInputPassword	=  args[1].GetString();
	INT nType					=  args[2].GetInteger();

	CDataPool::GetMe()->Rich_SetInputPassword( pInputPassword );

	// 根据不同的类型处理不同的事件
	switch( nType )
	{
	case 4:
		{
			CEventSystem::GetMe()->PushEvent( GE_INPUT_PASSWD_COMMAND );
			break;
		}
	};

	return 0;
}
*/

/*
// 检测密码输入状态
INT Lua_SetInputPasswordState( LuaPlus::LuaState* state )
{
	LuaStack args(state);

	if( !(args[1].IsInteger()) )
	{
		KLThrow("LUA: Lua_SetInputPasswordState 1 Wrong Param");
	}
	if( !(args[2].IsInteger()) )
	{
		KLThrow("LUA: Lua_SetInputPasswordState 2 Wrong Param");
	}

	INT nType	=  args[1].GetInteger();
	INT nState  =  args[2].GetInteger();

	CDataPool::GetMe()->Rich_SetInputPasswdState( nType, (BOOL)nState );

	return 0;
}
INT Lua_GetInputPasswordState( LuaPlus::LuaState* state )
{
	LuaStack args(state);
	if( !(args[1].IsInteger()) )
	{
		KLThrow("LUA: Lua_GetInputPasswordState 1 Wrong Param");
	}

	INT nType =  args[1].GetInteger();

	BOOL bState = CDataPool::GetMe()->Rich_GetInputPasswdState( nType );

	state->PushInteger( (INT)bState );

	return 1;
}
*/


INT Lua_GetCurMousePos( LuaPlus::LuaState* state)
{
	POINT ptMouse = CGameProcedure::s_pInputSystem->MouseGetPos();

	state->PushInteger(ptMouse.x);
	state->PushInteger(ptMouse.y);

	return 2;
}

//// 播放音乐。参数是音乐的ID  
//INT Lua_PlayMusic( LuaPlus::LuaState* state)
//{
//	LuaStack args(state);
//	if (! (args[1].IsString()) )
//	{
//		KLThrow("LUA: Lua_PlayMusic Wrong Param");
//	}
//
//	INT iId =  args[1].GetInteger();
//
//
//}
//
//
//// 停止音乐。参数是音乐的句柄  
//INT Lua_StopMusic( LuaPlus::LuaState* state)
//{
//
//}//


// 显示帮会邀请确认对话框.
INT Lua_ShowGuildInfoDlg(LuaPlus::LuaState* state)
{

	STRING szGuildName  = CDataPool::GetMe()->Guild_GetInviteGuildName( );		// 帮会名称
	STRING szInviteName = CDataPool::GetMe()->Guild_GetInviteGuildObjName( );	// 邀请者名称

	CEventSystem::GetMe()->PushEvent( GE_GUILD_INTIVE_CONFIRM, szGuildName.c_str(), szInviteName.c_str() );

	return 0;
}

// 向服务器请求排行榜
INT Lua_SendTopList(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! args[1].IsInteger() )
	{
		KLThrow("LUA: Lua_SendTopList Wrong Param");
	}

	//enum TOP_LIST_TYPE
	//{
	//	LEVEL_TOP_LIST = 0,			//等级排行榜
	//	MONEY_TOP_LIST ,			//财富排行榜
	//	REPUTE_TOP_LIST,			//声望排行榜
	//	PET_TOP_LIST,				//宠物排行榜
	//	ECTYPE_TOP_LIST,			//副本排行榜
	//	ERRANTRY_TOP_LIST,			//侠义排行榜
	//	CRIME_TOP_LIST,				//罪恶排行榜

	//	TOP_LIST_NUM,				//排行榜总数

	//};

	UCHAR type = (UCHAR)args[1].GetInteger();
	CGTopList msg;
	msg.SetType(type);
	CNetManager::GetMe()->SendPacket( &msg );

	return 0;
}

// 得到俳榜的名称
//INT Lua_GetTopListName(LuaPlus::LuaState* state)
//{
//	const tDataBase* pDBC = CDataBaseSystem::GetMe()->GetDataBase( DBC_TOP_LIST );
//	if( pDBC == NULL )
//		return 0;
//
//	DBC::DBCFile* pFile = (DBC::DBCFile*)pDBC->GetDBCFile();
//	if( pFile == NULL )
//		return 0;
//
//	enum TopListEnum
//	{
//		TopName		= 1,	// 排行榜名
//		TopTypeID,			// 所属类ID
//		TopType,			// 所属类
//	};
//
//	// 记录数
//	INT iRecordCnt	 =	pFile->GetRecordsNum();
//
//	// 字段数
//	INT iColumnCnt =	pFile->GetFieldsNum();
//
//	INT index = 0;
//	INT iLastID = 0;
//	for(INT i = 0; i < iRecordCnt; i++)
//	{
//		STRING strName = pFile->Search_Posistion(i,TopName)->pString;
//		INT	   iTypeID = pFile->Search_Posistion(i,TopTypeID)->iValue;
//		STRING strType = pFile->Search_Posistion(i,TopType)->pString;
//
//		if( iLastID != iTypeID )
//		{
//			index = 0;
//		}
//		iLastID = iTypeID;
//		index++;
//
//		// 向ui发送消息
//		std::vector< STRING > vParam;
//		CHAR szTemp[10];
//		// 填充类型，跟ui脚本相关
//		vParam.push_back("0");
//		// 排行榜名
//		vParam.push_back(strName);
//
//		// 所属类ID
//		_snprintf(szTemp, 10, "%d", iTypeID);
//		vParam.push_back(szTemp);
//
//		// 所属类
//		vParam.push_back(strType);
//
//		// 填充的Listbox的id
//		_snprintf(szTemp, 10, "%d", index);
//		vParam.push_back(szTemp);
//
//		CEventSystem::GetMe()->PushEvent( GE_UPDATE_TOP_LIST, vParam );	
//	}
//	CEventSystem::GetMe()->PushEvent( GE_UPDATE_TOP_LIST, "2" );	
//
//	return 0;
//}


INT Lua_PushEvent(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! args[1].IsString() )
	{
		KLThrow("LUA: Lua_PushEvent Wrong Param");
	}

	STRING eventName(args[1].GetString());

	if ( args[2].IsString() )
	{
		if ( args[3].IsString() )
		{
			if( args[4].IsInteger() )
			{
				if( args[5].IsInteger() )
					CEventSystem::GetMe()->PushEvent( eventName, args[2].GetString(), args[3].GetString(), 
													  args[4].GetInteger(), args[5].GetInteger() );
				else
					CEventSystem::GetMe()->PushEvent( eventName, args[2].GetString(), args[3].GetString(), args[4].GetInteger() );
			}
			else
				CEventSystem::GetMe()->PushEvent( eventName, args[2].GetString(), args[3].GetString() );
		}
		else
			CEventSystem::GetMe()->PushEvent( eventName, args[2].GetString()  );
	}
	else
	{
		CEventSystem::GetMe()->PushEvent( eventName  );
	}

	return 0;
}

#define MAX_TOOLTIP_ITEM_SIZE 2

CObject_Item* g_ItemForTips[MAX_TOOLTIP_ITEM_SIZE] = {0};

INT  CleanItemForTip(LuaPlus::LuaState* state)
{
	for(INT i=0; i<MAX_TOOLTIP_ITEM_SIZE; i++)
	{
		CObject_Item::DestroyItem(g_ItemForTips[i] );
		g_ItemForTips[i] = NULL;
	}
	return 0;
}

INT  CreatItemForTip(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! args[1].IsInteger() )
	{
		KLThrow("LUA: CreatItemForTip Wrong Param1");
	}
	if (! args[2].IsInteger() )
	{
		KLThrow("LUA: CreatItemForTip Wrong Param2");
	}
	INT Pos= args[2].GetInteger();

	if( Pos > MAX_TOOLTIP_ITEM_SIZE || Pos < 0 ) return 0;

	if( g_ItemForTips[Pos] )
	{
		CObject_Item::DestroyItem(g_ItemForTips[Pos] );
	}

	g_ItemForTips[Pos] = CObject_Item::NewItem(args[1].GetInteger());

	state->PushInteger( CActionSystem::GetMe()->CreateObjectItemByTableId( g_ItemForTips[Pos] ) );

	return 1;
}

INT Lua_OpenWindow( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! args[1].IsString() )
	{
		KLThrow("LUA: Lua_OpenWindow Wrong Param1");
	}
	std::vector<STRING> vParam;
	vParam.push_back(args[1].GetString());
	if (args[2].IsString())
	{
		vParam.push_back(args[2].GetString());
	}
	else
	{
		vParam.push_back("");
	}
	CGameProcedure::s_pUISystem->OpenWindow(args[1].GetString());
	CEventSystem::GetMe()->PushEvent(GE_OPEN_WINDOW, vParam);
	return 0;
}
INT Lua_CloseWindow( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! args[1].IsString() )
	{
		KLThrow("LUA: Lua_OpenWindow Wrong Param1");
	}
	std::vector<STRING> vParam;
	vParam.push_back(args[1].GetString());
	if (args[2].IsString())
	{
		vParam.push_back(args[2].GetString());
	}
	else
	{
		vParam.push_back("");
	}
	CGameProcedure::s_pUISystem->CloseWindow(args[1].GetString());
	CEventSystem::GetMe()->PushEvent(GE_CLOSE_WINDOW, vParam);
	return 0;
}
INT Lua_ToggleWindow( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! args[1].IsString() )
	{
		KLThrow("LUA: Lua_OpenWindow Wrong Param1");
	}
	std::vector<STRING> vParam;
	vParam.push_back(args[1].GetString());
	if (args[2].IsString())
	{
		vParam.push_back(args[2].GetString());
	}
	else
	{
		vParam.push_back("");
	}
	CGameProcedure::s_pUISystem->ToggleWindow(args[1].GetString());
	CEventSystem::GetMe()->PushEvent(GE_TOGGLE_WINDOW, vParam);
	return 0;
}


INT Lua_SplitUnifiedString( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! args[1].IsString() )
	{
		KLThrow("LUA: Lua_SplitUnifiedString Wrong Param1");
	}
	FLOAT rx,ry,ax,ay;
	sscanf(args[1].GetString() , " {{%f,%f},{%f,%f}}", &rx,&ax, &ry,&ay);

	state->PushNumber(rx);
	state->PushNumber(ax);
	state->PushNumber(ry);
	state->PushNumber(ay);

	return 4;
}

INT Lua_SaveAutoAttributePoint( LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if (! args[1].IsInteger() )
	{
		KLThrow("LUA: Lua_SaveAutoAttributePoint Wrong Param1");
	}
	if (! args[2].IsInteger() )
	{
		KLThrow("LUA: Lua_SaveAutoAttributePoint Wrong Param2");
	}
	if (! args[3].IsInteger() )
	{
		KLThrow("LUA: Lua_SaveAutoAttributePoint Wrong Param3");
	}
	if (! args[4].IsInteger() )
	{
		KLThrow("LUA: Lua_SaveAutoAttributePoint Wrong Param4");
	}
	INT val = 0;
	val = args[1].GetInteger() & 0x000000FF;
	val += (args[2].GetInteger()) << 8;
	val += (args[3].GetInteger()) << 16;
	val += (args[4].GetInteger()) << 24;

	//发送保存设置消息
	CGModifySetting msg;
	_OWN_SETTING* pSend_Value = msg.GetValue();
	msg.SetType(SETTING_TYPE_ATTRI_AUTO_POINT);
	pSend_Value->m_SettingData = val;
	pSend_Value->m_SettingType = 0;

	CNetManager::GetMe()->SendPacket(&msg);

	CDataPool::GetMe()->SetAttriAutoPoint(val);
	
	return 0;
}

INT Lua_GetAutoAttributePoint( LuaPlus::LuaState* state)
{
	for (INT i=1; i<5; i++)
	{
		state->PushInteger(CDataPool::GetMe()->GetAttriAutoPoint(i));
	}
	return 4;
}

INT Lua_IB_Shop( LuaPlus::LuaState* state)
{
	DBC_DEFINEHANDLE(pIBShopDBC,   DBC_IB_SHOP);	//  RMB商城

	_DBC_IB_SHOP*   pIBShopDef   = NULL;
	//清空原有商品列表
	CDataPool::GetMe()->IBShop_Clear();

	for (INT i=0; i<pIBShopDBC->GetRecordsNum(); ++i)
	{
		pIBShopDef = (_DBC_IB_SHOP*)pIBShopDBC->Search_First_Column_Equ( 0, i+1);

		//创建商品实例
		tObject_Item* pItem = CObject_Item::NewItem(pIBShopDef->nItemID);
		if(!pItem) continue;

		//设置该物品在货架上的位置
		pItem->SetPosIndex(i);
		CDataPool::GetMe()->IBShop_SetItemIsNewSell(i, pIBShopDef->bIsNewSell);
		//价格放入数据池
		if (pIBShopDef->bIsCutPrice > 0)
		{
			CDataPool::GetMe()->IBShop_SetItemPrice(i, (UINT)(pIBShopDef->nPrice *  pIBShopDef->CutPriceRate/100));
		} 
		else
		{
			CDataPool::GetMe()->IBShop_SetItemPrice(i, pIBShopDef->nPrice);
		} 

		//加入数据池
		CDataPool::GetMe()->IBShop_SetItem(i, pItem);
	}

	//更新到ActionSystem
	if(pIBShopDBC->GetRecordsNum() > 0) CActionSystem::GetMe()->IBShop_Update();

	//通知UI
	//CDataPool::GetMe()->Booth_Open();
	
	CEventSystem::GetMe()->PushEvent(GE_SHOW_ALL_IB_ITEM);
	return 0;
}







//得到IB商城内的操作对象
INT	IBShop::Lua_EnumActionFoIBShop(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if ( !(args[2].IsInteger()) )
	{
		KLThrow("LUA: Lua_EnumActionFoIBShop Wrong Param1");
	}

	if ( !(args[3].IsInteger()) )
	{
		KLThrow("LUA: Lua_EnumActionFoIBShop Wrong Param2");
	}

	if ( !(args[4].IsInteger()) )
	{
		KLThrow("LUA: Lua_EnumActionFoIBShop Wrong Param3");
	}


	INT Index = args[2].GetInteger();
	INT IBShopType = args[3].GetInteger();
	INT IBShopType2 = args[4].GetInteger();
	INT IBNum = CActionSystem::GetMe()->GetActionNum("ib_shopitem", -1) ;			//商城内的物品总数

	INT num = 0;
	for ( INT i=0; i<IBNum; ++i )
	{
		tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(i, "ib_shopitem");
		if( !IsActionFilter(pActionItem, IBShopType, IBShopType2) ) continue;

		if ( num++ == Index)
		{
			LuaObject objReturn = state->BoxPointer(pActionItem);
			objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
			objReturn.PushStack();
			return 1;
		}
	}

	LuaObject objReturn = state->BoxPointer(&(CActionItem::s_InvalidAction));
	objReturn.SetMetaTable(*CActionItem::s_pMetaTable);
	objReturn.PushStack();
	return 1;
}


INT IBShop::Lua_GetIBShopAccordNum(LuaPlus::LuaState* state)
{
	LuaStack args(state);
	if ( !(args[2].IsInteger()) )
	{
		KLThrow("LUA: Lua_GetIBShopAccordNum Wrong Param1");
	}

	if ( !(args[3].IsInteger()) )
	{
		KLThrow("LUA: Lua_GetIBShopAccordNum Wrong Param2");
	}
	
	INT type = args[2].GetInteger();
	INT type2 = args[3].GetInteger();

	INT num = 0;
	INT IBNum = CActionSystem::GetMe()->GetActionNum("ib_shopitem", -1) ;

	for ( INT i=0; i<IBNum; ++i )
	{
		tActionItem* pActionItem = CActionSystem::GetMe()->EnumAction(i, "ib_shopitem");
		if ( IsActionFilter(pActionItem,type,type2))
			num++;
	}

	state->PushInteger(num);
	return 1;
}



BOOL	IBShop::IsActionFilter( tActionItem* pActionItem, INT type, INT type2 )
{
	if( !pActionItem ) return FALSE;

	INT pos = pActionItem->GetPosIndex();

	DBC_DEFINEHANDLE(pIBShopDBC, DBC_IB_SHOP);	//  IB商城
	_DBC_IB_SHOP*	pIBShopDef = (_DBC_IB_SHOP*)pIBShopDBC->Search_First_Column_Equ( 0, pos+1);

	bool filter = false;

	switch ( type )
	{
	case 0:
		{
			filter = true;
		}
		break;

	case  1:
		{
			if( pIBShopDef->bIsNewSell == 1 )
				filter = true;
		}
		break;

	case  2:
		{
			if( pIBShopDef->bIsHotSell == 1 )
				filter = true;
		}
		break;

	case  3:
		{
			if( pIBShopDef->bIsCutPrice == 1 )
				filter = true;
		}
		break;
	}

	if( !filter ) return filter;
	
	if( type2 == 0 )
		return filter;

	switch( type2 )
	{
	case 1:
		{
			if( pIBShopDef->bIsPetShop == 1 )
				filter = true;
			else
				filter = false;
		}
		break;

	case 2:
		{
			if( pIBShopDef->bIsEquipShop == 1 )
				filter = true;
			else
				filter = false;
		}
		break;

	case 3:
		{
			if( pIBShopDef->bIsMedicShop == 1 )
				filter = true;
			else
				filter = false;
		}
		break;

	case 4:
		{
			if( pIBShopDef->bIsSpecialShop == 1 )
				filter = true;
			else
				filter = false;
		}
		break;

	case 5:
		{
			if( pIBShopDef->bIsColligateShop == 1 )
				filter = true;
			else
				filter = false;
		}
		break;
	}

	return filter;
	
}


}//namespace SCRIPT_SANDBOX
