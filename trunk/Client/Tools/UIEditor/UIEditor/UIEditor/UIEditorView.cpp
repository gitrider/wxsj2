// UIEditorView.cpp : CUIEditorView 类的实现
//

#include "stdafx.h"
#include "UIEditor.h"
#include <mmsystem.h>
#include "UIEditorDoc.h"
#include "UIEditorView.h"
#include "MainFrm.h"
#include "CoreSystem.h"
#include "FunctionDlg.h"
#include "CreateWindowDlg.h"
#include "DataPool.h"
#include <ceguisystem.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <CEGUIInputEvent.h>
#include <CEGUISystem.h>
#include <CEGUIExceptions.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "Winmm.lib")

// CUIEditorView

IMPLEMENT_DYNCREATE(CUIEditorView, CView)

BEGIN_MESSAGE_MAP(CUIEditorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	//ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MENUSELECT()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CUIEditorView 构造/析构


CUIEditorView::CUIEditorView()
{
	m_canSize = FALSE;
	m_lButtonDown = FALSE;
	m_rButtonDown = FALSE;
	s_renderPause = FALSE;
	m_beginDraw = DST_DRAW_NONE;
	m_binitialize = false;
	m_bProcessing = false;
	m_bCreateWindow = false;
	m_bIsShowWindow = true;
	m_ptMouseMovePos = CPoint(0,0);

	memset(&m_nQuadInfo, 0, sizeof(m_nQuadInfo));

	//设置默认红色
	for (INT i=0; i<QC_NUM; i++)
	{
		m_nQuadInfo[i].quadColor = RGB(255,0,0);
	}

	//左上和右下
	m_BorderResponse[BRP_SELECT_WINDOW_LEFT_TOP_QUAD].cursor = LoadCursor(NULL,IDC_SIZENWSE);
	m_BorderResponse[BRP_SELECT_WINDOW_RIGHT_BOTTOM_QUAD].cursor = m_BorderResponse[BRP_SELECT_WINDOW_LEFT_TOP_QUAD].cursor;

	//右上和左下
	m_BorderResponse[BRP_SELECT_WINDOW_RIGHT_TOP_QUAD].cursor = LoadCursor(NULL,IDC_SIZENESW);
	m_BorderResponse[BRP_SELECT_WINDOW_LEFT_BOTTOM_QUAD].cursor = m_BorderResponse[BRP_SELECT_WINDOW_RIGHT_TOP_QUAD].cursor;

	//中上和中下
	m_BorderResponse[BRP_SELECT_WINDOW_MIDDLE_TOP_QUAD].cursor = LoadCursor(NULL,IDC_SIZENS);
	m_BorderResponse[BRP_SELECT_WINDOW_MIDDLE_BOTTOM_QUAD].cursor = m_BorderResponse[BRP_SELECT_WINDOW_MIDDLE_TOP_QUAD].cursor;

	//中左和中右
	m_BorderResponse[BRP_SELECT_WINDOW_LEFT_MIDDLE_QUAD].cursor = LoadCursor(NULL,IDC_SIZEWE);
	m_BorderResponse[BRP_SELECT_WINDOW_RIGHT_MIDDLE_QUAD].cursor = m_BorderResponse[BRP_SELECT_WINDOW_LEFT_MIDDLE_QUAD].cursor;

	//正中间
	m_BorderResponse[BRP_SELECT_WINDOW_MIDDLE_MIDDLE_QUAD].cursor = LoadCursor(NULL,IDC_ARROW);

	//创建窗口的鼠标
	m_BorderResponse[BRP_CREATE_NEW_WINDOW_QUAD].cursor = LoadCursor(NULL, IDC_CROSS);

	//清理位置
	CRect rect;
	for (INT i=0; i<8; i++)
	{
		m_BorderResponse[i].hoverRect = rect;
	}

	m_pSelectedWindow = NULL;

	m_pCreatedWindow = NULL;

}

CUIEditorView::~CUIEditorView()
{
}

BOOL CUIEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CUIEditorView 绘制

void CUIEditorView::OnDraw(CDC* pDC)
{
	CUIEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//pDC->Draw3dRect(0,0,800,600,RGB(255,0,0),RGB(255,0,0));

}


// CUIEditorView 打印

BOOL CUIEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CUIEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CUIEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CUIEditorView 诊断

#ifdef _DEBUG
void CUIEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CUIEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUIEditorDoc* CUIEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUIEditorDoc)));
	return (CUIEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CUIEditorView 消息处理程序

int CUIEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	INT FPS = GetPrivateProfileIntA("System", "FPS", 30, "./UIEditor.inf");
	if (FPS > 100 || FPS <= 0)
	{
		FPS = 30;
	}
	
	timeBeginPeriod(1);
	//::SetTimer(NULL, 111, 1000/FPS, TimerProc);
	m_canSize = TRUE;
	g_pEditorView = this;
	m_hCursor = LoadCursor(NULL, IDC_ARROW);

	::SetRect(&(m_nQuadInfo[QC_PREVIEW_WINDOW].quadRect), 1024, 768, 1024 + 160,768 + 160);
	m_nQuadInfo[QC_PREVIEW_WINDOW].quadColor = RGB(0,0,255);
	m_nQuadInfo[QC_CREATE_WINDOW].quadColor = RGB(255,0,255);
		
	return 0;
}
BOOL CUIEditorView::s_renderPause = FALSE;
VOID CUIEditorView::TimerProc(  HWND hwnd,	UINT uMsg,	UINT_PTR idEvent,	DWORD dwTime)
{
	if (/*s_renderPause ||*/ AfxGetMainWnd()->IsIconic())
	{
		return;
	}
	static bool s_bReding = false;
	s_bReding = true;
	static DWORD lastTime = timeGetTime();
	DWORD eslape = timeGetTime() - lastTime;
	g_CoreSystem.render(eslape);
	g_pEditorView->drawAllQuad();
}

