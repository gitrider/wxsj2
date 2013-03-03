///////////////////////////////////////////////////////////////////////////////
// �ļ�����Obj_Character_Impact.cpp
// ����Ա������
// ����˵������ɫ���ϵ�Ч������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Obj_Character.h"
#include "ImpactLogic.h"
#include "LogicManager.h"
#include "Scene.h"
#include "ObjManager.h"
#include "SkillInfo.h"
#include "Obj_Human.h"
#include "Player.h"
#include "GCCharBUFF.h"
#include "GCCharDirectImpact.h"
#include "GCDetailBUFF.h"
#include "GameTable.h"
#include "Impact_Core.h"
#include "GCCharSkill_Missed.h"
#include "TimeManager.h"
#include  "GCUseBallImpact.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
using namespace Combat_Module::Impact_Module;
using namespace Packets;

VOID 	Obj_Character::Impact_InitList(VOID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	rList.CleanUp();
	__LEAVE_FUNCTION
}
INT		Obj_Character::Impact_GetListSize(VOID) const
{
	return ((Obj_Character*)this)->Impact_GetImpactList().m_Count;
}
//�����ɫ���ϵ�Ч���б�
VOID 	Obj_Character::Impact_HeartBeat(VOID)
{
	__ENTER_FUNCTION
	if(0 == Impact_GetListSize())
	{
		return;
	}
	Impact_CleanupAllFadeOutImpacts();
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	Time_t nDeltaTime =  GetLogicTime();
	if(0<rCount)
	{
		for(INT i=(rCount-1);0 <= i;--i)
		{
			OWN_IMPACT& rImp= rList.m_aImpacts[i];
			ImpactLogic_T const* pLogic = Impact_GetLogic(rImp);
			if(NULL==pLogic)
			{
				Impact_OnImpactFadeOut(rImp);
				continue;
			}
			if(TRUE==IsUnbreakable() && BEHAVIOR_TYPE_HOSTILITY==Impact_GetStandFlag(rImp))
			{
				Impact_OnImpactFadeOut(rImp);
				continue;
			}
			pLogic->HeartBeat(rImp, *this);
		}
	}
	__LEAVE_FUNCTION
}

//���Ч�����ӳ�ʱ��������Ч���أ���ʱ�䵽�ٴ���
//����ӳ�ʱ��Ϊ0����������ʹЧ����Ч
VOID Obj_Character::Impact_PushToDelayTimeList( OWN_IMPACT& oImpact )
{
	__ENTER_FUNCTION
	ID_t iSkillID = oImpact.GetSkillID();
	Obj* pObj = Impact_GetCaster( *this, oImpact );
	Time_t iTime = 0;
	if( NULL != pObj )
	{	//�ȴ���ͻ��˴�����ӳ�ʱ��
		if( IsCharacterObj( pObj->GetObjType() ) )
		{
			TargetingAndDepletingParams_T& rParams = ((Obj_Character*)pObj)->GetTargetingAndDepletingParams();
			if( rParams.GetHurtDelayTime() > 0 )
			{
				iTime = rParams.GetHurtDelayTime();
			}
			rParams.SetHurtDelayTime( 0 );
		}
	}

	//���ǵ������ӳ٣����Ե���50������ӳ���Ϊ������Ч
	if( iTime < 50 )
	{
		Impact_RegisterImpactImp( oImpact );
		Impact_SendImpactMsg( oImpact, iTime );
		return;
	}

	oImpact.SetDelayTime( iTime+g_pTimeManager->CurrentTime() );
	//�����ʱ���ܵ������Ѿ�����pool�ߴ������
	if( MAX_DELAY_TIME_IMPACT_NUM <= m_DelayTimeImpactList.m_Count )
	{
		return;
	}
	m_DelayTimeImpactList.m_aImpacts[m_DelayTimeImpactList.m_Count] = oImpact;
	++m_DelayTimeImpactList.m_Count;
	__LEAVE_FUNCTION
}

VOID Obj_Character::Impact_RegisterImpactImp(OWN_IMPACT& rImp)
{
	__ENTER_FUNCTION

	ImpactLogic_T const* pLogic = Impact_GetLogic(rImp);
	if(NULL==pLogic)
	{
		return;
	}
	if(TRUE==IsUnbreakable() && BEHAVIOR_TYPE_HOSTILITY==Impact_GetStandFlag(rImp))
	{
		return ;
	}
	//����ǳ�����Ч������Ҫ���ɫ���ϵ�Ч���б����
	if(TRUE==Impact_IsOverTimed(rImp))
	{
		//���ȿ��б�����û��ͬ���Ч�������滻֮ǰ��Ч��
		//�ô�Ч���滻Ч���б��е�����Ч��,���ʧ���������Ч��
		if(FALSE == Impact_ReplaceMutuallyExclusiveImpact(rImp))
		{
			//���ɫ���ϵ�Ч���б�������µ�Ч��
			Impact_AddNewImpact(rImp);
			return;
		}
	}
	//����Ƿǳ�����Ч����ֱ����Ч
	else
	{
		//ʹЧ����Ч
		Impact_OnImpactActived(rImp);
	}
	__LEAVE_FUNCTION
}

