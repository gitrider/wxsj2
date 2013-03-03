// HelperDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Helper.h"
#include "HelperDlg.h"
#include "HelperSystem.h"
#include "HelperItemset.h"
#include "HelperItem.h"
#include ".\helperdlg.h"
#include "HelperDocHostUIHandler.h"
#include "InnerString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHelperDlg 对话框
extern int g_nCodePage;
CHelperDlg::CHelperDlg(CWnd* pParent /*=NULL*/)
	: CBDialog(g_nCodePage==1258? CHelperDlg::IDD_VN : CHelperDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nLeftWidth = 0;
	m_CurShow = INVALID;
}

CHelperDlg::~CHelperDlg()
{
}

void CHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CBDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SPLIT, m_staSplitVer);
	DDX_Control(pDX, IDC_TREE_ITEMS, m_treeItems);
	DDX_Control(pDX, IDC_EXPLORER_HELPER, m_webHelper);
	DDX_Control(pDX, IDC_CLOSE_BTN, m_CloseBtn);
	DDX_Control(pDX, IDC_ALWAYS_ONTOP, m_AlwaysOnTop);
	DDX_Control(pDX, IDC_MINI_BTN, m_MiniBtn);
	DDX_Control(pDX, IDC_STATIC_TILE, m_StaTile);
	DDX_Control(pDX, IDC_STATIC_RANK, m_StaRank);
}

BEGIN_MESSAGE_MAP(CHelperDlg, CBDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ITEMS, OnTvnSelchangedTreeItems)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_ALWAYS_ONTOP, OnBnClickedAlwaysOntop)
	ON_BN_CLICKED(IDC_CLOSE_BTN, OnBnClickedCloseBtn)
	ON_BN_CLICKED(IDC_MINI_BTN, OnBnClickedMiniBtn)
END_MESSAGE_MAP()


// CHelperDlg 消息处理程序

BOOL CHelperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// Reposition all controls
	//CRect rect;
	//m_treeItems.GetClientRect(&rect);
	//m_nLeftWidth = rect.Width();
	m_nLeftWidth = 158;
	//RepositionDlg();
	
	COLORREF clr(RGB(255, 0, 255));
	m_CloseBtn.SetBitmaps(IDB_CLOSE_LIGHT, clr, IDB_CLOSE_NORMAL,clr);
	m_CloseBtn.DrawBorder(FALSE);
	m_CloseBtn.SetWindowText(L"");

	m_MiniBtn.SetBitmaps(IDB_MINI_LIGHT, clr,IDB_MINI_NORMAL,clr);
	m_MiniBtn.DrawBorder(FALSE);
	m_MiniBtn.SetWindowText(L"");

	m_AlwaysOnTop.SetBitmaps(IDB_PIN_LIGHT,clr,IDB_PIN_NORMAL,clr);
	m_AlwaysOnTop.DrawBorder(FALSE);
	m_AlwaysOnTop.SetWindowText(L"");

	// Add hook control btn(Caption button)
//	static CImageList s_ilPin;
//	s_ilPin.Create(IDB_BITMAP_PIN, 16, 5, RGB(255, 255, 255));
//	AddButton(ID_BUTTON_PIN, 16, 16, s_ilPin.m_hImageList, "Stay visible");
//	OnPinButtonPushed();

	// Add standard icons
	m_treeItems.AddIcon(AfxGetApp()->LoadIcon(IDI_ICON_FOLDER_CLOSE));
	m_treeItems.AddIcon(AfxGetApp()->LoadIcon(IDI_ICON_FOLDER_OPEN));
	m_treeItems.AddIcon(AfxGetApp()->LoadIcon(IDI_ICON_DOCUMENT));

	if(g_nCodePage==1258)
	{
		m_font.CreateFont(
			14, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, 
			DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, 
			_T("Arial") );

		m_treeItems.SetFont(&m_font);
	}
	// Refresh item list
	RefreshHelpItems();

	// Go to blank page
	m_webHelper.Navigate(L"about:blank", 0, 0, 0, 0);

	//取得IcustomDoc接口
	//ICustomDoc* pCustDoc = 0;
	//m_webHelper.get_Document()->QueryInterface(&pCustDoc);
	//if(pCustDoc)
	//{
	//	pCustDoc->SetUIHandler(new HelperDocHostUIHandler());
	//	pCustDoc->Release();
	//}

	///设置对话框的背景图片
	SetBitmapStyle(StyleTile);
	SetBitmap(IDB_BITMAP_NEWBKG);
	switch(m_CurShow) {
		case FLAG_SHOW_HELP:
			//m_StaTile.SetBitmap(::LoadBitmap(NULL, MAKEINTRESOURCE(IDB_BITMAP_TITLE)));  
			m_StaTile.ShowWindow(SW_SHOW);
			m_StaRank.ShowWindow(SW_HIDE);
			SetWindowText(getInnerString(ISI_TITLE_HELPER));
			break;
		case FLAG_SHOW_PAIHANG:
			m_StaTile.ShowWindow(SW_HIDE);
			m_StaRank.ShowWindow(SW_SHOW);
			//m_StaTile.SetBitmap(::LoadBitmap(NULL, MAKEINTRESOURCE(IDB_BITMAP_TITLE)));  
			SetWindowText(getInnerString(ISI_TITLE_PAIHANGBANG));
			break;
		default:
			m_StaTile.ShowWindow(SW_SHOW);
			m_StaRank.ShowWindow(SW_HIDE);
			//m_StaTile.SetBitmap(::LoadBitmap(NULL, MAKEINTRESOURCE(IDB_BITMAP_TITLE)));  
			SetWindowText(getInnerString(ISI_TITLE_HELPER));
			break;
	}

	CenterWindow();

	
	m_staSplitVer.ShowWindow(SW_HIDE);

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CHelperDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();

		// Draw grip size
		//DrawGripSize();
	}
}

