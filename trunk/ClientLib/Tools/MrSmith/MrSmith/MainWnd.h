#pragma once
#include "ResizableDialog.h"
#include "LegendWnd.h"
#include "afxcmn.h"
#include "AgentInfo_logDlg.h"
#include "AgentInfo_infoDlg.h"
#include "afxwin.h"

// CMainWnd dialog

class CMainWnd : public CResizableDialog
{
public:
	/*!
	\brief 
		Get the log record window.
	*/
	CAgentInfo_LogDlg* getLogDlg(void) { return &m_infodlg_Log; }

	/*!
	\brief 
		Get the agent info window.
	*/
	CAgentInfo_InfoDlg* getInfoDlg(void) { return &m_infodlg_Info; }

protected:
	/*!
	\brief 
		Create all agentinfo sub window.
	*/
	void createSubWindow(void);

protected:
	CLegendWnd		m_legendWnd;
	CTabCtrl		m_tabMain;

	CAgentInfo_LogDlg	m_infodlg_Log;
	CAgentInfo_InfoDlg	m_infodlg_Info;

	DECLARE_DYNAMIC(CMainWnd)

public:
	CMainWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainWnd();

	/*!
	\brief
		Return pointer to singleton CMainWnd object

	\return
		Pointer to singleton CMainWnd object
	*/
	static CMainWnd* GetMe(void) { return s_pMe; }

// Dialog Data
	enum { IDD = IDD_DIALOG_MAIN };
protected:
	static CMainWnd* s_pMe;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	// ÷∏¡Ó ‰»Î
	CEdit m_edtCmd;
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnSendall();
};
