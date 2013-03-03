

#include "StdAfx.h"
#include ".\bulletentitymanager.h"
#include "Global.h"
#include "ObjectManager.h"
#include "..\Logic\Character\Obj_Character.h"
#include "../../../GameInterface/Include/GIException.h"
#include "../../Procedure/GameProcedure.h"
#include "GITimeSystem.h"
#include "EffectSystem/FairyAnimationBulletFlow.h"
#include "Object/Entity/ObjEntityActor.h"
/*------------------------子弹的引擎回调事件---------------------------*/

// 技能子弹的创建回调
VOID BulletFlowCreateCallback::onCreateBulletFlow(const Fairy::String& bulletFlowTemplateName, 
												  const Fairy::TransformInfo& transInfo, void* pSkillUserData)
{
	if (pSkillUserData)
	{
		BulletInfo_Target* pTargetInfo = (BulletInfo_Target*)pSkillUserData;
		//检查目标数据
		for (UINT i=0; i<pTargetInfo->vTargetIDlist.size(); ++i)
		{
			// 目标有效
			CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject((INT)(pTargetInfo->vTargetIDlist[i]));
			if (!pObj)
			{	
				return;
			}
			// 创建一个子弹系统
			Fairy::BulletFlowSystem* pFlow = Fairy::BulletFlowSystemManager::getSingleton().createBulletFlowSystem( bulletFlowTemplateName );

			BulletFlow_Target* pBulletFlow_Target = new BulletFlow_Target();
			pBulletFlow_Target->nSenderObjID = pTargetInfo->nSenderObjID;
			pBulletFlow_Target->vTargetID = pTargetInfo->vTargetIDlist[i];
			pBulletFlow_Target->vTargetPos = pTargetInfo->vTargetPosList[i];

			pBulletFlow_Target->targetPoint = pTargetInfo->targetPoint;
			pBulletFlow_Target->offsetPoistion = pTargetInfo->offsetPoistion;
			pBulletFlow_Target->offsetRotation = pTargetInfo->offsetRotation;
			pBulletFlow_Target->attachPoint = pTargetInfo->attachPoint;
			// 起始位置
			pFlow->setPosition( transInfo.mPosition);
			pFlow->setRotation( transInfo.mRotation);

			// 创建子弹逻辑系统
			if (-1 == CBulletEntityManager::GetMe()->CreateBulletFlowSystem(pFlow, pBulletFlow_Target))
			{
				// 销毁该子弹系统
				Fairy::BulletFlowSystemManager::getSingleton().destroyBulletFlowSystem(pFlow);
			}
		}
		
	}
}


// 子弹创建回调
VOID CreatedBulletCallback::onAfterCreatedBulletSystem(Fairy::BulletFlowSystem* bulletFlow, Fairy::BulletSystem* bulletSystem)
{
	if (bulletFlow && bulletSystem && bulletFlow->m_userData)
	{		
		//// 设置目标列表ID
		//INT nID = *((INT*)bulletFlow->m_userData);

		//INT* pID = new INT;
		//*pID = nID;

		//bulletSystem->m_userData = (VOID*)pID;
	}
}

// 子弹销毁回调
VOID DeleteBulletCallback::onBeforeDeleteBulletSystem(Fairy::BulletSystem* bulletSystem)
{
	if (bulletSystem)
	{
	//	SAFE_DELETE(bulletSystem->m_userData);
	}
}

// 技能销毁回调
VOID SkillDestroyCallback::onDestroySkill(Fairy::Skill* pSkill)
{
	if (pSkill)
	{
		BulletInfo_Target* pInfo = (BulletInfo_Target*)pSkill->m_userData;
		SAFE_DELETE(pInfo);
	}
}

VOID BulletFindTargetCallback::onHitTargetCallback(Fairy::BulletFlowSystem* bulletFlow,Fairy::BulletSystem* bulletSystem)
{
	BulletFlow_Target* pTarget = (BulletFlow_Target*)bulletFlow->m_userData;
	ObjID_t TargetID = pTarget->vTargetID;

	CObject_Character* pChar = (CObject_Character*)CObjectManager::GetMe()->FindServerObject(TargetID);
	if (pChar)
	{
		pChar->OnBulletHit();
	}

}

VOID BulletCameraShakeCallback::onShakeCallBack(float amplitude,float duration,float frequency)
{
	CGameProcedure::s_pGfxSystem->Camera_VibrateOneTime(amplitude,frequency,duration);
}

/*---------------------------------------------------*/
/*---------------------------------------------------*/
/*
	子弹管理器
*/


