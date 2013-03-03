///////////////////////////////////////////////////////////////////////////////
// 文件名：Impact_Core.cpp
// 程序员：高骐
// 功能说明：效果内核模块
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "stdlib.h"
#include "Type.h"
#include "Impact_Core.h"
#include "Scene.h"
#include "LogicManager.h"
#include "Log.h"
#include "GameTable.h"
#include "DataRecords.h"


using namespace Combat_Module;
using namespace Combat_Module::Impact_Module;

Combat_Module::Impact_Module::ImpactCore_T g_ImpactCore;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL ImpactCore_T::InitImpactFromData(ID_t nDataIndex, OWN_IMPACT & rImp, Obj_Character& rTar, INT nReserveParam1, INT nReserveParam2) const
		{
			__ENTER_FUNCTION
			//清空OWN_IMPACT
			rImp.CleanUp();
			//设置impact效果表的索引
			rImp.SetDataIndex(nDataIndex);
			//根据索引获得保存效果项的对象
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				const_cast<ImpactData_T*>(pData)->SetReserveParam1(nReserveParam1);//获取保留参数(魂珠:背包索引)
				rImp.SetImpactID(pData->GetImpactID());
				//设置效果持续时间
				rImp.SetContinuance(pData->GetContinuance());
				//获得效果逻辑对象
				ImpactLogic_T const* pLogic = Impact_GetLogic(rImp);
				if(NULL!=pLogic)
				{	
					Scene* pScene = rTar.getScene();
					if( pScene == NULL )
					{
						return FALSE;
					}
					INT iSkillLevel = 0;
					Obj_Human* pHuman = pScene->GetHumanManager()->GetHuman(rImp.GetCasterObjID());
					if( pHuman != NULL )
					{
						const _OWN_SKILL* pOwnSkill = pHuman->Skill_GetSkill( rImp.GetSkillID() );
						if( pOwnSkill != NULL )
						{
							iSkillLevel = pOwnSkill->m_nLevel;
						}
					}
					iSkillLevel -= 1;
					if( 0 > iSkillLevel )
					{
						iSkillLevel = 0;
					}
					rImp.SetSkillLevel( iSkillLevel );
					//使用具体效果逻辑初始化rImp相关的属性
					if(TRUE == pLogic->InitFromData(rImp, *pData))
					{
						return TRUE;
					}
				}
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		};

		UINT ImpactCore_T::GetUniqueID()
		{
			__ENTER_FUNCTION
			++m_uUniqueID;
			if( 0 == m_uUniqueID )
				++m_uUniqueID;
			return m_uUniqueID;
			__LEAVE_FUNCTION
		}

		BOOL ImpactCore_T::SendImpactToUnit(Obj_Character& rTar, ID_t nDataIndex, ObjID_t nSender, SkillID_t nSkillID, Time_t nDelayTime, BOOL bCriticalFlag, INT nRefixRate) const
		{
			__ENTER_FUNCTION
				OWN_IMPACT impact;
			if(INVALID_ID == nDataIndex)
			{
				return FALSE;
			}
			//根据nDataIndex对应的效果逻辑初始化rImp
			if(TRUE == InitImpactFromData(nDataIndex, impact, rTar))
			{
				//获得逻辑对象
				ImpactLogic_T const* pLogic = Impact_GetLogic(impact);
				if(NULL==pLogic)
				{
					Assert(NULL=="[ImpactCore_T::SendImpactToUnit]: Can't find sprcific logic for this impact.");
					return FALSE;
				}
				//设置会心标记
				if(TRUE == bCriticalFlag)
				{
					impact.MarkCriticalFlag();
				}
				if(0!=nRefixRate)
				{
					//向效果逻辑设置修正率
					pLogic->RefixPowerByRate(impact, nRefixRate);
				}
				if(0>nDelayTime)
				{
					nDelayTime = 0;
				}
				Scene* pScene = rTar.getScene();
				Obj_Character* pChar = NULL;
				if(NULL!=pScene)
				{
					Obj* pObj = pScene->GetSpecificObjByID( nSender );

					if( IsCharacterObj(pObj->GetObjType()) )
					{
						impact.SetSkillID( nSkillID );
					}
					pScene->GetEventCore().RegisterImpactEvent(rTar.GetID(), nSender, impact, nDelayTime);
					return TRUE;
				}
			}
			// start to fill impact struct
			return TRUE;
			__LEAVE_FUNCTION
				return FALSE;
		}

		BOOL ImpactCore_T::SendImpactToUnit(Obj_Character& rTar, ID_t nDataIndex, ObjID_t nSender, Time_t nDelayTime, BOOL bCriticalFlag, INT nRefixRate, INT nReserveParam1, INT nReserveParam2) const
		{
			__ENTER_FUNCTION
			OWN_IMPACT impact;
			if(INVALID_ID == nDataIndex)
			{
				return FALSE;
			}
			//根据nDataIndex对应的效果逻辑初始化rImp
			if(TRUE == InitImpactFromData(nDataIndex, impact, rTar, nReserveParam1))
			{
				//获得逻辑对象
				ImpactLogic_T const* pLogic = Impact_GetLogic(impact);
				if(NULL==pLogic)
				{
					Assert(NULL=="[ImpactCore_T::SendImpactToUnit]: Can't find sprcific logic for this impact.");
					return FALSE;
				}
				//设置会心标记
				if(TRUE == bCriticalFlag)
				{
					impact.MarkCriticalFlag();
				}
				if(0!=nRefixRate)
				{
					//向效果逻辑设置修正率
					pLogic->RefixPowerByRate(impact, nRefixRate);
				}
				if(0>nDelayTime)
				{
					nDelayTime = 0;
				}
				Scene* pScene = rTar.getScene();
				Obj_Character* pChar = NULL;
				if(NULL!=pScene)
				{
					Obj* pObj = pScene->GetSpecificObjByID( nSender );
					
					if( IsCharacterObj(pObj->GetObjType()) )
					{
						pChar = static_cast<Obj_Character*>(pObj);
						SkillInfo_T& rSkillInfo = pChar->GetSkillInfo();
						impact.SetSkillID( rSkillInfo.GetSkillID() );
						rSkillInfo.SetSendSkillImpactID(impact.GetDataIndex());
					}
					pScene->GetEventCore().RegisterImpactEvent(rTar.GetID(), nSender, impact, nDelayTime);
					return TRUE;
				}
			}
			// start to fill impact struct
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		ImpactData_T const* Impact_GetDataBlock(ID_t nDataIndex)
		{
			__ENTER_FUNCTION
			ImpactData_T const* pData = g_StandardImpactMgr.GetInstanceByID(nDataIndex);
			if(NULL==pData)
			{
				CHAR szBuffer[256];
				sprintf(szBuffer, "[Impact_GetDataBlock]: Can't find specific Data for impact(%d)", nDataIndex);
				g_pLog->FastSaveLog( LOG_FILE_1, szBuffer ) ;
				//AssertEx(FALSE,szBuffer);
				return NULL;
			}
			return pData;
			__LEAVE_FUNCTION
			return NULL;
		}
		Obj* Impact_GetCaster(Obj& rMe, OWN_IMPACT& rImp)
		{
			__ENTER_FUNCTION
			Obj* pObj=NULL;
			//获得效果投送者的ID
			ObjID_t nCasterID = rImp.GetCasterObjID();
			//如果ID有效
			if(INVALID_ID != nCasterID)
			{		
				//在场景中获得效果投送者的对象实例
				pObj = rMe.GetSpecificObjInSameSceneByID(nCasterID);
			}
			//如果没找到对象
			if(NULL==pObj)
			{
				//如果此效果是玩家产生的
				if(TRUE==rImp.IsCreateByPlayer())
				{
					//用全局ID查找此对象
 					pObj = rMe.GetSpecificHumanInSameSceneByGUID(rImp.GetCasterUniqueID());
					if(NULL!=pObj)
					{
						nCasterID = pObj->GetID();
					}
					else
					{
						nCasterID = INVALID_ID;
					}
				}
				else
				{
					nCasterID = INVALID_ID;
				}
			}
			//设置效果投送者ID
			rImp.SetCasterObjID(nCasterID);
			return pObj;
			__LEAVE_FUNCTION
			return NULL;
		}

		ID_t Impact_GetLogicID(OWN_IMPACT const& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetLogicID();
			}
			return INVALID_ID;
		__LEAVE_FUNCTION
			return INVALID_ID;
		}
		BOOL Impact_IsOverTimed(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL==pData)
			{
				return FALSE;
			}
			BOOL bIsOverTimed = pData->IsOverTimed();
			ImpactLogic_T const* pLogic = Impact_GetLogic(rImp);
			if(NULL!=pLogic)
			{
				if(pLogic->IsOverTimed()!=bIsOverTimed)
				{
					Assert(NULL=="[Impact_IsOverTimed] Logic is not match the data.");
					return pLogic->IsOverTimed();
				}
			}
			return bIsOverTimed;
		__LEAVE_FUNCTION
			return FALSE;
		}
		ID_t Impact_GetImpactID(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetImpactID();
			}
			return INVALID_ID;
		__LEAVE_FUNCTION
			return INVALID_ID;
		}
		ID_t Impact_GetMutexID(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetMutexID();
			}
			return INVALID_ID;
		__LEAVE_FUNCTION
			return INVALID_ID;
		}
		ID_t Impact_GetLevel(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetLevel();
			}
			return 0;
		__LEAVE_FUNCTION
			return 0;
		}
		ID_t Impact_GetStandFlag(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetStandFlag();
			}
			return BEHAVIOR_TYPE_NEUTRALITY;
		__LEAVE_FUNCTION
			return BEHAVIOR_TYPE_NEUTRALITY;
		}
		BOOL Impact_IsRemainOnCorpse(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IsRemainOnCorpse();
			}
			return FALSE;
		__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL Impact_CanBeDispeled(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->CanBeDispeled();
			}
			return TRUE;
		__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL Impact_CanBeCanceled(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->CanBeCanceled();
			}
			return TRUE;
		__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL Impact_NeedChannelSupport(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->NeedChannelSupport();
			}
			return FALSE;
		__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL Impact_IsFadeOutWhenUnitOnDamage(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IsFadeOutWhenUnitOnDamage();
			}
			return TRUE;
		__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL Impact_IsFadeOutWhenUnitStartActions(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IsFadeOutWhenUnitStartActions();
			}
			return TRUE;
		__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL Impact_IsFadeOutWhenUnitOffline(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IsFadeOutWhenUnitOffline();
			}
			return TRUE;
		__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL Impact_IsStillTimingWhenUnitOffline(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IsStillTimingWhenUnitOffline();
			}
			return FALSE;
		__LEAVE_FUNCTION
			return FALSE;
		}
		ID_t Impact_IgnoreFliter(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->IgnoreFliter();
			}
			return FALSE;
		__LEAVE_FUNCTION
			return FALSE;
		}

		Time_t Impact_GetTableContinuance(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetContinuance();
			}
			return 0;
		__LEAVE_FUNCTION
			return 0;
		}
		Time_t Impact_GetInterval(OWN_IMPACT& rImp)
		{
		__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetInterval();
			}
			return 0;
		__LEAVE_FUNCTION
			return 0;
		}
		
		INT	Impact_GetImpactDataDescriptorValueByIndex(INT nDataIndex, INT nIndex, INT iLevel)
		{
			__ENTER_FUNCTION
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				Descriptor_T const* pDescriptor = pData->GetDescriptorByIndex(nIndex);
				if(NULL!=pDescriptor)
				{
					return pDescriptor->GetValueBySkillLevel(iLevel);
				}
				else
				{
					Assert(NULL=="[Impact_GetImpactDataDescriptorValueByIndex]: Illegal index found!");
				}
			}
			return 0;
			__LEAVE_FUNCTION
			return 0;
		}
		BOOL Impact_IsImpactInCollection(OWN_IMPACT& rImp, INT nCollectionID)
		{
			__ENTER_FUNCTION
			INT nID=INVALID_ID;
			IDCollection_T const* pCollection = NULL;
			pCollection = g_IDCollectionMgr.GetInstanceByID(nCollectionID);
			if(NULL!=pCollection)
			{
				switch (pCollection->GetType())
				{
					case IDCollection_T::TYPE_BUFF_ID:
						if(TRUE==Impact_IsOverTimed(rImp))
						{
							nID = Impact_GetImpactID(rImp);
						}
						break;
					case IDCollection_T::TYPE_IMPACT_LOGIC_ID:
						nID = Impact_GetLogicID(rImp);
						break;
					case IDCollection_T::TYPE_IMPACT_MUTEX_ID:
						nID = Impact_GetMutexID(rImp);
						break;
					case IDCollection_T::TYPE_SKILL_ID:
						nID = rImp.GetSkillID();
						break;
					case IDCollection_T::TYPE_DIRECT_IMPACT_ID:
						if(TRUE==Impact_IsOverTimed(rImp))
						{
							nID = Impact_GetImpactID(rImp);
						}
						break;
					case IDCollection_T::TYPE_SKILL_LOGIC_ID:
					default:
						break;
				}
				if(INVALID_ID != nID)
				{
					if(0<pCollection->GetCollectionSize())
					{
						return pCollection->IsThisIDInCollection(nID);
					}
				}
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		//获得同一个角色造成多个相同效果是否互斥
		BOOL Impact_GetMutexByCasterFlag(OWN_IMPACT& rImp)
		{
			__ENTER_FUNCTION
			ID_t nDataIndex = rImp.GetDataIndex();
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				return pData->GetMutexByCasterFlag();
			}
			return 0;
			__LEAVE_FUNCTION
			return 0;
		}
		//两个效果是否有互斥关系
		//比较两个角色身上效果对象是否为同一互斥组，并且两个效果是否为同一个玩家使用技能造成
		BOOL Impact_IsImpactsABMutexed(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB)
		{
			__ENTER_FUNCTION
			if(Impact_GetMutexID(rImpactA)!=Impact_GetMutexID(rImpactB))
			{
				return FALSE;
			}
			//如果两个效果都有同角色互斥属性
			if(TRUE==Impact_GetMutexByCasterFlag(rImpactA) && TRUE==Impact_GetMutexByCasterFlag(rImpactB))
			{
				//判断两个效果是否为同一个角色产生
				if((rImpactA.GetCasterUniqueID()!=rImpactB.GetCasterUniqueID()) || (rImpactA.IsCreateByPlayer()!=rImpactB.IsCreateByPlayer()))
				{
					return FALSE;
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//判断效果A是否可以替换效果B
		BOOL Impact_CanImpactAReplaceImpactB(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB)
		{
			__ENTER_FUNCTION
			//如果两个效果没有互斥关系则不能替换
			if(FALSE == Impact_IsImpactsABMutexed(rImpactA, rImpactB))
			{
				return FALSE;
			}
			//如果两个效果是同一种技能产生的，并且两个产生效果的技能ID都有效
			if(rImpactA.GetSkillID()==rImpactB.GetSkillID() && INVALID_ID!=rImpactA.GetSkillID() && INVALID_ID!=rImpactB.GetSkillID())
			{
				//待替换效果的级别小于被替换效果的级别则不能替换
				if(Impact_GetLevel(rImpactA)<Impact_GetLevel(rImpactB))
				{
					return FALSE;
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

	};
};
