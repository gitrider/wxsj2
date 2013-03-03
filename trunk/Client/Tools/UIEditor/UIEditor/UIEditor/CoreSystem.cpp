#include "StdAfx.h"
#include "CoreSystem.h"
#include <CEGUI.h>
#include <CEGUIAnimateManager.h>
#include <OgreCEGUIRenderer.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreStringConverter.h>
#include <OgreResourceGroupManager.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>
#include <OgreSceneManager.h>
#include <OgreStaticGeometry.h>
#include <OgreStringConverter.h>
#include <OgreVector2.h>
#include <OgreException.h>
#include <OgreParticleSystemManager.h>
#include <OgreArchiveManager.h>
#include <OgreTextureManager.h>
#include <OgreMeshManager.h>
#include <OgreLogManager.h>
#include <OgreRenderQueueListener.h>

using namespace CEGUI;
using namespace Ogre;

CCoreSystem g_CoreSystem;

CCoreSystem::CCoreSystem(void) : m_pCeguiSystem(NULL), m_pOgreRender(NULL),m_pOgreRoot(NULL),
m_pRenderWindow(NULL),m_pMainCamera(NULL),m_pSceneMgr(NULL),m_pSceneViewport(NULL),m_pCeguiRootWindow(NULL)
{
	
}

CCoreSystem::~CCoreSystem(void)
{
}

bool CCoreSystem::initialize(HWND hWnd)
{
	if ( !initializeOgre(hWnd) )
	{
		return false;
	}
	if ( !initializeCEGUI(hWnd) )
	{
		return false;
	}
	return true;
}

void CCoreSystem::onSizeChange(void)
{
	if(m_pRenderWindow)
	{
		m_pRenderWindow->windowMovedOrResized();
	}
	if (m_pOgreRender)
	{
		m_pOgreRender->_notifySizeChanged();
	}
}

bool CCoreSystem::initializeOgre(HWND hWnd)
{
	#if (_MSC_VER >= 1400)
		setlocale( LC_CTYPE, "" );
	#endif

	m_pOgreRoot = new Root("");

	loadPlugis("");

	m_pOgreRoot->initialise(false, Ogre::StringUtil::BLANK);
	Ogre::NameValuePairList miscParams;
	miscParams["externalWindowHandle"] = Ogre::StringConverter::toString((Ogre::ulong)hWnd);
	miscParams["FSAA"] = Ogre::StringConverter::toString((Ogre::ulong)2);
	miscParams["vsync"] = "true";
	m_pRenderWindow = m_pOgreRoot->createRenderWindow(Ogre::StringUtil::BLANK, 0, 0, false, &miscParams);
	m_pRenderWindow->setActive(true);

	setupResource("UIEditorResources.cfg");

	createScene();
	createCamera();
	createViewport();
	
	return true;
}

void CCoreSystem::setupResource(const std::string& resourceFileName)
{
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(resourceFileName);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		const Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			bool recursive = false;
			if (archName[0] == '*')
			{
				archName.erase(0, 1);
				Ogre::StringUtil::trim(archName);
				recursive = true;
			}
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName, recursive);
		}
	}
}

