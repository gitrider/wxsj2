/////////////////////////////////////////////////////////////////////////////
// Name:        msw/statbox.cpp
// Purpose:     wxStaticBox
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: statbox.cpp,v 1.82 2005/05/22 18:29:14 VZ Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "statbox.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_STATBOX

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/dcclient.h"
#endif

#include "wx/statbox.h"
#include "wx/notebook.h"
#include "wx/sysopt.h"
#include "wx/image.h"
#include "wx/dcmemory.h"
#include "wx/sysopt.h"

#include "wx/msw/uxtheme.h"
#include "wx/msw/private.h"
#include "wx/msw/missing.h"

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

#if wxUSE_EXTENDED_RTTI
WX_DEFINE_FLAGS( wxStaticBoxStyle )

wxBEGIN_FLAGS( wxStaticBoxStyle )
    // new style border flags, we put them first to
    // use them for streaming out
    wxFLAGS_MEMBER(wxBORDER_SIMPLE)
    wxFLAGS_MEMBER(wxBORDER_SUNKEN)
    wxFLAGS_MEMBER(wxBORDER_DOUBLE)
    wxFLAGS_MEMBER(wxBORDER_RAISED)
    wxFLAGS_MEMBER(wxBORDER_STATIC)
    wxFLAGS_MEMBER(wxBORDER_NONE)

    // old style border flags
    wxFLAGS_MEMBER(wxSIMPLE_BORDER)
    wxFLAGS_MEMBER(wxSUNKEN_BORDER)
    wxFLAGS_MEMBER(wxDOUBLE_BORDER)
    wxFLAGS_MEMBER(wxRAISED_BORDER)
    wxFLAGS_MEMBER(wxSTATIC_BORDER)
    wxFLAGS_MEMBER(wxBORDER)

    // standard window styles
    wxFLAGS_MEMBER(wxTAB_TRAVERSAL)
    wxFLAGS_MEMBER(wxCLIP_CHILDREN)
    wxFLAGS_MEMBER(wxTRANSPARENT_WINDOW)
    wxFLAGS_MEMBER(wxWANTS_CHARS)
    wxFLAGS_MEMBER(wxFULL_REPAINT_ON_RESIZE)
    wxFLAGS_MEMBER(wxALWAYS_SHOW_SB )
    wxFLAGS_MEMBER(wxVSCROLL)
    wxFLAGS_MEMBER(wxHSCROLL)

wxEND_FLAGS( wxStaticBoxStyle )

IMPLEMENT_DYNAMIC_CLASS_XTI(wxStaticBox, wxControl,"wx/statbox.h")

wxBEGIN_PROPERTIES_TABLE(wxStaticBox)
    wxPROPERTY( Label,wxString, SetLabel, GetLabel, wxString() , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY_FLAGS( WindowStyle , wxStaticBoxStyle , long , SetWindowStyleFlag , GetWindowStyleFlag , EMPTY_MACROVALUE, 0 /*flags*/ , wxT("Helpstring") , wxT("group")) // style
/*
    TODO PROPERTIES :
        label
*/
wxEND_PROPERTIES_TABLE()

wxBEGIN_HANDLERS_TABLE(wxStaticBox)
wxEND_HANDLERS_TABLE()

wxCONSTRUCTOR_6( wxStaticBox , wxWindow* , Parent , wxWindowID , Id , wxString , Label , wxPoint , Position , wxSize , Size , long , WindowStyle )
#else
IMPLEMENT_DYNAMIC_CLASS(wxStaticBox, wxControl)
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxStaticBox
// ----------------------------------------------------------------------------

bool wxStaticBox::Create(wxWindow *parent,
                         wxWindowID id,
                         const wxString& label,
                         const wxPoint& pos,
                         const wxSize& size,
                         long style,
                         const wxString& name)
{
    if ( !CreateControl(parent, id, pos, size, style, wxDefaultValidator, name) )
        return false;

    if ( !MSWCreateControl(wxT("BUTTON"), label, pos, size) )
        return false;

#ifndef __WXWINCE__
    if (!wxSystemOptions::IsFalse(wxT("msw.staticbox.optimized-paint")))
        Connect(wxEVT_PAINT, wxPaintEventHandler(wxStaticBox::OnPaint));
#endif // !__WXWINCE__

    return true;
}

wxBorder wxStaticBox::GetDefaultBorder() const
{
    return wxBORDER_NONE;
}

WXDWORD wxStaticBox::MSWGetStyle(long style, WXDWORD *exstyle) const
{
    long styleWin = wxStaticBoxBase::MSWGetStyle(style, exstyle);

    // no need for it anymore, must be removed for wxRadioBox child
    // buttons to be able to repaint themselves
    styleWin &= ~WS_CLIPCHILDREN;

    if ( exstyle )
    {
#ifndef __WXWINCE__
        if (wxSystemOptions::IsFalse(wxT("msw.staticbox.optimized-paint")))
            *exstyle = WS_EX_TRANSPARENT;
        else
#endif
            *exstyle = 0;
    }

    return styleWin | BS_GROUPBOX;
}

wxSize wxStaticBox::DoGetBestSize() const
{
    int cx, cy;
    wxGetCharSize(GetHWND(), &cx, &cy, GetFont());

    int wBox;
    GetTextExtent(wxGetWindowText(m_hWnd), &wBox, &cy);

    wBox += 3*cx;
    int hBox = EDIT_HEIGHT_FROM_CHAR_HEIGHT(cy);

    wxSize best(wBox, hBox);
    CacheBestSize(best);
    return best;
}

void wxStaticBox::GetBordersForSizer(int *borderTop, int *borderOther) const
{
    wxStaticBoxBase::GetBordersForSizer(borderTop, borderOther);

    // need extra space, don't know how much but this seems to be enough
    *borderTop += GetCharHeight()/3;
}

// all the hacks below are not necessary for WinCE
#ifndef __WXWINCE__

WXLRESULT wxStaticBox::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    if ( nMsg == WM_NCHITTEST )
    {
        // This code breaks some other processing such as enter/leave tracking
        // so it's off by default.

        static int s_useHTClient = -1;
        if (s_useHTClient == -1)
            s_useHTClient = wxSystemOptions::GetOptionInt(wxT("msw.staticbox.htclient"));
        if (s_useHTClient == 1)
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            ScreenToClient(&xPos, &yPos);

            // Make sure you can drag by the top of the groupbox, but let
            // other (enclosed) controls get mouse events also
            if ( yPos < 10 )
                return (long)HTCLIENT;
        }
    }

    if ( nMsg == WM_PRINTCLIENT )
    {
        // we have to process WM_PRINTCLIENT ourselves as otherwise child
        // windows' background (eg buttons in radio box) would never be drawn
        // unless we have a parent with non default background

        // so check first if we have one
        if ( !HandlePrintClient((WXHDC)wParam) )
        {
            // no, we don't, erase the background ourselves
            // (don't use our own) - see PaintBackground for explanation
            wxBrush brush(GetParent()->GetBackgroundColour());
            wxFillRect(GetHwnd(), (HDC)wParam, GetHbrushOf(brush));
        }

        return 0;
    }

    return wxControl::MSWWindowProc(nMsg, wParam, lParam);
}

