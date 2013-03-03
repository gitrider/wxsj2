
/**	FakeObjectManager.cpp
*/

#include "StdAfx.h"
#include "FakeObjectManager.h"
#include "ObjectManager.h"
#include "..\Logic\object.h"
#include "..\Logic\Character\Obj_Character.h"
#include "Procedure\GameProcedure.h"
#include "GIGfxSystem.h"
#include "GIException.h"
#include "GIUISystem.h"
#include "Global.h"


KL_IMPLEMENT_DYNAMIC(CFakeObjSystem, GETCLASS(tFakeObjSystem));

CFakeObjSystem* CFakeObjSystem::s_pMe = NULL;



CFakeObjSystem::CFakeObjSystem()
{
	s_pMe = this;
	m_pRotateObj = NULL;
	m_fRotateSpeed = 0.0f;
}

CFakeObjSystem::~CFakeObjSystem()
{
}


tObject* CFakeObjSystem::NewFakeObject(LPCTSTR szClassName, LPCTSTR szObjName, LPCTSTR szCameraName)
{ 
	//MessageBox( 0, szObjName, "CFakeObjSystem::NewFakeObject", 0);

	// 如果已经有这个名字的物体了,就不创建了
	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find( szObjName );
	if(it != m_listFakeMap.end())
	{
		KLThrow("Fake Object Multi define(%s)", szObjName);
	}

	tObject* pNewNode = CObjectManager::GetMe()->NewObject( szClassName );
	((CObject*)pNewNode)->SetFakeObjectFlag(TRUE);

	FakeObject newFakeObj;
	newFakeObj.bVisible = false;
	newFakeObj.nObjectID= pNewNode->GetID();
	newFakeObj.strName	= szObjName;
	newFakeObj.strCameraName = szCameraName;

	m_listFakeMap.insert(std::make_pair(szObjName, newFakeObj));

	SObjectInit init;
	init.m_fvPos.x = init.m_fvPos.y = init.m_fvPos.z = -100;
	pNewNode->Initial(&init);

	return pNewNode; 
}

VOID CFakeObjSystem::DestroyObject( LPCTSTR szObjName )
{
	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find(szObjName);
	if(it!=m_listFakeMap.end())
	{
		FakeObject& obj = it->second;

		// 找到显示该模型的UI窗口
		std::map< STRING, FakeObject* >::iterator itWin = m_listFakeUI.find(obj.strWindowName);
		if(itWin != m_listFakeUI.end())
		{
			// 断开和渲染层、UI层的连接，并释放相应资源
			OnUIUnHook(itWin->first.c_str());
		}

		// 销毁逻辑层数据
		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(obj.nObjectID);
		if(pObj)
		{
			pObj->Release();
			CObjectManager::GetMe()->DestroyObject(pObj);
		}

		// 从列表中删除
		m_listFakeMap.erase(it);
	}
}

VOID CFakeObjSystem::OnUIHook(LPCTSTR szWindowName, LPCTSTR szObjName,  int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName)
{
	// 察看是否该窗口已经有显示
	std::map< STRING, FakeObject* >::iterator itWin;
	itWin = m_listFakeUI.find(szWindowName);
	if(itWin != m_listFakeUI.end())
	{
		FakeObject* obj = itWin->second;

		// 一样的模型
		if(itWin->second->strName == szObjName) 
			return;

		// 首先隐藏该窗口上已有的模型
		OnUIUnHook(szWindowName);
	}

	if( 0 == strcmp(szObjName, "") )
		return ;

	// 找到先前创建的FakeObj
	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find(szObjName);
	if(it == m_listFakeMap.end()) 
		return;
	
	FakeObject& obj = it->second;

	// 目前不支持一个模型被多个窗口显示
	KLAssert(obj.strWindowName.empty());
	
	// 确认渲染层物体已经创建
	CObject* pObjPoint = (CObject*)CObjectManager::GetMe()->FindObject(obj.nObjectID);
	KLAssert(pObjPoint && pObjPoint->GetRenderInterface());


	// 通知渲染层，创建渲染纹理
	CGameProcedure::s_pGfxSystem->FakeObj_Create(szObjName, pObjPoint->GetRenderInterface(), g_theKernel.IsKindOf(pObjPoint->GetClass(), GETCLASS(CObject_Character)) ? ((CObject_Character*)pObjPoint)->GetMountRenderInterface() : 0,
		obj.strCameraName.c_str() ,nTexWidth,nTexHeight, szBackgroundName);
			
	// 通知UI创建相应的Image
	if(CGameProcedure::s_pUISystem)
		CGameProcedure::s_pUISystem->FakeObj_Show(szWindowName, szObjName);

	// 加入列表
	obj.strWindowName = szWindowName;
	m_listFakeUI.insert(std::make_pair(szWindowName, &obj));
}

