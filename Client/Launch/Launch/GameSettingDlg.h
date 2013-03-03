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
		内部接口
	****************************************************/
protected:
	/*!
	\brief
		从游戏目录下System.cfg中读取配置选项

	\return
		None
	*/
	void loadGameSetting(void);
	
	/*!
	\brief
		保存变量到游戏目录下System.cfg中

	\return
		None
	*/
	void saveGameSetting(void);

	/*!
	\brief
		得到某项设置，如果不存在则创建并设初始值

	\param szVarName
		选项名

	\return
		设置值
	*/
	const std::string& getVariable(const char* szVarName, const char* szDefValue="");

	/*!
	\brief
		用户重新选择登录方式

	\return
		None
	*/
	void onLoginTypeChanged(void);

	/***************************************************
		内部数据
	****************************************************/
protected:
	typedef std::map< std::string, std::string > VariableMap;
	//游戏配置选项
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
