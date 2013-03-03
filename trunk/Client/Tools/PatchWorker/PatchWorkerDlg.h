// PatchWorkerDlg.h : 头文件
//

#pragma once

#include "ProgressDlg.h"
#include "FileCollector.h"
#include "afxwin.h"

// CPatchWorkerDlg 对话框
class CPatchWorkerDlg : public CDialog
{
// 构造
public:
	CPatchWorkerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PATCHWORKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CFont m_hLargeFont;
	CBrush m_hYellowBrush;

	CProgressDlg m_dlgProgress;
	//CAxCryptoMath m_keyMath;

	CFileCollector m_fileCollecterOld;
	CEdit m_editVersionOld;
	CString m_strOldFold;

	CFileCollector m_fileCollecterNew;
	CEdit m_editVersionNew;
	CString m_strNewFold;
	CString m_strFilter;

	static UINT _ScanNewFoldThread(LPVOID pParam);
	void ScanNewFoldThread(void);

	static UINT _GeneratePatchThread(LPVOID pParam);
	void GeneratePatchThread(CPatchMaker* pMaker);

	void UpdateCreateButton(void);
	void	GenePairKey(void);
	void	ExportPubKey(void);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnStaticCtlColor(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFoldOld();
	afx_msg void OnBnClickedButtonFoldNew();
	afx_msg void OnBnClickedButtonGen();
	afx_msg void OnEnUpdateEditVersionNew();
	afx_msg void OnBnClickedButton1();
};