VOID CFakeObjSystem::OnUIUnHook(LPCTSTR szWindowName)
{
	std::map< STRING, FakeObject* >::iterator itWin;
	itWin = m_listFakeUI.find(szWindowName);
	if(itWin == m_listFakeUI.end()) 
		return;

	FakeObject& obj = *(itWin->second);

	// 通知UI
	if(CGameProcedure::s_pUISystem)
		CGameProcedure::s_pUISystem->FakeObj_Close(szWindowName, obj.strName.c_str());

	// 删除渲染层中的渲染纹理
	CGameProcedure::s_pGfxSystem->FakeObj_Destroy(obj.strName.c_str());

	// 从列表中删除
	obj.strWindowName.clear();
	m_listFakeUI.erase(itWin);
}

VOID CFakeObjSystem::OnUIShown(LPCTSTR szWindowName, float fAspectRatio)
{
	std::map< STRING, FakeObject* >::iterator it = m_listFakeUI.find(szWindowName);
	if(it == m_listFakeUI.end()) 
		return;

	// 通知渲染层，开始渲染该模型
	CGameProcedure::s_pGfxSystem->FakeObj_Show(it->second->strName.c_str(), TRUE, fAspectRatio);
}

VOID CFakeObjSystem::OnUIHiden(LPCTSTR szWindowName)
{
	std::map< STRING, FakeObject* >::iterator it = m_listFakeUI.find(szWindowName);
	if(it == m_listFakeUI.end()) 
		return;

	// 通知渲染层，停止渲染该模型
	CGameProcedure::s_pGfxSystem->FakeObj_Show(it->second->strName.c_str(), FALSE, 0.0f);
}

STRING CFakeObjSystem::GetObjName(LPCTSTR szWindowName)
{
	std::map< STRING, FakeObject* >::iterator it = m_listFakeUI.find(szWindowName);
	if(it == m_listFakeUI.end()) 
		return NULL;

	// 返回模型名字
	return it->second->strName;
}

VOID CFakeObjSystem::RotateObj_Begin(LPCTSTR szObjName, FLOAT fAngle)
{
	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find(szObjName);
	if(it == m_listFakeMap.end()) 
		return;

	// 开始旋转模型
	m_pRotateObj = &(it->second);
	m_fRotateSpeed = fAngle;
}

VOID CFakeObjSystem::RotateObj_End(LPCTSTR szObjName)
{
	// 停止旋转模型
	m_pRotateObj = NULL;
}

VOID CFakeObjSystem::Initial(VOID*)
{
}

VOID CFakeObjSystem::Tick(VOID)
{
	if(m_pRotateObj)
	{
		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(m_pRotateObj->nObjectID);
		if(!pObj) 
			return;

		FLOAT fAngleNow = pObj->GetFaceDir();
		pObj->SetFaceDir(fAngleNow+m_fRotateSpeed);
	}
}


VOID CFakeObjSystem::Release(VOID)
{
}
