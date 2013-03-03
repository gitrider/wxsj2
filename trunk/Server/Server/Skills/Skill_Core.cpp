///////////////////////////////////////////////////////////////////////////////
// �ļ�����Skill_Core.cpp
// ����Ա������
// ����˵���������ں�ģ��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file Skill_Core.cpp
*		\brief Skill Process module
*/
#include "stdafx.h"
#include "Type.h"
#include "Skill_Core.h"
#include "Scene.h"
#include "LogicManager.h"
#include "Log.h"
#include "OResultDef.h"
#include "HumanItemLogic.h"
#include "GameTable.h"
#include "ActionDelegator.h"
#include "Obj_Human.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Packets;
		using namespace Action_Module;
		//class SkillCore_T
		BOOL SkillCore_T::ProcessSkillRequest(Obj_Character& rMe, SkillID_t nSkillID, ObjID_t nTargetID, WORLD_POS const& rTargetPos, FLOAT fTargetDir, GUID_t guidTarget, INT iHurtDelayTime, BOOL bSubSkill) const
		{
			__ENTER_FUNCTION 
			//�Ƿ�Ϊ��Ч�ļ���ID
			if(INVALID_ID==nSkillID)
			{
				return TRUE;
			}
			//�ȷ�������
			return ProcessSkillRequestImp( rMe, nSkillID, nTargetID, rTargetPos, fTargetDir, guidTarget, iHurtDelayTime, bSubSkill );
			
			__LEAVE_FUNCTION
			return FALSE;
		}

		INT GetSkillLevelByEquip( INT iSkillID, Obj_Human* pMe )
		{
			__ENTER_FUNCTION 

			const _OWN_SKILL* pOwnSkill = pMe->Skill_GetSkill( iSkillID );
			if( NULL == pOwnSkill)
			{
				return 0;
			}
			INT iLevel = pOwnSkill->m_nLevel-1;

			//�����еļ�������
			_ITEM_EFFECT* pIE = pMe->ItemEffect(IATTRIBUTE_ALLSKILL_LEVEL_INC);
			Assert(pIE);
			if(pIE->IsActive())
			{
				iLevel++;
			}
			else
			{
				_ITEM_EFFECT_SKILL* pEffectSkill =  pMe->GetEquipEffectSkill(iSkillID);
				if (pEffectSkill != NULL)
				{
					INT iTmp = pEffectSkill->m_nLevel;
					if( iTmp < 0 )
					{
						iTmp = 0;
					}
					iLevel += iTmp;
				}
			}

			if( iLevel > 5 )
			{
				iLevel = 5;
			}
			return iLevel;
			__LEAVE_FUNCTION
			return 0;
		}

		BOOL IsCanGo( Obj_Character &rMe, WORLD_POS sFinalPos, WORLD_POS sPosCur, WORLD_POS sDir )
		{
			__ENTER_FUNCTION
			
			rMe.NormalizeWorldPos( sDir );

			FLOAT fLengthOfStep = 0.5f;
			FLOAT fLengthTmp	= 0;
			WORLD_POS sPosTmp;
			//����ǰ�������Ը��ӱ߳�(0.5)Ϊ������Ŀ�귽���ѯ,���ϰ��򵽴�Ŀ���Ϊֹ
			while( TRUE )
			{
				//�Ƚϵ�ǰλ�ú�Ŀ���ľ���,���С�ڲ�����ֱ�ӵ��ﲢ����
				fLengthTmp = rMe.WordPosLength( sFinalPos.m_fX-sPosCur.m_fX, sFinalPos.m_fZ-sPosCur.m_fZ );
				if( fLengthTmp < fLengthOfStep )
				{
					if ( rMe.getScene()->GetMap()->IsCanGo(sFinalPos) )
						sPosCur = sFinalPos;
					return TRUE;
				}
				//������ڲ���������һ�������ľ���
				sPosTmp.m_fX = sPosCur.m_fX + sDir.m_fX*fLengthOfStep;
				sPosTmp.m_fZ = sPosCur.m_fZ + sDir.m_fZ*fLengthOfStep;
				//�ж���λ���Ƿ����ͨ��
				if ( rMe.getScene()->GetMap()->IsCanGo(sPosTmp) )
				{
					sPosCur = sPosTmp;
				}
				//����ͨ�������
				else
				{
					return FALSE;
				}
			}
			__LEAVE_FUNCTION
			return FALSE;
		}

		BOOL SkillCore_T::ProcessSkillRequestImp( Obj_Character& rMe, SkillID_t nSkillID, ObjID_t nTargetID, WORLD_POS const& rTargetPos, FLOAT fTargetDir, GUID_t guidTarget, INT iHurtDelayTime, BOOL bSubSkill ) const
		{
			__ENTER_FUNCTION
			BOOL bRet = TRUE;
			//��ý�ɫ���ϵļ��ܶ���ʵ��
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			//��ý�ɫ���ϵ�Ŀ������Ĳ���ʵ��
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			//���ݼ���ID��ȫ�ּ���ģ��������л�ü���ģ��ʵ��
			Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nSkillID);
			//�Ƿ��ҵ���Ӧ��ģ��
			if(NULL==pSkillTemplate)
			{
				rParams.SetErrCode(OR_INVALID_SKILL);
				return FALSE;
			}
			//�Ƿ�Ϊ��Ч�ļ���ID
			if(INVALID_ID==nSkillID)
			{
				rParams.SetErrCode(OR_OK);
				return TRUE;
			}
			//��ɫ�Ƿ���
			if(FALSE==rMe.IsAlive())
			{
				rParams.SetErrCode(OR_DIE);
				return FALSE;
			}
			//�Ƿ�ǰ״̬������ʹ���������
			if(FALSE==rMe.Skill_CanUseThisSkillInThisStatus(nSkillID))
			{
				rParams.SetErrCode(OR_LIMIT_USE_SKILL);
				return FALSE;
			}
			//�����ǰ���������࣬���Ҵ˼�������Ҽ���
			if(Obj::OBJ_TYPE_HUMAN == rMe.GetObjType()&&A_SKILL_FOR_PLAYER==pSkillTemplate->GetClassByUser())
			{
				HumanDB* pDB = ((Obj_Human*)&rMe)->GetDB();

				//��ɫ�����д˼��ܣ����Ҳ��ܺ���ǰ�������򷵻�
				if(FALSE == rMe.Skill_HaveSkill(nSkillID)&&FALSE==rParams.GetIgnoreConditionCheckFlag())
				{	//��ɫ�����������,�����������������ƥ��᲻���������
					if( !CheckCanUseSkillByHorse(rMe, nSkillID) )
					{
						return FALSE;
					}
				}
				else
				{//��ɫ������д˼���
					INT iLevel = GetSkillLevelByEquip( nSkillID, ((Obj_Human*)&rMe) );
					rSkillInfo.SetCharacterSkillLevel( iLevel );
				}
			}
			//����˼�����Ҫ��ȴ״̬���ж��Ƿ�����ȴ�����������ȴӰ��
			if(FALSE==rMe.Skill_IsSkillCooldowned(nSkillID) && !bSubSkill)
			{
				rParams.SetErrCode(OR_COOL_DOWNING);
				return FALSE;
			}

			/// ��ɫ�����Ƿ��ж���δ��ɡ�
			if (FALSE == GetGlobalActionDelegator().CanDoNextAction(rMe))
			{
				// ��˲������
				if (TRUE == pSkillTemplate->IsInterruptMove())
				{
					// ��һ�����ܿ��Ա�˲�����ܴ��
					if (TRUE == TRUE/*rSkillInfo.GetStandFlag()*/)
					{
						// �ж���һ�����ܶ���
						GetGlobalActionDelegator().InterruptCurrentAction(rMe);
					}
					// ���ܱ����
					else
					{
						rParams.SetErrCode(OR_BUSY);
						return FALSE;
					}
				}
				// ����˲�����ܡ�����
				else
				{
					rParams.SetErrCode(OR_BUSY);
					return FALSE;
				}
			}
			
			////����˺��ӳ�ʱ��
			//rParams.SetHurtDelayTime( iHurtDelayTime );
			//�ڽ�ɫ���ϵ�Ŀ������Ķ����и����������ֵ
			//��ǰ�ļ���ID
			rParams.SetActivatedSkill(nSkillID);
			//��ǰ��Ŀ��ID
			rParams.SetTargetObj(nTargetID);
			//��ǰ��Ŀ��λ��
			rParams.SetTargetPosition(rTargetPos);
			//��ǰ��Ŀ�곯��
			rParams.SetTargetDirection(fTargetDir);
			//��ǰĿ�������ĸ���
			rParams.SetTargetGuid(guidTarget);
			//�����ʹ�õļ���
			bRet=ActiveSkillNow(rMe, bSubSkill);
			//�������ʧ�ܣ�����һ�������־
			if(FALSE==bRet)
			{
				g_SkillCore.OnException(rMe);
			}
			return bRet;
			__LEAVE_FUNCTION
				return FALSE;
		}

		BOOL SkillCore_T::InstanceSkill(SkillInfo_T& rSkillInfoOut,Obj_Character& rMe, SkillID_t nSkill) const
		{
			__ENTER_FUNCTION
			//��ý�ɫ����Ŀ������ĵ�����
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			//��ȫ�ּ���ģ��������л�ü���ģ������
			Skill_Module::SkillTemplateData_T const* pSkillTemplate = GetSkillTemplateByID(nSkill);
			if(NULL == pSkillTemplate)
			{
				AssertEx(FALSE,"[CombatCore_T::InstanceSkill]: Can't find skill date in the SkillTemplateDateMgr!");
				return FALSE;
			}

			rSkillInfoOut = *pSkillTemplate;

			//�����ǰ���������࣬���Ҵ˼�������Ҽ���
			if(Obj::OBJ_TYPE_HUMAN == rMe.GetObjType()&&A_SKILL_FOR_PLAYER==pSkillTemplate->GetClassByUser())
			{
				//�������ͨ�ü���
				if( pSkillTemplate->GetMenPai() != INVALID_ID )
				{

					INT iSkillLevel = GetSkillLevelByEquip( nSkill, ((Obj_Human*)&rMe) );

					rSkillInfoOut.SetCharacterSkillLevel( iSkillLevel );
				}
				else
				{
					//�����ְҵͨ�ü���
					rSkillInfoOut.SetCharacterSkillLevel( 0 );
				}

			}

			//���ݼ��ܵȼ�ȡ�ü���ID
			SkillID_t	nSkillInstance = nSkill;	//pSkillTemplate->GetSkillInstance(rSkillInfoOut.GetCharacterSkillLevel());
			//���ݼ���IDȡ�ü���ʵ������
			Skill_Module::SkillInstanceData_T const* pSkillInstance = GetSkillInstanceByID(nSkillInstance);
			if(NULL == pSkillInstance)
			{
				// Warning Skill data don't match the template data, plz check the data table.
				Log::SaveLog( SERVER_LOGFILE, "[SkillCore_T::InstanceSkill]: Warning Skill Data don't match SkillTemplate Data TemplateID=%d, CharacterSkillLevel=d%, SkillDataID=d%.", 
					nSkill, rSkillInfoOut.GetCharacterSkillLevel(), nSkillInstance) ;
				AssertEx(FALSE,"[SkillCore_T::InstanceSkill]: Can't find skill instance!");
				return FALSE;
			}
			//ȡ�ý�����ʵ�������еļ���ʵ��ID����ȴʱ�����Ϣ
			rSkillInfoOut= *pSkillInstance; //transfer data from skilldata to runtime skill instance
			rSkillInfoOut.SetSkillID( nSkill );

			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillCore_T::ActiveSkillNow(Obj_Character& rMe, BOOL bSubSkill) const
		{
			__ENTER_FUNCTION
			//��ý�ɫ���ϵ�Skillʵ��
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			//��ý�ɫ���ϵ�Ŀ���������ʱ���ݵļ���
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			//������ǷǴ��״̬���ڷǼ���״̬�򷵻�
			if(FALSE==rMe.IsAlive()||FALSE==rMe.IsActiveObj())
			{
				rParams.SetErrCode(OR_DIE);
				return FALSE;
			}
			//Skillʵ�����ݸ�λ
			rSkillInfo.Init();
			//��rSkillInfo��Ϣ���
			if(FALSE==InstanceSkill(rSkillInfo, rMe, rParams.GetActivatedSkill()))
			{
				rParams.SetErrCode(OR_INVALID_SKILL);
				rParams.SetErrParam(rParams.GetActivatedSkill());
				return FALSE;
			}
			//refix skill instance
			//���ݼ����߼��ͽ�ɫ�����Ѵ��ڵ�Ч���߼������˼�������
			rMe.RefixSkill(rSkillInfo);
			//perform checking
			INT nLogicID = rSkillInfo.GetLogicID();
			//����߼�����
			SkillLogic_T const* pLogic = g_SkillLogicList.GetLogicById(nLogicID);
			if(NULL==pLogic)
			{
				rParams.SetErrCode(OR_INVALID_SKILL);
				rParams.SetErrParam(rParams.GetActivatedSkill());
				return FALSE;
			}
			//����Ǳ��������򷵻�
			if(TRUE == pLogic->IsPassive())
			{
				AssertEx(FALSE,"[SkillCore_T::HeartBeat]: Found passive Skill!!!Data error!!");
				rParams.SetErrCode(OR_ERROR);
				return FALSE;
			}
			if(TRUE == pLogic->CancelSkillEffect(rMe))
			{
				rParams.SetErrCode(OR_OK);
				return TRUE;
			}
			//Prefix end
			//�жϼ�����Ϊ��ȴ������Ƿ����ʹ�ã��п��ܵ�����ǵ�ǰ������Ҫ��ȴ״̬��
			//��Ŀǰ��ȴ��û�������˼��ܲ���ʹ��
			if(FALSE==rMe.Skill_IsSkillCooldowned(rParams.GetActivatedSkill())&& 
			   FALSE==rParams.GetIgnoreConditionCheckFlag() &&
			   !bSubSkill )
			{
				rParams.SetErrCode(OR_COOL_DOWNING);
				return FALSE;
			}
			if( !IsCanPassCheckByWeapon(rSkillInfo, rMe, rParams) )
			{
				return FALSE;
			}

			//�Ƿ�Ϊ������
			if(FALSE==rParams.GetIgnoreConditionCheckFlag())
			{
				//���Ϊ�������ж������Ƿ�����
				if(FALSE == pLogic->IsConditionSatisfied(rMe))
				{
					return FALSE;
				}
				//����û����������������Ҫ����
				if(FALSE==pLogic->SpecificOperationOnSkillStart(rMe))
				{
					return FALSE;
				}
			}

			//���ݼ������;������յĲ���
			switch (rSkillInfo.GetSkillType())
			{
				//ֱ����Ч����
				case SKILL_INSTANT_LAUNCHING:
					pLogic->StartLaunching(rMe);
					break;
				//��ʼ���������������涨��ʱ���Ժ���Է���
				case SKILL_NEED_CHARGING:
					pLogic->StartCharging(rMe);
					break;
				//��ʼ����������������Чʱ���������������
				case SKILL_NEED_CHANNELING:
					pLogic->StartChanneling(rMe);
					break;
				default:
					AssertEx(FALSE,"[SkillCore_T::ActiveSkillNow]: Try to active a un-active skill!");
					break;
			}				
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE ;
		}
		VOID SkillCore_T::OnException(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_HUMAN == rMe.GetObjType())
			{
				SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
				TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
				if (OR_FAILED(rParams.GetErrCode()))
				{
					Obj_Human& rHuman = static_cast<Obj_Human&>(rMe);
					rHuman.SendOperateResultMsg(rParams.GetErrCode());
				}
			}
			__LEAVE_FUNCTION
		}

		// ��ɫ���ϵ���������Ϊ�������֣���˼��˫���Ƿ�Ϊ�ռ���
		BOOL SkillCore_T::IsCanPassCheckByWeapon( SkillInfo_T &rSkillInfo, Obj_Character &rMe, TargetingAndDepletingParams_T& rParams ) const
		{
			//��ǰ�����Ƿ���Ҫʹ��װ��
			if(TRUE == rSkillInfo.GetMustUseWeaponFlag())
			{
				if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
				{
					Obj_Human* pHuman = (Obj_Human*)&rMe;
					//Ŀǰֻ�ж��Ƿ����������Ҳ����˵ֻ�������ǳ��ֿ�ȭ���ǳ�������
					//����������������޹�
					//ȡ����������
					Item* pMainhand=HumanItemLogic::GetEquip(pHuman, HEQUIP_MAINHAND);
					//ȡ�ø�������
					Item* pAssihand=HumanItemLogic::GetEquip(pHuman, HEQUIP_ASSIHAND);
					if(NULL==pMainhand ||  NULL==pAssihand)
					{
						rParams.SetErrCode(OR_NEED_A_WEAPON);
						return FALSE;
					}
					//������ֶ��գ�֤��û���������κ��ֲ�Ϊ��Ϊ��������
					else if(TRUE==pMainhand->IsEmpty() && TRUE==pAssihand->IsEmpty())
					{
						rParams.SetErrCode(OR_NEED_A_WEAPON);
						return FALSE;						
					}
				}
			}
			return TRUE;
		}

		BOOL SkillCore_T::CheckCanUseSkillByHorse( Obj_Character& rMe, SkillID_t nSkillID ) const
		{
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();

			if( ((Obj_Human*)&rMe)->GetGUIDOfCallUpHorse().IsNull() )
			{
				rParams.SetErrCode(OR_INVALID_SKILL);
				return FALSE;
			}
			Item* pItem = ((Obj_Human*)&rMe)->GetHorseItem( ((Obj_Human*)&rMe)->GetGUIDOfCallUpHorse() );
			BOOL bCanUseSkill = FALSE;
			for( INT i=0; i<PET_MAX_SKILL_COUNT; ++i )
			{
				_PET_SKILL petSkill = pItem->GetSkill( i );
				if( petSkill.m_bCanUse && petSkill.m_nSkillID==nSkillID )
				{
					//�������ӵļ��ܵȼ���ԶΪ1
					rSkillInfo.SetCharacterSkillLevel( 0 );
					bCanUseSkill = TRUE;
					break;
				}
			}
			if( !bCanUseSkill )
			{
				rParams.SetErrCode(OR_INVALID_SKILL);
				return FALSE;
			}
			return TRUE;
		}

		BOOL Skill_IsSkillInCollection(SkillInfo_T const& rSkillInfo, INT nCollection)
		{
			__ENTER_FUNCTION
			INT nID=INVALID_ID;
			IDCollection_T const* pCollection = NULL;
			pCollection = g_IDCollectionMgr.GetInstanceByID(nCollection);
			if(NULL!=pCollection)
			{
				switch (pCollection->GetType())
				{
					case IDCollection_T::TYPE_SKILL_ID:
						nID = rSkillInfo.GetSkillID();
						break;
					case IDCollection_T::TYPE_BUFF_ID:
					case IDCollection_T::TYPE_IMPACT_LOGIC_ID:
					case IDCollection_T::TYPE_IMPACT_MUTEX_ID:
					case IDCollection_T::TYPE_DIRECT_IMPACT_ID:
					case IDCollection_T::TYPE_SKILL_LOGIC_ID:
					default:
						nID = INVALID_ID;
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

		INT Skill_GetSkillInfoDescriptorValueByIndex(SkillInfo_T const& rSkillInfo, INT nIndex)
		{
			__ENTER_FUNCTION
			Descriptor_T const* pDescriptor = rSkillInfo.GetDescriptorByIndex(nIndex);
			if(NULL!=pDescriptor)
			{
				return pDescriptor->GetValueBySkillLevel(rSkillInfo.GetCharacterSkillLevel());
			}
			return 0;
			__LEAVE_FUNCTION
			return 0;
		}
		ConDepTerm_T const* Skill_GetConDepTermByIndex(SkillInfo_T const& rSkillInfo, INT nIndex)
		{
			__ENTER_FUNCTION
			return rSkillInfo.GetConDepTermByIndex(nIndex);
			__LEAVE_FUNCTION
			return NULL;
		}

		INT Skill_GetSkillHurtDelayTime( INT iSkillID )
		{
			__ENTER_FUNCTION

			INT HurtDelayTime = 500;
			const SkillTemplateData_T* pSkillTemplate =  g_SkillTemplateDataMgr.GetInstanceByID( iSkillID );
			if( NULL != pSkillTemplate )
			{
				HurtDelayTime = pSkillTemplate->GetHurtDelayTime();
			}
			return HurtDelayTime;
			__LEAVE_FUNCTION
			return 500;
		}
		INT Skill_GetSkillStandFlag( SkillID_t nSkill )
		{
			__ENTER_FUNCTION

			INT nBehaviorType = BEHAVIOR_TYPE_NEUTRALITY;
			SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nSkill);
			if(NULL!=pSkillTemplate)
			{
				if(0<pSkillTemplate->GetStandFlag())
				{
					nBehaviorType = BEHAVIOR_TYPE_AMITY;
				}
				else if (0>pSkillTemplate->GetStandFlag())
				{
					nBehaviorType = BEHAVIOR_TYPE_HOSTILITY;
				}
				else
				{
					nBehaviorType = BEHAVIOR_TYPE_NEUTRALITY;
				}
			}
			return nBehaviorType;
			__LEAVE_FUNCTION
			return BEHAVIOR_TYPE_NEUTRALITY;
		}
	}
}
Combat_Module::Skill_Module::SkillCore_T	g_SkillCore;
