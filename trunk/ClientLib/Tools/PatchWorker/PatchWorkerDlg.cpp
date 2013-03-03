// PatchWorkerDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CPatchWorkerDlg �Ի���
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


// CPatchWorkerDlg ��Ϣ�������

BOOL CPatchWorkerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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
			pSysMenu->AppendMenu(MF_STRING, IDC_GEN_KEY, "������Կ/��Կ��");
			pSysMenu->AppendMenu(MF_STRING, IDC_EXPORT_PUBKEY, "������Կ");
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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
		"����");

	m_hYellowBrush.CreateSolidBrush(RGB(255, 255, 0));

	GetDlgItem(IDC_STATIC_VER_OLD)->SetFont(&m_hLargeFont);
	GetDlgItem(IDC_STATIC_VER_NEW)->SetFont(&m_hLargeFont);

	GetDlgItem(IDC_EDIT_VERSION_OLD)->SetFont(&m_hLargeFont);
	GetDlgItem(IDC_EDIT_VERSION_NEW)->SetFont(&m_hLargeFont);

	GetDlgItem(IDC_EDIT_VERSION_OLD)->SetWindowText("");
	GetDlgItem(IDC_EDIT_VERSION_NEW)->SetWindowText("");

	GetDlgItem(IDC_BUTTON_FOLD_OLD)->SetFocus();
	return FALSE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPatchWorkerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CPatchWorkerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPatchWorkerDlg::OnBnClickedButtonFoldOld()
{
	SetDlgItemText(IDC_BUTTON_FOLD_OLD, "...");
	SetDlgItemText(IDC_BUTTON_FOLD_OLD, "");

	//-----------------------------------------------------------
	//ѡ���Ŀ¼
	BROWSEINFO binfo; ZeroMemory(&binfo, sizeof(BROWSEINFO));
	binfo.hwndOwner = GetSafeHwnd();
	binfo.pidlRoot = NULL;
	binfo.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
	binfo.pszDisplayName = NULL;
	binfo.lpszTitle = "ѡ��ɰ汾����Ŀ¼";

	LPITEMIDLIST pRetFold = ::SHBrowseForFolder(&binfo);
	if(!pRetFold) return;

	CHAR szOldPath[MAX_PATH] = {0};
	::SHGetPathFromIDList(pRetFold, szOldPath);

	//-----------------------------------------------------------
	//����Ŀ¼�汾�����ļ�
	char szVersionFile[MAX_PATH];
	strncpy(szVersionFile, szOldPath, MAX_PATH);
	PathAppend(szVersionFile, "(version)");

	//-----------------------------------------------------------
	//����Ŀ¼
	CString strVersion;
	m_fileCollecterOld.ClearWorlk();
	if(!m_fileCollecterOld.CollectPath(szOldPath, szVersionFile, strVersion, &m_keyMath))
	{
		MessageBox("�Ƿ���Ŀ¼!", 0, MB_OK|MB_ICONSTOP);
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
	//ѡ����Ŀ¼
	BROWSEINFO binfo; ZeroMemory(&binfo, sizeof(BROWSEINFO));
	binfo.hwndOwner = GetSafeHwnd();
	binfo.pidlRoot = NULL;
	binfo.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
	binfo.pszDisplayName = NULL;
	binfo.lpszTitle = "ѡ���°汾����Ŀ¼";

	LPITEMIDLIST pRetFold = ::SHBrowseForFolder(&binfo);
	if(!pRetFold) return;

	CHAR szNewPath[MAX_PATH] = {0};
	::SHGetPathFromIDList(pRetFold, szNewPath);

	//-----------------------------------------------------------
	//�����Ŀ¼�Ƿ��Ѿ����ڰ汾�����ļ�
	m_strNewFold = szNewPath;
	char szVersionFile[MAX_PATH];
	strncpy(szVersionFile, szNewPath, MAX_PATH);
	PathAppend(szVersionFile, "(version)");

	//�����ļ�
	CString strVersion;
	m_fileCollecterNew.ClearWorlk();
	BOOL bAlreadyDefined = m_fileCollecterNew.CollectPath(szNewPath, szVersionFile, strVersion, &m_keyMath);
	
	//ȷ�ϰ汾��
	CNewVersionDlg dlg(strVersion);
	if(IDOK != dlg.DoModal())
	{
		m_fileCollecterNew.ClearWorlk();
		return;
	}

	if(bAlreadyDefined)
	{
		//�汾�Ÿı�
		if(dlg.GetVersion() != strVersion)
		{
			//�����°汾�ļ�
			m_fileCollecterNew.OutputVersionFile(szVersionFile, dlg.GetVersion(), &m_keyMath);
		}
		m_editVersionNew.SetWindowText(dlg.GetVersion());
		SetDlgItemText(IDC_BUTTON_FOLD_NEW, m_strNewFold);
	}
	//��������Ŀ¼
	else
	{
		//-----------------------------------------------------------
		//�����ļ��Ѽ��߳�
		m_fileCollecterNew.ClearWorlk();

		CWinThread* m_pScanThread = AfxBeginThread(_ScanNewFoldThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		m_dlgProgress.DoModal(m_pScanThread);

		// check thread
		DWORD nExitCode = 0;
		::GetExitCodeThread(m_pScanThread->m_hThread, &nExitCode);

		//-----------------------------------------------------------
		//���̶Ի���رգ��ж��߳��Ƿ�ر�
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
		//���û�ǿ����ֹ��
		if(m_dlgProgress.GetUserAbort() || 0xC0DE == nExitCode)
		{
			m_fileCollecterNew.ClearWorlk();
			return;
		}

		//-----------------------------------------------------------
		//���ɰ汾�ļ�
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
	//ѡ����Ŀ¼
	BROWSEINFO binfo; ZeroMemory(&binfo, sizeof(BROWSEINFO));
	binfo.hwndOwner = GetSafeHwnd();
	binfo.pidlRoot = NULL;
	binfo.ulFlags = BIF_NEWDIALOGSTYLE|BIF_RETURNONLYFSDIRS;
	binfo.pszDisplayName = NULL;
	binfo.lpszTitle = "ѡ��Patch�洢Ŀ¼";

	LPITEMIDLIST pRetFold = ::SHBrowseForFolder(&binfo);
	if(!pRetFold) return;

	CHAR  szPatchPath[MAX_PATH] = {0};
	::SHGetPathFromIDList(pRetFold, szPatchPath);

	//-----------------------------------------------------------
	//�Ƚ�Ŀ¼
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
	//����ѹ�������߳�
	CWinThread* pGenPatchThread = AfxBeginThread(_GeneratePatchThread, &maker, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_dlgProgress.DoModal(pGenPatchThread);

	// check thread
	DWORD nExitCode = 0;
	::GetExitCodeThread(pGenPatchThread->m_hThread, &nExitCode);

	//-----------------------------------------------------------
	//���̶Ի���رգ��ж��߳��Ƿ�ر�
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
	//���û�ǿ����ֹ��
	if(m_dlgProgress.GetUserAbort() || 0xC0DE == nExitCode)
	{
		return;
	}

	//�ɹ�����
	char szMsg[MAX_PATH] = {0};
	_snprintf(szMsg, MAX_PATH, "���ɲ����ļ�%s", maker.GetPatchFile().c_str());
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
	binfo.lpszTitle = "ѡ��洢Ŀ¼";

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
	binfo.lpszTitle = "ѡ��洢Ŀ¼";

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
		GetDlgItem(IDC_BUTTON1)->SetWindowText( "���������óɹ�" );
	}
	else {
		m_strFilter.Empty();
	}
}
