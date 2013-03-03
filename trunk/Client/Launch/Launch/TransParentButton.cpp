// TransParentButton.cpp : implementation file
//

#include "stdafx.h"
#include "TransParentButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransParentButton

CTransParentButton::CTransParentButton()
{
	m_bBkTransparent=true;
	m_bIsFlat=true;
	m_bHasFocus=false;
	m_bIsEnabled=true;
	m_bMove=false;
	m_bDCStored=false;
	m_hMouseInIcon=NULL;
	m_hNormalIcon=NULL;
	m_IconAlign=AL_LEFT;
	m_DrawState=ST_MOVEOUT;
	m_IconYPos=5;
	m_IconXPos=5;
	m_bUseIcon=false;
	m_clNormalText=RGB(255,255,255);//::GetSysColor(COLOR_WINDOWTEXT);
	m_clActiveText=m_clNormalText;//RGB(0,0,0);//::GetSysColor(COLOR_HIGHLIGHT);//bgr
	m_clMouseDownText=::GetSysColor(COLOR_WINDOWTEXT);
	m_clText=m_clNormalText;
	m_cl3D=RGB(125,125,125);
	m_clDisableText=RGB(125,125,125);
	m_pFont = NULL;
}

CTransParentButton::~CTransParentButton()
{
}


BEGIN_MESSAGE_MAP(CTransParentButton, CButton)
	//{{AFX_MSG_MAP(CTransParentButton)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransParentButton message handlers

void CTransParentButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	if(lpDrawItemStruct->itemState & ODS_DISABLED )
	{
		m_bIsEnabled=false;
		m_DrawState=ST_MOVEOUT;
	}
	if(!(lpDrawItemStruct->itemState & ODS_DISABLED) )
	{
		m_bIsEnabled=true;
	}
	if(lpDrawItemStruct->itemState & ODS_FOCUS)
	{
		m_bHasFocus=true;
	}
	if( !(lpDrawItemStruct->itemState & ODS_FOCUS) )
	{
		m_bHasFocus=false;
	}
	Draw();
}

void CTransParentButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
    SetButtonStyle(GetButtonStyle() | BS_OWNERDRAW);	
	//CButton::PreSubclassWindow();
}

BOOL CTransParentButton::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	// First we should save our DC.
	if(!m_bDCStored)
	{
    	CRect  clientRect;
	    GetClientRect(&clientRect);
	    int cx=clientRect.Width ();
	    int cy=clientRect.Height ();
		// Store our orignal DC.
		CBitmap	bitmap;
		memDC.CreateCompatibleDC (pDC);
		bitmap.CreateCompatibleBitmap (pDC,cx,cy);
		memDC.SelectObject (&bitmap);
		memDC.BitBlt (0,0,cx,cy,pDC,0,0,SRCCOPY);
		m_bDCStored=true;
	}
	// We'll not erase our button's back ground.
	return TRUE;
}

void CTransParentButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (nFlags & MK_LBUTTON)
		return;
	if(!m_bMove && m_bIsEnabled)
	{
		// We'll not draw this button's borders more than one time.
		m_bMove=true;
		m_DrawState=ST_MOVEIN;
		Draw();
		SetTimer(1,10,NULL);
	}
	CButton::OnMouseMove(nFlags, point);
}

void CTransParentButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_DrawState=ST_MOUSEDOWN;
    Draw();
	CButton::OnLButtonDown(nFlags, point);
}

void CTransParentButton::DrawBorders()
{
	CClientDC	dc(this);
	CRect	clientRect;
	GetClientRect(&clientRect);

	COLORREF  clBtnHiLight=::GetSysColor(COLOR_BTNHILIGHT);
	COLORREF  clBtnShadow=::GetSysColor(COLOR_BTNSHADOW);
	COLORREF  clBtnBlack=RGB(0,0,0);

	if(!m_bIsFlat)
	{
		// Draw a ordinary-look button.
		CRect  drawRect=clientRect;
		switch(m_DrawState)
		{
			case ST_MOUSEUP:
			case ST_MOVEOUT:
			case ST_MOVEIN:
				dc.Draw3dRect (&drawRect,clBtnHiLight,clBtnBlack);
				drawRect.DeflateRect(1, 1);
				dc.Draw3dRect (&drawRect,clBtnHiLight,clBtnShadow);
				drawRect.InflateRect(1, 1);
				break;
			case ST_MOUSEDOWN:
				dc.Draw3dRect (&drawRect,clBtnBlack,clBtnHiLight);
				drawRect.DeflateRect(1, 1);
				dc.Draw3dRect (&drawRect,clBtnShadow,clBtnHiLight);
				drawRect.InflateRect(1, 1);
				break;
			default:
				break;
		}
		if(m_bHasFocus)
		{
		    CBrush br(RGB(0,0,0));  
			dc.FrameRect(&drawRect, &br);
			CPen  pen(PS_SOLID,0,clBtnBlack);
			dc.SelectObject (&pen);
			dc.MoveTo(drawRect.right-2,drawRect.top );
			dc.LineTo(drawRect.right-2,drawRect.bottom-2 );
			dc.LineTo(drawRect.left,drawRect.bottom-2 );
			drawRect.DeflateRect(4, 4);
			dc.DrawFocusRect(&drawRect);
		}
	}
	else
	// We'll draw a flat button.
	{
		if(!m_bIsEnabled)
			return;
		switch(m_DrawState)
		{
			case ST_MOUSEUP:
			case ST_MOVEIN:
				dc.Draw3dRect (&clientRect,
					::GetSysColor(COLOR_BTNHILIGHT),
					::GetSysColor(COLOR_BTNSHADOW) );
				break;
			case ST_MOUSEDOWN:
				dc.Draw3dRect (&clientRect,
					::GetSysColor(COLOR_BTNSHADOW),
					::GetSysColor(COLOR_BTNHILIGHT) );
				break;
			default:
				break;
		}
	}
}


