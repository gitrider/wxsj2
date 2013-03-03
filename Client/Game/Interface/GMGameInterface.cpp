
/**	GMGameInterface.cpp
 *	
 *	功能：	
 *	
 *	修改记录：
 *			080303	在 CGameInterface::Object_ShowContexMenu 中增加处理 帮会 相关的处理
 *			080327	修改 Title_UpdateCurTitle 中的CGCharUpdateCurTitle消息关于设置称号的部分
 *			
 */

#include "StdAfx.h"
#include "GMGameInterface.h"
#include "Procedure\GameProcedure.h"
#include "Procedure\GamePro_main.h"
#include "world\worldmanager.h"
#include "network\netmanager.h"
#include "object\Logic\Object.h"
#include "object\Manager\Objectmanager.h"
#include "object\Logic\character\Obj_playermyself.h"
#include "global.h"
#include "GIinputsystem.h"
#include "GIUtil.h"
#include "GIworldsystem.h"
#include "GIObjectManager.h"
#include "GISound.h"
#include "packets\cgpickboxitem.h"
#include "packets\CGPickDropItem.h"
#include "packets\cguseequip.h"
#include "packets\CGManipulateMount.h"  //  [7/8/2010 陈军龙]
#include "object\ObjectCommand.h"
#include "object\ObjectCommanddef.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\GMDataPool.h"
#include "Object\Logic\Character\AI\GMAICommandDef.h"
#include "Object\Logic\Character\AI\GMAI_Base.h"
#include "CGCommand.h"
#include "World\Path.h"
#include "Object\Logic\ProjTex\Obj_ProjTex.h"
#include "Object\Logic\Item\Obj_Item_Equip.h"
#include "CGUseGem.h"
#include "CGUseItem.h"
#include "CGDiscardItem.h"
#include "CGDiscardEquip.h"
#include "CGUnEquip.h"
#include "CGReqResetAttr.h"
#include "CGCommand.h"
#include "CGReqLevelUp.h"
#include "Action\GMActionSystem.h"
#include "DataPool\DetailAttrib_Player.h"
#include "cgchat.h"
#include "DBC\GMDataBase.h"
#include "World\WorldManager.h"
#include "Event\GMEventSystem.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Input\GMInputSystem.h"
#include "CampAndStand.h"
#include "DB_Struct.h"

//商店
#include "CGShopBuy.h"
#include "CGShopClose.h"
#include "CGShopSell.h"
#include "CGShopRepair.h"
//称号
#include "CGCharUpdateCurTitle.h"
#include "GMInterface_Script_SystemSetup.h"
//stall
#include "CGStallOpen.h"
#include "CGBBSApply.h"

#include "Action\GMActionSystem_Item.h"
#include "Object\Logic\Item\Obj_Item_Medicine.h"
#include "Action\GMActionSystem_MouseCmd.h"
#include "CGEquipAddHole.h"
#include "CGEquipBind.h"
#include "CGSaveEquipSuit.h"
#include "CGItemCreate.h"
#include "CGEquipRemould.h"
#include "GMInterface_Script_Talk.h"    //  [7/20/2010 陈军龙]

CGameInterface* CGameInterface::s_pMe = NULL;

KL_IMPLEMENT_DYNAMIC(CGameInterface, GETCLASS(tGameInterfaceBase));


CGameInterface::CGameInterface()
{
	s_pMe = this;
}

CGameInterface::~CGameInterface()
{
	s_pMe = 0;
}


VOID CGameInterface::Initial(VOID*)
{
	Make_Count = -1;
	Make_Prescription = -1;
	//系统设置接口挂接变量控制系统
	CEventSystem::GetMe()->RegisterEventHandle("VARIABLE_CHANGED",	SCRIPT_SANDBOX::SystemSetup::_OnVariableChangedEvent);
}

VOID CGameInterface::Tick(VOID)
{
}

VOID CGameInterface::Release(VOID)
{
}

VOID CGameInterface::Object_SelectAsMainTarget(INT idObj, int iType)
{
	CObjectManager::GetMe()->SetMainTarget(idObj, (tObjectSystem::DESTROY_MAIN_TARGET_TYPE)iType);
}

