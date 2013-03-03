/*
���������͸��ͻ��˵Ĵ�����Ϣ
*/

#include "stdafx.h"
#include "GCStallError.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"

#include "GIException.h"

uint GCStallErrorHandler::Execute( GCStallError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			switch(pPacket->GetID())
			{
				//����ǿ��ȡ�����ν��׵Ĵ���
			case STALL_MSG::ERR_OWNER_INVALID:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Target_Not_Exist");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"Ŀ�����ﲻ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NEED_NEW_COPY:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Stall_Info_Changed_ReOpen");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"̯λ��Ϣ�Ѹı�,�����´�");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_ROOM:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Stall_Room_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�ռ䲻��");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_MONEY:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Stall_Money_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Stall_Is_Full");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"̯λ����");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_CLOSE:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Stall_Owner_Not_Stall");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"̯������̯");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_ALREADY_LOCK:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Can_Not_Sell");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���ܳ��۴���Ʒ");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_PET_LEVEL_TOO_HIGH:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_pet_level_too_high");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
					break;
				}

				//����ȡ�����ν��׵Ĵ���
			case STALL_MSG::ERR_NOT_ENOUGH_MONEY_TO_OPEN:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Moeny_Is_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ���㣬�޷���̯");
					return PACKET_EXE_CONTINUE ;
					break;
				}
			case STALL_MSG::ERR_ILLEGAL:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCStallErrHandler_Info_Err_Msg");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ƿ���Ϣ");
					return PACKET_EXE_CONTINUE ;
					break;
				}
			default:
				{
				
				}
				break;
			}
			
			//�����������Ʒ������
			//��Ҫ���������Ӧ��Ʒ������(ע�⣬��¡����ƷGUID��ͬ�����Կ�����������)
			for(INT i=0; i<STALL_BOX_SIZE; i++)
			{
				if(CDataPool::GetMe()->MyStallBox_GetItem(i))
				{
					__int64 ItemGUID = CDataPool::GetMe()->MyStallBox_GetItem(i)->GetGUID();

					tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
					if(pBagItem)
					{
						//ȥ������
						pBagItem->SetLock(FALSE);
					}
				}
			}


			//���Ŀ��̯λ
			CDataPool::GetMe()->OtStallBox_Clear();
			CDataPool::GetMe()->MyStallBox_Clear();

			//uiˢ��
			CActionSystem::GetMe()->UserStallSale_Update();
			CActionSystem::GetMe()->UserStallBuy_Update();
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
