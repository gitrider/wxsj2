#include "stdafx.h"
#include "CGUseItem.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "AI_Human.h"
#include "Obj_Human.h"
#include "GCUseItemResult.h"
#include "GCItemInfo.h"
#include "HumanItemLogic.h"
#include "ScriptLogic.h"

using namespace Combat_Module;
using namespace Action_Module;
using namespace Script_Module;

#define CLASS_OF_IDENT(x)	(x->GetItemClass()==ICLASS_IDENT && (x->GetItemType()==IDENT_NCIDENT || x->GetItemType()==IDENT_ARIDENT||x->GetItemType()==IDENT_WPIDENT))

uint CGUseItemHandler::Execute(CGUseItem* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
	USEITEM_RESULT	Result ;
	
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
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );
	//����״̬���ɲ���
	if(pHuman->m_ExchangBox.m_Status > 0)
	{//����
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ExchangBox::m_Status>0" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	//��̯״̬���ɲ���
	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{//����
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ServerStallBox::STALL_OPEN" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	//����Ƿ����ʹ����Ʒ
	ID_t	BagIndex = pPacket->GetBagIndex();

	//�����Ʒ�Ƿ�Ϸ�
	Item*	pBagItem = HumanItemLogic::GetBagItem(pHuman,BagIndex);
	if (pBagItem == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseItemHandler ERROR BagIndex=%d", BagIndex) ;
		return PACKET_EXE_ERROR ;
	}
	if(pBagItem->IsEmpty())
	{
		//Assert(FALSE) ; //��Ʒ�ò���
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseItemHandler Can't GetItem	BagIndex=%d", BagIndex) ;
		return PACKET_EXE_CONTINUE ;
	}
	ObjID_t	Target = pHuman->GetID();
	//1.0 ��ǰ�汾ʹ����Ʒֻ�����ҩ  
	//1.1 �汾ʹ����Ʒ����ҩƷ,�ر�ͼ,�䷽  
	if(!pBagItem->IsRuler(IRL_CANUSE))
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseItemHandler Can't use item BagIndex=%d", BagIndex);
		return PACKET_EXE_CONTINUE ;
	}
	if(pHuman->GetLevel()>=pBagItem->GetItemLevel()) //������
	{
		switch(pBagItem->GetItemClass()) 
		{
			case ICLASS_TASKITEM: //������Ʒ��
			case ICLASS_COMITEM: //ҩƷ��ʹ��
				{
					ScriptID_t nScript = pBagItem->GetScriptID();
					SkillID_t nSkill = pBagItem->GetSkillID();
					if(INVALID_ID!= nScript)
					{
						if(FALSE == g_ScriptLogic.Item_IsSkillLikeScript(*pHuman, nScript))
						{
							g_ScriptLogic.Item_CallDefaultEvent(*pHuman, nScript, BagIndex);
							break;
						}
						else if(FALSE == g_ScriptLogic.Script_CancelImpacts(*pHuman, nScript))
						{
							//ȡ��Ч��ʧ��
							PET_GUID_t	PetGUID = pPacket->GetTargetPetGUID();

							Result = (USEITEM_RESULT)pHuman->GetHumanAI()->PushCommand_UseItem(
								pPacket->GetBagIndex(), 
								pPacket->GetTargetObjID(),
								*(pPacket->GetTargetPos()),
								PetGUID,
								pPacket->GetTargetItem() 
								);
						}
						else
						{
							Result = USEITEM_SUCCESS;
						}
					}
				}
				break;
			case ICLASS_IDENT:
				{
					Result = (USEITEM_RESULT)pHuman->UseIdentScroll(pPacket->GetBagIndex(),pPacket->GetTargetItem());
				}
				break;
			case ICLASS_STOREMAP://�ر�ͼ
				{
					Result	=	(USEITEM_RESULT)pHuman->UseStoreMap(BagIndex);
				}
				break;

			case ICLASS_SOUL_BEAD://����
				{
					SOUL_BEAD_INFO_TB* pGet = g_ItemTable.GetSoulBeadTB(pBagItem->GetItemTableIndex());
					Assert(NULL != pGet);
					ImpactID_t nImpactId = pGet->m_nImpactId;

					//�Ѿ��л��鸽���״̬
					OWN_IMPACT* pImpact = pHuman->Impact_GetFirstImpactOfSpecificMutexID(nImpactId);
					if( NULL != pImpact )
					{
						g_pLog->FastSaveLog( LOG_FILE_1, "CGUseItemHandler Can't use item BagIndex=%d", BagIndex);
						return PACKET_EXE_CONTINUE ;
					}
	
					if(INVALID_ID != nImpactId)
					{
						if(g_ImpactCore.SendImpactToUnit(*(Obj_Character*)pHuman, nImpactId, pHuman->GetID(), pGet->m_nValidTime, FALSE, 0, BagIndex))
						{
							Result = USEITEM_SUCCESS;
							//ɾ���������
							ITEM_LOG_PARAM	ItemLogParam;
							ItemLogParam.OpType	= ITEM_USE;
							BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam, pHuman, BagIndex);
							if (bRet)
							{
								SaveItemLog(&ItemLogParam);
							}
						}
					}
				}
				break;
			default:
				{
					Result	=	USEITEM_INVALID;
				}
				break;
		}
	}
	else
	{
		Result = USEITEM_LEVEL_FAIL;
	}
	GCUseItemResult	Msg;
	Msg.SetResult(Result);
	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGUseItemHandler BagIndex=%d Result=%d",
		BagIndex, Result  ) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