CBulletEntityManager*				CBulletEntityManager::s_pMe = NULL;
BulletFlowCreateCallback			CBulletEntityManager::s_BulletFlowCreateCallback;
CreatedBulletCallback				CBulletEntityManager::s_CreatedBulletCallback;
DeleteBulletCallback				CBulletEntityManager::s_DeleteBulletCallback;
SkillDestroyCallback				CBulletEntityManager::s_SkillDestroyCallback;
BulletFindTargetCallback			CBulletEntityManager::s_BulletFindTargetCallBack;
BulletCameraShakeCallback			CBulletEntityManager::s_BulletCameraShakeCallback;


CBulletEntityManager::CBulletEntityManager(void)
{
	s_pMe		= this;
	mFrameTime  = 0; 
	Fairy::BulletFlowSystemManager::getSingleton().setCreateBulletFlowCallback( &s_BulletFlowCreateCallback );
	Fairy::BulletFlowSystemManager::getSingleton().setAfterCreatedBulletSystemCallback( &s_CreatedBulletCallback );
	Fairy::BulletFlowSystemManager::getSingleton().setBeforeDeleteBulletSystemCallback( &s_DeleteBulletCallback );
	Fairy::BulletFlowSystemManager::getSingleton().setBulletSystemHitTargetCallback(&s_BulletFindTargetCallBack);
	Fairy::BulletFlowSystemManager::getSingleton().setBulletCameraShakeCallback(&s_BulletCameraShakeCallback);
	

//	Fairy::EffectManager::getSingleton().setDestroySkillCallback( &s_SkillDestroyCallback );
}


// -----------------------------------
CBulletEntityManager::~CBulletEntityManager(void)
{	
	BulletFlowSystemList::iterator iter = m_BulletFlowSystemList.begin();

	while (iter != m_BulletFlowSystemList.end())
	{
		BulletFlowSystem_Logic* pFlow = iter->second;
		if(pFlow)
		{
			SAFE_DELETE(pFlow);
		}
		iter ++;
	}
	m_BulletFlowSystemList.clear();
}

// -----------------------------------

VOID CBulletEntityManager::Tick()
{
	UINT currentTime  = CGameProcedure::s_pTimeSystem->GetTimeNow();
	UINT timeSpace = currentTime - mFrameTime;
	BulletFlowSystemList::iterator iter = m_BulletFlowSystemList.begin();
	BulletFlowSystemList::iterator tempIter;

	while (iter != m_BulletFlowSystemList.end())
	{
		tempIter = iter;
		++iter;

		BulletFlowSystem_Logic* pFlow = tempIter->second;

		if (NULL == pFlow)
		{
			m_BulletFlowSystemList.erase(tempIter);
		}
		else
		{
			// 时间结束,删除
			if (TRUE == pFlow->IsTimeOver())
			{
				SAFE_DELETE(pFlow);
				m_BulletFlowSystemList.erase(tempIter);
			}
			// 子弹刷新
			else
			{
				pFlow->Tick(timeSpace);
			}			
		}
	}

	mFrameTime = currentTime;
}

// -----------------------------------
// 向技能内添加目标列表信息

VOID CBulletEntityManager::CreateSkillBulletInfo(Fairy::Skill* pSkill, INT nSenderID, const ObjID_t* pTargetIDList, INT nTargetNum)
{
	if (pSkill && pTargetIDList && 0 < nTargetNum)
	{
		BulletInfo_Target* pInfo = new BulletInfo_Target(nTargetNum);		

		pInfo->nSenderObjID = nSenderID;

		for (INT i=0; i<nTargetNum; ++i)
		{
			pInfo->vTargetIDlist[i] = pTargetIDList[i];
			if(pSkill->getNumAnimationBulletFlows()> 0)
			{
				Fairy::AnimationBulletFlow* pAnimationFlow = pSkill->getAnimationBulletFlow(0);
				pInfo->attachPoint = pAnimationFlow->getAttachPoint();
				pInfo->offsetPoistion = pAnimationFlow->getOffsetPos();
				pInfo->offsetRotation = pAnimationFlow->getOffsetRotation();
				pInfo->targetPoint = "身体受击点";
				
			}
		}	

		pSkill->m_userData = (VOID*)pInfo;
	}
}

// -----------------------------------

