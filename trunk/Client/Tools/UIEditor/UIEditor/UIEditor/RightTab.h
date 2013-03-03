#pragma once

#include "RightPropList.h"
#include "RightEventList.h"

// CRightTab

enum
{
	RTAB_SEL_PROPLIST = 0,
	RTAB_SEL_EVENT = 1,
};

class CRightTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CRightTab)

public:
	CRightTab();
	virtual ~CRightTab();
	BOOL CreateChild();
	void OnSize(UINT nType, int cx, int cy);
	CRightEventList* GetEventList()
	{
		return m_pEventList;
	}
	CRightPropList* GetPropList()
	{
		return m_pPropList;
	}

private:
	CRightPropList* m_pPropList;
	CRightEventList* m_pEventList;
	CImageList m_imageList;
	INT m_curSel;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};


