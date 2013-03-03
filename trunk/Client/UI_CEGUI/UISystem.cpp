
#include "StdAfx.h"
#include "UISystem.h"
//#include <OgreRoot.h>//移到了UISystem.h中
#include <CEGUISystem.h>
#include <OgreCEGUIRenderer.h>
#include <GIException.h>
#include <CEGUISchemeManager.h>
#include <CEGUIWindowManager.h>
#include <CEGUIFontManager.h>
#include "ceguiwindow.h"
#include "uiwindowmng.h"
#include "GIKernel.h"
#include <CEGUIScriptModule.h>
#include "UIScript.h"
#include "GIScriptSystem.h"
#include "UIIconsMng.h"
#include <CEGUIImageset.h>
#include <OgreCEGUITexture.h>
#include <OgreTexture.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreTextureManager.h>
#include "ceguianimatemanager.h"
#include "ogretimer.h"
#include "UIString.h"
#include "GIInputSystem.h"
#include "GIGfxSystem.h"
#include "GICursorSystem.h"
#include "../CEGUIFalagardEx/FalActionButton.h"
#include "GIActionSystem.h"
#include <CEGUIEventArgs.h>
#include "UICreatureBoardSystem.h"
#include "GITimeSystem.h"
#include "GIProfile.h"
#include "GIEventDefine.h"
#include "GIFakeObjectManager.h"
#include "elements\CEGUIStaticImage.h"
#include "CEGUIIme.h"
#include "AxTrace.h"
#include "windows.h"
#include "GIGameInterface.h"
#include <CEGUIHyperLinkManager.h>
#include "UIString.h"
#include "GIVariable.h"
#include "GITransferItemSystem.h"
#include "GISound.h"
#include "CEGUIPropertyHelper.h"
#include "GIWorldSystem.h"
#include "UIString.h"
#include "GIDBC_Struct.h"
#include "FalIMEEditBox.h"
#include "FalChatHistory.h"
#include "FalChatBoard.h"
#include "FalMultiIMEEditBox.h"





KL_IMPLEMENT_DYNAMIC(CUISystem, GETCLASS(tUISystem));
CUISystem* CUISystem::s_pMe = NULL;


CUISystem::CUISystem(VOID)
{
	s_pMe = this;	
	//m_pBtnInDragging = NULL;
	m_szNameBtnInDragging = "";
	m_hDraggingCursor = NULL;
	m_bIsDragEnd = false;
	m_bActivePaoPao = true;
	m_bIsCtrlInfoEnable = false;
}

CUISystem::~CUISystem(VOID)
{
}


VOID CUISystem::InjectInput(VOID)
{
	CEGUI::System& ceguiSystem = CEGUI::System::getSingleton();
	InputEventQueue& queueInput = g_pInputSys->GetInputQueue();
	POINT pt = g_pInputSys->MouseGetPos();

	for(INT i=0; i<(INT)queueInput.size(); i++)
	{
		InputEvent& event = queueInput[i];
		if(event.bProcessed) continue;	//已经处理过的事件

		switch(event.nEvent)
		{
		//---------------------------------------------------
		//鼠标
		case EVENT_MOUSE_MOVE:
		case EVENT_MOUSE_LDRAG_MOVE:
		case EVENT_MOUSE_RDRAG_MOVE:
			{
				POINT pt = g_pInputSys->MouseGetPos();	
				DWORD dwMouseFlag = g_pInputSys->MouseGetFlag();
				if(ceguiSystem.injectMousePosition((FLOAT)pt.x, (FLOAT)pt.y))
					event.bProcessed = true;
			}
			break;

		case EVENT_MOUSE_LBDOWN:
			{
				//如果在拖动中,处理拖动结束
				if(!m_szNameBtnInDragging.empty())
				{
					CEGUI::FalagardActionButton *pBtnInDragging = (CEGUI::FalagardActionButton *)CEGUI::WindowManager::getSingleton().getWindow(m_szNameBtnInDragging);
					//if(m_pBtnInDragging)
					if(pBtnInDragging)
					{
						CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)pt.x, (FLOAT)pt.y));
						if( pWindow )
						{
							OnDragEnd(pWindow);
							event.bProcessed = true;
						}
					}
				}
				else
				{
					if(ceguiSystem.injectMouseButtonDown( CEGUI::LeftButton ))
						event.bProcessed = true;
				}
			}
			break;

		case EVENT_MOUSE_LBUP:
			{
				if(CEGUI::Window::getCaptureWindow())
				{
					if(ceguiSystem.injectMouseButtonUp( CEGUI::LeftButton ))
					{
						event.bProcessed = true;
						//捕获本桢鼠标Click事件
						for(INT j=0; j<(INT)queueInput.size(); j++) 
						{
							if(queueInput[j].nEvent == EVENT_MOUSE_LBCLICK || queueInput[j].nEvent == EVENT_MOUSE_LBUP) //todo_yangjun
								queueInput[j].bProcessed = true;
						}
					}
				}
				if(!m_szNameBtnInDragging.empty())
				{
					CEGUI::FalagardActionButton *pBtnInDragging = (CEGUI::FalagardActionButton *)CEGUI::WindowManager::getSingleton().getWindow(m_szNameBtnInDragging);
					if(pBtnInDragging) // 如果正在拖动
					{
						CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)pt.x, (FLOAT)pt.y));
						if( pWindow && pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton") ) // 如果接受的是ActionButton
						{
							OnDragEnd(pWindow);
							event.bProcessed = true;
							break;
						}
					}
				}
			}
			break;
		case EVENT_MOUSE_LBCLICK:
			{
				if(ceguiSystem.injectMouseButtonUp( CEGUI::LeftButton ))
				{
					event.bProcessed = true;
					//捕获本桢鼠标Click事件
					for(INT j=0; j<(INT)queueInput.size(); j++) 
					{
						if(queueInput[j].nEvent == EVENT_MOUSE_LBCLICK || queueInput[j].nEvent == EVENT_MOUSE_LBUP) //todo_yangjun
							queueInput[j].bProcessed = true;
					}
				}
			}
			break;

		case EVENT_MOUSE_RBDOWN:
			{
				if(ceguiSystem.injectMouseButtonDown( CEGUI::RightButton ))
				{
					event.bProcessed = true;
				}
			}
			break;

		case EVENT_MOUSE_RBCLICK:
			{
                if(ceguiSystem.injectMouseButtonUp( CEGUI::RightButton ))
				{
					event.bProcessed = true;

					//捕获本桢鼠标Click事件
					for(INT j=0; j<(INT)queueInput.size(); j++) 
					{
						if(queueInput[j].nEvent == EVENT_MOUSE_RBCLICK || queueInput[j].nEvent == EVENT_MOUSE_RBUP)
							queueInput[j].bProcessed = true;
					}
				}

				//右键点击，恢复到缺省技能操作，用于退出修理，鉴定，等特殊鼠标命令状态
				g_pGameInterface->Skill_CancelAction();
			}
			break;

		case EVENT_MOUSE_RBUP:
			{
				if(ceguiSystem.injectMouseButtonUp( CEGUI::RightButton ))
				{
					event.bProcessed = true;
				}
			}
			break;

		case EVENT_MOUSE_LBDOWNREPEAT:
			{
			}
			break;

		default:
			continue;
		}

		if(!event.bProcessed)
		{
			//对于未加Handle的Keybord Event，根据位置判断是否应该捕获
			if(event.nEvent == EVENT_KEY_DOWN || event.nEvent == EVENT_KEY_UP)
			{
			}
			else
			//对于未加Handle的Mouse Event，根据位置判断是否应该捕获
			{
				CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)pt.x, (FLOAT)pt.y));

				if(	event.nEvent == EVENT_MOUSE_LBDOWN || event.nEvent == EVENT_MOUSE_LBUP ||
					event.nEvent == EVENT_MOUSE_LBCLICK || event.nEvent == EVENT_MOUSE_LDBCLICK ||
					event.nEvent == EVENT_MOUSE_LBDOWNREPEAT)
				{
					if(pWindow && !(pWindow->getMouseLButtonHollow()))
						event.bProcessed = true;
					//toooooooooooooooo bad. wait a better solution.
					if(pWindow && pWindow->testClassName((const CEGUI::utf8*)"FalagardChatBoard"))
						event.bProcessed = false;
				}
				else if(event.nEvent == EVENT_MOUSE_RBDOWN || event.nEvent == EVENT_MOUSE_RBUP ||
					event.nEvent == EVENT_MOUSE_RBCLICK || event.nEvent == EVENT_MOUSE_RDBCLICK)
				{
					if(pWindow && !(pWindow->getMouseRButtonHollow()))
						event.bProcessed = true;
					//if(pWindow && pWindow->testClassName((const CEGUI::utf8*)"FalagardChatBoard"))
					//	event.bProcessed = false;

				}
				else if(pWindow && !(pWindow->getMouseHollow()))
				{
					event.bProcessed = true;
				}
			}
		}
	}
}