VOID CGameInterface::Object_ShowContexMenu(INT idObj,BOOL showMyself)
{
	CObject* pObject = (CObject*)CObjectManager::GetMe()->FindServerObject(idObj);
	if(!pObject) return;

	CHAR szObjId[32]; _snprintf(szObjId, 32, "%d", pObject->GetID());

	// 得到鼠标位置
	POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();

	CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
	CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);
	
	std::vector< STRING > vParam;


	// 根据不同物体产生不同右键事件

	// 主角自身
	if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_PlayerMySelf)))
	{
		if(showMyself)
		{
			// 如果自己已经在队伍中了
			if( CUIDataPool::GetMe()->IsInTeam())
			{
				vParam.push_back("player_in_team");
				vParam.push_back(szObjId);
				vParam.push_back(szXPos);
				vParam.push_back(szYPos);
				CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
				
				return;
			}

			// 自己没有组队
			vParam.push_back("player");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
		}
	}
	// 其他玩家
	else if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_PlayerOther)))
	{
		CObject_PlayerOther* pOtherObject = (CObject_PlayerOther*)pObject;


		// 是否有帮会
		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			
			// 根据策划的需求再进行修改： 是否判断其他玩家已经入帮了	GetCharacterData()->Get_Guild


			INT tt = CDataPool::GetMe()->Guild_GetCurPosition();

			// 自己是帮主或副帮主， 点击其他玩家
			if( ( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN ) ||
				( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_ASS_CHIEFTAIN ))
			{

				// 自己有队伍， 对方有队伍
				if( (pOtherObject->GetCharacterData()->Get_HaveTeamFlag())&&( CUIDataPool::GetMe()->IsInTeam()) )
				{
					vParam.push_back("guild_other_team_member");
					vParam.push_back(szObjId);
					vParam.push_back(szXPos);
					vParam.push_back(szYPos);
					CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
					
					return;
				}
				// 其他玩家没有队伍
				if((!pOtherObject->GetCharacterData()->Get_HaveTeamFlag()))
				{
					vParam.push_back("guild_other_not_team_member");
					vParam.push_back(szObjId);
					vParam.push_back(szXPos);
					vParam.push_back(szYPos);
					CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
					
					return;
				}
				// 自己没队伍, 其他玩家有队伍
				if((pOtherObject->GetCharacterData()->Get_HaveTeamFlag())&&( !CUIDataPool::GetMe()->IsInTeam()))
				{
					vParam.push_back("guild_other_team_member_me_not_teamer");
					vParam.push_back(szObjId);
					vParam.push_back(szXPos);
					vParam.push_back(szYPos);
					CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
					
					return;
				}

				vParam.push_back("guild_other_player");
				vParam.push_back(szObjId);
				vParam.push_back(szXPos);
				vParam.push_back(szYPos);
				CEventSystem::GetMe()->PushEvent( GE_SHOW_CONTEXMENU, vParam );

				return ;
			}

			// 自己有帮会（帮主）， 对方已加帮
					// 自己有队伍， 对方有队伍
					// 其他玩家没有队伍
					// 自己没队伍, 其他玩家有队伍

			// 自己有帮会（帮主）， 对方没加帮
			
		}


		// 自己有队伍, 点击的其他玩家也有队伍
		if((pOtherObject->GetCharacterData()->Get_HaveTeamFlag())&&( CUIDataPool::GetMe()->IsInTeam()))
		{
			vParam.push_back("other_team_member");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
			
			return;
		}

		// 点击其他玩家没有队伍
		if((!pOtherObject->GetCharacterData()->Get_HaveTeamFlag()))
		{
			vParam.push_back("other_not_team_member");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
			
			return;
		}

		
		// 自己没队伍, 点击其他玩家有队伍
		if((pOtherObject->GetCharacterData()->Get_HaveTeamFlag())&&( !CUIDataPool::GetMe()->IsInTeam()))
		{
			vParam.push_back("other_team_member_me_not_teamer");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
			
			return;
		}


		vParam.push_back("other_player");
		vParam.push_back(szObjId);
		vParam.push_back(szXPos);
		vParam.push_back(szYPos);
		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
	}
	// NPC 
	else if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_PlayerNPC)))
	{
		//PET_GUID_t pg = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID();

		// 自己的宠物
		/*if(!(pg.IsNull()) && CDataPool::GetMe()->Pet_GetPet(pg) && CDataPool::GetMe()->Pet_GetPet(pg)->m_idServer == pObject->GetServerID())
		{
			vParam.push_back("my_pet");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
			
			return;
		}*/

		//CObject_PlayerNPC* pNpcObject = (CObject_PlayerNPC*)pObject;
		// 其他宠物
		//if(pNpcObject->GetNpcType() == NPC_TYPE_PET)
		//{
		//	if(INVALID_UID != pNpcObject->GetCharacterData()->Get_OwnerID())
		//	{
		//		// 有归属的宠物才显示菜单
		//		vParam.push_back("other_pet");
		//		vParam.push_back(szObjId);
		//		vParam.push_back(szXPos);
		//		vParam.push_back(szYPos);
		//		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
		//	}

		//	return;
		//}

		vParam.push_back("npc");
		vParam.push_back(szObjId);
		vParam.push_back(szXPos);
		vParam.push_back(szYPos);
		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
	}
	else 
		return;

}

VOID CGameInterface::Player_UseSkill(INT idSkill)
{
	// 骑乘状态下不能使用技能   [7/8/2010 陈军龙]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// 显示到聊天框
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// 判断是否为合法的瞬发技能
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) 
		return;

	// 取得技能数据
	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) 
		return;

	CAI_Base* pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	// 发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= INVALID_UID;
	cmdTemp.m_afParam[2]	= -1.f;
	cmdTemp.m_afParam[3]	= -1.f;
	cmdTemp.m_afParam[4]	= -1.f;
	cmdTemp.m_auParam[5]	= INVALID_GUID;
	pMySelfAI->PushCommand( &cmdTemp );

	// 发送事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseSkill(INT idSkill, INT idObj)
{
	// 骑乘状态下不能使用技能   [7/8/2010 陈军龙]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// 显示到聊天框
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// 判断是否为合法的目标技能
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) 
		return;

	// 取得技能数据
	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) 
		return;

	// 检测目标是否合法
	