INT CBulletEntityManager::CreateBulletFlowSystem(Fairy::BulletFlowSystem* pBulletSystem, const BulletFlow_Target* pTarget)
{
	if (NULL == pBulletSystem || NULL == pTarget)
		return -1;

	static INT nListID = 0;

	INT nID = nListID++;

	BulletFlowSystemList::iterator iter = m_BulletFlowSystemList.find(nID);

	if (iter == m_BulletFlowSystemList.end())
	{
		BulletFlowSystem_Logic* pFlow = new BulletFlowSystem_Logic(pBulletSystem, pTarget);

		m_BulletFlowSystemList.insert(std::make_pair(nID, pFlow));		
	}	
	else
	{
		KLAssert(!"CreateBulletFlowSystem() Error!!!");
		nID = -1;
	}

	return nID;
}

// -----------------------------------

CObject_Bullet* CBulletEntityManager::CreateBulletEffect( SObject_BulletInit* infoBullet )
{
	CObject_Bullet *pBullet = (CObject_Bullet*)CObjectManager::GetMe()->NewObject( "CObject_Bullet");
	if (pBullet)
		pBullet->Initial( infoBullet );

	return pBullet;
}



// -----------------------------------

CObject_Bullet* CBulletEntityManager::CreateBulletEffect( int nBulletID, 
														  int nSenderSeverID, int nTargetSeverID, 
														  fVector3* pDesPos, fVector3* pTargetPos )
{
	SObject_BulletInit infoBullet;

	infoBullet.m_nBulletID = nBulletID;

	// 设置起始位置
	if (-1 != nSenderSeverID)
	{
		infoBullet.m_nSendID = nSenderSeverID;

		CObject *pSender = (CObject*)(CObjectManager::GetMe()->FindServerObject(nSenderSeverID));
		if (pSender && g_theKernel.IsKindOf(pSender->GetClass(), GETCLASS(CObject_Character)))
		{
			CObject_Character *pCharacterSender = (CObject_Character*)(pSender);

			infoBullet.m_fvPos = pCharacterSender->GetPosition();
			infoBullet.m_fvRot = pCharacterSender->GetPosition();
			if (pCharacterSender->GetRenderInterface())
				pCharacterSender->GetRenderInterface()->Actor_GetLocator("身体受击点", infoBullet.m_fvPos);
		}		
	}
	else if (NULL != pDesPos)
	{
		infoBullet.m_fvPos = *pDesPos;
	}

	// 设置目标位置
	if (-1 != nTargetSeverID)
	{
		infoBullet.m_nTargetID = nTargetSeverID;
	}
	else if (NULL != pTargetPos)
	{
		infoBullet.m_fvTargetPos = *pTargetPos;
	}


	// 创建子弹
	CObject_Bullet *pBullet = (CObject_Bullet*)CObjectManager::GetMe()->NewObject("CObject_Bullet");
	if (pBullet)
	{
		// 初始化
		pBullet->Initial( &infoBullet );		
	}

	return pBullet;
}


// -----------------------------------

BOOL CBulletEntityManager::DestroyBulletByID(int nID)
{
	tObject* pObj = CObjectManager::GetMe()->FindObject(nID);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Bullet)))
	{
		CObject_Bullet *pBullet = (CObject_Bullet*)pObj;
		if (pBullet)
		{
			pBullet->Destroy();
			return true;
		}
	}

	return false;
}

// -----------------------------------

BOOL CBulletEntityManager::DestroyBulletByObj( CObject_Bullet* pBullet )
{
	if (pBullet)
	{
		pBullet->Destroy();
		return true;
	}
	return false;
}


// -----------------------------------

CObject_Bullet* CBulletEntityManager::GetBulletByID( int nID )
{
	tObject* pObj = CObjectManager::GetMe()->FindObject(nID);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Bullet)))
	{
		return  (CObject_Bullet*)pObj;
	}

	return NULL;
}


// -----------------------------------

CObject_Bullet* CBulletEntityManager::GetBulletBySeverID( int nSeverID )
{
	tObject* pObj = CObjectManager::GetMe()->FindServerObject(nSeverID);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Bullet)))
	{
		return (CObject_Bullet*)pObj;
	}

	return NULL;
	
}


VOID	CBulletEntityManager::OnObjectDestroy(INT nID)
{

	BulletFlowSystemList::iterator iter = m_BulletFlowSystemList.begin();
	BulletFlowSystemList::iterator tempIter;

	while (iter != m_BulletFlowSystemList.end())
	{
		tempIter = iter;
		++iter;

		BulletFlowSystem_Logic* pFlow = tempIter->second;
		

		if (NULL == pFlow)
		{
			m_BulletFlowSystemList.erase(tempIter);
		}
		else
		{
			Fairy::BulletFlowSystem* pBulletFlowSystem = pFlow->getFlowSystem();
			BulletFlow_Target* pTarget = (BulletFlow_Target*)pBulletFlowSystem->m_userData;
			// 时间结束,删除
			if (pTarget->vTargetID == nID)
			{
				SAFE_DELETE(pFlow);
				m_BulletFlowSystemList.erase(tempIter);
			}
		}
	}

}

