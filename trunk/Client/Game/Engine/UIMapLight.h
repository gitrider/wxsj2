/*
	С��ͼ̽�յƹ���
*/
#pragma once

#include "GIGfxSystem.h"
#include <OgreTexture.h>

namespace Ogre
{
	class Camera;
	class Viewport;
	class RenderTexture;
	class Rectangle2D;
	class Material;
};

class CUIMapLight
{
public:
	//��ʼ��
	void Initial(void);

	//�ͷ�
	void Release(void);

	//����/��ʾ
	void Show(bool bShow);

	//���÷���
	void SetDirection(float fDir);

	//ˢ��
	void Update(void);

	//������Ҫ����
	void SetUpdate(void) { m_bNeedUpdate = true; }

public:
	Ogre::TexturePtr		m_ptrRenderTexture;		//������Ⱦ��Texture
	Ogre::Camera*			m_pCamera;				//�����
	Ogre::Viewport*			m_pViewPort;			//������Ⱦ��View
	Ogre::Rectangle2D*		m_pRectange;			//��Ⱦģ��
	Ogre::Material*			m_pMaterial;			//̽�Ʋ���
	float					m_fDirection;			//��ת����
	bool					m_bNeedUpdate;			//�Ƿ���Ҫ����

public:
	CUIMapLight();
	virtual ~CUIMapLight();

	static CUIMapLight* GetMe(void) { return s_pMe; }

protected:
	static CUIMapLight* s_pMe;
};
