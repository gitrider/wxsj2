// PathRegionTransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PathRegionTransfer.h"
#include "PathRegionTransferDlg.h"
#include <PathLibInterface.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PathLib::PathLibRoot* gPathLibRoot = NULL;
PathLib::PathData* gPathData = NULL;


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CPathRegionTransferDlg dialog




CPathRegionTransferDlg::CPathRegionTransferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPathRegionTransferDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPathRegionTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_bTransAllDir);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
	DDX_Control(pDX, IDC_STATUS_TEXT, m_StatusText);
}

BEGIN_MESSAGE_MAP(CPathRegionTransferDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPathRegionTransferDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CPathRegionTransferDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPathRegionTransferDlg message handlers

BOOL CPathRegionTransferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	if (!gPathLibRoot)
		gPathLibRoot = new PathLib::PathLibRoot;

	if (!gPathData)
		gPathData = gPathLibRoot->getPathData();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPathRegionTransferDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPathRegionTransferDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPathRegionTransferDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPathRegionTransferDlg::OnBnClickedButton1()
{
	BOOL bCheck = m_bTransAllDir.GetCheck();

	if (bCheck)
	{
		BROWSEINFO bi;
		char szPathName[MAX_PATH];
		char szTitle[] = "选择路径";
		ZeroMemory(&bi, sizeof(BROWSEINFO));
		bi.hwndOwner = GetSafeHwnd();
		bi.pszDisplayName = szPathName;
		bi.lpszTitle = szTitle;
		bi.ulFlags = 0x0040 ; 
		CString str;
		CString strDir; //选择的目录 

		LPITEMIDLIST idl = SHBrowseForFolder(&bi);
		if(idl == NULL)
		{
			strDir= "";
			return;
		}
		SHGetPathFromIDList(idl, str.GetBuffer(MAX_PATH * 2));
		str.ReleaseBuffer();
		if(str != "" && str.GetAt(str.GetLength() - 1) != '\\')
			str += "\\";
		m_szFolderName = str;
		m_Edit.SetWindowText(m_szFolderName);
		m_szFileName = "";
	}
	else
	{
		CFileDialog dlg(TRUE, "", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"Region文件(*.region)|*.region||", this);

		if (dlg.DoModal() == IDOK)
		{
			std::string szName = dlg.GetPathName().GetString();
			m_szFileName = szName.substr( 0, szName.find_last_of('.') ).c_str();
			m_Edit.SetWindowText(szName.c_str());
			m_szFolderName = "";
		}
	}
}

void CPathRegionTransferDlg::OnBnClickedOk()
{
	//开始转换
	if (m_szFolderName.IsEmpty() == FALSE)
	{
		ProcessFolder(m_szFolderName);
	}
	else if (m_szFileName.IsEmpty() == FALSE)
	{
		ProcessPathFile(m_szFileName.GetString());
	}
	m_StatusText.SetWindowText("转化完毕");
}

void	CPathRegionTransferDlg::ProcessFolder(const CString& szFolderName)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(szFolderName + "\\*.*");
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if( finder.IsDots() ) 
		{
			continue;
		}

		if(finder.IsDirectory())
		{
			CString szFolder = finder.GetFilePath();
			ProcessFolder(szFolder);
		}
		else
		{
			std::string szName =  finder.GetFilePath().MakeLower().GetString();
			if( szName.find(".region") != std::string::npos )
			{
				std::string strName = szName.substr(0,szName.find_last_of('.'));
				ProcessPathFile(strName);
			}
		}
	}
}

void	CPathRegionTransferDlg::ProcessPathFile(const std::string& szName)
{
	try
	{
		if( gPathData->getDataFromRegion(szName, 8) )
		{
			gPathData->exportPathFile(szName + ".CRegion");
			m_StatusText.SetWindowText(("正在转化: " + szName).c_str());
		}
	}
	catch(...)
	{
		m_StatusText.SetWindowText(("转化" + szName + "出现问题,可能是这个文件里没有阻挡区域").c_str());
	}
}