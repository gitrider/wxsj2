
#pragma once

/*
|
|	��������˳��ӽ������
|
|
|       CameraEye -> * _______
|                  �u �v�s	Pitch
|                   �I  �v      
|                 Ditance �v    
|                       �K  �K  
|                          �u o <-  CameraLookAt (PlayerHead)
|                            _|_
|                             |   <-PlayerMySelf
|                            / \
|        ---------------------+----------------------
|                              �I
|                                �v PlayerPos
|
*/

namespace Ogre
{
	class Camera;
}

class CCamera
{
public:

	virtual VOID		SetLookAt(const fVector3& fvPos);
	virtual fVector3	GetGameLookAt( VOID )			{ return m_fvGameLookatPos; }

	virtual VOID		SetPitch(FLOAT fPitch);
	virtual FLOAT		GetPitch(VOID) const			{ return m_fPitch; }

	virtual VOID		SetApprochPitch( FLOAT fPitch );
	virtual FLOAT		GetApprochPitch( VOID ) const	{ return m_fPitchApproch; }
	
	virtual VOID		SetDistance(FLOAT fDistance);
	virtual FLOAT		GetDistance(VOID) const			{ return m_fDistance; }

	virtual VOID		SetDirection(FLOAT fDirection);
	virtual FLOAT		GetDirection(VOID) const		{ return m_fDirection; }

public:

	Ogre::Camera*		GetOgreCamera(VOID)				{ return m_pOgreCamera; }
	virtual VOID		Update(VOID);

public:

	CCamera(Ogre::Camera* pOgreCamera);
	virtual ~CCamera();

protected:

	/// �������ע�ӵ�λ�� (��Ⱦϵͳ����)
	fVector3			m_fvLookatPos;
	/// �������ע�ӵ�λ�� (��Ϸϵͳ����)
	fVector3			m_fvGameLookatPos;
	/// �����������ˮƽ�ߵļн�(����)
	FLOAT				m_fPitch;
	FLOAT				m_fPitchApproch;
	/// ������۾�����ע��λ�õľ���(��Ⱦϵͳ����)
	FLOAT				m_fDistance;
	/// �������������������x��ĽǶ�
	FLOAT				m_fDirection;
	/// Ogre���
	Ogre::Camera*		m_pOgreCamera;

};


