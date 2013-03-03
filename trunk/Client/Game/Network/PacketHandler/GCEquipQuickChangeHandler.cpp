
#include "StdAfx.h"
#include "GCEquipSuitExchangeResult.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Item\Obj_Item_Equip.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\GMDataPool.h"
#include "Action\GMActionSystem.h"
#include "Event\GMEventSystem.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Sound\GMSoundSystem.h"


using namespace Packets;


uint GCEquipSuitExchangeResultHandler::Execute(GCEquipSuitExchangeResult* pPacket, Player* pPlayer )
{

__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		// ˢ�����ݳ�
		CCharacterData * pCharacterData = ((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData();

		if( pCharacterData )
		{
			EQUIPSUIT_RESULT ret = (EQUIPSUIT_RESULT)pPacket->getResult();

			switch( ret )
			{
			case EQUIPSUIT_SUCCESS:
				{
					for( BYTE i = 0; i < MAX_EQUIPSUIT_POINT_NUMBER; i++ )
					{
						EXCHANGE_EQUIP_INFO pInfo = pPacket->getEquipInfo(i);

						if( pInfo.m_BagIndex == (BYTE)INVALID_ITEM_POS &&
							pInfo.m_EquipPoint == (BYTE)INVALID_ITEM_POS )
						{
							STRING strTemp = "�Ҳ���װ������װʧ��";
							//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
							continue;
						}
						else if( pInfo.m_BagIndex == (BYTE)INVALID_ITEM_POS &&
							pInfo.m_EquipPoint != (BYTE)INVALID_ITEM_POS )
						{
							STRING strTemp = "װ���㲻��Ҫ�滻";
							continue;
						}
						else if( pInfo.m_BagIndex != (BYTE)INVALID_ITEM_POS &&
							pInfo.m_EquipPoint == (BYTE)INVALID_ITEM_POS )
						{
							continue;
						}
						//---------------------------------------------------------
						// UI���ݳ�
						CDataPool* pDataPool = CDataPool::GetMe();

						// �������װ��
						tObject_Item* pItemAtBag = pDataPool->UserBag_GetItem(pInfo.m_BagIndex);
						if(!pItemAtBag) 
							return PACKET_EXE_CONTINUE;

						// ���ϵ�װ��
						tObject_Item* pItemAtUser= pDataPool->UserEquip_GetItem( (HUMAN_EQUIP)pInfo.m_EquipPoint);


						// װ����Ϣ����
						if(pItemAtBag->GetItemClass() != ICLASS_EQUIP || 
							((CObject_Item_Equip*)pItemAtBag)->GetItemType() != pInfo.m_EquipPoint) 
							return PACKET_EXE_CONTINUE;


						// ���������Ʒת�ƣ���ı�ͻ���id���Ա�ʾ�ڿͻ����ǲ�ͬ����
						((CObject_Item*)pItemAtBag)->ChangeClientID();
						if(pItemAtUser)	((CObject_Item*)pItemAtUser)->ChangeClientID();


						//---------------------------------------
						// ˢ�½�ɫ����
						CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Equip(
							((CObject_Item_Equip*)pItemAtBag)->GetItemType(), pItemAtBag->GetIdTable());		

						USHORT uWorld, uServer;
						UINT uSerial;
						pItemAtBag->GetGUID(uWorld, uServer, uSerial);

						//---------------------------------------
						// ˢ�°�������
						if( pItemAtUser )
							pItemAtUser->SetGUID(uWorld, uServer, uSerial);

						pDataPool->UserBag_SetItem(pInfo.m_BagIndex, (tObject_Item*)pItemAtUser, FALSE);

						CActionSystem::GetMe()->UserBag_Update();
						// ˢ�����ݳ�
						pDataPool->UserEquip_SetItem(((CObject_Item_Equip*)pItemAtBag)->GetItemType(), (tObject_Item*)pItemAtBag, FALSE);


						// �����¼�
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, pInfo.m_BagIndex);
						CEventSystem::GetMe()->PushEvent(GE_QUICK_CHANGE_SUCCESS, pInfo.m_BagIndex);
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��װ�ɹ�" );
					}

					CSoundSystemFMod::_PlayUISoundFunc(67);
				}
				break;

			case EQUIPSUIT_JOB_FAIL:
				{
					STRING strTemp = "ְҵ����ȷ����װʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
				}
				break;

			case EQUIPSUIT_LEVEL:
				{
					STRING strTemp = "�ȼ���������װʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
				}
				break;

			case EQUIPSUIT_EQUIP_FAIL:
				{
					STRING strTemp = "��װʧ��";
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
				}
				break;
			}
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}