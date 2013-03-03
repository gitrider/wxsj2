#include "stdafx.h"
#include "CGPickBoxItem.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "Obj_ItemBox.h"
#include "GCPickResult.h"
#include "GCMyBagList.h"
#include "ItemRuler.h"
#include "GCItemInfo.h"
#include "GameTable.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "GCNotifyEquip.h"

using namespace Packets;




uint CGPickBoxItemHandler::Execute(CGPickBoxItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


	//GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	//Assert( pGamePlayer ) ;

	//Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	//Assert( pHuman ) ;

	//Scene* pScene = pHuman->getScene() ;
	//if( pScene==NULL )
	//{
	//	Assert(FALSE) ;
	//	return PACKET_EXE_ERROR ;
	//}

	////����߳�ִ����Դ�Ƿ���ȷ
	//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	//
	//ObjID_t	 ItemBoxObjID	=	pPacket->getItemBoxId();

	//if( ItemBoxObjID == INVALID_ID )
	//{
	//	Assert(FALSE) ;
	//	return PACKET_EXE_ERROR ;
	//}
	//
	//_ITEM_GUID	 ItemObjID	=	pPacket->getItemId();

	//if(ItemObjID.m_Serial == -1 || ItemObjID.m_Server == -1 || ItemObjID.m_World == -1)
	//{
	//	Assert(FALSE) ;
	//	return PACKET_EXE_ERROR ;
	//}

	//Obj* pObj = pScene->GetObjManager()->GetObj( ItemBoxObjID ) ;

	//if( pObj==NULL )
	//{
	//	g_pLog->FastSaveLog( LOG_FILE_1, "CGPickBoxItem: not find obj=%d", ItemBoxObjID ) ;
	//	return PACKET_EXE_CONTINUE ;
	//}

	//Obj_ItemBox*	pItemBox = (Obj_ItemBox*)pObj;

	//if( pItemBox->GetObjType()!= Obj::OBJ_TYPE_ITEM_BOX)
	//{
	//	g_pLog->FastSaveLog( LOG_FILE_1, "CGPickBoxItem: Obj_ItemBox obj=%d\r\n  is Not class of ITEM_CLASS_BOX", ItemBoxObjID ) ;
	//	return PACKET_EXE_CONTINUE ;
	//}


	//
	//PickResultCode	ResultCode = PICK_INVALID_ITEM;
	//GCPickResult	 Msg;
	//uint			uBagIndex;	
	//INT				tableIndex;
	//GCItemInfo		ChangeItemMsg;
	//
	//if(pHuman->IsInValidRadius(pObj->getWorldPos(),MAX_BOX_OP_DISTANCE)==FALSE)
	//{
	//	Msg.setResult(PICK_TOO_FAR);
	//	pGamePlayer->SendPacket( &Msg );
	//	return PACKET_EXE_CONTINUE ;
	//}

	//if(!pHuman->IsAlive())
	//{
	//	Msg.setResult(PICK_DEAD);
	//	pGamePlayer->SendPacket( &Msg );
	//	return PACKET_EXE_CONTINUE ;
	//}

	//
	//

	//if(pItemBox->GetType()!=ITYPE_DROPBOX)
	//{	

	//	/*
	//	 *	����������ItemBox ʰȡ����
	//	 */
	//	if(pItemBox->GetOpenFlag())
	//	{

	//			for(INT i =0;i<MAX_BOXITEM_NUMBER;i++)
	//			{

	//				ItemContainer*	pIBContainer = pItemBox->GetContainer();
	//				Assert(pIBContainer);

	//				if(!pIBContainer)
	//				{
	//					AssertEx(FALSE,"ItemBox ��IItemContainer �쳣");	    
	//					return PACKET_EXE_CONTINUE;
	//				}

	//				Item*	pSourItem	= pIBContainer->GetItem(i);


	//				if(pSourItem->GetGUID() == ItemObjID)
	//				{
	//						//���Һ��ʵ�λ��

	//					
	//					ITEM_LOG_PARAM	ItemLogParam;
	//					ItemLogParam.OpType		= ITEM_PICK_FROM_GROWPOINT;
	//					ItemLogParam.CharGUID	= pHuman->GetGUID();
	//					ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
	//					ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
	//					ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;

	//					if(HumanItemLogic::MoveItem(&ItemLogParam,pHuman,pIBContainer,i,uBagIndex))
	//					{
	//							SaveItemLog(&ItemLogParam);
	//							Item* pBagItem = HumanItemLogic::GetBagItem(pHuman,uBagIndex);
	//							if(!pBagItem)
	//							{
	//								Assert(FALSE);
	//							}
	//							ResultCode = PICK_SUCCESS;
	//							Msg.setBagIndex(uBagIndex);
	//							Msg.setItemTableIndex(pSourItem->GetItemTableIndex());
	//							Msg.setItemBoxId(ItemBoxObjID);
	//							Msg.setItemID(ItemObjID);
	//							Msg.setBagItemGUID(pBagItem->GetGUID());
	//							
	//							//��������
	//							pItemBox->SetItemCount(pItemBox->GetItemCount()-1);
	//							

	//							if(pItemBox->GetItemCount()<=0)
	//							{
	//								pItemBox->SetOpenFlag(FALSE);

	//								INT ItemBoxType			 =	pItemBox->GetType();
	//								_GROW_POINT_INFO*	pGET =  g_GrowPointInfoTbl.Get(ItemBoxType);
	//								Assert(pGET);
	//								if(pGET->m_ScriptID>0)
	//								{

	//									if(pScene->GetGrowPointManager()->CallScriptRecycleFunc(pGET->m_ScriptID,pHuman->GetID(),ItemBoxObjID,pScene->SceneID()))
	//									{	
	//										//����Recycle
	//										pItemBox->Recycle();
	//									}
	//									else
	//									{	//��������

	//									}

	//								}
	//								else
	//								{
	//									Assert(FALSE); //û���ṩ���պ���
	//								}
	//								
	//							}
	//							
	//							//��������仯
	//							Item* pItem =	HumanItemLogic::GetBagItem(pHuman,uBagIndex);
	//							Assert(pItem);
	//							
	//							//������
	//							if(pItem->IsRuler(IRL_PICKBIND))
	//							{
	//								
	//								ItemContainer* pContainer = HumanItemLogic::GetContainer(pHuman,uBagIndex);
	//								if(pContainer)
	//								{
	//									INT ContainerIndex = pContainer->BagIndex2ConIndex(uBagIndex);
	//									g_ItemOperator.SetItemBind(pContainer,ContainerIndex);
	//								}
	//							}
	//							
	//							if(pItem->IsCanLay())
	//							{
	//								
	//								ChangeItemMsg.setID(uBagIndex);

	//								if(pItem->GetLayedNum()>0)
	//								{
	//									ChangeItemMsg.setIsNull(FALSE);
	//								}
	//								else
	//									ChangeItemMsg.setIsNull(TRUE);
	//								
	//								pItem->SaveValueTo(ChangeItemMsg.getItem());
	//								
	//							}
	//							else
	//							{
	//								GCNotifyEquip	Msg;
	//								Msg.SetBagIndex(uBagIndex);
	//								pItem->SaveValueTo(Msg.GetItem());
	//								pGamePlayer->SendPacket(&Msg);
	//							}
	//						}
	//						else
	//							ResultCode	=	PICK_BAG_FULL;

	//						break;
	//					}

	//			}

	//	
	//	}
	//	else
	//	{
	//		ResultCode	=	PICK_CLOSED_BOX;
	//	}

	//	Msg.setResult(ResultCode);
	//	pGamePlayer->SendPacket( &Msg );
	//	if(ResultCode == PICK_SUCCESS)
	//		pGamePlayer->SendPacket(&ChangeItemMsg);
	//}
	//else
	//{
	//	/*
	//	 *	�����ʰȡ����
	//	 */
	//		if(pItemBox->CanPickBox(pHuman->GetGUID(),pHuman->GetID()))
	//		{
	//			
	//			for(INT i =0;i<MAX_BOXITEM_NUMBER;i++)
	//			{
	//				
	//				ItemContainer*	pIBContainer = pItemBox->GetContainer();
	//				Assert(pIBContainer);

	//				if(!pIBContainer)
	//				{
	//					AssertEx(FALSE,"ItemBox ��IItemContainer �쳣");	    
	//					return PACKET_EXE_CONTINUE;
	//				}

	//				Item*	pSourItem	= pIBContainer->GetItem(i);
	//				
	//				if(pSourItem->GetGUID() == ItemObjID)
	//				{
	//					//Ȩ�޼��
	//					if(!(pItemBox->CanPickBox(pHuman->GetGUID(),pHuman->GetID())))
	//						return	PICK_INVALID_OWNER;

	//					
	//					ITEM_LOG_PARAM	ItemLogParam;
	//					ItemLogParam.OpType		= ITEM_PICK_FROM_MONSTER;
	//					ItemLogParam.CharGUID	= pHuman->GetGUID();
	//					ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
	//					ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
	//					ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
	//					//���Һ��ʵ�λ��

	//					if(HumanItemLogic::MoveItem(&ItemLogParam,pHuman,pIBContainer,i,uBagIndex))
	//					{
	//						SaveItemLog(&ItemLogParam);
	//						Item* pBagItem = HumanItemLogic::GetBagItem(pHuman,uBagIndex);
	//						if(!pBagItem)
	//						{
	//							Assert(FALSE);
	//						}

	//						ResultCode = PICK_SUCCESS;
	//						Msg.setBagIndex(uBagIndex);
	//						tableIndex = pItemBox->GetItem(i)->m_ItemIndex;
	//						Msg.setItemTableIndex(tableIndex);
	//						Msg.setItemBoxId(ItemBoxObjID);
	//						Msg.setItemID(ItemObjID);
	//						Msg.setBagItemGUID(pBagItem->GetGUID());		
	//					
	//						//��������
	//						pItemBox->SetItemCount(pItemBox->GetItemCount()-1);

	//						if(pItemBox->GetItemCount()<=0)
	//						{
	//							pItemBox->Recycle();	
	//						}

	//						//��������仯
	//						//const	_ITEM* pBagItem = pHuman->GetItem(uBagIndex);
	//						Item* pItem =	HumanItemLogic::GetBagItem(pHuman,uBagIndex);
	//						Assert(pItem);

	//						//������
	//						if(pItem->IsRuler(IRL_PICKBIND))
	//						{

	//							ItemContainer* pContainer = HumanItemLogic::GetContainer(pHuman,uBagIndex);
	//							if(pContainer)
	//							{
	//								INT ContainerIndex = pContainer->BagIndex2ConIndex(uBagIndex);
	//								g_ItemOperator.SetItemBind(pContainer,ContainerIndex);
	//							}
	//						}

	//						if(pItem->IsCanLay())
	//						{
	//							ChangeItemMsg.setID(uBagIndex);

	//							if(pItem->GetLayedNum()>0)
	//							{
	//								ChangeItemMsg.setIsNull(FALSE);
	//							}
	//							else
	//								ChangeItemMsg.setIsNull(TRUE);

	//							pItem->SaveValueTo(ChangeItemMsg.getItem());
	//						}
	//						else
	//						{
	//							GCNotifyEquip	Msg;
	//							Msg.SetBagIndex(uBagIndex);
	//							pItem->SaveValueTo(Msg.GetItem());
	//								pGamePlayer->SendPacket(&Msg);
	//						}
	//							
	//					}
	//					else
	//						ResultCode	=	PICK_BAG_FULL;
	//					break;
	//				}

	//			}

	//		

	//		}
	//		else
	//		{
	//			
	//			ResultCode = PICK_INVALID_OWNER;
	//			
	//		}
	//		
	//		Msg.setResult(ResultCode);
	//		//���ͽ��
	//		pGamePlayer->SendPacket( &Msg ) ;
	//		
	//		//������Ʒ�仯
	//		if(ResultCode == PICK_SUCCESS) pGamePlayer->SendPacket(&ChangeItemMsg);
	//}


	//g_pLog->FastSaveLog( LOG_FILE_1, "CGPickBoxItem: ok ObjId=", ItemBoxObjID ) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}