BOOL 	Obj_Character::Impact_RegisterImpact(OWN_IMPACT& rImp)
{
	__ENTER_FUNCTION
	//���Ч�����ӳ�ʱ��������Ч���أ���ʱ�䵽�ٴ���
	//����ӳ�ʱ��Ϊ0����������ʹЧ����Ч
	Impact_PushToDelayTimeList( rImp );
	__LEAVE_FUNCTION
	return TRUE;
}
BOOL 	Obj_Character::Impact_UnRegisterImpactBySN(INT const nSn)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	if(-1 == nSn)
	{
		return FALSE;
	}
	for(INT i=0;rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		if(nSn == rImp.GetSN())
		{
			if(TRUE == Impact_CanBeCanceled(rImp))
			{
				Impact_OnImpactFadeOut(rImp);
				return TRUE;
			}
		}
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL 	Obj_Character::Impact_UnRegisterImpactBySkillIDAndImpactID(SkillID_t const nSkillID, ImpactID_t const nImpactID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	if(INVALID_ID == nSkillID || INVALID_ID == nImpactID)
	{
		return FALSE;
	}
	for(INT i=0;rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		if(nSkillID == rImp.GetSkillID() && nImpactID==Impact_GetImpactID(rImp))
		{
			if(TRUE == Impact_CanBeCanceled(rImp))
			{
				Impact_OnImpactFadeOut(rImp);
				return TRUE;
			}
		}
	}
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL Obj_Character::Impact_UnregisterImpactByDataIndex( INT iDataIndex )
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	if( INVALID_ID == iDataIndex )
	{
		return FALSE;
	}
	ImpactLogic_T const* pLogic = NULL;	

	for(INT i=0;rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		
		if( iDataIndex == rImp.GetDataIndex() )
		{
			pLogic = Impact_GetLogic(rImp);
			if(NULL==pLogic)
			{
				Impact_OnImpactFadeOut(rImp);
				continue;
			}
			if(FALSE==Impact_IsOverTimed(rImp))
			{
				continue;
			}
			if(TRUE==rImp.IsFadeOut())
			{
				continue;
			}

			if(TRUE == Impact_CanBeCanceled(rImp))
			{
				Impact_OnImpactFadeOut(rImp);
				return TRUE;
			}
		}
	}
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL 	Obj_Character::Impact_HaveImpactOfSpecificImpactID(INT const nImpactID)
{
	__ENTER_FUNCTION
	return (NULL!=Impact_GetFirstImpactOfSpecificImpactID(nImpactID));
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL 	Obj_Character::Impact_HaveImpactOfSpecificLogicID(INT const nLogicID)
{
	__ENTER_FUNCTION
	return (NULL!=Impact_GetFirstImpactOfSpecificLogicID(nLogicID));
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL 	Obj_Character::Impact_HaveImpactOfSpecificMutexID(INT const nMutexID)
{
	__ENTER_FUNCTION
	return (NULL!=Impact_GetFirstImpactOfSpecificMutexID(nMutexID));
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL 	Obj_Character::Impact_HaveImpactOfSpecificSkillID(INT const nSkillID)
{
	__ENTER_FUNCTION
	return (NULL!=Impact_GetFirstImpactOfSpecificSkillID(nSkillID));
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL 	Obj_Character::Impact_HaveImpactInSpecificCollection(INT const nCollectionID)
{
	__ENTER_FUNCTION
	return (NULL!=Impact_GetFirstImpactInSpecificCollection(nCollectionID));
	__LEAVE_FUNCTION
	return FALSE;
}

INT 	Obj_Character::Impact_OnFiltrateImpact(OWN_IMPACT& rImpact)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else
		{
			ID_t nRet = pLogic->OnFiltrateImpact(rImp, *this, rImpact);
			if(MissFlag_T::FLAG_NORMAL!= nRet)
			{
				return nRet;
			}
		}
	}
	return MissFlag_T::FLAG_NORMAL;
	__LEAVE_FUNCTION
	return MissFlag_T::FLAG_NORMAL;
}
INT	Obj_Character::Impact_DispelSpecificImpactByDataIndex(INT nDataIndex, INT nNeedDispel)
{
	__ENTER_FUNCTION
	INT nDispeled = 0;
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rImpactCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;	
	for(INT nIdx=0; rImpactCount>nIdx&&nDispeled<nNeedDispel; ++nIdx)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[nIdx];
		if(nNeedDispel>0 && nNeedDispel<=nDispeled)
		{
			break;
		}
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		if(FALSE==Impact_IsOverTimed(rImp))
		{
			continue;
		}
		if(TRUE==rImp.IsFadeOut())
		{
			continue;
		}
		//������ܱ���ɢ
		if(FALSE == Impact_CanBeDispeled(rImp))
		{
			continue;
		}		
		if(nDataIndex==rImp.GetDataIndex())
		{
			Impact_OnImpactFadeOut(rImp);
			++nDispeled;
		}
	}
	return nDispeled;
	__LEAVE_FUNCTION
	return 0;
}

INT	Obj_Character::Impact_DispelImpactOfSpecificImpactID(INT nImpactID, INT nNeedDispel)
{
	__ENTER_FUNCTION
	INT nDispeled = 0;
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rImpactCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;	
	for(INT nIdx=0; rImpactCount>nIdx&&nDispeled<nNeedDispel; ++nIdx)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[nIdx];
		if(nNeedDispel>0 && nNeedDispel<=nDispeled)
		{
			break;
		}
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		if(FALSE==Impact_IsOverTimed(rImp))
		{
			continue;
		}
		if(TRUE==rImp.IsFadeOut())
		{
			continue;
		}
		//������ܱ���ɢ
		if(FALSE == Impact_CanBeDispeled(rImp))
		{
			continue;
		}
		if(nImpactID==Impact_GetImpactID(rImp))
		{
			Impact_OnImpactFadeOut(rImp);
			++nDispeled;
		}
	}
	return nDispeled;
	__LEAVE_FUNCTION
	return 0;
}

INT	Obj_Character::Impact_DispelImpactOfSpecificStand(ID_t nStandID, INT nNeedDispel)
{
	__ENTER_FUNCTION
	INT nDispeled = 0;
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rImpactCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;	
	for(INT nIdx=0; rImpactCount>nIdx&&nDispeled<nNeedDispel; ++nIdx)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[nIdx];
		if(nNeedDispel>0 && nNeedDispel<=nDispeled)
		{
			break;
		}
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		if(FALSE==Impact_IsOverTimed(rImp))
		{
			continue;
		}
		if(TRUE==rImp.IsFadeOut())
		{
			continue;
		}
		//������ܱ���ɢ
		if(FALSE == Impact_CanBeDispeled(rImp))
		{
			continue;
		}
		if(nStandID==Impact_GetStandFlag(rImp))
		{
			Impact_OnImpactFadeOut(rImp);
			++nDispeled;
		}
	}
	return nDispeled;
	__LEAVE_FUNCTION
	return 0;
}

INT	Obj_Character::Impact_DispelImpactOfSpecificMutexID(INT nMutexID, INT nNeedDispel)
{
	__ENTER_FUNCTION
	INT nDispeled = 0;
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rImpactCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;	
	for(INT nIdx=0; rImpactCount>nIdx&&nDispeled<nNeedDispel; ++nIdx)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[nIdx];
		if(nNeedDispel>0 && nNeedDispel<=nDispeled)
		{
			break;
		}
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		if(FALSE==Impact_IsOverTimed(rImp))
		{
			continue;
		}
		if(TRUE==rImp.IsFadeOut())
		{
			continue;
		}
		//������ܱ���ɢ
		if(FALSE == Impact_CanBeDispeled(rImp))
		{
			continue;
		}		
		if(nMutexID==Impact_GetMutexID(rImp))
		{
			Impact_OnImpactFadeOut(rImp);
			++nDispeled;
		}
	}
	return nDispeled;
	__LEAVE_FUNCTION
	return 0;
}