// ----------------------------------------------------------------------------
// static box drawing
// ----------------------------------------------------------------------------

/*
   We draw the static box ourselves because it's the only way to prevent it
   from flickering horribly on resize (because everything inside the box is
   erased twice: once when the box itself is repainted and second time when
   the control inside it is repainted) without using WS_EX_TRANSPARENT style as
   we used to do and which resulted in other problems.
 */

// MSWGetRegionWithoutSelf helper: removes the given rectangle from region
static inline void
SubtractRectFromRgn(HRGN hrgn, int left, int top, int right, int bottom)
{
    AutoHRGN hrgnRect(::CreateRectRgn(left, top, right, bottom));
    if ( !hrgnRect )
    {
        wxLogLastError(_T("CreateRectRgn()"));
        return;
    }

    ::CombineRgn(hrgn, hrgn, hrgnRect, RGN_DIFF);
}

void wxStaticBox::MSWGetRegionWithoutSelf(WXHRGN hRgn, int w, int h)
{
    HRGN hrgn = (HRGN)hRgn;

    // remove the area occupied by the static box borders from the region
    int borderTop, border;
    GetBordersForSizer(&borderTop, &border);

    // top
    SubtractRectFromRgn(hrgn, 0, 0, w, borderTop);

    // bottom
    SubtractRectFromRgn(hrgn, 0, h - border, w, h);

    // left
    SubtractRectFromRgn(hrgn, 0, 0, border, h);

    // right
    SubtractRectFromRgn(hrgn, w - border, 0, w, h);
}

WXHRGN wxStaticBox::MSWGetRegionWithoutChildren()
{
    RECT rc;
    ::GetWindowRect(GetHwnd(), &rc);
    HRGN hrgn = ::CreateRectRgn(rc.left, rc.top, rc.right + 1, rc.bottom + 1);

    // iterate over all child windows (not just wxWindows but all windows)
    for ( HWND child = ::GetWindow(GetHwndOf(GetParent()), GW_CHILD);
          child;
          child = ::GetWindow(child, GW_HWNDNEXT) )
    {
        if ( ! ::IsWindowVisible(child) )
        {
            // if the window isn't visible then it doesn't need clipped
            continue;
        }

        LONG style = ::GetWindowLong(child, GWL_STYLE);
        wxString str(wxGetWindowClass(child));
        str.UpperCase();
        if ( str == wxT("BUTTON") && (style & BS_GROUPBOX) == BS_GROUPBOX )
        {
            // Don't clip any static boxes, not just this one.  This will
            // result in flicker in overlapping static boxes, but at least
            // they will all be drawn correctly and we shouldn't have
            // overlapping windows anyway.
            continue;
        }

        ::GetWindowRect(child, &rc);
        if ( ::RectInRegion(hrgn, &rc) )
        {
            // need to remove WS_CLIPSIBLINGS from all sibling windows
            // that are within this staticbox if set
            if ( style & WS_CLIPSIBLINGS )
            {
                style &= ~WS_CLIPSIBLINGS;
                ::SetWindowLong(child, GWL_STYLE, style);

                // MSDN: "If you have changed certain window data using
                // SetWindowLong, you must call SetWindowPos to have the
                // changes take effect."
                ::SetWindowPos(child, NULL, 0, 0, 0, 0,
                               SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                               SWP_FRAMECHANGED);
            }

            AutoHRGN hrgnChild(::CreateRectRgnIndirect(&rc));
            ::CombineRgn(hrgn, hrgn, hrgnChild, RGN_DIFF);
        }
    }

    return (WXHRGN)hrgn;
}