void CUIEditorView::setLayoutSize(LAYOUT_SIZE type)
{
	if (type == LS_800X600)
	{
		::SetRect(&(m_nQuadInfo[QC_LAYOUT_TYPE].quadRect), 0, 0, 800,600);
		g_CoreSystem.setLayout(800);
	}
	else if(type == LS_1024X768)
	{
		::SetRect(&(m_nQuadInfo[QC_LAYOUT_TYPE].quadRect), 0, 0, 1024,768);
		g_CoreSystem.setLayout(1024);
	}
	else
	{
		//LS_SHOW_ALL
		g_CoreSystem.setLayout(0);
	}
	m_nQuadInfo[QC_LAYOUT_TYPE].quadColor = RGB(0,255,0);
	
}

void CUIEditorView::drawAllQuad(void)
{
	CDC* pDC = GetDC();
	INT begin = 0;
	INT end = QC_TYPE_SPLIT;
	if ( !m_bIsShowWindow )
	{
		begin = QC_TYPE_SPLIT + 1;
		end = QC_NUM;
	}
	for (INT i=begin; i<end; i++)
	{
		pDC->Draw3dRect(
			m_nQuadInfo[i].quadRect.left,
			m_nQuadInfo[i].quadRect.top,
			m_nQuadInfo[i].quadRect.right,
			m_nQuadInfo[i].quadRect.bottom,
			m_nQuadInfo[i].quadColor,
			m_nQuadInfo[i].quadColor
			);
	}
	ReleaseDC(pDC);
}

void CUIEditorView::SetRenderFlag(BOOL flag)
{
	s_renderPause = flag;
}

void CUIEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	g_CoreSystem.onSizeChange();
}

void CUIEditorView::OnDestroy()
{
	//m_canSize = FALSE;
	//theApp.m_pRender->UnInitialize();
	timeEndPeriod(1);
	CView::OnDestroy();
}

BOOL CUIEditorView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
	return CView::OnEraseBkgnd(pDC);
}
void CUIEditorView::GetKeyStates(UINT nFlags, UINT & key)
{
	if (nFlags == 0)
	{
		if (GetAsyncKeyState(VK_SHIFT) &0x8000)
		{
			key |= EK_SHIFT;
		}
		if (GetAsyncKeyState(VK_MENU) &0x8000)
		{
			key |= EK_ALT;
		}
		if (GetAsyncKeyState(VK_CONTROL) &0x8000)
		{
			key |= EK_CONTROL;
		}
	}
	else
	{
			if (nFlags & MK_CONTROL)
			{
				key |= EK_CONTROL;
			}
			if (nFlags & MK_ALT)
			{
				key |= EK_ALT;
			}
			if (nFlags & MK_SHIFT)
			{
				key |= EK_SHIFT;
			}
	}
	
}