// -----------------------------------

VOID CBulletEntityManager::CreateExpEffect(int nExp, int nSenderSeverID, int nTargetSeverID)
{
	SObject_BulletInit infoBullet;	
	
	int nNum = 0;
	int nBulletID = 1;

	if (nExp > 300000)
	{
		nNum = 1;//经验太大超出范围
		nBulletID = 1;
	}
	else if (nExp > 10000)
	{
		nNum = nExp / 10000;
		nBulletID = 1;
	}					
	else if (nExp > 1000)
	{
		nNum = nExp / 1000;
		nBulletID = 1;
	}
	else if (nExp > 100)
	{
		nNum = nExp / 100;
		nBulletID = 1;
	}
	else if (nExp > 10)
	{
		nNum = nExp / 10;
		nBulletID = 1;
	}
	else if (nExp >= 0)
	{
		nNum = nExp;
		nBulletID = 1;
	}
	// 负数情况
	else
	{
		nNum = 0;
	}	
	
	// 创建子弹
	for(int i = 0; i < nNum; ++i)
	{
		CObject_Bullet* pBullet = CreateBulletEffect(nBulletID, nSenderSeverID, nTargetSeverID);

		float fAngle = (float)(rand() % 90) - 45.0f;
		pBullet->SetAngle( fAngle );

		float fSpeed = (float)(rand() % 4) + 2.0f;
		pBullet->SetSpeed( fSpeed );

		float fAcceleration = (float)(rand() % 10) * 0.1f + 1.0f;
		pBullet->SetAcceleration( fAcceleration );

		pBullet->SetContrailType( BULLET_CONTRAIL_TYPE_EXP );
	}
}

/*---------------------------------------------------*/
/*---------------------------------------------------*/
/*
	子弹信息
*/
// -----------------------------------
BulletInfo_Target::BulletInfo_Target(UINT uNum)
	: nIndex(-1)
{
	nType = BULLETINFOTYPE_TARGET_AND_POS;
	
	vTargetIDlist.resize(uNum);
	vTargetPosList.resize(uNum);
}

// -----------------------------------
BulletInfo_Target::BulletInfo_Target(const BulletInfo_Target* pTargetInfo)
	: nIndex(-1)
{
	nType = BULLETINFOTYPE_TARGET_AND_POS;

	if (pTargetInfo)
	{
		nSenderObjID = pTargetInfo->nSenderObjID;

		vTargetIDlist  = pTargetInfo->vTargetIDlist;
		vTargetPosList = pTargetInfo->vTargetPosList;

		targetPoint = pTargetInfo->targetPoint;
		offsetPoistion = pTargetInfo->offsetPoistion;
		offsetRotation = pTargetInfo->offsetRotation;
		attachPoint = pTargetInfo->attachPoint;
	}	
}

// -----------------------------------
BulletInfo_Target::~BulletInfo_Target()
{
	vTargetIDlist.clear();
	vTargetPosList.clear();
}

BulletFlow_Target::BulletFlow_Target()
{

}
BulletFlow_Target::BulletFlow_Target(const BulletFlow_Target* pInfo)
{

	vTargetID = pInfo->vTargetID;
	vTargetPos = pInfo->vTargetPos;
	nSenderObjID = pInfo->nSenderObjID;
	vSenderPos = pInfo->vSenderPos;
	targetPoint = pInfo->targetPoint;
	offsetPoistion = pInfo->offsetPoistion;
	offsetRotation = pInfo->offsetRotation;
	attachPoint = pInfo->attachPoint;
	
}
BulletFlow_Target::~BulletFlow_Target()
{

}
/*---------------------------------------------------*/
/*---------------------------------------------------*/
/*
	子弹系统逻辑
*/


