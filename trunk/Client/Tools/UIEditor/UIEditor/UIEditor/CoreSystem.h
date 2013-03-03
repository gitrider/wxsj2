/*****************************************************************************
文件:	CoreSystem
作者:	方远
邮箱:	fangyuanning@126.com
说明:	初始化各种系统Ogre,CEGUI等
******************************************************************************/
#pragma once

namespace CEGUI
{
	class System;
	class OgreCEGUITexture;
	class OgreCEGUIRenderer;
};

namespace Ogre
{
	class Root;
	class RenderWindow;
	class Camera;
	class SceneManager;
	class Viewport;
};

#include <string>
#include <CEGUIWindowManager.h>

class CCoreSystem
{
public:
	CCoreSystem(void);
	~CCoreSystem(void);

public:
	bool initialize(HWND hWnd);
	bool render(unsigned long eslape);
	void cleanup(void);
	void onSizeChange(void);
	void drawQuad(float x, float y, float w, float h, unsigned long color = 0xff0000ff);
	void setLayout(INT width);

public:
	CEGUI::Window* LoadLayOut(const CHAR* szName,CEGUI::WindowManager::EventCallBack func);

protected:
	bool initializeOgre(HWND hWnd);
	void setupResource(const std::string& resourceFileName);
	void loadPlugis(const std::string& resourceFileName);
	bool createCamera(void);
	bool createScene(void);
	bool createViewport(void);

protected:
	bool initializeCEGUI(HWND hWnd);

public:
	CEGUI::System*	getCEGUISystem(void) { return m_pCeguiSystem; }
	Ogre::Root*	getOgreRoot(void) { return m_pOgreRoot; }
	
private:
	CEGUI::System*	m_pCeguiSystem;
	CEGUI::OgreCEGUIRenderer* m_pOgreRender;
	CEGUI::Window* m_pCeguiRootWindow;
	Ogre::Root * m_pOgreRoot;
	Ogre::RenderWindow* m_pRenderWindow;
	Ogre::Camera* m_pMainCamera;
	Ogre::SceneManager* m_pSceneMgr;
	Ogre::Viewport* m_pSceneViewport;
	
};

extern CCoreSystem g_CoreSystem;
