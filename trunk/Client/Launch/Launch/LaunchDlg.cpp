// LaunchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Launch.h"
#include "LaunchDlg.h"
#include "LaunchSystem.h"
#include "UpdateSystem.h"
#include "GameSettingDlg.h"
#include ".\launchdlg.h"

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


// CLaunchDlg �Ի���



CLaunchDlg::CLaunchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLaunchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bFaultMode = FALSE;
}

CLaunchDlg::~CLaunchDlg()
{
	delete 	LaunchSystem::GetMe();
}

void CLaunchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EXPLORER_HELP, m_webHelp);
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD, m_proDownload);
	DDX_Control(pDX, IDC_PROGRESS_INSTALL, m_proUpdate);
//	DDX_Control(pDX, IDC_BUTTON_SETTING, m_btnSetting);
	DDX_Control(pDX, IDC_BUTTON_HOMEPAGE, m_btnHomePage);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStartGame);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_BTN_STATUS, m_btnStatus);
	DDX_Control(pDX, IDC_BTN_SPEED, m_btnSpeed);
}

BEGIN_MESSAGE_MAP(CLaunchDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnBnClickedButtonExit)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
//	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnBnClickedButtonSetting)
	ON_BN_CLICKED(IDC_BUTTON_START, OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CLaunchDlg ��Ϣ�������

BOOL CLaunchDlg::OnInitDialog()
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
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_proDownload.SetRange32(0, 100);
	m_proUpdate.SetRange32(0, 100);

	//InitLaunchSystem
	new LaunchSystem();
	LaunchSystem::GetMe()->init(this->GetSafeHwnd());

	//���ñ�����
	CString strCaption;
	strCaption.LoadString(IDS_MAIN_CAPTION);
	SetWindowText(strCaption);

	//��������ĵ�
	BrowseHelpDoc(LaunchSystem::GetMe()->getHelpURL());

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

	m_font.CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, GB2312_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, 
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("����") );
	
//	m_btnSetting.SetAlign( 8, 2 );
//	m_btnSetting.SetFont( &m_font );
	m_btnHomePage.SetAlign( 8, 2 );
	m_btnHomePage.SetFont( &m_font );
	m_btnStartGame.SetAlign( 8, 2 );
	m_btnStartGame.SetFont( &m_font );
	m_btnExit.SetAlign( 20, 2 );
	m_btnExit.SetFont( &m_font );

	GetDlgItem(IDC_BTN_STATUS)->EnableWindow(FALSE);
	m_btnStatus.SetAlign( AL_LEFT );
	m_btnStatus.SetFont( &m_font );
	m_btnStatus.SetDisableTextColor( RGB(25,25,25) );

	GetDlgItem(IDC_BTN_SPEED)->EnableWindow(FALSE);
	m_btnSpeed.SetAlign( AL_LEFT );
	m_btnSpeed.SetFont( &m_font );
	m_btnSpeed.SetDisableTextColor( RGB(25,25,25) );

	//m_proDownload.SubclassDlgItem( IDC_PROGRESS_DOWNLOAD, this );
	m_proDownload.SetBitmap(IDB_BITMAP3);
	m_proUpdate.SetBitmap(IDB_BITMAP3);


	HBITMAP hBitmapExit = ::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE( IDB_BITMAP_EXIT ) );
	m_btnExit.SetBitmap( hBitmapExit );

	HBITMAP hBitmapRunGame = ::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE( IDB_BITMAP_RUNGAME ) );
	m_btnStartGame.SetBitmap( hBitmapRunGame );

	HBITMAP hBitmapMainPage = ::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE( IDB_BITMAP_MAINPAGE ) );
	m_btnHomePage.SetBitmap( hBitmapMainPage );


	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CLaunchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLaunchDlg::OnPaint() 
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
		drawBackground();

		//CStatic* pStatus = (CStatic*)GetDlgItem(IDC_STATIC_STATUS);
		//if( pStatus )
		//{
		//	CClientDC dc(pStatus);
		//	dc.SelectObject(&m_font);
		//	dc.SetBkMode(TRANSPARENT);
		//}
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CLaunchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLaunchDlg::BrowseHelpDoc(const std::string& helpURL)
{
	DWORD dwServiceType;
	CString strServer, strObject;
	INTERNET_PORT nPort;

	CString strFullURL;
	AfxParseURL(helpURL.c_str(), dwServiceType, strServer, strObject, nPort);

	//�����ļ�
	if(AFX_INET_SERVICE_UNK == dwServiceType)
	{
		//local file
		CHAR szFullLocalFile[MAX_PATH] = {0};
		::GetCurrentDirectory(MAX_PATH, szFullLocalFile);
		::PathAppend(szFullLocalFile, helpURL.c_str());
		strFullURL = szFullLocalFile;
	}
	else
	{
		strFullURL = helpURL.c_str();
	}

	COleVariant vaURL2(strFullURL);
	m_webHelp.Navigate2(vaURL2, NULL, NULL, NULL, NULL);
}

