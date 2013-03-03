
#include "StdAfx.h"
#include "../Engine/EngineInterface.h"
#include "Camera.h"
#include <OgreCamera.h>
#include <OgreRoot.h>
#include "GIUtil.h"
#include "GIException.h"



//============================================================================================

CCamera::CCamera(Ogre::Camera* pOgreCamera) : m_pOgreCamera(pOgreCamera)
{
	KLAssert(pOgreCamera);

	//-----------------------------------------------
	//���ӵ�Ogre�������
	m_pOgreCamera = pOgreCamera;
//	m_pOgreCamera->setNearClipDistance(0.01f);
}

CCamera::~CCamera()
{
}


VOID CCamera::SetLookAt(const fVector3& fvPos)
{
	m_fvLookatPos = fvPos;
	Update();

}

VOID CCamera::SetPitch(FLOAT fPitch)
{
	m_fPitch = fPitch;
	Update();
}

VOID CCamera::SetApprochPitch( FLOAT fPitch )
{
	m_fPitchApproch = fPitch;
}


VOID CCamera::SetDistance(FLOAT fDistance)
{
	m_fDistance = fDistance;
	Update();
}

VOID CCamera::SetDirection(FLOAT fDirection)
{
	m_fDirection = fDirection;
	Update();
}

VOID CCamera::Update(VOID)
{
	// ������۾����ڵ�λ��
	FLOAT y		= m_fDistance * ::sin(-m_fPitch);
	FLOAT r_xy	= m_fDistance * ::cos(-m_fPitch);
	FLOAT x		= -r_xy * ::sin(m_fDirection);
	FLOAT z		= -r_xy * ::cos(m_fDirection);

	fVector3 vEyePos(	m_fvLookatPos.x + x, 
						m_fvLookatPos.y + y,
						m_fvLookatPos.z + z);

	// ����OGRE���
	m_pOgreCamera->setPosition(Ogre::Vector3(vEyePos.x, vEyePos.y, vEyePos.z));
	m_pOgreCamera->lookAt(Ogre::Vector3(m_fvLookatPos.x, m_fvLookatPos.y, m_fvLookatPos.z));

}

