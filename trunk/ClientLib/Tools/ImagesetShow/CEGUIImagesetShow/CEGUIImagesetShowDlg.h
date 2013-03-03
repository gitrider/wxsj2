// CEGUIImagesetShowDlg.h : 头文件
//

#pragma once

#include "Imageset.h"
#include "canvaswnd.h"
#include "afxcmn.h"

// CCEGUIImagesetShowDlg 对话框
class CCEGUIImagesetShowDlg : public CDialog
{
// 构造
public:
	CCEGUIImagesetShowDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CEGUIIMAGESETSHOW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	void _fresh(void);
	void _onItemSelectChanged(void);

// 实现
protected:
	HICON m_hIcon;
	CImageset	m_theImageset;
	CCanvasWnd m_staCanvas;

	// 生成的消息映射函数
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