void CLaunchDlg::OnOK()
{
	//������Ϸ
	//LaunchSystem::GetMe()->runGame();

	CDialog::OnOK();
}

void CLaunchDlg::OnBnClickedButtonExit()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CLaunchDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//����ESC
	if(LOWORD(wParam) == IDCANCEL && lParam == 0)return TRUE;

	return CDialog::OnCommand(wParam, lParam);
}

void CLaunchDlg::drawBackground(void)
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CDC dcBmp;
	dcBmp.CreateCompatibleDC(&dc);

	CBitmap bmpBkg;
	bmpBkg.LoadBitmap( IDB_BITMAP1 );

	CBitmap* pOldBmp = dcBmp.SelectObject(&bmpBkg);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcBmp, 0, 0, SRCCOPY);

	dcBmp.SelectObject(pOldBmp);
	dcBmp.DeleteDC();

	bmpBkg.DeleteObject();

}

HBRUSH CLaunchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//��������StaticText�ı���Ϊ�׻�ˢ
	//if(pWnd->GetDlgCtrlID() == IDC_STATIC_STATUS)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	if(m_bFaultMode)
	//		pDC->SetTextColor(RGB(255, 0, 0));
	//	else
	//		pDC->SetTextColor(RGB(0, 0, 0));
	//	return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	//}
	if(pWnd->GetDlgCtrlID() == IDC_BTN_STATUS)
	{
		if(m_bFaultMode)
			m_btnStatus.SetDisableTextColor( RGB(200,25,25) );
		else
			m_btnStatus.SetDisableTextColor( RGB(25,25,25) );
		return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}

	//if(pWnd->GetDlgCtrlID() == IDC_STATIC_SPEED)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	//}
	
	return hbr;
}

void CLaunchDlg::OnTimer(UINT nIDEvent)
{

	CDialog::OnTimer(nIDEvent);
}

