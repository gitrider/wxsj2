#pragma once


// CDlg_Cfg_Integrate dialog

class CDlg_Cfg_Integrate : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Cfg_Integrate)

public:
	CDlg_Cfg_Integrate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Cfg_Integrate();

// Dialog Data
	enum { IDD = IDD_DIALOG_INTEGRATE, IDD_VN = IDD_DIALOG_INTEGRATE_VN };

public:
	void freshSelect(void);
	void toCommendConfig(void); //Ω¯»ÎÕ∆ºˆ≈‰÷√

protected:
	CToolTipCtrl	m_tooltips;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioSetLow();
	afx_msg void OnBnClickedRadioSetMiddle();
	afx_msg void OnBnClickedRadioSetHigh();
	afx_msg void OnBnClickedRadioSetDef();
};
