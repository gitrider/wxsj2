#pragma once

#include "LeftTreeImage.h"
#include "LeftTreeWindow.h"

// CLeftTab

class CLeftTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CLeftTab)

public:
	CLeftTab();
	virtual ~CLeftTab();
	BOOL CreateChild();
	void OnSize(UINT nType, int cx, int cy);

private:
	CLeftTreeImage * m_treeImage;
	CLeftTreeWindow* m_treeWindow;
	int m_curSel;	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


