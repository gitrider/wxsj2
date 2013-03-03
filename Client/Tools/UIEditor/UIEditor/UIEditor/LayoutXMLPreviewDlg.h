/*****************************************************************************
文件:	LayoutXMLPreviewDlg
作者:	方远
邮箱:	fangyuanning@126.com
说明:	显示布局的预览对话框
******************************************************************************/
#pragma once
#include "afxcmn.h"

#include "TWScriptEdit.h"


// CLayoutXMLPreviewDlg dialog

class CLayoutXMLPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CLayoutXMLPreviewDlg)

public:
	CLayoutXMLPreviewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLayoutXMLPreviewDlg();

// Dialog Data
	enum { IDD = IDD_XML_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTWScriptEdit m_richEdit;


public:
	void SetLayoutText(const CString& szText);
	void Initailzie();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnSaveXml();
public:
	afx_msg void OnCloseXml();
};
