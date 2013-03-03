#pragma once
#include "afxwin.h"


// CDlg_Cfg_Video dialog

class CDlg_Cfg_Video : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Cfg_Video)

public:
	CDlg_Cfg_Video(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Cfg_Video();

// Dialog Data
	enum { IDD = IDD_DIALOG_VIDEO, IDD_VN = IDD_DIALOG_VIDEO_VN };

public:
	void refreshFromModifyed(void);

protected:
	void _refreshSliderToValue(int nIndex);
	void _refreshSliderFromValue(int nIndex, const char* szValue);

	void _refreshCheckToValue(int nIndex);
	void _refreshCheckFromValue(int nIndex, const char* szValue);

    void _recheckVideoOption(void);

	bool _checkValueInvalid(const char* szName, int nValue, bool bShowWarning);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	CComboBox Combo_CardSet;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	CEdit m_edtMaxPitch;
	CEdit m_edtMinPitch;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
//	afx_msg void OnEnUpdateEdit2();
};