INT	Obj_Character::Impact_DispelImpactOfSpecificLogicID(INT nLogicID, INT nNeedDispel)
{
	__ENTER_FUNCTION
	INT nDispeled = 0;
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rImpactCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;	
	for(INT nIdx=0; rImpactCount>nIdx&&nDispeled<nNeedDispel; ++nIdx)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[nIdx];
		if(nNeedDispel>0 && nNeedDispel<=nDispeled)
		{
			break;
		}
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		if(FALSE==Impact_IsOverTimed(rImp))
		{
			continue;
		}
		if(TRUE==rImp.IsFadeOut())
		{
			continue;
		}
		//������ܱ���ɢ
		if(FALSE == Impact_CanBeDispeled(rImp))
		{
			continue;
		}		
		if(nLogicID==Impact_GetLogicID(rImp))
		{
			Impact_OnImpactFadeOut(rImp);
			++nDispeled;
		}
	}
	return nDispeled;
	__LEAVE_FUNCTION
	return 0;
}

INT	Obj_Character::Impact_DispelImpactOfSpecificSkillID(INT nSkillID, INT nNeedDispel)
{
	__ENTER_FUNCTION
	INT nDispeled = 0;
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rImpactCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;	
	for(INT nIdx=0; rImpactCount>nIdx&&nDispeled<nNeedDispel; ++nIdx)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[nIdx];
		if(nNeedDispel>0 && nNeedDispel<=nDispeled)
		{
			break;
		}
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		if(FALSE==Impact_IsOverTimed(rImp))
		{
			continue;
		}
		if(TRUE==rImp.IsFadeOut())
		{
			continue;
		}
		//������ܱ���ɢ
		if(FALSE == Impact_CanBeDispeled(rImp))
		{
			continue;
		}		
		if(nSkillID==rImp.GetSkillID())
		{
			Impact_OnImpactFadeOut(rImp);
			++nDispeled;
		}
	}
	return nDispeled;
	__LEAVE_FUNCTION
	return 0;
}
INT	Obj_Character::Impact_DispelImpactInSpecificCollection(INT nCollectionID, INT nDispelLevel, INT nNeedDispel)
{
	__ENTER_FUNCTION
	INT nDispeled = 0;
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rImpactCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;	
	for(INT nIdx=0; rImpactCount>nIdx&&nDispeled<nNeedDispel; ++nIdx)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[nIdx];
		if(nNeedDispel>0 && nNeedDispel<=nDispeled)
		{
			break;
		}
		if(FALSE==rImp.IsFadeOut() && TRUE == Impact_CanBeDispeled(rImp))
		{
			if(Impact_GetLevel(rImp)<=nDispelLevel)
			{
				if(TRUE==Impact_IsImpactInCollection(rImp, nCollectionID))
				{
					Impact_OnImpactFadeOut(rImp);
					++nDispeled;
				}
			}
		}
	}
	return nDispeled;
	__LEAVE_FUNCTION
	return 0;
}
VOID Obj_Character::Impact_CancelImpactInSpecificCollection(INT nCollectionID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rImpactCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;	
	for(INT nIdx=0; rImpactCount>nIdx; ++nIdx)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[nIdx];
		if(FALSE==rImp.IsFadeOut() && TRUE==Impact_CanBeCanceled(rImp))
		{
			if(TRUE==Impact_IsImpactInCollection(rImp, nCollectionID))
			{
				Impact_OnImpactFadeOut(rImp);
			}
		}
	}
	__LEAVE_FUNCTION
}
//ִ���������г�����Ч���߼�����ÿһ��Ч�������OnDamages
VOID	Obj_Character::Impact_OnDamages(INT* const pDamage, Obj_Character* const pAttacker, SkillID_t nSkillID)
{
	__ENTER_FUNCTION
	//��������Ч���б�
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	//����Ч����ѭ��
	for(short i = 0; rCount>i;++i)
	{
		//���б���ȡ��Ч����������
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		//����Ч�����������ȡЧ������
		pLogic = Impact_GetLogic(rImp);
		//û���ҵ���ִ��Ч����ɢ�߼�
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		//���Ч��û����ɢ
		else if(FALSE==rImp.IsFadeOut())
		{
			//ִ��Ч��
			pLogic->OnDamages(rImp, *this, pAttacker, pDamage, nSkillID);
		}
	}
	__LEAVE_FUNCTION
}

