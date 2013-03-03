
#include "stdafx.h"
#include "Log.h"
#include "Scene.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Obj_Monster.h"
#include "PetPlacard.h"
#include "PacketFactoryManager.h"
#include "GCOperateResult.h"

#include "CGIssuePetPlacard.h"

uint CGIssuePetPlacardHandler::Execute( CGIssuePetPlacard* pPacket, Player* pPlayer )
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

	Obj_Monster *pNpc = (Obj_Monster*)(pScene->GetObjManager()->GetObj(pPacket->GetNpcID()));
	if(pNpc != NULL)
	{
		PetPlacardSystem *pPetPlacardSystem = pNpc->GetPetPlacardSystem();
		if(pPetPlacardSystem == NULL)
		{
			pNpc->CreatePetPlacardSystem();
			pPetPlacardSystem = pNpc->GetPetPlacardSystem();
		}
		if(pPetPlacardSystem != NULL)
		{
			ORESULT oResult = pPetPlacardSystem->IssuePlacard(pHuman, pPacket->GetGUID(), pPacket->GetMessage());
			if(OR_FAILED(oResult))
			{
				pHuman->SendOperateResultMsg(oResult);
			}
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