void CHelperDlg::DrawGripSize(void)
{
	static CImageList s_ilGripSize;
	if(s_ilGripSize.m_hImageList == 0)
	{
		s_ilGripSize.Create(IDB_BITMAP_GRIP_SIZE, 12, 1, RGB(255, 0, 255));
	}

	CRect rect;
	GetWindowRect(&rect);

	CDC *dc = GetWindowDC();
	ImageList_Draw(s_ilGripSize.m_hImageList, 0, dc->m_hDC, rect.Width()-12-3, rect.Height()-12-3, ILD_NORMAL);
	::ReleaseDC(m_hWnd, dc->m_hDC);
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CHelperDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	LPNMHDR pnmh = (LPNMHDR) lParam;

	if(pnmh->code == NM_SW_CHANGED )
	{
		if(pnmh->hwndFrom = GetDlgItem(IDC_STATIC_SPLIT)->GetSafeHwnd())
		{
			//m_nLeftWidth += m_staSplitVer.GetPosOffset();
			RepositionDlg();
			Invalidate();
		}
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CHelperDlg::RepositionDlg(void)
{
	CRect rectClient;
	GetClientRect(&rectClient);

	if(m_treeItems.GetSafeHwnd())
	{
		m_treeItems.MoveWindow(
			5, 
			25, 
			m_nLeftWidth,
			rectClient.Height()- 6  -25);
	}

	if(m_staSplitVer.GetSafeHwnd())
	{
		m_staSplitVer.MoveWindow(
			m_nLeftWidth + 11 + 5, 
			5,
			5, 
			rectClient.Height() - 21);
	}
	

	if(m_webHelper.GetSafeHwnd())
	{
		m_webHelper.MoveWindow(
			m_nLeftWidth /*+70*/+5, 
			25,
			rectClient.Width() - m_nLeftWidth /*- 70*/ -5 -4, 
			rectClient.Height()- 6 -25);
	}

	CBitmap cbmp;
	cbmp.LoadBitmap(IDB_PIN_NORMAL);
	BITMAP bmp;
	cbmp.GetBitmap(&bmp);

	if(m_CloseBtn.GetSafeHwnd())
	{
		m_CloseBtn.MoveWindow(
			rectClient.right-1*bmp.bmWidth-6, 
			5,
			bmp.bmWidth, 
			bmp.bmHeight);
	}

	if(m_AlwaysOnTop.GetSafeHwnd())
	{
		m_AlwaysOnTop.MoveWindow(
			rectClient.right-3*bmp.bmWidth-6, 
			5,
			bmp.bmWidth, 
			bmp.bmHeight);
	}
	CBitmap cbmp1;
	cbmp1.LoadBitmap(IDB_BITMAP_RANK_VN); //g_nCodePage==1258? IDB_BITMAP_TITLE_VN : IDB_BITMAP_TITLE);
	BITMAP bmp1;
	cbmp1.GetBitmap(&bmp1);
	if(m_StaTile.GetSafeHwnd())
	{
		m_StaTile.MoveWindow(
			(rectClient.right-3*bmp.bmWidth-3-bmp1.bmWidth)/2, 
			5, 
			bmp1.bmWidth, 
			bmp1.bmHeight);
	}

	CBitmap cbmp2;
	cbmp2.LoadBitmap(IDB_BITMAP_RANK);
	BITMAP bmp2;
	cbmp2.GetBitmap(&bmp2);
	if(m_StaRank.GetSafeHwnd())
	{
		m_StaRank.MoveWindow(
			(rectClient.right-3*bmp.bmWidth-3-bmp2.bmWidth)/2, 
			5, 
			bmp2.bmWidth, 
			bmp2.bmHeight);
	}
	if(m_MiniBtn.GetSafeHwnd())
	{
		m_MiniBtn.MoveWindow(
			rectClient.right-2*bmp.bmWidth-6, 
			5,
			bmp.bmWidth, 
			bmp.bmHeight);
	}
}

void CHelperDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(GetSafeHwnd())
	{
		RepositionDlg();

		Invalidate();
	}
}
void CHelperDlg::Refresh(FLAG_SHOW ShowWhat)
{
	static int firstIn = 0;
	if(!firstIn)
	{
		m_CurShow = ShowWhat;
		Create(MAKEINTRESOURCE(g_nCodePage==1258 ? IDD_VN : IDD));
		firstIn++;
	}
	if(m_CurShow!=ShowWhat)
	{	
		m_CurShow = ShowWhat;
		RefreshHelpItems();
		m_webHelper.Navigate(L"about:blank", 0, 0, 0, 0);
		SetBitmapStyle(StyleTile);
		switch(ShowWhat) {
		case FLAG_SHOW_HELP:
			m_StaTile.ShowWindow(SW_SHOW);
			m_StaRank.ShowWindow(SW_HIDE);
			SetWindowText(getInnerString(ISI_TITLE_HELPER));
			break;
		case FLAG_SHOW_PAIHANG:
			m_StaTile.ShowWindow(SW_HIDE);
			m_StaRank.ShowWindow(SW_SHOW);
			SetWindowText(getInnerString(ISI_TITLE_PAIHANGBANG));
			break;
		default:
			m_StaTile.ShowWindow(SW_SHOW);
			m_StaRank.ShowWindow(SW_HIDE);
			SetWindowText(getInnerString(ISI_TITLE_HELPER));
			break;
		}
		
		//CenterWindow();

	}
	DWORD ThreadID1;
	DWORD ThreadID2;
	BOOL nRet =0;
	if(this->m_hWnd == ::GetForegroundWindow())
	{
	}
	else
	{
		ThreadID1 =::GetWindowThreadProcessId(m_hWnd,NULL);
		ThreadID2 =::GetWindowThreadProcessId(m_hWnd,NULL);
		if(ThreadID1!=ThreadID2)
		{
			::AttachThreadInput(ThreadID1, ThreadID2, true);
			nRet=::SetForegroundWindow(m_hWnd);
			
			::AttachThreadInput(ThreadID1, ThreadID2, false);
		}
		else
		{
			nRet=::SetForegroundWindow(m_hWnd);
		}
	}
	if(!nRet)
	{
		bool bAlwaysTop = (GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST)!=0;
		//	CheckButton(0, bAlwaysTop);

		if(bAlwaysTop)
		{
			::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		}
		else
		{
			::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		}
	}
	if(this->IsIconic())
	{
		this->ShowWindow(SW_RESTORE);
	}
	else
		this->ShowWindow(SW_SHOW);
	this->CenterWindow();	
	if(ShowWhat==FLAG_SHOW::FLAG_SHOW_PAIHANG)
	{
		//设置选中项
		CHelperItem* pItem = CHelperSystem::GetMe()->GetHelpItemSet()->GetRootItem();
		CHelperItem::ChildBuf& theChild = pItem->GetChildBuf();
	   //if(!theChild) return;
		CHelperItem::ChildBuf::iterator it=theChild.begin();
		
		//goto item
		HTREEITEM hItem = (HTREEITEM)((*it).GetUserData());
		if(0 == hItem) return;

		((CHelperDlg*)AfxGetApp()->GetMainWnd())->GotoHelperItem(hItem);
	}
}
void CHelperDlg::RefreshHelpItems(void)
{
	m_treeItems.DeleteAllItems();
	CHelperItemSet* pTheItemSet = CHelperSystem::GetMe()->GetHelpItemSet();

	// insert root item
	_refreshHelpItem(TVI_ROOT, *(pTheItemSet->GetRootItem()));
}

void CHelperDlg::_refreshHelpItem(HTREEITEM hItem, CHelperItem& helperItem)
{
	// get child buf
	CHelperItem::ChildBuf& theChild = helperItem.GetChildBuf();
	
	CHelperItem::ChildBuf::iterator it;
	// insert root item's child item
	for(it=theChild.begin(); it!=theChild.end(); it++)
	{
		CHelperItem& thisItem = *it;

		// insert this

		HTREEITEM hThisItem;
		if(thisItem.GetChildBuf().empty())
		{
			// It's a document
			hThisItem = m_treeItems.InsertItem(thisItem.GetName().c_str(),hItem, hItem);
		}
		else
		{
			// It's a folder
			hThisItem = m_treeItems.InsertItem(thisItem.GetName().c_str(), hItem, hItem);
		}

		// set user data
		m_treeItems.SetItemData(hThisItem, (DWORD_PTR)&thisItem);
		thisItem.SetUserData((int)hThisItem);

		_refreshHelpItem(hThisItem, thisItem);
	}
}

void CHelperDlg::OnTvnSelchangedTreeItems(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	OnItemSelectChanged();
	*pResult = 0;
}

void CHelperDlg::OnItemSelectChanged(void)
{
	HTREEITEM hItem = m_treeItems.GetSelectedItem();
	if(hItem == NULL) return;
	
	CHelperItem* pHelperItem = (CHelperItem*)(DWORD_PTR)m_treeItems.GetItemData(hItem);
	if(!pHelperItem) return;

	if(pHelperItem->GetURL().empty()) return;

	wchar_t wUrl[MAX_PATH]={0};
	::MultiByteToWideChar(CP_ACP, 0, pHelperItem->GetURL().c_str(), pHelperItem->GetURL().size(), wUrl, MAX_PATH);
	m_webHelper.Navigate(wUrl, 0, 0, 0, 0);
}

void CHelperDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}