//ִ���������г�����Ч���߼�����ÿһ��Ч�������OnDamage
VOID 	Obj_Character::Impact_OnDamage(INT& nDamage, Obj_Character* const pAttacker, SkillID_t nSkillID)
{
	__ENTER_FUNCTION
	//��ý�ɫ����Ч���б�
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	//����Ч����ѭ��
	for(short i = 0; rCount>i;++i)
	{
		//��Ч���б���ȡ��Ч����������
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		//ȡ��Ч���߼�����
		pLogic = Impact_GetLogic(rImp);
		//û�д˶�����ִ����ɢ�߼�
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		//���Ч��û����ɢ
		else if(FALSE==rImp.IsFadeOut())
		{
			if(FALSE==Impact_IsFadeOutWhenUnitOnDamage(rImp))
			{
				pLogic->OnDamage(rImp, *this, pAttacker, nDamage, nSkillID);
			}
			else
			{
				Impact_OnImpactFadeOut(rImp);
			}
		}
	}
	__LEAVE_FUNCTION
}
VOID	Obj_Character::Impact_OnDamageTarget(INT& nDamage, Obj_Character& rTar, SkillID_t nSkillID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut())
		{
			pLogic->OnDamageTarget(rImp, *this, rTar, nDamage, nSkillID);
		}
	}
	__LEAVE_FUNCTION
}

VOID 	Obj_Character::Impact_RefixSkill(SkillInfo_T& rSkill)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->RefixSkill(rImp, rSkill);
		}
	}
	__LEAVE_FUNCTION
}
VOID 	Obj_Character::Impact_RefixImpact(OWN_IMPACT& rImpactNeedRefix)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->RefixImpact(rImp, *this, rImpactNeedRefix);
		}
	}
	__LEAVE_FUNCTION
}

VOID 	Obj_Character::Impact_RefixRageRegeneration(INT& rRageRegeneration)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->RefixRageRegeneration(rImp, rRageRegeneration);
		}
	}
	__LEAVE_FUNCTION
}

BOOL Obj_Character::Impact_GetIntAttrRefix(CharIntAttrRefixs_T::Index_T nIdx, INT& rIntAttrRefix)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->GetIntAttrRefix(rImp, *this, nIdx, rIntAttrRefix);
		}
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL Obj_Character::Impact_GetBoolAttrRefix(CharBoolAttrs_T::Index_T nIdx, BOOL& rBoolAttrRefix)
{
	__ENTER_FUNCTION
	//��ý�ɫ���ϵ�Impact�б�
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	//����Impact��ѭ��
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		//��ȡImpact�߼�����
		pLogic = Impact_GetLogic(rImp);
		//���û���ҵ���Ӧ�Ķ��󣬵�����ɢ�߼�
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			if(TRUE==pLogic->GetBoolAttrRefix(rImp, *this, nIdx, rBoolAttrRefix))
			{
				return TRUE;
			}
		}
	}
	__LEAVE_FUNCTION
	return FALSE;
}
VOID	Obj_Character::Impact_OnDie(VOID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->OnDie(rImp, *this);
		}
	}
	__LEAVE_FUNCTION
}
VOID 	Obj_Character::Impact_OnBeHit(Obj_Character & rAttacker)
{
	__ENTER_FUNCTION
	//ȡ�ý�ɫ���ϵ�Ч���б�
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		//�����б�ȡ��Ч���߼�
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		//���δ�ҵ�Impact�߼�����,��Ч����ɢ����
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		//�������Ч��û����ɢ��Ҳ���ǳ�����Ч��
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			//������ӦImpact����Ϣ����
			pLogic->OnBeHit(rImp, *this, rAttacker);
		}
	}
	__LEAVE_FUNCTION
}

VOID 	Obj_Character::Impact_OnMissTarget(Obj_Character& rTarget)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->OnMissTarget(rImp, *this, rTarget);
		}
	}
	__LEAVE_FUNCTION
}
///////////////////////////////////////////////
VOID 	Obj_Character::Impact_OnMissHit(Obj_Character & rAttacker)
{
	__ENTER_FUNCTION
	//ȡ�ý�ɫ���ϵ�Ч���б�
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		//�����б�ȡ��Ч���߼�
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		//���δ�ҵ�Impact�߼�����,��Ч����ɢ����
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		//�������Ч��û����ɢ��Ҳ���ǳ�����Ч��
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			//������ӦImpact����Ϣ����
			pLogic->OnMissHit(rImp, *this, rAttacker);
		}
	}
	__LEAVE_FUNCTION
}

VOID 	Obj_Character::Impact_OnHitTarget(Obj_Character& rTarget)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->OnHitTarget(rImp, *this, rTarget);
		}
	}
	__LEAVE_FUNCTION
}

