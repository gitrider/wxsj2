#include "stdafx.h"
#include "CGAskItemInfo.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "GCItemInfo.h"
#include "HumanItemLogic.h"



uint	CGAskItemInfoHandler::Execute(CGAskItemInfo* pPacket, Player* pPlayer )
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

	WORD	BagIndex	=	pPacket->getID();

	if(BagIndex<EXTRA_CONTAINER_OFFSET)
	{
		Item*			pBagItem = HumanItemLogic::GetBagItem(pHuman,BagIndex);
		if(pBagItem ==NULL)
		{
			//Assert(FALSE);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGAskItemInfoHandler: BagIndex=%d occur bad memory ",	BagIndex ) ;
			return PACKET_EXE_ERROR ;
		}
		
		GCItemInfo	Msg;

		if(!pBagItem->IsEmpty())
		{

				Msg.setID(BagIndex);
				pBagItem->SaveValueTo(Msg.getItem());
				Msg.setIsNull(FALSE);
				pGamePlayer->SendPacket(&Msg);
		}
		else
		{
				Msg.setID(BagIndex);
				Msg.setIsNull(TRUE);
				pGamePlayer->SendPacket(&Msg);

		}

	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskItemInfoHandler: BagIndex=%d",
		BagIndex ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}