// helper for OnPaint(): really erase the background, i.e. do it even if we
// don't have any non default brush for doing it (DoEraseBackground() doesn't
// do anything in such case)
void wxStaticBox::PaintBackground(wxDC& dc, const RECT& rc)
{
    // note that we do not use the box background colour here, it shouldn't
    // apply to its interior for several reasons:
    //  1. wxGTK doesn't do it
    //  2. controls inside the box don't get correct bg colour because they
    //     are not our children so we'd have some really ugly colour mix if
    //     we did it
    //  3. this is backwards compatible behaviour and some people rely on it,
    //     see http://groups.google.com/groups?selm=4252E932.3080801%40able.es
    wxWindow *parent = GetParent();
    HBRUSH hbr = (HBRUSH)parent->MSWGetBgBrush(dc.GetHDC(), GetHWND());

    // if there is no special brush for painting this control, just use the
    // solid background colour
    wxBrush brush;
    if ( !hbr )
    {
        brush = wxBrush(parent->GetBackgroundColour());
        hbr = GetHbrushOf(brush);
    }

    ::FillRect(GetHdcOf(dc), &rc, hbr);
}

void wxStaticBox::PaintForeground(wxDC& dc, const RECT& WXUNUSED(rc))
{
    MSWDefWindowProc(WM_PAINT, (WPARAM)GetHdcOf(dc), 0);

    // when using XP themes, neither setting the text colour nor transparent
    // background mode doesn't change anything: the static box def window proc
    // still draws the label in its own colours, so we need to redraw the text
    // ourselves if we have a non default fg colour
    if ( m_hasFgCol && wxUxThemeEngine::GetIfActive() )
    {
        // draw over the text in default colour in our colour
        dc.SetFont(GetFont());

        HDC hdc = GetHdcOf(dc);
        ::SetTextColor(hdc, GetForegroundColour().GetPixel());

        // FIXME: value of x is hardcoded as this is what it is on my system,
        //        no idea if it's true everywhere
        const int y = dc.GetCharHeight();
        const int x = 9;

        // TODO: RTL?
        RECT rc = { x, 0, GetSize().x - x, y };

        const wxString label = GetLabel();
        ::DrawText(hdc, label, label.length(), &rc, DT_SINGLELINE | DT_VCENTER);
    }
}

void wxStaticBox::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    RECT rc;
    ::GetClientRect(GetHwnd(), &rc);

    // draw the entire box in a memory DC
    wxMemoryDC memdc;
    wxBitmap bitmap(rc.right, rc.bottom);
    memdc.SelectObject(bitmap);

    PaintBackground(memdc, rc);
    PaintForeground(memdc, rc);

    // now only blit the static box border itself, not the interior, to avoid
    // flicker when background is drawn below
    //
    // note that it seems to be faster to do 4 small blits here and then paint
    // directly into wxPaintDC than painting background in wxMemoryDC and then
    // blitting everything at once to wxPaintDC, this is why we do it like this
    wxPaintDC dc(this);
    int borderTop, border;
    GetBordersForSizer(&borderTop, &border);

    // top
    dc.Blit(border, 0, rc.right - border, borderTop,
            &memdc, border, 0);
    // bottom
    dc.Blit(border, rc.bottom - border, rc.right - border, rc.bottom,
            &memdc, border, rc.bottom - border);
    // left
    dc.Blit(0, 0, border, rc.bottom,
            &memdc, 0, 0);
    // right
    dc.Blit(rc.right - border, 0, rc.right, rc.bottom,
            &memdc, rc.right - border, 0);


    // create the region excluding box children
    AutoHRGN hrgn((HRGN)MSWGetRegionWithoutChildren());
    RECT rcWin;
    ::GetWindowRect(GetHwnd(), &rcWin);
    ::OffsetRgn(hrgn, -rcWin.left, -rcWin.top);

    // and also the box itself
    MSWGetRegionWithoutSelf((WXHRGN) hrgn, rc.right, rc.bottom);
    HDCClipper clipToBg(GetHdcOf(dc), hrgn);

    // paint the inside of the box (excluding box itself and child controls)
    PaintBackground(dc, rc);
}

#endif // !__WXWINCE__

#endif // wxUSE_STATBOX