///////////////////////////////////////////////
VOID	Obj_Character::Impact_OnCriticalHitTarget(SkillID_t const nSkill, Obj_Character& rTarget)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->OnCriticalHitTarget(rImp, *this);
		}
	}
	__LEAVE_FUNCTION
}
VOID 	Obj_Character::Impact_OnBeCriticalHit(SkillID_t const nSkill, Obj_Character& rAttacker)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->OnBeCriticalHit(rImp, *this);
		}
	}
	__LEAVE_FUNCTION
}
VOID 	Obj_Character::Impact_OnHealTarget(SkillID_t const nSkill, Obj_Character& rTarget, INT& rHealedHP)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->OnHealTarget(rImp, *this, rHealedHP);
		}
	}
	__LEAVE_FUNCTION
}
VOID 	Obj_Character::Impact_OnBeHeal(SkillID_t const nSkill, Obj_Character& rCaster, INT& rHealedHP)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->OnBeHeal(rImp, *this, rHealedHP);
		}
	}
	__LEAVE_FUNCTION
}
VOID	Obj_Character::Impact_OnActionStarted(VOID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			if(TRUE == Impact_IsFadeOutWhenUnitStartActions(rImp))
			{
				Impact_OnImpactFadeOut(rImp);
			}
		}
	}
	__LEAVE_FUNCTION	
}
VOID	Obj_Character::Impact_OnUseSkillSuccessfully(SkillInfo_T& rSkill)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			pLogic->OnUseSkillSuccessfully(rImp, *this, rSkill);
		}
	}
	__LEAVE_FUNCTION	
}
BOOL 	Obj_Character::Impact_RemoveImpactByIndex(INT const nIdx)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	if(0>nIdx || (rCount)<=nIdx)
	{
		AssertEx(FALSE,"[Obj_Character::Impact_RemoveImpactByIndex]: nIdx is out of range!");
		return FALSE;
	}
	_OWN_IMPACT& rCurImp= rList.m_aImpacts[nIdx];
	_OWN_IMPACT& rLastImpact = rList.m_aImpacts[rCount-1];
	if(nIdx < (rCount-1))
	{
		rCurImp = rLastImpact;
	}
	rLastImpact.CleanUp();
	--(rCount);
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Character::Impact_SendImpactMsg( OWN_IMPACT& rImp, Time_t iDelayTime )
{
	__ENTER_FUNCTION
	//���Ч���߼�����
	ImpactLogic_T const* pLogic = NULL;
	pLogic = Impact_GetLogic(rImp);
	if(NULL==pLogic)
	{
		return;
	}

	Scene * pScene = getScene();
	if(NULL==pScene||FALSE==IsActiveObj())
	{
		return;
	}

	//����ǳ�����Ч��
	if(TRUE == Impact_IsOverTimed(rImp))
	{
		//��Ч����Ӧ�Ľ�ɫ���Ե�����
		//pLogic->MarkModifiedAttrDirty(rImp, *this);

		// Info Self Impact
		//֪ͨ�Լ��ͻ���Ч����Ч
		Packets::GCDetailBuff Msg2Self;
		Msg2Self.SetReceiverID(GetID());
		Msg2Self.SetSenderID(rImp.GetCasterObjID());
		Msg2Self.SetEnable(TRUE);
		Msg2Self.SetSkillID(rImp.GetSkillID());
		Msg2Self.SetSN(rImp.GetSN());
		Msg2Self.SetBuffID(Impact_GetImpactID(rImp));
		Msg2Self.SetContinuance(rImp.GetContinuance());
		Msg2Self.SetDelayTime( iDelayTime );
		INT nSenderLogicCount = 0;
		Scene* pScene = getScene();
		if(NULL!=pScene)
		{
			Msg2Self.SetSenderLogicCount(rImp.GetCasterLogicCount());
			if(Obj::OBJ_TYPE_HUMAN==GetObjType())
			{
				if(INVALID_ID!=Impact_GetImpactID(rImp))
				{
					Player* pGamePlayer = ((Obj_Human*)this)->GetPlayer();
					if(NULL!=pGamePlayer)
					{
						pGamePlayer->SendPacket(&Msg2Self);
					}
				}
			}				
		}
		// Info Other Impact
		//֪ͨ�����ͻ���Ч����Ч
		Packets::GCCharBuff Msg2Other;
		Msg2Other.SetReceiverID(GetID());
		Msg2Other.SetSenderID(rImp.GetCasterObjID());
		Msg2Other.SetEnable(TRUE);
		Msg2Other.SetSN(rImp.GetSN());
		Msg2Other.SetBuffID(Impact_GetImpactID(rImp));
		Msg2Other.SetSenderLogicCount(Msg2Self.GetSenderLogicCount());
		Msg2Other.SetSkillID( rImp.GetSkillID() );
		Msg2Other.SetLayerCount( rImp.GetLayerCount() );
		Msg2Other.SetDelayTime( iDelayTime );
		Msg2Other.SetContinuance( rImp.GetContinuance());
		if(INVALID_ID!=Impact_GetImpactID(rImp))
		{
			pScene->BroadCast(&Msg2Other, this, TRUE);
		}
	}
	//������ǳ�����Ч����㲥Ч����Ϣ,��client������Ч
	else
	{
		Packets::GCCharDirectImpact Msg2All;
		Msg2All.SetReceiverID(GetID());
		Msg2All.SetSenderID(rImp.GetCasterObjID());
		Msg2All.SetSkillID(rImp.GetSkillID());
		//�ο�GameStruct_Impact.h��DIRECT_IMPACT_SEID,��ʾ��ɫ״̬
		Msg2All.SetImpactID(Impact_GetImpactID(rImp));
		Msg2All.SetDelayTime( iDelayTime );
		INT nSenderLogicCount = 0;
		Scene* pScene = getScene();

		Obj_Character * pImpactSender = (Obj_Character*)Impact_GetCaster(*this, rImp);
		if(NULL!=pImpactSender)
		{
			nSenderLogicCount = pImpactSender->GetLogicCount();
		}
		Msg2All.SetSenderLogicCount(nSenderLogicCount);
		if(INVALID_ID!=Impact_GetImpactID(rImp))
		{
			pScene->BroadCast( &Msg2All, this, TRUE );
		}
	}
	__LEAVE_FUNCTION
}