BOOL CUISystem::MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Ime::ImeMsgProc(message, wParam, lParam);
}

BOOL CUISystem::IsMouseHover(VOID)
{
	if(m_szNameBtnInDragging != "")
	{
		CEGUI::FalagardActionButton *pBtnInDragging = (CEGUI::FalagardActionButton *)CEGUI::WindowManager::getSingleton().getWindow(m_szNameBtnInDragging);
		//if(m_pBtnInDragging) 
		if(pBtnInDragging) 
			return true;
	}

	POINT pt = g_pInputSys->MouseGetPos();

	CEGUI::System& ceguiSystem = CEGUI::System::getSingleton();
	CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)pt.x, (FLOAT)pt.y), 0);
	if(pWindow && !(pWindow->getMouseHollow() || pWindow->getMouseLButtonHollow()))
	{
		//toooooooooooooooo bad. wait a better solution.
		if(pWindow->testClassName((CEGUI::utf8*)"FalagardChatBoard") && getChatBoard_HyplinkContex(pt.x,pt.y).empty() )
		//	!( ( (CEGUI::FalagardChatBoard*)pWindow)->IsPointOnInfoItem(CEGUI::Point(pt.x, pt.y)) ) )
		{
			g_pGfxSystem->SetDebugString("UI", "-");
			return false;
		}
		g_pGfxSystem->SetDebugString("UI", pWindow->getName().c_str());
		return true;
	}

	return false;
}

BOOL CUISystem::IsMouseHover_ChatBoard(LONG x, LONG y)
{
	CEGUI::System& ceguiSystem = CEGUI::System::getSingleton();
	CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)x, (FLOAT)y));

	const char *name = pWindow->getName().c_str();
	if(pWindow && pWindow->testClassName((CEGUI::utf8*)"FalagardChatBoard"))
		return TRUE;
	else
		return FALSE;
}

STRING CUISystem::getChatBoard_HyplinkContex(LONG x, LONG y)
{
	if(IsMouseHover_ChatBoard(x,y))
	{
		CEGUI::System& ceguiSystem = CEGUI::System::getSingleton();
		CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)x, (FLOAT)y));

		CEGUI::String32 strUtf = CEGUI::HyperLinkManager::getSingleton().getHyperLinkContex(pWindow, CEGUI::Point((FLOAT)x, (FLOAT)y));
		STRING strMbc;
		CUIStringSystem::utf8_to_mbcs(strUtf.c_str(), strMbc);

		return strMbc;
	}

	return STRING("");
}

