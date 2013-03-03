#pragma once


// CDlg_Cfg_Sound dialog

class CDlg_Cfg_Sound : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Cfg_Sound)

public:
	CDlg_Cfg_Sound(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Cfg_Sound();

// Dialog Data
	enum { IDD = IDD_DIALOG_SOUND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
};
