/*----------------------------------------------------------------------------*/
/*            CFaderWnd implementation                                        */
/*----------------------------------------------------------------------------*/
//
//	File:			FaderWnd.cpp
//	Author:		Phil J. Pearson
//	Created:	20 June 2000  14:26
//	Last Mod: 23 June 2000  19:06
//
/*----------------------------------------------------------------------------
/*
/* Copyright (C) 2000 by Real World Software
/* All Rights Reserved.
/*
/*----------------------------------------------------------------------------*/

///////////////////////////////////////使用说明 ////////////////////////////////////////
//
// An MFC class to fade out any window, requiring only one extra line of
// code, typically:
//	new CFaderWnd(this);
//
// It uses the UpdateLayeredWindow function, not available on Win9x or NT.
// It uses GetProcAddress instead of implicitly linking to the function so
// that code using this class will load and run on any Win32 platform. If
// the necessary function is not available then the fade will simply not
// happen.
// It works by making an exact copy of the source window and fading the copy
// so it is usual to hide or destroy the source window immediately after
// creating an instance of this class. For example,
//	new CFaderWnd(this);
//	ShowWindow(SW_HIDE);
// or
//	new CFaderWnd(this);
//	DestroyWindow();
// or
//	new CFaderWnd(this);
//	EndDialog(nResult);
// Note that it's essential to construct the CFaderWnd FIRST.
//
// CFaderWnd must ALWAYS be contructed on the heap (with new CFaderWnd...).
// It is NEVER necessary to call the destructor (delete ...) since CFaderWnd
// takes care of deleting itself (tidies up and closes the door behind itself).
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaderWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ALPHA_STEP 5  //每次减少的透明度数

/////////////////////////////////////////////////////////////////////////////


CFaderWnd::CFaderWnd(CWnd *pWndToFade, UINT nFadeTime /*= 1000*/, BYTE byAlpha /*= 255*/)
	                 : m_pWndToFade(pWndToFade)	
{
	ASSERT(pWndToFade);
	ASSERT_VALID(pWndToFade);

   HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));

   m_pUpdateLayeredWindow = (lpfnUpdateLayeredWindow)GetProcAddress(hUser32, "UpdateLayeredWindow");

	// If OS doesn't provide the function we can't fade, just commit suicide.
	if (NULL == m_pUpdateLayeredWindow)
		delete this;
	else
		{
		CRect		rc;
		CPoint	ptSrc(0, 0);
		SIZE		size;
	
		m_pWndToFade->GetWindowRect(rc);

		// Make a new window to match.
		//	WS_EX_LAYERED is necessary for UpdateLayeredWindow to be enabled.
		//	WS_EX_TRANSPARENT allows mouse clicks through to the window "underneath",
		//										(it's nothing to do with optical transparency).
		CreateEx(WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_TOPMOST|WS_EX_TOOLWINDOW, 
							_T("STATIC"), _T(""), WS_POPUP|WS_VISIBLE, rc, AfxGetMainWnd(), 0);

		// UpdateLayeredWindow needs the size and origin of the source window.
		size.cx = rc.Width();
		size.cy = rc.Height();
		ptSrc = rc.TopLeft();
  
		// Set up the BLENDFUNCTION struct used by UpdateLayeredWindow
		m_Blend.BlendOp = AC_SRC_OVER;					// the only BlendOp defined in Windows 2000
		m_Blend.BlendFlags = 0;									// nothing else is special ...
		m_Blend.AlphaFormat = 0;								// ...
		m_Blend.SourceConstantAlpha = byAlpha;	// the initial alpha value

		// Display the new static window with the exact content and position of the source window.
		// When we return the caller can hide or destroy the source window and nothing will
		// appear to change. Subsequently (in OnTimer) we will reduce the alpha value to fade away
		// this copy window.
		m_pUpdateLayeredWindow(GetSafeHwnd(),
			                   NULL, 
							   NULL, 
							   &size, 
							   ::GetDC(m_pWndToFade->GetSafeHwnd()),				
							   &ptSrc, 
							   0, 
							   &m_Blend, 
							   ULW_ALPHA
							   );      //调用动态连接库中的函数

		// Calculate the timer interval required to complete the fade in the specified time.
		UINT	nElapse = nFadeTime / (byAlpha / ALPHA_STEP);
		SetTimer(1, nElapse, NULL);
		}
	}


CFaderWnd::~CFaderWnd()
	{
	}


BEGIN_MESSAGE_MAP(CFaderWnd, CWnd)
	//{{AFX_MSG_MAP(CFaderWnd)
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFaderWnd message handlers


void CFaderWnd::OnTimer(UINT nIDEvent) 	
{
	if (m_Blend.SourceConstantAlpha >= ALPHA_STEP)	
	{		
		m_Blend.SourceConstantAlpha -= ALPHA_STEP;  //减少透明度
		m_pUpdateLayeredWindow(GetSafeHwnd(), 
			                   NULL, 
							   NULL, 
							   NULL, 
							   NULL, 
							   NULL, 
							   NULL, 
							   &m_Blend, 
							   ULW_ALPHA);		
	}
	else		
	{
		KillTimer(nIDEvent);
		DestroyWindow();		
	}	
}


void CFaderWnd::OnSetFocus(CWnd* pOldWnd) 	
{
	
	if (pOldWnd)
		pOldWnd->SetFocus();// When we get the input focus pass it back to the previous holder, if any.
	
}

