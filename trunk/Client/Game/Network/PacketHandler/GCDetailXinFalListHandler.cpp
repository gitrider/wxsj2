#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "GCDetailXinFaList.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"

uint GCDetailXinFaListHandler::Execute( GCDetailXinFaList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
//		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pUIDataPool);//$$
///*
//		SCommand_DPC cmdTemp;
//		cmdTemp.m_wID			= DPC_UPDATE_XINFA_ATTRIB;
//		cmdTemp.m_adwParam[0]	= (UINT)(pPacket->getObjID());
//		cmdTemp.m_adwParam[1]	= (UINT)(pPacket->getNumXinFa());
//		cmdTemp.m_apParam[2]	= (VOID*)(pPacket->getXinFaList());
//		pDataPool->OnCommand_( &cmdTemp );
//*/
//		//确认是自己的数据
//		CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
//		if(pMySelf->GetServerID() != pPacket->getObjID()) 
//		{
//			return PACKET_EXE_CONTINUE;
//		}
//
//		//刷新到用户数据
//		for(INT i=0; i<(INT)pPacket->getNumXinFa(); i++)
//		{
//			const _OWN_XINFA* pOwnerXinFa = pPacket->getXinFa(i);
//
//			pMySelf->GetCharacterData()->Set_XinFa(pOwnerXinFa->m_nXinFaID, pOwnerXinFa->m_nLevel);
//		}
//		CEventSystem::GetMe()->PushEvent(GE_SKILL_UPDATE);
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