void CTransParentButton::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent!=1)
		return;
	CPoint	point;
	CRect	rect;
	GetWindowRect(&rect);	
	GetCursorPos(&point);
	// If the mouse left our button,redraw it.
	if (!rect.PtInRect(point) && m_bMove && m_bIsEnabled)
	{
		KillTimer (1);
		m_DrawState=ST_MOVEOUT;
		m_bMove=false;
		Draw();
	}
	CButton::OnTimer(nIDEvent);
}

void CTransParentButton::Refresh()
{
	CRect	rect;
	GetClientRect(&rect);	
	CClientDC  dc(this);
	if(m_bBkTransparent==false)
		dc.FillRect(&rect,&CBrush(::GetSysColor(COLOR_BTNFACE)) );
	else
		dc.BitBlt (0,0,rect.Width (),rect.Height (),&memDC,0,0,SRCCOPY);
}

void CTransParentButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_DrawState=ST_MOUSEUP;

/*	CString  str;
	str.Format ("%d",m_IconXPos);
	MessageBox(str);
*/
    Draw();
	CButton::OnLButtonUp(nFlags, point);
}

void CTransParentButton::DrawText()
{
	CString		itemString;
	CRect  clientRect;
	CClientDC  dc(this);
	GetClientRect(&clientRect);
	GetWindowText(itemString);
	// Draw text depend on different state.
	if(itemString)
	{
		// First Get some necessary infomation.
		CSize size=dc.GetTextExtent (itemString);
		int rectwidth=clientRect.Width();
		int rectheight=clientRect.Height();
		int textwidth=size.cx ;
		int textheight=size.cy ;
		int iconwidth=::GetSystemMetrics(SM_CXICON);
		int iconheight=::GetSystemMetrics(SM_CYICON);
		int x,y; // the position of the text.
		// Next caculate the text output position.
		switch(m_IconAlign)
		{
			case AL_LEFT:
			{
				x = 2;
				y = 0;
				//if(m_bUseIcon==true)
				//{
				//	x=m_IconXPos+iconwidth+
				//		(rectwidth-textwidth-m_IconXPos-iconwidth)/2;
				//}
				//else
				//{
				//	x=(rectwidth-textwidth)/2;
				//}
				//y=(rectheight-textheight)/2;
				break;
			}
			case AL_TOP:
			{
				if(m_bUseIcon==true)
				{
					y=m_IconYPos+iconheight+
						(rectheight-textheight-m_IconYPos-iconheight)/2;
				}
				else
				{
					y=(rectheight-textheight)/2;
				}
				x=(rectwidth-textwidth)/2;
				break;
			}
			case AL_ME:
			{
				x = m_nAlignX;
				y = m_nAlignY;
				break;
			}
			default:
				break;
		}
		// Should we draw the text as if it is disabled?
		if(!m_bIsEnabled)
		{
			// .. Yes.Now draw the text disabled.m_clDisableText
			//dc.DrawState(CPoint(x,y),CSize(0,0),itemString,DST_TEXT | DSS_DISABLED,FALSE,0,(HBRUSH)NULL);
			m_clText = m_clDisableText;
			CFont* pOldFont = NULL;
			if( m_pFont )
				pOldFont = dc.SelectObject(m_pFont);
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(m_clText);
			dc.TextOut (x,y,itemString);
			if( pOldFont )
				dc.SelectObject(pOldFont);

		}
		else
		{
			switch(m_DrawState)
			{
				case ST_MOVEIN:
				{
					m_clText=m_clActiveText;
				}
					break;
				case ST_MOVEOUT:
				{
					m_clText=m_clNormalText;
				}
					break;
				case ST_MOUSEUP:
				{
					m_clText=m_clActiveText;
				}
					break;
				case ST_MOUSEDOWN:
				{
					m_clText=m_clNormalText;
				}
					break;
				default:
					break;
			}
			// Draw the text,as if it is pressed
			if(m_DrawState==ST_MOUSEDOWN)
			{
				x++;
				y++;
			}
			CFont* pOldFont = NULL;
			if( m_pFont )
				pOldFont = dc.SelectObject(m_pFont);
			dc.SetBkMode(TRANSPARENT);
			//dc.SetTextColor(m_cl3D);
			//dc.TextOut (x+1,y+1,itemString);
			dc.SetTextColor(m_clText);
			dc.TextOut (x,y,itemString);
			if( pOldFont )
				dc.SelectObject(pOldFont);
		}
	}
}

