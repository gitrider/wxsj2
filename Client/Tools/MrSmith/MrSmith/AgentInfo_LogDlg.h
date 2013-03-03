#pragma once

#include "ResizableDialog.h"
#include "afxcmn.h"
// CAgentInfo_LogDlg dialog

class CAgentInfo_LogDlg : public CResizableDialog
{
public:
	/*!
	\brief
		Clean all log.
	\return 
		None
	*/
	void	clearAllLog(void);

	/*!
	\brief
		Add a log
	\return 
		None
	*/
	void	addLog(const SYSTEMTIME& tTime, const std::string& strContents);

protected:
	CListCtrl m_listLog;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CAgentInfo_LogDlg)
public:
	CAgentInfo_LogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAgentInfo_LogDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_AGENTINFO_LOG };
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void On32773();
};
