// LaunchDlg.h : ͷ�ļ�
//

#pragma once
#include "explorer_help.h"
#include "TransParentButton.h"
#include "ProgressST.h"
#include "afxcmn.h"
#include "afxwin.h"

// CLaunchDlg �Ի���
class CLaunchDlg : public CDialog
{
	/***************************************************
		���нӿ�
	****************************************************/
public:
	/*!
	\brief
		�����Ϣҳ��,�����Ǳ���ҳ��,Ҳ������httpЭ��ҳ��

	\param helpURL
		��Ϣҳ���ַ

	\return
		None
	*/
	void		BrowseHelpDoc(const std::string& helpURL);

	/***************************************************
		����/����
	****************************************************/
public:
	CLaunchDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CLaunchDlg();

	enum { IDD = IDD_LAUNCH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	//!< DDX/DDV ֧��

protected:
	/*!
	\brief
		������
	*/
	void drawBackground(void);
	/*!
	\brief
		����״̬
	*/
	void enterFaultMode(LPCTSTR szMsg);

	/***************************************************
		�ڲ�����
	****************************************************/
protected:
	HICON m_hIcon;
	CExplorer_help	m_webHelp;
	BOOL m_bFaultMode;
	//CProgressCtrl m_proDownload;
	//CProgressCtrl m_proUpdate;
	CProgressST m_proDownload;
	CProgressST m_proUpdate;

	// ���ɵ���Ϣӳ�亯��
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonExit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnTimer(UINT nIDEvent);
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnBnClickedButtonSetting();
	afx_msg void OnBnClickedButtonStart();
	CFont m_font;
//	CTransParentButton m_btnSetting;
	CTransParentButton m_btnHomePage;
	CTransParentButton m_btnStartGame;
	CTransParentButton m_btnExit;
	CTransParentButton m_btnStatus;
	CTransParentButton m_btnSpeed;
};