//ʹЧ����Ч�����þ���Ч�������OnActive
VOID	Obj_Character::Impact_OnImpactActived(OWN_IMPACT& rImp)
{
	__ENTER_FUNCTION
	//���Ч���߼�����
	ImpactLogic_T const* pLogic = NULL;
	pLogic = Impact_GetLogic(rImp);
	if(NULL==pLogic)
	{
		return;
	}
	//����ǳ�����Ч��
	if(TRUE == Impact_IsOverTimed(rImp))
	{
		//��Ч����Ӧ�Ľ�ɫ���Ե�����
		pLogic->MarkModifiedAttrDirty(rImp, *this);
	}

	//Ч���������Ϣ����
	pLogic->OnActive(rImp, *this);


	////����ǳ�����Ч��
	//if(TRUE == Impact_IsOverTimed(rImp))
	//{
	//	//��Ч����Ӧ�Ľ�ɫ���Ե�����
	//	pLogic->MarkModifiedAttrDirty(rImp, *this);

	//	// Info Self Impact
	//	//֪ͨ�Լ��ͻ���Ч����Ч
	//	Packets::GCDetailBuff Msg2Self;
	//	Msg2Self.SetReceiverID(GetID());
	//	Msg2Self.SetSenderID(rImp.GetCasterObjID());
	//	Msg2Self.SetEnable(TRUE);
	//	Msg2Self.SetSkillID(rImp.GetSkillID());
	//	Msg2Self.SetSN(rImp.GetSN());
	//	Msg2Self.SetBuffID(Impact_GetImpactID(rImp));
	//	Msg2Self.SetContinuance(rImp.GetContinuance());
	//	INT nSenderLogicCount = 0;
	//	Scene* pScene = getScene();
	//	if(NULL!=pScene)
	//	{
	//		Msg2Self.SetSenderLogicCount(rImp.GetCasterLogicCount());
	//		if(Obj::OBJ_TYPE_HUMAN==GetObjType())
	//		{
	//			if(INVALID_ID!=Impact_GetImpactID(rImp))
	//			{
	//				Player* pGamePlayer = ((Obj_Human*)this)->GetPlayer();
	//				if(NULL!=pGamePlayer)
	//				{
	//					pGamePlayer->SendPacket(&Msg2Self);
	//				}
	//			}
	//		}				
	//	}
	//	// Info Other Impact
	//	//֪ͨ�����ͻ���Ч����Ч
	//	Packets::GCCharBuff Msg2Other;
	//	Msg2Other.SetReceiverID(GetID());
	//	Msg2Other.SetSenderID(rImp.GetCasterObjID());
	//	Msg2Other.SetEnable(TRUE);
	//	Msg2Other.SetSN(rImp.GetSN());
	//	Msg2Other.SetBuffID(Impact_GetImpactID(rImp));
	//	Msg2Other.SetSenderLogicCount(Msg2Self.GetSenderLogicCount());
	//	Msg2Other.SetSkillID( rImp.GetSkillID() );
	//	Msg2Other.SetLayerCount( rImp.GetLayerCount() );

	//	if(INVALID_ID!=Impact_GetImpactID(rImp))
	//	{
	//		pScene->BroadCast(&Msg2Other, this, TRUE);
	//	}
	//}
	////������ǳ�����Ч����㲥Ч����Ϣ,��client������Ч
	//else
	//{
	//	Packets::GCCharDirectImpact Msg2All;
	//	Msg2All.SetReceiverID(GetID());
	//	Msg2All.SetSenderID(rImp.GetCasterObjID());
	//	Msg2All.SetSkillID(rImp.GetSkillID());
	//	//�ο�GameStruct_Impact.h��DIRECT_IMPACT_SEID,��ʾ��ɫ״̬
	//	Msg2All.SetImpactID(Impact_GetImpactID(rImp));
	//	INT nSenderLogicCount = 0;
	//	Scene* pScene = getScene();

	//	Obj_Character * pImpactSender = (Obj_Character*)Impact_GetCaster(*this, rImp);
	//	if(NULL!=pImpactSender)
	//	{
	//		nSenderLogicCount = pImpactSender->GetLogicCount();
	//	}
	//	Msg2All.SetSenderLogicCount(nSenderLogicCount);
	//	if(INVALID_ID!=Impact_GetImpactID(rImp))
	//	{
	//		pScene->BroadCast( &Msg2All, this, TRUE );
	//	}
	//}

	__LEAVE_FUNCTION
}

