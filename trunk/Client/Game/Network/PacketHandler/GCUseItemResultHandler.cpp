#include "StdAfx.h"
#include "GCUseItemResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\event\GMEventSystem.h"

uint GCUseItemResultHandler::Execute(GCUseItemResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
 		 USEITEM_RESULT res = pPacket->GetResult();
		switch(res) 
		{
		case USEITEM_SUCCESS:
			//ҩƿʹ�óɹ�  how can i get that???
			break;

		case USEITEM_CANNT_USE:
			{
				STRING strTemp = NOCOLORMSGFUNC("Obj_Item_Info_Item_Can_Not_Use");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			}
			break;

		case USEITEM_LEVEL_FAIL:
			{
				STRING strTemp = NOCOLORMSGFUNC("GMActionSystem_Item_Info_Level_Not_Enough");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			}
			break;

		case USEITEM_TYPE_FAIL:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��Ʒ���ʹ���!");
			}
			break;

		case USEITEM_TARGET_TYPE_FAIL:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "Ŀ�����ʹ���!");
			}
			break;

		case USEITEM_SKILL_FAIL:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��Ʒ���Ӽ��ܴ���!");
			}
			break;

		case USEITEM_IDENT_TYPE_FAIL: //�������ʹ���
			{
//				STRING strTemp = "";
//				strTemp = NOCOLORMSGFUNC("LCRetCharListHandler_Info_Get_Char_Info_Err");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���ܼ���������Ʒ��");
				
				break;
			}
		case USEITEM_IDENT_TARGET_TYPE_FAIL://Ŀ�����ʹ���
			{
//				STRING strTemp = "";
//				strTemp = NOCOLORMSGFUNC("LCRetCharListHandler_Info_Get_Char_Info_Err");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "���ܼ���������Ʒ��");
				break;
			}
		case USEITEM_IDENT_LEVEL_FAIL://����ȼ�����
			{
//				STRING strTemp = "";
//				strTemp = NOCOLORMSGFUNC("LCRetCharListHandler_Info_Get_Char_Info_Err");
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "��������ȼ�������");
				break;
			}
		default:
			break;
		}

	}

	// ȡ������״̬
	CDataPool::GetMe()->Identify_UnLock();

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}