bool CUIEditorView::isControlDown(void)
{
	return GetAsyncKeyState(VK_CONTROL) &0x8000;
}
bool CUIEditorView::isAltlDown(void)
{
	return GetAsyncKeyState(VK_MENU) &0x8000;
}
bool CUIEditorView::isShiftDown(void)
{
	return GetAsyncKeyState(VK_SHIFT) &0x8000;
}
void CUIEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_lButtonDown = TRUE;
	g_CoreSystem.getCEGUISystem()->injectMousePosition(point.x, point.y);
	g_CoreSystem.getCEGUISystem()->injectMouseButtonDown(CEGUI::LeftButton);
	if (m_beginDraw == DST_DRAW_PREPARE)
	{
		m_beginDraw = DST_DRAW_BEGIN;
	}

	//如果要创建窗口
	if( getCreateWindowFlag() && m_bProcessing == false && getShowMode() )
	{
		if( g_DataPool.OnCreateNewWindow(m_pSelectedWindow) )
		{
			setProcessing(true);
			CEGUI::Rect rect = m_pCreatedWindow->getPixelRect();
			m_pCreatedWindow->setPosition(CEGUI::Absolute,CEGUI::Point(point.x - rect.d_left, point.y -rect.d_top));
			//setWindowSelected(m_pCreatedWindow->getName().c_str());
		}
	}
	CView::OnLButtonDown(nFlags, point);
}

void CUIEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	//查看模式是否正确
	if ( !getShowMode() ) return;

	if (m_lButtonDown)
	{
		setProcessing(false);
		m_lButtonDown = FALSE;
		m_ptMouseMovePos = CPoint(0,0);
		if (getCreateWindowFlag())
		{
			showRenameDlg();
			setCreateWindowFlag(false);
			setCreateWindow(NULL);
			::SetRect(&(m_nQuadInfo[QC_CREATE_WINDOW].quadRect),0,0,0,0);	
			SetCursor(m_BorderResponse[BRP_SELECT_WINDOW_MIDDLE_MIDDLE_QUAD].cursor);
		}
	}
	g_CoreSystem.getCEGUISystem()->injectMousePosition(point.x, point.y);
	g_CoreSystem.getCEGUISystem()->injectMouseButtonUp(CEGUI::LeftButton);
	while( ShowCursor(TRUE) < 0 );
	CView::OnLButtonUp(nFlags, point);
}

void CUIEditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	g_CoreSystem.getCEGUISystem()->injectMouseButtonDown(CEGUI::LeftButton);

	//找到鼠标点击的窗口
	CEGUI::Window* pWin = CEGUI::System::getSingleton().getWindowContainingMouse();
	if(pWin)
	{
		//检测窗口是否可以显示函数对话框
		BOOL bShow = g_DataPool.m_scriptModule.DoFunction("canShowFunctionDlg",pWin->getWidgetType().c_str(),NULL);
		//显示对话框
		if(bShow)
		{
			CFunctionDlg dlg;
			CString szString,szName;
			szName = pWin->getName().c_str();
			szString = szName;
			szString += "_OnClicked";
			dlg.SetLeftFunction(szString);
			szString = szName;
			szString += "_OnRClicked";
			dlg.SetRightFunction(szString);
			dlg.SetWindowName(szName);
			dlg.DoModal();
		}
	}

	CView::OnLButtonDblClk(nFlags, point);
}

void CUIEditorView::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	g_CoreSystem.getCEGUISystem()->injectMousePosition(point.x, point.y);
	CView::OnMButtonDblClk(nFlags, point);
}

void CUIEditorView::OnMButtonDown(UINT nFlags, CPoint point)
{
	g_CoreSystem.getCEGUISystem()->injectMousePosition(point.x, point.y);
	g_CoreSystem.getCEGUISystem()->injectMouseButtonDown(CEGUI::MiddleButton);
	CView::OnMButtonDown(nFlags, point);
}

void CUIEditorView::OnMButtonUp(UINT nFlags, CPoint point)
{
	g_CoreSystem.getCEGUISystem()->injectMousePosition(point.x, point.y);
	g_CoreSystem.getCEGUISystem()->injectMouseButtonUp(CEGUI::MiddleButton);
	m_hCursor = LoadCursor(NULL, IDC_ARROW);
	CView::OnMButtonUp(nFlags, point);
}

void CUIEditorView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	g_CoreSystem.getCEGUISystem()->injectMouseButtonDown(CEGUI::RightButton);

	//取消当前选择窗口
	setWindowSelected("");

	//取消当前描绘窗口的功能
	g_DataPool.OnToolBarListWindowTypeChange("");

	CView::OnRButtonDblClk(nFlags, point);
}

void CUIEditorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	g_CoreSystem.getCEGUISystem()->injectMousePosition(point.x, point.y);
	g_CoreSystem.getCEGUISystem()->injectMouseButtonDown(CEGUI::RightButton);

	if( getShowMode() == false ) return;

	CMenu menu;
	menu.CreatePopupMenu();
	CEGUI::Window* mouseWindow = g_CoreSystem.getCEGUISystem()->getWindowContainingMouse();
	bool showMenu = false;
	INT menuId = ID_RIGHT_WINDOW_SELECT;
	for ( ; mouseWindow ; mouseWindow = mouseWindow->getParent(),++menuId )
	{
		if (mouseWindow != CEGUI::System::getSingleton().getGUISheet() && !mouseWindow->isAutoWindow())
		{
			menu.AppendMenu(MF_STRING, menuId,mouseWindow->getName().c_str());
			showMenu = true;
		}		
	}
	if (showMenu)
	{
		POINT pos;
		GetCursorPos(&pos);
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,pos.x, pos.y, this);
		g_DataPool.OnSelectWindowChanged(NULL, m_pSelectedWindow);
	}
	CView::OnRButtonDown(nFlags, point);
}

void CUIEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	g_CoreSystem.getCEGUISystem()->injectMousePosition(point.x, point.y);
	g_CoreSystem.getCEGUISystem()->injectMouseButtonUp(CEGUI::RightButton);
	CView::OnRButtonUp(nFlags, point);
}
void CUIEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	g_CoreSystem.getCEGUISystem()->injectMousePosition(point.x, point.y);

	if( !getShowMode() ) return;

	//如果是创建窗口的过程
	if (getCreateWindowFlag())
	{
		//创建了窗口,那么改变他的大小
		if (m_pCreatedWindow)
		{
			//setWindowSelected(m_pCreatedWindow->getName().c_str());
			m_nResponseType = BRP_CREATE_NEW_WINDOW_QUAD;
		}
		SetCursor(m_BorderResponse[BRP_CREATE_NEW_WINDOW_QUAD].cursor);
	}
	else
	{
		//如果按住了CTRL那么鼠标移动的时候选择窗口
		if ( isControlDown() && getShowMode() )
		{
			CEGUI::Window* mouseWindow = g_CoreSystem.getCEGUISystem()->getWindowContainingMouse();
			if (mouseWindow)
			{
				setWindowSelected(mouseWindow->getName(),false);
			}
		}
		//检测鼠标感应
		if ( !m_bProcessing )
		{
			m_nResponseType = BRP_INVALID;
			for (INT i=0; i<BRP_NUM; i++)
			{
				if( m_BorderResponse[i].hoverRect.PtInRect(point) )
				{
					SetCursor(m_BorderResponse[i].cursor);
					m_nResponseType = (BorderResponsePos)i;
					break;
				}
			}
		}
	}

	//处理拖动和改变查看大小的操作
	if (m_lButtonDown)
	{
		if(m_nResponseType == BRP_SELECT_WINDOW_MIDDLE_MIDDLE_QUAD)
		{
			while( ShowCursor(FALSE) > 0 );
		}
		SetCursor(m_BorderResponse[m_nResponseType].cursor);
		setProcessing(true);
		processMoveAndSizing(point);
	}

	//显示鼠标位置
	CHAR szMouseText[MAX_PATH] = {0};
	sprintf(szMouseText, "鼠标(%d,%d)",point.x, point.y);
	((CMainFrame*)AfxGetMainWnd())->SetStatusText(1,szMouseText);

	updateCurrentOperationStatusText();

	SetFocus();

	CView::OnMouseMove(nFlags, point);
	
}

void CUIEditorView::updateCurrentOperationStatusText(void)
{
	//显示鼠标位置
	CHAR *szMouseText;
	switch(m_nResponseType)
	{
	case BRP_SELECT_WINDOW_LEFT_TOP_QUAD:
		szMouseText = "缩放并调整位置(暂不支持)";
		break;
	case BRP_SELECT_WINDOW_MIDDLE_TOP_QUAD:
		szMouseText = "缩放并调整位置(暂不支持)";
		break;
	case BRP_SELECT_WINDOW_RIGHT_TOP_QUAD:
		szMouseText = "缩放并调整位置(暂不支持)";
		break;
	case BRP_SELECT_WINDOW_LEFT_MIDDLE_QUAD:
		szMouseText = "缩放并调整位置(暂不支持)";
		break;
	case BRP_SELECT_WINDOW_RIGHT_MIDDLE_QUAD:
		szMouseText = "调整宽度";
		break;
	case BRP_SELECT_WINDOW_LEFT_BOTTOM_QUAD:
		szMouseText = "缩放并调整位置(暂不支持)";
		break;
	case BRP_SELECT_WINDOW_MIDDLE_BOTTOM_QUAD:
		szMouseText = "调整高度";
		break;
	case BRP_SELECT_WINDOW_RIGHT_BOTTOM_QUAD:
		szMouseText = "调整宽度和高度";
		break;
		//中间区块,移动
	case BRP_SELECT_WINDOW_MIDDLE_MIDDLE_QUAD:
		szMouseText = "拖动窗口,改变位置";
		break;
	default:
		szMouseText = "";
	}
	((CMainFrame*)AfxGetMainWnd())->SetStatusText(0,szMouseText);
}

