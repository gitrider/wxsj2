// ChildView.h : CChildView 类的接口
//


#pragma once
#include "MainWnd.h"

// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();
	virtual ~CChildView();

protected:
	CMainWnd	m_mainWnd;

// 重写
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