BOOL CLaunchDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == LaunchSystem::NM_STATUS)
	{
		//SetDlgItemText(IDC_STATIC_STATUS, (LPCTSTR)wParam);
		SetDlgItemText(IDC_BTN_STATUS, (LPCTSTR)wParam);
		return TRUE;
	}
	else if(message == LaunchSystem::NM_FILE)
	{
		//SetDlgItemText(IDC_STATIC_SPEED, (LPCTSTR)wParam);
		SetDlgItemText(IDC_BTN_SPEED, (LPCTSTR)wParam);
		return TRUE;
	}
	else if(message == LaunchSystem::NM_FAULT)
	{
		//SetDlgItemText(IDC_STATIC_STATUS, (LPCTSTR)wParam);
		SetDlgItemText(IDC_BTN_STATUS, (LPCTSTR)wParam);
		enterFaultMode((LPCTSTR)wParam);
		return TRUE;
	}
	else if(message == LaunchSystem::NM_PROCESS1)
	{
		int nPosNow = m_proDownload.GetPos();		
		int nPosNew = (int)wParam;
		if(nPosNew != nPosNow)
		{
			m_proDownload.SetPos(nPosNew);
		}

	}
	else if(message == LaunchSystem::NM_PROCESS2)
	{
		//int nPosNow = m_proDownload.GetPos();		
		//int nPosNew = (int)wParam;
		//if(nPosNew != nPosNow)
		//{
			m_proUpdate.SetPos((int)wParam);
		//}

	}
	else if( message == UpdateSystem::US_DELETEFILE )
	{
		DWORD dw = UpdateSystem::GetMe()->GetProcess();
		int nPosNow = m_proDownload.GetPos();		
		int nPosNew = (int)dw;
		if(nPosNew != nPosNow)
		{
			m_proUpdate.SetPos(nPosNew);
		}

		CString strUpdateInfo;
		strUpdateInfo.LoadString( IDS_DELETE_FILE );
		SetDlgItemText(IDC_BTN_STATUS, (LPCTSTR)strUpdateInfo);
		//SetDlgItemText(IDC_STATIC_STATUS, (LPCTSTR)strUpdateInfo);
		SetDlgItemText(IDC_BTN_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());
		//SetDlgItemText(IDC_STATIC_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());
		return TRUE;
	}
	else if( message == UpdateSystem::US_APPENDFILE )
	{
		DWORD dw = UpdateSystem::GetMe()->GetProcess();
		int nPosNow = m_proDownload.GetPos();		
		int nPosNew = (int)dw;
		if(nPosNew != nPosNow)
		{
			m_proUpdate.SetPos(nPosNew);
		}

		CString strUpdateInfo;
		strUpdateInfo.LoadString( IDS_ADD_FILE );
		SetDlgItemText(IDC_BTN_STATUS, (LPCTSTR)strUpdateInfo);
		//SetDlgItemText(IDC_STATIC_STATUS, (LPCTSTR)strUpdateInfo);
		SetDlgItemText(IDC_BTN_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());
		//SetDlgItemText(IDC_STATIC_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());

		return TRUE;
	}
	else if( message == UpdateSystem::US_BACKUPFILE )
	{
		DWORD dw = UpdateSystem::GetMe()->GetProcess();
		int nPosNow = m_proDownload.GetPos();		
		int nPosNew = (int)dw;
		if(nPosNew != nPosNow)
		{
			m_proUpdate.SetPos(nPosNew);
		}

		CString strUpdateInfo;
		strUpdateInfo.LoadString( IDS_BACKUP );
		SetDlgItemText(IDC_BTN_STATUS, (LPCTSTR)strUpdateInfo);
		//SetDlgItemText(IDC_STATIC_STATUS, (LPCTSTR)strUpdateInfo);
		SetDlgItemText(IDC_BTN_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());
		//SetDlgItemText(IDC_STATIC_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());

		return TRUE;
	}
	else if( message == UpdateSystem::US_DELETEPACKFILE )
	{
		DWORD dw = UpdateSystem::GetMe()->GetProcess();
		int nPosNow = m_proDownload.GetPos();		
		int nPosNew = (int)dw;
		if(nPosNew != nPosNow)
		{
			m_proUpdate.SetPos(nPosNew);
		}

		CString strUpdateInfo;
		strUpdateInfo.LoadString( IDS_PACK_DELETE_FILE );
		SetDlgItemText(IDC_BTN_STATUS, (LPCTSTR)strUpdateInfo);
		//SetDlgItemText(IDC_STATIC_STATUS, (LPCTSTR)strUpdateInfo);
		SetDlgItemText(IDC_BTN_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());
		//SetDlgItemText(IDC_STATIC_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());

		return TRUE;
	}
	else if( message == UpdateSystem::US_APPENDPACKFILE )
	{
		DWORD dw = UpdateSystem::GetMe()->GetProcess();
		int nPosNow = m_proDownload.GetPos();		
		int nPosNew = (int)dw;
		if(nPosNew != nPosNow)
		{
			m_proUpdate.SetPos(nPosNew);
		}

		CString strUpdateInfo;
		strUpdateInfo.LoadString( IDS_PACK_ADD_FILE );
		SetDlgItemText(IDC_BTN_STATUS, (LPCTSTR)strUpdateInfo);
		//SetDlgItemText(IDC_STATIC_STATUS, (LPCTSTR)strUpdateInfo);
		SetDlgItemText(IDC_BTN_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());
		//SetDlgItemText(IDC_STATIC_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());

		return TRUE;
	}
	else if( message == UpdateSystem::US_ROLLBACK )
	{
		DWORD dw = UpdateSystem::GetMe()->GetProcess();
		int nPosNow = m_proDownload.GetPos();		
		int nPosNew = (int)dw;
		if(nPosNew != nPosNow)
		{
			m_proUpdate.SetPos(nPosNew);
		}

		CString strUpdateInfo;
		strUpdateInfo.LoadString( IDS_UPDATE_ERR );
		SetDlgItemText(IDC_BTN_STATUS, (LPCTSTR)strUpdateInfo);
		//SetDlgItemText(IDC_STATIC_STATUS, (LPCTSTR)strUpdateInfo);
		SetDlgItemText(IDC_BTN_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());
		//SetDlgItemText(IDC_STATIC_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());

		return TRUE;
	}
	else if( message == UpdateSystem::US_UPDATESYSTEM )
	{
		DWORD dw = UpdateSystem::GetMe()->GetProcess();
		int nPosNow = m_proDownload.GetPos();		
		int nPosNew = (int)dw;
		if(nPosNew != nPosNow)
		{
			m_proUpdate.SetPos(nPosNew);
		}

		CString strUpdateInfo;
		strUpdateInfo.LoadString( IDS_UPDATE_SYSTEM );
		SetDlgItemText(IDC_BTN_STATUS, (LPCTSTR)strUpdateInfo);
		//SetDlgItemText(IDC_STATIC_STATUS, (LPCTSTR)strUpdateInfo);
		SetDlgItemText(IDC_BTN_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());
		//SetDlgItemText(IDC_STATIC_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());

		return TRUE;
	}
	else if( message == UpdateSystem::US_CHECK_ALL )
	{
		DWORD dw = UpdateSystem::GetMe()->GetProcess();
		int nPosNow = m_proDownload.GetPos();		
		int nPosNew = (int)dw;
		if(nPosNew != nPosNow)
		{
			m_proUpdate.SetPos(nPosNew);
		}

		SetDlgItemText(IDC_BTN_STATUS, "ȫ�ּ��...");
		//SetDlgItemText(IDC_STATIC_STATUS, (LPCTSTR)strUpdateInfo);
		SetDlgItemText(IDC_BTN_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());
		//SetDlgItemText(IDC_STATIC_SPEED, (LPCTSTR)UpdateSystem::GetMe()->GetCurrentFileName());

		return TRUE;
	}
	else if( message == LaunchSystem::NM_ENABLE_CANCEL)
	{
		GetDlgItem(IDC_BUTTON_EXIT)->EnableWindow(TRUE);
	}
	else if( message == LaunchSystem::NM_DISABLE_CANCEL)
	{
		GetDlgItem(IDC_BUTTON_EXIT)->EnableWindow(FALSE);
	}
	else if( message == LaunchSystem::NM_OK)
	{
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	}

	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}

void CLaunchDlg::enterFaultMode(LPCTSTR szMsg)
{
	m_bFaultMode = true;

	SetDlgItemText(IDC_BTN_STATUS, szMsg);
	//SetDlgItemText(IDC_STATIC_STATUS, szMsg);
	//SetDlgItemText(IDC_STATIC_SPEED, "");
	SetDlgItemText(IDC_BTN_SPEED, "");

//	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
}

void CLaunchDlg::OnBnClickedButtonSetting()
{
	// TODO: Add your control notification handler code here
	CGameSettingDlg dlg;
	dlg.DoModal();
}

void CLaunchDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	LaunchSystem::GetMe()->runGame();

	OnOK();
}