VOID CUISystem::Initial(VOID*)
{
	//----------------------------------------------------------------------
	//保存全局变量
	g_pScriptSys = (tScriptSystem*)g_pKernel->GetNode("bin\\script");
	KLAssert(g_pScriptSys);

	g_pEventSys = (tEventSystem*)g_pKernel->GetNode("bin\\event");
	KLAssert(g_pEventSys);

	g_pActionSys = (tActionSystem*)g_pKernel->GetNode("bin\\action");
	KLAssert(g_pActionSys);

	g_pInputSys = (tInputSystem*)g_pKernel->GetNode("bin\\input");
	KLAssert(g_pInputSys);

	g_pGfxSystem = (tGfxSystem*)g_pKernel->GetNode("bin\\gfx");
	KLAssert(g_pGfxSystem);

	g_pObjectSystem = (tObjectSystem*)g_pKernel->GetNode("bin\\objman");
	KLAssert(g_pObjectSystem);

	g_pCursorSystem = (tCursorSystem*)g_pKernel->GetNode("bin\\cursor");
	KLAssert(g_pCursorSystem);

	g_pDebuger = (tDebuger*)g_pKernel->GetNode("bin\\debuger");
	//KLAssert(g_pDebuger);

	g_pWorldSystem = ( tWorldSystem* ) g_pKernel->GetNode("bin\\worldman");
	KLAssert( g_pWorldSystem );

	g_pTimer = (tTimeSystem*)g_pKernel->GetNode("bin\\time");
	KLAssert(g_pTimer);

	g_pFakeSystem = (tFakeObjSystem*)g_pKernel->GetNode("bin\\fake");
	KLAssert(g_pFakeSystem);

	g_pVariableSystem = (tVariableSystem*)g_pKernel->GetNode("bin\\var");
	KLAssert(g_pVariableSystem);

	g_pGameInterface = (tGameInterfaceBase*)g_pKernel->GetNode("bin\\interface");
	KLAssert(g_pVariableSystem);

	g_pDataBaseSystem = (tDataBaseSystem*)g_pKernel->GetNode("bin\\dbc");
	KLAssert(g_pDataBaseSystem);

	g_pSoundSystem = (tSoundSystem*)g_pKernel->GetNode("bin\\snd");
	KLAssert(g_pSoundSystem);

	g_pTransferItemSystem = (tTransferItemSystem*)g_pKernel->GetNode("bin\\transfer");
	KLAssert(g_pTransferItemSystem);

	g_hMainWnd = g_pGfxSystem->GetWindow();

	//----------------------------------------------------------------------
	//初始化CEGUI
	m_pOgreRoot = Ogre::Root::getSingletonPtr();
	KLAssert(m_pOgreRoot);

	Ogre::RenderWindow* pWindow = (Ogre::RenderWindow*)m_pOgreRoot->getRenderTarget("");

	m_pCEGUIScript = new CEGUI::CGameUIScript;
	m_pCEGUIRender = new CEGUI::OgreCEGUIRenderer((Ogre::RenderTarget*)pWindow, 
		Ogre::RENDER_QUEUE_OVERLAY, false, 0, m_pOgreRoot->getSceneManager("FairyMainScene"));

	m_pCEGUISystem = new CEGUI::System( m_pCEGUIRender, m_pCEGUIScript, (const CEGUI::utf8*)"");

	//使用UI资源组
	m_pCEGUISystem->getResourceProvider()->setDefaultResourceGroup("General");
	
	//加载总控文件
	CEGUI::SchemeManager::getSingleton().loadScheme("Schema\\WoWLookSkin.scheme.xml");

	m_pCEGUISystem->setTooltip("WoWLook/Tooltip");

	//----------------------------------------------------------------------
	//挂接超连接管理器
	CEGUI::HyperLinkManager::getSingleton().subscribeEvent("HyperLinkActive",CEGUI::Event::Subscriber(&CUISystem::handleHyperLinkActive, CUISystem::GetMe()));
	CEGUI::HyperLinkManager::getSingleton().subscribeEvent("HyperLinkLeftActive",CEGUI::Event::Subscriber(&CUISystem::handleHyperLinkLeftActive, CUISystem::GetMe()));
	CEGUI::HyperLinkManager::getSingleton().subscribeEvent("HyperLinkRightActive",CEGUI::Event::Subscriber(&CUISystem::handleHyperLinkRightActive, CUISystem::GetMe()));
	CEGUI::HyperLinkManager::getSingleton().subscribeEvent("HyperLinkInactive",CEGUI::Event::Subscriber(&CUISystem::handleHyperLinkInactive, CUISystem::GetMe()));

	//----------------------------------------------------------------------
	//初始化动画管理器
	new CEGUI::AnimateManager( "schema\\wowanimate.xml");

	//中文输入初始化
	Ime::InitializeImm((unsigned int)g_hMainWnd);
	
	//----------------------------------------------------------------------
	//文本管理器
	m_pStringSystem = new CUIStringSystem;
	m_pStringSystem->Init();

	//----------------------------------------------------------------------
	//初始化图标管理器
	m_pIconsMng = new CUIIconsManager;
	KLAssert( m_pIconsMng);
	m_pIconsMng->Initial();

	//----------------------------------------------------------------------
	//初始化布局管理器
	m_pWindowMng = new CUIWindowMng;
	KLAssert( m_pWindowMng );
	m_pWindowMng->Init();
	//----------------------------------------------------------------------

	//初始化角色信息板管理器
	m_pCreatureBoardSystem = new CUICreatureBoardSystem;
	m_pCreatureBoardSystem->Initial();

	//----------------------------------------------------------------------
	//创建地图探灯
	const char* szMapLightName = "MapLight_RenderTexture";
	CEGUI::Texture* rttMapLightTexture = 
		((CEGUI::OgreCEGUIRenderer*)m_pCEGUIRender)->createTexture((CEGUI::utf8*)szMapLightName);
	KLAssert(rttMapLightTexture);

	//创建Imageset
	CEGUI::Imageset* rttImageSet = 
		CEGUI::ImagesetManager::getSingleton().createImageset(szMapLightName, rttMapLightTexture);
	rttImageSet->disableAutoFree();
	rttImageSet->defineImage((CEGUI::utf8*)"View", 
		CEGUI::Point(0.0f, 0.0f),
		CEGUI::Size(rttMapLightTexture->getWidth(), rttMapLightTexture->getHeight()),
		CEGUI::Point(0.0f,0.0f));

	//----------------------------------------------------------------------
	//拖动操作
	//m_pBtnInDragging = NULL;
	m_szNameBtnInDragging= "";

	RECT rect;
	GetClientRect(g_hMainWnd, &rect);
	g_pEventSys->PushEvent(GE_CHAT_ADJUST_MOVE_CTL, rect.right-rect.left, rect.bottom-rect.top);

}

VOID CUISystem::Release(VOID)
{
	if(m_pCreatureBoardSystem)
		m_pCreatureBoardSystem->Release();
	SAFE_DELETE( m_pCreatureBoardSystem );
	SAFE_DELETE( m_pIconsMng );
	SAFE_DELETE( m_pWindowMng );
	SAFE_DELETE( m_pStringSystem );

	delete CEGUI::AnimateManager::getSingletonPtr();

	Ime::UninitializeImm();

	SAFE_DELETE( m_pCEGUISystem );
	SAFE_DELETE( m_pCEGUIRender );
	SAFE_DELETE( m_pCEGUIScript );
}

VOID CUISystem::Tick(VOID)
{
	static UINT s_nLastTime = 0;

	UINT nTimeNow = g_pTimer->GetTimeNow();

	if(s_nLastTime == 0)
	{
		s_nLastTime = nTimeNow;
		return;
	}
    
	if( CEGUI::System::getSingletonPtr() )
		CEGUI::System::getSingleton().injectTimePulse(g_pTimer->CalSubTime(s_nLastTime, nTimeNow)/1000.0f); 

	s_nLastTime = nTimeNow;

	if( m_pCreatureBoardSystem )
		m_pCreatureBoardSystem->Tick();

	static UINT s_nLastFreeTime = g_pTimer->GetTimeNow();
	if( g_pTimer->CalSubTime( s_nLastFreeTime, nTimeNow ) > 6000 ) // 每分钟释放一次
	{
		CEGUI::ImagesetManager::getSingleton().FreeUnuseImageset();
		s_nLastFreeTime = nTimeNow;
	}
//	AxVar(0, 0, AX_FLOAT32, "elapsed", &fElapsedTime);
}

