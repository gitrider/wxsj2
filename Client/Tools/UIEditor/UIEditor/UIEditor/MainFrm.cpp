// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "UIEditor.h"
#include "LeftView.h"
#include "RightView.h"
#include "UIEditorView.h"
#include "FontManger.h"
#include "MainFrm.h"
#include "CoreSystem.h"
#include "DataPool.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_TOOL, OnViewTool)
	ON_COMMAND(ID_HIDE_TOOL, OnHideTool)
	ON_WM_ERASEBKGND()
	ON_COMMAND_RANGE(ID_FACE_0, ID_FACE_7, OnFaceChange)
	ON_COMMAND_RANGE(ID_TOOL_BEGIN, ID_TOOL_END, OnToolCommand)
	ON_COMMAND(ID_HIDE_L_TOOL, &CMainFrame::OnHideLTool)
	ON_UPDATE_COMMAND_UI(ID_HIDE_L_TOOL, &CMainFrame::OnUpdateHideLTool)
	ON_COMMAND(ID_HIDE_R_TOOL, &CMainFrame::OnHideRTool)
	ON_UPDATE_COMMAND_UI(ID_HIDE_R_TOOL, &CMainFrame::OnUpdateHideRTool)
	ON_UPDATE_COMMAND_UI(ID_HIDE_TOOL, &CMainFrame::OnUpdateHideTool)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOL, &CMainFrame::OnUpdateViewTool)
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_COMMAND(ID_SHOW_TOOLBAR, &CMainFrame::OnShowToolbar)
	ON_COMMAND(ID_FLOAT_BAR, &CMainFrame::OnFloatBar)
	ON_COMMAND(ID_1024_768, &CMainFrame::OnLimitSize_1024_768)
	ON_COMMAND(ID_800_600, &CMainFrame::OnLimitSize_800_600)
	ON_COMMAND(ID_SHOW_ALL, &CMainFrame::OnLimitSize_ShowAll)
	ON_UPDATE_COMMAND_UI(ID_800_600, &CMainFrame::OnUpdate800600)
	ON_UPDATE_COMMAND_UI(ID_1024_768, &CMainFrame::OnUpdate1024768)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ALL, &CMainFrame::OnUpdateShowAll)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_OPEN_PRJ, &CMainFrame::OnOpenPrj)
	ON_COMMAND(ID_OPEN_OR_CLOSE_LUA, &CMainFrame::OnOpenOrCloseLua)
	ON_COMMAND(ID_RELOAD_SCRIPT, &CMainFrame::OnReloadScript)
	ON_COMMAND(ID_XML_VIEW, &CMainFrame::OnXmlView)
	ON_COMMAND(ID_FILE_SAVE_SELF, &CMainFrame::OnFileSaveSelf)
	ON_COMMAND(ID_FILE_SAVE_AS_SELF, &CMainFrame::OnFileSaveAsSelf)
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	//1000,
	//1002,
	//1003,
};


// CMainFrame 构造/析构

DESING_SIZE CMainFrame::m_desingSize;

