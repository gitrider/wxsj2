// ChildView.h : CChildView ��Ľӿ�
//


#pragma once
#include "MainWnd.h"

// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();
	virtual ~CChildView();

protected:
	CMainWnd	m_mainWnd;

// ��д
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

