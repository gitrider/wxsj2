/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/scrolwin.h
// Purpose:     wxGenericScrolledWindow class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: scrolwin.h,v 1.41 2005/03/31 05:08:24 SC Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_SCROLLWIN_H_
#define _WX_GENERIC_SCROLLWIN_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "genscrolwin.h"
#endif

// ----------------------------------------------------------------------------
// headers and constants
// ----------------------------------------------------------------------------

#include "wx/window.h"
#include "wx/panel.h"

extern WXDLLEXPORT_DATA(const wxChar*) wxPanelNameStr;

// default scrolled window style
#ifndef wxScrolledWindowStyle
    #define wxScrolledWindowStyle (wxHSCROLL | wxVSCROLL)
#endif

// avoid triggering this stupid VC++ warning
#ifdef __VISUALC__
    #pragma warning(disable:4355) // 'this' used in base member initializer list
#endif

// ----------------------------------------------------------------------------
// wxGenericScrolledWindow
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxGenericScrolledWindow : public wxPanel,
                                            public wxScrollHelper
{
public:
    wxGenericScrolledWindow() : wxScrollHelper(this) { }
    wxGenericScrolledWindow(wxWindow *parent,
                     wxWindowID winid = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxScrolledWindowStyle,
                     const wxString& name = wxPanelNameStr)
        : wxScrollHelper(this)
    {
        Create(parent, winid, pos, size, style, name);
    }

    virtual ~wxGenericScrolledWindow();

    bool Create(wxWindow *parent,
                wxWindowID winid,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxScrolledWindowStyle,
                const wxString& name = wxPanelNameStr);

    virtual void PrepareDC(wxDC& dc) { DoPrepareDC(dc); }

    // lay out the window and its children
    virtual bool Layout();

    virtual void DoSetVirtualSize(int x, int y);

    // wxWindow's GetBestVirtualSize returns the actual window size,
    // whereas we want to return the virtual size
    virtual wxSize GetBestVirtualSize() const;

    // Return the size best suited for the current window
    // (this isn't a virtual size, this is a sensible size for the window)
    virtual wxSize DoGetBestSize() const;

protected:
    // this is needed for wxEVT_PAINT processing hack described in
    // wxScrollHelperEvtHandler::ProcessEvent()
    void OnPaint(wxPaintEvent& event);

    // we need to return a special WM_GETDLGCODE value to process just the
    // arrows but let the other navigation characters through
#ifdef __WXMSW__
    virtual WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);
#endif // __WXMSW__

private:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxGenericScrolledWindow)
    DECLARE_EVENT_TABLE()
};

#ifdef __VISUALC__
    #pragma warning(default:4355)
#endif

#endif
    // _WX_GENERIC_SCROLLWIN_H_

