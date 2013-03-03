// tConfigDlg.h : 头文件
//

#pragma once

#include "Dlg_Cfg_Integrate.h"
#include "Dlg_Cfg_Video.h"
#include "Dlg_Cfg_Sound.h"
#include "afxwin.h"

// CtConfigDlg 对话框
class CtConfigDlg : public CDialog
{
// 构造
public:
	CtConfigDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TCONFIG_DIALOG, IDD_VN=IDD_TCONFIG_DIALOG_VN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CDlg_Cfg_Integrate* getIntergrateDlg(void) { return &m_dlgIntegrate; }
	CDlg_Cfg_Video* getVideoDlg(void) { return &m_dlgVideo; }

	void setModifyFlag(bool bModifyed);

protected:
	//创建子窗口
	void		CreateSubWindow(void);

// 实现
protected:
	HICON		m_hIcon;
	CTabCtrl	m_tabMain;
	CStatic		m_staParent;

	CDlg_Cfg_Integrate		m_dlgIntegrate;
	CDlg_Cfg_Video			m_dlgVideo;
	CDlg_Cfg_Sound			m_dlgSound;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	CButton m_btnOk;
	CButton m_btnCancel;
};