/// 窗口大小发生改变
VOID CUISystem::OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam)
{
	//用户点击最大化或者最小化
	if(WM_SIZE == message)
	{
		m_pCEGUIRender->_notifySizeChanged();
	}
	//用户拖动窗口边缘结束
	else if(WM_EXITSIZEMOVE == message)
	{
		m_pCEGUIRender->_notifySizeChanged();
	}

	if (m_pWindowMng)
	{
		m_pWindowMng->OnSizeChange();
	}
}

bool CUISystem::handleActionDragDropStarted(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!(pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton"))) return false;

	OnDragBeging((CEGUI::FalagardActionButton*)pWindow);

	return false;
}

bool CUISystem::handleActionButtonMouseEnter(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!pWindow || !(pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton"))) return false;
	//拖拽中...
	if(m_hDraggingCursor) return false;

	//得到逻辑数据
	CEGUI::IFalagardActionButton* pIBtn = (CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)pWindow;
	tActionItem* pActionItem = (tActionItem*)(g_pActionSys->GetAction((INT)(INT_PTR)(pIBtn->getLogicItemData())));
	if(pActionItem)
	{
		if(AOT_CHATMOOD == pActionItem->GetType())
		{
			std::string tip = pActionItem->GetName();
			CEGUI::String32 strUtf32;
			CUIStringSystem::GetMe()->ParserString_Runtime(tip, strUtf32);
			pWindow->setTooltipText(strUtf32);
		}
		else
		{
			//调用逻辑接口
			CEGUI::Rect rect = pWindow->getPixelRect();
			pActionItem->NotifyTooltipsShow((int)rect.d_left, (int)rect.d_top, (int)rect.d_right, (int)rect.d_bottom);
		}
	}
	return false;
}

bool CUISystem::handleActionButtonMouseLeave(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!pWindow || !(pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton"))) return false;

	//得到逻辑数据
	CEGUI::IFalagardActionButton* pIBtn = (CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)pWindow;
	tActionItem* pActionItem = (tActionItem*)(g_pActionSys->GetAction((INT)(INT_PTR)(pIBtn->getLogicItemData())));
	
	if(pActionItem)
	{
		//调用逻辑接口
		pActionItem->NotifyTooltipsHide();
	}
	return false;
}

bool CUISystem::handleActionButtonParentHidden(const CEGUI::EventArgs& e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!pWindow || !(pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton"))) return false;

	//得到逻辑数据
	CEGUI::IFalagardActionButton* pIBtn = (CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)pWindow;
	tActionItem* pActionItem = (tActionItem*)(g_pActionSys->GetAction((INT)(INT_PTR)(pIBtn->getLogicItemData())));
	
	if(pActionItem)
	{
		//调用逻辑接口
		pActionItem->NotifyTooltipsHide();
	}
	return false;
}

bool CUISystem::handleMeshWindowShown(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!pWindow || !(pWindow->testClassName((CEGUI::utf8*)"FalagardMeshWindow"))) return false;

	CEGUI::Size sz = pWindow->getAbsoluteSize();
	g_pFakeSystem->OnUIShown(pWindow->getName().c_str(), sz.d_width/sz.d_height);

	return false;
}

bool CUISystem::handleMeshWindowHiden(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!pWindow || !(pWindow->testClassName((CEGUI::utf8*)"FalagardMeshWindow"))) return false;

	g_pFakeSystem->OnUIHiden(pWindow->getName().c_str());

	return false;
}

VOID CUISystem::OnDragBeging(CEGUI::FalagardActionButton* pBtnDraging)
{
	CEGUI::IFalagardActionButton* pIBtn = (CEGUI::IFalagardActionButton*)pBtnDraging;
	tActionItem* pActionItem = (tActionItem*)g_pActionSys->GetAction((INT)(pIBtn->getLogicItemData()));

	if(!pActionItem) return;

	//ui捕获输入
	g_pInputSys->SetCapture(ICS_UI);

	//将光标设为相应的Image
	if(m_hDraggingCursor)
	{
		::DestroyIcon(m_hDraggingCursor);
		m_hDraggingCursor = NULL;
	}

	m_hDraggingCursor = CUIIconsManager::GetMe()->CreateAsWinCursor(pActionItem->GetIconName());
	if(m_hDraggingCursor)
	{
		g_pCursorSystem->EnterUICursor(m_hDraggingCursor);
	}

	//保存正在托拽中的ActionButton
	//m_pBtnInDragging = (CEGUI::FalagardActionButton*)pBtnDraging;
	m_szNameBtnInDragging = ((CEGUI::Window*)pBtnDraging)->getName();
}

VOID CUISystem::OnDragEnd(CEGUI::Window* pWindowTarget)
{	
	//释放捕获
	g_pInputSys->SetCapture(ICS_NONE);
	//恢复光标
	if(m_hDraggingCursor)
	{
		g_pCursorSystem->LeaveUICursor();
		::DestroyIcon(m_hDraggingCursor);
		m_hDraggingCursor = NULL;
	}
	g_pCursorSystem->SetCursor(CURSOR_NORMAL);

	//通知正在托拽的ActionButton
	if(m_szNameBtnInDragging != "")
	{
		CEGUI::FalagardActionButton *pBtnInDragging = (CEGUI::FalagardActionButton *)CEGUI::WindowManager::getSingleton().getWindow(m_szNameBtnInDragging);
		CEGUI::IFalagardActionButton* pIBtn = (CEGUI::IFalagardActionButton*)pBtnInDragging;

		if(pBtnInDragging)
		{
			pIBtn->notifyDragingEnd();
		}

		//判断是否被销毁
		BOOL bBeDestroy = (pWindowTarget && pWindowTarget->getMouseHollow());

		//如果被拖入一个actionbutton上，取得该ActionButton的DragAcceptName
		CEGUI::IFalagardActionButton* pITargetBtn = NULL;
		if(pWindowTarget && pWindowTarget->testClassName((CEGUI::utf8*)"FalagardActionButton"))
		{
			pITargetBtn = (CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)pWindowTarget;
			////pITargetBtn->notifyDragDrop_Droped(m_pBtnInDragging);
			pITargetBtn->notifyDragDrop_Droped(pBtnInDragging);
		}

		//通知逻辑系统
		tActionItem* pActionItem = (tActionItem*)g_pActionSys->GetAction((INT)(pIBtn->getLogicItemData()));
		if(pActionItem)
		{
			pActionItem->NotifyDragDropDragged(bBeDestroy, pITargetBtn ? (LPCTSTR)(pITargetBtn->getDragAcceptName()) : NULL, ((CEGUI::FalagardActionButton*)pIBtn)->getDragAcceptName());
		}
		//m_pBtnInDragging = NULL;
		m_szNameBtnInDragging = "";
		ChangeDragFlag(true);
	}

	//HACK by jinchao begin.
	CEGUI::WindowManager::WindowIterator itAllWindow = CEGUI::WindowManager::getSingleton().getIterator();
	for(itAllWindow.toStart(); !itAllWindow.isAtEnd(); itAllWindow++)
	{
		CEGUI::Window* pWindow = itAllWindow.getCurrentValue();
		if(pWindow && pWindow->isVisible() && pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton"))
		{
			CEGUI::IFalagardActionButton* pIBtn = 
				(CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton *)pWindow;

			pIBtn->notifyDragingEnd();
		}
	}

	//HACK by jinchao end.
}

tCreatureBoard*	CUISystem::CreateCreatureBoard(VOID)
{
	return (tCreatureBoard*)m_pCreatureBoardSystem->CreateCreatureBoard();
}

VOID CUISystem::AddNewBeHitBoard( bool bDouble, LPCTSTR szInfo, FLOAT fStartX, FLOAT fStartY, int nType, int nMoveType )
{
	m_pCreatureBoardSystem->AddNewBeHit( bDouble, szInfo, fStartX, fStartY, nType, nMoveType );
}

//某一组UI是否处于打开中
BOOL CUISystem::IsWindowShow(LPCTSTR szUIName, LPCTSTR szChildName)
{
	return m_pWindowMng->IsWindowShow(szUIName, szChildName);
}

VOID CUISystem::FakeObj_Show(LPCTSTR szWinName, LPCTSTR szName)
{
	//取得UI窗口
	CEGUI::Window* pMeshWindow = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)szWinName);
	if(!pMeshWindow || !(pMeshWindow->testClassName((CEGUI::utf8*)"FalagardMeshWindow"))) return;

	//按照固定的命名规则取得渲染纹理
	STRING strImageName = szName;
	strImageName += FAKE_RENDERTEXTURE;

	//取得纹理，该纹理应该在渲染层已经创建
	CEGUI::Texture* rttTexture = 
		((CEGUI::OgreCEGUIRenderer*)m_pCEGUIRender)->createTexture((CEGUI::utf8*)strImageName.c_str());
	KLAssert(rttTexture);

	//创建UI用的Imageset
	CEGUI::Imageset* rttImageSet = 
		CEGUI::ImagesetManager::getSingleton().createImageset(strImageName.c_str(), rttTexture);
	rttImageSet->disableAutoFree();
	rttImageSet->defineImage((CEGUI::utf8*)"View", 
		CEGUI::Point(0.0f, 0.0f),
		CEGUI::Size(rttTexture->getWidth(), rttTexture->getHeight()),
		CEGUI::Point(0.0f,0.0f));

	//设置显示纹理

	((CEGUI::StaticImage*)pMeshWindow)->setImage(&(rttImageSet->getImage((CEGUI::utf8*)"View")));
}

VOID CUISystem::FakeObj_Close(LPCTSTR szWinName, LPCTSTR szName)
{
	//取得UI窗口
	CEGUI::Window* pMeshWindow = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)szWinName);
	if(!(pMeshWindow->testClassName((CEGUI::utf8*)"FalagardMeshWindow"))) return;

	//通知UI
	((CEGUI::StaticImage*)pMeshWindow)->setImage(0);

	//销毁UI的Imageset
	STRING strImageName = szName;
	strImageName += FAKE_RENDERTEXTURE;

	CEGUI::ImagesetManager::getSingleton().destroyImageset((CEGUI::utf8*)strImageName.c_str());
}

