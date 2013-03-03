#pragma once
#include "RightTab.h"

// CRightView 视图

class CRightView : public CView
{
	DECLARE_DYNCREATE(CRightView)

protected:
	CRightView();           // 动态创建所使用的受保护的构造函数
	virtual ~CRightView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate( );
	afx_msg void OnSize(UINT nType,int cx,	int cy 	);

private:
	CRightTab* m_pRightTab;
	BOOL m_createFlag;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	void Hide();
	CRightTab* GetRightTab()
	{
		return m_pRightTab;
	}
public:
	afx_msg void OnDestroy();
};


