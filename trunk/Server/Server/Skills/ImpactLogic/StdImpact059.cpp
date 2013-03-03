/********************************************************************
	created:	2007/11/24
	created:	24:11:2007   13:51
	filename: 	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic\StdImpact058.cpp
	file path:	d:\project\projectNew\MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact058
	file ext:	cpp
	author:		Richard
*********************************************************************/

#include "stdafx.h"
#include "StdImpact059.h"
#include "Scene.h"
#include "ScanOperator_SectorSkill.h"
#include "Impact_Core.h"
#include "TimeManager.h"//g_pTimeManager
#include "GCTargetListAndHitFlags.h"
#include "GCCharBUFF.h"
#include "Skill_Core.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{

		StdImpact059_T::StdImpact059_T()
		{
			m_iAccuracy = 0;
		}

		StdImpact059_T::~StdImpact059_T()
		{

		}

		VOID StdImpact059_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION

			__LEAVE_FUNCTION
		}

		BOOL StdImpact059_T::InitFromData( OWN_IMPACT& rImp, ImpactData_T const& rData ) const
		{
			__ENTER_FUNCTION
			rImp.SetMaxLayerCount( rData.GetBallCount() );
			rImp.SetLayerCount( rImp.GetMaxLayerCount() );

			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		VOID StdImpact059_T::OnIntervalOver( OWN_IMPACT& rImp, Obj_Character& rMe ) const
		{
			__ENTER_FUNCTION

			//处理消散
			BOOL bFadeOut = LayerFadeOut( rImp );
			UINT uCurrentBallCount = rImp.GetLayerCount();
			//执行优化，因为m_uCurrentBallCount=0此技能就不能再使用，因此不要再
			//执行下面的搜索操作了，搜索操作是比较昂贵的			
			if( 0 >= uCurrentBallCount )
			{
				////层数已经有变化,给client发送层数改变的消息
				if( bFadeOut )
				{
					SendLayerChangeMsg( rImp, rMe );
				}
				return;
			}
			
			GetAccuracy( rImp.GetSkillID() );

			OBJLIST	rTargets;
			HitFlagsForOBJLIST_T HitFlagList;
			UINT nCount	= 0;
			//搜寻及打击生效的逻辑
			DoHitTarget( rImp, rMe, rTargets, HitFlagList, nCount );
			
			//执行优化,如果没有有效击中任何目标,不必继续处理
			if( 0 >= nCount )
			{
				//层数已经有变化,给client发送层数改变的消息
				if( bFadeOut )
				{
					SendLayerChangeMsg( rImp, rMe );
				}
				return;
			}

			INT iCurrentLayer = rImp.GetLayerCount();

			//根据搜索结果计算当前剩余的小球数量
			if( nCount > uCurrentBallCount )
			{
				nCount = uCurrentBallCount;
				rImp.SetLayerCount( 0 );			
			}
			else
			{
				uCurrentBallCount -= nCount;
				rImp.SetLayerCount( uCurrentBallCount );
			}
			
			//给client发送消息，包括小球ID
			BroadcastTargetListMessage( rImp, rMe, rTargets, HitFlagList );

			ComputeLayerChange( rImp, rMe, nCount, iCurrentLayer );
			//给client发送层数改变的消息
			SendLayerChangeMsg( rImp, rMe );

			__LEAVE_FUNCTION
		}

		VOID StdImpact059_T::ComputeLayerChange( OWN_IMPACT& rImp, Obj_Character& rMe, INT nCount, INT iCurrentLayer ) const
		{
			__ENTER_FUNCTION

			INT iMaxLayerCount = rImp.GetMaxLayerCount();
			//获取子效果的DataIndex
			INT nDataIndex = GetSubImpactDataIndexByIndex(rImp, 0);
			//获取子效果的描述对象
			const ImpactData_T* pSubImpactData = Impact_GetDataBlock( nDataIndex );
			//获得子效果的总持续时间
			Time_t iSubImpactContinuance = pSubImpactData->GetContinuance();
			for( INT i=0; i<nCount; ++i )
			{
				//设置新添加的层的计时
				rImp.SetLayerContinuanceByIdx( iMaxLayerCount-iCurrentLayer+i, g_pTimeManager->CurrentTime()+iSubImpactContinuance );
			}
			__LEAVE_FUNCTION
		}

		BOOL StdImpact059_T::LayerFadeOut( OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION
			//获得当前效果的最大叠加层数
			INT iMaxLayerCount = rImp.GetMaxLayerCount();
			//获得当前效果的当前叠加层数
			INT iCurrentLayer = rImp.GetLayerCount();
			INT iMaxLayer = iMaxLayerCount-rImp.GetLayerCount();
			BOOL ret = FALSE;
			//处理当前所有正在计时的层是否到时,如果到时则恢复
			for( INT i=0; i<iMaxLayer; ++i )
			{
				Time_t iContinuance = rImp.GetLayerContinuanceByIdx( i );
				if( g_pTimeManager->CurrentTime() >= (UINT)iContinuance )
				{
					rImp.SetLayerCount( ++iCurrentLayer );
					ret = TRUE;
					for( INT j=i; j<iMaxLayer; ++j )
					{
						//OWN_IMPACT::SetLayerContinuanceByIdx中已经为此预留了最后一个元素的位置
						//所以j+1是安全的,不需要额外判定是否是最后一个元素,此元素将永为0
						Time_t iTemp = rImp.GetLayerContinuanceByIdx( j+1 );
						rImp.SetLayerContinuanceByIdx( j, iTemp );
					}
					--iMaxLayer;
					--i;
				}
			}
			return ret;
			__LEAVE_FUNCTION
			return FALSE;
		}

		BOOL StdImpact059_T::IsCanSendToUnit( OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* pChar ) const
		{		
			//获得技能使用者身上的效果列表
			__ENTER_FUNCTION
			OWN_IMPACT* pImpact = pChar->Impact_GetFirstImpactOfSpecificSkillID( rImp.GetSkillID() );
			if( NULL == pImpact )
			{
				if( pChar->Impact_IsCanReciveBallImpact( rImp ) )
				return TRUE;
			}
			else if( pImpact->GetCasterObjID()!=rMe.GetID() )
			{
				return TRUE;
			}

			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		BOOL StdImpact059_T::ScanUnitForTarget( OWN_IMPACT& rImp, Obj_Character& rMe, FLOAT fX,FLOAT fZ, OBJLIST& rTargets) const
		{
			__ENTER_FUNCTION
			UINT uCurrentBallCount = rImp.GetLayerCount();
			//初始化搜索需要的基础属性
			SkillInfo_T rSkillInfo = rMe.GetSkillInfo();
			//为了此类buff可以和其它技能并存，所以给ScanOperatorIniter提供造成此类buff的技能SkillInfo_T
			g_SkillCore.InstanceSkill( rSkillInfo, rMe, rImp.GetSkillID() );
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

			//保护
			if( rTargets.MAX_OBJ_LIST_SIZE < uCurrentBallCount )
				return FALSE;

			//搜索人数上限为当前自身实际的小球个数
			ScanOperatorIniter.m_nCount = uCurrentBallCount;

			//初始化搜索对象
			ScanOperator_SectorSkill ScanOperator;
			ScanOperator.Init(&ScanOperatorIniter);
			//执行搜索
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

		BOOL StdImpact059_T::HitThisTarget(Obj_Character& rMe, Obj_Character& rTar, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			//SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Scene* pScene = rMe.getScene();
			if(TRUE==rMe.IsFriend(&rTar))
			{
				//给友方使用的技能100%命中
				return TRUE;
			}
			//Hit Or Miss
			//根据命中率判断是否击中
			//注意：此时对技能释放而言，目标已验证为合法，因此只受技能本身的命中率影响
			//如果没有命中
			if(FALSE == IsHit(rMe, rTar, m_iAccuracy))
			{
				if(NULL!=pScene)
				{
					//直接向全局event对象注册没有命中的事件
					pScene->GetEventCore().RegisterSkillMissEvent(rTar.GetID(), rMe.GetID(), nSkillID, 0);
				}
				return FALSE;
			}
			//如果最终命中，向全局event对象注册技能命中目标事件
			if(NULL!=pScene)
			{
				pScene->GetEventCore().RegisterSkillHitEvent(rTar.GetID(), rMe.GetID(), nSkillID, 0);
			}
			return TRUE;
			__LEAVE_FUNCTION
				return FALSE;
		}

		BOOL StdImpact059_T::IsHit(Obj_Character& rMe, Obj_Character& rTar, INT nAccuracy) const
		{
			__ENTER_FUNCTION
			CombatCore_T myCombatCore;
			if(-1 == nAccuracy)
			{
				nAccuracy = myCombatCore.CalculateHitRate(rMe, rTar);
			}
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

		VOID StdImpact059_T::BroadcastTargetListMessage( OWN_IMPACT& rImp, Obj_Character& rMe, OBJLIST& rTargets, HitFlagsForOBJLIST_T& rHitFlagsForObjList ) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			GCTargetListAndHitFlags msg;
			GCTargetListAndHitFlags::HitFlagList_T& rHitFlags		= msg.GetHitFlagList();
			msg.SetDataType(msg.BUFF_ACTIVETE);
			msg.SetObjID(rMe.GetID());
			msg.SetUserPos(*(rMe.getWorldPos()));
			msg.SetLogicCount(rMe.GetLogicCount());
			msg.SetSkillOrSpecialObjDataID(rImp.GetSkillID());
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
						//msg.SetBallIDByIndex( nIdx, m_aBallUniqueID[nIdx].m_uUniqueID );
					}
				}
			}
			Scene* pScene = rMe.getScene();
			if(NULL!=pScene)
			{
				pScene->BroadCast(&msg, &rMe, TRUE);
			}
			__LEAVE_FUNCTION
		}

		VOID StdImpact059_T::GetAccuracy( SkillID_t nSkillID ) const
		{
			__ENTER_FUNCTION
			Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nSkillID);
			//是否找到相应的模板
			if(NULL==pSkillTemplate)
			{
				return;
			}

			//(INT)m_iAccuracy = pSkillTemplate->GetAccuracy();

			__LEAVE_FUNCTION
		}

		VOID StdImpact059_T::DoHitTarget( OWN_IMPACT& rImp, Obj_Character& rMe, OBJLIST& rTargets, HitFlagsForOBJLIST_T& HitFlagList, UINT& nCount ) const
		{
			__ENTER_FUNCTION

			WORLD_POS const* pPos = rMe.getWorldPos();
			//搜索目标列表
			if(NULL!=pPos)
			{
				ScanUnitForTarget( rImp, rMe, pPos->m_fX, pPos->m_fZ, rTargets );
			}
			SkillID_t nSkillID = rImp.GetSkillID();
			////根据技能ID从全局技能模板管理器中获得技能模板实例
			//const SkillTemplateData_T* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID( iID );
			HitFlagList.ClearAllFlags();
			INT nIdx	= 0;
			for(nIdx=0; rTargets.m_Count>nIdx; ++nIdx)
			{
				Obj* pTarget = rTargets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					Obj_Character* pChar = (Obj_Character*)pTarget;
					//如果命中，则把命中与否的列表置为命中
					if(TRUE==HitThisTarget(rMe, *pChar, nSkillID))
					{
						//判断小球投送对象身上是否已经有同类型的效果存在,如果有
						if( !IsCanSendToUnit( rImp, rMe, pChar ) )
						{
							continue;
						}
						////记录小球投送的对象
						//(Obj_Character*)(m_aBallUniqueID[nIdx].m_pCharacter)	= pChar;
						////记录小球的全局ID
						//(UINT)(m_aBallUniqueID[nIdx].m_uUniqueID)				= g_ImpactCore.GetUniqueID();
						//pChar->Impact_SetImpactUniqueID( m_aBallUniqueID[nIdx].m_uUniqueID );
						INT nDataIndex = GetSubImpactDataIndexByIndex(rImp, 0);
						if(0<nDataIndex)
						{
							g_ImpactCore.SendImpactToUnit( *pChar, nDataIndex, rMe.GetID(), nSkillID );
						}

						//g_ImpactCore.SendBallImpactToUnit( *pChar, STD_IMPACT_058, rMe.GetID() );
						//Mark this Target Hitted
						HitFlagList.MarkFlagByIndex(nIdx); 
						++nCount;
					}
				}
			}
			__LEAVE_FUNCTION

		}

		VOID StdImpact059_T::SendLayerChangeMsg( OWN_IMPACT& rImp, Obj_Character& rMe ) const
		{
			__ENTER_FUNCTION

			//通知客户端层数改变
			// Info Other Impact
			//通知其它客户端效果生效
			Scene * pScene = rMe.getScene();
			if(NULL==pScene||FALSE==rMe.IsActiveObj())
			{
				return;
			}
			GCCharBuff Msg2Other;
			Msg2Other.SetReceiverID(rImp.GetCasterObjID());
			Msg2Other.SetSenderID(rImp.GetCasterObjID());
			Msg2Other.SetEnable(TRUE);
			Msg2Other.SetSN(rImp.GetSN());
			Msg2Other.SetBuffID(Impact_GetImpactID(rImp));
			Msg2Other.SetSenderLogicCount(rImp.GetCasterLogicCount());
			Msg2Other.SetSkillID( rImp.GetSkillID() );
			Msg2Other.SetLayerCount( rImp.GetLayerCount() );

			// 设置技能效果的持续时间  20100531 BLL
			Msg2Other.SetContinuance( rImp.GetContinuance() );

			if(INVALID_ID!=Impact_GetImpactID(rImp))
			{
				pScene->BroadCast(&Msg2Other, &rMe, TRUE);
			}
			__LEAVE_FUNCTION
		}

	};
};