VOID CUISystem::Debug_SaveFontTexture(VOID)
{
#if 1
	//Make sure have out dir
	::CreateDirectory("DebugOut", NULL);

	CEGUI::FontManager* pFontManager = CEGUI::FontManager::getSingletonPtr();
	CEGUI::FontManager::FontIterator it = pFontManager->getIterator();

	for(it.toStart(); !it.isAtEnd(); it++)
	{
		const CEGUI::String&  strName = it.getCurrentKey();
		CEGUI::FontBase* pFont = it.getCurrentValue();

		//------------------------------------------------------------------
		//Create Directiory
		CHAR szFontOut[MAX_PATH];
		strncpy(szFontOut, ".\\DebugOut", MAX_PATH);
		PathAppend(szFontOut, strName.c_str());
		CreateDirectory(szFontOut, NULL);

		std::vector< CEGUI::Imageset* > vImagesets;
		INT nShouldLen = pFont->debug_GetFontImageset(vImagesets);
		if(nShouldLen != vImagesets.size()) vImagesets.resize(nShouldLen);
		pFont->debug_GetFontImageset(vImagesets);

		//------------------------------------------------------------------
		for(INT i=0; i<(INT)vImagesets.size(); i++)
		{
			const CEGUI::OgreCEGUITexture* pTexture = (const CEGUI::OgreCEGUITexture*)vImagesets[i]->getTexture();
			const Ogre::TexturePtr pOgreTex = pTexture->getOgreTexture();
			const Ogre::HardwarePixelBufferSharedPtr pPixelBuf = pOgreTex->getBuffer(0, 0);
			
			Ogre::Image imageOut;
			imageOut.load("_TEMP_.tga", "General");
//			imageOut.loadDynamicImage(

			imageOut.resize((Ogre::ushort)pPixelBuf->getWidth(), (Ogre::ushort)pPixelBuf->getHeight());
			pPixelBuf->blitToMemory(imageOut.getPixelBox());

			CHAR szFileName[MAX_PATH];
			strncpy(szFileName, szFontOut, MAX_PATH);
			PathAppend(szFileName, vImagesets[i]->getName().c_str());
			strcat(szFileName, ".tga");
			imageOut.save(szFileName);
		}
	}
#endif
}

