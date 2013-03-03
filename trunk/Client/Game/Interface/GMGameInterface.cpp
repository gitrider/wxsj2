
/**	GMGameInterface.cpp
 *	
 *	���ܣ�	
 *	
 *	�޸ļ�¼��
 *			080303	�� CGameInterface::Object_ShowContexMenu �����Ӵ��� ��� ��صĴ���
 *			080327	�޸� Title_UpdateCurTitle �е�CGCharUpdateCurTitle��Ϣ�������óƺŵĲ���
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
#include "packets\CGManipulateMount.h"  //  [7/8/2010 �¾���]
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

//�̵�
#include "CGShopBuy.h"
#include "CGShopClose.h"
#include "CGShopSell.h"
#include "CGShopRepair.h"
//�ƺ�
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
#include "GMInterface_Script_Talk.h"    //  [7/20/2010 �¾���]

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
	//ϵͳ���ýӿڹҽӱ�������ϵͳ
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

	// �õ����λ��
	POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();

	CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
	CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);
	
	std::vector< STRING > vParam;


	// ���ݲ�ͬ���������ͬ�Ҽ��¼�

	// ��������
	if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_PlayerMySelf)))
	{
		if(showMyself)
		{
			// ����Լ��Ѿ��ڶ�������
			if( CUIDataPool::GetMe()->IsInTeam())
			{
				vParam.push_back("player_in_team");
				vParam.push_back(szObjId);
				vParam.push_back(szXPos);
				vParam.push_back(szYPos);
				CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
				
				return;
			}

			// �Լ�û�����
			vParam.push_back("player");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
		}
	}
	// �������
	else if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_PlayerOther)))
	{
		CObject_PlayerOther* pOtherObject = (CObject_PlayerOther*)pObject;


		// �Ƿ��а��
		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			
			// ���ݲ߻��������ٽ����޸ģ� �Ƿ��ж���������Ѿ������	GetCharacterData()->Get_Guild


			INT tt = CDataPool::GetMe()->Guild_GetCurPosition();

			// �Լ��ǰ����򸱰����� ����������
			if( ( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN ) ||
				( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_ASS_CHIEFTAIN ))
			{

				// �Լ��ж��飬 �Է��ж���
				if( (pOtherObject->GetCharacterData()->Get_HaveTeamFlag())&&( CUIDataPool::GetMe()->IsInTeam()) )
				{
					vParam.push_back("guild_other_team_member");
					vParam.push_back(szObjId);
					vParam.push_back(szXPos);
					vParam.push_back(szYPos);
					CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
					
					return;
				}
				// �������û�ж���
				if((!pOtherObject->GetCharacterData()->Get_HaveTeamFlag()))
				{
					vParam.push_back("guild_other_not_team_member");
					vParam.push_back(szObjId);
					vParam.push_back(szXPos);
					vParam.push_back(szYPos);
					CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
					
					return;
				}
				// �Լ�û����, ��������ж���
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

			// �Լ��а�ᣨ�������� �Է��ѼӰ�
					// �Լ��ж��飬 �Է��ж���
					// �������û�ж���
					// �Լ�û����, ��������ж���

			// �Լ��а�ᣨ�������� �Է�û�Ӱ�
			
		}


		// �Լ��ж���, ������������Ҳ�ж���
		if((pOtherObject->GetCharacterData()->Get_HaveTeamFlag())&&( CUIDataPool::GetMe()->IsInTeam()))
		{
			vParam.push_back("other_team_member");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
			
			return;
		}

		// ����������û�ж���
		if((!pOtherObject->GetCharacterData()->Get_HaveTeamFlag()))
		{
			vParam.push_back("other_not_team_member");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
			
			return;
		}

		
		// �Լ�û����, �����������ж���
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

		// �Լ��ĳ���
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
		// ��������
		//if(pNpcObject->GetNpcType() == NPC_TYPE_PET)
		//{
		//	if(INVALID_UID != pNpcObject->GetCharacterData()->Get_OwnerID())
		//	{
		//		// �й����ĳ������ʾ�˵�
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
	// ���״̬�²���ʹ�ü���   [7/8/2010 �¾���]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// ��ʾ�������
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// �ж��Ƿ�Ϊ�Ϸ���˲������
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) 
		return;

	// ȡ�ü�������
	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) 
		return;

	CAI_Base* pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	// ������Ϣ
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= INVALID_UID;
	cmdTemp.m_afParam[2]	= -1.f;
	cmdTemp.m_afParam[3]	= -1.f;
	cmdTemp.m_afParam[4]	= -1.f;
	cmdTemp.m_auParam[5]	= INVALID_GUID;
	pMySelfAI->PushCommand( &cmdTemp );

	// �����¼�
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseSkill(INT idSkill, INT idObj)
{
	// ���״̬�²���ʹ�ü���   [7/8/2010 �¾���]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// ��ʾ�������
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// �ж��Ƿ�Ϊ�Ϸ���Ŀ�꼼��
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) 
		return;

	// ȡ�ü�������
	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) 
		return;

	// ���Ŀ���Ƿ�Ϸ�
	
//	if(!(pSkill->IsValidTarget(idObj))) return;
	

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	// ������Ϣ
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= idObj;
	cmdTemp.m_afParam[2]	= -1.f;
	cmdTemp.m_afParam[3]	= -1.f;
	cmdTemp.m_afParam[4]	= -1.f;
	cmdTemp.m_auParam[5]	= INVALID_GUID;

	pMySelfAI->PushCommand( &cmdTemp );

	// �����¼�
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseSkill(INT idSkill, const fVector2& fvPos)
{
	// ���״̬�²���ʹ�ü���   [7/8/2010 �¾���]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// ��ʾ�������
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// �ж��Ƿ�Ϊ�Ϸ��ķ�Χ����
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	// ������Ϣ
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= INVALID_UID;
	cmdTemp.m_afParam[2]	= fvPos.x;
	cmdTemp.m_afParam[3]	= fvPos.y;
	cmdTemp.m_afParam[4]	= 0.f;
	cmdTemp.m_auParam[5]	= INVALID_GUID;

	pMySelfAI->PushCommand( &cmdTemp );

	// �����¼�
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseSkill(INT idSkill, FLOAT fDir)
{
	// ���״̬�²���ʹ�ü���   [7/8/2010 �¾���]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// ��ʾ�������
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// �ж��Ƿ�Ϊ�Ϸ��ķ�����
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	// �ı����ﳯ��
	CObjectManager::GetMe()->GetMySelf()->SetFaceDir(fDir);

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	// ������Ϣ
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

	// �����¼�
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseSkill( INT idSkill, const fVector2& fvPos, const GUID_t& fDir )
{
	// ���״̬�²���ʹ�ü���   [7/8/2010 �¾���]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// ��ʾ�������
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// �ж��Ƿ�Ϊ�Ϸ��ķ�����
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	// �ı����ﳯ��
//	CObjectManager::GetMe()->GetMySelf()->SetFaceDir(fDir);

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	// ������Ϣ
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

	// �����¼�
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}


//ʹ�ü���(��ĳ������)
VOID CGameInterface::Player_UseSkill( INT idSkill, const GUID_t& guid)
{
	// ���״̬�²���ʹ�ü���   [7/8/2010 �¾���]
	if (INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// ��ʾ�������
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Forbid_Do_These_When_Mounting");
			ADDTALKMSG(szMsg);
		}

		return;
	}

	// �ж��Ƿ�Ϊ�Ϸ���Ŀ�꼼��
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	// ȡ�ü�������
	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	// ������Ϣ
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= -1;
	cmdTemp.m_afParam[2]	= -1.f;
	cmdTemp.m_afParam[3]	= -1.f;
	cmdTemp.m_afParam[4]	= -1.f;
	cmdTemp.m_auParam[5]	= guid;

	pMySelfAI->PushCommand( &cmdTemp );

	// �����¼�
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
	// �ж��Ƿ�����ʹ�ü���֮����
	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	// ������Ϣ
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_COMPOSE_ITEM;
	cmdTemp.m_adwParam[0]	= idPrescription;
	pMySelfAI->PushCommand( &cmdTemp );
}

VOID CGameInterface::Player_MoveTo( const fVector2&  fvAimPos )
{
	static const INT	DRAG_MOVE_TIME_INTERVAL	= 0;
	static unsigned long  ulTime = 0;
	
	// ����ѡ�����ƶ�ʱ���뼰ʱ��Ӧ���޸���ԭ����ʹ�ü��ܵ�ʱ��
	// �ƶ������Ե�����
	//if( ulTime +  DRAG_MOVE_TIME_INTERVAL < ::timeGetTime() )
	{
		// ��ʾָʾ��
		CGameProcedure::s_pProcMain->SetMouseTargetProjTex( fvAimPos );

		// �ƶ�ָ��
		CAI_Base* pMySelfAI = NULL;
		pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

		const _DBC_SCENE_DEFINE* pSceneDef = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine();
		INT x = pSceneDef->nXSize;
		INT y = pSceneDef->nZSize;
		bool move = (fvAimPos.x>0)&&(fvAimPos.x<x)&&(fvAimPos.y>0)&&(fvAimPos.y<y);

		if( pMySelfAI && move )
			pMySelfAI->PushCommand_MoveTo( fvAimPos.x, fvAimPos.y );
		else
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "������ͼ��Χ��");

		// ��¼ʱ��
		ulTime = ::timeGetTime();
	}
}

VOID CGameInterface::Player_MoveTo( INT nSceneID, const fVector2&  fvAimPos )
{
	INT nCurrentScene = CGameProcedure::s_pWorldManager->GetActiveSceneID();
	//ͬ����Ѱ·
	if (nCurrentScene == nSceneID)
	{
		Player_MoveTo(fvAimPos);
	}
	//�糡��Ѱ·
	else
	{
		FindResult oResult;
		if( g_ShortestPath.FindPath(nSceneID,nCurrentScene, oResult) )
		{
			m_SceneFindPathStatus.SetValue(nSceneID, fvAimPos, oResult);
			//�ƶ�����һ�����͵�
			fVector2 vPos = m_SceneFindPathStatus.GetNextPosition();
			if (vPos.x > -1 && vPos.y > -1)
			{
				Player_MoveTo(vPos);
			}
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��ǰ�����޷��ﵽĿ�ĳ�����");
		}
	}
}

VOID CGameInterface::CheckAutoMove( VOID )
{
	if( m_SceneFindPathStatus.IsUse() )
	{
		//�ƶ�����һ�����͵�
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

//��������
VOID CGameInterface::Player_AskLevelUp(VOID)
{
	CGReqLevelUp msg;

	CNetManager::GetMe()->SendPacket(&msg);
}

//����ϴһ�����Ե�
VOID CGameInterface::Player_AskCleanBat1Point(VOID)
{
	CGReqResetAttr msg;

	CNetManager::GetMe()->SendPacket(&msg);
}

//̸��
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
// �ӻ�����ͷ����һ����Ʒ
VOID CGameInterface::Booth_BuyItem( tObject_Item* pItem )
{
	CGShopBuy msg;
	// ���ж��Ƿ�Ǯ��

	msg.SetIndex(pItem->GetPosIndex());
	msg.SetUniqueID(CUIDataPool::GetMe()->GetCurShopNpcId());
	CNetManager::GetMe()->SendPacket( &msg );
}

// ����һ����Ʒ
VOID CGameInterface::Booth_SellItem( tObject_Item* pItem )
{
	//��֤�Ƿ��������
	if(!pItem->Rule(tObject_Item::RULE_SALE))
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_SALE).c_str());
		return;
	}

	//�жϵ�ǰ��NPC�ǲ����չ�������Ʒ
	if( !CDataPool::GetMe()->Booth_IsCanBuy(pItem) )
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("CGameInterface_Info_Not_Buy_ThisItem");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�Ҳ��չ������Ʒ");
		return;
	}

	CGShopSell msg;

	int nIndex = pItem->GetPosIndex();
	msg.SetBagIndex( pItem->GetPosIndex() );
	msg.SetUniqueID(CUIDataPool::GetMe()->GetCurShopNpcId());
	CNetManager::GetMe()->SendPacket( &msg );
}


//��ɽ���
VOID CGameInterface::Booth_Close()
{
	//���ν��׽���
	CGShopClose msg;
	CNetManager::GetMe()->SendPacket( &msg );

	//���ԭ����Ʒ�б�,�������ʾ�̵�����Ѿ�������
	CDataPool::GetMe()->Booth_Close();
	if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
	{
		CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
	}

	return;
}

//����
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






//����������״̬
VOID CGameInterface::Booth_EnterRepair(VOID)
{
	CActionSystem::GetMe()->SetDefaultAction(CActionItem_MouseCmd_Repair::GetMe());
}

 

//�ָ���ȱʡ���ܲ����������˳������������������������״̬
VOID CGameInterface::Skill_CancelAction(VOID)
{
	//����
	if(	CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
	{
		CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	}
	else if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())  //����
	{
		if(CActionSystem::GetMe()->GetNotCancleIden())
		{
			//��ȡ����������
			CActionSystem::GetMe()->SetNotCancleIden(false);

		}
		else
		{

			CDataPool::GetMe()->Identify_UnLock();
			CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
		}
	}
	//else if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_AddFriend::GetMe())  //����
	//{
	//	CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	//}
	else if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Exchange::GetMe())  //����
	{
		CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	}
	//else if( CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Sale::GetMe())  //����Ϣ
	//{
	//	CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	//}
	//else if( CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_BuyMult::GetMe())  //��������
	//{
	//	CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	//}
	//else if( CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Asunder::GetMe())  //��ֶ���
	//{
	//	CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	//}
	//else if( CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_ShopFitting::GetMe())  //�Դ�/��
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
//���³ƺ�
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
	//����Ʒ
	if(!pActionItem || pActionItem->GetType() != AOT_ITEM) return;
	CObject_Item* pItem = (CObject_Item*)(((CActionItem_Item*)pActionItem)->GetItemImpl());
	if(!pItem) return;
	//�������ܹ�ʹ�õ���Ʒ
	if(pItem->GetItemClass()!=ICLASS_COMITEM && pItem->GetItemClass()!=ICLASS_TASKITEM && pItem->GetItemClass()!=ICLASS_SOUL_BEAD ) return;
	//������Ʒ�����ڱ�����ֱ��ʹ�ã����磬���＼����
	STRING strTemp;
	if(!CObject_Item::CheckUseInPackage(pItem, strTemp))
	{
		if(!strTemp.empty()) CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return;
	}
	//��Ӹ�����...
	if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag()) return;

	//���Ŀǰѡ�е�Ŀ��
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(targetServerID);
	
	//�����Ʒ�Ƿ��ܹ�ֱ��ʹ��
	int objID;
	PET_GUID_t petID;
	bool bCanuseDir = ((CObject_Item_Medicine*)pItem)->IsValidTarget(pObj, fvPos, objID, petID);
	bool bSoul = ( pItem->GetItemClass() == ICLASS_SOUL_BEAD );

	if(bCanuseDir || bSoul )	//�������ֱ��ʹ��
	{
		WORLD_POS posTarget(fvPos.x, fvPos.y);

		//�ܹ�ֱ��ʹ��
		CGUseItem msg;
		msg.SetBagIndex( pItem->GetPosIndex() );
		msg.SetTargetObjID(objID);
		msg.SetTargetPetGUID(petID);
		msg.SetTargetPos(&posTarget);

		CNetManager::GetMe()->SendPacket( &msg );
		return;
	}

	//����Ѿ�ѡ��Ŀ�꣬˵��Ŀ�겻����,����������Լ������ϵ���Ʒ��˵������û���ͷ�
	if(pObj || ((CObject_Item_Medicine*)pItem)->IsTargetOne())
	{
		STRING strTemp = NOCOLORMSGFUNC("GMDP_Struct_Skill_Info_Invalidate_Target");
		CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return;
	}

	//��Ҫѡ��Ŀ��,������Ϲ�����Ʒ
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

	//��̯ʱ��������㻻װ
	if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
	{
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_You_Stalling");
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
		return;
	}

	int iNeedLevel   = 0;
	int iPlayerLevel = 0;

	//��֤�Ƿ����ʹ��
	if(!pItem->Rule(tObject_Item::RULE_USE))
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
		return;
	}

	//�жϵȼ�
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

	// ȡ����ϸ��Ϣ
	LPCTSTR szExtraInfo = ((tObject_Item*)pItem)->GetExtraDesc();

	// ���û����ϸ��Ϣ���ӷ�������������ϸ��Ϣ
	if(!szExtraInfo)
	{
		CDataPool::GetMe()->UserBag_AskExtraInfo( nIndex );
	}

	CGUseEquip msg;
	msg.setBagIndex( nIndex );
	CNetManager::GetMe()->SendPacket( &msg );
}

//  [7/8/2010 �¾���]
VOID CGameInterface::PacketItem_EquipMount( INT nIndex )
{
	if( MAX_MOUNT_SIZE == CDataPool::GetMe()->Mount_GetMountCount())
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

		// ��ʾ�������
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

//  [7/8/2010 �¾���]
VOID CGameInterface::PacketItem_CallMount( INT nIndex )
{
	KLAssert((nIndex >= 0) && (nIndex < MAX_MOUNT_SIZE));

	CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
	INT mountID = pCharacterData->Get_MountID();
    INT mountIndex = pCharacterData->Get_MountIndex();

	// ����ǰδ������ֱ�ӷ���
	if (INVALID_ID != mountID)
	{
		return;
	}

    // ����ǰ����ľ�����ƥ����ֱ�ӷ���
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

//  [7/8/2010 �¾���]
VOID CGameInterface::PacketItem_RecallMount( INT nIndex )
{
	KLAssert((nIndex >= 0) && (nIndex < MAX_MOUNT_SIZE));

	CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
	INT mountIndex = pCharacterData->Get_MountIndex();
	INT mountID = pCharacterData->Get_Mount(mountIndex);

	// ����ǰδ������ֱ�ӷ���
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

//  [7/8/2010 �¾���]
VOID CGameInterface::PacketItem_DiscardMount( INT nIndex )
{
	KLAssert((nIndex >= 0) && (nIndex < MAX_MOUNT_SIZE));

	CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
	INT mountIndex = pCharacterData->Get_MountIndex();
	INT mountID = pCharacterData->Get_Mount(nIndex);

	// ����ǰ���ڳ����򷵻�
	if (mountIndex == nIndex)
	{
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL) ? TRUE : FALSE;

		// ��ʾ�������
		if(bLog)
		{
			STRING szMsg = COLORMSGFUNC("Mount_Can_Not_Discard_When_Mounting");;
			ADDTALKMSG(szMsg);
		}
		return;
	}
	
	// ��Ϊ��ֱ�ӷ���
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

//  [7/8/2010 �¾���]
VOID CGameInterface::PacketItem_DestroyMount( INT nIndex )
{
	KLAssert((nIndex >= 0) && (nIndex < MAX_MOUNT_SIZE));

	CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
	INT mountID = pCharacterData->Get_Mount(nIndex);

	// ��Ϊ��ֱ�ӷ���
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

//���������װ�����
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
		STRING strTemp = "��װ���Ŀ��Ѵﵽ�������";
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
		STRING strTemp = "ȱ�ٴ�׵���";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;

	}
	 if (nBagItemNum < CDataPool::GetMe()->AddHole_GetNeedItemNum())
	{

		STRING strTemp = "��׵�����������";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;
	}
	
	if ( nMoney < CDataPool::GetMe()->AddHole_GetNeedMoney())
	{
		STRING strTemp = "��Ǯ����";
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
			msg.setEquipFromType(ENUM_BAG);  //��Դ
			msg.setEquipFromPos(nIndex);  //װ���㣬�򱳰�λ��
			msg.setItemID(sDataGuid);		//װ����GUID
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
   //��Ҫ��׵�װ��
	CObject_Item_Equip* pItem = ( CObject_Item_Equip* )( CDataPool::GetMe()->UserEquip_GetItem(point) );
	if(pItem == NULL ) return;


	if(pItem->GetEquipHoleNum() >= pItem->GetEquipHoleMaxNum())

	{
		STRING strTemp = "��װ���Ŀ��Ѵﵽ�������";
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
		STRING strTemp = "ȱ�ٴ�׵���";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;
		 
	}

	 if ( nBagItemNum <  CDataPool::GetMe()->AddHole_GetNeedItemNum())
	{

		STRING strTemp = "��׵�����������";
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;
	}
	if ( nMoney < CDataPool::GetMe()->AddHole_GetNeedMoney())
	{
		STRING strTemp = "��Ǯ����";
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
			msg.setEquipFromType(ENUM_EQUIP_POINT);  // ���װ��
			msg.setEquipFromPos(point);  //װ���� 
			msg.setItemID(sDataGuid);		//װ����GUID
			msg.SetUniqueID(  CDataPool::GetMe()->AddHole_GetNpcId()); //NPC id        
			CNetManager::GetMe()->SendPacket( &msg );
		

}


//���������װ����
VOID	CGameInterface::PacketItem_BangDing()

{

    INT nIndex = -1 ,nItemIndex= -1;

	nItemIndex  = CDataPool::GetMe()->Bind_GetNeedItemPos();
	nIndex = CDataPool::GetMe()->Bind_GetItemPos();

     if (nItemIndex == -1  )

	 {

		STRING strTemp = "ȱ�ٰ󶨵���" ;
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
		STRING strTemp = "��װ���Ѿ���" ;
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

		msg.SetEquipPosType(ENUM_BAG);  //��Դ
		msg.SetPos(nIndex);		// �򱳰�λ��
		msg.setGemBagPos(nItemIndex);
		msg.setItemID(sDataGuid);
		msg.SetUniqueID(CUIDataPool::GetMe()->GetCurDialogNpcId() ); //NPC id      
		CNetManager::GetMe()->SendPacket( &msg );
		 
 
	//}


}

//����װ��
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
//�ϳ�װ��
 VOID    CGameInterface::EquipSynthesize(INT ProId ,INT  *Grad)    

{

	if ( !Grad)
	{
		return ;
	}

	CGItemCreate msg;
	//Product_info.tab
	DBC_DEFINEHANDLE(s_pItem_Equip, DBC_EQUIP_PRODUCT_INFO);
	//������¼
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


//�����ϵ�װ����
VOID	CGameInterface::HumanEquip_BangDing()

{

	INT  nItemIndex= -1;

	nItemIndex  = CDataPool::GetMe()->Bind_GetNeedItemPos();

	HUMAN_EQUIP point  =  HUMAN_EQUIP (CDataPool::GetMe()->Bind_GetItemPos());


	if (nItemIndex == -1)

	{

		STRING strTemp = "ȱ�ٰ󶨵���" ;
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return ;

	}

     if (point < HEQUIP_MAINHAND ||point > HEQUIP_NUMBER||  nItemIndex < 0 || nItemIndex> MAX_BAG_SIZE)
     {
		 return ;
     }
	//��Ҫ��׵�װ��
	CObject_Item_Equip* pItem = ( CObject_Item_Equip* )( CDataPool::GetMe()->UserEquip_GetItem(point) );
	if(pItem == NULL ) return;

	if ( pItem->GetItemSecLevelBindInfo() == CObject_Item_Equip::EQUIP_BIND_TWO )
	{
		STRING strTemp = "=��װ���Ѿ���" ;
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
		msg.SetEquipPosType(ENUM_EQUIP_POINT);  // ���װ��
		msg.SetPos(point);		//װ���� 
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
	// ���ü����
	CGameProcedure::s_pProcMain->SetActiveSkill(pActiveSkill);

	// ֪ͨUI���ð�ť��Check״̬
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
//��һ����ҵ�̯λ
VOID CGameInterface::Stall_OpenStallSale(INT nObjId)
{
	CObject *pChar = (CObject*) CObjectManager::GetMe()->FindServerObject(nObjId);
	if(pChar)
	{
		//���ж������ǲ��ǺϷ�
		const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
		const fVector3 pOtherPos = (pChar)->GetPosition();

		FLOAT fDistance = KLU_GetDist( fVector2(pMePos.x, pMePos.z), fVector2(pOtherPos.x, pOtherPos.z) );

		if( EXCHANGE_MAX_DISTANCE < fDistance )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Info_Too_Far_To_Open_Stall");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����̫Զ�����ܴ����̯λ");
			return;
		}

		// �ض�״̬���ܴ�̯λ		20100706 BLL
		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GMGameInterface_Info_Too_Cannot_To_Open_Stall"); // "��ǰ״̬���ܴ����̯λ"

		//�ж��Ƿ������״̬���߱���״̬��������״̬���ܴ�̯λ
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

		//�ж��û��ǲ��Ǵ����Զ��ܶ�״̬�У�����������״̬���ܴ�̯λ
		if(	CGameProcedure::s_pProcMain->IsAutoRun() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			return;
		}

		//�����ս״̬�����ܴ�̯λ
		if( !CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID().IsNull() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			return;
		}	

		if( CObjectManager::GetMe()->GetMySelf()->IsMoving())   //2�ж϶�����
			//	if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get(CObject_Character::LOGIC_BASE)  == CObject_Character::CHARACTER_LOGIC_MOVE)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			return;
		}

		// ս��״̬���ܴ�̯λ
		if ( CObjectManager::GetMe()->GetMySelf()->IsFightState() == TRUE )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			return;
		}

		//���ʹ�̯λ����Ϣ
		CGStallOpen OpenStallMsg;
		OpenStallMsg.SetObjID(nObjId);
		CNetManager::GetMe()->SendPacket(&OpenStallMsg);

	}

}

// 20100416 AddCodeBegin 
//����ǳ������Զ�Ѱ·��NPC���򱣴��Զ�Ѱ·���������
VOID CGameInterface::SetAutoPathNPC( INT nSceneID, STRING strNPCName )
{
	// ����NPC�Զ�Ѱ·���ܵ��������
	CObjectManager::GetMe()->GetMySelf()->SetIsAutoNPCPath(nSceneID, strNPCName, nSceneID != -1 );
}
// 20100416 AddCodeEnd