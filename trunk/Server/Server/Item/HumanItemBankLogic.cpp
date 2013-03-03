/********************************************************************
��������:	2008��3��17��
�ļ�����:	HumanItemBankLogic.cpp
������:		�

�ļ����ܣ� ��������߼�����
�޸ļ�¼��

*********************************************************************/
#include "stdafx.h"
#include "HumanItemLogic.h"
#include "Player.h"
#include "Item.h"
#include "ItemOperator.h"
#include "GCItemInfo.h"
#include "ItemRuler.h"
#include "GCNotifyEquip.h"
#include "Obj_Human.h"
#include "LogDefine.h"
#include "Scene.h"

Item*	HumanItemLogic::GetBankItem(Obj_Human* pHuman,UINT BankIndex)
{
	__ENTER_FUNCTION

		Assert(BankIndex<MAX_BANK_SIZE);
	Assert(BankIndex>=0);
	if(BankIndex<0||BankIndex>MAX_BANK_SIZE)
	{
		AssertEx(FALSE,"HumanItemLogic::GetBankItem() BankIndex ���ںϷ���Χ ");
		return NULL;
	}

	ItemContainer*	pBankContainer = pHuman->GetBankContain();

	Assert(pBankContainer);

	if(!pBankContainer)
	{
		return NULL;
	}

	return pBankContainer->GetItem((UINT)BankIndex);

	__LEAVE_FUNCTION

		return NULL;
}