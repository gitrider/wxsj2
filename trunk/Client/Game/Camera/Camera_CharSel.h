
#pragma once


#include "Camera.h"


/*
|	��������ѡ��ʱ�������
|
*/
class CCamera_CharSel : public CCamera
{
public:

	CCamera_CharSel(Ogre::Camera* pOgreCamera);
	virtual ~CCamera_CharSel();

public:

	virtual VOID		Update(VOID);
};