BulletFlowSystem_Logic::BulletFlowSystem_Logic(Fairy::BulletFlowSystem* pBulletFlow, const BulletFlow_Target* pTarget)
  : m_nID(-1), m_pBulletFlow(NULL)
{	
	if (NULL == pBulletFlow || NULL == pTarget)
	{
		KLAssert(!"BulletFlowSystem_Logic(); Error???");
		return;
	}

	m_pBulletFlow = pBulletFlow;

	// 创建目标信息
	BulletFlow_Target* pNewTarget = new BulletFlow_Target(pTarget);
	// 存入子弹
	m_pBulletFlow->m_userData = (VOID*)pNewTarget;
	m_pBulletFlow->setTargetPoint(pTarget->targetPoint);
	m_pBulletFlow->setAttachPoint(pTarget->attachPoint);
	m_pBulletFlow->setOffsetPosition(pTarget->offsetPoistion);
	m_pBulletFlow->setOffsetRotation(pTarget->offsetRotation);


	// 设置子弹目标

	// 目标有效
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject((INT)(pNewTarget->vTargetID));
	if (pObj)
	{
	
		tEntityNode* pRenderInterface = pObj->GetRenderInterface();

		if(pRenderInterface != NULL)
		{	
			if(pRenderInterface->GetType() == tEntityNode::ETYPE_ACTOR)
			{
				CObjEntityActor* pActor = (CObjEntityActor*)pRenderInterface;
				Fairy::LogicModel* pModel = pActor->GetActorImpl();
				if(pModel)
				{
					m_pBulletFlow->setTargetModel(pModel);
				}			
			}	
		}
							
	}
	CObject* pSenderObj = (CObject*)CObjectManager::GetMe()->FindServerObject((INT)(pNewTarget->nSenderObjID));
	if(pSenderObj)
	{
		tEntityNode* pRenderInterface = pSenderObj->GetRenderInterface();

		if(pRenderInterface != NULL)
		{	
			if(pRenderInterface->GetType() == tEntityNode::ETYPE_ACTOR)
			{
				CObjEntityActor* pActor = (CObjEntityActor*)pRenderInterface;
				Fairy::LogicModel* pModel = pActor->GetActorImpl();
				if(pModel)
				{
					m_pBulletFlow->setCasterModel(pModel);
				}			
			}	
		}
	}


}

// -----------------------------------

BulletFlowSystem_Logic::BulletFlowSystem_Logic(Fairy::BulletFlowSystem* pBulletFlow, ObjID_t nTargetObjID)
	: m_nID(-1), m_pBulletFlow(NULL)	
{
	if (NULL == pBulletFlow)
	{
		KLAssert(!"BulletFlowSystem_Logic(); Error!!!");
		return;
	}

	m_pBulletFlow = pBulletFlow;

	// 初始化一个目标的信息
	BulletInfo_Target* pTargetInfo = new BulletInfo_Target(1);
	// 存入子弹
	m_pBulletFlow->m_userData = (VOID*)pTargetInfo;	

	// 存储目标ID
	pTargetInfo->vTargetIDlist[0] = nTargetObjID;

	// 目标有效
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(nTargetObjID);
	if (pObj)
	{
		tEntityNode* pRenderInterface = pObj->GetRenderInterface();

		if(pRenderInterface != NULL)
		{	
			if(pRenderInterface->GetType() == tEntityNode::ETYPE_ACTOR)
			{
				CObjEntityActor* pActor = (CObjEntityActor*)pRenderInterface;
				Fairy::LogicModel* pModel = pActor->GetActorImpl();
				if(pModel)
				{
					m_pBulletFlow->setTargetModel(pModel);
				}			
			}	
		}	
	}
}

// -----------------------------------

BulletFlowSystem_Logic::~BulletFlowSystem_Logic(VOID)
{
	if (m_pBulletFlow)
	{
		// 销毁目标列表
		BulletInfo* pBulletInfo = (BulletInfo*)m_pBulletFlow->m_userData;
		SAFE_DELETE(pBulletInfo);

		// 销毁该子弹系统
		Fairy::BulletFlowSystemManager::getSingleton().destroyBulletFlowSystem(m_pBulletFlow);
	}	
}

// -----------------------------------

VOID BulletFlowSystem_Logic::Tick(UINT time)
{
	if (NULL == m_pBulletFlow)
	{
		KLThrow("BulletFlowSystem_Logic::Tick();  Error1!!!");
		return;
	}
	if (NULL == m_pBulletFlow->m_userData)
	{
		KLThrow("BulletFlowSystem_Logic::Tick();  Error2!!!");
		return;
	}


	m_pBulletFlow->update(float(time) * 0.001);
}

// -----------------------------------
BOOL BulletFlowSystem_Logic::IsTimeOver()
{
	if (m_pBulletFlow)
	{
		if (m_pBulletFlow->getAge() < m_pBulletFlow->getMaxTime())
		{
			return FALSE;
		}
	}
	return TRUE;
}
















