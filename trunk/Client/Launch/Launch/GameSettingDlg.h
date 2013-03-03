#pragma once


// CGameSettingDlg dialog

class CGameSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameSettingDlg)

public:
	CGameSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGameSettingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING };

	/***************************************************
		�ڲ��ӿ�
	****************************************************/
protected:
	/*!
	\brief
		����ϷĿ¼��System.cfg�ж�ȡ����ѡ��

	\return
		None
	*/
	void loadGameSetting(void);
	
	/*!
	\brief
		�����������ϷĿ¼��System.cfg��

	\return
		None
	*/
	void saveGameSetting(void);

	/*!
	\brief
		�õ�ĳ�����ã�����������򴴽������ʼֵ

	\param szVarName
		ѡ����

	\return
		����ֵ
	*/
	const std::string& getVariable(const char* szVarName, const char* szDefValue="");

	/*!
	\brief
		�û�����ѡ���¼��ʽ

	\return
		None
	*/
	void onLoginTypeChanged(void);

	/***************************************************
		�ڲ�����
	****************************************************/
protected:
	typedef std::map< std::string, std::string > VariableMap;
	//��Ϸ����ѡ��
	VariableMap m_vGameVariable;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioLogin();
	afx_msg void OnBnClickedRadioGameserver();
protected:
	virtual void OnOK();
};