#define WINDOW_MOVE_STEP 0.25f

void CUIEditorView::processMoveAndSizing(CPoint point)
{
	switch(m_nResponseType)
	{
	case BRP_SELECT_WINDOW_LEFT_TOP_QUAD:
		moveAndSizingWindow(point,0);
		break;
	case BRP_SELECT_WINDOW_MIDDLE_TOP_QUAD:
		moveAndSizingWindow(point,1);
		break;
	case BRP_SELECT_WINDOW_RIGHT_TOP_QUAD:
		moveAndSizingWindow(point,3);
		break;
	case BRP_SELECT_WINDOW_LEFT_MIDDLE_QUAD:
		moveAndSizingWindow(point,2);
		break;
	case BRP_SELECT_WINDOW_RIGHT_MIDDLE_QUAD:
		sizingWindow(point, 1);
		break;
	case BRP_SELECT_WINDOW_LEFT_BOTTOM_QUAD:
		break;
	case BRP_SELECT_WINDOW_MIDDLE_BOTTOM_QUAD:
		sizingWindow(point,0);
		break;
	case BRP_SELECT_WINDOW_RIGHT_BOTTOM_QUAD:
		sizingWindow(point,2);
		break;
	//中间区块,移动
	case BRP_SELECT_WINDOW_MIDDLE_MIDDLE_QUAD:
		moveWindow(point);
		break;
	case BRP_CREATE_NEW_WINDOW_QUAD:
		sizingWindow(point,2,m_pCreatedWindow);
		setCreateWindowQuad();
		break;
	default:
		return;
	}
}

void CUIEditorView::moveAndSizingWindow(CPoint point, INT type)	//0左上,1上,2左,右上3
{
	CEGUI::Window* pWindow =  m_pSelectedWindow;
	if (pWindow)
	{
		//moveWindow(point);
	}
}
//只改变大小,不改变位置
void CUIEditorView::sizingWindow(CPoint point,INT type,CEGUI::Window* pWin/* = NULL*/)
{
	CEGUI::Window* pWindow =  pWin ? pWin : m_pSelectedWindow;
	if (pWindow)
	{
		CEGUI::Window* pParent = pWindow;
		CEGUI::Size pt(0,0);
		if (pWindow->getParent() == CEGUI::System::getSingleton().getGUISheet())
		{
			CEGUI::Point pos = pWindow->getPixelRect().getPosition();
			pt = CEGUI::Size(point.x - pos.d_x, point.y - pos.d_y);
		}
		else
		{
			while (pParent && pParent->getParent() != CEGUI::System::getSingleton().getGUISheet())
			{
				pParent = pParent->getParent();
			}
			CEGUI::Rect	 rectWindow = pWindow->getPixelRect();
			pt = CEGUI::Size(point.x -rectWindow.getPosition().d_x   ,point.y -rectWindow.getPosition().d_y);
		}
		pWindow->setClippedByParent(true);
		if(type == 0)
		{
			pWindow->setHeight(CEGUI::Absolute,pt.d_height);
		}
		else if (type == 1)
		{
			pWindow->setWidth(CEGUI::Absolute,pt.d_width);
		}
		else
		{
			pWindow->setSize(CEGUI::Absolute, pt);
		}
	}
}

