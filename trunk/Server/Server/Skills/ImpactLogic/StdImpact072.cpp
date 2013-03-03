/********************************************************************
	created:	2008/05/10
	created:	10:5:2008   10:23
	filename: 	MapServer\Server\Skills\ImpactLogic\StdImpact072.cpp
	file path:	MapServer\Server\Skills\ImpactLogic
	file base:	StdImpact072
	file ext:	cpp
	author:		Richard
	
	purpose:	˲�Ƽ���ר��Impact
*********************************************************************/

#include "stdafx.h"
#include "StdImpact072.h"
#include "Obj_Human.h"
#include "Scene.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact072_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			//INT nDamage = rData.GetDescriptorByIndex(ImpactDataDescriptorIndex_T::IDX_DAMAGE)->GetValueBySkillLevel(rImp.GetSkillLevel());
			//SetDamage(rImp, nDamage);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact072_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			//g_pLog->FastSaveLog( LOG_FILE_1, "˲�Ƽ��� StdImpact072_T::OnActive ����" );

			WORLD_POS sFinalPos;
			WORLD_POS sDir;
			WORLD_POS sPosTag;
			WORLD_POS oTargetPos;

			FLOAT fDistance;


			if( !GetTargetPos(rImp, rMe, oTargetPos, fDistance) )
			{
				return;
			}


			GetFinalPos(rImp, rMe, sFinalPos, sDir, sPosTag, oTargetPos, fDistance);

			GetRealPos(rMe, sFinalPos, sPosTag, sDir);


			rMe.DirectTo( &sPosTag, FALSE );
			//g_pLog->FastSaveLog( LOG_FILE_1, "˲�Ƽ�����������" );
			__LEAVE_FUNCTION
		}

		VOID StdImpact072_T::GetFinalPos( OWN_IMPACT& rImp, Obj_Character &rMe, WORLD_POS& sFinalPos, WORLD_POS& sDir, WORLD_POS& sPosTag, WORLD_POS& oTargetPos, FLOAT fDistance ) const
		{
			__ENTER_FUNCTION

			const WORLD_POS* pMyPos		= rMe.getWorldPos();
			sDir.m_fX	= oTargetPos.m_fX - pMyPos->m_fX;
			sDir.m_fZ	= oTargetPos.m_fZ - pMyPos->m_fZ;
			sPosTag = *pMyPos;
			//��ȡ����
			rMe.NormalizeWorldPos( sDir );
			//����ϣ�������Ŀ���
			sFinalPos.m_fX = sPosTag.m_fX + fDistance*sDir.m_fX;
			sFinalPos.m_fZ = sPosTag.m_fZ + fDistance*sDir.m_fZ;
			__LEAVE_FUNCTION

		}

		VOID StdImpact072_T::GetRealPos( Obj_Character &rMe, WORLD_POS &sFinalPos, WORLD_POS &sPosTag, WORLD_POS &sDir ) const
		{
			__ENTER_FUNCTION

			FLOAT fLengthOfStep = 0.5f;
			FLOAT fLengthTmp	= 0;
			WORLD_POS sPosTmp;
			//����ǰ�������Ը��ӱ߳�(0.5)Ϊ������Ŀ�귽���ѯ,���ϰ��򵽴�Ŀ���Ϊֹ
			while( TRUE )
			{
				//�Ƚϵ�ǰλ�ú�Ŀ���ľ���,���С�ڲ�����ֱ�ӵ��ﲢ����
				fLengthTmp = rMe.WordPosLength( sFinalPos.m_fX-sPosTag.m_fX, sFinalPos.m_fZ-sPosTag.m_fZ );
				if( fLengthTmp < fLengthOfStep )
				{
					if ( rMe.getScene()->GetMap()->IsCanGo(sFinalPos) )
						sPosTag = sFinalPos;
					break;
				}
				//������ڲ���������һ�������ľ���
				sPosTmp.m_fX = sPosTag.m_fX + sDir.m_fX*fLengthOfStep;
				sPosTmp.m_fZ = sPosTag.m_fZ + sDir.m_fZ*fLengthOfStep;
				//�ж���λ���Ƿ����ͨ��
				if ( rMe.getScene()->GetMap()->IsCanGo(sPosTmp) )
					sPosTag = sPosTmp;
				//����ͨ�������
				else break;
			}
			__LEAVE_FUNCTION

		}

		BOOL StdImpact072_T::GetTargetPos( OWN_IMPACT& rImp, Obj_Character &rMe, WORLD_POS& oTargetPos, FLOAT& fDistance ) const
		{
			__ENTER_FUNCTION

			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			//���㷽��
			FLOAT fMaxDistance = (FLOAT)GetDistance( rImp );
			ID_t nTargetMode = rMe.GetSkillInfo().GetTargetingLogic();
			
			if( INVALID_ID == nTargetMode )
			{
				//g_pLog->FastSaveLog( LOG_FILE_1, "˲�Ƽ��� error, INVALID_ID == nTargetMode" );

				return FALSE;
			}
			WORLD_POS oMyPos = *(rMe.getWorldPos());
			if( TARGET_POS == nTargetMode )
			{
				oTargetPos	= rParams.GetTargetPosition();
				fDistance = GetFinalDistance(rMe, oMyPos, oTargetPos, fMaxDistance);

			}
			else if( TARGET_SPECIFIC_UNIT == nTargetMode )
			{
				ObjID_t iID = rParams.GetTargetObj();
				
				Obj_Character* pTag = (Obj_Character*)rMe.getScene()->GetObjManager()->GetObj( iID );
				if( NULL == pTag )
				{
					//g_pLog->FastSaveLog( LOG_FILE_1, "˲�Ƽ��� error, NULL == pTag" );

					return FALSE;
				}
				oTargetPos = *(pTag->getWorldPos());

				fDistance = GetFinalDistance(rMe, oMyPos, oTargetPos, fMaxDistance);
				fDistance -= 0.5f;
			}
			else if( TARGET_SELF == nTargetMode )
			{
				fDistance		= fMaxDistance;
				oTargetPos		= *(rMe.getWorldPos());
				FLOAT fDir		= rParams.GetTargetDirection();
				oTargetPos.m_fX	+= sinf( fDir ) * fDistance;
				oTargetPos.m_fZ	+= cosf( fDir ) * fDistance;
			}
			else 
			{
				//g_pLog->FastSaveLog( LOG_FILE_1, "˲�Ƽ��� error, TargetMode invalid" );

				return FALSE;
			}
			
			if( fDistance < 0.005f )
			{
				rMe.DirectTo( &oMyPos, FALSE );
				//g_pLog->FastSaveLog( LOG_FILE_1, "˲�Ƽ��� OK, but too near to target" );
				return FALSE;
			}

			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		FLOAT StdImpact072_T::GetFinalDistance( Obj_Character &rMe, WORLD_POS &oMyPos, WORLD_POS &oTargetPos, FLOAT fMaxDistance ) const
		{
			FLOAT fDistance	= rMe.WordPosLength( oMyPos.m_fX-oTargetPos.m_fX, oMyPos.m_fZ-oTargetPos.m_fZ );
			if( fMaxDistance < fDistance )
			{
				fDistance = fMaxDistance;
			}	
			return fDistance;
		}
	};
};


