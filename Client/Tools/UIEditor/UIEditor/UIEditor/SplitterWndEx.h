// SplitterWndEx.h: interface for the CSplitterWndEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLITTERWNDEX_H__F045A340_0506_46E7_8F67_BE5776DC97EC__INCLUDED_)
#define AFX_SPLITTERWNDEX_H__F045A340_0506_46E7_8F67_BE5776DC97EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LeftView.h"


class CSplitterWndEx : public CSplitterWnd  
{
public:
	CSplitterWndEx();
	virtual ~CSplitterWndEx();
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	virtual void OnInvertTracker(CRect &rect);

#define CX_BORDER 4
#define CY_BORDER 4

	int   AddView(int viewid ,int   nRow,   int   nCol,   CRuntimeClass   *   pViewClass,     
	 CCreateContext*   pContext);   
	virtual void OnDestroy();

private:
	//CWnd* m_CurView;		//1  CHistory* 2 CRootDir*CRootDir* 3 CLeftView*
};

#endif // !defined(AFX_SPLITTERWNDEX_H__F045A340_0506_46E7_8F67_BE5776DC97EC__INCLUDED_)
