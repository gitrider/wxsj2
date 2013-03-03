#include "stdafx.h"
#include "GCExchangeSynchLockItemII.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "GIException.h"

using namespace EXCHANGE_MSG;
uint GCExchangeSynchLockItemIIHandler::Execute( GCExchangeSynchLockItemII* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		BOOL bIsMyself  = pPacket->GetIsMyself();
		BOOL bLock  = pPacket->GetLock();
		BYTE nIndex = pPacket->GetIndex();
		BYTE nType  = pPacket->GetType();
		PET_GUID_t	PetGuid = pPacket->GetPetGuid(); 
		

		//if ( bIsMyself == TRUE )
		//{
		//	switch( nType )
		//	{
		//	case POS_BAG:
		//		CDataPool::GetMe()->MyExBox_SetItemLock( nIndex, bLock );
		//		break;
		//	case POS_PET:
		//		{
		//			INT IndexInPet = CDataPool::GetMe()->Pet_GetPetByGuid( PetGuid );
		//			CDataPool::GetMe()->MyExBox_SetRidePetLock( IndexInPet, bLock );
		//			break;
		//		}			
		//	case POS_FOLLOWPET:
		//		break;
		//	default:
		//		break;
		//	}
		//}
		//else
		//{
		//	switch( nType )
		//	{
		//	case POS_BAG:
		//		CDataPool::GetMe()->OtExBox_SetItemLock( nIndex, bLock );
		//		break;
		//	case POS_PET:
		//		{
		//			INT IndexInPet = CDataPool::GetMe()->Pet_GetPetByGuid( PetGuid );
		//			CDataPool::GetMe()->OtExBox_SetRidePetLock( IndexInPet, bLock );
		//			break;
		//		}				
		//	case POS_FOLLOWPET:
		//		break;
		//	default:
		//		break;
		//	}
		//}

		//CEventSystem::GetMe()->PushEvent( GE_UPDATE_EXCHANGE );

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
