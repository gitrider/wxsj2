// tConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "tConfig.h"
#include "tConfigDlg.h"
#include ".\tconfigdlg.h"
#include "TDUnicodeLayer.h"
#include "InnerString.h"

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


// CtConfigDlg �Ի���



CtConfigDlg::CtConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(g_nCodePage==1258? (CtConfigDlg::IDD_VN) : (CtConfigDlg::IDD), pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabMain);
	DDX_Control(pDX, IDC_STATIC_PARENT, m_staParent);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CtConfigDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnTcnSelchangeTabMain)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CtConfigDlg ��Ϣ�������

BOOL CtConfigDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CreateSubWindow();

	SetWindowText( (getInnerString(INNER_TITLE)) ) ;
	m_btnOk.SetWindowText( getInnerString(INNER_OK) ) ;
	m_btnCancel.SetWindowText( getInnerString(INNER_CANCEL) ) ;
	
	//����ǵ�һ��������Ϸ,�Զ�����Ϊ�Ƽ�����
	if(g_bFirstRun)
	{
		m_dlgIntegrate.toCommendConfig();
	}

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CtConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtConfigDlg::OnPaint() 
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
HCURSOR CtConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtConfigDlg::CreateSubWindow(void)
{
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_tabMain.InsertItem(0, getInnerString(INNER_ZONG_HE));
	m_tabMain.InsertItem(1, getInnerString(INNER_XIAN_SHI_SHE_ZHE));

	CRect rect;
	m_staParent.GetClientRect(&rect);

	m_dlgIntegrate.Create(g_nCodePage==1258? (CDlg_Cfg_Integrate::IDD_VN) : (CDlg_Cfg_Integrate::IDD), &m_staParent);
	m_dlgIntegrate.MoveWindow(&rect);
	m_dlgIntegrate.ShowWindow(SW_SHOW);

	m_dlgVideo.Create(g_nCodePage==1258? (CDlg_Cfg_Video::IDD_VN) : (CDlg_Cfg_Video::IDD), &m_staParent);
	m_dlgVideo.MoveWindow(&rect);
	m_dlgVideo.ShowWindow(SW_HIDE);

//	m_dlgSound.Create(IDD_DIALOG_SOUND, &m_staParent);
//	m_dlgSound.MoveWindow(&rect);
//	m_dlgSound.ShowWindow(SW_HIDE);
}

void CtConfigDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	INT nSel = m_tabMain.GetCurSel();

	switch(nSel)
	{
	case 0:
		m_dlgIntegrate.ShowWindow(SW_SHOW);
		m_dlgVideo.ShowWindow(SW_HIDE);
//		m_dlgSound.ShowWindow(SW_HIDE);
		break;

	case 1:
		m_dlgIntegrate.ShowWindow(SW_HIDE);
		m_dlgVideo.ShowWindow(SW_SHOW);
//		m_dlgSound.ShowWindow(SW_HIDE);
		break;

//	case 2:
//		m_dlgIntegrate.ShowWindow(SW_HIDE);
//		m_dlgVideo.ShowWindow(SW_HIDE);
//		m_dlgSound.ShowWindow(SW_SHOW);
//		break;
	}

	*pResult = 0;
}


void CtConfigDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}

void CtConfigDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(g_theGameConfigs.NeedUpdate())
	{
		if(IDYES == TD_MessageBoxW(m_hWnd, getInnerString(INNER_SAVE), getInnerString(INNER_TITLE), MB_YESNO|MB_ICONQUESTION))
		{
			g_theGameConfigs.SaveModifed();
		}
	}

	OnOK();
}

void CtConfigDlg::setModifyFlag(bool bModifyed)
{
	CString strTitle;
	GetWindowText(strTitle);

	if(strTitle.Right(1) == "*")
	{
		strTitle = strTitle.Left(strTitle.GetLength()-1);
	}

	SetWindowText(bModifyed ? strTitle+L"*" : strTitle);
}