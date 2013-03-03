/*
	小地图探照灯功能
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
	//初始化
	void Initial(void);

	//释放
	void Release(void);

	//隐藏/显示
	void Show(bool bShow);

	//设置方向
	void SetDirection(float fDir);

	//刷新
	void Update(void);

	//设置需要更新
	void SetUpdate(void) { m_bNeedUpdate = true; }

public:
	Ogre::TexturePtr		m_ptrRenderTexture;		//用于渲染的Texture
	Ogre::Camera*			m_pCamera;				//摄像机
	Ogre::Viewport*			m_pViewPort;			//用于渲染的View
	Ogre::Rectangle2D*		m_pRectange;			//渲染模型
	Ogre::Material*			m_pMaterial;			//探灯材质
	float					m_fDirection;			//旋转方向
	bool					m_bNeedUpdate;			//是否需要更新

public:
	CUIMapLight();
	virtual ~CUIMapLight();

	static CUIMapLight* GetMe(void) { return s_pMe; }

protected:
	static CUIMapLight* s_pMe;
};
