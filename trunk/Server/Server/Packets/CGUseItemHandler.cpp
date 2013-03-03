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
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );
	//交易状态不可操作
	if(pHuman->m_ExchangBox.m_Status > 0)
	{//丢弃
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ExchangBox::m_Status>0" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	//摆摊状态不可操作
	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{//丢弃
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: ObjID=%d, ServerStallBox::STALL_OPEN" ,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	//检查是否可以使用物品
	ID_t	BagIndex = pPacket->GetBagIndex();

	//检查物品是否合法
	Item*	pBagItem = HumanItemLogic::GetBagItem(pHuman,BagIndex);
	if (pBagItem == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseItemHandler ERROR BagIndex=%d", BagIndex) ;
		return PACKET_EXE_ERROR ;
	}
	if(pBagItem->IsEmpty())
	{
		//Assert(FALSE) ; //物品得不到
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseItemHandler Can't GetItem	BagIndex=%d", BagIndex) ;
		return PACKET_EXE_CONTINUE ;
	}
	ObjID_t	Target = pHuman->GetID();
	//1.0 当前版本使用物品只处理吃药  
	//1.1 版本使用物品处理药品,藏宝图,配方  
	if(!pBagItem->IsRuler(IRL_CANUSE))
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseItemHandler Can't use item BagIndex=%d", BagIndex);
		return PACKET_EXE_CONTINUE ;
	}
	if(pHuman->GetLevel()>=pBagItem->GetItemLevel()) //级别检查
	{
		switch(pBagItem->GetItemClass()) 
		{
			case ICLASS_TASKITEM: //任务物品类
			case ICLASS_COMITEM: //药品的使用
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
							//取消效果失败
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
			case ICLASS_STOREMAP://藏宝图
				{
					Result	=	(USEITEM_RESULT)pHuman->UseStoreMap(BagIndex);
				}
				break;

			case ICLASS_SOUL_BEAD://魂珠
				{
					SOUL_BEAD_INFO_TB* pGet = g_ItemTable.GetSoulBeadTB(pBagItem->GetItemTableIndex());
					Assert(NULL != pGet);
					ImpactID_t nImpactId = pGet->m_nImpactId;

					//已经有魂珠附体的状态
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
							//删除魂珠道具
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