CMainFrame::CMainFrame()
{
	m_checkLHide = false;
	m_checkRHide = false;
	m_isInit =false;
	m_desingSize = DESIGN_SIZE_800_600;
	m_hCurrentCursor = LoadCursor(AfxGetInstanceHandle(), IDC_ARROW);
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::InitializeToolBar()
{
	m_toolDlg.Create(CToolBarListDlg::IDD, this);
	m_toolDlg.ShowWindow(SW_SHOW);
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CFontManger::Initialize();
		
	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT  , WS_CHILD | WS_VISIBLE | CBRS_SIZE_FIXED |
	//	CBRS_LEFT | CBRS_TOOLTIPS ))//||
	//	//!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("未能创建工具栏\n");
	//	return -1;      // 未能创建
	//}
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	InitializeToolBar();
	InitializeMenu();
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_LEFT);
	EnableDocking(CBRS_ALIGN_LEFT);
	//DockControlBar(&m_wndToolBar);
	//FloatControlBar(&m_wndToolBar, CPoint(250,50) );

	//创建luaDlg
	m_luaDlg = new CLuaDlg();
	g_luaDlg = m_luaDlg;
	BOOL ret = m_luaDlg->Create(CLuaDlg::IDD, this);
	
	//创建xml浏览器
	m_xmlDlg.Create(CLayoutXMLPreviewDlg::IDD, this);
	g_xmlViewDlg = &m_xmlDlg;

	return 0;
}
void CMainFrame::InitializeMenu()
{
	//CMenu* pMenu = GetMenu();
	//if (pMenu == NULL)
	//{
	//	printf("错误：无法获得主菜单\n");
	//	return ;
	//}
	//UINT count = 0;
	//MENU_ITEM * pMenuItem;
	//bool showMenu = theApp.m_pSystem->GetMenuInfo(MENU_TYPE_MAIN, count, &pMenuItem);
	//if (showMenu == FALSE)
	//{
	//	//printf("插件没有导出主菜单");
	//	return;
	//}
	//UINT pos = 3;
	//CMenu SubMenu ;
	//MENUITEMINFO info;
	//memset(&info, 0, sizeof(info));
	//info.cbSize = sizeof(info);
	//info.fMask = MIIM_SUBMENU | MIIM_STRING;
	//for (UINT i=0; i<count; i++)
	//{
	//	if (pMenuItem[i].nCmdID == MENU_PARENT_ITEM)
	//	{
	//		info.dwTypeData = pMenuItem[i].ItemName;
	//		SubMenu.Detach();
	//		SubMenu.CreateMenu();
	//		info.hSubMenu = SubMenu.GetSafeHmenu();
	//		 pMenu->InsertMenuItem(pos++, &info, TRUE);
	//	}
	//	else 
	//	{
	//		if (pMenuItem[i].nCmdID == MENU_SPERATOR)
	//		{
	//			SubMenu.AppendMenu(MF_SEPARATOR, 0, _T(""));
	//		}
	//		else
	//		{
	//			SubMenu.AppendMenu(MF_STRING  | MF_POPUP, pMenuItem[i].nCmdID, pMenuItem[i].ItemName);
	//		}
	//	}
	//}
	//SubMenu.Detach();
	//
}
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!m_wndSpliter1.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSpliter1.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(200,0), pContext)) 
		return FALSE;

	if(!m_wndSpliter2.CreateStatic(&m_wndSpliter1, 1, 2, WS_CHILD|WS_VISIBLE, m_wndSpliter1.IdFromRowCol(0,1)))
		return FALSE;
	
	if(!m_wndSpliter2.CreateView(0,0,RUNTIME_CLASS(CUIEditorView), CSize(800,0), pContext))
		return FALSE;

	if(!m_wndSpliter2.CreateView(0,1,RUNTIME_CLASS(CRightView), CSize(200,0),pContext))
		return FALSE;
	m_pRightView = (CRightView*)m_wndSpliter2.GetPane(0,1);
	m_pLeftView = (CLeftView*)m_wndSpliter1.GetPane(0,0);

	ASSERT(m_pLeftView != NULL && m_pRightView != NULL);
	m_isInit = true;
	return  TRUE;//CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnSize(UINT nType,int cx,	int cy 	)
{

	if (m_isInit)
	{
		m_wndSpliter1.SetColumnInfo(0,250 , 0);
		m_wndSpliter1.SetColumnInfo(1, cx - 500 >0 ? cx - 500: 100,100);
		m_wndSpliter1.RecalcLayout();
		m_wndSpliter2.SetColumnInfo(0,cx - 500 >0 ? cx - 500: 100,100);
		m_wndSpliter2.SetColumnInfo(1, 250,0);
		m_wndSpliter2.RecalcLayout();
		((CUIEditorView*)m_wndSpliter2.GetPane(0,0))->initialize();
	}
	//else
	{
		CFrameWnd::OnSize(nType, cx, cy);
	}
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= WS_MAXIMIZE ;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	//if(pMsg->message == WM_IME_COMPOSITION )
	//{
	//	CUIEditorView::SetRenderFlag( TRUE);
	//}
	//else if (pMsg->message == WM_IME_ENDCOMPOSITION)
	//{
	//	CUIEditorView::SetRenderFlag( FALSE);
	//}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnViewTool()
{
	if (!m_checkLHide && !m_checkRHide)
	{
		return;
	}
	CRect rect;
	m_wndSpliter1.GetClientRect(&rect);
	m_wndSpliter1.SetColumnInfo(0, 250,0);
	m_wndSpliter1.SetColumnInfo(1,rect.right, 400);

	m_wndSpliter2.SetColumnInfo(0, rect.right - 500 , 400);
	m_wndSpliter2.SetColumnInfo(1, 250,0);
	m_wndSpliter1.RecalcLayout();
	m_wndSpliter2.RecalcLayout();
	m_checkLHide = false;
	m_checkRHide = false;
}
void CMainFrame::OnHideTool()
{
	if (m_checkLHide && m_checkRHide)
	{
		return;
	}
	CRect rect;
	m_wndSpliter1.GetClientRect(&rect);
	m_wndSpliter1.SetColumnInfo(0, 0,0);
	m_wndSpliter1.SetColumnInfo(1,rect.right, 800);

	m_wndSpliter2.SetColumnInfo(0, rect.right , 800);
	m_wndSpliter2.SetColumnInfo(1, 0,0);
	m_wndSpliter1.RecalcLayout();
	m_wndSpliter2.RecalcLayout();
	m_checkLHide = true;
	m_checkRHide = true;
}
void CMainFrame::OnHideLTool()
{
	if (m_checkLHide)
	{
		return;
	}
	CRect rect;
	int mx, mmx;
	m_wndSpliter2.GetColumnInfo(1, mx, mmx);
	m_wndSpliter1.GetClientRect(&rect);
	m_wndSpliter1.SetColumnInfo(0, 0,0);
	m_wndSpliter1.SetColumnInfo(1,rect.right  , 800);
	m_wndSpliter2.SetColumnInfo(1, mx, mmx);
	m_wndSpliter2.SetColumnInfo(0, rect.right - mx, 800);
	m_wndSpliter1.RecalcLayout();
	m_wndSpliter2.RecalcLayout();
	m_checkLHide = true;
}


void CMainFrame::OnHideRTool()
{
	if (m_checkRHide)
	{
		return;
	}
	CRect rect;
	m_wndSpliter2.GetClientRect(&rect);
	m_wndSpliter2.SetColumnInfo(0,rect.right , 800);
	m_wndSpliter2.SetColumnInfo(1, 0,0);
	m_wndSpliter2.RecalcLayout();
	m_wndSpliter1.RecalcLayout();
	m_checkRHide = true;
}
void CMainFrame::OnToolCommand(UINT nID)
{
	//UINT id = m_wndToolBar.GetToolBarCtrl().CommandToIndex(nID);
	//if (id >= 0 && id < m_barInfo.numBars)
	//{
	//	if (m_barInfo.barItem[id].hCursor)
	//	{
	//		m_hCurrentCursor = m_barInfo.barItem[id].hCursor;
	//	}
	//	else
	//	{
	//		m_hCurrentCursor = LoadCursor(AfxGetInstanceHandle(), IDC_ARROW);
	//	}
	//	g_pEditorView->SetUserCursor(m_hCurrentCursor);
	//	printf("调用SetCursor\n");
	//}
}
void CMainFrame::OnFaceChange(UINT nID)
{
	char fileName[MAX_PATH];
	switch(nID)
	{
	case ID_FACE_0:
		strcpy_s(fileName, MAX_PATH, "./kin/kin00.skin");
		m_nIndex = 0;
		break;
	case ID_FACE_1:
		strcpy_s(fileName, MAX_PATH, "./kin/kin01.skin");
		m_nIndex = 1;
		break;
	case ID_FACE_2:
		strcpy_s(fileName, MAX_PATH, "./kin/kin02.skin");
		m_nIndex = 2;
		break;
	case ID_FACE_3:
		strcpy_s(fileName, MAX_PATH, "./kin/kin03.skin");
		m_nIndex = 3;
		break;
	case ID_FACE_4:
		strcpy_s(fileName, MAX_PATH, "./kin/kin04.skin");
		m_nIndex = 4;
		break;
	case ID_FACE_5:
		strcpy_s(fileName, MAX_PATH, "./kin/kin05.skin");
		m_nIndex = 5;
		break;
	case ID_FACE_6:
		strcpy_s(fileName, MAX_PATH, "./kin/kin06.skin");
		m_nIndex = 6;
		break;
	case ID_FACE_7:
		strcpy_s(fileName, MAX_PATH, "./kin/kin07.skin");
		m_nIndex = 7;
		break;
	/*case ID_FACE_8:
		strcpy_s(fileName, MAX_PATH, "./kin/kin08.skin");
		m_nIndex = 8;
		break;
	case ID_FACE_9:
		strcpy_s(fileName, MAX_PATH, "./kin/kin09.skin");
		m_nIndex = 9;
		break;*/
	default:
		ASSERT(FALSE);
		break;
	}
	WritePrivateProfileStringA("Skin",  "LastSelected" , fileName, "./UIEditor.inf");
	//RemoveWindowSkin(m_hWnd);
	//if(LoadSkinFile(fileName) != 1)
	//{
	//	::MessageBoxA(m_hWnd, "加载皮肤文件失败", "提示", MB_OK);
	//}
	//VERIFY( 1 == SetWindowSkin( AfxGetMainWnd()->GetSafeHwnd() , "MainFrame" ));
	//VERIFY( 1 == SetDialogSkin( "Dialog" ) );
//	MessageBoxA(NULL, fileName, "Test", MB_OK);
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序




BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return FALSE;//CFrameWnd::OnEraseBkgnd(pDC);
}

void CMainFrame::OnUpdateHideLTool(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_checkLHide);
}