void CTransParentButton::SetIcon(HICON  hNormal,HICON hMouseIn)
{
	m_hNormalIcon=hNormal;
	m_hMouseInIcon=hMouseIn;
	m_bUseIcon=true;
}

void CTransParentButton::DrawIcon()
{
	if(m_bIsEnabled)
	{
		switch(m_DrawState)
		{
			case ST_MOVEIN:
			case ST_MOUSEDOWN:
			case ST_MOUSEUP:
				{
					if(m_hMouseInIcon!=NULL)
					{
						DrawIcon(m_hMouseInIcon);
					}
				}
				break;
			case ST_MOVEOUT:
				{
					if(m_hNormalIcon!=NULL)
						DrawIcon(m_hNormalIcon);
				}
				break;
			default:
				break;
		}
	}
	else
	{
	    DrawIcon(m_hNormalIcon,TRUE);
	}
}

void CTransParentButton::DrawIcon(HICON hIcon,BOOL bDisabled)
{
	CRect	clientrect;
	CClientDC	dc(this);
	int cx=::GetSystemMetrics(SM_CXICON);
	int cy=::GetSystemMetrics(SM_CYICON);
	int x,y; // the icon's output position.
	int rectwidth,rectheight;
	GetClientRect(&clientrect);
	rectwidth=clientrect.Width ();
	rectheight=clientrect.Height ();

	CString		itemString;
	GetWindowText(itemString);

	if(itemString!="")
	{
		switch(m_IconAlign)
		{
			case AL_LEFT:
			// draw the icon left.
			{
				x=m_IconXPos;
				y=(rectheight-cy)/2;
				break;
			}
			case AL_TOP:
			{
				x=(rectwidth-cx)/2;
				y=m_IconYPos;
				break;
			}
			case AL_ME:
			{
				x = m_nAlignX;
				y = m_nAlignY;
				break;
			}
			default:
				break;
		}
	}
	else
	// If there is not any text,we'll center the icon.
	{
		x=(rectwidth-cx)/2;
		y=(rectheight-cy)/2;
	}
	// Draw the icon,as if it is pressed
	if(m_DrawState==ST_MOUSEDOWN)
	{
		x++;
		y++;
	}
	// Draw the icon.
	if(!bDisabled)
	{
	    dc.DrawIcon (x,y,hIcon);
	}
	else
	{
    	dc.DrawState(CPoint(x,y),CSize(32,32),hIcon,DST_ICON | DSS_DISABLED,(HBRUSH)NULL);
	}
}

void CTransParentButton::Draw()
{
	Refresh();
	DrawBorders();
	DrawIcon();
	DrawText();
}

void CTransParentButton::SetIconXPos(int x)
{
	m_IconXPos=x;
	Draw();
}

void CTransParentButton::SetAlign(int nAlign)
{
	m_IconAlign=nAlign;
	Draw();
}

void CTransParentButton::SetAlign(int x, int y)
{
	m_IconAlign = AL_ME;
	m_nAlignX = x;
	m_nAlignY = y;
	Draw();
}

void CTransParentButton::SetIconYPos(int y)
{
	m_IconYPos=y;
	Draw();
}

void CTransParentButton::SetActiveTextColor(COLORREF color)
{
	m_clActiveText=color;
	Draw();
}

void CTransParentButton::SetDisableTextColor(COLORREF color)
{
	m_clDisableText = color;
	Draw();
}

void CTransParentButton::SetNormalTextColor(COLORREF color)
{
	m_clNormalText=color;
	Draw();
}

void CTransParentButton::SetMouseDownTextColor(COLORREF color)
{
	m_clMouseDownText=color;
	Draw();
}

void CTransParentButton::SetBkMode(bool bTransparent)
{
	m_bBkTransparent=bTransparent;
}

void CTransParentButton::SetStyle(bool bIsflat)
{
	m_bIsFlat=bIsflat;
}

BOOL CTransParentButton::SetCursor(UINT nCursorID)
{
	HINSTANCE hInstResource;
	// Destroy any previous cursor
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
	m_hCursor = NULL;

	// If we want a cursor
	if (nCursorID != -1)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nCursorID),
											RT_GROUP_CURSOR);
		// Load icon resource
		m_hCursor = (HCURSOR)::LoadImage(hInstResource/*AfxGetApp()->m_hInstance*/, MAKEINTRESOURCE(nCursorID), IMAGE_CURSOR, 0, 0, 0);
		// If something wrong then return FALSE
		if (m_hCursor == NULL) return FALSE;
	}

	return TRUE;
}

BOOL CTransParentButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}	
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

void CTransParentButton::SetIcon(UINT  nNormal,UINT nMouseIn)
{
	m_hNormalIcon=AfxGetApp()->LoadIcon(nNormal);
	m_hMouseInIcon=AfxGetApp()->LoadIcon(nMouseIn);
	m_bUseIcon=true;
}

void CTransParentButton::SetFont(CFont* pFont)
{
	m_pFont = pFont;
}
