///////////////////////////////////////////////////////////////////////////////
// 文件名：CommonSkill001.cpp
// 程序员：frankwu
// 功能说明：捉宠物
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Type.h"
#include "LogDefine.h"
#include "AI_Character.h"
#include "Obj_Character.h"
#include "CommonSkill001.h"
#include "Obj_Human.h"
#include "Obj_Horse.h"
#include "StdImpact000.h"
#include "LogicManager.h"
#include "GCManipulatePetRet.h"
#include "PetManager.h"
#include "Player.h"
#include "Scene.h"
#include "HorseManager.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Impact_Module;
		//class CommonSkill001_T : public SkillLogic_T
		BOOL CommonSkill001_T::SpecificOperationOnSkillStart(Obj_Character& rMe)const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
            Obj_Character* pTar = (Obj_Character*)GetTargetObj(rMe);
			if(NULL==pTar)
			{
				rParams.SetErrCode(OR_INVALID_TARGET);
				return FALSE;
			}
			if (Obj::OBJ_TYPE_HORSE != pTar->GetObjType() )
			{
				rParams.SetErrCode(OR_INVALID_TARGET);
				return FALSE;
			}

			if (((Obj_Horse*)pTar)->GetCreator() )
			{// 如果已经有主人则不可以捕捉
				rParams.SetErrCode(OR_INVALID_TARGET);
				return FALSE;
			}

			//玩家携带坐骑数量是否已达到上限
			if( static_cast<Obj_Human&>(rMe).GetHorseCount() >= HUMAN_PET_MAX_COUNT )
			{
				return FALSE;
			}
			
			// 增加该宠物身上的捕获玩家数
			((Obj_Horse*)pTar)->AddCapturer(rMe.GetID());
			rParams.SetErrCode(OR_OK);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL CommonSkill001_T::OnInterrupt(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			Obj_Character* pTar = (Obj_Character*)(GetTargetObj(rMe));
			if(NULL==pTar)
			{
				rParams.SetErrCode(OR_INVALID_TARGET);
				return FALSE;
			}
			if(NULL==pTar || FALSE==pTar->IsAlive() || FALSE == pTar->IsActiveObj() || Obj::OBJ_TYPE_PET != pTar->GetObjType())
			{
				rParams.SetErrCode(OR_INVALID_TARGET);
				return FALSE;
			}

			GCManipulatePetRet msg;
			msg.SetGUID(((Obj_Human*)&rMe)->GetGUIDOfCallUpPet());
			msg.SetFightingFlag(FALSE);
			msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_CALLUPFALID);

			((Obj_Human*)&rMe)->GetPlayer()->SendPacket(&msg);
			// 减少该宠物身上的捕获玩家数
			((Obj_Horse*)pTar)->DelCapturer(rMe.GetID());

			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		BOOL CommonSkill001_T::OnCancel(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			Obj_Character* pTar = (Obj_Character*)(GetTargetObj(rMe));
			if(NULL==pTar)
			{
				rParams.SetErrCode(OR_INVALID_TARGET);
				return FALSE;
			}
			if(NULL==pTar || FALSE==pTar->IsAlive() || FALSE == pTar->IsActiveObj() || Obj::OBJ_TYPE_PET != pTar->GetObjType())
			{
				rParams.SetErrCode(OR_INVALID_TARGET);
				return FALSE;
			}

			GCManipulatePetRet msg;
			msg.SetGUID(((Obj_Human*)&rMe)->GetGUIDOfCallUpPet());
			msg.SetFightingFlag(FALSE);
			msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_CALLUPFALID);

			((Obj_Human*)&rMe)->GetPlayer()->SendPacket(&msg);
			// 减少该宠物身上的捕获玩家数
			((Obj_Horse*)pTar)->DelCapturer(rMe.GetID());

			return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
		}

		BOOL CommonSkill001_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			if(FALSE==rTar.IsAlive() || FALSE == rTar.IsActiveObj() || Obj::OBJ_TYPE_HORSE != rTar.GetObjType())
			{
				rParams.SetErrCode(OR_INVALID_TARGET);
				return FALSE;
			}
			if(Obj::OBJ_TYPE_HUMAN != rMe.GetObjType())
			{
				rParams.SetErrCode(OR_U_CANNT_DO_THIS_RIGHT_NOW);
				return FALSE;
			}
			{// Test code
				
				INT nDataIndex = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, 0);
				g_ImpactCore.SendImpactToUnit(rTar, nDataIndex, rMe.GetID(), 500);
			}
			PET_LOG_PARAM	PetLogParam;
			BOOL bRet = ((Obj_Human*)&rMe)->CaptureHorse(&PetLogParam,rTar.GetID());
			GCManipulatePetRet msg;
			if(bRet)
			{
				msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_CAPTURESUCC);

				((Obj_Human*)&rMe)->GetPlayer()->SendPacket(&msg);
				// 向正在捕获的其它玩家发送失败消息
				((Obj_Horse&)rTar).SendCaptureFailedToOthers(rMe.GetID());
				// 清空宠物身上所有的玩家数
				((Obj_Horse&)rTar).DelCapturer(rMe.GetID());
				HorseManager* pHorseMgr = rMe.getScene()->GetHorseManager();
				if (pHorseMgr)
				{
					pHorseMgr->RemoveHorse(rTar.GetID());
				}
				
				PetLogParam.OPType	=	PET_OP_CATCH;
				SavePetLog(&PetLogParam);
			}
			else
			{
				// 减少该宠物身上的捕获玩家数
				((Obj_Horse&)rTar).DelCapturer(rMe.GetID());

				msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_CAPTUREFALID);

				((Obj_Human*)&rMe)->GetPlayer()->SendPacket(&msg);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};

