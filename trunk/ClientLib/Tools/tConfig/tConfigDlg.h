// tConfigDlg.h : ͷ�ļ�
//

#pragma once

#include "Dlg_Cfg_Integrate.h"
#include "Dlg_Cfg_Video.h"
#include "Dlg_Cfg_Sound.h"
#include "afxwin.h"

// CtConfigDlg �Ի���
class CtConfigDlg : public CDialog
{
// ����
public:
	CtConfigDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TCONFIG_DIALOG, IDD_VN=IDD_TCONFIG_DIALOG_VN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	CDlg_Cfg_Integrate* getIntergrateDlg(void) { return &m_dlgIntegrate; }
	CDlg_Cfg_Video* getVideoDlg(void) { return &m_dlgVideo; }

	void setModifyFlag(bool bModifyed);

protected:
	//�����Ӵ���
	void		CreateSubWindow(void);

// ʵ��
protected:
	HICON		m_hIcon;
	CTabCtrl	m_tabMain;
	CStatic		m_staParent;

	CDlg_Cfg_Integrate		m_dlgIntegrate;
	CDlg_Cfg_Video			m_dlgVideo;
	CDlg_Cfg_Sound			m_dlgSound;

	// ���ɵ���Ϣӳ�亯��
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