//只改变位置不改变大小
void CUIEditorView::moveWindow(CPoint point,CPoint step,CEGUI::Window* pWin/* = NULL*/)
{
	CEGUI::Window* pWindow =  pWin ? pWin : m_pSelectedWindow;
	if (pWindow)
	{
		//方向键操作
		if ( step.x != 0 || step.y != 0 )
		{
			CEGUI::Point pos = pWindow->getAbsolutePosition();
			pos.d_x += step.x;
			pos.d_y += step.y;
			pWindow->setPosition(CEGUI::Absolute, pos);
		}
		//鼠标操作
		else
		{
			CEGUI::Window* pParent = pWindow;
			CEGUI::Point pt = CEGUI::Point(point.x, point.y);
			if (pWindow->getParent() == CEGUI::System::getSingleton().getGUISheet())
			{
				pParent = pWindow->getParent();
				CEGUI::Point pointWindow = pWindow->getPixelRect().getPosition();
				//初始化位置
				if(m_ptMouseMovePos.x == 0 && m_ptMouseMovePos.y == 0)
				{
					m_ptMouseMovePos.x = point.x -  pointWindow.d_x;
					m_ptMouseMovePos.y = point.y -  pointWindow.d_y;
				}
				pt.d_x -= m_ptMouseMovePos.x;
				pt.d_y -= m_ptMouseMovePos.y;
			}
			else
			{
				while (pParent && pParent->getParent() != CEGUI::System::getSingleton().getGUISheet())
				{
					pParent = pParent->getParent();
				}
				CEGUI::Point pointParent = pParent->getPixelRect().getPosition();
				CEGUI::Point pointWindow = pWindow->getPixelRect().getPosition();
				//初始化位置
				if(m_ptMouseMovePos.x == 0 && m_ptMouseMovePos.y == 0)
				{
					m_ptMouseMovePos.x = point.x -  pointWindow.d_x;
					m_ptMouseMovePos.y = point.y -  pointWindow.d_y;
				}
				pt = CEGUI::Point(point.x-pointParent.d_x /*+pointWindow.d_x*/ - m_ptMouseMovePos.x, 
					point.y - pointParent.d_y/* +pointWindow.d_y*/ - m_ptMouseMovePos.y);
			}
			pWindow->setClippedByParent(true);
			pWindow->setPosition(CEGUI::Absolute, pt);
		}
	}
}

BOOL CUIEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UINT op = zDelta > 0 ?OP_BUTTON_SCROLLDOWN : OP_BUTTON_SCROLLUP;
	if (zDelta < 0)
	{
		zDelta = -zDelta;
	}
	g_CoreSystem.getCEGUISystem()->injectMouseWheelChange(zDelta);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
void CUIEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//g_CoreSystem.getCEGUISystem()->injectKeyDown(nChar);
	processArrowMove(nChar, nRepCnt);
	//VK_BACK策划容易误操作,所以屏蔽之by方远
	if(nChar == VK_DELETE /*|| nChar == VK_BACK*/)
	{
		deleteSelectedWindow();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CUIEditorView::deleteSelectedWindow(void)
{
	if(m_pSelectedWindow)
	{
		CEGUI::String szDelName = m_pSelectedWindow->getName();
		g_DataPool.OnDeleteWindow(szDelName.c_str());
		setWindowSelected("");
		CEGUI::WindowManager::getSingleton().destroyWindow(szDelName);
	}
}

void CUIEditorView::processArrowMove(UINT nChar, UINT cnt)
{
	CPoint point = CPoint(0,0);
	switch(nChar)
	{
	case VK_LEFT:
		point.x = -2;
		break;
	case VK_RIGHT:
		point.x = 2;
		break;
	case VK_UP:
		point.y = -2;
		break;
	case VK_DOWN:
		point.y = 2;
		break;
	default:
		return;
	}
	moveWindow(point,point);
}

void CUIEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UINT key = EK_KEYUP;
	GetKeyStates(0, key);
	if (nChar == VK_CONTROL)
	{
		g_DataPool.OnSelectWindowChanged(NULL, m_pSelectedWindow);
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CUIEditorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	g_CoreSystem.getCEGUISystem()->injectChar(nChar);
	CView::OnChar(nChar, nRepCnt, nFlags);
}



void CUIEditorView::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CView::OnMenuSelect(nItemID, nFlags, hSysMenu);
	
	char szString[256] = {0};
	GetMenuString(hSysMenu, nItemID, szString, 255, 0);
	if(strlen(szString) > 0)
	{
		setWindowSelected((CEGUI::utf8*)szString,false);
	}
}

