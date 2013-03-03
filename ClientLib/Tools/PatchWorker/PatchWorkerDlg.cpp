// PatchWorkerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PatchWorker.h"
#include "PatchWorkerDlg.h"
#include ".\patchworkerdlg.h"
#include "CryptoKey.h"
#include "NewVersionDlg.h"
#include "FileFilterDlg.h"


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


// CPatchWorkerDlg 对话框
CPatchWorkerDlg::CPatchWorkerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatchWorkerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPatchWorkerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VERSION_NEW, m_editVersionNew);
	DDX_Control(pDX, IDC_EDIT_VERSION_OLD, m_editVersionOld);
}

BEGIN_MESSAGE_MAP(CPatchWorkerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_CTLCOLORSTATIC, OnStaticCtlColor)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FOLD_OLD, OnBnClickedButtonFoldOld)
	ON_BN_CLICKED(IDC_BUTTON_FOLD_NEW, OnBnClickedButtonFoldNew)
	ON_BN_CLICKED(IDC_BUTTON_GEN, OnBnClickedButtonGen)
	ON_EN_UPDATE(IDC_EDIT_VERSION_NEW, OnEnUpdateEditVersionNew)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CPatchWorkerDlg 消息处理程序

BOOL CPatchWorkerDlg::OnInitDialog()
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
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDC_GEN_KEY, "产生密钥/公钥对");
			pSysMenu->AppendMenu(MF_STRING, IDC_EXPORT_PUBKEY, "导出公钥");
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	TryLoadKeyFromFile(".\\pair_key.key");
	m_keyMath.ImportPairKey(g_byPairKey, g_nPairKeySize);

	m_hLargeFont.CreateFont(24,0,0,0,
		FW_NORMAL,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FF_MODERN | FIXED_PITCH | FF_SWISS,
		"宋体");

	m_hYellowBrush.CreateSolidBrush(RGB(255, 255, 0));

	GetDlgItem(IDC_STATIC_VER_OLD)->SetFont(&m_hLargeFont);
	GetDlgItem(IDC_STATIC_VER_NEW)->SetFont(&m_hLargeFont);

	GetDlgItem(IDC_EDIT_VERSION_OLD)->SetFont(&m_hLargeFont);
	GetDlgItem(IDC_EDIT_VERSION_NEW)->SetFont(&m_hLargeFont);

	GetDlgItem(IDC_EDIT_VERSION_OLD)->SetWindowText("");
	GetDlgItem(IDC_EDIT_VERSION_NEW)->SetWindowText("");

	GetDlgItem(IDC_BUTTON_FOLD_OLD)->SetFocus();
	return FALSE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CPatchWorkerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if(nID == IDC_GEN_KEY)
	{
		GenePairKey();
	}
	else if(nID == IDC_EXPORT_PUBKEY)
	{
		ExportPubKey();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPatchWorkerDlg::OnPaint() 
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
HCURSOR CPatchWorkerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPatchWorkerDlg::OnBnClickedButtonFoldOld()
{
	SetDlgItemText(IDC_BUTTON_FOLD_OLD, "...");
	SetDlgItemText(IDC_BUTTON_FOLD_OLD, "");

	//-----------------------------------------------------------
	//选择旧目录
	BROWSEINFO binfo; ZeroMemory(&binfo, sizeof(BROWSEINFO));
	binfo.hwndOwner = GetSafeHwnd();
	binfo.pidlRoot = NULL;
	binfo.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
	binfo.pszDisplayName = NULL;
	binfo.lpszTitle = "选择旧版本所在目录";

	LPITEMIDLIST pRetFold = ::SHBrowseForFolder(&binfo);
	if(!pRetFold) return;

	CHAR szOldPath[MAX_PATH] = {0};
	::SHGetPathFromIDList(pRetFold, szOldPath);

	//-----------------------------------------------------------
	//检查旧目录版本定义文件
	char szVersionFile[MAX_PATH];
	strncpy(szVersionFile, szOldPath, MAX_PATH);
	PathAppend(szVersionFile, "(version)");

	//-----------------------------------------------------------
	//加载目录
	CString strVersion;
	m_fileCollecterOld.ClearWorlk();
	if(!m_fileCollecterOld.CollectPath(szOldPath, szVersionFile, strVersion, &m_keyMath))
	{
		MessageBox("非法的目录!", 0, MB_OK|MB_ICONSTOP);
		return;
	}

	m_editVersionOld.SetWindowText(strVersion);
	SetDlgItemText(IDC_BUTTON_FOLD_OLD, szOldPath);

	UpdateCreateButton();
}

void CPatchWorkerDlg::OnBnClickedButtonFoldNew()
{
	SetDlgItemText(IDC_BUTTON_FOLD_NEW, "...");

	//-----------------------------------------------------------
	//选择新目录
	BROWSEINFO binfo; ZeroMemory(&binfo, sizeof(BROWSEINFO));
	binfo.hwndOwner = GetSafeHwnd();
	binfo.pidlRoot = NULL;
	binfo.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
	binfo.pszDisplayName = NULL;
	binfo.lpszTitle = "选择新版本所在目录";

	LPITEMIDLIST pRetFold = ::SHBrowseForFolder(&binfo);
	if(!pRetFold) return;

	CHAR szNewPath[MAX_PATH] = {0};
	::SHGetPathFromIDList(pRetFold, szNewPath);

	//-----------------------------------------------------------
	//检查新目录是否已经存在版本定义文件
	m_strNewFold = szNewPath;
	char szVersionFile[MAX_PATH];
	strncpy(szVersionFile, szNewPath, MAX_PATH);
	PathAppend(szVersionFile, "(version)");

	//检查该文件
	CString strVersion;
	m_fileCollecterNew.ClearWorlk();
	BOOL bAlreadyDefined = m_fileCollecterNew.CollectPath(szNewPath, szVersionFile, strVersion, &m_keyMath);
	
	//确认版本号
	CNewVersionDlg dlg(strVersion);
	if(IDOK != dlg.DoModal())
	{
		m_fileCollecterNew.ClearWorlk();
		return;
	}

	if(bAlreadyDefined)
	{
		//版本号改变
		if(dlg.GetVersion() != strVersion)
		{
			//生成新版本文件
			m_fileCollecterNew.OutputVersionFile(szVersionFile, dlg.GetVersion(), &m_keyMath);
		}
		m_editVersionNew.SetWindowText(dlg.GetVersion());
		SetDlgItemText(IDC_BUTTON_FOLD_NEW, m_strNewFold);
	}
	//完整分析目录
	else
	{
		//-----------------------------------------------------------
		//创建文件搜集线程
		m_fileCollecterNew.ClearWorlk();

		CWinThread* m_pScanThread = AfxBeginThread(_ScanNewFoldThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		m_dlgProgress.DoModal(m_pScanThread);

		// check thread
		DWORD nExitCode = 0;
		::GetExitCodeThread(m_pScanThread->m_hThread, &nExitCode);

		//-----------------------------------------------------------
		//进程对话框关闭，判断线程是否关闭
		if(STILL_ACTIVE == nExitCode)
		{
			// send terminate command
			m_fileCollecterNew.TerminateCollecting();

			//wait 1 sec...
			if(WAIT_TIMEOUT == ::WaitForSingleObject(m_pScanThread->m_hThread, 1000))
			{
				// force!!
				::TerminateThread(m_pScanThread->m_hThread, 0xC0DE);
			}
		}

		//-----------------------------------------------------------
		//是用户强制终止！
		if(m_dlgProgress.GetUserAbort() || 0xC0DE == nExitCode)
		{
			m_fileCollecterNew.ClearWorlk();
			return;
		}

		//-----------------------------------------------------------
		//生成版本文件
		m_fileCollecterNew.OutputVersionFile(szVersionFile, dlg.GetVersion(), &m_keyMath);

		m_fileCollecterNew.ClearWorlk();
		bAlreadyDefined = m_fileCollecterNew.CollectPath(szNewPath, szVersionFile, strVersion, &m_keyMath);

		m_editVersionNew.SetWindowText( dlg.GetVersion());
		SetDlgItemText(IDC_BUTTON_FOLD_NEW, m_strNewFold);
	}

	UpdateCreateButton();
}

LRESULT CPatchWorkerDlg::OnStaticCtlColor(WPARAM wParam, LPARAM lParam)
{
	HDC hDC = (HDC)wParam;
	HWND hWnd = (HWND)lParam;
	if(	IDC_EDIT_VERSION_OLD == ::GetDlgCtrlID(hWnd) ||
		IDC_EDIT_VERSION_NEW == ::GetDlgCtrlID(hWnd))
	{
		::SetBkColor(hDC, RGB(255, 255, 0));
		::SetTextColor(hDC, RGB(255, 0, 0));
		return (LRESULT)m_hYellowBrush.m_hObject;
	}
	else
		return 0;
}

UINT CPatchWorkerDlg::_ScanNewFoldThread(LPVOID pParam)
{
	((CPatchWorkerDlg*)pParam)->ScanNewFoldThread();
	ExitThread(0);
}

void CPatchWorkerDlg::ScanNewFoldThread(void)
{
	m_fileCollecterNew.SetProgressWnd(m_dlgProgress.GetSafeHwnd());
	bool bret = m_fileCollecterNew.CollectPath(m_strNewFold);

	// notify progress 
	::PostMessage(m_dlgProgress.GetSafeHwnd(), WM_CLOSE, 0, 0);

	if(!bret) 
	{
		::ExitThread(0xC0DE);
	}
}

UINT CPatchWorkerDlg::_GeneratePatchThread(LPVOID pParam)
{
	((CPatchWorkerDlg*)(AfxGetApp()->GetMainWnd()))->GeneratePatchThread((CPatchMaker*)pParam);
	ExitThread(0);
}

void CPatchWorkerDlg::GeneratePatchThread(CPatchMaker* pMaker)
{
	pMaker->SetProgressWnd(m_dlgProgress.GetSafeHwnd());
	bool bret = pMaker->WritePatchFile(&m_keyMath);

	// notify progress 
	::PostMessage(m_dlgProgress.GetSafeHwnd(), WM_CLOSE, 0, 0);

	if(!bret) 
	{
		::ExitThread(0xC0DE);
	}
}

void CPatchWorkerDlg::OnBnClickedButtonGen()
{
	//-----------------------------------------------------------
	//选择新目录
	BROWSEINFO binfo; ZeroMemory(&binfo, sizeof(BROWSEINFO));
	binfo.hwndOwner = GetSafeHwnd();
	binfo.pidlRoot = NULL;
	binfo.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
	binfo.pszDisplayName = NULL;
	binfo.lpszTitle = "选择Patch存储目录";

	LPITEMIDLIST pRetFold = ::SHBrowseForFolder(&binfo);
	if(!pRetFold) return;

	CHAR  szPatchPath[MAX_PATH] = {0};
	::SHGetPathFromIDList(pRetFold, szPatchPath);

	//-----------------------------------------------------------
	//比较目录
	CString strOldVer;
	m_editVersionOld.GetWindowText(strOldVer);

	CString strNewVer;
	m_editVersionNew.GetWindowText(strNewVer);

	if(strNewVer.GetLength() == 0 ||
		strOldVer.GetLength() == 0 ||
		m_fileCollecterNew.GetNodeNumber() == 0) return;

	//Compare
	CPatchMaker maker(m_fileCollecterOld, m_fileCollecterNew, szPatchPath);
	maker.GeneratePatchData( (LPCSTR)m_strFilter );

	//-----------------------------------------------------------
	//创建压缩工作线程
	CWinThread* pGenPatchThread = AfxBeginThread(_GeneratePatchThread, &maker, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_dlgProgress.DoModal(pGenPatchThread);

	// check thread
	DWORD nExitCode = 0;
	::GetExitCodeThread(pGenPatchThread->m_hThread, &nExitCode);

	//-----------------------------------------------------------
	//进程对话框关闭，判断线程是否关闭
	if(STILL_ACTIVE == nExitCode)
	{
		// send terminate command
		maker.TerminateZipping();

		//wait 10 sec...
		if(WAIT_TIMEOUT == ::WaitForSingleObject(pGenPatchThread->m_hThread, 10000))
		{
			// force!!
			::TerminateThread(pGenPatchThread->m_hThread, 0xC0DE);
		}
	}

	//-----------------------------------------------------------
	//是用户强制终止！
	if(m_dlgProgress.GetUserAbort() || 0xC0DE == nExitCode)
	{
		return;
	}

	//成功结束
	char szMsg[MAX_PATH] = {0};
	_snprintf(szMsg, MAX_PATH, "生成补丁文件%s", maker.GetPatchFile().c_str());
	MessageBox(szMsg, 0, MB_OK|MB_ICONINFORMATION);
}

void CPatchWorkerDlg::OnEnUpdateEditVersionNew()
{
	UpdateCreateButton();
}

void CPatchWorkerDlg::UpdateCreateButton(void)
{
	CString strOldVer;
	m_editVersionOld.GetWindowText(strOldVer);

	CString strNewVer;
	m_editVersionNew.GetWindowText(strNewVer);

	GetDlgItem(IDC_BUTTON_GEN)->EnableWindow(
		strNewVer.GetLength() != 0 && 
		strOldVer.GetLength() != 0 &&
		m_fileCollecterNew.GetNodeNumber() != 0);
}

void CPatchWorkerDlg::GenePairKey()
{
	CAxCryptoMath keyMath;

	DWORD dwBufSize = 1024;
	BYTE szTemp[1024] = {0};

	keyMath.CreatePairKey(szTemp, dwBufSize);

	BROWSEINFO binfo; ZeroMemory(&binfo, sizeof(BROWSEINFO));
	binfo.hwndOwner = GetSafeHwnd();
	binfo.pidlRoot = NULL;
	binfo.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
	binfo.pszDisplayName = NULL;
	binfo.lpszTitle = "选择存储目录";

	LPITEMIDLIST pRetFold = ::SHBrowseForFolder(&binfo);
	if(!pRetFold) return;

	CHAR szReturn[MAX_PATH] = {0};
	::SHGetPathFromIDList(pRetFold, szReturn);

	//PairKey
	CHAR szFile[MAX_PATH];
	strncpy(szFile, szReturn, MAX_PATH);
	PathAppend(szFile, "pair_key.key");
	FILE* fp = fopen(szFile, "wb");
	fwrite(szTemp, 1, dwBufSize, fp);
	fclose(fp);

	//PubKey
	dwBufSize = 1024;
	keyMath.ExportPubKey(szTemp, dwBufSize);
	strncpy(szFile, szReturn, MAX_PATH);
	PathAppend(szFile, "pub_key.key");
	fp = fopen(szFile, "wb");
	fwrite(szTemp, 1, dwBufSize, fp);
	fclose(fp);
}

void CPatchWorkerDlg::ExportPubKey()
{
	BROWSEINFO binfo; ZeroMemory(&binfo, sizeof(BROWSEINFO));
	binfo.hwndOwner = GetSafeHwnd();
	binfo.pidlRoot = NULL;
	binfo.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
	binfo.pszDisplayName = NULL;
	binfo.lpszTitle = "选择存储目录";

	LPITEMIDLIST pRetFold = ::SHBrowseForFolder(&binfo);
	if(!pRetFold) return;

	CHAR szReturn[MAX_PATH] = {0};
	::SHGetPathFromIDList(pRetFold, szReturn);

	//PairKey
	CHAR szFile[MAX_PATH];
	strncpy(szFile, szReturn, MAX_PATH);
	PathAppend(szFile, "pub_key.key");
	FILE* fp = fopen(szFile, "wb");
	fwrite(g_byPubKey, 1, g_nPubKeySize, fp);
	fclose(fp);

}

void CPatchWorkerDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if( PathFileExists( "PatchWorker.ini" ) == FALSE ) {
		FILE* fp = fopen( "PatchWorker.ini", "w" );
		if( fp ) {
			fprintf( fp, "[FF]\n" );
			fclose( fp );
		}
	}
	CFileFilterDlg dlg;
	if( dlg.DoModal() == IDOK ) {
		m_strFilter = dlg.m_strText;
		GetDlgItem(IDC_BUTTON1)->SetWindowText( "过滤器设置成功" );
	}
	else {
		m_strFilter.Empty();
	}
}