//	if(!(pSkill->IsValidTarget(idObj))) return;
	

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	// 发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= idObj;
	cmdTemp.m_afParam[2]	= -1.f;
	cmdTemp.m_afParam[3]	= -1.f;
	cmdTemp.m_afParam[4]	= -1.f;
	cmdTemp.m_auParam[5]	= INVALID_GUID;

	pMySelfAI->PushCommand( &cmdTemp );

	// 发送事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseSkill(INT idSkill, const fVector2& fvPos)
{
	// 骑乘状态下不能使用技能   [7/8/2010 陈军龙]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// 显示到聊天框
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// 判断是否为合法的范围技能
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	// 发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= INVALID_UID;
	cmdTemp.m_afParam[2]	= fvPos.x;
	cmdTemp.m_afParam[3]	= fvPos.y;
	cmdTemp.m_afParam[4]	= 0.f;
	cmdTemp.m_auParam[5]	= INVALID_GUID;

	pMySelfAI->PushCommand( &cmdTemp );

	// 发送事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseSkill(INT idSkill, FLOAT fDir)
{
	// 骑乘状态下不能使用技能   [7/8/2010 陈军龙]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// 显示到聊天框
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// 判断是否为合法的方向技能
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	// 改变人物朝向
	CObjectManager::GetMe()->GetMySelf()->SetFaceDir(fDir);

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	// 发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= INVALID_UID;
	cmdTemp.m_afParam[2]	= -1.0f;
	cmdTemp.m_afParam[3]	= -1.0f;
	cmdTemp.m_afParam[4]	= fDir;
//	cmdTemp.m_afParam[4]	= -1.f;
	cmdTemp.m_auParam[5]	= INVALID_GUID;

	pMySelfAI->PushCommand( &cmdTemp );

	// 发送事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseSkill( INT idSkill, const fVector2& fvPos, const GUID_t& fDir )
{
	// 骑乘状态下不能使用技能   [7/8/2010 陈军龙]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// 显示到聊天框
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// 判断是否为合法的方向技能
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	// 改变人物朝向
//	CObjectManager::GetMe()->GetMySelf()->SetFaceDir(fDir);

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	// 发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= INVALID_UID;
	cmdTemp.m_afParam[2]	= fvPos.x;
	cmdTemp.m_afParam[3]	= fvPos.y;
	cmdTemp.m_afParam[4]	= fDir;
//	cmdTemp.m_afParam[4]	= -1.f;
	cmdTemp.m_auParam[5]	= INVALID_GUID;

	pMySelfAI->PushCommand( &cmdTemp );

	// 发送事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}


//使用技能(对某个队友)
VOID CGameInterface::Player_UseSkill( INT idSkill, const GUID_t& guid)
{
	// 骑乘状态下不能使用技能   [7/8/2010 陈军龙]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// 显示到聊天框
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// 判断是否为合法的目标技能
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	// 取得技能数据
	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	// 发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= -1;
	cmdTemp.m_afParam[2]	= -1.f;
	cmdTemp.m_afParam[3]	= -1.f;
	cmdTemp.m_afParam[4]	= -1.f;
	cmdTemp.m_auParam[5]	= guid;

	pMySelfAI->PushCommand( &cmdTemp );

	// 发送事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseLifeAbility(INT idPrescription,INT nMakeCount)
//VOID CGameInterface::Player_UseLifeAbility(INT idPrescription)
{
	if( nMakeCount < 1 )
		return;
	Make_Count = nMakeCount;
	Make_Prescription = idPrescription;

	Player_UseLifeAbility(idPrescription);
	return;
}

VOID CGameInterface::Player_UseLifeAbility(INT idPrescription)
{
	if( (--Make_Count) < 0 )
		return;
	if( idPrescription != Make_Prescription)
		return;
	// 判断是否满足使用技能之条件
	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	// 发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_COMPOSE_ITEM;
	cmdTemp.m_adwParam[0]	= idPrescription;
	pMySelfAI->PushCommand( &cmdTemp );
}

VOID CGameInterface::Player_MoveTo( const fVector2&  fvAimPos )
{
	static const INT	DRAG_MOVE_TIME_INTERVAL	= 0;
	static unsigned long  ulTime = 0;
	
	// 当点选地面移动时必须及时响应，修复了原来在使用技能的时候，
	// 移动被忽略的现像
	//if( ulTime +  DRAG_MOVE_TIME_INTERVAL < ::timeGetTime() )
	{
		// 显示指示环
		CGameProcedure::s_pProcMain->SetMouseTargetProjTex( fvAimPos );

		// 移动指令
		CAI_Base* pMySelfAI = NULL;
		pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

		const _DBC_SCENE_DEFINE* pSceneDef = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine();
		INT x = pSceneDef->nXSize;
		INT y = pSceneDef->nZSize;
		bool move = (fvAimPos.x>0)&&(fvAimPos.x<x)&&(fvAimPos.y>0)&&(fvAimPos.y<y);

		if( pMySelfAI && move )
			pMySelfAI->PushCommand_MoveTo( fvAimPos.x, fvAimPos.y );
		else
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "超出地图范围！");

		// 记录时间
		ulTime = ::timeGetTime();
	}
}

VOID CGameInterface::Player_MoveTo( INT nSceneID, const fVector2&  fvAimPos )
{
	INT nCurrentScene = CGameProcedure::s_pWorldManager->GetActiveSceneID();
	//同场景寻路
	if (nCurrentScene == nSceneID)
	{
		Player_MoveTo(fvAimPos);
	}
	//跨场景寻路
	else
	{
		FindResult oResult;
		if( g_ShortestPath.FindPath(nSceneID,nCurrentScene, oResult) )
		{
			m_SceneFindPathStatus.SetValue(nSceneID, fvAimPos, oResult);
			//移动到第一个传送点
			fVector2 vPos = m_SceneFindPathStatus.GetNextPosition();
			if (vPos.x > -1 && vPos.y > -1)
			{
				Player_MoveTo(vPos);
			}
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "当前场景无法达到目的场景！");
		}
	}
}

VOID CGameInterface::CheckAutoMove( VOID )
{
	if( m_SceneFindPathStatus.IsUse() )
	{
		//移动到下一个传送点
		fVector2 vPos = m_SceneFindPathStatus.GetNextPosition();
		if (vPos.x > -1 && vPos.y > -1)
		{
			Player_MoveTo(vPos);
		}
	}
}


VOID CGameInterface::Player_DestroyEquip(const tObject_Item* pEquip)
{
	if(!pEquip || pEquip->GetItemClass() != ICLASS_EQUIP) return;

	CGDiscardEquip msg;
	msg.SetEquipPoint((BYTE)((CObject_Item_Equip*)pEquip)->GetItemType());

	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::Player_UnEquip(const tObject_Item* pEquip)
{
	if(!pEquip || pEquip->GetItemClass() != ICLASS_EQUIP) return;

	CGUnEquip msg;
	msg.setEquipPoint((BYTE)((CObject_Item_Equip*)pEquip)->GetItemType());

	CNetManager::GetMe()->SendPacket( &msg );
}

//请求升级
VOID CGameInterface::Player_AskLevelUp(VOID)
{
	CGReqLevelUp msg;

	CNetManager::GetMe()->SendPacket(&msg);
}

//请求洗一级属性点
VOID CGameInterface::Player_AskCleanBat1Point(VOID)
{
	CGReqResetAttr msg;

	CNetManager::GetMe()->SendPacket(&msg);
}

//谈话
VOID CGameInterface::Player_Speak(INT idObj)
{
	CAI_Base *pMySelfAI;
	pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	SCommand_AI cmdSpeak;
	cmdSpeak.m_wID = AIC_DEFAULT_EVENT;
	cmdSpeak.m_adwParam[0] = idObj;

	pMySelfAI->PushCommand(&cmdSpeak);
}

//-----------------------------------------------------------
// 从货架上头购买一个物品
VOID CGameInterface::Booth_BuyItem( tObject_Item* pItem )
{
	CGShopBuy msg;
	// 先判断是否够钱买

	msg.SetIndex(pItem->GetPosIndex());
	msg.SetUniqueID(CUIDataPool::GetMe()->GetCurShopNpcId());
	CNetManager::GetMe()->SendPacket( &msg );
}

// 卖出一个物品
VOID CGameInterface::Booth_SellItem( tObject_Item* pItem )
{
	//验证是否可以卖出
	if(!pItem->Rule(tObject_Item::RULE_SALE))
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_SALE).c_str());
		return;
	}

	//判断当前的NPC是不是收购这类物品
	if( !CDataPool::GetMe()->Booth_IsCanBuy(pItem) )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("CGameInterface_Info_Not_Buy_ThisItem");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "我不收购这个物品");
		return;
	}

	CGShopSell msg;

	int nIndex = pItem->GetPosIndex();
	msg.SetBagIndex( pItem->GetPosIndex() );
	msg.SetUniqueID(CUIDataPool::GetMe()->GetCurShopNpcId());
	CNetManager::GetMe()->SendPacket( &msg );
}