VOID	Obj_Character::Impact_OnImpactFadeOut(OWN_IMPACT& rImp)
{
	__ENTER_FUNCTION
	//Mark fade out first!!
	rImp.MarkFadeOutFlag();
	
	ImpactLogic_T const* pLogic = NULL;
	pLogic = Impact_GetLogic(rImp);
	if(NULL==pLogic)
	{
		return;
	}
	pLogic->OnFadeOut(rImp, *this);	
	if(TRUE == Impact_IsOverTimed(rImp))
	{
		pLogic->MarkModifiedAttrDirty(rImp, *this);
	}
	Scene * pScene = getScene();
	if(NULL==pScene||FALSE==IsActiveObj())
	{
		return;
	}
	if(TRUE==Impact_IsOverTimed(rImp))
	{
		if(Obj::OBJ_TYPE_HUMAN==GetObjType())
		{
			// Info Self Impact
			//��client self ����Ч����ʧ����Ϣ,��ֹ����Ч�� Msg2Self.SetEnable(FALSE);Ϊ��ʧ���
			Packets::GCDetailBuff Msg2Self;
			Msg2Self.SetReceiverID(GetID());
			Msg2Self.SetSenderID(rImp.GetCasterObjID());
			Msg2Self.SetEnable(FALSE);
			Msg2Self.SetSkillID(rImp.GetSkillID());
			Msg2Self.SetSN(rImp.GetSN());
			Msg2Self.SetBuffID(Impact_GetImpactID(rImp));
			Msg2Self.SetSenderLogicCount(0);
			
			if(Obj::OBJ_TYPE_HUMAN==GetObjType())
			{
				if(INVALID_ID!=Impact_GetImpactID(rImp))
				{
					Player* pGamePlayer = ((Obj_Human*)this)->GetPlayer();
					if(NULL!=pGamePlayer)
					{
						pGamePlayer->SendPacket(&Msg2Self);
					}
				}
			}
		}
		// Info Other Impact
		//�㲥��client��Ч��ɢ��Ϣ
		Packets::GCCharBuff Msg2Other;
		Msg2Other.SetReceiverID(GetID());
		Msg2Other.SetSenderID(rImp.GetCasterObjID());
		Msg2Other.SetEnable(FALSE);
		Msg2Other.SetSN(rImp.GetSN());
		Msg2Other.SetBuffID(Impact_GetImpactID(rImp));
		Msg2Other.SetSenderLogicCount(0);
		Msg2Other.SetLayerCount(rImp.GetLayerCount());
		Msg2Other.SetSkillID(rImp.GetSkillID());

		// ���ü���Ч���ĳ���ʱ�� 20100531 BLL
		Msg2Other.SetContinuance( rImp.GetContinuance() );

		if(INVALID_ID!=Impact_GetImpactID(rImp))
		{
			pScene->BroadCast(&Msg2Other, this, TRUE);
		}

		// ��ɢ��buff�������������֧�ֵģ���ֹ��������
		if(TRUE==Impact_NeedChannelSupport(rImp))
		{
			Obj* pObj = Impact_GetCaster(*this, rImp);
			if(NULL == pObj)
			{
				return;
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				return;
			}
			Obj_Character* pCaster = (Obj_Character*)pObj;
			if(TRUE==GetGlobalActionDelegator().IsChanneling(*pCaster)
			|| TRUE==GetGlobalActionDelegator().IsChanneling(*pCaster))
			{
				if(pCaster->GetLogicCount() == rImp.GetCasterLogicCount())
				{
					GetGlobalActionDelegator().InterruptCurrentAction(*pCaster);
				}
			}
		}
	}
	__LEAVE_FUNCTION
}
BOOL 	Obj_Character::Impact_ReplaceMutuallyExclusiveImpact(OWN_IMPACT& rImpactIn)
{
	__ENTER_FUNCTION
	//��������Ч���б�
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pCurrentImpactLogic = NULL;
	//�����б��е���
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rCurrentImpact = rList.m_aImpacts[i];
		pCurrentImpactLogic = Impact_GetLogic(rCurrentImpact);
		//δ�ҵ���Ӧ���߼����������Ч����ɢ�߼�
		if(NULL==pCurrentImpactLogic)
		{
			Impact_OnImpactFadeOut(rCurrentImpact);
			continue;
		}
		//�����ǰ״̬������ɢ�������ǳ�����Ч��
		else if(FALSE==rCurrentImpact.IsFadeOut()&&TRUE==Impact_IsOverTimed(rCurrentImpact))
		{
			//�����ǰЧ���ʹ����Ч������ͬһ��������
			if((Impact_GetMutexID(rCurrentImpact)==Impact_GetMutexID(rImpactIn))&&(INVALID_ID!=Impact_GetMutexID(rCurrentImpact)))
			{
				//�������Ч�����ų�ͬһ���˷���
				if(TRUE == Impact_IsImpactsABMutexed(rImpactIn, rCurrentImpact))
				{
					//�����������Ч���滻�ɵ�Ч�����滻Ч��
					if(TRUE == Impact_CanImpactAReplaceImpactB(rImpactIn, rCurrentImpact))
					{
						Impact_ReplaceImpact(rCurrentImpact, rImpactIn);
					}
					//������client����δ������Ϣ
					else
					{
						Scene* pScene = getScene();
						if(NULL!=pScene)
						{
							GCCharSkill_Missed Msg2All;
							Msg2All.SetReceiverID(this->GetID());
							Msg2All.SetSenderID(rImpactIn.GetCasterObjID());
							Msg2All.SetFlag(MissFlag_T::FLAG_IMMU);
							Msg2All.SetSenderLogicCount(rImpactIn.GetCasterLogicCount());
							pScene->BroadCast(&Msg2All, this, TRUE);
						}
					}
					return TRUE;//����滻����,����Ҫ�¼�Ч����.
				}
			}
		}
	}
	return FALSE;//û���κ��滻����,��Ȼ��Ҫ�¼�Ч����.
	__LEAVE_FUNCTION
	return FALSE;
}
VOID	Obj_Character::Impact_ReplaceLastGoodImpact(OWN_IMPACT& rImpactIn)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pCurrentImpactLogic = NULL;
	for(short i = rCount-1; 0<=i;++i)
	{
		if( i>= MAX_IMPACT_NUM )
		{
			return;
		}
		OWN_IMPACT& rCurrentImpact = rList.m_aImpacts[i];
		pCurrentImpactLogic = Impact_GetLogic(rCurrentImpact);
		if(NULL==pCurrentImpactLogic)
		{
			Impact_OnImpactFadeOut(rCurrentImpact);
			continue;
		}
		else if(FALSE==rCurrentImpact.IsFadeOut()&&TRUE==Impact_IsOverTimed(rCurrentImpact))
		{
			if(BEHAVIOR_TYPE_HOSTILITY!=Impact_GetStandFlag(rCurrentImpact) && TRUE==Impact_CanBeDispeled(rCurrentImpact))
			{
				Impact_ReplaceImpact(rCurrentImpact, rImpactIn);
				return;
			}
		}
	}
	__LEAVE_FUNCTION
}

BOOL 	Obj_Character::Impact_AddNewImpact(OWN_IMPACT& rImpactIn)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	if(rCount<MAX_IMPACT_NUM)
	{
		Impact_AddSNSeed();
		rImpactIn.SetSN(Impact_GetSNSeed());
		OWN_IMPACT& rLast = rList.m_aImpacts[rCount];
		rLast= rImpactIn;
		++rCount;
		Impact_OnImpactActived(rLast);
		return TRUE;
	}
	else
	{
		Impact_ReplaceLastGoodImpact(rImpactIn);
	}
	__LEAVE_FUNCTION
	return FALSE;
}
VOID	Obj_Character::Impact_ReplaceImpact(OWN_IMPACT& rDest, OWN_IMPACT& rSrc)
{
	__ENTER_FUNCTION
	Impact_AddSNSeed();
	rSrc.SetSN(Impact_GetSNSeed());
	Impact_OnImpactFadeOut(rDest);
	rDest = rSrc;
	Impact_OnImpactActived(rDest);
	__LEAVE_FUNCTION
}
VOID	Obj_Character::Impact_CleanupAllFadeOutImpacts(VOID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	for(INT nIdx = Impact_GetListSize()-1; 0<=nIdx; --nIdx)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[nIdx];
		if(TRUE==rImp.IsFadeOut())
		{
			Impact_RemoveImpactByIndex(nIdx);
		}
	}
	__LEAVE_FUNCTION
}

