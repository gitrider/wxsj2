///////////////////////////////////////////////////////////////////////////////
// �ļ�����ImpactLogic.cpp
// ����Ա������
// ����˵����Ч���Ļ����࣬ʵ�������еĹ����ӿں͹����Ĺ̶��߼��ӿ�
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ImpactLogic.h"
#include "Scene.h"
#include "GameStruct_Impact.h"
#include "LogicManager.h"
#include "ScanOperator_AEImpact.h"
#include "ActionDelegator.h"

using namespace Action_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Impact_Module
	{
		//class ImpactLogic_T
		//event handler
		VOID ImpactLogic_T::HeartBeat(OWN_IMPACT& rImp, Obj_Character& rMe)const
		{
			__ENTER_FUNCTION
			BOOL bContinue = TRUE;
			//�Ƿ���Ҫ����֧��
			if(TRUE==Impact_NeedChannelSupport(rImp))
			{
				//����֧���Ƿ���Ч
				if(FALSE == IsSupportStillOn(rImp, rMe))
				{
					bContinue = FALSE;
				}
			}
			if(FALSE == rMe.IsAliveInDeed())
			{
				if(FALSE==Impact_IsRemainOnCorpse(rImp))
				{
					bContinue = FALSE;
				}
			}
			if(FALSE == SpecialHeartBeatCheck(rImp, rMe))
			{
				bContinue = FALSE;
			}
			if(TRUE == bContinue)
			{
				INT nDeltaTime = rMe.GetLogicTime();
				//����ǳ�����Ч��
				if(TRUE==IsOverTimed())
				{
					//����ǰ�ʱ��������
					if(TRUE==IsIntervaled())
					{
						//������
						IntervalCalc(rImp,rMe, nDeltaTime);
					}
					//�������ʱ��
					ContinuanceCalc(rImp, rMe, nDeltaTime);
				}
			}
			else
			{
				rMe.Impact_OnImpactFadeOut(rImp);
			}
			__LEAVE_FUNCTION
		}
		//public function
		BOOL ImpactLogic_T::ScanUnitForTarget(OWN_IMPACT& rImp, Obj_Character& rMe, WORLD_POS const& rPos, FLOAT fRadius, INT nListSize, OBJLIST& rTargets)const
		{
			Scene* pScene = rMe.getScene();
			SCANOPERATOR_AEIMPACT_INIT ScanOperatorIniter;
			ScanOperatorIniter.m_pLogic = this;
			ScanOperatorIniter.m_pMe = &rMe;
			ScanOperatorIniter.m_pScene = pScene;
			ScanOperatorIniter.m_pTargets = &rTargets;
			ScanOperatorIniter.m_fRadius = fRadius;

			if(-1==nListSize) //-1Ϊ��������
			{
				ScanOperatorIniter.m_nCount = rTargets.MAX_OBJ_LIST_SIZE-1;
			}
			else
			{
				ScanOperatorIniter.m_nCount = nListSize;
			}
			ScanOperatorIniter.m_Impact = rImp;
			ScanOperatorIniter.m_CentrePoint.m_fX = rPos.m_fX;
			ScanOperatorIniter.m_CentrePoint.m_fZ = rPos.m_fZ;
			ScanOperator_AEImpact ScanOperator;
			ScanOperator.Init(&ScanOperatorIniter);
			if(NULL!=pScene)
			{
				if(FALSE==pScene->Scan(&ScanOperator))
				{
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
			return TRUE;
		}
		BOOL ImpactLogic_T::IsSupportStillOn(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			Obj* pObj = Impact_GetCaster(rMe, rImp);
			if(NULL==pObj)
			{
				return FALSE;
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				return FALSE;
			}
			Obj_Character& rCaster = *(Obj_Character*)pObj;
			if(FALSE == GetGlobalActionDelegator().IsChanneling(rCaster))
			{
				return FALSE;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID ImpactLogic_T::ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const
		{
			__ENTER_FUNCTION
			//ȡ���ܳ���ʱ��
			INT const nContinuance = rImp.GetContinuance();
			//ȡ����ȥ�ĳ���ʱ��
			INT nContinuanceElapsed = rImp.GetContinuanceElapsed();
			//���������ɢ״̬
			if(FALSE == rImp.IsFadeOut())
			{
				//����ǳ�����Ч��
				if(TRUE == IsOverTimed())
				{
					if(-1==nContinuance)
					{
						return; //-1Ϊ����ʱ�����޳�
					}
					if(nContinuanceElapsed<=nContinuance)
					{
						nContinuanceElapsed += nDeltaTime;
					}
					//�����ʱ,����ɢ
					if(nContinuanceElapsed>nContinuance)
					{
						// prepare delete this impact
						rMe.Impact_OnImpactFadeOut(rImp);
					}
					//������ȥʱ��
					rImp.SetContinuanceElapsed(nContinuanceElapsed);
				}
			}
			__LEAVE_FUNCTION
		}
		VOID ImpactLogic_T::IntervalCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const 
		{
			__ENTER_FUNCTION
			//process interval
			INT nElapsedInterval = rImp.GetIntervalElapsed();
			INT nContinuance = rImp.GetContinuance();
			if(nDeltaTime>nContinuance && 0<=nContinuance)
			{
				nDeltaTime = nContinuance;
			}
			nElapsedInterval += nDeltaTime;
			INT const nInterval = Impact_GetInterval(rImp);
			while (nElapsedInterval >= nInterval)
			{
				// Impact activited
				OnIntervalOver(rImp, rMe);
				nElapsedInterval -= nInterval;
			}
			rImp.SetIntervalElapsed(nElapsedInterval);
			__LEAVE_FUNCTION
		}
		BOOL ImpactLogic_T::RefixContinuanceByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			if(0<rImp.GetContinuance() && TRUE == Impact_IsOverTimed(rImp))
			{
				rImp.SetContinuance(Float2Int((rImp.GetContinuance()*nRate)/100.0f));
			}
			__LEAVE_FUNCTION
			return TRUE;
		}

		ImpactLogic_T const* Impact_GetLogic(OWN_IMPACT const& rImp)
		{
		__ENTER_FUNCTION
			ID_t nLogicID = Impact_GetLogicID(rImp);

			return g_ImpactLogicList.GetLogicById(nLogicID);
		__LEAVE_FUNCTION
			return NULL;
		}
	};
};