VOID CUISystem::ChangeDragFlag(bool Flag) 
{
	m_bIsDragEnd = Flag;
}

BOOL CUISystem::IsDragFlag(VOID)
{
	return m_bIsDragEnd;
}

STRING CUISystem::ParserString_VarParam(LPCTSTR strID, ... )
{
	STRING strR = "";
	if(m_pStringSystem)
	{

		va_list ptr;
		
		va_start(ptr, strID);
		strR = m_pStringSystem->ParserString_VarParam(strID, ptr);
		va_end(ptr);
	}

	return strR;
}

VOID CUISystem::ParserString_NoColor(const STRING& strSrc, STRING& strOut, BOOL bControl)
{
	if(m_pStringSystem) return m_pStringSystem->ParserString_NoColor(strSrc, strOut, bControl);
}

STRING CUISystem::ParserString_NoColor_VarParam(LPCTSTR strID, ... )
{
	STRING strR = "";

	if(m_pStringSystem)
	{
		va_list ptr;
		va_start(ptr, strID);
		STRING strColor = m_pStringSystem->ParserString_VarParam(strID, ptr);
		va_end(ptr);
		m_pStringSystem->ParserString_NoColor(strColor, strR);
	}

	return strR;
}

STRING CUISystem::ParserString_NoVarParam(LPCTSTR strID)
{
	STRING strR = "";

	if(m_pStringSystem)
	{
		m_pStringSystem->ParserString_NoVarParam(strID);
	}

	return strR;
}

STRING CUISystem::ParserString_NoColor_NoVarParam(LPCTSTR strID)
{
	STRING strR = "";
	if(m_pStringSystem)
	{
		STRING strColor = m_pStringSystem->ParserString_NoVarParam(strID);
		m_pStringSystem->ParserString_NoColor(strColor, strR);
	}
	return strR;
}

BOOL CUISystem::CloseAllWindow()
{
	CUIWindowMng::GetMe()->CloseAllWindow();
	return TRUE;
}

bool CUISystem::handleHyperLinkActive(const CEGUI::EventArgs &e)
{
	return true;
}

bool CUISystem::handleHyperLinkLeftActive(const CEGUI::EventArgs &e)
{
	const CEGUI::HyperLinkEventArgs& eHyperLink = (const CEGUI::HyperLinkEventArgs&)e;

	STRING strWnd, strLink;
	CUIStringSystem::utf8_to_mbcs(eHyperLink.windowName.c_str(), strWnd);
	CUIStringSystem::utf8_to_mbcs(eHyperLink.hyperLinkName.c_str(), strLink);

	if(CEGUI::WindowManager::getSingleton().isWindowPresent(eHyperLink.windowName))
	{
		CEGUI::Window* pWindow = CEGUI::WindowManager::getSingleton().getWindow(eHyperLink.windowName);
		if(pWindow->testClassName((CEGUI::utf8*)"FalagardChatBoard"))
		{
			//if(g_pInputSys->IsKeyDown(KC_LCONTROL))
			g_pEventSys->PushEvent(GE_CHAT_CHANGE_PRIVATENAME, strLink.c_str());
		}
		else// 如果不是就说明是特殊链接
		{
			if( !strLink.empty() ) //如果是空的
			{
				int nX,nY,nSceneID;
				char szName[128] = {0};

				// 20100416 ModifyCodeBegin  任务超链接字串加上NPC名称，方便通过该名称寻路后自动打开NPC对话框
				// sscanf( strLink.c_str(),"%d,%d,%d,%s", &nX,&nY,&nSceneID,szName )
				char szNPC[128] = {0};
				sscanf( strLink.c_str(),"%d,%d,%d,%[^,],%[^,]", &nX,&nY,&nSceneID,szName,szNPC );
				// 20100416 ModifyCodeEnd

				if( g_pWorldSystem && g_pWorldSystem->GetActiveScene() && g_pWorldSystem->GetActiveScene()->GetSceneDefine() )
				{
					int currentSceneID = g_pWorldSystem->GetActiveScene()->GetSceneDefine()->nLocalID;

					//------------------------------------------
					////HACKFIX: 解决“雪洛阳”问题
					//if( nSceneID != currentSceneID && currentSceneID == 242)
					//{
					//	currentSceneID = 0;
					//}
					//------------------------------------------

					//if( nSceneID != currentSceneID )
					//{
					//	STRING strTemp = "不在本场景,无法到达";
					//	g_pEventSys->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
					//	return true;
					//}


					// 20100416 AddCodeBegin 

					// 判断是否是可以对话的功能任务NPC，如果是则设置自动寻路的标记值
					if( strlen(szNPC) > 0 )
					{
						g_pGameInterface->SetAutoPathNPC( nSceneID, szNPC );
					}
					else
					{
						g_pGameInterface->SetAutoPathNPC( -1, "" );
					}

					// 20100416 AddCodeEnd

					g_pGameInterface->Player_MoveTo(nSceneID, fVector2( nX,nY ) );
					////g_pWorldSystem->setSelectNPC( szName );

				}
			}
		}

	}

	return true;
}

bool CUISystem::handleHyperLinkRightActive(const CEGUI::EventArgs &e)
{
	const CEGUI::HyperLinkEventArgs& eHyperLink = (const CEGUI::HyperLinkEventArgs&)e;

	STRING strWnd, strLink;
	CUIStringSystem::utf8_to_mbcs(eHyperLink.windowName.c_str(), strWnd);
	CUIStringSystem::utf8_to_mbcs(eHyperLink.hyperLinkName.c_str(), strLink);

	if(CEGUI::WindowManager::getSingleton().isWindowPresent(eHyperLink.windowName))
	{
		CEGUI::Window* pWindow = CEGUI::WindowManager::getSingleton().getWindow(eHyperLink.windowName);
		if(pWindow->testClassName((CEGUI::utf8*)"FalagardChatBoard"))
		{
			//if(g_pInputSys->IsKeyDown(KC_LCONTROL))
				g_pEventSys->PushEvent(GE_CHAT_CONTEX_MENU, strLink.c_str());
		}
	}
	return true;
}

