// CEGUIImagesetShowDlg.h : ͷ�ļ�
//

#pragma once

#include "Imageset.h"
#include "canvaswnd.h"
#include "afxcmn.h"

// CCEGUIImagesetShowDlg �Ի���
class CCEGUIImagesetShowDlg : public CDialog
{
// ����
public:
	CCEGUIImagesetShowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CEGUIIMAGESETSHOW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	void _fresh(void);
	void _onItemSelectChanged(void);

// ʵ��
protected:
	HICON m_hIcon;
	CImageset	m_theImageset;
	CCanvasWnd m_staCanvas;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedRadioRgb();
	afx_msg void OnBnClickedRadioAlpha();
	CListCtrl m_listImage;
	afx_msg void OnLvnItemchangedListImage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonFresh();
};
