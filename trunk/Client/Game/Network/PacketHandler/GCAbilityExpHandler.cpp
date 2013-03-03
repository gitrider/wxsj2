#include "StdAfx.h"
#include "GCAbilityExp.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\Action\GMActionSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"

uint GCAbilityExpHandler::Execute( GCAbilityExp* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//加入UI数据池
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pUIDataPool);

		//--------------------------------------------------------------
		//生活技能刷新
/*
		SCommand_DPC cmdTemp;
		cmdTemp.m_wID			= DPC_UPDATE_LIFE_ABILITY;
		cmdTemp.m_adwParam[0]	= 3;
		cmdTemp.m_adwParam[1]	= (UINT)(pPacket->getAbilityId());
		cmdTemp.m_adwParam[2]	= (UINT)(pPacket->getExp());
		pDataPool->OnCommand_( &cmdTemp );
*/		
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_LifeAbility(
			pPacket->getAbilityId(), -1, pPacket->getExp());

		//添加提示信息
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
		if(bLog && CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(pPacket->getAbilityId()))
		{
			LPCTSTR pAbilityName = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(
				pPacket->getAbilityId())->m_pDefine->szName;
			UINT exp = pPacket->getExp()/100;
			STRING strTemp = COLORMSGFUNC("skill_exp_up", pAbilityName, exp);
			ADDTALKMSG(strTemp);
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