//完成交易
VOID CGameInterface::Booth_Close()
{
	//本次交易结束
	CGShopClose msg;
	CNetManager::GetMe()->SendPacket( &msg );

	//清空原有商品列表,用这个标示商店界面已经关上了
	CDataPool::GetMe()->Booth_Close();
	if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
	{
		CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
	}

	return;
}

//修理
VOID CGameInterface::Booth_Repair(BOOL bAll, INT bagindex, BOOL isInBag)
{	
	if(isInBag)
	{
		CGShopRepair msg;
		msg.SetOpt(CGShopRepair::FromBag);
		msg.RepairAll(bAll);
		if(bAll)
		{
			msg.SetUniqueID(  CUIDataPool::GetMe()->GetCurDialogNpcId()); //NPC id     
			CNetManager::GetMe()->SendPacket( &msg );
		}
		else
		{
			msg.SetBagIndex(bagindex);
			msg.SetUniqueID(  CUIDataPool::GetMe()->GetCurDialogNpcId()); //NPC id     
			CNetManager::GetMe()->SendPacket( &msg );
		}
	}
	else
	{
		CGShopRepair msg;
		msg.RepairAll(bAll);
		msg.SetBagIndex(bagindex);
		msg.SetOpt(CGShopRepair::FromEquip);
		msg.SetUniqueID(  CUIDataPool::GetMe()->GetCurDialogNpcId()); //NPC id     
		CNetManager::GetMe()->SendPacket( &msg );
	}

}






//鼠标进入修理状态
VOID CGameInterface::Booth_EnterRepair(VOID)
{
	CActionSystem::GetMe()->SetDefaultAction(CActionItem_MouseCmd_Repair::GetMe());
}

 

//恢复到缺省技能操作，用于退出修理，鉴定，等特殊鼠标命令状态
VOID CGameInterface::Skill_CancelAction(VOID)
{
	//修理
	if(	CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
	{
		CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	}
	else if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())  //鉴定
	{
		if(CActionSystem::GetMe()->GetNotCancleIden())
		{
			//不取消鉴定操作
			CActionSystem::GetMe()->SetNotCancleIden(false);

		}
		else
		{

			CDataPool::GetMe()->Identify_UnLock();
			CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
		}
	}
	//else if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_AddFriend::GetMe())  //鉴定
	//{
	//	CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	//}
	else if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Exchange::GetMe())  //鉴定
	{
		CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	}
	//else if( CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Sale::GetMe())  //卖动息
	//{
	//	CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	//}
	//else if( CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_BuyMult::GetMe())  //复数买东西
	//{
	//	CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	//}
	//else if( CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Asunder::GetMe())  //拆分东西
	//{
	//	CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	//}
	//else if( CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_ShopFitting::GetMe())  //试穿/骑
	//{
	//	CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	//	CEventSystem::GetMe()->PushEvent(GE_CLOSE_SHOP_FITTING);
	//}
	//else if( CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_SaleToRecShop::GetMe())
	//{
	//	CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	//}
}

//-------------------------------------------------
//更新称号
VOID CGameInterface::Title_UpdateCurTitle(BYTE titleType, SHORT titleData)
{
	/*CGCharUpdateCurTitle Msg;
	Msg.setTitleType(titleType);
//	Msg.setTitleData(titleData);
//	Msg.setTitle("");

	CNetManager::GetMe()->SendPacket( &Msg );*/
}



//=====================================================================
VOID CGameInterface::TripperObj_Active( UINT dwId )
{
	CAI_Base *pMySelfAI;
	pMySelfAI		= CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();
	
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_TRIPPER_ACTIVE;
	cmdTemp.m_adwParam[0]	= dwId;

	pMySelfAI->PushCommand( &cmdTemp );
}

VOID CGameInterface::ItemBox_PickItem( tObject_Item* pItem )
{
	_ITEM_GUID  sDataGuid;
	
	CGPickBoxItem msg;
	msg.setItemBoxId( CDataPool::GetMe()->ItemBox_GetID() );

	WORD wServer, wWorld;
	GUID_t dwSerial;
	pItem->GetGUID(wWorld, wServer, dwSerial);

	sDataGuid.m_Serial	= dwSerial;
	sDataGuid.m_Server	= (BYTE)wServer;
	sDataGuid.m_World	= (BYTE)wWorld;
	msg.setItemId( sDataGuid );

	CNetManager::GetMe()->SendPacket( &msg );
}


VOID CGameInterface::PacketItem_UserItem(tActionItem* pActionItem, int targetServerID, fVector2& fvPos)
{
	//空物品
	if(!pActionItem || pActionItem->GetType() != AOT_ITEM) return;
	CObject_Item* pItem = (CObject_Item*)(((CActionItem_Item*)pActionItem)->GetItemImpl());
	if(!pItem) return;
	//必须是能够使用的物品
	if(pItem->GetItemClass()!=ICLASS_COMITEM && pItem->GetItemClass()!=ICLASS_TASKITEM && pItem->GetItemClass()!=ICLASS_SOUL_BEAD ) return;
	//特殊物品不能在背包中直接使用，例如，宠物技能书
	STRING strTemp;
	if(!CObject_Item::CheckUseInPackage(pItem, strTemp))
	{
		if(!strTemp.empty()) CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return;
	}
	//组队跟随中...
	if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag()) return;

	//检查目前选中的目标
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(targetServerID);
	
	//检查物品是否能够直接使用
	int objID;
	PET_GUID_t petID;
	bool bCanuseDir = ((CObject_Item_Medicine*)pItem)->IsValidTarget(pObj, fvPos, objID, petID);
	bool bSoul = ( pItem->GetItemClass() == ICLASS_SOUL_BEAD );

	if(bCanuseDir || bSoul )	//魂珠可以直接使用
	{
		WORLD_POS posTarget(fvPos.x, fvPos.y);

		//能够直接使用
		CGUseItem msg;
		msg.SetBagIndex( pItem->GetPosIndex() );
		msg.SetTargetObjID(objID);
		msg.SetTargetPetGUID(petID);
		msg.SetTargetPos(&posTarget);

		CNetManager::GetMe()->SendPacket( &msg );
		return;
	}

	//如果已经选中目标，说明目标不合适,如果是用在自己宠物上的物品，说明宠物没有释放
	if(pObj || ((CObject_Item_Medicine*)pItem)->IsTargetOne())
	{
		STRING strTemp = NOCOLORMSGFUNC("GMDP_Struct_Skill_Info_Invalidate_Target");
		CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return;
	}

	//需要选中目标,在鼠标上挂上物品
	CActionSystem::GetMe()->SetDefaultAction(pActionItem);
}