void CUIEditorView::setWindowSelected(const CEGUI::String& name,bool addEvent /*= true*/)
{
	try
	{
		if( name.length()> 0 && name != m_nSelectWindowName)
		{
			if(name == "___Shower_Window____") return;
			CEGUI::Window* pWindow = CEGUI::WindowManager::getSingleton().getWindow(name);
			CEGUI::Window* pOldSel = NULL;
			if (pWindow && !pWindow->isAutoWindow())
			{
				if(m_nSelectWindowName.length() > 0)
				{
					pOldSel = CEGUI::WindowManager::getSingleton().getWindow(m_nSelectWindowName);
					if (pOldSel)
					{
						for(;pOldSel && pOldSel != CEGUI::System::getSingleton().getGUISheet(); pOldSel = pOldSel->getParent())
						{
							pOldSel->unsubscribeEvent(CEGUI::Window::EventMoved, CEGUI::Event::Subscriber(&CUIEditorView::handleSelectedWindowMoved, this));
						}
					}
				}
				//pWindow->moveToFront();
				m_nSelectWindowName = pWindow->getName();
				CEGUI::Window* pSet = pWindow;
				for (; pSet&& pSet !=CEGUI::System::getSingleton().getGUISheet(); pSet = pSet->getParent() )
				{
					pSet->subscribeEvent(CEGUI::Window::EventMoved, CEGUI::Event::Subscriber(&CUIEditorView::handleSelectedWindowMoved, this));
				}
				CEGUI::Rect rect = pWindow->getPixelRect();
				setSelectWindowPos(CRect(rect.d_left, rect.d_top, rect.getWidth(), rect.getHeight()));
				m_ptMouseMovePos = CPoint(0,0);			
			}
			m_pSelectedWindow = pWindow;
			updateCurrentWindowStatusText();
			if (addEvent)
			{
				g_DataPool.OnSelectWindowChanged(pOldSel,pWindow);
			}
		}
		else if (name.length() == 0)
		{
			if (addEvent)
			{
				g_DataPool.OnSelectWindowChanged(m_pSelectedWindow,NULL);
			}
			m_nSelectWindowName = "";
			setSelectWindowPos(CRect(), true);
			m_pSelectedWindow = NULL;
		}
	}
	catch(CEGUI::UnknownObjectException& e)
	{
		
	}
}

void CUIEditorView::updateCurrentWindowStatusText(void)
{
	if (m_pSelectedWindow)
	{
		CHAR szText[MAX_PATH] = {0};
		CEGUI::Rect rect = m_pSelectedWindow->getRect(CEGUI::Absolute);
		sprintf(szText, "当前窗口:%s 位置:(%d,%d) 大小:(%d,%d)",m_pSelectedWindow->getName().c_str(),
			(INT)rect.getPosition().d_x,(INT)rect.getPosition().d_y,
			(INT)rect.getWidth(), (INT)rect.getHeight());
		((CMainFrame*)AfxGetMainWnd())->SetStatusText(2,szText);

		sprintf(szText, "当前窗口类型:%s",m_pSelectedWindow->getWidgetType().c_str());
		((CMainFrame*)AfxGetMainWnd())->SetStatusText(3,szText);
	}
}

bool CUIEditorView::handleSelectedWindowMoved(const CEGUI::EventArgs &e)
{
	if(m_pSelectedWindow)
	{
		CEGUI::Rect rect = m_pSelectedWindow->getPixelRect();
		setSelectWindowPos(CRect(rect.d_left, rect.d_top, rect.getWidth(), rect.getHeight()));
		updateCurrentWindowStatusText();
		GetRightPropList()->UpdateBasePosAndSizeProperty(m_pSelectedWindow);
		return true;
	}
	return false;
}

#define BORDER_WIDTH 8