void CMainFrame::OnUpdateHideRTool(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_checkRHide);
}

void CMainFrame::OnUpdateHideTool(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_checkRHide && m_checkLHide);
}

void CMainFrame::OnUpdateViewTool(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_checkRHide && !m_checkLHide);
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize = CPoint(800, 600);
//	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnDestroy()
{
	m_isInit = false;
	g_CoreSystem.cleanup();
	CFrameWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CMainFrame::OnShowToolbar()
{
	if(m_toolDlg.IsWindowVisible())
	{
		m_toolDlg.ShowWindow(SW_HIDE);
	}
	else
	{
		m_toolDlg.ShowWindow(SW_SHOW);
	}
	//ShowControlBar(&m_wndToolBar, TRUE, FALSE);
}

void CMainFrame::OnFloatBar()
{
	m_toolDlg.ShowWindow(SW_SHOW);
}

INT_PTR CMainFrame::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	return TRUE;
	//return CFrameWnd::OnToolHitTest(point, pTI);
}

void CMainFrame::OnLimitSize_1024_768()
{
	((CUIEditorView*)m_wndSpliter2.GetPane(0,0))->setLayoutSize(LS_1024X768);
	m_desingSize = DESIGN_SIZE_1024_768;
}

void CMainFrame::OnLimitSize_800_600()
{
	((CUIEditorView*)m_wndSpliter2.GetPane(0,0))->setLayoutSize(LS_800X600);
	m_desingSize = DESIGN_SIZE_800_600;
}

