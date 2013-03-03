/*
�ͻ���ͬ�⽻�ף����Ի���
*/

#include "stdafx.h"
#include "CGExchangeOkIII.h"
#include "GCExchangeSuccessIII.h"
#include "GCExchangeError.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "Obj_Pet.h"
#include "HorseFindMateModule.h"
#include "GameStruct_Query.h"

uint CGExchangeOkIIIHandler::Execute( CGExchangeOkIII* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	EXCHANGE_CERTIFY_EACH_OTHER(pHuman)
	ObjID_t	DestID = pHuman->m_ExchangBox.m_ObjID;
	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( DestID );
	Assert( pDestHuman ) ;

	if(pHuman->m_ExchangBox.m_Status == ServerExchangeBox::EXCHANGE_WAIT_FOR_CONFIRM)
	{
		//���ÿ��Խ���
		pHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_CONFIRM_READY;
		if(pDestHuman->m_ExchangBox.m_Status ==  ServerExchangeBox::EXCHANGE_CONFIRM_READY)
		{//���Խ�����

			//1.������ʼ��
			ItemContainer*		pMyExchangeContainer = &(pHuman->m_ExchangBox.m_Container);
			ItemContainer*		pOtExchangeContainer = &(pDestHuman->m_ExchangBox.m_Container);
			ItemContainer*		pMyExchangePetContainer = &(pHuman->m_ExchangBox.m_PetContainer);
			ItemContainer*		pOtExchangePetContainer = &(pDestHuman->m_ExchangBox.m_PetContainer);

			_EXCHANGE_ITEM_LIST			ItemListMeToOt;
			_EXCHANGE_ITEM_LIST			ItemListOtToMe;

			ItemListMeToOt.Init();
			ItemListOtToMe.Init();

			//2.��֤ѭ��
			g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �� [%s] ��ʼ��֤ѭ��",	pHuman->GetName(), pDestHuman->GetName()) ;
			for(INT i = 0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if( pMyExchangeContainer->GetItem(i) != NULL && pMyExchangeContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					ItemListMeToOt.AddItem( pMyExchangeContainer->GetItem(i) );
				}
				if( pOtExchangeContainer->GetItem(i) != NULL && pOtExchangeContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					ItemListOtToMe.AddItem( pOtExchangeContainer->GetItem(i) );
				}
			}

			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				if( pMyExchangePetContainer->GetItem(i) != NULL && pMyExchangePetContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					ItemListMeToOt.AddItem( pMyExchangePetContainer->GetItem(i) );
				} 
				if( pOtExchangePetContainer->GetItem(i) != NULL && pOtExchangePetContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					ItemListOtToMe.AddItem( pOtExchangePetContainer->GetItem(i) );
				}
			}

			if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pHuman, ItemListOtToMe) )
			{//�ռ䲻��
				GCExchangeError MsgSelf;
				MsgSelf.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_SELF);
				pHuman->GetPlayer()->SendPacket(&MsgSelf);

				GCExchangeError MsgOther;
				MsgOther.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_OTHER);
				pDestHuman->GetPlayer()->SendPacket(&MsgOther);

				pHuman->m_ExchangBox.CleanUp();
				pDestHuman->m_ExchangBox.CleanUp();
				g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] �ռ䲻��", pHuman->GetName()) ;
				return PACKET_EXE_CONTINUE;
			}

			if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pDestHuman, ItemListMeToOt) )
			{//�ռ䲻��
				GCExchangeError MsgSelf;
				MsgSelf.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_SELF);
				pDestHuman->GetPlayer()->SendPacket(&MsgSelf);

				GCExchangeError MsgOther;
				MsgOther.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_OTHER);
				pHuman->GetPlayer()->SendPacket(&MsgOther);

				pHuman->m_ExchangBox.CleanUp();
				pDestHuman->m_ExchangBox.CleanUp();
				g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] �ռ䲻��", pDestHuman->GetName()) ;
				return PACKET_EXE_CONTINUE;
			}

			if( pHuman->m_ExchangBox.m_Money > pHuman->GetMoney() )
			{//��Ǯ�Ƿ�
				GCExchangeError MsgSelf;
				MsgSelf.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_SELF);
				pHuman->GetPlayer()->SendPacket(&MsgSelf);

				GCExchangeError MsgOther;
				MsgOther.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_OTHER);
				pDestHuman->GetPlayer()->SendPacket(&MsgOther);

				pHuman->m_ExchangBox.CleanUp();
				pDestHuman->m_ExchangBox.CleanUp();
				g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] ��Ǯ����", pHuman->GetName()) ;
				return PACKET_EXE_CONTINUE;
			}

			if( pDestHuman->m_ExchangBox.m_Money > pDestHuman->GetMoney() )
			{//��Ǯ�Ƿ�
				GCExchangeError MsgSelf;
				MsgSelf.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_SELF);
				pDestHuman->GetPlayer()->SendPacket(&MsgSelf);

				GCExchangeError MsgOther;
				MsgOther.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_OTHER);
				pHuman->GetPlayer()->SendPacket(&MsgOther);

				pHuman->m_ExchangBox.CleanUp();
				pDestHuman->m_ExchangBox.CleanUp();
				g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] ��Ǯ����", pDestHuman->GetName()) ;
				return PACKET_EXE_CONTINUE;
			}

			//2.1������֤
			//Obj_Pet* pDestPet	= pDestHuman->GetPet();
			//Obj_Pet* pMyPet		= pHuman->GetPet();

			PET_GUID_t	DestPetGuid;
			PET_GUID_t	MyPetGuid;

			//if(pDestPet)
			//{
			//	DestPetGuid = pDestPet->GetPetGUID();
			//}
			//if(pMyPet)
			//{
			//	MyPetGuid = pMyPet->GetPetGUID();
			//}
			DestPetGuid = pDestHuman->GetGUIDOfCallUpHorse();
			MyPetGuid	= pHuman->GetGUIDOfCallUpHorse();
			//����ǵ�ǰ�ٻ������ĳ���,���ջ����ٻ�
			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				if( pMyExchangePetContainer->GetItem(i) != NULL && pMyExchangePetContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					INT nPetLevel			= pMyExchangePetContainer->GetItem(i)->GetLevel();
					INT nHumanLevel			= pDestHuman->GetLevel();
					PET_GUID_t	ExchPetGuid	= pMyExchangePetContainer->GetItem(i)->GetPetGUID();
					if(nPetLevel>nHumanLevel)
					{
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_PET_LEVEL_TOO_HIGH);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> ���Ｖ����� petlevel = %d, humanlevel = %d", nPetLevel, nHumanLevel) ;
						return PACKET_EXE_CONTINUE;
					}
					if(ExchPetGuid == MyPetGuid)
					{
						pHuman->ReCallHorse();
					}
				}

				if( pOtExchangePetContainer->GetItem(i) != NULL && pOtExchangePetContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					INT nPetLevel = pOtExchangePetContainer->GetItem(i)->GetLevel();
					INT nHumanLevel = pHuman->GetLevel();
					PET_GUID_t	ExchPetGuid	= pOtExchangePetContainer->GetItem(i)->GetPetGUID();
					if(nPetLevel>nHumanLevel)
					{
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_PET_LEVEL_TOO_HIGH);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> ���Ｖ����� petlevel = %d, humanlevel = %d", nPetLevel, nHumanLevel) ;
						return PACKET_EXE_CONTINUE;
					}
					if(ExchPetGuid == DestPetGuid)
					{
						pDestHuman->ReCallHorse();
					}
				}
			}

			//3.�ƶ���Ʒѭ��
			g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �� [%s] ��ʼ�ƶ�ѭ��",	pHuman->GetName(), pDestHuman->GetName()) ;
			GCExchangeSuccessIII MsgSuccessToMe, MsgSuccessToOt;
			GCExchangeSuccessIII::_SUCC_ITEM ItemListToMe[EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE];
			INT	MyItemNum = 0;
			GCExchangeSuccessIII::_SUCC_ITEM ItemListToOt[EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE];
			INT	OtItemNum = 0;
			BOOL	bFlag = FALSE;

			//������¼���Խ�������Ʒ
			struct ExItem_t
			{
				UCHAR	uItemType;
				INT		nIndex;
				INT		nIndexInEx;
				ExItem_t()
				{
					uItemType	= 0;
					nIndex		= -1;
					nIndexInEx	= -1;
				}
			};

			INT				ExItemNumOt = 0;
			ExItem_t		ExItemIndexInOt[EXCHANGE_BOX_SIZE];
			INT				ExItemNumMy = 0;
			ExItem_t		ExItemIndexInMy[EXCHANGE_BOX_SIZE];
			INT				ExPetItemNumOt = 0;
			ExItem_t		ExPetItemIndexInOt[EXCHANGE_PET_BOX_SIZE];
			INT				ExPetItemNumMy = 0;
			ExItem_t		ExPetItemIndexInMy[EXCHANGE_PET_BOX_SIZE];

			for(INT i = 0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if( pMyExchangeContainer->GetItem(i) != NULL && pMyExchangeContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					Item* pItemRef = pMyExchangeContainer->GetItem(i);

					Assert( pItemRef ) ;

					INT BagIndex = -1;
					ItemContainer*	pBagContainer = HumanItemLogic::GetBagItemContain( pHuman,&pItemRef->GetGUID(), BagIndex);

					// ���˫��������˽��װ�ť���������һ��ʱ�����ݳ�������ʾ˫����ң���ȡ������
					if ( pBagContainer == NULL ) 
					{
						GCExchangeError MsgSelf;
						MsgSelf.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&MsgSelf);

						GCExchangeError MsgOther;
						MsgOther.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pDestHuman->GetPlayer()->SendPacket(&MsgOther);		

						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();

						return PACKET_EXE_ERROR;
					}
					ExItemIndexInMy[ExItemNumMy].uItemType	= pItemRef->GetItemClass();
					ExItemIndexInMy[ExItemNumMy].nIndex		= BagIndex;
					ExItemIndexInMy[ExItemNumMy].nIndexInEx	= i;
					ExItemNumMy++;
				}

				if( pOtExchangeContainer->GetItem(i) != NULL && pOtExchangeContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					Item* pItemRef = pOtExchangeContainer->GetItem(i);

					Assert( pItemRef );

					INT BagIndex = -1;
					ItemContainer*	pBagContainer = HumanItemLogic::GetBagItemContain( pDestHuman,&pItemRef->GetGUID(), BagIndex );
					
					if ( pBagContainer == NULL ) 
					{
						// ���˫��������˽��װ�ť���������һ��ʱ�����ݳ�������ʾ˫����ң���ȡ������
						GCExchangeError MsgSelf;
						MsgSelf.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&MsgSelf);

						GCExchangeError MsgOther;
						MsgOther.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pDestHuman->GetPlayer()->SendPacket(&MsgOther);	

						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();

						return PACKET_EXE_ERROR;
					}

					ExItemIndexInOt[ExItemNumOt].uItemType	= pItemRef->GetItemClass();
					ExItemIndexInOt[ExItemNumOt].nIndex		= BagIndex;
					ExItemIndexInOt[ExItemNumOt].nIndexInEx	= i;
					ExItemNumOt++;
				}
			}

			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				if( pMyExchangePetContainer->GetItem(i) != NULL && pMyExchangePetContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					Item* pItemRef = pMyExchangePetContainer->GetItem(i);

					Assert( pItemRef );

					ItemContainer*	pMyPetContainer = pHuman->GetPetContain();

					if ( pMyPetContainer == NULL )
					{
						// ���˫��������˽��װ�ť���������һ��ʱ�����ݳ�������ʾ˫����ң���ȡ������
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);

						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();

						return PACKET_EXE_ERROR;
					}

					ExPetItemIndexInMy[ExPetItemNumMy].nIndex = pMyPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());
					ExPetItemIndexInMy[ExPetItemNumMy].nIndexInEx = i;
					ExPetItemNumMy++;
				}

				if( pOtExchangePetContainer->GetItem(i) != NULL && pOtExchangePetContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					Item* pItemRef = pOtExchangePetContainer->GetItem(i);

					Assert( pItemRef );

					ItemContainer*	pOtPetContainer = pDestHuman->GetPetContain(); 

					if ( pOtPetContainer == NULL )
					{
						// ���˫��������˽��װ�ť���������һ��ʱ�����ݳ�������ʾ˫����ң���ȡ������
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);

						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();

						return PACKET_EXE_ERROR;
					}

					ExPetItemIndexInOt[ExPetItemNumOt].nIndex		= pOtPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());
					ExPetItemIndexInOt[ExPetItemNumOt].nIndexInEx = i;
					ExPetItemNumOt++;
				}
			}

			//3.1�Ȱ��ܽ����Ľ�����������ڶԷ��İ����ҵ������ڸ��Լ����׵���Ʒ��ֱ�ӽ�����������Ʒ��λ��
			for(INT i = 0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if( pMyExchangeContainer->GetItem(i) != NULL && pMyExchangeContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					INT result = 0;
					Item* pItemRef = pMyExchangeContainer->GetItem(i);

					Assert( pItemRef );

					//�Լ�������
					INT BagIndex = -1;
					ItemContainer*	pMyBagContainer = HumanItemLogic::GetBagItemContain( pHuman,&pItemRef->GetGUID(), BagIndex);

					if ( pMyBagContainer == NULL )
					{
						// ���˫��������˽��װ�ť���������һ��ʱ�����ݳ�������ʾ˫����ң���ȡ������
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);

						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();

						return PACKET_EXE_ERROR;
					}

					//ȥ���ܻ���
					for(INT j = 0; j<ExItemNumOt; j++)
					{

						//��λ���Ѿ���Ч
						if(ExItemIndexInOt[j].nIndex == -1)
							continue;

						//ֻ��ͬһ���͵���Ʒ���ܹ�����
						if(pItemRef->GetItemClass()!=ExItemIndexInOt[j].uItemType)
							continue;

						//�Է�������
						ItemContainer*	pOtBagContainer = HumanItemLogic::GetBagContainer(pDestHuman, ExItemIndexInOt[j].nIndex);

						if ( pOtBagContainer == NULL )
						{
							// ���˫��������˽��װ�ť���������һ��ʱ�����ݳ�������ʾ˫����ң���ȡ������
							GCExchangeError Msg;
							Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
							pHuman->GetPlayer()->SendPacket(&Msg);
							pDestHuman->GetPlayer()->SendPacket(&Msg);

							pHuman->m_ExchangBox.CleanUp();
							pDestHuman->m_ExchangBox.CleanUp();

							return PACKET_EXE_ERROR;
						}

						if ((!pOtBagContainer->IsCanUse())||(!pMyBagContainer->IsCanUse()))
						{
							GCExchangeError Msg;
							Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
							pHuman->GetPlayer()->SendPacket(&Msg);
							pDestHuman->GetPlayer()->SendPacket(&Msg);
							pHuman->m_ExchangBox.CleanUp();
							pDestHuman->m_ExchangBox.CleanUp();
							g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] <-> [%s] ��Ʒ [%d]ʧ�� result = %d", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetItemTableIndex(), result );
							return PACKET_EXE_CONTINUE;
						}
						//�����Լ�
						g_ItemOperator.UnlockItem( pMyBagContainer, pMyBagContainer->BagIndex2ConIndex(BagIndex));

						//�Ƚ����Է�
						g_ItemOperator.UnlockItem( pOtBagContainer, pOtBagContainer->BagIndex2ConIndex(ExItemIndexInOt[j].nIndex));

						//�����Լ���Ʒ�Ѿ����ǽ�����Ʒ��
						pMyBagContainer->GetItem(pMyBagContainer->BagIndex2ConIndex(BagIndex))->SetInExchange(FALSE);

						//���öԷ���Ʒ�Ѿ����ǽ�����Ʒ��
						pOtBagContainer->GetItem(pOtBagContainer->BagIndex2ConIndex(ExItemIndexInOt[j].nIndex))->SetInExchange(FALSE);

						//��һ���Է������׵���Ʒ����
						result = 
							g_ItemOperator.ExchangeItem
							(
							pMyBagContainer,
							pMyBagContainer->BagIndex2ConIndex(BagIndex),
							pOtBagContainer,
							pOtBagContainer->BagIndex2ConIndex(ExItemIndexInOt[j].nIndex)
							);
						if(result<0)
						{//����ʧ��
							GCExchangeError Msg;
							Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
							pHuman->GetPlayer()->SendPacket(&Msg);
							pDestHuman->GetPlayer()->SendPacket(&Msg);
							pHuman->m_ExchangBox.CleanUp();
							pDestHuman->m_ExchangBox.CleanUp();
							g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] <-> [%s] ��Ʒ [%d]ʧ�� result = %d", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetItemTableIndex(), result );
							return PACKET_EXE_CONTINUE;
						}
						else
						{
							ITEM_LOG_PARAM	ItemLogParam;
							ItemLogParam.OpType		=	ITEM_EXCHANGE_TOOTHER;
							ItemLogParam.CharGUID	=	pHuman->GetGUID();
							ItemLogParam.TargetGUID	=	pDestHuman->GetGUID();
							ItemLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
							ItemLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
							ItemLogParam.SceneID	=	pHuman->getScene()->SceneID();
							ItemLogParam.ItemGuid	=	pItemRef->GetGUID();
							ItemLogParam.ItemType	=	pItemRef->GetItemTableIndex();

							ItemListToOt[OtItemNum].m_FromType	= 0;
							ItemListToOt[OtItemNum].m_FromIndex = i;
							ItemListToOt[OtItemNum].m_ToType	= EXCHANGE_MSG::POS_BAG;
							ItemListToOt[OtItemNum++].m_ToIndex	= ExItemIndexInOt[j].nIndex;

							ItemListToMe[MyItemNum].m_FromType	= 0;
							ItemListToMe[MyItemNum].m_FromIndex = ExItemIndexInOt[j].nIndexInEx;
							ItemListToMe[MyItemNum].m_ToType	= EXCHANGE_MSG::POS_BAG;
							ItemListToMe[MyItemNum++].m_ToIndex	= BagIndex;

							//�ӽ��׺���ɾ������Ʒ,�ں�����ƶ��оͲ������ƶ�����
							g_ItemOperator.EraseItem(pMyExchangeContainer, i);
							g_ItemOperator.EraseItem(pOtExchangeContainer, ExItemIndexInOt[j].nIndexInEx);
							ExItemIndexInOt[j].nIndex = -1;

							g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] <-> [%s] ��Ʒ [%d]�ɹ�", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetItemTableIndex() );

							break;
						}
					}
				}
			}

			//3.2���ܽ�����һ��һ���Ž�ȥ��ʣ�µ�ֱ����ϵͳ�Ҹ����
			for(INT i = 0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if( pMyExchangeContainer->GetItem(i) != NULL && pMyExchangeContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					INT result = 0;
					Item* pItemRef = pMyExchangeContainer->GetItem(i);

					Assert( pItemRef );

					//�Լ�������
					INT BagIndex = -1;
					ItemContainer*	pMyBagContainer = HumanItemLogic::GetBagItemContain( pHuman,&pItemRef->GetGUID(), BagIndex);

					if (pMyBagContainer == NULL) 
					{
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);

						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						
						return PACKET_EXE_ERROR;
					}

					UINT conIndex = pMyBagContainer->BagIndex2ConIndex(BagIndex);
					//�Ƚ���
					g_ItemOperator.UnlockItem( pMyBagContainer, conIndex );
					pMyBagContainer->GetItem(conIndex)->SetInExchange(FALSE);


					//�Զ��Ҹ�֧���Զ�����
					//������Զ��Ҹ�ĳɿ����Զ����ӣ��������һ�����⣬������ȡ���ʱ�ǲ����ǵ���״̬�ģ����ԣ�����ǵ���״̬�µ�
					//������ȡһϵ����Ʒ���ɵ��ӵ�һ�����Խ��ܡ����ķ�Χ���һ�㡣
					ITEM_LOG_PARAM ItemLogParam;
					result = -1;
					BOOL Ret = HumanItemLogic::MoveItem(&ItemLogParam, pHuman, pMyBagContainer, conIndex, pDestHuman, result);
					if(!Ret)
					{//����ʧ��
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] -> [%s] ��Ʒ [%d]ʧ�� result = %d", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetItemTableIndex(), result );
						return PACKET_EXE_CONTINUE;
					}
					else
					{
						ITEM_LOG_PARAM	ItemLogParam;
						ItemLogParam.OpType		=	ITEM_EXCHANGE_TOOTHER;
						ItemLogParam.CharGUID	=	pHuman->GetGUID();
						ItemLogParam.TargetGUID	=	pDestHuman->GetGUID();
						ItemLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
						ItemLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
						ItemLogParam.SceneID	=	pHuman->getScene()->SceneID();
						ItemLogParam.ItemGuid	=	pItemRef->GetGUID();
						ItemLogParam.ItemType	=	pItemRef->GetItemTableIndex();

						ItemListToOt[OtItemNum].m_FromType	= 0;
						ItemListToOt[OtItemNum].m_FromIndex = i;
						ItemListToOt[OtItemNum].m_ToType	= EXCHANGE_MSG::POS_BAG;
						ItemListToOt[OtItemNum++].m_ToIndex	= result;
						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] -> [%s] ��Ʒ [%d]�ɹ�", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetItemTableIndex() );
					}
				}
				if( pOtExchangeContainer->GetItem(i) != NULL && pOtExchangeContainer->GetItem(i)->IsEmpty() == FALSE )
				{
					INT result = 0;
					Item* pItemRef = pOtExchangeContainer->GetItem(i);

					Assert( pItemRef );

					//�Է�������
					INT BagIndex = -1;
					ItemContainer*	pOtBagContainer = HumanItemLogic::GetBagItemContain( pDestHuman,&pItemRef->GetGUID(), BagIndex);

					if ( pOtBagContainer == NULL )
					{
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						return PACKET_EXE_ERROR;
					}

					UINT conIndex = pOtBagContainer->BagIndex2ConIndex(BagIndex);
					//�Ƚ���
					g_ItemOperator.UnlockItem( pOtBagContainer, conIndex );

					pOtBagContainer->GetItem(conIndex)->SetInExchange(FALSE);


					//�Զ��Ҹ�֧���Զ�����
					ITEM_LOG_PARAM ItemLogParam;
					result = -1;
					BOOL Ret = HumanItemLogic::MoveItem(&ItemLogParam, pDestHuman, pOtBagContainer, conIndex, pHuman, result);
					if(!Ret)
					{//����ʧ��
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] -> [%s] ��Ʒ [%d]ʧ�� result = %d", pDestHuman->GetName(), pHuman->GetName(), pItemRef->GetItemTableIndex(), result );
						return PACKET_EXE_CONTINUE;
					}
					else
					{
						ITEM_LOG_PARAM	ItemLogParam;
						ItemLogParam.OpType		=	ITEM_EXCHANGE_TOOTHER;
						ItemLogParam.CharGUID	=	pDestHuman->GetGUID();
						ItemLogParam.TargetGUID	=	pHuman->GetGUID();
						ItemLogParam.XPos		=	pDestHuman->getWorldPos()->m_fX;
						ItemLogParam.ZPos		=	pDestHuman->getWorldPos()->m_fZ;
						ItemLogParam.SceneID	=	pDestHuman->getScene()->SceneID();
						ItemLogParam.ItemGuid	=	pItemRef->GetGUID();
						ItemLogParam.ItemType	=	pItemRef->GetItemTableIndex();

						SaveItemLog(&ItemLogParam);
						ItemListToMe[MyItemNum].m_FromType	= 0;
						ItemListToMe[MyItemNum].m_FromIndex = i;
						ItemListToMe[MyItemNum].m_ToType	= EXCHANGE_MSG::POS_BAG;
						ItemListToMe[MyItemNum++].m_ToIndex	= result;
						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] -> [%s] ��Ʒ [%d]�ɹ�", pDestHuman->GetName(), pHuman->GetName(), pItemRef->GetItemTableIndex() );
					}
				}
			}

			//3.3������
			g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �� [%s] ��ʼ��������",	pHuman->GetName(), pDestHuman->GetName()) ;

			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				INT result = 0;
				Item* pItemRef = pMyExchangePetContainer->GetItem(i);

				if(pItemRef->IsEmpty())
					continue;

				PET_LOG_PARAM	PetLogParam;
				PetLogParam.PetGUID		=	pItemRef->GetPetGUID();
				PetLogParam.DataID		=	pItemRef->GetDataID();
				//�Լ�������
				ItemContainer*	pMyPetContainer = pHuman->GetPetContain();
	
				if ( pMyPetContainer == NULL )
				{
					GCExchangeError Msg;
					Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
					pHuman->GetPlayer()->SendPacket(&Msg);
					pDestHuman->GetPlayer()->SendPacket(&Msg);
					pHuman->m_ExchangBox.CleanUp();
					pDestHuman->m_ExchangBox.CleanUp();
					return PACKET_EXE_ERROR;
				}

				INT	PetIndexInBag = pMyPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());

				//�Է�������
				ItemContainer*	pOtPetContainer = pDestHuman->GetPetContain();

				if ( pOtPetContainer == NULL )
				{
					GCExchangeError Msg;
					Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
					pHuman->GetPlayer()->SendPacket(&Msg);
					pDestHuman->GetPlayer()->SendPacket(&Msg);
					pHuman->m_ExchangBox.CleanUp();
					pDestHuman->m_ExchangBox.CleanUp();
					return PACKET_EXE_ERROR;
				}

				//ȥ���ܻ���
				for(INT j = 0; j<ExPetItemNumOt; j++)
				{
					//�����Ч
					if(ExPetItemIndexInOt[j].nIndex == -1)
						continue;

					//�����Լ�
					g_ItemOperator.UnlockItem( pMyPetContainer, PetIndexInBag );

					//�Ƚ����Է�
					g_ItemOperator.UnlockItem( pOtPetContainer, ExPetItemIndexInOt[j].nIndex );

					//�����Լ���Ʒ�Ѿ����ǽ�����Ʒ��
					pMyPetContainer->GetItem(PetIndexInBag)->SetInExchange(FALSE);

					//���öԷ���Ʒ�Ѿ����ǽ�����Ʒ��
					pOtPetContainer->GetItem(ExPetItemIndexInOt[j].nIndex )->SetInExchange(FALSE);

					//��һ���Է������׵���Ʒ����
					result = 
						g_ItemOperator.ExchangeItem
						(
						pMyPetContainer,
						PetIndexInBag,
						pOtPetContainer,
						ExPetItemIndexInOt[j].nIndex
						);
					if(result<0)
					{//����ʧ��
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] <-> [%s] ��Ʒ [%d]ʧ�� result = %d", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetItemTableIndex(), result );
						return PACKET_EXE_CONTINUE;
					}
					else
					{
						PetLogParam.CharGUID	=	pDestHuman->GetGUID();
						PetLogParam.OPType		=	PET_OP_EXCHANGE_OTHER;
						PetLogParam.SceneID		=	pDestHuman->getScene()->SceneID();
						PetLogParam.TargetGUID	=	pHuman->GetGUID();
						PetLogParam.XPos		=	pDestHuman->getWorldPos()->m_fX;
						PetLogParam.ZPos		=	pDestHuman->getWorldPos()->m_fZ;
						SavePetLog(&PetLogParam);

						ItemListToMe[MyItemNum].m_FromType	= EXCHANGE_MSG::POS_PET;
						ItemListToMe[MyItemNum].m_FromIndex = ExPetItemIndexInOt[j].nIndexInEx;
						ItemListToMe[MyItemNum].m_ToType	= EXCHANGE_MSG::POS_PET;
						ItemListToMe[MyItemNum++].m_ToIndex	= PetIndexInBag;

						ItemListToOt[OtItemNum].m_FromType	= EXCHANGE_MSG::POS_PET;
						ItemListToOt[OtItemNum].m_FromIndex = i;
						ItemListToOt[OtItemNum].m_ToType	= EXCHANGE_MSG::POS_PET;
						ItemListToOt[OtItemNum++].m_ToIndex	= ExPetItemIndexInOt[j].nIndex;

						//�ӽ��׺���ɾ������Ʒ,�ں�����ƶ��оͲ������ƶ�����
						g_ItemOperator.EraseItem(pMyExchangePetContainer, i);
						g_ItemOperator.EraseItem(pOtExchangePetContainer, ExPetItemIndexInOt[j].nIndexInEx);

						ExPetItemIndexInOt[j].nIndex = -1;

						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] <-> [%s] ��Ʒ [%s]�ɹ�", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetName() );

						break;
					}
				}
			}

			//3.4�Ƴ���
			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				if( pMyExchangePetContainer->GetItem(i) != NULL && pMyExchangePetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					INT result = 0;
					Item* pItemRef = pMyExchangePetContainer->GetItem(i);

					PET_LOG_PARAM	PetLogParam;
					PetLogParam.PetGUID		=	pItemRef->GetPetGUID();
					PetLogParam.DataID		=	pItemRef->GetDataID();
					//�Լ�������
					ItemContainer*	pMyPetContainer = pHuman->GetPetContain();

					if ( pMyPetContainer == NULL )
					{
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						return PACKET_EXE_ERROR;
					}
					
					INT	PetIndexInBag = pMyPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());

					//�Է�������
					ItemContainer*	pOtPetContainer = pDestHuman->GetPetContain();

					if ( pOtPetContainer == NULL )
					{
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						return PACKET_EXE_ERROR;
					}

					//�Ƚ���
					g_ItemOperator.UnlockItem( pMyPetContainer, PetIndexInBag );

					pMyPetContainer->GetItem(PetIndexInBag)->SetInExchange(FALSE);

					// ��ǰ�ҵ�Ŀ��PetDB��ȷ��ֻ�����������׳ɹ����֪ͨ�������ģ�� wuwenbin
					PET_GUID_t petGuid;
					_PET_DB* pPetDB = pHuman->GetHorseDB(pHuman->GetHorseIndexByGUID(PetLogParam.PetGUID));
					if(pPetDB){
						petGuid = pPetDB->m_GUID;
					}

					//�Զ��Ҹ�֧���Զ�����
					//������Զ��Ҹ�ĳɿ����Զ����ӣ��������һ�����⣬������ȡ���ʱ�ǲ����ǵ���״̬�ģ����ԣ�����ǵ���״̬�µ�
					//������ȡһϵ����Ʒ���ɵ��ӵ�һ�����Խ��ܡ����ķ�Χ���һ�㡣
					result = 
						g_ItemOperator.MoveItem
						(
						pMyPetContainer,
						PetIndexInBag,
						pOtPetContainer
						);
					if(result<0)
					{//����ʧ��
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] -> [%s] ���� [%s]ʧ�� result = %d", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetName(), result );
						return PACKET_EXE_CONTINUE;
					}
					else
					{
						PetLogParam.CharGUID	=	pDestHuman->GetGUID();
						PetLogParam.OPType		=	PET_OP_EXCHANGE_OTHER;
						PetLogParam.SceneID		=	pDestHuman->getScene()->SceneID();
						PetLogParam.TargetGUID	=	pHuman->GetGUID();
						PetLogParam.XPos		=	pDestHuman->getWorldPos()->m_fX;
						PetLogParam.ZPos		=	pDestHuman->getWorldPos()->m_fZ;
						SavePetLog(&PetLogParam);

						ItemListToOt[OtItemNum].m_FromType	= EXCHANGE_MSG::POS_PET;
						ItemListToOt[OtItemNum].m_FromIndex = i;
						ItemListToOt[OtItemNum].m_ToType	= EXCHANGE_MSG::POS_PET;
						ItemListToOt[OtItemNum++].m_ToIndex	= result;

						// ���ｻ�׳ɹ�ʱ֪ͨ�������ģ�� [6/16/2008 wuwenbin]
						HorseFindMateModule::GetInstance().OnHorseChangeState(pHuman, petGuid, ST_TRADE);

						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] -> [%s] ���� [%s]�ɹ�", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetName() );
					}
				}

				if( pOtExchangePetContainer->GetItem(i) != NULL && pOtExchangePetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					INT result = 0;
					Item* pItemRef = pOtExchangePetContainer->GetItem(i);
					PET_LOG_PARAM	PetLogParam;
					PetLogParam.PetGUID		=	pItemRef->GetPetGUID();
					PetLogParam.DataID		=	pItemRef->GetDataID();

					//�Է�������
					ItemContainer*	pOtPetContainer = pDestHuman->GetPetContain();

					if ( pOtPetContainer == NULL )
					{
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						return PACKET_EXE_ERROR;
					}

					INT	PetIndexInBag = pOtPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());

					//�Լ�������
					ItemContainer*	pMyPetContainer = pHuman->GetPetContain();

					if ( pMyPetContainer == NULL )
					{
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						return PACKET_EXE_ERROR;
					}

					//�Ƚ���
					g_ItemOperator.UnlockItem( pOtPetContainer, PetIndexInBag );
					pOtPetContainer->GetItem(PetIndexInBag)->SetInExchange(FALSE);

					// ��ǰ�ҵ�Ŀ��PetDB��ȷ��ֻ�����������׳ɹ����֪ͨ�������ģ�� wuwenbin
					PET_GUID_t petGuid;
					_PET_DB* pPetDB = pDestHuman->GetHorseDB(pDestHuman->GetHorseIndexByGUID(PetLogParam.PetGUID));
					if(pPetDB){
						 petGuid = pPetDB->m_GUID;
					}

					//�Զ��Ҹ�֧���Զ�����
					result = 
						g_ItemOperator.MoveItem
						(
						pOtPetContainer,
						PetIndexInBag,
						pMyPetContainer
						);
					if(result<0)
					{//����ʧ��
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] -> [%s] ���� [%s]ʧ�� result = %d", pDestHuman->GetName(), pHuman->GetName(), pItemRef->GetName(), result );
						return PACKET_EXE_CONTINUE;
					}
					else
					{
						PetLogParam.CharGUID	=	pHuman->GetGUID();
						PetLogParam.OPType		=	PET_OP_EXCHANGE_OTHER;
						PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
						PetLogParam.TargetGUID	=	pDestHuman->GetGUID();
						PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
						PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
						SavePetLog(&PetLogParam);
						ItemListToMe[MyItemNum].m_FromType	= EXCHANGE_MSG::POS_PET;
						ItemListToMe[MyItemNum].m_FromIndex = i;
						ItemListToMe[MyItemNum].m_ToType	= EXCHANGE_MSG::POS_PET;
						ItemListToMe[MyItemNum++].m_ToIndex	= result;

						// ���ｻ�׳ɹ�ʱ֪ͨ�������ģ�� [6/16/2008 wuwenbin]
						HorseFindMateModule::GetInstance().OnHorseChangeState(pDestHuman, petGuid, ST_TRADE);

						g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] -> [%s] ���� [%s]�ɹ�", pDestHuman->GetName(), pHuman->GetName(), pItemRef->GetName() );
					}
				}
			}
			

			//2.��Ǯ
			g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �� [%s] ��ʼ������Ǯ",	pHuman->GetName(), pDestHuman->GetName()) ;
			if( pHuman->m_ExchangBox.m_Money <= pHuman->GetMoney() && pHuman->m_ExchangBox.m_Money>0 )
			{
				pDestHuman->SetMoney(pDestHuman->GetMoney()+pHuman->m_ExchangBox.m_Money);
				pHuman->SetMoney(pHuman->GetMoney()-pHuman->m_ExchangBox.m_Money);

				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pDestHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_EXCHANGE_OUTCOME;	
				MoneyLogParam.Count		=	pHuman->m_ExchangBox.m_Money;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);

				MoneyLogParam.CharGUID	=	pDestHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_EXCHANGE_INCOME;	
				MoneyLogParam.Count		=	pHuman->m_ExchangBox.m_Money;
				MoneyLogParam.SceneID	=	pDestHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pDestHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pDestHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);


				g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] -> [%s] ��Ǯ [%d]", pHuman->GetName(), pDestHuman->GetName(), pHuman->m_ExchangBox.m_Money );
			}

			if( pDestHuman->m_ExchangBox.m_Money <= pDestHuman->GetMoney() && pDestHuman->m_ExchangBox.m_Money>0 )
			{
				pHuman->SetMoney(pHuman->GetMoney()+pDestHuman->m_ExchangBox.m_Money);
				pDestHuman->SetMoney(pDestHuman->GetMoney() - pDestHuman->m_ExchangBox.m_Money);

				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pDestHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_EXCHANGE_INCOME;	
				MoneyLogParam.Count		=	pDestHuman->m_ExchangBox.m_Money;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);

				MoneyLogParam.CharGUID	=	pDestHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_EXCHANGE_OUTCOME;	
				MoneyLogParam.Count		=	pDestHuman->m_ExchangBox.m_Money;
				MoneyLogParam.SceneID	=	pDestHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pDestHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pDestHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);


				g_pLog->FastSaveLog( LOG_FILE_1, "		<����> [%s] -> [%s] ��Ǯ [%d]", pDestHuman->GetName(), pHuman->GetName(), pDestHuman->m_ExchangBox.m_Money );
			}


			//��˫�����ͳɹ���Ϣ
			MsgSuccessToMe.SetItemNum(MyItemNum);
			MsgSuccessToMe.SetItemList(ItemListToMe);
			pHuman->GetPlayer()->SendPacket(&MsgSuccessToMe);

			MsgSuccessToOt.SetItemNum(OtItemNum);
			MsgSuccessToOt.SetItemList(ItemListToOt);
			pDestHuman->GetPlayer()->SendPacket(&MsgSuccessToOt);

			g_pLog->FastSaveLog( LOG_FILE_1, "<����> [%s] �� [%s] ���׳ɹ�",	pHuman->GetName(), pDestHuman->GetName()) ;

			//���ߵ����֤�����в������������ո��ԵĽ��׺�
			pHuman->m_ExchangBox.CleanUp();
			pDestHuman->m_ExchangBox.CleanUp();

		}
		else
		{//ɶҲ����,�ȴ��Է���Ϣ

		}//if(pDestHuman->m_ExchangBox.m_Status ==  ServerExchangeBox::EXCHANGE_CONFIRM_READY)
	}
	else
	{//һ��������
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
		pHuman->GetPlayer()->SendPacket(&Msg);
		pHuman->m_ExchangBox.CleanUp();
		pDestHuman->GetPlayer()->SendPacket(&Msg);
		pDestHuman->m_ExchangBox.CleanUp();
		return PACKET_EXE_CONTINUE;
	}//if(pHuman->m_ExchangBox.m_Status == ServerExchangeBox::EXCHANGE_WAIT_FOR_CONFIRM)

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
