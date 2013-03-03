#include "StdAfx.h"
#include "GCUseExtraBagResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Action\GMActionSystem.h"

using namespace Packets;
uint	GCUseExtraBagResultHandler::Execute(GCUseExtraBagResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		BOOL bSucc = (pPacket->getResult() != 1);

		//-----------------------------------------------------
		//ʧ�ܵĲ���
		if(bSucc)
		{
			
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GCUseExtraBagResult_Info_package_Operate_Fail");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��������ʧ��!");
			return PACKET_EXE_CONTINUE;
		}

		//-----------------------------------------------------
		//UI���ݳ�
		CDataPool* pDataPool = CDataPool::GetMe();
		INT nIndex1 = pPacket->getSourIndex();
		INT nIndex2 = pPacket->getDestBagIndex();
		//�������װ��
		tObject_Item* pItem1 = pDataPool->UserBag_GetItem(nIndex1);
		tObject_Item* pItem2 = pDataPool->UserBag_GetItem(nIndex2);

		//ȫ��
		if(!pItem1 && !pItem2) return PACKET_EXE_CONTINUE;
		//Item1 -> Item2
		else if(pItem1 && !pItem2)
		{
			pDataPool->UserBag_SetItem(nIndex2, pItem1, FALSE);
			pDataPool->UserBag_SetItem(nIndex1, NULL, FALSE);
		}
		//Item1 <- Item2
		else if(!pItem1 && pItem2)
		{
			pDataPool->UserBag_SetItem(nIndex1, pItem2, FALSE);
			pDataPool->UserBag_SetItem(nIndex2, NULL, FALSE);
		}
		//Item1 <-> Item2
		else
		{
			pDataPool->UserBag_SetItem(nIndex2, pItem1, FALSE);
			pDataPool->UserBag_SetItem(nIndex1, pItem2, FALSE);
		}

		// �����������ϵı�����ActionSystem
		CActionSystem::GetMe()->UserBag_Update();

		// ֪ͨ�����¼�
		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		CEventSystem::GetMe()->PushEvent(GE_EXT_PACKAGE_OPEN, 36,1);
	}
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}