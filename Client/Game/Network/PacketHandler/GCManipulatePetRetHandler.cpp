
#include "stdafx.h"
#include "GCManipulatePetRet.h"
#include "..\..\DataPool\GMDP_Struct_Pet.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Global.h"
#include "../../Event/GMEventSystem.h"
#include "../../DataPool/GMUIDataPool.h"


UINT GCManipulatePetRetHandler::Execute( GCManipulatePetRet* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{	
		switch (pPacket->GetManipulateRet())
		{
		case GCManipulatePetRet::MANIPULATEPET_RET_CAPTUREFALID:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Arrest_Fail");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "捕捉失败");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_CAPTURESUCC:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Arrest__Ok");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "捕捉成功");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_CALLUPFALID:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Call_Fail");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "召唤失败");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_CALLUPSUCC:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Call_Ok");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
				//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "召唤成功");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_RECALLFALID:
			{
				STRING strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_CallUp_Failed");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_RECALLSUCC:
			{
				STRING strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_CallUp_Success");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_FREEFALID:
			{
				STRING strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Free_Failed");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_FREESUCC:
			{
				STRING strTemp = NOCOLORMSGFUNC("GCManipulatePetRetHandler_Info_Free_Success");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;
			// 打开交配界面成功
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OPEN_MATING_UI_OK:
			{
				INT nNPCSeverID = pPacket->GetObjID();
				CEventSystem::GetMe()->PushEvent(GE_TOGLE_MOUNT_MATING_PAGE, nNPCSeverID);
			}
			break;
			// 打开交配界面失败
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OPEN_MATING_UI_FAIL:		
			{				
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "坐骑繁殖界面开启失败");
			}
			break;
			// 交配坐骑锁定成功
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_LOCK_OK:
			{
				// 自己加锁
				CEventSystem::GetMe()->PushEvent(GE_MY_MOUNT_MATING_LOCKED, 1);
			}
			break;
			// 交配坐骑锁定失败
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_LOCK_FAIL:
			{
				// 自己解锁
				CEventSystem::GetMe()->PushEvent(GE_MY_MOUNT_MATING_LOCKED, 0);
			}
			break;
			// 交配坐骑解锁
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_UNLOCK:
			{
				CEventSystem::GetMe()->PushEvent(GE_MY_MOUNT_MATING_LOCKED, 0);
			}
			break;
			// 对方交配坐骑锁定
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OTHER_MATING_LOCK:
			{
				CEventSystem::GetMe()->PushEvent(GE_OTHER_MOUNT_MATING_LOCKED, 1);
			}
			break;			
			// 对方交配坐骑解锁
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_OTHER_MATING_UNLOCK:
			{
				CEventSystem::GetMe()->PushEvent(GE_OTHER_MOUNT_MATING_LOCKED, 0);
			}
			break;
			// 坐骑交配确认成功
		case GCManipulatePetRet::MANIPULATEPET_RET_MATINGSUCCESS:
			{
				CEventSystem::GetMe()->PushEvent(GE_CLOSE_MOUNT_MATING_PAGE);
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "坐骑交配成功");
			}
			break;
			// 坐骑交配确认失败
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_CONFIRM_FAIL:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "坐骑交配确认失败");
			}
			break;
			// 坐骑交配取消
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_CANCEL:
			{
				CEventSystem::GetMe()->PushEvent(GE_CLOSE_MOUNT_MATING_PAGE);
			}
			break;
			// 坐骑领取成功
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_RETRIEVE:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "坐骑已经领取");
			}
			break;

		
		case GCManipulatePetRet::MANIPULATEPET_RET_NO_TEAM:					// 没有组队
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "没有组队");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_TEAM_MEM_COUNT:			// 队伍人数不是2人, 或两人不在同一场景中
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "队伍人数不是2人, 或两人不在同一场景中");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_TEAM_LEADER:				// 不是队长
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "坐骑已经领取");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_AREA:						// 与NPC距离不对
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "与NPC距离不对");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_LOCK:				// 参与交配的马处于锁定状态,不能交配
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马处于锁定状态,不能交配");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SEX_SAME:			// 参与交配的马性别相同
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马性别相同");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_HAPPINESS:			// 参与交配的马快乐度不足100
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马快乐度不足100");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_GENERATION: 		// 参与交配的马不是一代马
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马不是一代马");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_LEVEL:			// 参与交配的马等级差超过5
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马等级差超过5");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SPOUSED:			// 参与交配的马有配偶且不是对方
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马有配偶且不是对方");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SCENE:				// 参与交配的马主人不在同一场景中
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马主人不在同一场景中");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_LEVEL15:			// 参与交配的马等级必须大于15级
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马等级必须大于15级");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_CALLUP:				// 参与交配的马不能是出战状态
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马不能是出战状态");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_VARIATION:			// 参与交配的马不能是变异
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马不能是变异");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATINGLEVEL:		// 参与交配的马等级与上次交配相差不足20级
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马等级与上次交配相差不足20级");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING:				// 参与交配的马的主人有其它马在交配
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "参与交配的马的主人有其它马在交配");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_SPOUSE:				// 并没有骑乘在这里繁殖
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "并没有骑乘在这里繁殖");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_MATING_FINISH:		//骑乘还未繁殖完毕
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "骑乘还未繁殖完毕");
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_HORSE_NO_SOLT:			// 没有空格子了
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "没有空格子了");
			break;
		default :
			break;
		}
		//CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_PAGE);

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
