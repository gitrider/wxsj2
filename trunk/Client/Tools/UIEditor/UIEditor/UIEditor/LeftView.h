#pragma once

#include "LeftTab.h"

// CLeftView ��ͼ

class CLeftView : public CView
{
	DECLARE_DYNCREATE(CLeftView)

protected:
	CLeftView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLeftView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate( );
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif


private:
	CLeftTab m_leftTab;
	bool m_initFlag;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