VOID CGameInterface::PacketItem_UserItem_ToTarget( INT nIndex, UINT nTargetID)
{
	if ( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag() )
	{
		return;
	}

	CGUseItem msg;
	msg.SetBagIndex( nIndex );
	msg.SetTargetObjID(nTargetID);

	CNetManager::GetMe()->SendPacket( &msg );
}


VOID CGameInterface::PacketItem_UserEquip( INT nIndex )
{
	tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem( nIndex );
	if(pItem == NULL ) return;

	//摆摊时不可以随便换装
	if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_You_Stalling");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
		return;
	}

	int iNeedLevel   = 0;
	int iPlayerLevel = 0;

	//验证是否可以使用
	if(!pItem->Rule(tObject_Item::RULE_USE))
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
		return;
	}

	//判断等级
	iNeedLevel = pItem->GetNeedLevel();
	CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
	if(pCharData)
	{
		iPlayerLevel = pCharData->Get_Level();
	}

	if(iPlayerLevel < iNeedLevel)
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Level_Not_Enough");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return;
	}

	// 取得详细信息
	LPCTSTR szExtraInfo = ((tObject_Item*)pItem)->GetExtraDesc();

	// 如果没有详细信息，从服务器上请求详细信息
	if(!szExtraInfo)
	{
		CDataPool::GetMe()->UserBag_AskExtraInfo( nIndex );
	}

	CGUseEquip msg;
	msg.setBagIndex( nIndex );
	CNetManager::GetMe()->SendPacket( &msg );
}

//  [7/8/2010 陈军龙]
VOID CGameInterface::PacketItem_EquipMount( INT nIndex )
{
	if( MAX_MOUNT_SIZE == CDataPool::GetMe()->Mount_GetMountCount())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// 显示到聊天框
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Count_Up_To_Max_Number");;
			ADDTALKMSG(szMsg);
		}
		return;
	}

	CGManipulateMount msg;
	msg.setBagIndex(nIndex);
	msg.setMountOperator(CGManipulateMount::MANIPULATE_EQUIP_MOUNT);
	CNetManager::GetMe()->SendPacket( &msg );
}

//  [7/8/2010 陈军龙]
VOID CGameInterface::PacketItem_CallMount( INT nIndex )
{
	KLAssert((nIndex >= 0) && (nIndex < MAX_MOUNT_SIZE));

	CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
	INT mountID = pCharacterData->Get_MountID();
    INT mountIndex = pCharacterData->Get_MountIndex();

	// 若当前未乘骑则直接返回
	if (INVALID_ID != mountID)
	{
		return;
	}

    // 若当前乘骑的就是这匹马则直接返回
	if ((mountIndex == nIndex))
	{
		return;
	}

	CGManipulateMount msg;
	msg.setMountIndex(nIndex);
	msg.setMountID(pCharacterData->Get_Mount(nIndex));
	msg.setMountOperator(CGManipulateMount::MANIPULATE_CALL_MOUNT);
	CNetManager::GetMe()->SendPacket( &msg );
}

//  [7/8/2010 陈军龙]
VOID CGameInterface::PacketItem_RecallMount( INT nIndex )
{
	KLAssert((nIndex >= 0) && (nIndex < MAX_MOUNT_SIZE));

	CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
	INT mountIndex = pCharacterData->Get_MountIndex();
	INT mountID = pCharacterData->Get_Mount(mountIndex);

	// 若当前未乘骑则直接返回
	if (INVALID_ID == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		return;
	}

	CGManipulateMount msg;
	msg.setMountIndex(mountIndex);
	msg.setMountID(mountID);
	msg.setMountOperator(CGManipulateMount::MANIPULATE_RECALL_MOUNT);
	CNetManager::GetMe()->SendPacket( &msg );
}

//  [7/8/2010 陈军龙]
VOID CGameInterface::PacketItem_DiscardMount( INT nIndex )
{
	KLAssert((nIndex >= 0) && (nIndex < MAX_MOUNT_SIZE));

	CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
	INT mountIndex = pCharacterData->Get_MountIndex();
	INT mountID = pCharacterData->Get_Mount(nIndex);

	// 若当前正在乘骑则返回
	if (mountIndex == nIndex)
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL) ? TRUE : FALSE;

		// 显示到聊天框
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Can_Not_Discard_When_Mounting");;
			ADDTALKMSG(szMsg);
		}
		return;
	}
	
	// 若为空直接返回
	if (INVALID_ID ==mountID)
	{
		return;
	}

	CGManipulateMount msg;
	msg.setMountIndex(nIndex);
	msg.setMountID(mountID);
	msg.setMountOperator(CGManipulateMount::MANIPULATE_DISCARD_MOUNT);
	CNetManager::GetMe()->SendPacket( &msg );
}

