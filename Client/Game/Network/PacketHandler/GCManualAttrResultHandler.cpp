#include "StdAfx.h"
#include "GCManualAttrResult.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Procedure\GameProcedure.h"

uint GCManualAttrResultHandler::Execute(GCManualAttrResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
		
		ATTR_RESUlT res = pPacket->GetResult();
		switch(res)
		{
		case ATTR_RESUlT_SUCCESS:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_MANUAL_ATTR_SUCCESS_EQUIP );

				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManualAttriResultHandler_Info_Addjust_Attr_Success");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "调整属性成功." );
				break;
			}
		case ATTR_RESULT_NOT_ENOUGH_REMAIN_POINT:
			{

				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManualAttriResultHandler_Info_Addjust_Attr_Fail_No_Enough_point");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "调整属性失败,没有足够的点数." );
				break;
			}
		case ATTR_RESULT_NO_SUCH_ATTR:
			{
				
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManualAttriResultHandler_Info_Addjust_Attr_Fail_No_This_Attri");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "调整属性失败, 没有这个属性." );
				break;
			}
		case ATTR_RESUlT_NO_POINT:
			{

				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManualAttriResultHandler_Info_Addjust_Attr_Fail_No_Enough_point");
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "调整属性失败,没有足够的点数." );
				break;
			}

		default:
			{
				break;
			}
		}

		
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION	

		return PACKET_EXE_ERROR;
}