bool CUISystem::handleHyperLinkInactive(const CEGUI::EventArgs &e)
{
	//AxTrace(0,0,"handleActionButtonMouseEnter");

	STRING strR = "HelpWindow";
	const CEGUI::HyperLinkEventArgs& eHyperLink = (const CEGUI::HyperLinkEventArgs&)e;

	STRING strUtf8,strUtf9;
	CUIStringSystem::utf8_to_mbcs(eHyperLink.hyperLinkName.c_str(), strUtf9);

	//g_pEventSys->PushEvent(GE_NEW_DEBUGMESSAGE, "mouse on hyperlink.");
	//g_pEventSys->PushEvent(GE_NEW_DEBUGMESSAGE, strUtf9.c_str());
	CUIStringSystem::utf8_to_mbcs(eHyperLink.windowName.c_str(), strUtf8);
	//g_pEventSys->PushEvent(GE_NEW_DEBUGMESSAGE, strUtf8.c_str());

	if(eHyperLink.windowName == "HelpWindow")
	{
	}
	else
	{
		return false;
	}

	return true;
}


bool CUISystem::handleElementDelete(const CEGUI::EventArgs& e)
{
	const CEGUI::ItemElementEventArgs& eItem = (const CEGUI::ItemElementEventArgs&)e;
	CEGUI::Window* pWindow = eItem.window;

	INT itemId = eItem.itemID;
	//通知TransferSystem删除这个ItemElement.
	//char dbgmsg[128] = {0};
	//_snprintf(dbgmsg, 127, "Delete ItemElementEvent ID:%d ", itemId);
	//::OutputDebugString(dbgmsg);

	g_pTransferItemSystem->DeleteElement(itemId);
	return false;
}

bool CUISystem::handleChatHistoryInfoElementClick(const CEGUI::EventArgs& e)
{
	const CEGUI::ItemElementEventArgs& eItem = (const CEGUI::ItemElementEventArgs&)e;
	CEGUI::Window* pWindow = eItem.window;

	INT msgID = eItem.msgID;
	INT itemId = eItem.itemID;
	//通知TransferSystem点击这个InfoElement.
	//char dbgmsg[128] = {0};
	//_snprintf(dbgmsg, 127, "InfoElementEvent ID:%d\n", itemId);
	//::OutputDebugString(dbgmsg);

	if(eItem.button == CEGUI::RightButton)
	{
		g_pTransferItemSystem->DoElement(itemId, ELEMENT_R_CLICK,msgID);
	}
	else if(eItem.button == CEGUI::LeftButton)
	{
		if( IsCtrlInfoEnable() && !(::GetKeyState(VK_LCONTROL)&0xFF00) )
			return false;
		g_pTransferItemSystem->DoElement(itemId, ELEMENT_L_CLICK);
	}

	return false;
}

bool CUISystem::handleChatHistoryInfoElementMoveIn(const CEGUI::EventArgs& e)
{
	const CEGUI::ItemElementEventArgs& eItem = (const CEGUI::ItemElementEventArgs&)e;
	CEGUI::Window* pWindow = eItem.window;

	INT itemId = eItem.itemID;
	g_pTransferItemSystem->DoElement(itemId, ELEMENT_MOVE_IN);

	return false;
}

bool CUISystem::handleChatHistoryInfoElementMoveOut(const CEGUI::EventArgs& e)
{
	const CEGUI::ItemElementEventArgs& eItem = (const CEGUI::ItemElementEventArgs&)e;
	CEGUI::Window* pWindow = eItem.window;

	INT itemId = eItem.itemID;
	g_pTransferItemSystem->DoElement(itemId, ELEMENT_MOVE_OUT);

	return false;
}

bool CUISystem::handlePushButtonClickSound(const CEGUI::EventArgs& e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;

	if(pWindow)
	{
		int nSoundId = CEGUI::PropertyHelper::stringToInt(pWindow->getProperty((CEGUI::utf8*)"ClickSound"));
		g_pSoundSystem->PlayUISoundDirect(nSoundId);
	}

	return false;
}

BOOL CUISystem::IsIMEEditboxActive(LPCTSTR szWinName)
{
	if(Ime::GetActiveIMEEditBox())
	{
		CEGUI::Window* pWin = Ime::GetActiveIMEEditBox();

		if( 0 == strcmp(pWin->getName().c_str(), szWinName))
			return TRUE;
	}
	return FALSE;
}

INT CUISystem::GetCurrentIMEState()
{
	return Ime::GetState();
}


//将一个物品信息传入
bool CUISystem::InjectItemInfo(tTransferItemSystem::Element *pElement)
{
	//是否有激活的IME窗口
	CEGUI::Window* pWindow = Ime::GetActiveIMEEditBox();
	if( !pWindow ) return false;

	//必须是单行编辑器
	if ( pWindow->testClassName("FalagardIMEEditBox") )
	{
		//转化为utf8
		CEGUI::String32 strName;
		CUIStringSystem::GetMe()->ParserString_Runtime( pElement->strDisplay, strName );

		return ((CEGUI::IFalagardIMEEditBox*)(CEGUI::FalagardIMEEditBox*)pWindow)->injectItemInfo(
			pElement->nID, strName, (INT)(pElement->strContents.length()) );
	}
	else if( pWindow->testClassName("FalagardMultiIMEEditBox") )
	{
		//转化为utf8
		CEGUI::String32 strName;
		CUIStringSystem::GetMe()->ParserString_Runtime( pElement->strDisplay, strName );

		return ( (CEGUI::FalagardMultiIMEEditBox*)pWindow )->injectItemInfo(
			pElement->nID, strName, (INT)(pElement->strContents.length() ) );
	}
	return false;
}


STRING CUISystem::getTalkTemplate(const STRING& strKey, UINT index)
{
	return m_pStringSystem->getTalkTemplate(strKey, index);
}

STRING CUISystem::ModifyChatTemplate(const STRING& strTemp, const STRING& strTalker, const STRING& strTarget)
{
	return m_pStringSystem->ModifyChatTemplate(strTemp,strTalker,strTarget);
}

STRING CUISystem::getTalkRandHelpMsg()
{
	return m_pStringSystem->getTalkRandHelpMsg();
}

void CUISystem::OnPushEcsape()
{
	CUIWindowMng::GetMe()->OnEscape();
}


// 过滤字符串中的非法敏感字符
BOOL CUISystem::CheckStringFilter(const STRING& strIn, const FILTER_TYPE filterType)
{
	if(m_pStringSystem)
	{
		return m_pStringSystem->_CheckStringFilter(strIn, filterType);
	}

	return   FALSE;
}
// 过滤截断字符的<ERROR>
BOOL CUISystem::CheckStringCode(const STRING& strIn, STRING& strOut) 
{
	if(m_pStringSystem)
	{
		return m_pStringSystem->_CheckStringCode(strIn,strOut);
	}

	return   FALSE;
}