//  [7/8/2010 陈军龙]
VOID CGameInterface::PacketItem_DestroyMount( INT nIndex )
{
	KLAssert((nIndex >= 0) && (nIndex < MAX_MOUNT_SIZE));

	CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
	INT mountID = pCharacterData->Get_Mount(nIndex);

	// 若为空直接返回
	if (INVALID_ID == mountID)
	{
		return;
	}

	CGManipulateMount msg;
	msg.setMountIndex(nIndex);
	msg.setMountOperator(CGManipulateMount::MANIPULATE_DESTROY_MOUNT);
	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::PacketItem_UserGem(INT nGemIndex, INT nEquipIndex)
{
	CGUseGem msg;
//	msg.SetGemBagIndex(nGemIndex);
//	msg.SetEquipBagIndex(nEquipIndex);
	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::PacketItem_Destroy(INT nIndex)
{
	CGDiscardItem msg;
	msg.SetBagIndex(nIndex);

	CNetManager::GetMe()->SendPacket( &msg );
}

VOID	CGameInterface::PacketItem_SaveSuit(INT nIndex)
{
	_SUIT_SETTING aSuit = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_EquipSuit(nIndex);

	CGSaveEquipSuit msg;
	msg.setSuitNum(nIndex);
	msg.setSuitSetting( aSuit );
	CNetManager::GetMe()->SendPacket( &msg );
}

//给包裹里的装备打孔
VOID	CGameInterface::PacketItem_AddHole()
{
      

	INT nIndex = CDataPool::GetMe()->AddHole_GetItemPos();
	if ( nIndex < 0 ||nIndex > MAX_BAG_SIZE )
	{
		return ;
	}

	CObject_Item_Equip* pItem =( CObject_Item_Equip* )( CDataPool::GetMe()->UserBag_GetItem( nIndex ));
	if(pItem == NULL ) return;

	if(pItem->GetEquipHoleNum() >= pItem->GetEquipHoleMaxNum())

	{
		STRING strTemp = "此装备的孔已达到最大数量";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;

	}


	tObject_Item* pItemBag  =NULL;

	BOOL  bHasItem = FALSE;
	INT   nBagItemNum = 0;

	for ( INT i =0 ;i<  MAX_BAG_SIZE ;++i)
	{
		pItemBag =  CDataPool::GetMe()->UserBag_GetItem(  i );
		if ( pItemBag)
		{
			if (  pItemBag->GetIdTable() == CDataPool::GetMe()->AddHole_GetNeedItemId())
			{
				bHasItem = TRUE;
				nBagItemNum += pItemBag->GetNumber();
			}
		}
	}

	INT  nMoney      = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money();

	if ( !bHasItem)
	{
		STRING strTemp = "缺少打孔道具";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;

	}
	 if (nBagItemNum < CDataPool::GetMe()->AddHole_GetNeedItemNum())
	{

		STRING strTemp = "打孔道具数量不足";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;
	}
	
	if ( nMoney < CDataPool::GetMe()->AddHole_GetNeedMoney())
	{
		STRING strTemp = "金钱不足";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;

	}


			_ITEM_GUID  sDataGuid;
			WORD wServer, wWorld;
			GUID_t dwSerial;
			 
			pItem->GetGUID(wWorld, wServer, dwSerial);

			sDataGuid.m_Serial	= dwSerial;
			sDataGuid.m_Server	= (BYTE)wServer;
			sDataGuid.m_World	= (BYTE)wWorld;
		 

			CGEquipAddHole msg;
			msg.setEquipFromType(ENUM_BAG);  //来源
			msg.setEquipFromPos(nIndex);  //装备点，或背包位置
			msg.setItemID(sDataGuid);		//装备的GUID
			msg.SetUniqueID(  CDataPool::GetMe()->AddHole_GetNpcId()); //NPC id                 
			CNetManager::GetMe()->SendPacket( &msg );
	

}
VOID	CGameInterface::HumanEquip_AddHole()
{ 

	HUMAN_EQUIP point = HUMAN_EQUIP ( CDataPool::GetMe()->AddHole_GetItemPos());
	if (point < HEQUIP_MAINHAND || point > HEQUIP_NUMBER)
	{
		return ;
	}
   //需要打孔的装备
	CObject_Item_Equip* pItem = ( CObject_Item_Equip* )( CDataPool::GetMe()->UserEquip_GetItem(point) );
	if(pItem == NULL ) return;


	if(pItem->GetEquipHoleNum() >= pItem->GetEquipHoleMaxNum())

	{
		STRING strTemp = "此装备的孔已达到最大数量";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;

	}


    tObject_Item* pItemBag  =NULL;

	BOOL  bHasItem = FALSE;
	INT   nBagItemNum = 0;

	for ( INT i =0 ;i<  MAX_BAG_SIZE ;++i)
	{
		pItemBag =  CDataPool::GetMe()->UserBag_GetItem(  i );
		if ( pItemBag)
		{
			if (  pItemBag->GetIdTable() == CDataPool::GetMe()->AddHole_GetNeedItemId())
			{
				bHasItem = TRUE;
			    nBagItemNum += pItemBag->GetNumber();
			}
		}

	}

	 INT  nMoney      =  CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money();

	if ( !bHasItem)
	{
		STRING strTemp = "缺少打孔道具";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;
		 
	}

	 if ( nBagItemNum <  CDataPool::GetMe()->AddHole_GetNeedItemNum())
	{

		STRING strTemp = "打孔道具数量不足";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;
	}
	if ( nMoney < CDataPool::GetMe()->AddHole_GetNeedMoney())
	{
		STRING strTemp = "金钱不足";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;
	}


			_ITEM_GUID  sDataGuid;
			WORD wServer, wWorld;
			GUID_t dwSerial;

			pItem->GetGUID(wWorld, wServer, dwSerial);

			sDataGuid.m_Serial	= dwSerial;
			sDataGuid.m_Server	= (BYTE)wServer;
			sDataGuid.m_World	= (BYTE)wWorld;

			CGEquipAddHole msg;
			msg.setEquipFromType(ENUM_EQUIP_POINT);  // 玩家装备
			msg.setEquipFromPos(point);  //装备点 
			msg.setItemID(sDataGuid);		//装备的GUID
			msg.SetUniqueID(  CDataPool::GetMe()->AddHole_GetNpcId()); //NPC id        
			CNetManager::GetMe()->SendPacket( &msg );
		

}


//给包裹里的装备绑定
VOID	CGameInterface::PacketItem_BangDing()

{

    INT nIndex = -1 ,nItemIndex= -1;

	nItemIndex  = CDataPool::GetMe()->Bind_GetNeedItemPos();
	nIndex = CDataPool::GetMe()->Bind_GetItemPos();

     if (nItemIndex == -1  )

	 {

		STRING strTemp = "缺少绑定道具" ;
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;

	 }


	if (  nIndex < 0 || nIndex > MAX_BAG_SIZE  )
	{
		return ;
	}

	CObject_Item_Equip* pItem =( CObject_Item_Equip* )( CDataPool::GetMe()->UserBag_GetItem( nIndex ));
	if(pItem == NULL ) return;

	/*if ( pItem->GetItemSecLevelBindInfo() == CObject_Item_Equip::EQUIP_BIND_TWO )
	{
		STRING strTemp = "该装备已经绑定" ;
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());

        
		return ;

	}
	else  

	{*/

		_ITEM_GUID  sDataGuid;
		WORD wServer, wWorld;
		GUID_t dwSerial;

		pItem->GetGUID(wWorld, wServer, dwSerial);

		sDataGuid.m_Serial	= dwSerial;
		sDataGuid.m_Server	= (BYTE)wServer;
		sDataGuid.m_World	= (BYTE)wWorld;

		CGEquipBind  msg;

		msg.SetEquipPosType(ENUM_BAG);  //来源
		msg.SetPos(nIndex);		// 或背包位置
		msg.setGemBagPos(nItemIndex);
		msg.setItemID(sDataGuid);
		msg.SetUniqueID(CUIDataPool::GetMe()->GetCurDialogNpcId() ); //NPC id      
		CNetManager::GetMe()->SendPacket( &msg );
		 
 
	//}


}

//改造装备
VOID    CGameInterface::EquipRework(INT  *Grad)   

{
	if ( !Grad)
	{
		return ;
	}


	CGEquipRemould msg;
	CObject_Item_Equip* pItem = NULL;
    INT pos = CDataPool::GetMe()->Rework_GetItemPos();

	if (CDataPool::GetMe()->Rework_GetItemConta() == tObject_Item::IO_MYSELF_PACKET)
	{
		msg.SetEquipPosType(ENUM_BAG);
		 pItem	= ( CObject_Item_Equip* )( CDataPool::GetMe()->UserBag_GetItem(pos) );
	}
	else if(CDataPool::GetMe()->Rework_GetItemConta() == tObject_Item::IO_MYSELF_EQUIP)
	{
		msg.SetEquipPosType(ENUM_EQUIP_POINT);
	    pItem = ( CObject_Item_Equip* )( CDataPool::GetMe()->UserEquip_GetItem(HUMAN_EQUIP(pos)) );
	}
     if (!pItem)
     {
		 return ;
     }


	DBC_DEFINEHANDLE(s_pItem_Equip, DBC_EQUIP_PRODUCT_INFO);
	const _DBC_EQUIP_PRODUCT_INFO* pGEquipProduct = (const _DBC_EQUIP_PRODUCT_INFO*)s_pItem_Equip->Search_Index_EQU( CDataPool::GetMe()->Rework_GetIdTable());
	if (pGEquipProduct)
	{
		 
		EQUIPREMOULD_MATERIAL_INFO  material[MAX_EQUIP_RWMATL_NUM];

		for (INT i= 0 ;i< MAX_EQUIP_RWMATL_NUM ; ++i) 
		{
			if (Grad[i]!= -1)
			{
				material[i].m_nMaterialLevel  =  Grad[i];
				material[i].m_nMaterialType   =  pGEquipProduct->szMat[i].nMaterial;
				msg.SetMaterialInfo(material[i]);
			}     		

		}    

		_ITEM_GUID  sDataGuid;
		WORD wServer, wWorld;
		GUID_t dwSerial;

		pItem->GetGUID(wWorld, wServer, dwSerial);

		sDataGuid.m_Serial	= dwSerial;
		sDataGuid.m_Server	= (BYTE)wServer;
		sDataGuid.m_World	= (BYTE)wWorld;

		msg.SetPos(pos);
		msg.setItemID(sDataGuid);

		CNetManager::GetMe()->SendPacket(&msg);

	}

    return ;


}
//合成装备
 VOID    CGameInterface::EquipSynthesize(INT ProId ,INT  *Grad)    

{

	if ( !Grad)
	{
		return ;
	}

	CGItemCreate msg;
	//Product_info.tab
	DBC_DEFINEHANDLE(s_pItem_Equip, DBC_EQUIP_PRODUCT_INFO);
	//搜索纪录
	const _DBC_EQUIP_PRODUCT_INFO* pGEquipProduct = (const _DBC_EQUIP_PRODUCT_INFO*)s_pItem_Equip->Search_Index_EQU(ProId);

	if (pGEquipProduct)
	{
        msg.SetItemIndex(pGEquipProduct->nProductId);

	   CREATEITEM_MATERIAL_INFO material[MAX_EQUIP_PROMATL_NUM];


		for (INT i= 0 ;i< MAX_EQUIP_PROMATL_NUM ; ++i) 
		{
                if (Grad[i]!= -1)
                {
					material[i].m_nMaterialLevel  =  Grad[i];
					material[i].m_nMaterialType   =  pGEquipProduct->szMat[i].nMaterial;
					msg.SetMaterialReq(material[i]);
                }     		
	
		}
		msg.SetUniqueID(CUIDataPool::GetMe()->GetCurDialogNpcId() ); //NPC id      
     	CNetManager::GetMe()->SendPacket(&msg);

	}


}


//给身上的装备绑定
VOID	CGameInterface::HumanEquip_BangDing()

{

	INT  nItemIndex= -1;

	nItemIndex  = CDataPool::GetMe()->Bind_GetNeedItemPos();

	HUMAN_EQUIP point  =  HUMAN_EQUIP (CDataPool::GetMe()->Bind_GetItemPos());


	if (nItemIndex == -1)

	{

		STRING strTemp = "缺少绑定道具" ;
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;

	}

     if (point < HEQUIP_MAINHAND ||point > HEQUIP_NUMBER||  nItemIndex < 0 || nItemIndex> MAX_BAG_SIZE)
     {
		 return ;
     }
	//需要打孔的装备
	CObject_Item_Equip* pItem = ( CObject_Item_Equip* )( CDataPool::GetMe()->UserEquip_GetItem(point) );
	if(pItem == NULL ) return;

	if ( pItem->GetItemSecLevelBindInfo() == CObject_Item_Equip::EQUIP_BIND_TWO )
	{
		STRING strTemp = "=该装备已经绑定" ;
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;

	}else
	{
		_ITEM_GUID  sDataGuid;
		WORD wServer, wWorld;
		GUID_t dwSerial;

		pItem->GetGUID(wWorld, wServer, dwSerial);

		sDataGuid.m_Serial	= dwSerial;
		sDataGuid.m_Server	= (BYTE)wServer;
		sDataGuid.m_World	= (BYTE)wWorld;


		CGEquipBind  msg;
		msg.SetEquipPosType(ENUM_EQUIP_POINT);  // 玩家装备
		msg.SetPos(point);		//装备点 
		msg.setGemBagPos(nItemIndex);
		msg.setItemID(sDataGuid);
		msg.SetUniqueID(CUIDataPool::GetMe()->GetCurDialogNpcId() ); //NPC id      
		CNetManager::GetMe()->SendPacket( &msg );
	}
}
 

VOID CGameInterface::Scene_SetPostFilter(LPCTSTR szFilterName)
{
	CGameProcedure::s_pGfxSystem->Scene_SetPostFilter(szFilterName);
}

VOID CGameInterface::Scene_SetLightmapQuality(INT nQuality)
{
	CGameProcedure::s_pGfxSystem->Scene_SetLightmapQuality(nQuality);
}
VOID CGameInterface::SendChatCommand( INT nChannelID, INT nChatType, INT nTeamID, LPCTSTR szContex, LPCTSTR szTargetName )
{
	if( strlen( szContex ) == 0 )
		return;
	CGChat   msg;
	msg.SetChannelID( nChannelID );
	msg.SetContexSize( BYTE( strlen( szContex ) ) );
	msg.SetContex( (CHAR*)szContex );
	msg.SetTargetName( (CHAR*)szTargetName );
	msg.SetTeamID( nTeamID );
	msg.SetChatType( nChatType );
	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::GM_SendCommand( LPCSTR strCommand )
{
	if(!strCommand || strCommand[0] == 0) return;

	CGCommand msg;
	msg.SetCommand((CHAR*)strCommand);
	msg.SetCommandSize((BYTE)strlen(strCommand));

	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::Skill_SetActive(tActionItem* pActiveSkill)
{
	// 设置激活技能
	CGameProcedure::s_pProcMain->SetActiveSkill(pActiveSkill);

	// 通知UI设置按钮的Check状态
	CActionSystem::GetMe()->SetDefaultAction(pActiveSkill);
}

tActionItem* CGameInterface::Skill_GetActive(VOID)
{
	return CGameProcedure::s_pProcMain ? CGameProcedure::s_pProcMain->GetActiveSkill() : 0;
}

ENUM_RELATION CGameInterface::GetCampType( CObject *pObj_A, CObject *pObj_B )
{
	if ( pObj_A == NULL || pObj_B == NULL )
	{
		return RELATION_ENEMY;
	}

	if ( pObj_A == pObj_B )
	{
		return RELATION_FRIEND;
	}

	ObjID_t idOwner_A, idOwner_B, idA, idB;
	idA			= pObj_A->GetServerID();
	idB			= pObj_B->GetServerID();
	idOwner_A	= pObj_A->GetOwnerID();
	idOwner_B	= pObj_B->GetOwnerID();
	if ( idOwner_A != INVALID_ID || idOwner_B != INVALID_ID )
	{
		if ( idOwner_A == idOwner_B
			|| idOwner_A == idB
			|| idOwner_B == idA )
		{
			return RELATION_FRIEND;
		}
	}

	const _CAMP_DATA *pCamp_A, *pCamp_B;
	BOOL bHuman_A, bHuman_B;
	const CampAndStandDataMgr_T *pCampMgr;

	pCamp_A		= pObj_A->GetCampData();
	pCamp_B		= pObj_B->GetCampData();
	bHuman_A	= g_theKernel.IsKindOf( pObj_A->GetClass(), GETCLASS(CObject_PlayerMySelf));
	bHuman_B	= g_theKernel.IsKindOf( pObj_B->GetClass(), GETCLASS(CObject_PlayerMySelf));
	pCampMgr	= CGameProcedure::s_pCampDataMgr;

	return CalcRelationType( pCamp_A, bHuman_A, pCamp_B, bHuman_B, pCampMgr );
}
//打开一个玩家的摊位
VOID CGameInterface::Stall_OpenStallSale(INT nObjId)
{
	CObject *pChar = (CObject*) CObjectManager::GetMe()->FindServerObject(nObjId);
	if(pChar)
	{
		//先判定距离是不是合法
		const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
		const fVector3 pOtherPos = (pChar)->GetPosition();

		FLOAT fDistance = KLU_GetDist( fVector2(pMePos.x, pMePos.z), fVector2(pOtherPos.x, pOtherPos.z) );

		if( EXCHANGE_MAX_DISTANCE < fDistance )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Info_Too_Far_To_Open_Stall");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"距离太远，不能打开这个摊位");
			return;
		}

		// 特定状态不能打开摊位		20100706 BLL
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Info_Too_Cannot_To_Open_Stall"); // "当前状态不能打开这个摊位"

		//判断是否处于骑乘状态或者变身状态，这样的状态不能打开摊位
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_ModelID() != -1 )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			return;
		}
		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID() != -1 )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			return;
		}

		//判断用户是不是处于自动跑动状态中，处于这样的状态不能打开摊位
		if(	CGameProcedure::s_pProcMain->IsAutoRun() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			return;
		}

		//宠物出战状态，不能打开摊位
		if( !CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID().IsNull() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			return;
		}	

		if( CObjectManager::GetMe()->GetMySelf()->IsMoving())   //2判断都可以
			//	if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get(CObject_Character::LOGIC_BASE)  == CObject_Character::CHARACTER_LOGIC_MOVE)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			return;
		}

		// 战斗状态不能打开摊位
		if ( CObjectManager::GetMe()->GetMySelf()->IsFightState() == TRUE )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			return;
		}

		//发送打开摊位的消息
		CGStallOpen OpenStallMsg;
		OpenStallMsg.SetObjID(nObjId);
		CNetManager::GetMe()->SendPacket(&OpenStallMsg);

	}

}

// 20100416 AddCodeBegin 
//如果是超链接自动寻路的NPC，则保存自动寻路所需的属性
VOID CGameInterface::SetAutoPathNPC( INT nSceneID, STRING strNPCName )
{
	// 保存NPC自动寻路功能的相关属性
	CObjectManager::GetMe()->GetMySelf()->SetIsAutoNPCPath(nSceneID, strNPCName, nSceneID != -1 );
}
// 20100416 AddCodeEnd