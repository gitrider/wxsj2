///////////////////////////////////////////////////////////////////////////////
// �ļ�����SkillLogic.cpp
// ����Ա������
// ����˵�������ܵĻ����࣬���������м��ܿ�����Ҫ�ṩ�Ľӿں�һЩ���õĹ̶��߼�
//
// �޸ļ�¼��
//	ף�������������������������⣬ʹ�����µ�scan��
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillLogic.h"
#include "Obj_Human.h"
#include "Obj_Character.h"
#include "Obj_Monster.h"
#include "Obj_Pet.h"
#include "Scene.h"
#include "LogicManager.h"
//#include "ScanOperator_AESkill.h"
//#include "SOT_XiaoYaoTraps.h"
#include "ScanOperator_SectorSkill.h"
#include "SpecialObjLogic.h"
#include "ActionDelegator.h"
#include "GCTargetListAndHitFlags.h"
#include "GameTable.h"
#include "ConditionAndDeplete_Core.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Packets;
		using namespace Impact_Module;
		using namespace Combat_Module::Special_Obj_Module;
		using namespace Action_Module;
		
		// class SkillLogic_T
		// interface overwrite by sub class
		BOOL SkillLogic_T::SpecificOperationOnSkillStart(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			return TRUE; //ȱʡ����û�����������������Ͳ���
			__LEAVE_FUNCTION
			return FALSE;
		};
		BOOL SkillLogic_T::IsPassive(VOID) const
		{
			__ENTER_FUNCTION
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		};
		BOOL SkillLogic_T::OnInterrupt(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		};
		BOOL SkillLogic_T::OnCancel(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			rParams.SetErrCode(OR_INVALID_SKILL);
			rParams.SetErrParam(rParams.GetActivatedSkill());
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		};
		
		BOOL SkillLogic_T::DepleteProcess(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE == CommonDeplete(rMe))
			{
				return FALSE;
			}
			if(FALSE == SpecificDeplete(rMe))
			{
				return FALSE;
			}
			return TRUE;//������������Ҫ����
			__LEAVE_FUNCTION		
			return FALSE;
		};
		BOOL SkillLogic_T::IsConditionSatisfied(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE == CommonConditionCheck(rMe))
			{
				return FALSE;
			}
			if(FALSE == SpecificConditionCheck(rMe))
			{
				return FALSE;
			}
			return TRUE;//������������Ҫ����
			__LEAVE_FUNCTION		
			return FALSE;
		};
		// interface common
		BOOL SkillLogic_T::StartCharging(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			long nMaxTime = 0;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			nMaxTime = rSkillInfo.GetChargeTime();
			BOOL bRet = TRUE;
			if(0>=nMaxTime)
			{
				bRet = Action_ActivateOnceHandler(rMe);
			}
			else
			{
				bRet = GetGlobalActionDelegator().RegisterChargeActionForSkill(rMe, rSkillInfo.GetSkillID(), nMaxTime);
			}
			if(TRUE== bRet)
			{
				rParams.SetErrCode(OR_OK);
			}
			return bRet;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::StartChanneling(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			long nMaxTime = rSkillInfo.GetChannelTime();
			if(0 >= nMaxTime)
			{
				AssertEx(FALSE,"[SkillLogic_T::StartChanneling]: Zero channel time found!!");
				rParams.SetErrCode(OR_ERROR);
				return FALSE;
			}
			//��������
			BOOL bRet = TRUE;
			if(FALSE==rParams.GetIgnoreConditionCheckFlag())
			{
				bRet=DepleteProcess(rMe);
			}
			if(TRUE == bRet)
			{
				//����ʹ�óɹ�
				rMe.OnUseSkillSuccessfully(rSkillInfo);
				if(TRUE==GetGlobalActionDelegator().RegisterChannelActionForSkill(rMe, rSkillInfo.GetSkillID(), nMaxTime, rSkillInfo.GetChargesOrInterval()))
				{
					//��ȴʱ��
					CooldownProcess(rMe);
					rParams.SetErrCode(OR_OK);
					ActivateOnce(rMe);
					return TRUE;
				}
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::StartLaunching(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			return Action_ActivateOnceHandler(rMe);
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::Action_ActivateOnceHandler(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			//����У�鼼��ʹ������
			if(FALSE == IsConditionSatisfied(rMe))
			{
				return FALSE;
			}
			//��������
			BOOL bRet = TRUE;
			if(FALSE==rParams.GetIgnoreConditionCheckFlag())
			{
				bRet=DepleteProcess(rMe);
			}
			if(TRUE == bRet)
			{
				//����ʹ�óɹ�,�ص�Impact����OnUseSkillSuccessfully
				rMe.OnUseSkillSuccessfully(rSkillInfo);
				//������ʾʱ��
				Time_t nPlayActionTime = CalculateActionTime(rMe);
				//��ȫ�ֶ���ϵͳע�Ἴ���ͷųɹ�������ϵͳ�ᷢ��GCCharSkill_Send��Ϣ��client֪ͨ���ܳɹ��ͷ���Ϣ
				if(FALSE==GetGlobalActionDelegator().RegisterInstantActionForSkill(rMe, rSkillInfo.GetSkillID(), nPlayActionTime))
				{
					return FALSE;
				}
				rMe.SetActionTime(nPlayActionTime);
				//��ȴ����
				CooldownProcess(rMe);
				INT nActivateTimes = rSkillInfo.GetChargesOrInterval();
				if(0>=nActivateTimes)
				{
					nActivateTimes = 1;
				}

				for(INT nIdx=0; nActivateTimes>nIdx; ++nIdx)
				{
					rParams.SetDelayTime(rParams.GetDelayTime() + rSkillInfo.GetDelayTime());
					ActivateOnce(rMe);
				}
			}
			return bRet;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::Action_ActivateEachTickHandler(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(TRUE==TargetCheckForEachTick(rMe))
			{
				rParams.SetDelayTime(rSkillInfo.GetDelayTime());
				ActivateEachTick(rMe);
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//internal used
		Time_t SkillLogic_T::CalculateActionTime(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Time_t nPlayActionTime = rSkillInfo.GetPlayActionTime();
			if(MELEE_ATTACK==rParams.GetActivatedSkill())
			{
				if(Obj::OBJ_TYPE_MONSTER == rMe.GetObjType())
				{
					Obj_Monster& rMonster = static_cast<Obj_Monster&>(rMe);
					nPlayActionTime = rMonster.GetAttackAnimTime();
				}
				else if(Obj::OBJ_TYPE_PET == rMe.GetObjType())
				{
					Obj_Pet& rPet = static_cast<Obj_Pet&>(rMe);
					//�����������ı���д
				}
			}
			return nPlayActionTime;
			__LEAVE_FUNCTION
			return 1000; //1�룬��ȫֵ
		}

		VOID SkillLogic_T::CooldownProcess(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Time_t nPlayActionTime = rSkillInfo.GetPlayActionTime();
			Time_t nCooldown = rSkillInfo.GetCooldownTime();
			Time_t nDelayTime = rSkillInfo.GetDelayTime();
			if(MELEE_ATTACK==rParams.GetActivatedSkill())
			{
				if(Obj::OBJ_TYPE_MONSTER == rMe.GetObjType())
				{
					Obj_Monster& rMonster = static_cast<Obj_Monster&>(rMe);
					nCooldown = rMonster.GetAttackCooldownTime();
					nPlayActionTime = rMonster.GetAttackAnimTime();
				}
				else if(Obj::OBJ_TYPE_PET == rMe.GetObjType())
				{
					Obj_Pet& rPet = static_cast<Obj_Pet&>(rMe);
					//�����������ı���д
				}
			}
			if(nCooldown<nPlayActionTime) //��ȴʱ�䲻��С�ڶ���ʱ��
			{
				nCooldown = nPlayActionTime;
			}
			//���ڲ��������ˣ������뱣����δ����ѷ�ֹ�Ժ���ġ�
			/*
			if(TRUE==rSkillInfo.UseNormalAttackRate())
			{
				nCoolDown = RefixCooldownTimeWithAttackRate(nCoolDown,rMe.GetAttackSpeed()); 
			}
			*/
			//������ȴ
			if(TRUE==rSkillInfo.IsAutoShotSkill())
			{
				SetAutoRepeatCooldown(rMe, nCooldown);
			}
			else
			{
				SetCooldown(rMe,(CooldownID_t)rSkillInfo.GetCooldownID(),nCooldown);
			}
			__LEAVE_FUNCTION
		}
		BOOL SkillLogic_T::CommonConditionCheck(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			//ǰ��������������
			INT nIdx=0;
			for( nIdx=0; SkillInstanceData_T::CONDITION_AND_DEPLETE_TERM_NUMBER-1> nIdx; ++nIdx)
			{
				ConDepTerm_T const* pConDepTerm = Skill_GetConDepTermByIndex(rSkillInfo, nIdx);
				if(NULL!=pConDepTerm)
				{
					if(FALSE==g_ConditionAndDepleteCore.ConditionCheck(rMe, *pConDepTerm))
					{
						return FALSE;
					}
				}
			}
			
			INT iIdx = SkillInstanceData_T::CONDITION_AND_DEPLETE_TERM_NUMBER-1;
			ConDepTerm_T const* pConDepTerm = Skill_GetConDepTermByIndex(rSkillInfo, nIdx);
			if(NULL!=pConDepTerm)
			{
				if(FALSE==g_ConditionAndDepleteCore.ConditionCheckByItem(rMe, *pConDepTerm))
				{
					return FALSE;
				}
			}


			if(FALSE == TargetCheckForActivateOnce(rMe))
			{
				return FALSE;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::CommonDeplete(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nIdx=0;
			for( nIdx=0; SkillInstanceData_T::CONDITION_AND_DEPLETE_TERM_NUMBER-1> nIdx; ++nIdx)
			{
				ConDepTerm_T const* pConDepTerm = Skill_GetConDepTermByIndex(rSkillInfo, nIdx);
				if(NULL!=pConDepTerm)
				{
					if(FALSE==g_ConditionAndDepleteCore.Deplete(rMe, *pConDepTerm))
					{
						return FALSE;
					}
				}
			}

			INT iIdx = SkillInstanceData_T::CONDITION_AND_DEPLETE_TERM_NUMBER-1;
			ConDepTerm_T const* pConDepTerm = Skill_GetConDepTermByIndex(rSkillInfo, nIdx);
			if(NULL!=pConDepTerm)
			{
				if(FALSE==g_ConditionAndDepleteCore.DepleteByItem(rMe, *pConDepTerm))
				{
					return FALSE;
				}
			}


			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		BOOL SkillLogic_T::TargetCheckForEachTick(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(1!=rSkillInfo.GetSelectType()) //���ܱ����ѡĿ���
			{
				return TRUE;
			}
			Obj* pObj = GetTargetObj(rMe);
			if(NULL!=pObj)
			{
				if(IsCharacterObj(pObj->GetObjType()))
				{
					Obj_Character& rTar = *(Obj_Character*)pObj;
					if(FALSE == rTar.IsCanViewMe(&rMe))
					{
						rParams.SetErrCode(OR_INVALID_TARGET);
						rParams.SetErrParam(0);
						return FALSE;
					}
					switch (rSkillInfo.GetTargetMustInSpecialState())
					{
						case 0: //Ŀ������ǻ��
							if(FALSE == rTar.IsAlive())
							{
								rParams.SetErrCode(OR_TARGET_DIE);
								rParams.SetErrParam(0);
								return FALSE;
							}
							break;
						case 1://Ŀ�������ʬ��
							if(TRUE == rTar.IsAliveInDeed())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;
							}
							break;
						default:
							break;
					};
					if(TRUE==IsOutOfRange(rMe, rTar))
					{
						rParams.SetErrCode(OR_OUT_RANGE);
						rParams.SetErrParam(0);
						return FALSE;
					}
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::TargetCheckForActivateOnce(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(1!=rSkillInfo.GetSelectType()) //���ܱ����ѡĿ���
			{
				if(2==rSkillInfo.GetSelectType()) //��ѡ�����Ƿ����
				{
					WORLD_POS TargetPosition = rParams.GetTargetPosition();
					if(TRUE == IsOutOfRange(rMe, TargetPosition))
					{
						return FALSE;
					}
				}
				return TRUE;
			}
			Obj* pObj = GetTargetObj(rMe);
			if(NULL!=pObj)
			{
				if(IsCharacterObj(pObj->GetObjType()))
				{
					Obj_Character& rTar = *(Obj_Character*)pObj;
					if(FALSE == rTar.IsCanViewMe(&rMe))
					{
						rParams.SetErrCode(OR_INVALID_TARGET);
						rParams.SetErrParam(0);
						return FALSE;
					}
					switch (rSkillInfo.GetTargetCheckByObjType())
					{
						case 0: //Ŀ����������
							if(Obj::OBJ_TYPE_HUMAN!=rTar.GetObjType())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;//��������
							};
							break;
						case 1://Ŀ������ǳ���
							if(Obj::OBJ_TYPE_PET!=rTar.GetObjType())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;//��������
							}
							break;
						case 2://Ŀ������ǹ���
							if(Obj::OBJ_TYPE_MONSTER!=rTar.GetObjType())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;//��������
							}
							break;
						case -1:
						default:
							break;
					};
					switch (rSkillInfo.GetTargetLogicByStand())
					{
						case 0: //Ŀ���ʹ����Ӧ�����Ѻù�ϵ
							if(FALSE==rMe.IsFriend(&rTar) || FALSE == rTar.IsFriend(&rMe))
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;//��������
							};
							break;
						case 1://Ŀ���ʹ����Ӧ���ǵжԹ�ϵ
							if(FALSE == rMe.IsEnemy(&rTar) || FALSE == rTar.IsEnemy(&rMe))
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;//��������
							}
							break;
						case -1:
						default:
							break;
					};
					switch (rSkillInfo.GetTargetMustInSpecialState())
					{
						case 0: //Ŀ������ǻ��
							if(FALSE == rTar.IsAlive())
							{
								rParams.SetErrCode(OR_TARGET_DIE);
								rParams.SetErrParam(0);
								return FALSE;
							}
							break;
						case 1://Ŀ�������ʬ��
							if(TRUE == rTar.IsAliveInDeed())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;
							}
							break;
						default:
							break;
					};
					if(TRUE==rSkillInfo.IsPartyOnly())
					{
						if(FALSE == rMe.IsPartner(&rTar)|| FALSE == rTar.IsPartner(&rMe))
						{
							rParams.SetErrCode(OR_INVALID_TARGET);
							rParams.SetErrParam(0);
							return FALSE;
						}
					}
					if(TRUE==IsOutOfRange(rMe, rTar))
					{
						rParams.SetErrCode(OR_OUT_RANGE);
						rParams.SetErrParam(0);
						return FALSE;
					}
					if(-1!=rSkillInfo.GetSkillLevel())
					{
						if(TRUE == rMe.IsFriend(&rTar))
						{
							if(rSkillInfo.GetSkillLevel()>rTar.GetLevel())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);							
								return FALSE;
							}
						}
						if(TRUE == rMe.IsEnemy(&rTar))
						{
							if(rSkillInfo.GetSkillLevel()<rTar.GetLevel())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);							
								return FALSE;
							}
						}
					}
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		//method for interval use
		BOOL SkillLogic_T::IsOutOfRange(Obj_Character& rMe, Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			if (rMe.GetObjType() == Obj::OBJ_TYPE_PET)
				return FALSE;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			FLOAT fAcceptableDistanceError = 0.0f;
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
			{	
				fAcceptableDistanceError = AcceptableDistanceError_NS::ADE_FOR_HUMAN/2.0;
			}
			else
			{
				fAcceptableDistanceError = AcceptableDistanceError_NS::ADE_FOR_NPC/4.0;
			}

			if(CHARACTER_LOGIC_MOVE == rTar.GetCharacterLogic())
			{
				fAcceptableDistanceError += 0.5; //Ŀ���ƶ��У�����ٷŴ�0.5��
			}
			FLOAT fRangeMinSq = rSkillInfo.GetOptimalRangeMin();
			FLOAT fRangeMaxSq = rSkillInfo.GetOptimalRangeMax();

			fRangeMinSq -= fAcceptableDistanceError;
			fRangeMaxSq += fAcceptableDistanceError;
			fRangeMinSq = fRangeMinSq>0?fRangeMinSq*fRangeMinSq:0;
			fRangeMaxSq = fRangeMaxSq>0?fRangeMaxSq*fRangeMaxSq:0;
			WORLD_POS const& posStart = *(rMe.getWorldPos());
			WORLD_POS const& posEnd = *(rTar.getWorldPos());
			FLOAT fDistToTargetSq	= DistanceSq(posStart, posEnd);
			
			if(fDistToTargetSq < fRangeMinSq)
			{
				rParams.SetErrCode(OR_OUT_RANGE);
				rParams.SetErrParam((INT)(fDistToTargetSq - fRangeMinSq));
				return TRUE;
			}
			if(fDistToTargetSq > fRangeMaxSq)
			{
				rParams.SetErrCode(OR_OUT_RANGE);
				rParams.SetErrParam((INT)(fDistToTargetSq - fRangeMaxSq));
				return TRUE;
			}
			rParams.SetErrCode(OR_OK);
			return FALSE;
			__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL SkillLogic_T::IsOutOfRange(Obj_Character& rMe, WORLD_POS const& rPosTarget) const
		{
			__ENTER_FUNCTION
			if (rMe.GetObjType() == Obj::OBJ_TYPE_PET)
			{// ����ǳ�����û�о��������
                return FALSE;
			}
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			FLOAT nAcceptableDistanceError = 0.0f;
			FLOAT fRangeMinSq = rSkillInfo.GetOptimalRangeMin() - nAcceptableDistanceError;
			FLOAT fRangeMaxSq = rSkillInfo.GetOptimalRangeMax() + nAcceptableDistanceError;
			fRangeMinSq = fRangeMinSq>0?fRangeMinSq*fRangeMinSq:0;
			fRangeMaxSq = fRangeMaxSq>0?fRangeMaxSq*fRangeMaxSq:0;
			const WORLD_POS& posStart = *(rMe.getWorldPos());
			FLOAT fDistToTargetSq	= DistanceSq(posStart, rPosTarget);
			
			if(fDistToTargetSq < fRangeMinSq)
			{
				rParams.SetErrCode(OR_OUT_RANGE);
				rParams.SetErrParam((INT)(fDistToTargetSq - fRangeMinSq));
				return TRUE;
			}
			if(fDistToTargetSq > fRangeMaxSq)
			{
				rParams.SetErrCode(OR_OUT_RANGE);
				rParams.SetErrParam((INT)(fDistToTargetSq - fRangeMaxSq));
				return TRUE;
			}
			rParams.SetErrCode(OR_OK);
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		FLOAT SkillLogic_T::DistanceSq(WORLD_POS const& posStart, WORLD_POS const& posEnd) const
		{
			__ENTER_FUNCTION
			FLOAT fDistX, fDistZ;
			fDistX	= posEnd.m_fX - posStart.m_fX;
			fDistZ	= posEnd.m_fZ - posStart.m_fZ;
			return fDistX*fDistX+fDistZ*fDistZ; 
			__LEAVE_FUNCTION
			return 0;
		}
		VOID SkillLogic_T::SetCooldown(Obj_Character& rMe, CooldownID_t nID, INT nCooldownTime) const
		{
			__ENTER_FUNCTION
			rMe.SetCooldown((CooldownID_t)nID, nCooldownTime);
			__LEAVE_FUNCTION
		}
		VOID SkillLogic_T::SetAutoRepeatCooldown(Obj_Character& rMe, INT nCooldownTime) const
		{
			__ENTER_FUNCTION
			nCooldownTime += rMe.GetAutoRepeatCooldown();
			rMe.SetAutoRepeatCooldown(nCooldownTime);
			__LEAVE_FUNCTION
		}
		Obj* SkillLogic_T::GetTargetObj(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		
			//Check Target require, return err when failure
			Obj_Character* pTar = (Obj_Character*)rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
			if(NULL==pTar)
			{
				rParams.SetErrCode(OR_INVALID_TARGET);
				return NULL;
			}
			if(NULL==pTar || FALSE == pTar->IsActiveObj())
			{
				rParams.SetErrCode(OR_INVALID_TARGET);
				return NULL;
			}
			return (Obj*)pTar;
			__LEAVE_FUNCTION
			return NULL;
		}
		BOOL SkillLogic_T::IsTargetAlive(Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			if(rTar.IsAlive())
				return TRUE;
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;		
		}
		BOOL SkillLogic_T::IsHit(Obj_Character& rMe, Obj_Character& rTar, INT nAccuracy) const
		{
			__ENTER_FUNCTION
			CombatCore_T myCombatCore;

			nAccuracy = myCombatCore.CalculateHitRate(rMe, rTar);

			INT nRand = 0;
			Scene* pScene = rMe.getScene();
			if(NULL!=pScene)
			{
				nRand = pScene->GetRand100();
			}			
			return myCombatCore.IsHit(nAccuracy, nRand);
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::IsCriticalHit(Obj_Character& rMe, INT nCriticalRate, Obj_Character& rTag) const
		{
			__ENTER_FUNCTION
			CombatCore_T myCombatCore;
			if(-1==nCriticalRate)
			{
				nCriticalRate = rMe.GetCritical();
			}
			INT nRand = 0;
			Scene* pScene = rMe.getScene();
			if(NULL!=pScene)
			{
				nRand = pScene->GetRand100();
			}
			return myCombatCore.IsCriticalHit( rMe, nCriticalRate, nRand, rTag.GetToughness() );
			__LEAVE_FUNCTION
			return FALSE;
		}
		Time_t SkillLogic_T::RefixPlayActionTimeWithAttackRate(Time_t nPlayActionTime, INT nAttackRate) const
		{
			return (nPlayActionTime * nAttackRate)/100;			
		}
		Time_t SkillLogic_T::RefixCooldownTimeWithAttackRate(Time_t nCooldownTime, INT nAttackRate) const
		{
			return (nCooldownTime * nAttackRate)/100;
			
		}
		VOID SkillLogic_T::RefixPowerByRate(SkillInfo_T& rSkill, INT nRate) const
		{
			__ENTER_FUNCTION
			INT nDeplete = 0;
			if (TRUE == CanBeRefixed())
			{
				rSkill.SetPowerRefixByRate(nRate);
			}
			__LEAVE_FUNCTION
		}
		VOID SkillLogic_T::RefixDepleteByRate(SkillInfo_T& rSkill, INT nRate) const
		{
			__ENTER_FUNCTION
			__ENTER_FUNCTION
			INT nDeplete = 0;
			if (TRUE == CanBeRefixed())
			{
				rSkill.SetDepleteRefixByRate(nRate);
			}
			__LEAVE_FUNCTION
			__LEAVE_FUNCTION
		}

		BOOL SkillLogic_T::ScanUnitForTarget(Obj_Character& rMe, FLOAT fX,FLOAT fZ, OBJLIST& rTargets)const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Scene* pScene = rMe.getScene();
			SCANOPERATOR_SECTORSKILL_INIT ScanOperatorIniter;
			ScanOperatorIniter.m_pSkillInfo = &rSkillInfo;
			ScanOperatorIniter.m_pMe = &rMe;
			ScanOperatorIniter.m_pScene = rMe.getScene();
			ScanOperatorIniter.m_pTargets = &rTargets;
			ScanOperatorIniter.m_fRadius = rSkillInfo.GetRadius();
			ScanOperatorIniter.m_CentrePoint.m_fX = fX;
			ScanOperatorIniter.m_CentrePoint.m_fZ = fZ;

			if(-1==rSkillInfo.GetMaxTargetNumber()) //-1Ϊ��������
			{
				ScanOperatorIniter.m_nCount = rTargets.MAX_OBJ_LIST_SIZE-1;
			}
			else
			{
				ScanOperatorIniter.m_nCount= rSkillInfo.GetMaxTargetNumber();
			}
			ScanOperator_SectorSkill ScanOperator;
			ScanOperator.Init(&ScanOperatorIniter);
			if(NULL!=pScene)
			{
				if(FALSE==pScene->Scan(&ScanOperator))
				{
					rParams.SetErrCode(OR_ERROR);
					return FALSE;					
				}
			}
			else
			{
				rParams.SetErrCode(OR_ERROR);
				return FALSE;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		
		BOOL SkillLogic_T::ActivateOnce(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			//��ý�ɫ���ϵļ�����Ϣ����
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			//��ý�ɫ���ϵ�Ŀ�꼰���Ķ���
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(rSkillInfo.GetSkillID());
			if( NULL == pSkillTemplate )
			{
				return FALSE;
			}
			OBJLIST	Targets;
			HitFlagsForOBJLIST_T HitFlagList;

			//���㹥����Χ�ڵ���ЧĿ�����
			if(FALSE == CalculateTargetList(rMe, Targets))
			{
				rParams.SetErrCode( OR_NO_TARGET );
				return FALSE;
			}
			
			////���ݼ���ID��ȫ�ּ���ģ��������л�ü���ģ��ʵ��
			//const SkillTemplateData_T* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID( iID );
			HitFlagList.ClearAllFlags();
			INT nIdx = 0;
			for(nIdx=0; Targets.m_Count>nIdx; ++nIdx)
			{
				Obj* pTarget = Targets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					Obj_Character* pChar = (Obj_Character*)pTarget;
					//������У�������������б���Ϊ����
					if(TRUE==HitThisTarget(rMe, *pChar))
					{
						HitFlagList.MarkFlagByIndex(nIdx); //Mark this Target Hitted
					}
					RegisterBeSkillEvent(*pChar, rMe, rSkillInfo.GetSkillID(), rSkillInfo.GetDelayTime());
				}
			}
			rParams.SetTargetCount(Targets.m_Count);

			//BroadCast The Target List message
			BroadcastTargetListMessage( rMe, Targets, HitFlagList );

			for(nIdx=0; Targets.m_Count>nIdx; ++nIdx)
			{
				Obj* pTarget = Targets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					//����Ѿ�����
					if(TRUE==HitFlagList.GetFlagByIndex(nIdx))
					{
						Obj_Character* pChar = (Obj_Character*)pTarget;
						//�ж��Ƿ�Ϊ����һ��
						BOOL bCriticalHit = CriticalHitThisTarget(rMe, *pChar);
						if( pSkillTemplate->IsRunSkill() )
						{
							rMe.setWorldPos( pTarget->getWorldPos() );
							//g_pLog->FastSaveLog( LOG_FILE_1, "˲�Ƽ��� �յ����Ѵ���" );
						}
						//ʵ�־���Ĵ��Ч�����˷����ɾ���������ʵ��
						//����Ƿǳ�����Ч�������RegisterImpactEvent
						EffectOnUnitOnce(rMe, *pChar, bCriticalHit);
					}
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::ActivateEachTick(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			OBJLIST	Targets;
			HitFlagsForOBJLIST_T HitFlagList;

			if(FALSE == CalculateTargetList(rMe, Targets))
			{
				return FALSE;
			}
			HitFlagList.ClearAllFlags();
			INT nIdx = 0;
			for(nIdx=0; Targets.m_Count>nIdx; ++nIdx)
			{
				Obj* pTarget = Targets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					Obj_Character* pChar = (Obj_Character*)pTarget;
					if(TRUE==HitThisTarget(rMe, *pChar))
					{
						HitFlagList.MarkFlagByIndex(nIdx); //Mark this Target Hitted
					}
				}
			}
			rParams.SetTargetCount(Targets.m_Count);

			//BroadCast The Target List message
			BroadcastTargetListMessage( rMe, Targets, HitFlagList );

			for(nIdx=0; Targets.m_Count>nIdx; ++nIdx)
			{
				Obj* pTarget = Targets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					if(TRUE==HitFlagList.GetFlagByIndex(nIdx))
					{
						Obj_Character* pChar = (Obj_Character*)pTarget;
						BOOL bCriticalHit = CriticalHitThisTarget(rMe, *pChar);
						EffectOnUnitEachTick(rMe, *pChar, bCriticalHit);
					}
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::EffectOnUnitEachTick(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			return TRUE;
		}
		BOOL SkillLogic_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			return TRUE;
		}
		BOOL SkillLogic_T::HitThisTarget(Obj_Character& rMe, Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Scene* pScene = rMe.getScene();
			if(TRUE==rMe.IsFriend(&rTar))
			{
				//���ѷ�ʹ�õļ���100%����
				return TRUE;
			}
			
			INT iHurtDelayTime = GetHurtDelayTime(rSkillInfo, rMe, rTar);
			rParams.SetHurtDelayTime( iHurtDelayTime );
			//Hit Or Miss
			//�����������ж��Ƿ����
			//ע�⣺��ʱ�Լ����ͷŶ��ԣ�Ŀ������֤Ϊ�Ϸ������ֻ�ܼ��ܱ����������Ӱ��
			//���û������
			if(FALSE == IsHit(rMe, rTar, rSkillInfo.GetAccuracy()))
			{
				if(NULL!=pScene)
				{
					//ֱ����ȫ��event����ע��û�����е��¼�
					pScene->GetEventCore().RegisterSkillMissEvent(rTar.GetID(), rMe.GetID(), rSkillInfo.GetSkillID(), rParams.GetHurtDelayTime() );
				}
				return FALSE;
			}
			//����������У���ȫ��event����ע�Ἴ������Ŀ���¼�
			if(NULL!=pScene)
			{
				pScene->GetEventCore().RegisterSkillHitEvent(rTar.GetID(), rMe.GetID(), rSkillInfo.GetSkillID(), rSkillInfo.GetDelayTime());
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::CriticalHitThisTarget(Obj_Character & rMe, Obj_Character & rTar) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			if(TRUE==rMe.IsFriend(&rTar))
			{
				//�����Լ��ܲ��л���һ�����Ѻü���û��
				return FALSE;
			}
			// calculate critical hit
			if(TRUE == IsCriticalHit(rMe, rSkillInfo.GetCriticalRate(), rTar))
			{
				rMe.OnCriticalHitTarget(rSkillInfo.GetSkillID(), rTar);
				rTar.OnBeCriticalHit(rSkillInfo.GetSkillID(), rMe);
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		
		BOOL SkillLogic_T::CalculateTargetList(Obj_Character & rMe, OBJLIST& rTargets) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			rTargets.CleanUp();
			ID_t nTargetMode = rSkillInfo.GetTargetingLogic();
			BOOL bRet=FALSE;
			switch (nTargetMode)
			{
				case TARGET_SELF:
					{
						rTargets.m_aObj[0] = &rMe;
						++rTargets.m_Count;
						bRet = TRUE;
					}
					break;
				case TARGET_MY_PET:
					{
						for( INT i=0; i<MAX_MONSTER_PET_TAKE; ++i )
						{
							Obj* pPet = rMe.GetMyPet( i );
							if(NULL!=pPet)
							{
								rTargets.m_aObj[rTargets.m_Count] = pPet;
								++rTargets.m_Count;
								bRet = TRUE;
							}
						}
					}
					break;
				case TARGET_MY_SHADOW_GUARD:
					{
						Obj* pGuard = rMe.GetMyShadowGuard();
						if(NULL!=pGuard)
						{
							rTargets.m_aObj[0] = pGuard;
							++rTargets.m_Count;
							bRet = TRUE;
						}
					}
					break;
				case TARGET_MY_MASTER:
					{
						Obj* pMaster = rMe.GetMyMaster();
						if(NULL!=pMaster)
						{
							rTargets.m_aObj[0] = pMaster;
							++rTargets.m_Count;
							bRet = TRUE;
						}
					}
					break;
				case TARGET_AE_AROUND_SELF:
					{
						WORLD_POS const* pPos = rMe.getWorldPos();
						if(NULL!=pPos)
						{
							bRet = ScanUnitForTarget(rMe, pPos->m_fX,pPos->m_fZ,rTargets);
						}
					}
					break;
				case TARGET_SPECIFIC_UNIT:
					{
						Obj* pTarget = rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
						if(NULL!=pTarget)
						{
							rTargets.m_aObj[0] = pTarget;
							++rTargets.m_Count;
							bRet = TRUE;
						}
					}
					break;
				case TARGET_AE_AROUND_UNIT:
					{
						Obj* pTarget = rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
						if(NULL!=pTarget)
						{
							WORLD_POS const* pPos = pTarget->getWorldPos();
							if(NULL!=pPos)
							{
								bRet = ScanUnitForTarget(rMe, pPos->m_fX, pPos->m_fZ, rTargets);
							}
						}
					}
					break;
				case TARGET_AE_AROUND_POSITION:
					{
						WORLD_POS const& rPos = rParams.GetTargetPosition();
						bRet = ScanUnitForTarget(rMe, rPos.m_fX, rPos.m_fZ, rTargets);
					}
					break;
				case TARGET_POS:
					rTargets.m_aObj[0] = &rMe;
					++rTargets.m_Count;
					bRet = TRUE;
					break;
				default:
					{
						bRet = FALSE;
					}
					break;
			}
			return bRet;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::RegisterImpactEvent(Obj_Character& rReceiver, Obj_Character& rSender, OWN_IMPACT& rImp, Time_t nDelayTime, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rSender.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rSender.GetTargetingAndDepletingParams();
			Scene* pScene = rReceiver.getScene();
			// Routine process
			rImp.SetSkillID(rSkillInfo.GetSkillID());
			if(TRUE==bCriticalFlag)
			{
				rImp.MarkCriticalFlag();
			}
			// register impact
			//��ȫ���¼�����ע��Ч����Ч
			if(NULL!=pScene)
			{
				return pScene->GetEventCore().RegisterImpactEvent(rReceiver.GetID(), rSender.GetID(), rImp, nDelayTime, rImp.GetSkillID());
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::RegisterSkillMissEvent(Obj_Character& rReceiver, Obj_Character& rSender, SkillID_t nSkill, Time_t nDelayTime) const
		{
			__ENTER_FUNCTION
			Scene* pScene = rReceiver.getScene();
			if(NULL!=pScene)
			{
				return pScene->GetEventCore().RegisterSkillMissEvent(rReceiver.GetID(), rSender.GetID(), nSkill, nDelayTime);
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::RegisterBeSkillEvent(Obj_Character& rReceiver, Obj_Character& rSender, SkillID_t nSkill, Time_t nDelayTime) const
		{
			__ENTER_FUNCTION

			INT nBehaviorType = Skill_GetSkillStandFlag(nSkill);

			Scene* pScene = rReceiver.getScene();
			if(NULL!=pScene)
			{
				return pScene->GetEventCore().RegisterBeSkillEvent(rReceiver.GetID(), rSender.GetID(), nBehaviorType, nDelayTime);
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::RegisterActiveObj(Obj& rObj, Obj_Character& rSender, Time_t nDelayTime) const
		{
			__ENTER_FUNCTION
			Scene* pScene = rSender.getScene();
			if(NULL!=pScene)
			{
				return pScene->GetEventCore().RegisterActiveSpecialObjEvent(rObj.GetID(), nDelayTime);
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID SkillLogic_T::BroadcastTargetListMessage(	Obj_Character& rMe, 
														OBJLIST& rTargets, 
														HitFlagsForOBJLIST_T& rHitFlagsForObjList
														) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			GCTargetListAndHitFlags msg;
			GCTargetListAndHitFlags::HitFlagList_T& rHitFlags		= msg.GetHitFlagList();

			msg.SetDataType(msg.UNIT_USE_SKILL);
			msg.SetObjID(rMe.GetID());
			msg.SetUserPos(*(rMe.getWorldPos()));
			msg.SetLogicCount(rMe.GetLogicCount());
			msg.SetSkillOrSpecialObjDataID(rSkillInfo.GetSkillID());
			msg.SetTargetID(rParams.GetTargetObj());
			msg.SetTargetPos(rParams.GetTargetPosition());
			msg.SetDir(rParams.GetTargetDirection());
			BYTE nNum = rTargets.m_Count;
			if(msg.MAX_TARGET_LIST_SIZE<nNum)
			{
				nNum = msg.MAX_TARGET_LIST_SIZE;
			}
			msg.SetTargetNum(nNum);
			int iTemp = nNum;

			INT nIdx=0;
			for(nIdx=0; nNum>nIdx; ++nIdx)
			{
				Obj* pTarget = rTargets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					msg.SetTargetObjIDByIndex(nIdx, pTarget->GetID());
					if(TRUE == rHitFlagsForObjList.GetFlagByIndex(nIdx))
					{
						rHitFlags.MarkFlagByIndex(nIdx);
					}
				}
			}
			Scene* pScene = rMe.getScene();
			if(NULL!=pScene)
			{
				pScene->BroadCast(&msg, &rMe, TRUE);
			}
			g_pLog->FastSaveLog(LOG_FILE_1, "SkillLogic_T::BroadcastTargetListMessage nNum = %d", nNum);
			__LEAVE_FUNCTION
		}
		BOOL SkillLogic_T::IsWantedTarget(Obj_Character& rMe, Obj_Character& rTar, SkillInfo_T const& rSkillInfo) const
		{
			__ENTER_FUNCTION
			switch (rSkillInfo.GetTargetCheckByObjType())
			{
				case 0: //Ŀ����������
					if(Obj::OBJ_TYPE_HUMAN!=rTar.GetObjType())
					{
						return FALSE;//��������
					};
					break;
				case 1://Ŀ������ǳ���
					if(Obj::OBJ_TYPE_PET!=rTar.GetObjType())
					{
						return FALSE;//��������
					}
					break;
				case 2://Ŀ������ǹ���
					if(Obj::OBJ_TYPE_MONSTER!=rTar.GetObjType())
					{
						return FALSE;//��������
					}
					break;
				case -1:
				default:
					break;
			};
			if(0==rSkillInfo.GetTargetMustInSpecialState())
			{
				if(FALSE==rTar.IsAlive())
				{
					return FALSE;
				}
			}
			if(1==rSkillInfo.GetTargetMustInSpecialState())
			{
				if(FALSE==rTar.IsDie())
				{
					return FALSE;
				}
			}
			if(0==rSkillInfo.GetTargetLogicByStand())
			{
				if(FALSE==rMe.IsFriend(&rTar))
				{
					return FALSE;
				}
				else
				{
					if(TRUE == rSkillInfo.IsPartyOnly())
					{
						if(FALSE == rMe.IsPartner(&rTar))
						{
							return FALSE;
						}
					}
				}
			}
			if(1==rSkillInfo.GetTargetLogicByStand())
			{
				if(FALSE==rMe.IsEnemy(&rTar))
				{
					return FALSE;
				}
			}
			return IsSpecificTargetWanted(rMe, rTar, rSkillInfo);
			__LEAVE_FUNCTION
			return FALSE;
		}
	
		SkillLogic_T const* Skill_GetLogic(SkillInfo_T const& rSkill)
		{
		__ENTER_FUNCTION
			ID_t nLogicID = rSkill.GetLogicID();
			if(INVALID_ID==nLogicID)
			{
				return NULL;
			}

			return g_SkillLogicList.GetLogicById(nLogicID);
		__LEAVE_FUNCTION
			return NULL;
		}
		//������ӵ�������Ҫ�����ӵ��ķ���ʱ��Ͷ���ʱ��,����ֻ���Ƕ���ʱ��
		INT SkillLogic_T::GetHurtDelayTime( SkillInfo_T &rSkillInfo, Obj_Character &rMe, Obj_Character &rTar ) const
		{
			//����ӵ��ٶ�,��λ�� ����/��
			INT iBulletSpeed = rSkillInfo.GetSpeedOfBullet();
			//ʱ�䵥λΪ����
			INT iTime = 0;
			//�ٶ����Ϊ INVALID_ID ˵�������ӵ����ܣ����������ʱ�䣬ֻ������ʱ��
			if( INVALID_ID != iBulletSpeed )
			{
				const WORLD_POS* pMyPos		= rMe.getWorldPos();
				const WORLD_POS* pTarPos	= rTar.getWorldPos();
				//��Ϊ����еĵ�λԼ��������ľ������ף��ٶȵ�λΪ���ף�Ҫ����һ�µĵ�λ
				INT iDistance	= (INT)(rMe.WordPosLength(pMyPos->m_fX-pTarPos->m_fX, pMyPos->m_fZ-pTarPos->m_fZ) * 100);
				//��ʱ�䵥λת��Ϊ����
				iTime = iDistance * 1000 / iBulletSpeed;
			}
			iTime += rSkillInfo.GetHurtDelayTime() - 100;
			if( 0>iTime )
			{
				iTime = 0;
			}
g_pLog->FastSaveLog( LOG_FILE_1, "SkillLogic_T::GetHurtDelayTime = %d", iTime );			
			return iTime;
		}
		//BOOL SkillLogic_T::IsBeatFly( Obj_Character& rMe, Skill_Module::SkillTemplateData_T const* pTemplate ) const
		//{
		//	__ENTER_FUNCTION
		//		if( pTemplate == NULL )
		//			return FALSE;

		//	FLOAT fBeatFlyRate = pTemplate->GetBeatFlyRate();
		//	return IsHappenByRate( fBeatFlyRate, rMe );

		//	__LEAVE_FUNCTION
		//		return FALSE;
		//}

		//BOOL SkillLogic_T::IsBeatBack( Obj_Character& rMe, Skill_Module::SkillTemplateData_T const * pTemplate ) const
		//{
		//	__ENTER_FUNCTION
		//		if( pTemplate == NULL )
		//			return FALSE;

		//	FLOAT fBeatFlyRate = pTemplate->GetBeatBackRate();
		//	return IsHappenByRate( fBeatFlyRate, rMe );

		//	__LEAVE_FUNCTION
		//		return FALSE;
		//}

		//BOOL SkillLogic_T::IsHappenByRate( FLOAT fRate, Obj_Character& rMe ) const
		//{
		//	__ENTER_FUNCTION

		//		Scene * pScene = rMe.getScene();

		//	if(NULL==pScene)
		//		return FALSE;

		//	FLOAT fRand = (FLOAT)(pScene->GetRand100());

		//	if( fRand <= fRate  )
		//		return TRUE;

		//	return FALSE;

		//	__LEAVE_FUNCTION
		//		return FALSE;

		//}

		//VOID SkillLogic_T::CalculateBeatBackPos( Obj_Character& rMe, Obj* rTag, SkillTemplateData_T const* pTemplate, WORLD_POS &sPos ) const
		//{
		//	__ENTER_FUNCTION

		//	FLOAT fDistance			= pTemplate->GetBeatBackDistance();
		//	WORLD_POS const* pPos	= rMe.getWorldPos();
		//	WORLD_POS sPosMe		= *pPos;
		//	pPos					= rTag->getWorldPos();
		//	WORLD_POS sPosTag		= *pPos;
		//	WORLD_POS sDir;
		//	sDir.m_fX = sPosTag.m_fX - sPosMe.m_fX;
		//	sDir.m_fZ = sPosTag.m_fZ - sPosMe.m_fZ;

		//	//��ȡ����
		//	NormalizeWorldPos( sDir );
		//	//����ϣ�������Ŀ���
		//	WORLD_POS sFinalPos;
		//	sFinalPos.m_fX = sPosTag.m_fX + fDistance*sDir.m_fX;
		//	sFinalPos.m_fZ = sPosTag.m_fZ + fDistance*sDir.m_fZ;

		//	FLOAT fLengthOfStep = 0.5f;
		//	FLOAT fLengthTmp	= 0;
		//	WORLD_POS sPosTmp;
		//	//����ǰ�������Ը��ӱ߳�(0.5)Ϊ������Ŀ�귽���ѯ,���ϰ��򵽴�Ŀ���Ϊֹ
		//	while( TRUE )
		//	{
		//		//�Ƚϵ�ǰλ�ú�Ŀ���ľ���,���С�ڲ�����ֱ�ӵ��ﲢ����
		//		fLengthTmp = WordPosLength( sFinalPos.m_fX-sPosTag.m_fX, sFinalPos.m_fZ-sPosTag.m_fZ );
		//		if( fLengthTmp < fLengthOfStep )
		//		{
		//			if ( rMe.getScene()->GetMap()->IsCanGo(sFinalPos) )
		//				sPosTag = sFinalPos;
		//			break;
		//		}
		//		//������ڲ���������һ�������ľ���
		//		sPosTmp.m_fX = sPosTag.m_fX + sDir.m_fX*fLengthOfStep;
		//		sPosTmp.m_fZ = sPosTag.m_fZ + sDir.m_fZ*fLengthOfStep;
		//		//�ж���λ���Ƿ����ͨ��
		//		if ( rMe.getScene()->GetMap()->IsCanGo(sPosTmp) )
		//			sPosTag = sPosTmp;
		//		//����ͨ�������
		//		else break;
		//	}
		//	sPos = sPosTag;
		//	__LEAVE_FUNCTION

		//}

		//VOID SkillLogic_T::NormalizeWorldPos( WORLD_POS& sPos ) const
		//{
		//	__ENTER_FUNCTION
		//	FLOAT fLength = WordPosLength( sPos.m_fX, sPos.m_fZ );
		//	sPos.m_fX /= fLength;
		//	sPos.m_fZ /= fLength;
		//	__LEAVE_FUNCTION
		//}

		//FLOAT SkillLogic_T::WordPosLength( FLOAT fX, FLOAT fY ) const
		//{
		//	__ENTER_FUNCTION
		//	FLOAT fLength = sqrt( pow(fX,2.0f) + pow(fY,2.0f) );
		//	return fLength;

		//	__LEAVE_FUNCTION
		//	return 0;
		//}
	}
}






