void CMainFrame::OnLimitSize_ShowAll()
{
	((CUIEditorView*)m_wndSpliter2.GetPane(0,0))->setLayoutSize(LS_SHOW_ALL);
	m_desingSize = DESIGN_SIZE_800_600;
}

void CMainFrame::OnUpdate800600(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_desingSize == DESIGN_SIZE_800_600);
}

void CMainFrame::OnUpdate1024768(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_desingSize == DESIGN_SIZE_1024_768);
}

void CMainFrame::OnUpdateShowAll(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_desingSize == DESIGN_SIZE_UNLIMIT);
}

BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//SetCursor(m_hCurrentCursor);
	return  CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void	CMainFrame::SetStatusText(INT id, LPCTSTR szStr)
{
	if( id > -1 && id < 4 )
	{
		m_wndStatusBar.SetPaneText(id, szStr);
	}
}
void CMainFrame::OnOpenPrj()
{
	CFileDialog dlg(TRUE, "", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Layout文件(*.layout.xml)|*.layout.xml||", this);
	if( dlg.DoModal() == IDOK )
	{
		CString szName = dlg.GetPathName();
		g_DataPool.OpenLayOut(szName);
	}
}

VOID	CMainFrame::SetLuaText(const CString& text) 
{
	m_luaDlg->SetLuaText(text);
}
VOID	CMainFrame::ShowLuaDlg(BOOL bShow)
{
	if(bShow)
	{
		m_luaDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_luaDlg->ShowWindow(SW_HIDE);
	}
}
void CMainFrame::OnOpenOrCloseLua()
{
	if( m_luaDlg->IsWindowVisible() )
	{
		ShowLuaDlg(FALSE);
		g_DataPool.OnShowLuaWindow(FALSE);
	}
	else
	{
		ShowLuaDlg(TRUE);
		g_DataPool.OnShowLuaWindow(TRUE);
	}
}

void CMainFrame::OnReloadScript()
{
	g_DataPool.OnReloadScript();
}

void CMainFrame::OnXmlView()
{
	if( m_xmlDlg.IsWindowVisible() )
	{
		m_xmlDlg.ShowWindow(SW_HIDE);
		g_DataPool.OnShowLayouXmlWindow(FALSE);
	}
	else
	{
		m_xmlDlg.ShowWindow(SW_SHOW);
		g_DataPool.OnShowLayouXmlWindow(TRUE);
	}
}

void CMainFrame::OnFileSaveSelf()
{
	if( g_pEditorView->getShowMode() == false ) return;

	CString szName = g_pEditorView->getSelectedWindowName();
	if ( szName.GetLength() > 0 )
	{
		g_DataPool.OnSaveCommand(szName, TRUE);
	}
	else
	{
		MessageBox("请先选择一个窗口","提示");
	}
}

void CMainFrame::OnFileSaveAsSelf()
{
	OnFileSaveSelf();
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

}

void CMainFrame::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CFrameWnd::OnActivateApp(bActive, dwThreadID);

	g_pEditorView->renderPause( !bActive );
}