// 完全匹配过滤
BOOL CUISystem::CheckStringFullCompare(const STRING& strIn, const STRING& strFilterType, BOOL bUseAllTable)
{
	if(m_pStringSystem)
	{
		return m_pStringSystem->_CheckStringFullCompare(strIn, strFilterType, bUseAllTable);
	}

	return   FALSE;
}
INT CUISystem::OgreColor_Lumination( UINT OgreColor )
{
	CEGUI::colour CEGUIColor;

	FLOAT fColor_a,fColor_r,fColor_g,fColor_b;

	fColor_a = (FLOAT)COLOR_A(OgreColor)/ 255.0f;
	fColor_r = (FLOAT)COLOR_R(OgreColor)/ 255.0f;
	fColor_g = (FLOAT)COLOR_G(OgreColor)/ 255.0f;
	fColor_b = (FLOAT)COLOR_B(OgreColor)/ 255.0f;

	CEGUIColor.setAlpha( fColor_a);
	CEGUIColor.setRed(fColor_r);
	CEGUIColor.setGreen(fColor_g);
	CEGUIColor.setBlue(fColor_b);

//	FLOAT xx = CEGUIColor.getLumination();

	return (INT)(CEGUIColor.getLumination() * 240);
}

UINT CUISystem::Lumination_OgreColor( UINT OgreColor, INT Lumination )
{
	CEGUI::colour CEGUIColor;

	
	FLOAT fColor_a,fColor_r,fColor_g,fColor_b,fLumination;

	fColor_a = (FLOAT)COLOR_A(OgreColor)/ 255.0f;
	fColor_r = (FLOAT)COLOR_R(OgreColor)/ 255.0f;
	fColor_g = (FLOAT)COLOR_G(OgreColor)/ 255.0f;
	fColor_b = (FLOAT)COLOR_B(OgreColor)/ 255.0f;
	fLumination = (FLOAT)Lumination / 240.0f;

	CEGUIColor.setAlpha( fColor_a);
	CEGUIColor.setRed( fColor_r );
	CEGUIColor.setGreen( fColor_g );
	CEGUIColor.setBlue( fColor_b );

	if( CEGUIColor.getLumination() != Lumination )
	{
		CEGUIColor.setHSL(CEGUIColor.getHue(),CEGUIColor.getSaturation(), fLumination );
	}
	
	INT Color_a,Color_r,Color_g,Color_b;
	Color_a = CEGUIColor.getAlpha() * 0xFF;
	Color_r = CEGUIColor.getRed() * 0xFF;
	Color_g = CEGUIColor.getGreen() * 0xFF;
	Color_b = CEGUIColor.getBlue() * 0xFF;

	return MAKE_COLOR(Color_r,Color_g,Color_b,Color_a);

}

STRING	CUISystem::GetDebugString(VOID)
{
	if ( m_pCEGUISystem )
	{
		CEGUI::Window* pWindow = m_pCEGUISystem->getWindowContainingMouse();
		if (pWindow)
		{
			STRING szDesc = "UI: ";
			szDesc += pWindow->getName().c_str();
			szDesc += "\nType: ";
			szDesc += pWindow->getType().c_str();
			szDesc += "\nParent: ";
			if (pWindow->getParent())
			{
				szDesc += pWindow->getParent()->getName().c_str();
			}
			else
			{
				szDesc += "none";
			}

			return szDesc;  
		}
	}
	return "";
}

void CUISystem::OpenWindow(const STRING& winName) 
{
	CUIWindowMng::GetMe()->OpenWindow(winName);
}
void CUISystem::CloseWindow(const STRING& winName) 
{
	CUIWindowMng::GetMe()->CloseWindow(winName);
}
void CUISystem::ToggleWindow(const STRING& winName)
{
	CUIWindowMng::GetMe()->ToggleWindow(winName);
}

bool CUISystem::HasInputFocus()
{
	return Ime::HasInputFocus();
}


STRING CUISystem::getIMEEditboxString(LPCTSTR szWinName)
{
	if(Ime::GetActiveIMEEditBox())
	{
		CEGUI::Window* pWin = Ime::GetActiveIMEEditBox();

		if( 0 == strcmp(pWin->getName().c_str(), szWinName))
		{
			CEGUI::IFalagardIMEEditBox *pWindow = (CEGUI::IFalagardIMEEditBox*)(CEGUI::FalagardIMEEditBox*)pWin;
			CEGUI::String32 strTxt = pWindow->getItemElementString();

			STRING strOut;
			CUIStringSystem::GetMe()->ParserString_ReverseRuntime(strTxt, strOut);

			return strOut;
		}
	}
	return "";
}

bool CUISystem::addChatHistoryMessage(INT msgID,LPCTSTR szWinName, LPCTSTR szMessage, INT nOp /*= -1*/, UINT nDisappearTime /*= 0*/)
{
	if(!szWinName || !szMessage) return false;

	CEGUI::Window* pWindow = CEGUI::WindowManager::getSingleton().getWindow(szWinName);
	if(pWindow->testClassName((CEGUI::utf8*)"FalagardChatHistory"))
	{
		std::string strMbcs = szMessage;
		CEGUI::String32 strUtf32;
		m_pStringSystem->ParserString_Runtime(strMbcs, strUtf32);

		CEGUI::Window* pChild = ((CEGUI::IFalagardChatHistory*)(CEGUI::FalagardChatHistory*)pWindow)->insertChatString(msgID,strUtf32, nOp);
		if(pChild && nDisappearTime > 0)
		{
			char strKillTimer[16] = {0};
			_snprintf(strKillTimer, 16, " %f", FLOAT(nDisappearTime));
			pChild->setProperty("KillTimer", strKillTimer );
		}
		return true;
	}

	return false;
}

VOID	CUISystem::ReplaceStringUseFilter(const STRING& strIn, STRING& strOut, FILTER_TYPE nFilterType /* = STR_FILTER_TALK */)
{
	if(m_pStringSystem)
	{
		return m_pStringSystem->_ReplaceStringUseFilter(strIn, strOut, nFilterType);
	}
}

std::string		CUISystem::string_check_valid(const char* szString)
{
	if(m_pStringSystem)
	{
		return m_pStringSystem->string_check_valid(szString);
	}
	return "";
}

bool CUISystem::ReloadWindowScript( const STRING& windowName )
{
	return m_pWindowMng->ReloadWindowScript(windowName.c_str());
}

//显示/隐藏整个ui
VOID CUISystem::UIRenderToggleShow(VOID)
{
	m_pCEGUIRender->setRenderDisable(!(m_pCEGUIRender->isRenderDisable()));
}