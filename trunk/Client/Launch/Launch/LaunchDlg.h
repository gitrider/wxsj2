// LaunchDlg.h : 头文件
//

#pragma once
#include "explorer_help.h"
#include "TransParentButton.h"
#include "ProgressST.h"
#include "afxcmn.h"
#include "afxwin.h"

// CLaunchDlg 对话框
class CLaunchDlg : public CDialog
{
	/***************************************************
		共有接口
	****************************************************/
public:
	/*!
	\brief
		浏览信息页面,可以是本地页面,也可以是http协议页面

	\param helpURL
		信息页面地址

	\return
		None
	*/
	void		BrowseHelpDoc(const std::string& helpURL);

	/***************************************************
		构造/析构
	****************************************************/
public:
	CLaunchDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CLaunchDlg();

	enum { IDD = IDD_LAUNCH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	//!< DDX/DDV 支持

protected:
	/*!
	\brief
		画背景
	*/
	void drawBackground(void);
	/*!
	\brief
		错误状态
	*/
	void enterFaultMode(LPCTSTR szMsg);

	/***************************************************
		内部数据
	****************************************************/
protected:
	HICON m_hIcon;
	CExplorer_help	m_webHelp;
	BOOL m_bFaultMode;
	//CProgressCtrl m_proDownload;
	//CProgressCtrl m_proUpdate;
	CProgressST m_proDownload;
	CProgressST m_proUpdate;

	// 生成的消息映射函数
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