void CUIEditorView::setSelectWindowPos(CRect rect, BOOL clear /*= FALSE*/)
{
	if(clear)
	{
		memset(&(m_nQuadInfo[QC_SELECT_WINDOW]), 0 , sizeof(m_nQuadInfo[QC_SELECT_WINDOW]));
		return;
	}
	//主选择对象
	m_nQuadInfo[QC_SELECT_WINDOW].quadRect = rect;
	m_nQuadInfo[QC_SELECT_WINDOW].quadColor = RGB(255,0,0);

	//设置感应位置
	m_BorderResponse[BRP_SELECT_WINDOW_LEFT_TOP_QUAD].hoverRect.SetRect(
		CPoint(rect.left-BORDER_WIDTH,rect.top -BORDER_WIDTH), CPoint(rect.left+BORDER_WIDTH,rect.top +BORDER_WIDTH));

	m_BorderResponse[BRP_SELECT_WINDOW_MIDDLE_TOP_QUAD].hoverRect.SetRect(
		CPoint(rect.left+BORDER_WIDTH,rect.top -BORDER_WIDTH), CPoint(rect.left +rect.right-BORDER_WIDTH,rect.top +BORDER_WIDTH));

	m_BorderResponse[BRP_SELECT_WINDOW_RIGHT_TOP_QUAD].hoverRect.SetRect(
		CPoint(rect.left +rect.right-BORDER_WIDTH,rect.top -BORDER_WIDTH), CPoint(rect.left +rect.right+BORDER_WIDTH,rect.top +BORDER_WIDTH));

	m_BorderResponse[BRP_SELECT_WINDOW_LEFT_MIDDLE_QUAD].hoverRect.SetRect(
		CPoint(rect.left -BORDER_WIDTH,rect.top +BORDER_WIDTH), CPoint(rect.left +BORDER_WIDTH,rect.top+ rect.bottom -BORDER_WIDTH));

	m_BorderResponse[BRP_SELECT_WINDOW_RIGHT_MIDDLE_QUAD].hoverRect.SetRect(
		CPoint(rect.left + rect.right-BORDER_WIDTH,rect.top +BORDER_WIDTH), CPoint(rect.left + rect.right +BORDER_WIDTH,rect.top +rect.bottom -BORDER_WIDTH));

	m_BorderResponse[BRP_SELECT_WINDOW_LEFT_BOTTOM_QUAD].hoverRect.SetRect(
		CPoint(rect.left -BORDER_WIDTH,rect.top + rect.bottom- BORDER_WIDTH), CPoint(rect.left +BORDER_WIDTH,rect.top +rect.bottom +BORDER_WIDTH));

	m_BorderResponse[BRP_SELECT_WINDOW_MIDDLE_BOTTOM_QUAD].hoverRect.SetRect(
		CPoint(rect.left + BORDER_WIDTH,rect.top + rect.bottom -BORDER_WIDTH), CPoint(rect.left + rect.right -BORDER_WIDTH,rect.top +rect.bottom +BORDER_WIDTH));	

	m_BorderResponse[BRP_SELECT_WINDOW_RIGHT_BOTTOM_QUAD].hoverRect.SetRect(
		CPoint(rect.left + rect.right - BORDER_WIDTH,rect.top + rect.bottom -BORDER_WIDTH), CPoint(rect.left + rect.right +BORDER_WIDTH,rect.top +rect.bottom +BORDER_WIDTH));	

	m_BorderResponse[BRP_SELECT_WINDOW_MIDDLE_MIDDLE_QUAD].hoverRect.SetRect(
		CPoint(rect.left +  BORDER_WIDTH,rect.top + BORDER_WIDTH), CPoint(rect.left + rect.right -BORDER_WIDTH,rect.top +rect.bottom -BORDER_WIDTH));	

}

void CUIEditorView::setCreateWindowQuad()
{
	if(m_pCreatedWindow)
	{
		CEGUI::Rect rect = m_pCreatedWindow->getPixelRect();
		::SetRect(&(m_nQuadInfo[QC_CREATE_WINDOW].quadRect),rect.d_left, rect.d_top, rect.d_right - rect.d_left, rect.d_bottom - rect.d_top);	
	}
}

BOOL CUIEditorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//if(m_hCursor)
	//{
	//	SetCursor(m_hCursor);
	//}
	//else
	{
		CView::OnSetCursor(pWnd, nHitTest, message);
	}
	return  TRUE; 
}

void CUIEditorView::SetUserCursor(HCURSOR cur)
{
	m_beginDraw = DST_DRAW_PREPARE;
	m_hCursor = cur;
}

//初始化Ogre，CEGUI
void CUIEditorView::initialize(void)
{
	if ( !m_binitialize )
	{
		g_CoreSystem.initialize(GetSafeHwnd());
		setLayoutSize(LS_800X600);
		m_binitialize = true;
	}
}

void CUIEditorView::onRenameSelectedWindow(const CString& name, const CEGUI::Window* pWho)
{
	if( pWho == m_pSelectedWindow )
	{
		m_nSelectWindowName = (CEGUI::utf8*)mbcs_to_utf8(name.GetString());
	}
}

void CUIEditorView::setImageQuad(const CRect & rect)
{
	::SetRect(&(m_nQuadInfo[QC_CURRENT_IMAGE].quadRect),rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);	
}

void CUIEditorView::showRenameDlg()
{
	CCreateWindowDlg dlg(TRUE, g_DataPool.GetCreateWindowDefaultName(m_pCreatedWindow));

	if (dlg.DoModal() == IDCANCEL)
	{
		//删除创建窗口
		if (m_pCreatedWindow)
		{
			g_DataPool.OnDeleteWindow(m_pCreatedWindow->getName().c_str());
			CEGUI::WindowManager::getSingleton().destroyWindow(m_pCreatedWindow);
			m_pCreatedWindow = NULL;
		}
		return ;
	}


	CString name ;
	name = dlg.m_szDefaultName;

	//设置窗口的名字
	g_DataPool.OnPropertyChange("Name", name, m_pCreatedWindow);
}