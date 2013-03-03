///////////////////////////////////////////////////////////////////////////////
// Name:        msw/tooltip.h
// Purpose:     wxToolTip class - tooltip control
// Author:      Vadim Zeitlin
// Modified by:
// Created:     31.01.99
// RCS-ID:      $Id: tooltip.h,v 1.13 2005/02/28 01:22:35 VZ Exp $
// Copyright:   (c) 1999 Robert Roebling, Vadim Zeitlin
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

class WXDLLEXPORT wxToolTip : public wxObject
{
public:
    // ctor & dtor
    wxToolTip(const wxString &tip);
    virtual ~wxToolTip();

    // accessors
        // tip text
    void SetTip(const wxString& tip);
    const wxString& GetTip() const { return m_text; }

        // the window we're associated with
    void SetWindow(wxWindow *win);
    wxWindow *GetWindow() const { return m_window; }

    // controlling tooltip behaviour: globally change tooltip parameters
        // enable or disable the tooltips globally
    static void Enable(bool flag);
        // set the delay after which the tooltip appears
    static void SetDelay(long milliseconds);

    // implementation only from now on
    // -------------------------------

    // should be called in responde to WM_MOUSEMOVE
    void RelayEvent(WXMSG *msg);

    // add a window to the tooltip control
    void Add(WXHWND hwnd);

private:
    // the one and only one tooltip control we use - never access it directly
    // but use GetToolTipCtrl() which will create it when needed
    static WXHWND ms_hwndTT;

    // create the tooltip ctrl if it doesn't exist yet and return its HWND
    static WXHWND GetToolTipCtrl();

    // remove this tooltip from the tooltip control
    void Remove();

    wxString  m_text;           // tooltip text
    wxWindow *m_window;         // window we're associated with

    DECLARE_ABSTRACT_CLASS(wxToolTip)
    DECLARE_NO_COPY_CLASS(wxToolTip)
};

