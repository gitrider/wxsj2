///////////////////////////////////////////////////////////////////////////////
// 文件名：Skill_Core.cpp
// 程序员：高骐
// 功能说明：技能内核模块
//
// 修改记录：
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
			//是否为无效的技能ID
			if(INVALID_ID==nSkillID)
			{
				return TRUE;
			}
			//先发父既能
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

			//给所有的技能升级
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
			//按照前进方向以格子边长(0.5)为步长向目标方向查询,遇障碍或到达目标点为止
			while( TRUE )
			{
				//比较当前位置和目标点的距离,如果小于步长则直接到达并结束
				fLengthTmp = rMe.WordPosLength( sFinalPos.m_fX-sPosCur.m_fX, sFinalPos.m_fZ-sPosCur.m_fZ );
				if( fLengthTmp < fLengthOfStep )
				{
					if ( rMe.getScene()->GetMap()->IsCanGo(sFinalPos) )
						sPosCur = sFinalPos;
					return TRUE;
				}
				//如果大于步长则增加一个步长的距离
				sPosTmp.m_fX = sPosCur.m_fX + sDir.m_fX*fLengthOfStep;
				sPosTmp.m_fZ = sPosCur.m_fZ + sDir.m_fZ*fLengthOfStep;
				//判断新位置是否可以通过
				if ( rMe.getScene()->GetMap()->IsCanGo(sPosTmp) )
				{
					sPosCur = sPosTmp;
				}
				//不可通过则结束
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
			//获得角色身上的技能对象实例
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			//获得角色身上的目标和消耗参数实例
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			//根据技能ID从全局技能模板管理器中获得技能模板实例
			Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nSkillID);
			//是否找到相应的模板
			if(NULL==pSkillTemplate)
			{
				rParams.SetErrCode(OR_INVALID_SKILL);
				return FALSE;
			}
			//是否为无效的技能ID
			if(INVALID_ID==nSkillID)
			{
				rParams.SetErrCode(OR_OK);
				return TRUE;
			}
			//角色是否存活
			if(FALSE==rMe.IsAlive())
			{
				rParams.SetErrCode(OR_DIE);
				return FALSE;
			}
			//是否当前状态不允许使用这个技能
			if(FALSE==rMe.Skill_CanUseThisSkillInThisStatus(nSkillID))
			{
				rParams.SetErrCode(OR_LIMIT_USE_SKILL);
				return FALSE;
			}
			//如果当前对象是人类，并且此技能是玩家技能
			if(Obj::OBJ_TYPE_HUMAN == rMe.GetObjType()&&A_SKILL_FOR_PLAYER==pSkillTemplate->GetClassByUser())
			{
				HumanDB* pDB = ((Obj_Human*)&rMe)->GetDB();

				//角色不具有此技能，并且不能忽略前提条件则返回
				if(FALSE == rMe.Skill_HaveSkill(nSkillID)&&FALSE==rParams.GetIgnoreConditionCheckFlag())
				{	//角色不会这个技能,看看他如果骑着马，这匹马会不会这个技能
					if( !CheckCanUseSkillByHorse(rMe, nSkillID) )
					{
						return FALSE;
					}
				}
				else
				{//角色自身具有此技能
					INT iLevel = GetSkillLevelByEquip( nSkillID, ((Obj_Human*)&rMe) );
					rSkillInfo.SetCharacterSkillLevel( iLevel );
				}
			}
			//如果此技能需要冷却状态则判断是否已冷却，否则忽略冷却影响
			if(FALSE==rMe.Skill_IsSkillCooldowned(nSkillID) && !bSubSkill)
			{
				rParams.SetErrCode(OR_COOL_DOWNING);
				return FALSE;
			}

			/// 角色身上是否还有动作未完成。
			if (FALSE == GetGlobalActionDelegator().CanDoNextAction(rMe))
			{
				// 是瞬发技能
				if (TRUE == pSkillTemplate->IsInterruptMove())
				{
					// 上一个技能可以被瞬发技能打断
					if (TRUE == TRUE/*rSkillInfo.GetStandFlag()*/)
					{
						// 中断上一个技能动作
						GetGlobalActionDelegator().InterruptCurrentAction(rMe);
					}
					// 不能被打断
					else
					{
						rParams.SetErrCode(OR_BUSY);
						return FALSE;
					}
				}
				// 不是瞬发技能。返回
				else
				{
					rParams.SetErrCode(OR_BUSY);
					return FALSE;
				}
			}
			
			////填充伤害延迟时间
			//rParams.SetHurtDelayTime( iHurtDelayTime );
			//在角色身上的目标和消耗对象中更新相关属性值
			//当前的技能ID
			rParams.SetActivatedSkill(nSkillID);
			//当前的目标ID
			rParams.SetTargetObj(nTargetID);
			//当前的目标位置
			rParams.SetTargetPosition(rTargetPos);
			//当前的目标朝向
			rParams.SetTargetDirection(fTargetDir);
			//当前目标所处的格子
			rParams.SetTargetGuid(guidTarget);
			//激活本次使用的技能
			bRet=ActiveSkillNow(rMe, bSubSkill);
			//如果激活失败，设置一个错误标志
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
			//获得角色身上目标和消耗的数据
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			//从全局技能模板管理器中获得技能模板数据
			Skill_Module::SkillTemplateData_T const* pSkillTemplate = GetSkillTemplateByID(nSkill);
			if(NULL == pSkillTemplate)
			{
				AssertEx(FALSE,"[CombatCore_T::InstanceSkill]: Can't find skill date in the SkillTemplateDateMgr!");
				return FALSE;
			}

			rSkillInfoOut = *pSkillTemplate;

			//如果当前对象是人类，并且此技能是玩家技能
			if(Obj::OBJ_TYPE_HUMAN == rMe.GetObjType()&&A_SKILL_FOR_PLAYER==pSkillTemplate->GetClassByUser())
			{
				//如果不是通用技能
				if( pSkillTemplate->GetMenPai() != INVALID_ID )
				{

					INT iSkillLevel = GetSkillLevelByEquip( nSkill, ((Obj_Human*)&rMe) );

					rSkillInfoOut.SetCharacterSkillLevel( iSkillLevel );
				}
				else
				{
					//如果是职业通用技能
					rSkillInfoOut.SetCharacterSkillLevel( 0 );
				}

			}

			//根据技能等级取得技能ID
			SkillID_t	nSkillInstance = nSkill;	//pSkillTemplate->GetSkillInstance(rSkillInfoOut.GetCharacterSkillLevel());
			//根据技能ID取得技能实例对象
			Skill_Module::SkillInstanceData_T const* pSkillInstance = GetSkillInstanceByID(nSkillInstance);
			if(NULL == pSkillInstance)
			{
				// Warning Skill data don't match the template data, plz check the data table.
				Log::SaveLog( SERVER_LOGFILE, "[SkillCore_T::InstanceSkill]: Warning Skill Data don't match SkillTemplate Data TemplateID=%d, CharacterSkillLevel=d%, SkillDataID=d%.", 
					nSkill, rSkillInfoOut.GetCharacterSkillLevel(), nSkillInstance) ;
				AssertEx(FALSE,"[SkillCore_T::InstanceSkill]: Can't find skill instance!");
				return FALSE;
			}
			//取得将技能实例对象中的技能实例ID、冷却时间等信息
			rSkillInfoOut= *pSkillInstance; //transfer data from skilldata to runtime skill instance
			rSkillInfoOut.SetSkillID( nSkill );

			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillCore_T::ActiveSkillNow(Obj_Character& rMe, BOOL bSubSkill) const
		{
			__ENTER_FUNCTION
			//获得角色身上的Skill实例
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			//获得角色身上的目标和消耗临时数据的集合
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			//如果我是非存活状态或处于非激活状态则返回
			if(FALSE==rMe.IsAlive()||FALSE==rMe.IsActiveObj())
			{
				rParams.SetErrCode(OR_DIE);
				return FALSE;
			}
			//Skill实例数据复位
			rSkillInfo.Init();
			//将rSkillInfo信息填充
			if(FALSE==InstanceSkill(rSkillInfo, rMe, rParams.GetActivatedSkill()))
			{
				rParams.SetErrCode(OR_INVALID_SKILL);
				rParams.SetErrParam(rParams.GetActivatedSkill());
				return FALSE;
			}
			//refix skill instance
			//根据技能逻辑和角色身上已存在的效果逻辑修正此技能数据
			rMe.RefixSkill(rSkillInfo);
			//perform checking
			INT nLogicID = rSkillInfo.GetLogicID();
			//获得逻辑对象
			SkillLogic_T const* pLogic = g_SkillLogicList.GetLogicById(nLogicID);
			if(NULL==pLogic)
			{
				rParams.SetErrCode(OR_INVALID_SKILL);
				rParams.SetErrParam(rParams.GetActivatedSkill());
				return FALSE;
			}
			//如果是被动技能则返回
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
			//判断技能因为冷却问题而是否可以使用，有可能的情况是当前技能需要冷却状态，
			//但目前冷却还没有完成则此技能不能使用
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

			//是否为无条件
			if(FALSE==rParams.GetIgnoreConditionCheckFlag())
			{
				//如果为有条件判断条件是否满足
				if(FALSE == pLogic->IsConditionSatisfied(rMe))
				{
					return FALSE;
				}
				//还有没有其它特殊条件需要满足
				if(FALSE==pLogic->SpecificOperationOnSkillStart(rMe))
				{
					return FALSE;
				}
			}

			//根据技能类型决定最终的操作
			switch (rSkillInfo.GetSkillType())
			{
				//直接生效技能
				case SKILL_INSTANT_LAUNCHING:
					pLogic->StartLaunching(rMe);
					break;
				//开始蓄气，经过蓄气规定的时间以后可以发招
				case SKILL_NEED_CHARGING:
					pLogic->StartCharging(rMe);
					break;
				//开始引导，在引导的有效时间里可以连续发招
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

		// 角色身上的武器容器为主、副手，因此检察双手是否为空即可
		BOOL SkillCore_T::IsCanPassCheckByWeapon( SkillInfo_T &rSkillInfo, Obj_Character &rMe, TargetingAndDepletingParams_T& rParams ) const
		{
			//当前技能是否需要使用装备
			if(TRUE == rSkillInfo.GetMustUseWeaponFlag())
			{
				if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
				{
					Obj_Human* pHuman = (Obj_Human*)&rMe;
					//目前只判断是否持有武器，也就是说只区分了是赤手空拳还是持有武器
					//具体的武器类型则无关
					//取得主手容器
					Item* pMainhand=HumanItemLogic::GetEquip(pHuman, HEQUIP_MAINHAND);
					//取得副手容器
					Item* pAssihand=HumanItemLogic::GetEquip(pHuman, HEQUIP_ASSIHAND);
					if(NULL==pMainhand ||  NULL==pAssihand)
					{
						rParams.SetErrCode(OR_NEED_A_WEAPON);
						return FALSE;
					}
					//如果两手都空，证明没有武器，任何手不为空为持有武器
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
					//马给人添加的技能等级永远为1
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
