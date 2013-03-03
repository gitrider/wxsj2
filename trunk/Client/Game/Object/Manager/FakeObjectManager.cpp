
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

	// ����Ѿ���������ֵ�������,�Ͳ�������
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

		// �ҵ���ʾ��ģ�͵�UI����
		std::map< STRING, FakeObject* >::iterator itWin = m_listFakeUI.find(obj.strWindowName);
		if(itWin != m_listFakeUI.end())
		{
			// �Ͽ�����Ⱦ�㡢UI������ӣ����ͷ���Ӧ��Դ
			OnUIUnHook(itWin->first.c_str());
		}

		// �����߼�������
		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(obj.nObjectID);
		if(pObj)
		{
			pObj->Release();
			CObjectManager::GetMe()->DestroyObject(pObj);
		}

		// ���б���ɾ��
		m_listFakeMap.erase(it);
	}
}

VOID CFakeObjSystem::OnUIHook(LPCTSTR szWindowName, LPCTSTR szObjName,  int nTexWidth, int nTexHeight, LPCTSTR szBackgroundName)
{
	// �쿴�Ƿ�ô����Ѿ�����ʾ
	std::map< STRING, FakeObject* >::iterator itWin;
	itWin = m_listFakeUI.find(szWindowName);
	if(itWin != m_listFakeUI.end())
	{
		FakeObject* obj = itWin->second;

		// һ����ģ��
		if(itWin->second->strName == szObjName) 
			return;

		// �������ظô��������е�ģ��
		OnUIUnHook(szWindowName);
	}

	if( 0 == strcmp(szObjName, "") )
		return ;

	// �ҵ���ǰ������FakeObj
	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find(szObjName);
	if(it == m_listFakeMap.end()) 
		return;
	
	FakeObject& obj = it->second;

	// Ŀǰ��֧��һ��ģ�ͱ����������ʾ
	KLAssert(obj.strWindowName.empty());
	
	// ȷ����Ⱦ�������Ѿ�����
	CObject* pObjPoint = (CObject*)CObjectManager::GetMe()->FindObject(obj.nObjectID);
	KLAssert(pObjPoint && pObjPoint->GetRenderInterface());


	// ֪ͨ��Ⱦ�㣬������Ⱦ����
	CGameProcedure::s_pGfxSystem->FakeObj_Create(szObjName, pObjPoint->GetRenderInterface(), g_theKernel.IsKindOf(pObjPoint->GetClass(), GETCLASS(CObject_Character)) ? ((CObject_Character*)pObjPoint)->GetMountRenderInterface() : 0,
		obj.strCameraName.c_str() ,nTexWidth,nTexHeight, szBackgroundName);
			
	// ֪ͨUI������Ӧ��Image
	if(CGameProcedure::s_pUISystem)
		CGameProcedure::s_pUISystem->FakeObj_Show(szWindowName, szObjName);

	// �����б�
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

	// ֪ͨUI
	if(CGameProcedure::s_pUISystem)
		CGameProcedure::s_pUISystem->FakeObj_Close(szWindowName, obj.strName.c_str());

	// ɾ����Ⱦ���е���Ⱦ����
	CGameProcedure::s_pGfxSystem->FakeObj_Destroy(obj.strName.c_str());

	// ���б���ɾ��
	obj.strWindowName.clear();
	m_listFakeUI.erase(itWin);
}

VOID CFakeObjSystem::OnUIShown(LPCTSTR szWindowName, float fAspectRatio)
{
	std::map< STRING, FakeObject* >::iterator it = m_listFakeUI.find(szWindowName);
	if(it == m_listFakeUI.end()) 
		return;

	// ֪ͨ��Ⱦ�㣬��ʼ��Ⱦ��ģ��
	CGameProcedure::s_pGfxSystem->FakeObj_Show(it->second->strName.c_str(), TRUE, fAspectRatio);
}

VOID CFakeObjSystem::OnUIHiden(LPCTSTR szWindowName)
{
	std::map< STRING, FakeObject* >::iterator it = m_listFakeUI.find(szWindowName);
	if(it == m_listFakeUI.end()) 
		return;

	// ֪ͨ��Ⱦ�㣬ֹͣ��Ⱦ��ģ��
	CGameProcedure::s_pGfxSystem->FakeObj_Show(it->second->strName.c_str(), FALSE, 0.0f);
}

STRING CFakeObjSystem::GetObjName(LPCTSTR szWindowName)
{
	std::map< STRING, FakeObject* >::iterator it = m_listFakeUI.find(szWindowName);
	if(it == m_listFakeUI.end()) 
		return NULL;

	// ����ģ������
	return it->second->strName;
}

VOID CFakeObjSystem::RotateObj_Begin(LPCTSTR szObjName, FLOAT fAngle)
{
	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find(szObjName);
	if(it == m_listFakeMap.end()) 
		return;

	// ��ʼ��תģ��
	m_pRotateObj = &(it->second);
	m_fRotateSpeed = fAngle;
}

VOID CFakeObjSystem::RotateObj_End(LPCTSTR szObjName)
{
	// ֹͣ��תģ��
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
