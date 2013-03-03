#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ResizableDialog.h"

#include <set>

// CDlgAddAgent dialog
namespace SMITH
{
	struct AgentCreateParam;
}

class CDlgAddAgent : public CResizableDialog
{
	DECLARE_DYNAMIC(CDlgAddAgent)

public:
	CDlgAddAgent(INT nAgentNum, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddAgent();

// Dialog Data
	enum { IDD = IDD_DIALOG_ADDAGENT };

public:
	int getAgentNum(void) const { return m_nAgentNum; }
	const SMITH::AgentCreateParam* getAgentCreateParam(void) const { return m_createParam; }

protected:
	CEdit	m_editAgentNum;
	INT		m_nAgentNum;
	CButton m_btnAdd;
	CComboBox m_comTemplateName;
	CListCtrl m_listParam;
	SMITH::AgentCreateParam* m_createParam;
	std::set< std::string > m_setTemplateName;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnOK();
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnCbnSelchangeComboAgentType();
	afx_msg void OnNMDblclkListParam(NMHDR *pNMHDR, LRESULT *pResult);
};