VOID	Obj_Character::Impact_CleanAllImpactWhenPetDead(ObjID_t idPet)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	for(INT i=0;rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		if (rImp.GetCasterObjID() == idPet)
		{
			Impact_OnImpactFadeOut(rImp);
		}
	}
	Impact_CleanupAllFadeOutImpacts();
	__LEAVE_FUNCTION

}

OWN_IMPACT* Obj_Character::Impact_GetFirstImpactOfSpecificImpactID(INT nImpactID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		if(nImpactID == Impact_GetImpactID(rImp))
		{
			return &rImp;
		}
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

OWN_IMPACT* Obj_Character::Impact_GetFirstImpactOfSpecificLogicID(INT nLogicID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		if(nLogicID == Impact_GetLogicID(rImp))
		{
			return &rImp;
		}
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

OWN_IMPACT* Obj_Character::Impact_GetFirstImpactOfSpecificMutexID(INT nMutexID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		if(nMutexID == Impact_GetMutexID(rImp))
		{
			return &rImp;
		}
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

OWN_IMPACT* Obj_Character::Impact_GetFirstImpactOfSpecificSkillID(INT nSkillID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		if(nSkillID == rImp.GetSkillID())
		{
			return &rImp;
		}
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}
	
OWN_IMPACT* Obj_Character::Impact_GetFirstImpactInSpecificCollection(INT nCollectionID)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(nCollectionID);
	if(NULL==pCollection)
	{
		AssertEx(FALSE,"[Obj_Character::Impact_GetFirstImpactInSpecificCollection]: Illegal Collection ID, check it");
		return NULL;
	}
	if(0==pCollection->GetCollectionSize())
	{
		AssertEx(FALSE,"[Obj_Character::Impact_GetFirstImpactInSpecificCollection]: Empty Collection, check it");
		return NULL;
	}
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		if(TRUE==Impact_IsImpactInCollection(rImp, nCollectionID))
		{
			return &rImp;
		}
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

BOOL 	Obj_Character::Impact_CanViewThisSpecialObj(Obj_Special& rObj)
{
	__ENTER_FUNCTION
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		else
		{
			if(TRUE == pLogic->CanViewSpecialObj(rImp, rObj))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Character::ImpactDelayTimeHeartBeat()
{
	//��ʱ������Ч����
	for( INT i=0; i<m_DelayTimeImpactList.m_Count; ++i )
	{
		if( m_DelayTimeImpactList.m_aImpacts[i].GetDelayTime() <= (Time_t)g_pTimeManager->CurrentTime() )
		{
			Impact_RegisterImpactImp( m_DelayTimeImpactList.m_aImpacts[i] );
			Impact_SendImpactMsg( m_DelayTimeImpactList.m_aImpacts[i], 0 );
			m_DelayTimeImpactList.ReplaseArrayItem( i );
		}
	}
}

BOOL Obj_Character::Impact_IsCanReciveBallImpact( OWN_IMPACT& rImp )
{
	__ENTER_FUNCTION
	//������ʱ�е�Ч���б�
	for( INT i=0; i<m_DelayTimeImpactList.m_Count; ++i )
	{
		if( m_DelayTimeImpactList.m_aImpacts[i].GetCasterObjID()==rImp.GetCasterObjID() &&
			m_DelayTimeImpactList.m_aImpacts[i].GetSkillID() == rImp.GetSkillID() )
		{
			return FALSE;
		}
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Character::Impact_StealImpact(Obj_Character& rTar, INT nCollectionID, INT nStandFlag, INT nStealCount, INT nRefixRate)
{
	__ENTER_FUNCTION
	if(FALSE==rTar.IsAlive() || FALSE==rTar.IsActiveObj())
	{
		return;
	}

	//�Ѿ�͵�˵�����
	INT nStealed = 0;
	_IMPACT_LIST& rList = rTar.Impact_GetImpactList();
	BYTE& rImpactCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;	
	for(INT nIdx=0; rImpactCount>nIdx&&nStealed<nStealCount; ++nIdx)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[nIdx];
		if(nStealCount>0 && nStealCount<=nStealed)
		{
			break;
		}
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			rTar.Impact_OnImpactFadeOut(rImp);
			continue;
		}
		if(FALSE==Impact_IsOverTimed(rImp))
		{
			continue;
		}
		if(TRUE==rImp.IsFadeOut())
		{
			continue;
		}
		if(nCollectionID > 0)
		{
			if(TRUE==Impact_IsImpactInCollection(rImp, nCollectionID))
			{
				g_ImpactCore.SendImpactToUnit(*this, Impact_GetImpactID(rImp), GetID(), rImp.GetSkillID(), rImp.GetDelayTime(), rImp.IsCriticalHit(), nRefixRate);
				rTar.Impact_OnImpactFadeOut(rImp);
				++nStealed;
			}
		}
		else
		{
			if(nStandFlag == Impact_GetStandFlag(rImp))
			{
				g_ImpactCore.SendImpactToUnit(*this, Impact_GetImpactID(rImp), GetID(), rImp.GetSkillID(), rImp.GetDelayTime(), rImp.IsCriticalHit(), nRefixRate);
				rTar.Impact_OnImpactFadeOut(rImp);
				++nStealed;
			}
		}
	}
	__LEAVE_FUNCTION
}

