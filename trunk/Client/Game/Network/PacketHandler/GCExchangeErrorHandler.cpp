
#include "stdafx.h"
#include "GCExchangeError.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "GIException.h"


uint GCExchangeErrorHandler::Execute( GCExchangeError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			switch(pPacket->GetID())
			{
			//����ǿ��ȡ�����ν��׵Ĵ���
			case EXCHANGE_MSG::ERR_ALREADY_LOCKED:
				{
					// "�����������ף������²���"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_You_Are_Locked");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_EXROOM:
				{

					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Over_Exchange_Count");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ɽ�����Ʒ���������ޣ������²���");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			//����ȡ�����ν��׵Ĵ���
			case EXCHANGE_MSG::ERR_SELF_IN_EXCHANGE:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Doing_Exchange");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Լ����ڽ�����");
					break;
				}
			case EXCHANGE_MSG::ERR_TARGET_IN_EXCHANGE:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Other_Doing_Exchange");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Է����ڽ�����");
					break;
				}
			case EXCHANGE_MSG::ERR_DROP:
				{
					//STRING strTemp = "";
					//strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Other_Cancel_Exchange");
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Է�ȡ������");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_OTHER:
				{
					// "�Է��ռ䲻��"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Pag_Size_Not_Enough_Other");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_SELF:
				{
					// "��ı����ռ䲻��"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Pag_Size_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_OTHER:
				{
					// "�Է���Ǯ���㣬�����²���"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Money_Is_Not_Enough_Other");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"��Ǯ���㣬�����²���");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_SELF:
				{
					// "��Ľ�Ǯ���㣬�����²���"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Money_Is_Not_Enough");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			case EXCHANGE_MSG::ERR_TOO_FAR:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Over_Exchange_Range");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ܹ����׵ķ�Χ");
					break;
				}

			case EXCHANGE_MSG::ERR_REFUSE_TRADE:
				{
					// "�Է��ܾ�����"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Refuse_To_Trade");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}

			case EXCHANGE_MSG::ERR_PET_LEVEL_TOO_HIGH:
				{
					// "��˼������,�޷�����"
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Pet_Level_Too_High");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			
			case EXCHANGE_MSG::ERR_SELF_DEAD_OR_OFFLINE:  // "�Լ��������������뽻��" 
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Exchange_Info_Dead_Can_Not_Exchange");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				break;
			}
			case EXCHANGE_MSG::ERR_TARGET_DEAD_OR_OFFLINE: // "�Է������߻����������ܼ�������" 20100524LL
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_ExchangeObj_OffLine");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			case EXCHANGE_MSG::ERR_IN_PROTECT_TIME: // "���߱���ʱ���ڲ��ܽ���"  20100524LL
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Exchange_In_ProtectTime");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					break;
				}
			case EXCHANGE_MSG::ERR_SELF_OPEN_STALL: // "�����ڰ�̯��"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GMAI_Base_Info_You_Stalling" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_TARGET_OPEN_STALL:  // "�Է����ڰ�̯�����ܽ���"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GCExchangeErrHandler_Exchange_In_OtherStall" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_SELF_USE_SKILL:  // "������ʹ�ü��ܣ����ܽ���"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GCExchangeErrHandler_Exchange_In_UseSkill" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_TARGET_USE_SKILL:  // "�Է�����ʹ�ü��ܣ����ܽ���"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GCExchangeErrHandler_Exchange_In_Other_UseSkill" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_SELF_MOVE:  // "�������н��У����ܽ���"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GMGameInterface_Script_Exchange_Info_Run_Can_Not_Exchange" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_TARGET_MOVE:  // "�Է������н��У����ܽ���"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GCExchangeErrHandler_Exchange_In_Other_Run_CanNotExchange" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}
			case EXCHANGE_MSG::ERR_ITEM_LOCKED:  // "����Ʒ�Ѿ���"
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC( "GMActionSystem_Item_Info_Item_Is_Binded" );
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str() );
					break;
				}				
			case EXCHANGE_MSG::ERR_ILLEGAL:
			default:
				{
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GCExchangeErrHandler_Info_Err_Status");
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�Ƿ�״̬������ȡ��");
					break;
				}
			}

			//��Ҫ���������Ӧ��Ʒ������(ע�⣬��¡����ƷGUID��ͬ�����Կ�����������)
			for(INT i=0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if(CDataPool::GetMe()->MyExBox_GetItem(i))
				{
					__int64 ItemGUID = CDataPool::GetMe()->MyExBox_GetItem(i)->GetGUID();

					tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
					if(pBagItem)
					{
						//ȥ������
						pBagItem->SetLock(FALSE);
					}
				}
			}

			//һ�����
			CDataPool::GetMe()->MyExBox_Clear();
			CDataPool::GetMe()->OtExBox_Clear();

			//����Action
			CActionSystem::GetMe()->UserExchangeOther_Update();
			CActionSystem::GetMe()->UserExchangeSelf_Update();


			//���±�����Ʒ��������ܻ���Ҫ����װ������Ʒ�����Ҫ��װ�����ϵĶ����ǲ��ǿ����������ף�
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

			//�رս��׽���
			CEventSystem::GetMe()->PushEvent(GE_SUCCEED_EXCHANGE_CLOSE);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
