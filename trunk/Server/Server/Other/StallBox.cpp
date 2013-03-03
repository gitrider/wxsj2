#include "stdafx.h"
#include "StallBox.h"

#include "ItemContainer.h"
#include "Obj_Human.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "Scene.h"
#include "StallInfoManager.h"

VOID ServerStallBox::Init(Obj_Human* pHuman)
{
	__ENTER_FUNCTION
	m_pMySelf = pHuman;

//	Assert(pHuman->getScene());
//	m_pStallInfoMgr = pHuman->getScene()->GetStallInfoManager();

	//�����Ʒ����
	memset(m_ItemList, 0, sizeof(_ITEM)*STALL_BOX_SIZE);
	memset(m_PetItemList, 0, sizeof(_PET_DB_LOAD)*STALL_PET_BOX_SIZE);

	//��ʼ��container
	_ITEMCONTAINER_INIT	containInit;
	containInit.m_nContainerSize	=	STALL_BOX_SIZE;
	m_Container.Init(&containInit);

	for(int i=0;i<STALL_BOX_SIZE;i++)
	{
		_ITEM_INIT	itemInit(&m_ItemList[i]);
		m_Container.GetItem(i)->Init(&itemInit);
	}

	//��ʼ������container
	_ITEMCONTAINER_INIT	PetContainInit;
	PetContainInit.m_nContainerSize		=	STALL_PET_BOX_SIZE;
	m_PetContainer.Init(&PetContainInit);

	for(UINT i = 0 ;i<STALL_PET_BOX_SIZE;i++)
	{
		_ITEM_INIT itemInit(&m_PetItemList[i]);
		m_PetContainer.GetItem(i)->Init(&itemInit);
	}

	m_nFirstPage = 0;

	CleanUp();

	__LEAVE_FUNCTION
}

VOID ServerStallBox::CleanUp()
{
	__ENTER_FUNCTION
	//��������
	UnLockAllItem();

	//�ͷŵ�ǰλ��
	UnLockMapPos();

	//�������״̬
	m_StallStatus	=	STALL_CLOSE;
	m_IsOpen		=	FALSE;
	m_bOpenPublic	=	FALSE;
	m_PosTax		=	0;
	m_TradeTax		=	0;
	memset(m_szStallName, 0, MAX_STALL_NAME);

	m_SerialIncStep	=	1;
	memset(m_ItemSerial, 0, STALL_BOX_SIZE*sizeof(UINT));
	memset(m_ItemPrice, 0, STALL_BOX_SIZE*sizeof(UINT));

	memset(m_PetSerial, 0, STALL_PET_BOX_SIZE*sizeof(UINT));
	memset(m_PetPrice, 0, STALL_PET_BOX_SIZE*sizeof(UINT));

	//�����Ʒ����
	memset(m_ItemList, 0, sizeof(_ITEM)*STALL_BOX_SIZE);
	memset(m_PetItemList, 0, sizeof(_PET_DB_LOAD)*STALL_PET_BOX_SIZE);

	//���̯λBBS
	m_StallBBS.CleanUp();

	//��ʼ��̯λ���������
	memset(m_szStallName, 0, MAX_STALL_NAME);
	sprintf(m_szStallName, "%s��̯λ",m_pMySelf->GetName());
	
	CHAR szBBSTitle[MAX_BBS_MESSAGE_LENGTH];
	memset(szBBSTitle, 0, MAX_BBS_MESSAGE_LENGTH);
	sprintf(szBBSTitle, "��ӭ�������ҵ�̯λ");
	m_StallBBS.SetBBSTitle(szBBSTitle,(INT)strlen(szBBSTitle));
	__LEAVE_FUNCTION
}

VOID ServerStallBox::UnLockAllItem()
{
	__ENTER_FUNCTION
	if(m_pMySelf == NULL)
		return;

	for(INT i = 0; i<STALL_BOX_SIZE; i++)
	{
		if(m_Container.GetItem(i)->IsEmpty() == FALSE)
		{
			Item* pItemRef = m_Container.GetItem(i);
			INT	BagIndex = -1;
			ItemContainer*	pMyBagContainer = HumanItemLogic::GetBagItemContain(m_pMySelf, &pItemRef->GetGUID(), BagIndex);

			if(BagIndex != INVALID_INDEX && pMyBagContainer != NULL)
				g_ItemOperator.UnlockItem( pMyBagContainer, pMyBagContainer->BagIndex2ConIndex(BagIndex) );
			else
				Assert(FALSE);
		}
	}

	for(INT i = 0; i<STALL_PET_BOX_SIZE; i++)
	{
		if( m_PetContainer.GetItem(i)->IsEmpty() == FALSE )
		{
			Item* pItemRef = m_PetContainer.GetItem(i);
			ItemContainer*	pPetContainer = m_pMySelf->GetPetContain();
			INT	iPetIndex = pPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());

			if(iPetIndex != INVALID_INDEX && pPetContainer != NULL)
				g_ItemOperator.UnlockItem( pPetContainer, iPetIndex );
		}
	}
	__LEAVE_FUNCTION
}

VOID	ServerStallBox::UnLockMapPos()
{
	__ENTER_FUNCTION
	if(m_pMySelf == NULL)
		return;
	
	if(!m_pMySelf->getScene())
		return;

	m_pStallInfoMgr = m_pMySelf->getScene()->GetStallInfoManager();

	//�ͷ����λ��
	m_pStallInfoMgr->SetCanStall(m_PosX, m_PosZ, TRUE);
	m_PosX = 0;
	m_PosZ = 0;

	__LEAVE_FUNCTION
}

VOID	ServerStallBox::SetStallName(const CHAR *pszTitle, BYTE nSize )
{
	__ENTER_FUNCTION
	memset(m_szStallName, 0, MAX_STALL_NAME);
	memcpy(m_szStallName, pszTitle, nSize);
	m_pMySelf->StallNameChanged();//̯λ���仯ˢ�������ͻ���
	__LEAVE_FUNCTION
}