void CCoreSystem::loadPlugis(const std::string& resourceFileName)
{
	m_pOgreRoot->loadPlugin("RenderSystem_Direct3D9.dll");
	m_pOgreRoot->setRenderSystem(m_pOgreRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem"));
	m_pOgreRoot->loadPlugin("Plugin_ParticleFX.dll");
	m_pOgreRoot->loadPlugin("Plugin_ParticleFX2.dll");
	m_pOgreRoot->loadPlugin("Plugin_OctreeSceneManager.dll");
	m_pOgreRoot->loadPlugin("Plugin_CgProgramManager.dll");
}





bool CCoreSystem::initializeCEGUI(HWND hWnd)
{
	m_pOgreRender = new OgreCEGUIRenderer(m_pRenderWindow,Ogre::RENDER_QUEUE_OVERLAY,false,0,m_pSceneMgr);
	m_pCeguiSystem = new CEGUI::System(m_pOgreRender);
	//使用UI资源组
	m_pCeguiSystem->getResourceProvider()->setDefaultResourceGroup("General");

	//加载总控文件
	SchemeManager::getSingleton().loadScheme("Schema\\WoWLookSkin.scheme.xml");

	m_pCeguiSystem->setTooltip("WoWLook/Tooltip");
	Window* pWindow = WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"__BACKGOUNDSHEET__");
	pWindow->setWindowArea(URect(cegui_reldim(0), cegui_reldim(0), cegui_reldim(1), cegui_reldim(1)));

	pWindow->setMouseHollow(true);
	pWindow->moveToFront();
	pWindow->setDistributesCapturedInputs(true);

	pWindow->setPosition(CEGUI::Absolute, Point(0,0));
	pWindow->setSize(CEGUI::Absolute, Size(800,600));

	m_pCeguiRootWindow = pWindow;

	System::getSingleton().setGUISheet( pWindow );

	//初始化动画管理器
	new CEGUI::AnimateManager( "schema\\wowanimate.xml");

	return true;
}
bool CCoreSystem::createScene(void)
{
	m_pSceneMgr = m_pOgreRoot->createSceneManager(Ogre::ST_GENERIC, "MainSceneMgr");
	assert(m_pSceneMgr);

	Ogre::RenderQueue* rq = m_pSceneMgr->getRenderQueue();
	// Disable shadows for all future groups
	//rq->setEnableShadowsForUnregisteredGroup(false);

	// Disable shadow for all registered render queue group
	Ogre::RenderQueue::QueueGroupIterator it = rq->_getQueueGroupIterator();
	while (it.hasMoreElements())
	{
		Ogre::uint8 qid = it.peekNextKey();
		Ogre::RenderQueueGroup* group = it.getNext();
		group->setShadowsEnabled(false);
	}
	return true;
}
bool CCoreSystem::createViewport(void)
{
	// Create one viewport, entire window
	m_pSceneViewport = m_pRenderWindow->addViewport(m_pMainCamera);
	m_pSceneViewport->setBackgroundColour(Ogre::ColourValue(0,0.5,0.5));

	if (!m_pMainCamera->getAutoAspectRatio())
	{
		// Alter the camera aspect ratio to match the viewport
		m_pMainCamera->setAspectRatio(
			Ogre::Real(m_pSceneViewport->getActualWidth()) / Ogre::Real(m_pSceneViewport->getActualHeight()));
	}
	return true;
}
bool CCoreSystem::createCamera(void)
{
	m_pMainCamera = m_pSceneMgr->createCamera("MainCamera");
	assert(m_pMainCamera);
	// Set camera near/far clip
	Ogre::Real s = 1;
	m_pMainCamera->setNearClipDistance(0.25f * s);//LM修改
	m_pMainCamera->setFarClipDistance(150.0f * s);
	// 45 degree fov
	m_pMainCamera->setFOVy(Ogre::Degree(45));
	// Auto aspect ratio
	m_pMainCamera->setAutoAspectRatio(true);
	return true;
}


bool CCoreSystem::render(unsigned long eslape)
{
	if(m_pCeguiSystem)
	{
		//m_pCeguiSystem->signalRedraw();
		m_pCeguiSystem->injectTimePulse((float)eslape);
	}
	if(m_pOgreRoot)
	{
		return m_pOgreRoot->renderOneFrame();
	}
	return false;
}

//退出清理
void CCoreSystem::cleanup(void)
{
	//删除cegui系统
	delete m_pCeguiSystem;
	m_pCeguiSystem = NULL;

	//删除render
	delete m_pOgreRender;
	m_pOgreRender = NULL;

	//清空各种指针
	m_pRenderWindow = NULL;
	m_pMainCamera = NULL;
	m_pSceneMgr = NULL;
	m_pSceneViewport = NULL;

	//删除ogre系统
	delete m_pOgreRoot;
	m_pOgreRoot = NULL;

}



void CCoreSystem::drawQuad(float x, float y, float w, float h, unsigned long color /*= 0xff0000ff*/)
{
	
}

CEGUI::Window* CCoreSystem::LoadLayOut(const CHAR* szName,CEGUI::WindowManager::EventCallBack func)
{
	try
	{
		AUTO_LOCALE_CONVERT local;
		if(m_pCeguiSystem->getSingleton().getGUISheet())
		{
			Window* pWindow = m_pCeguiSystem->getSingleton().getGUISheet();
			Window* pRetWindow = WindowManager::getSingleton().loadWindowLayout2(szName,"","",NULL,func);
			pWindow->addChildWindow(pRetWindow);
			return pRetWindow;
		}
	}
	catch (CEGUI::Exception& e)
	{
		std::string exceptName = "加载";
		exceptName += szName;
		exceptName += "\n描述:";
		exceptName += e.what();
		MessageBox(AfxGetMainWnd()->GetSafeHwnd(), exceptName.c_str() , "错误",MB_OK |MB_ICONWARNING);
	}
	catch (...)
	{
		std::string exceptName = "加载";
		exceptName += szName;
		exceptName += "失败!";
		MessageBox(AfxGetMainWnd()->GetSafeHwnd(), exceptName.c_str() , "错误",MB_OK |MB_ICONWARNING);
	}
	
	return NULL;
}

void CCoreSystem::setLayout(INT width)
{
	if (m_pCeguiRootWindow)
	{
		if (width == 800)
		{
			m_pCeguiRootWindow->setSize(CEGUI::Absolute, Size(800,600));
		}
		else if(width == 1024)
		{
			m_pCeguiRootWindow->setSize(CEGUI::Absolute, Size(1024,768));
		}
		else
		{
			m_pCeguiRootWindow->setSize(CEGUI::Relative, Size(1,1));
		}
	}
}