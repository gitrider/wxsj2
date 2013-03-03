#pragma once

#include "ResizableDialog.h"
#include "afxcmn.h"
// CAgentInfo_InfoDlg dialog

class CAgentInfo_InfoDlg : public CResizableDialog
{

public:
	CAgentInfo_InfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAgentInfo_InfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_AGENTINFO_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CAgentInfo_InfoDlg)
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listLog1;
	void	clearAllLog(void);
	void	addLog(const SYSTEMTIME& tTime, const std::string& strContents);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
