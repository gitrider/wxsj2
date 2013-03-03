// SplitterWndEx.cpp: implementation of the CSplitterWndEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SplitterWndEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSplitterWndEx::CSplitterWndEx()
{

}

CSplitterWndEx::~CSplitterWndEx()
{

}

void CSplitterWndEx::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
                  if(pDC==NULL) 
                  { 
                  RedrawWindow(rectArg,NULL,RDW_INVALIDATE|RDW_NOCHILDREN);
                  return;
                  } 
                  ASSERT_VALID(pDC);
                  CRect rc=rectArg;
				  char str[100];
				  sprintf_s(str, "(top = %d, left = %d, bottom = %d, right = %d)\n", rc.top, rc.left, rc.bottom, rc.right);
				  printf(str);
			         switch(nType) 
                  { 
                  case splitBorder:
                  //重画分割窗口边界,使之为红色 
						  if ( rc.top ==rc.bottom)
						  {
								return;
						  }
                          pDC->Draw3dRect(rc,RGB(255,0,0),RGB(255,255,255));
                         // rc.InflateRect(-CX_BORDER,-CY_BORDER); 
								 
                          //pDC->Draw3dRect(rc,RGB(255,0,0),RGB(255,0,0)); 
                          return; 
                  case splitBox:
                          pDC->Draw3dRect(rc,RGB(0,0,0),RGB(0,0,0));
                          rc.InflateRect(-CX_BORDER,-CY_BORDER); 
                          pDC->Draw3dRect(rc,RGB(0,0,0),RGB(0,0,0));
                          rc.InflateRect(-CX_BORDER,-CY_BORDER);
                          pDC->FillSolidRect(rc,RGB(0,0,0)); 
                          pDC->Draw3dRect(rc,RGB(0,0,0),RGB(0,0,0));
								 
                          return; 
                  case splitBar: 
                  //重画分割条，使之为绿色 
                          pDC->FillSolidRect(rc,RGB(255,255,255));
                       //   rc.InflateRect(-5,-5); 
                        //  pDC->Draw3dRect(rc,RGB(255,0,0),RGB(255,0,0)); 
          
                          return; 
                  default: 
                          ASSERT(FALSE); 
                  } 
                  pDC->FillSolidRect(rc,RGB(0,0,255));
					

}


void CSplitterWndEx::OnInvertTracker(CRect &rect) 
{ 
                  ASSERT_VALID(this);
                  ASSERT(!rect.IsRectEmpty()); 
                  ASSERT((GetStyle()&WS_CLIPCHILDREN)==0);
                  CRect rc=rect; 
                  //rc.InflateRect(2,2);
                  CDC* pDC=GetDC(); 
                  CBrush* pBrush=CDC::GetHalftoneBrush();
                  HBRUSH hOldBrush=NULL;
                  if(pBrush!=NULL) hOldBrush=(HBRUSH)SelectObject(pDC->m_hDC,pBrush->m_hObject);
                  pDC->PatBlt(rc.left,rc.top,rc.Width(),rc.Height(),BLACKNESS); 
          
                  if(hOldBrush!=NULL) 
                  SelectObject(pDC->m_hDC,hOldBrush);
                  ReleaseDC(pDC); 
} 
  
   

void CSplitterWndEx::OnDestroy()
{
	//MessageBox("run in OnDestroy in CSplitrerWndEx");
	CSplitterWnd::OnDestroy();
}

