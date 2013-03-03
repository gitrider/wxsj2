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

			//������ɢ
			BOOL bFadeOut = LayerFadeOut( rImp );
			UINT uCurrentBallCount = rImp.GetLayerCount();
			//ִ���Ż�����Ϊm_uCurrentBallCount=0�˼��ܾͲ�����ʹ�ã���˲�Ҫ��
			//ִ����������������ˣ����������ǱȽϰ����			
			if( 0 >= uCurrentBallCount )
			{
				////�����Ѿ��б仯,��client���Ͳ����ı����Ϣ
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
			//��Ѱ�������Ч���߼�
			DoHitTarget( rImp, rMe, rTargets, HitFlagList, nCount );
			
			//ִ���Ż�,���û����Ч�����κ�Ŀ��,���ؼ�������
			if( 0 >= nCount )
			{
				//�����Ѿ��б仯,��client���Ͳ����ı����Ϣ
				if( bFadeOut )
				{
					SendLayerChangeMsg( rImp, rMe );
				}
				return;
			}

			INT iCurrentLayer = rImp.GetLayerCount();

			//��������������㵱ǰʣ���С������
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
			
			//��client������Ϣ������С��ID
			BroadcastTargetListMessage( rImp, rMe, rTargets, HitFlagList );

			ComputeLayerChange( rImp, rMe, nCount, iCurrentLayer );
			//��client���Ͳ����ı����Ϣ
			SendLayerChangeMsg( rImp, rMe );

			__LEAVE_FUNCTION
		}

		VOID StdImpact059_T::ComputeLayerChange( OWN_IMPACT& rImp, Obj_Character& rMe, INT nCount, INT iCurrentLayer ) const
		{
			__ENTER_FUNCTION

			INT iMaxLayerCount = rImp.GetMaxLayerCount();
			//��ȡ��Ч����DataIndex
			INT nDataIndex = GetSubImpactDataIndexByIndex(rImp, 0);
			//��ȡ��Ч������������
			const ImpactData_T* pSubImpactData = Impact_GetDataBlock( nDataIndex );
			//�����Ч�����ܳ���ʱ��
			Time_t iSubImpactContinuance = pSubImpactData->GetContinuance();
			for( INT i=0; i<nCount; ++i )
			{
				//��������ӵĲ�ļ�ʱ
				rImp.SetLayerContinuanceByIdx( iMaxLayerCount-iCurrentLayer+i, g_pTimeManager->CurrentTime()+iSubImpactContinuance );
			}
			__LEAVE_FUNCTION
		}

		BOOL StdImpact059_T::LayerFadeOut( OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION
			//��õ�ǰЧ���������Ӳ���
			INT iMaxLayerCount = rImp.GetMaxLayerCount();
			//��õ�ǰЧ���ĵ�ǰ���Ӳ���
			INT iCurrentLayer = rImp.GetLayerCount();
			INT iMaxLayer = iMaxLayerCount-rImp.GetLayerCount();
			BOOL ret = FALSE;
			//����ǰ�������ڼ�ʱ�Ĳ��Ƿ�ʱ,�����ʱ��ָ�
			for( INT i=0; i<iMaxLayer; ++i )
			{
				Time_t iContinuance = rImp.GetLayerContinuanceByIdx( i );
				if( g_pTimeManager->CurrentTime() >= (UINT)iContinuance )
				{
					rImp.SetLayerCount( ++iCurrentLayer );
					ret = TRUE;
					for( INT j=i; j<iMaxLayer; ++j )
					{
						//OWN_IMPACT::SetLayerContinuanceByIdx���Ѿ�Ϊ��Ԥ�������һ��Ԫ�ص�λ��
						//����j+1�ǰ�ȫ��,����Ҫ�����ж��Ƿ������һ��Ԫ��,��Ԫ�ؽ���Ϊ0
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
			//��ü���ʹ�������ϵ�Ч���б�
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
			//��ʼ��������Ҫ�Ļ�������
			SkillInfo_T rSkillInfo = rMe.GetSkillInfo();
			//Ϊ�˴���buff���Ժ��������ܲ��棬���Ը�ScanOperatorIniter�ṩ��ɴ���buff�ļ���SkillInfo_T
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

			//����
			if( rTargets.MAX_OBJ_LIST_SIZE < uCurrentBallCount )
				return FALSE;

			//������������Ϊ��ǰ����ʵ�ʵ�С�����
			ScanOperatorIniter.m_nCount = uCurrentBallCount;

			//��ʼ����������
			ScanOperator_SectorSkill ScanOperator;
			ScanOperator.Init(&ScanOperatorIniter);
			//ִ������
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
				//���ѷ�ʹ�õļ���100%����
				return TRUE;
			}
			//Hit Or Miss
			//�����������ж��Ƿ����
			//ע�⣺��ʱ�Լ����ͷŶ��ԣ�Ŀ������֤Ϊ�Ϸ������ֻ�ܼ��ܱ����������Ӱ��
			//���û������
			if(FALSE == IsHit(rMe, rTar, m_iAccuracy))
			{
				if(NULL!=pScene)
				{
					//ֱ����ȫ��event����ע��û�����е��¼�
					pScene->GetEventCore().RegisterSkillMissEvent(rTar.GetID(), rMe.GetID(), nSkillID, 0);
				}
				return FALSE;
			}
			//����������У���ȫ��event����ע�Ἴ������Ŀ���¼�
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
			//�Ƿ��ҵ���Ӧ��ģ��
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
			//����Ŀ���б�
			if(NULL!=pPos)
			{
				ScanUnitForTarget( rImp, rMe, pPos->m_fX, pPos->m_fZ, rTargets );
			}
			SkillID_t nSkillID = rImp.GetSkillID();
			////���ݼ���ID��ȫ�ּ���ģ��������л�ü���ģ��ʵ��
			//const SkillTemplateData_T* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID( iID );
			HitFlagList.ClearAllFlags();
			INT nIdx	= 0;
			for(nIdx=0; rTargets.m_Count>nIdx; ++nIdx)
			{
				Obj* pTarget = rTargets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					Obj_Character* pChar = (Obj_Character*)pTarget;
					//������У�������������б���Ϊ����
					if(TRUE==HitThisTarget(rMe, *pChar, nSkillID))
					{
						//�ж�С��Ͷ�Ͷ��������Ƿ��Ѿ���ͬ���͵�Ч������,�����
						if( !IsCanSendToUnit( rImp, rMe, pChar ) )
						{
							continue;
						}
						////��¼С��Ͷ�͵Ķ���
						//(Obj_Character*)(m_aBallUniqueID[nIdx].m_pCharacter)	= pChar;
						////��¼С���ȫ��ID
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

			//֪ͨ�ͻ��˲����ı�
			// Info Other Impact
			//֪ͨ�����ͻ���Ч����Ч
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

			// ���ü���Ч���ĳ���ʱ��  20100531 BLL
			Msg2Other.SetContinuance( rImp.GetContinuance() );

			if(INVALID_ID!=Impact_GetImpactID(rImp))
			{
				pScene->BroadCast(&Msg2Other, &rMe, TRUE);
			}
			__LEAVE_FUNCTION
		}

	};
};

