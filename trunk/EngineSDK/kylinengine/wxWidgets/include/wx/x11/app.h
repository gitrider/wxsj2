/////////////////////////////////////////////////////////////////////////////
// Name:        app.h
// Purpose:     wxApp class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: app.h,v 1.31 2004/05/23 20:51:51 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_APP_H_
#define _WX_APP_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "app.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/gdicmn.h"
#include "wx/event.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxFrame;
class WXDLLEXPORT wxWindow;
class WXDLLEXPORT wxApp;
class WXDLLEXPORT wxKeyEvent;
class WXDLLEXPORT wxLog;
class WXDLLEXPORT wxXVisualInfo;

// ----------------------------------------------------------------------------
// the wxApp class for wxX11 - see wxAppBase for more details
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxApp : public wxAppBase
{
    DECLARE_DYNAMIC_CLASS(wxApp)

public:
    wxApp();
    ~wxApp();

    // override base class (pure) virtuals
    // -----------------------------------

    virtual void Exit();

    virtual bool Yield(bool onlyIfNeeded = FALSE);
    virtual void WakeUpIdle();

    virtual bool OnInitGui();

    // implementation from now on
    // --------------------------

    // Processes an X event.
    virtual bool ProcessXEvent(WXEvent* event);

#ifdef __WXDEBUG__
    virtual void OnAssert(const wxChar *file, int line, const wxChar* cond, const wxChar *msg);
#endif // __WXDEBUG__

protected:
    bool                  m_showOnInit;

public:
    // Implementation
    virtual bool Initialize(int& argc, wxChar **argv);
    virtual void CleanUp();

    WXWindow       GetTopLevelWidget() const { return m_topLevelWidget; }
    WXColormap     GetMainColormap(WXDisplay* display);
    long           GetMaxRequestSize() const { return m_maxRequestSize; }

    // This handler is called when a property change event occurs
    virtual bool HandlePropertyChange(WXEvent *event);

    // Values that can be passed on the command line.
    // Returns -1, -1 if none specified.
    const wxSize& GetInitialSize() const { return m_initialSize; }
    bool GetShowIconic() const { return m_showIconic; }

#if wxUSE_UNICODE
    // Global context for Pango layout. Either use X11
    // or use Xft rendering according to GDK_USE_XFT
    // environment variable
    PangoContext* GetPangoContext();
#endif

    wxXVisualInfo* GetVisualInfo(WXDisplay* display)
    {
        // this should be implemented correctly for wxBitmap to work
        // with multiple display
        return m_visualInfo;
    }

    // We need this before creating the app
    static   WXDisplay* GetDisplay() { return ms_display; }
    static   WXDisplay* ms_display;

public:
    static long           sm_lastMessageTime;
    bool                  m_showIconic;
    wxSize                m_initialSize;

#if !wxUSE_NANOX
    wxXVisualInfo*        m_visualInfo;
#endif

protected:
    bool                  m_keepGoing;

    WXWindow              m_topLevelWidget;
    WXColormap            m_mainColormap;
    long                  m_maxRequestSize;

    DECLARE_EVENT_TABLE()
};

#endif // _WX_APP_H_

