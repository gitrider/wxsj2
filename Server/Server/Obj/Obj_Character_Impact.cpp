///////////////////////////////////////////////////////////////////////////////
// 文件名：Obj_Character_Impact.cpp
// 程序员：高骐
// 功能说明：角色身上的效果处理
//
// 修改记录：
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
//处理角色身上的效果列表
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

//如果效果有延迟时间则推入效果池，等时间到再处理
//如果延迟时间为0则立即处理，使效果生效
VOID Obj_Character::Impact_PushToDelayTimeList( OWN_IMPACT& oImpact )
{
	__ENTER_FUNCTION
	ID_t iSkillID = oImpact.GetSkillID();
	Obj* pObj = Impact_GetCaster( *this, oImpact );
	Time_t iTime = 0;
	if( NULL != pObj )
	{	//先处理客户端传入的延迟时间
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

	//考虑到网络延迟，所以低于50毫秒的延迟视为立即生效
	if( iTime < 50 )
	{
		Impact_RegisterImpactImp( oImpact );
		Impact_SendImpactMsg( oImpact, iTime );
		return;
	}

	oImpact.SetDelayTime( iTime+g_pTimeManager->CurrentTime() );
	//如果延时技能的数量已经超过pool尺寸则忽略
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
	//如果是持续性效果则需要向角色身上的效果列表添加
	if(TRUE==Impact_IsOverTimed(rImp))
	{
		//首先看列表中有没有同类的效果可以替换之前的效果
		//用此效果替换效果列表中的已有效果,如果失败则添加新效果
		if(FALSE == Impact_ReplaceMutuallyExclusiveImpact(rImp))
		{
			//向角色身上的效果列表中添加新的效果
			Impact_AddNewImpact(rImp);
			return;
		}
	}
	//如果是非持续性效果则直接生效
	else
	{
		//使效果生效
		Impact_OnImpactActived(rImp);
	}
	__LEAVE_FUNCTION
}

BOOL 	Obj_Character::Impact_RegisterImpact(OWN_IMPACT& rImp)
{
	__ENTER_FUNCTION
	//如果效果有延迟时间则推入效果池，等时间到再处理
	//如果延迟时间为0则立即处理，使效果生效
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
		//如果不能被驱散
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
		//如果不能被驱散
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
		//如果不能被驱散
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
		//如果不能被驱散
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
		//如果不能被驱散
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
		//如果不能被驱散
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
//执行自身所有持续性效果逻辑调用每一个效果对象的OnDamages
VOID	Obj_Character::Impact_OnDamages(INT* const pDamage, Obj_Character* const pAttacker, SkillID_t nSkillID)
{
	__ENTER_FUNCTION
	//获得自身的效果列表
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	//所有效果的循环
	for(short i = 0; rCount>i;++i)
	{
		//从列表中取出效果描述对象
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		//根据效果描述对象获取效果对象
		pLogic = Impact_GetLogic(rImp);
		//没有找到则执行效果消散逻辑
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		//如果效果没有消散
		else if(FALSE==rImp.IsFadeOut())
		{
			//执行效果
			pLogic->OnDamages(rImp, *this, pAttacker, pDamage, nSkillID);
		}
	}
	__LEAVE_FUNCTION
}

//执行自身所有持续性效果逻辑调用每一个效果对象的OnDamage
VOID 	Obj_Character::Impact_OnDamage(INT& nDamage, Obj_Character* const pAttacker, SkillID_t nSkillID)
{
	__ENTER_FUNCTION
	//获得角色身上效果列表
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	//所有效果的循环
	for(short i = 0; rCount>i;++i)
	{
		//从效果列表中取得效果描述对象
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		//取得效果逻辑对象
		pLogic = Impact_GetLogic(rImp);
		//没有此对象则执行消散逻辑
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		//如果效果没有消散
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
	//获得角色身上的Impact列表
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	//所有Impact的循环
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		//获取Impact逻辑对象
		pLogic = Impact_GetLogic(rImp);
		//如果没有找到相应的对象，调用消散逻辑
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
	//取得角色身上的效果列表
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		//根据列表取得效果逻辑
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		//如果未找到Impact逻辑对象,按效果消散处理
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		//否则，如果效果没有消散，也不是持续性效果
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			//调用相应Impact的消息函数
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
	//取得角色身上的效果列表
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pLogic = NULL;
	for(short i = 0; rCount>i;++i)
	{
		//根据列表取得效果逻辑
		OWN_IMPACT& rImp = rList.m_aImpacts[i];
		//如果未找到Impact逻辑对象,按效果消散处理
		pLogic = Impact_GetLogic(rImp);
		if(NULL==pLogic)
		{
			Impact_OnImpactFadeOut(rImp);
			continue;
		}
		//否则，如果效果没有消散，也不是持续性效果
		else if(FALSE==rImp.IsFadeOut()&&TRUE==Impact_IsOverTimed(rImp))
		{
			//调用相应Impact的消息函数
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
	//获得效果逻辑对象
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

	//如果是持续性效果
	if(TRUE == Impact_IsOverTimed(rImp))
	{
		//置效果相应的角色属性的脏标记
		//pLogic->MarkModifiedAttrDirty(rImp, *this);

		// Info Self Impact
		//通知自己客户端效果生效
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
		//通知其它客户端效果生效
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
	//如果不是持续性效果则广播效果信息,让client产生特效
	else
	{
		Packets::GCCharDirectImpact Msg2All;
		Msg2All.SetReceiverID(GetID());
		Msg2All.SetSenderID(rImp.GetCasterObjID());
		Msg2All.SetSkillID(rImp.GetSkillID());
		//参考GameStruct_Impact.h的DIRECT_IMPACT_SEID,表示角色状态
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

//使效果生效，调用具体效果对象的OnActive
VOID	Obj_Character::Impact_OnImpactActived(OWN_IMPACT& rImp)
{
	__ENTER_FUNCTION
	//获得效果逻辑对象
	ImpactLogic_T const* pLogic = NULL;
	pLogic = Impact_GetLogic(rImp);
	if(NULL==pLogic)
	{
		return;
	}
	//如果是持续性效果
	if(TRUE == Impact_IsOverTimed(rImp))
	{
		//置效果相应的角色属性的脏标记
		pLogic->MarkModifiedAttrDirty(rImp, *this);
	}

	//效果激活的消息函数
	pLogic->OnActive(rImp, *this);


	////如果是持续性效果
	//if(TRUE == Impact_IsOverTimed(rImp))
	//{
	//	//置效果相应的角色属性的脏标记
	//	pLogic->MarkModifiedAttrDirty(rImp, *this);

	//	// Info Self Impact
	//	//通知自己客户端效果生效
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
	//	//通知其它客户端效果生效
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
	////如果不是持续性效果则广播效果信息,让client产生特效
	//else
	//{
	//	Packets::GCCharDirectImpact Msg2All;
	//	Msg2All.SetReceiverID(GetID());
	//	Msg2All.SetSenderID(rImp.GetCasterObjID());
	//	Msg2All.SetSkillID(rImp.GetSkillID());
	//	//参考GameStruct_Impact.h的DIRECT_IMPACT_SEID,表示角色状态
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
			//向client self 发送效果消失的消息,终止自身效果 Msg2Self.SetEnable(FALSE);为消失标记
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
		//广播给client特效消散消息
		Packets::GCCharBuff Msg2Other;
		Msg2Other.SetReceiverID(GetID());
		Msg2Other.SetSenderID(rImp.GetCasterObjID());
		Msg2Other.SetEnable(FALSE);
		Msg2Other.SetSN(rImp.GetSN());
		Msg2Other.SetBuffID(Impact_GetImpactID(rImp));
		Msg2Other.SetSenderLogicCount(0);
		Msg2Other.SetLayerCount(rImp.GetLayerCount());
		Msg2Other.SetSkillID(rImp.GetSkillID());

		// 设置技能效果的持续时间 20100531 BLL
		Msg2Other.SetContinuance( rImp.GetContinuance() );

		if(INVALID_ID!=Impact_GetImpactID(rImp))
		{
			pScene->BroadCast(&Msg2Other, this, TRUE);
		}

		// 消散的buff如果是引导技能支持的，中止引导技能
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
	//获得自身的效果列表
	_IMPACT_LIST& rList = Impact_GetImpactList();
	BYTE& rCount=rList.m_Count;
	ImpactLogic_T const* pCurrentImpactLogic = NULL;
	//遍历列表中的项
	for(short i = 0; rCount>i;++i)
	{
		OWN_IMPACT& rCurrentImpact = rList.m_aImpacts[i];
		pCurrentImpactLogic = Impact_GetLogic(rCurrentImpact);
		//未找到相应的逻辑对象，则进入效果消散逻辑
		if(NULL==pCurrentImpactLogic)
		{
			Impact_OnImpactFadeOut(rCurrentImpact);
			continue;
		}
		//如果当前状态不是消散，并且是持续性效果
		else if(FALSE==rCurrentImpact.IsFadeOut()&&TRUE==Impact_IsOverTimed(rCurrentImpact))
		{
			//如果当前效果和传入的效果属于同一个互斥组
			if((Impact_GetMutexID(rCurrentImpact)==Impact_GetMutexID(rImpactIn))&&(INVALID_ID!=Impact_GetMutexID(rCurrentImpact)))
			{
				//如果两个效果都排斥同一个人发出
				if(TRUE == Impact_IsImpactsABMutexed(rImpactIn, rCurrentImpact))
				{
					//如果可以用新效果替换旧的效果则替换效果
					if(TRUE == Impact_CanImpactAReplaceImpactB(rImpactIn, rCurrentImpact))
					{
						Impact_ReplaceImpact(rCurrentImpact, rImpactIn);
					}
					//否则向client发送未击中消息
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
					return TRUE;//完成替换操作,不需要新加效果了.
				}
			}
		}
	}
	return FALSE;//没有任何替换操作,仍然需要新加效果了.
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
	//延时技能生效处理
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
	//遍历延时中的效果列表
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

	//已经偷了的数量
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

