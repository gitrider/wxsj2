#include "StdAfx.h"
#include "UIMapLight.h"

#include <OgreRenderTarget.h>
#include <OgreRenderTexture.h>
#include <OgreRenderTargetListener.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreRectangle2D.h>
#include <OgreMaterialManager.h>
#include <OgreRenderSystem.h>
#include <OgreRoot.h>

#include "EngineInterface.h"
#include "../Object\Manager\FakeObjectEntityManager.h"
#include "GIException.h"

#include "Core/FairyFlags.h"
#include "Core/FairySystem.h"


class CUILightMapDeviceListener : public Ogre::RenderSystem::Listener
{
public:
	virtual void eventOccurred(const Ogre::String& eventName, 
			const Ogre::NameValuePairList* parameters = 0)
	{
		if(eventName == "DeviceRestored")
		{
			CUIMapLight::GetMe()->SetUpdate();
		}
	}
};

CUILightMapDeviceListener g_theDeviceListener;

//=====================================================================
//Ogre ViewPort��Ⱦ����
class CUILightMapRenderListener : public Ogre::RenderTargetListener
{
public:
	virtual void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
	{
		Ogre::SceneManager* pSceneManager = CEngineInterface::GetMe()->GetFairySystem()->getSceneManager();

		//----------------------------------------------------------
		//����VisibleFlag
		m_nRenderMask = pSceneManager->getVisibilityMask();
		pSceneManager->setVisibilityMask(Fairy::OVF_GUI_ELEMENTS);

		//----------------------------------------------------------
		//��ʾ̽��
		CUIMapLight::GetMe()->Show(true);
		CFakeObjectEntityManager::GetMe()->SetActiveObject(0);
	}

    virtual void postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
	{
		Ogre::SceneManager* pSceneManager = CEngineInterface::GetMe()->GetFairySystem()->getSceneManager();

		//----------------------------------------------------------
		//�ָ�VisibleFlag
		pSceneManager->setVisibilityMask(m_nRenderMask);

		//----------------------------------------------------------
		//����̽��
		CUIMapLight::GetMe()->Show(false);

		//ֹͣ��Ⱦ
		if(!m_ptrRenderTexture.isNull() && m_ptrRenderTexture->getBuffer()->getRenderTarget())
		{
			m_ptrRenderTexture->getBuffer()->getRenderTarget()->setAutoUpdated(false);
		}

#if 0
		{
			//static int index = 0;
			//if(index%100 == 0)
			{
				CHAR szFileName[MAX_PATH];
				_snprintf(szFileName, MAX_PATH, "c:\\maplight.tga");
				m_ptrRenderTexture->getBuffer()->getRenderTarget()->writeContentsToFile(szFileName);
			}
			//index++;//
		}
#endif

	}

public:
	//������ʱ���泡����Ⱦ����

	//Visible Flag
	unsigned int			m_nRenderMask;
	//������Ⱦ��Texture
	Ogre::TexturePtr		m_ptrRenderTexture;
};

CUILightMapRenderListener	g_theListener;

//////////////////////////////////////////////////////////////////////////
CUIMapLight* CUIMapLight::s_pMe = 0;
CUIMapLight::CUIMapLight() :
	m_pCamera(0),
	m_pViewPort(0),
	m_pRectange(0),
	m_pMaterial(0),
	m_fDirection(0.f),
	m_bNeedUpdate(true)
{
	s_pMe = this;

}

CUIMapLight::~CUIMapLight()
{
	s_pMe = 0;
}

void CUIMapLight::Initial(void)
{
	//--------------------------------------------------
	//����RenderTarget
	Ogre::TexturePtr ptrTex =
		Ogre::TextureManager::getSingleton().createManual(
			"MapLight_RenderTexture",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D,
			128, 128, 1, 0,
			Ogre::PF_R8G8B8A8,
			Ogre::TU_RENDERTARGET,
			0);

	ptrTex->load();
	m_ptrRenderTexture = ptrTex;
	ptrTex->getBuffer()->getRenderTarget()->addListener(&g_theListener);
	ptrTex->getBuffer()->getRenderTarget()->setAutoUpdated(false);
	g_theListener.m_ptrRenderTexture = ptrTex;

	//--------------------------------------------------
	//���������
	Ogre::SceneManager* pScnManager = CEngineInterface::GetMe()->GetFairySystem()->getSceneManager();
	m_pCamera = pScnManager->createCamera("MapLight_Camera");
	m_pCamera->setAspectRatio(1.0f);
	m_pCamera->setPosition(0.f, 0.f, 0.f);
	m_pCamera->lookAt(0.f, 100.f, 0.f);
	m_pCamera->setFarClipDistance(100);
	m_pCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

	//--------------------------------------------------
	//����ViewPort
	m_pViewPort = m_ptrRenderTexture->getBuffer()->getRenderTarget()->addViewport(m_pCamera, 1);
	m_pViewPort->setClearEveryFrame(true);
	m_pViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0,0));
	m_pViewPort->setOverlaysEnabled(false);
	m_pViewPort->setSkiesEnabled(false);
	m_pViewPort->setShadowsEnabled(false);

	//--------------------------------------------------
	//����Rectance2D
	m_pRectange = new Ogre::Rectangle2D(true);
	m_pRectange->setCorners(-1.0f, 1.0f, 1.0f, -1.0f);
	m_pRectange->setMaterial("MapSearchLight");
	m_pRectange->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
	m_pRectange->setVisibilityFlags(Fairy::OVF_GUI_ELEMENTS);
	CEngineInterface::GetMe()->GetFairySystem()->getBaseSceneNode()->attachObject(m_pRectange);

	//--------------------------------------------------
	//�������
	Ogre::MaterialPtr ptrMat = Ogre::MaterialManager::getSingleton().getByName("MapSearchLight");
	m_pMaterial = ptrMat.get();
	KLAssert(m_pMaterial);

	//--------------------------------------------------
	//�ҽ��豸Listener
	CEngineInterface::GetMe()->GetFairySystem()->getRoot()->getRenderSystem()->addListener(&g_theDeviceListener);
}

void CUIMapLight::Release(void)
{
	m_ptrRenderTexture.setNull();
	g_theListener.m_ptrRenderTexture.setNull();

	CEngineInterface::GetMe()->GetFairySystem()->getBaseSceneNode()->detachObject(m_pRectange);
	delete m_pRectange; m_pRectange = 0;
}

void CUIMapLight::Show(bool bShow)
{
	m_pRectange->setVisible(bShow);
}

void CUIMapLight::Update(void)
{
	if(m_bNeedUpdate && !m_ptrRenderTexture.isNull() && m_ptrRenderTexture->getBuffer()->getRenderTarget())
	{
		m_ptrRenderTexture->getBuffer()->getRenderTarget()->update();
		m_bNeedUpdate = false;
	}
}

void CUIMapLight::SetDirection(float fDir)
{
	if(m_pMaterial)
	{
		Ogre::TextureUnitState* texture = m_pMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);

		if (texture)
		{
			m_fDirection = fDir;
			texture->setTextureRotate(Ogre::Radian(fDir));

			m_bNeedUpdate = true;
		}

	}
}