BOOL CHelperDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDOK)
	{
		return TRUE;
	}
	else if(LOWORD(wParam) == ID_BUTTON_PIN)
	{
		OnPinButtonPushed();
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CHelperDlg::OnPinButtonPushed(void)
{
	bool bAlwaysTop = (GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST)!=0;
	
	bAlwaysTop = !bAlwaysTop;

//	CheckButton(0, bAlwaysTop);

	if(bAlwaysTop)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	}
	else
	{
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	}

}

void CHelperDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	DestroyWindow();
}

void CHelperDlg::GotoHelperItem(HTREEITEM hItem)
{
	m_treeItems.SelectItem(hItem);
}

LRESULT CHelperDlg::OnWMU_Hitcapbutton(WPARAM wParam, LPARAM lParam)
{
	switch ((UINT)wParam) 
	{
	case 1:	// Button #1 has been hit:
//		OnCapButton((BOOL)lParam);
		break;
	default:
		return 0;;
	}
	return 0; // Allways return 0(LRESULT->void)
}
BEGIN_EVENTSINK_MAP(CHelperDlg, CBDialog)
	ON_EVENT(CHelperDlg, IDC_EXPLORER_HELPER, 250, BeforeNavigate2ExplorerHelper, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CHelperDlg, IDC_EXPLORER_HELPER, 108, ProgressChangeExplorerHelper, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CHelperDlg::BeforeNavigate2ExplorerHelper(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	CStringW strURLIn(URL->bstrVal);
	CStringW strProtol = strURLIn.Mid(0, 7);
	if(strProtol.CompareNoCase(L"anch://")==0)
	{
		*Cancel = TRUE;
		CString strAnchor = CString(strURLIn.Mid(7));
		strAnchor.TrimRight(L"/\\");

		char szAnchor[MAX_PATH]={0};
		::WideCharToMultiByte(CP_ACP, 0, strAnchor, strAnchor.GetLength(), szAnchor, MAX_PATH, 0, 0);
		CHelperSystem::GetMe()->GotoAnchor(szAnchor);
	}
}

void CHelperDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CBDialog::OnLButtonDown(nFlags, point);
	if(point.y < 25)
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION,(LPARAM)&point);
	}
}

void CHelperDlg::OnBnClickedAlwaysOntop()
{
	OnPinButtonPushed();
}

void CHelperDlg::OnBnClickedCloseBtn()
{
	// TODO: Add your control notification handler code here
	PostMessage(WM_QUIT,0);
}

void CHelperDlg::OnBnClickedMiniBtn()
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_MINIMIZE);
}


void CHelperDlg::ProgressChangeExplorerHelper(long Progress, long ProgressMax)
{
	// TODO: Add your message handler code here
	CHelperSystem::GetMe()->GotoAnchor("yxpz.html");
}
