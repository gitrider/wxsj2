#include "StdAfx.h"
#include "GCShopMerchandiseList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "GIException.h"

uint GCShopMerchandiseListHandler::Execute( GCShopMerchandiseList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//���ԭ����Ʒ�б�
		CDataPool::GetMe()->Booth_Clear();

		//���ԭ����Ʒ�б�
		CDataPool::GetMe()->Booth_Sold_Clear();

		//��Ʒ����
		INT nNum = pPacket->GetMerchadiseNum();

		//��ӵ����ݳ���
		for(INT i=0; i<nNum; i++)
		{
			//������Ʒʵ��
			tObject_Item* pItem = CObject_Item::NewItem( pPacket->GetMerchadiseList()[i].idTable);
			if(!pItem) continue;

			//���ø���Ʒ�ڻ����ϵ�λ��
			pItem->SetPosIndex(i);

			//���ø���Ʒ��������ÿһ���������
			pItem->SetNumber(pPacket->GetMerchadiseList()[i].byNumber);

			//����������Ʒ�������������ʾ�ã�
			pItem->SetMax(pPacket->GetMerchadiseList()[i].MaxNumber);

			//�۸�������ݳ�
			CDataPool::GetMe()->Booth_SetItemPrice(i, pPacket->GetMerchadiseList()[i].nPrice);

			//�������ݳ�
			CDataPool::GetMe()->Booth_SetItem(i, pItem);
		}
		//��������
		CDataPool::GetMe()->Booth_SetNumber(nNum);

		CDataPool::GetMe()->Booth_SetBuyType(pPacket->GetBuyType());
		CDataPool::GetMe()->Booth_SetRepairType(pPacket->GetRepairType());
		CDataPool::GetMe()->Booth_SetRepairLevel(pPacket->GetRepairLevel());
		CDataPool::GetMe()->Booth_SetRepairSpend(pPacket->GetRepairSpend());
		CDataPool::GetMe()->Booth_SetRepairOkProb(pPacket->GetRepairOkProb());
		CDataPool::GetMe()->Booth_SetBuyLevel(pPacket->GetBuyLevel());

		//���µ�ActionSystem
		if(nNum > 0) CActionSystem::GetMe()->Booth_Update();

		//֪ͨUI
		CDataPool::GetMe()->Booth_Open();

		//�õ�����Obj
		INT nID = -1;

		tObject* pObj = CObjectManager::GetMe()->FindServerObject(pPacket->GetObjID());
		if(pObj) 
		{
			nID = pObj->GetID();
			CUIDataPool::GetMe()->SetCurShopNpcId( pObj->GetServerID() );
		}
		else
		{
			nID = -1;
		}

		CDataPool::GetMe()->Booth_SetShopNpcId(nID);
		CEventSystem::GetMe()->PushEvent(GE_OPEN_BOOTH);//, nID);



	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
