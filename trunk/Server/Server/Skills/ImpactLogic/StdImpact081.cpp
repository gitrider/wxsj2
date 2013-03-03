/********************************************************************
	created:	2010/05/13
	author:		�¾�
	purpose:	����һ�������Ĵ���impact(���˼·�ο�65�Ĵ����������impact��65���Ը���)
*********************************************************************/
#include "stdafx.h"
#include "StdImpact081.h"
#include "GameTable.h"
#include "Scene.h"
#include "AI_Character.h"
#include "AI_Human.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact081_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION

			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			
			SendImpactToMe(rImp, rMe);

			__LEAVE_FUNCTION
		}
		BOOL StdImpact081_T::IsHappend( Obj_Character &rMe, OWN_IMPACT& rImp , INT nIndex ) const
		{
			__ENTER_FUNCTION

			Scene * pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return FALSE;
			}

			INT iRate = GetEffectRate( rImp , nIndex);
			INT nRand = pScene->GetRand100();
			if( nRand <= iRate )
			{
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION

				return FALSE;
		}
		VOID StdImpact081_T::SendImpactToMe( OWN_IMPACT& rImp, Obj_Character& rMe ) const
		{
			//��һ�������Ƿ���Ա�����
			BOOL bFirstHappend = IsHappend(rMe, rImp, 0);

			for(INT nID = 0; nID < MAX_IMPACT_NUM_AT_SAME_TIME; nID++)
			{
				INT iImpactID = GetImpactID(rImp, nID);
				if(INVALID_ID == iImpactID)
				{
					break;
				}
				if(0 == nID)
				{
					//�����һ������û�и��ʴ���
					if(!bFirstHappend)
					{
						continue;
					}
				}
				else
				{
					if(INVALID_ID == GetEffectRate( rImp , nID))
					{
						//������impact�ĸ���ֵд-1����ΪӦ��ȡ��һ�������ĸ���ֵ
						if(!bFirstHappend)
						{
							continue;
						}
					}
					else
					{
						//�������������ֵ
						if(!IsHappend(rMe, rImp, nID))
						{
							continue;
						}
					}
				}
				
				Obj_Character* pAttacker = GetBeHitTarget( rMe, rImp );
				if(pAttacker)
				{
					INT iTargetType = GetTargetType( rImp, nID );
					if( iTargetType == TargetTypeMe )
					{
						g_ImpactCore.SendImpactToUnit(rMe, iImpactID, pAttacker->GetID(), 0);
					}
					else
					{
						g_ImpactCore.SendImpactToUnit(*pAttacker, iImpactID, rMe.GetID(), 0);
					}				
				}
			}
		}

		Obj_Character* StdImpact081_T::GetBeHitTarget( Obj_Character& rMe, OWN_IMPACT& rImp ) const
		{
			__ENTER_FUNCTION

			Scene* pScene = rMe.getScene();
			if( pScene == NULL )
			{
				return NULL;
			}
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Obj_Character* pChar = (Obj_Character*)pScene->GetObjManager()->GetObj( rParams.GetTargetObj() );
			if(NULL == pChar)
			{
				pChar = (Obj_Character*)pScene->GetObjManager()->GetObj( rImp.GetCasterObjID());
			}
			return pChar;
			__LEAVE_FUNCTION
			return NULL;
		}	
	};
};




