#pragma once


// CDlgDebuger dialog
#include "Resource.h"
#include "ResizableDialog.h"

#include "DlgProfile.h"
#include "DlgUI.h"
#include "CaptionButton.h"

#include "afxcmn.h"

class CDlgDebuger :  
	public CResizableDialog
	//public CDialog 
{
	DECLARE_DYNAMIC(CDlgDebuger)

public:
	CDlgUI*			GetDlgUI(void) { return &m_dlgUI; }

public:
	CDlgDebuger(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDebuger();

// Dialog Data
	enum { IDD = IDD_DIALOG_DEBUGER };

	//�Ӵ���
protected:
	void			CreateSubWindow(void);
	//Ч�ʲ���
	CDlgProfile		m_dlgProfile;
	//UI����
	CDlgUI			m_dlgUI;

	//�������ϰ�ť
	CCaptionButton*	m_pCaptionBtn;

protected:
	//��С��
	void			OnMinimizeWindow(void);

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL	OnInitDialog();
	void			OnCapButton(BOOL bBtnState);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CTabCtrl m_tabMain;
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnWMU_Hitcapbutton(WPARAM wParam, LPARAM lParam);
};
