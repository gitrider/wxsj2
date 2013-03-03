#include "stdafx.h"
#include "CGOpenBoxManipulate.h"
#include "GamePlayer.h"
#include "GCOpenBoxManipulate.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"


UINT 
CGOpenBoxManipulateHandler::Execute( CGOpenBoxManipulate* pPacket, Player* pPlayer ) 
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );


	GCOpenBoxManipulate Msg;

	//if ( !pHuman->GetHumanOpenBox()->GetState() )
	//{
	//	Assert(FALSE);
	//}

	switch ( pPacket->GetManipulate() )
	{
	case CGOpenBoxManipulate::MANIPULATE_GET_DIRECTLY:
		{
			 Msg.SetManipulate( GCOpenBoxManipulate::MANIPULATE_GET_DIRECTLY );

			 ItemContainer* pContainer = pHuman->GetHumanOpenBox()->GetContainer();

			 if ( pContainer )
			 {
				 if ( HumanItemLogic::RecieveItemToBag( pHuman, pContainer, HumanOpenBox::CENTER_ITEM_INDEX ) )
				 {

					 Msg.SetRet( GCOpenBoxManipulate::MANIPULATE_NORMAL );
					 //清掉Obj_Human上的容器
					 pHuman->GetHumanOpenBox()->CleanUp();

				 }
				 else
				 {
					 Msg.SetRet( GCOpenBoxManipulate::MANIPULATE_BAG_FULL );

				 }

				 pHuman->GetPlayer()->SendPacket(&Msg);
				 g_pLog->FastSaveLog( LOG_FILE_1, "CGOpenBoxManipulateHandler::Execute() MANIPULATE_GET_DIRECTLY,Ret=%d",
					 Msg.GetRet());

				 return PACKET_EXE_CONTINUE;

			 }
			 g_pLog->FastSaveLog( LOG_FILE_1, "CGOpenBoxManipulateHandler::Execute() MANIPULATE_GET_DIRECTLY, GetHumanOpenBox error, PlayerGUID=%X",pHuman->GetGUID());

			 Assert(FALSE);

		}
		break;

	case CGOpenBoxManipulate::MANIPULATE_ONCE_MORE:
		{
			Msg.SetManipulate( GCOpenBoxManipulate::MANIPULATE_ONCE_MORE );

			ItemContainer* pContainer = pHuman->GetHumanOpenBox()->GetContainer();
			if(pContainer)
			{

				Item* pItem = pContainer->GetItem( HumanOpenBox::RAND_ITEM_INDEX );
				if(pItem)
				{

					if(!pItem->IsEmpty())
					{
						Msg.SetGUID( pItem->GetGUID() );
						Msg.SetRet( GCOpenBoxManipulate::MANIPULATE_NORMAL );
						g_ItemOperator.EraseItem( pContainer, HumanOpenBox::CENTER_ITEM_INDEX );   //删掉中间位置的物品
						pHuman->GetPlayer()->SendPacket(&Msg);
						g_pLog->FastSaveLog( LOG_FILE_1, "CGOpenBoxManipulateHandler::Execute() MANIPULATE_ONCE_MORE,Ret=%d",
						        Msg.GetRet());

						return PACKET_EXE_CONTINUE; 
					}
				}

			}
			g_pLog->FastSaveLog( LOG_FILE_1, "CGOpenBoxManipulateHandler::Execute() MANIPULATE_ONCE_MORE, GetHumanOpenBox error, PlayerGUID=%X",pHuman->GetGUID());

             Assert(FALSE);

		}
		break;

	case CGOpenBoxManipulate::MANIPULATE_GET:
		{
			Msg.SetManipulate( GCOpenBoxManipulate::MANIPULATE_GET );

			ItemContainer* pContainer = pHuman->GetHumanOpenBox()->GetContainer();

			if ( pContainer )
			{
				if ( HumanItemLogic::RecieveItemToBag( pHuman, pContainer, HumanOpenBox::RAND_ITEM_INDEX ) )
				{

					Msg.SetRet( GCOpenBoxManipulate::MANIPULATE_NORMAL );
					//清掉Obj_Human上的容器
					pHuman->GetHumanOpenBox()->CleanUp();

				}
				else
				{
					Msg.SetRet( GCOpenBoxManipulate::MANIPULATE_BAG_FULL );

				}

				pHuman->GetPlayer()->SendPacket(&Msg);
				g_pLog->FastSaveLog( LOG_FILE_1, "CGOpenBoxManipulateHandler::Execute() MANIPULATE_GET Ret=%d", 
									Msg.GetRet());

				 return PACKET_EXE_CONTINUE;

			}

			g_pLog->FastSaveLog( LOG_FILE_1, "CGOpenBoxManipulateHandler::Execute() MANIPULATE_GET, GetHumanOpenBox error, PlayerGUID=%X",pHuman->GetGUID());
		    Assert(FALSE);

		}
		break;

	default:
		{
			//玩家发过来的数据有问题
			g_pLog->FastSaveLog( LOG_FILE_1, "CGOpenBoxManipulateHandler::Execute() Player packet Error,PlayerGUID=%X",
								pHuman->GetGUID());
			Assert(FALSE);
		  
		}
		break;
	}
	
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}