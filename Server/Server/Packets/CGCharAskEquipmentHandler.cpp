

#include "stdafx.h"
#include "CGCharAskEquipment.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCCharEquipment.h"
#include "Scene.h"

extern UINT GetEquipmentMaxLevelGemID(Item *pEquipment);

uint CGCharAskEquipmentHandler::Execute( CGCharAskEquipment* pPacket, Player* pPlayer )
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

		//检查线程执行资源是否正确
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;



		ObjID_t ObjID = pPacket->getObjID();
		if( ObjID == INVALID_ID )
		{
			return PACKET_EXE_ERROR ;
		}

		Obj* pObj = pScene->GetObjManager()->GetObj( ObjID ) ;
		if( pObj==NULL )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskEquipmentHandler: not find obj=%d", ObjID ) ;
			return PACKET_EXE_CONTINUE ;
		}

		if( !IsCharacterObj( pObj->GetObjType() ) )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskEquipmentHandler: not character obj=%d", ObjID ) ;
			return PACKET_EXE_CONTINUE ;
		}

		Obj_Character* pCharacter = (Obj_Character*)pObj ;

		GCCharEquipment	Msg;
		Msg.setObjID(pCharacter->GetID());
		if ( pCharacter->GetObjType() == Obj::OBJ_TYPE_HUMAN )
		{
			Obj_Human* pDest = (Obj_Human*)pCharacter ;
			if(pDest->GetEquipID(HEQUIP_MAINHAND)> 0)
				Msg.setMainWeapon(pDest->GetEquipID(HEQUIP_MAINHAND));
			if(pDest->GetEquipID(HEQUIP_ASSIHAND)> 0)
				Msg.setAssiWeapon(pDest->GetEquipID(HEQUIP_ASSIHAND));
			if(pDest->GetEquipID(HEQUIP_HEAD)> 0)
				Msg.setCap(pDest->GetEquipID(HEQUIP_HEAD));
			if(pDest->GetEquipID(HEQUIP_SHOULDER)> 0)
				Msg.setShoulder(pDest->GetEquipID(HEQUIP_SHOULDER));
			if(pDest->GetEquipID(HEQUIP_BACK)>0)
				Msg.setArmour(pDest->GetEquipID(HEQUIP_BACK));
			if(pDest->GetEquipID(HEQUIP_HAND)>0)
				Msg.setCuff(pDest->GetEquipID(HEQUIP_HAND));
			if(pDest->GetEquipID(HEQUIP_FEET)>0)
				Msg.setFoot(pDest->GetEquipID(HEQUIP_FEET));
			if(pDest->GetEquipID(HEQUIP_SUIT)>0)
				Msg.setSuit(pDest->GetEquipID(HEQUIP_SUIT));
			if(pDest->GetEquipID(HEQUIP_RIDER)>0)
				Msg.setRider(pDest->GetEquipID(HEQUIP_RIDER));
		
			pGamePlayer->SendPacket( &Msg ) ;
		}
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskEquipmentHandler: ok obj=%d scene=%d", 
		ObjID, pScene->SceneID() ) ;

		return PACKET_EXE_CONTINUE ;
		

		__LEAVE_FUNCTION
		return PACKET_EXE_ERROR ;
}
