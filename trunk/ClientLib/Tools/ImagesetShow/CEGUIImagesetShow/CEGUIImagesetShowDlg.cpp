// CEGUIImagesetShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CEGUIImagesetShow.h"
#include "CEGUIImagesetShowDlg.h"
#include ".\ceguiimagesetshowdlg.h"
#include "Imageset_XmlHandler.h"
#include "ImagesetTinyXMLParser.h"
#include "CEGUIImagesetShowTGA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCEGUIImagesetShowDlg 对话框



CCEGUIImagesetShowDlg::CCEGUIImagesetShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCEGUIImagesetShowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCEGUIImagesetShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CANVAS, m_staCanvas);
	DDX_Control(pDX, IDC_LIST_IMAGE, m_listImage);
}

BEGIN_MESSAGE_MAP(CCEGUIImagesetShowDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_RADIO_RGB, OnBnClickedRadioRgb)
	ON_BN_CLICKED(IDC_RADIO_ALPHA, OnBnClickedRadioAlpha)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IMAGE, OnLvnItemchangedListImage)
	ON_BN_CLICKED(IDC_BUTTON_FRESH, OnBnClickedButtonFresh)
END_MESSAGE_MAP()


// CCEGUIImagesetShowDlg 消息处理程序

BOOL CCEGUIImagesetShowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->CheckRadioButton(IDC_RADIO_RGB, IDC_RADIO_ALPHA, IDC_RADIO_RGB);
	m_listImage.InsertColumn(0, "Name", LVCFMT_LEFT, 130);
	m_listImage.InsertColumn(1, "Rect", LVCFMT_LEFT, 130);
	::SetWindowLong(m_listImage.GetSafeHwnd(), GWL_STYLE, ::GetWindowLong(m_listImage.GetSafeHwnd(), GWL_STYLE)|LVS_SHOWSELALWAYS);
	m_listImage.SetExtendedStyle(m_listImage.GetExtendedStyle() | LVS_EX_FULLROWSELECT );

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CCEGUIImagesetShowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCEGUIImagesetShowDlg::OnPaint() 
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
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CCEGUIImagesetShowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCEGUIImagesetShowDlg::OnBnClickedButtonBrowse()
{
	CFileDialog dlg(TRUE, "xml", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "CEGUI Imageset fil(*.imageset.xml)|*.imageset.xml|All Files(*.*)|*.*||");
	if(IDOK != dlg.DoModal()) return;

	//open file
	m_theImageset.m_strImagesetFileName = dlg.GetPathName();
	this->SetDlgItemText(IDC_EDIT_FILENAME, dlg.GetPathName());

	//刷新
	_fresh();
}

void CCEGUIImagesetShowDlg::_fresh(void)
{
	//释放旧文件
	m_theImageset.imageVector.clear();
	delete m_theImageset.m_pImageFile; m_theImageset.m_pImageFile=0;

	//解析imageset.xml
	CImagesetItemSet_xmlHandler handler(&m_theImageset);
	CImagesetTinyXMLParser parser;
	parser.parseXMLFile(handler, m_theImageset.m_strImagesetFileName.c_str(), "");

	//路径纠正
	if(!::PathFileExists(m_theImageset.m_strImageFileName.c_str()))
	{
		if(::PathFileExists(::PathFindFileName(m_theImageset.m_strImageFileName.c_str())))	//尝试文件名
		{
			m_theImageset.m_strImageFileName = ::PathFindFileName(m_theImageset.m_strImageFileName.c_str());
		}
		else
		{
			//尝试加入全路径
			char szTemp[MAX_PATH]={0};
			::GetCurrentDirectory(MAX_PATH, szTemp);
			::PathRemoveFileSpec(szTemp);
			::PathAppend(szTemp, m_theImageset.m_strImageFileName.c_str());
			if(::PathFileExists(szTemp))
			{
				m_theImageset.m_strImageFileName = szTemp;
			}
			else
			{
				CString strErrorMsg;
				strErrorMsg.Format("无法定位图像文件%s!", m_theImageset.m_strImageFileName.c_str());
				MessageBox(strErrorMsg, 0, MB_OK|MB_ICONSTOP);
				return;
			}
		}
	}
	
	//加载image
	const char* pszFileExt = ::PathFindExtension(m_theImageset.m_strImageFileName.c_str());
	if(stricmp(pszFileExt, ".tga") == 0)
	{
		m_theImageset.m_pImageFile = new CTGAFile;
		if(!m_theImageset.m_pImageFile->loadFromFile(m_theImageset.m_strImageFileName.c_str()))
		{
			delete m_theImageset.m_pImageFile; m_theImageset.m_pImageFile=0;

			CString strErrorMsg;
			strErrorMsg.Format("打开文件%s失败!", m_theImageset.m_strImageFileName.c_str());
			MessageBox(strErrorMsg, 0, MB_OK|MB_ICONSTOP);
			return;
		}
	}
	else
	{
		CString strErrorMsg;
		strErrorMsg.Format("目前不支持%s文件！", pszFileExt);
		MessageBox(strErrorMsg, 0, MB_OK|MB_ICONSTOP);
		return;
	}

	//刷新列表
	m_listImage.DeleteAllItems();
	for(int i=0; i<(int)m_theImageset.imageVector.size(); i++)
	{
		ImagesetImage& image = m_theImageset.imageVector[i];
		m_listImage.InsertItem(i, image.strName.c_str());

		CString strRect;
		strRect.Format("%d,%d-%d,%d", image.xPos, image.yPos, image.width, image.height);
		m_listImage.SetItemText(i, 1, strRect);
	}


	//刷新图像
	m_staCanvas.setImageset(&m_theImageset);
	m_staCanvas.InvalidateRect(0);

	//INFO
	CString strInfo;
	strInfo.Format("ImageFile: %s\r\nImageCounts:%d", 
		m_theImageset.m_strImageFileName.c_str(), 
		(int)m_theImageset.imageVector.size());
	this->SetDlgItemText(IDC_STATIC_INFO, strInfo);
}


void CCEGUIImagesetShowDlg::OnBnClickedRadioRgb()
{
	// TODO: Add your control notification handler code here
	m_staCanvas.freshType(CCanvasWnd::FT_RGB);
}

void CCEGUIImagesetShowDlg::OnBnClickedRadioAlpha()
{
	m_staCanvas.freshType(CCanvasWnd::FT_ALPHA);
}

void CCEGUIImagesetShowDlg::OnLvnItemchangedListImage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	_onItemSelectChanged();
	*pResult = 0;
}

void CCEGUIImagesetShowDlg::_onItemSelectChanged(void)
{
	int counts = m_listImage.GetItemCount();
	for(int i=0; i<counts; i++)
	{
		UINT state = m_listImage.GetItemState(i, LVIS_SELECTED);

		if(state == LVIS_SELECTED)
		{
			m_staCanvas.setFocus(i);
			return;
		}
	}
}


void CCEGUIImagesetShowDlg::OnBnClickedButtonFresh()
{
	_fresh();
}
