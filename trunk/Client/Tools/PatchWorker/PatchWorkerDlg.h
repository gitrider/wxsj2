// PatchWorkerDlg.h : ͷ�ļ�
//

#pragma once

#include "ProgressDlg.h"
#include "FileCollector.h"
#include "afxwin.h"

// CPatchWorkerDlg �Ի���
class CPatchWorkerDlg : public CDialog
{
// ����
public:
	CPatchWorkerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PATCHWORKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
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

	// ���ɵ���Ϣӳ�亯��
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
