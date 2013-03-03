#pragma once
#include "afxcmn.h"
#include <map>


// CToolBarListDlg dialog


typedef std::map<CString , CString> ToolTypeDefine;

class CToolBarListDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolBarListDlg)

public:
	CToolBarListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CToolBarListDlg();

// Dialog Data
	enum { IDD = IDD_CEGUI_WINDOW_TOOLBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtrl;
	ToolTypeDefine m_toolTypeMap;
	CString m_szLastTypeName;
public:
	virtual BOOL OnInitDialog();
	VOID InsertToolWindowToList();
	void OnSelChanged();
	CString GetInnerTypeName(const CString& szName);
public:
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
};
