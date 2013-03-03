#pragma once

#ifndef WS_EX_LAYERED
/////////////////////////////////////////////////////////////////////////////

#define WS_EX_LAYERED           0x00080000
#define ULW_ALPHA               0x00000002
#endif // ndef WS_EX_LAYERED


typedef BOOL (WINAPI *lpfnUpdateLayeredWindow)(HWND hWnd,
											   HDC hdcDst, 
											   POINT *pptDst,
											   SIZE *psize, 
											   HDC hdcSrc, 			
											   POINT *pptSrc, 
											   COLORREF crKey, 
											   BLENDFUNCTION *pblend, 
											   DWORD dwFlags
											   );
/////////////////////////////////////////////////////////////////////////////
class CFaderWnd : public CWnd
{

public:
	CFaderWnd(CWnd *pWndToFade, UINT nFadeTime = 500, BYTE byAlpha =255);  

public:

	//{{AFX_VIRTUAL(CFaderWnd)  //重载的虚函数
	protected:
	virtual void PostNcDestroy() 
	{
		delete this;
	}
	//}}AFX_VIRTUAL

private:
	virtual ~CFaderWnd();

protected:
	//{{AFX_MSG(CFaderWnd)               //消息处理函数
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	lpfnUpdateLayeredWindow m_pUpdateLayeredWindow;

	BLENDFUNCTION m_Blend;
	CWnd			 *m_pWndToFade;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

