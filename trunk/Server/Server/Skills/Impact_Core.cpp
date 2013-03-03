///////////////////////////////////////////////////////////////////////////////
// �ļ�����Impact_Core.cpp
// ����Ա������
// ����˵����Ч���ں�ģ��
//
// �޸ļ�¼��
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
			//���OWN_IMPACT
			rImp.CleanUp();
			//����impactЧ���������
			rImp.SetDataIndex(nDataIndex);
			//����������ñ���Ч����Ķ���
			ImpactData_T const* pData = Impact_GetDataBlock(nDataIndex);
			if(NULL!=pData)
			{
				const_cast<ImpactData_T*>(pData)->SetReserveParam1(nReserveParam1);//��ȡ��������(����:��������)
				rImp.SetImpactID(pData->GetImpactID());
				//����Ч������ʱ��
				rImp.SetContinuance(pData->GetContinuance());
				//���Ч���߼�����
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
					//ʹ�þ���Ч���߼���ʼ��rImp��ص�����
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
			//����nDataIndex��Ӧ��Ч���߼���ʼ��rImp
			if(TRUE == InitImpactFromData(nDataIndex, impact, rTar))
			{
				//����߼�����
				ImpactLogic_T const* pLogic = Impact_GetLogic(impact);
				if(NULL==pLogic)
				{
					Assert(NULL=="[ImpactCore_T::SendImpactToUnit]: Can't find sprcific logic for this impact.");
					return FALSE;
				}
				//���û��ı��
				if(TRUE == bCriticalFlag)
				{
					impact.MarkCriticalFlag();
				}
				if(0!=nRefixRate)
				{
					//��Ч���߼�����������
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
			//����nDataIndex��Ӧ��Ч���߼���ʼ��rImp
			if(TRUE == InitImpactFromData(nDataIndex, impact, rTar, nReserveParam1))
			{
				//����߼�����
				ImpactLogic_T const* pLogic = Impact_GetLogic(impact);
				if(NULL==pLogic)
				{
					Assert(NULL=="[ImpactCore_T::SendImpactToUnit]: Can't find sprcific logic for this impact.");
					return FALSE;
				}
				//���û��ı��
				if(TRUE == bCriticalFlag)
				{
					impact.MarkCriticalFlag();
				}
				if(0!=nRefixRate)
				{
					//��Ч���߼�����������
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
			//���Ч��Ͷ���ߵ�ID
			ObjID_t nCasterID = rImp.GetCasterObjID();
			//���ID��Ч
			if(INVALID_ID != nCasterID)
			{		
				//�ڳ����л��Ч��Ͷ���ߵĶ���ʵ��
				pObj = rMe.GetSpecificObjInSameSceneByID(nCasterID);
			}
			//���û�ҵ�����
			if(NULL==pObj)
			{
				//�����Ч������Ҳ�����
				if(TRUE==rImp.IsCreateByPlayer())
				{
					//��ȫ��ID���Ҵ˶���
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
			//����Ч��Ͷ����ID
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

		//���ͬһ����ɫ��ɶ����ͬЧ���Ƿ񻥳�
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
		//����Ч���Ƿ��л����ϵ
		//�Ƚ�������ɫ����Ч�������Ƿ�Ϊͬһ�����飬��������Ч���Ƿ�Ϊͬһ�����ʹ�ü������
		BOOL Impact_IsImpactsABMutexed(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB)
		{
			__ENTER_FUNCTION
			if(Impact_GetMutexID(rImpactA)!=Impact_GetMutexID(rImpactB))
			{
				return FALSE;
			}
			//�������Ч������ͬ��ɫ��������
			if(TRUE==Impact_GetMutexByCasterFlag(rImpactA) && TRUE==Impact_GetMutexByCasterFlag(rImpactB))
			{
				//�ж�����Ч���Ƿ�Ϊͬһ����ɫ����
				if((rImpactA.GetCasterUniqueID()!=rImpactB.GetCasterUniqueID()) || (rImpactA.IsCreateByPlayer()!=rImpactB.IsCreateByPlayer()))
				{
					return FALSE;
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//�ж�Ч��A�Ƿ�����滻Ч��B
		BOOL Impact_CanImpactAReplaceImpactB(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB)
		{
			__ENTER_FUNCTION
			//�������Ч��û�л����ϵ�����滻
			if(FALSE == Impact_IsImpactsABMutexed(rImpactA, rImpactB))
			{
				return FALSE;
			}
			//�������Ч����ͬһ�ּ��ܲ����ģ�������������Ч���ļ���ID����Ч
			if(rImpactA.GetSkillID()==rImpactB.GetSkillID() && INVALID_ID!=rImpactA.GetSkillID() && INVALID_ID!=rImpactB.GetSkillID())
			{
				//���滻Ч���ļ���С�ڱ��滻Ч���ļ��������滻
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
