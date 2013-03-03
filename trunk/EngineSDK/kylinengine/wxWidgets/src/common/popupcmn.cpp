///////////////////////////////////////////////////////////////////////////////
// Name:        common/popupcmn.cpp
// Purpose:     implementation of wxPopupTransientWindow
// Author:      Vadim Zeitlin
// Modified by:
// Created:     06.01.01
// RCS-ID:      $Id: popupcmn.cpp,v 1.57 2005/05/31 09:19:52 JS Exp $
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// License:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "popupwinbase.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_POPUPWIN

#include "wx/popupwin.h"

#ifndef WX_PRECOMP
    #include "wx/combobox.h"        // wxComboControl
    #include "wx/app.h"             // wxPostEvent
    #include "wx/log.h"
    #include "wx/app.h"
#endif //WX_PRECOMP

#ifdef __WXUNIVERSAL__
    #include "wx/univ/renderer.h"
#endif // __WXUNIVERSAL__

#ifdef __WXGTK__
    #include <gtk/gtk.h>
#endif
#ifdef __WXX11__
#include "wx/x11/private.h"
#endif

IMPLEMENT_DYNAMIC_CLASS(wxPopupWindow, wxWindow)
IMPLEMENT_DYNAMIC_CLASS(wxPopupTransientWindow, wxPopupWindow)

#if wxUSE_COMBOBOX && defined(__WXUNIVERSAL__)
    IMPLEMENT_DYNAMIC_CLASS(wxPopupComboWindow, wxPopupTransientWindow)
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// event handlers which we use to intercept events which cause the popup to
// disappear
class wxPopupWindowHandler : public wxEvtHandler
{
public:
    wxPopupWindowHandler(wxPopupTransientWindow *popup) : m_popup(popup) {}
    ~wxPopupWindowHandler();

protected:
    // event handlers
    void OnLeftDown(wxMouseEvent& event);

private:
    wxPopupTransientWindow *m_popup;

    DECLARE_EVENT_TABLE()
    DECLARE_NO_COPY_CLASS(wxPopupWindowHandler)
};

class wxPopupFocusHandler : public wxEvtHandler
{
public:
    wxPopupFocusHandler(wxPopupTransientWindow *popup) : m_popup(popup) {}
    ~wxPopupFocusHandler();

protected:
    void OnKillFocus(wxFocusEvent& event);
    void OnKeyDown(wxKeyEvent& event);

private:
    wxPopupTransientWindow *m_popup;

    DECLARE_EVENT_TABLE()
    DECLARE_NO_COPY_CLASS(wxPopupFocusHandler)
};

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxPopupWindowHandler, wxEvtHandler)
    EVT_LEFT_DOWN(wxPopupWindowHandler::OnLeftDown)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxPopupFocusHandler, wxEvtHandler)
    EVT_KILL_FOCUS(wxPopupFocusHandler::OnKillFocus)
    EVT_KEY_DOWN(wxPopupFocusHandler::OnKeyDown)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxPopupTransientWindow, wxPopupWindow)
#ifdef __WXMSW__
    EVT_IDLE(wxPopupTransientWindow::OnIdle)
#endif
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxPopupWindowBase
// ----------------------------------------------------------------------------

wxPopupWindowBase::~wxPopupWindowBase()
{
    // this destructor is required for Darwin
}

bool wxPopupWindowBase::Create(wxWindow* WXUNUSED(parent), int WXUNUSED(flags))
{
    return true;
}

void wxPopupWindowBase::Position(const wxPoint& ptOrigin,
                                 const wxSize& size)
{
    wxSize sizeScreen = wxGetDisplaySize(),
           sizeSelf = GetSize();

    // is there enough space to put the popup below the window (where we put it
    // by default)?
    wxCoord y = ptOrigin.y + size.y;
    if ( y + sizeSelf.y > sizeScreen.y )
    {
        // check if there is enough space above
        if ( ptOrigin.y > sizeSelf.y )
        {
            // do position the control above the window
            y -= size.y + sizeSelf.y;
        }
        //else: not enough space below nor above, leave below
    }

    // now check left/right too
    wxCoord x = ptOrigin.x + size.x;
    if ( x + sizeSelf.x > sizeScreen.x )
    {
        // check if there is enough space to the left
        if ( ptOrigin.x > sizeSelf.x )
        {
            // do position the control to the left
            x -= size.x + sizeSelf.x;
        }
        //else: not enough space there neither, leave in default position
    }

    Move(x, y, wxSIZE_NO_ADJUSTMENTS);
}

// ----------------------------------------------------------------------------
// wxPopupTransientWindow
// ----------------------------------------------------------------------------

void wxPopupTransientWindow::Init()
{
    m_child =
    m_focus = (wxWindow *)NULL;

    m_handlerFocus = NULL;
    m_handlerPopup = NULL;
}

wxPopupTransientWindow::wxPopupTransientWindow(wxWindow *parent, int style)
{
    Init();

    (void)Create(parent, style);
}

wxPopupTransientWindow::~wxPopupTransientWindow()
{
    if (m_handlerPopup && m_handlerPopup->GetNextHandler())
        PopHandlers();
        
    wxASSERT(!m_handlerFocus || !m_handlerFocus->GetNextHandler());
    wxASSERT(!m_handlerPopup || !m_handlerPopup->GetNextHandler());

    delete m_handlerFocus;
    delete m_handlerPopup;
}

void wxPopupTransientWindow::PopHandlers()
{
    if ( m_child )
    {
        if ( !m_child->RemoveEventHandler(m_handlerPopup) )
        {
            // something is very wrong and someone else probably deleted our
            // handler - so don't risk deleting it second time
            m_handlerPopup = NULL;
        }
        if (m_child->HasCapture())
        {       
            m_child->ReleaseMouse();
        }
        m_child = NULL;
    }

    if ( m_focus )
    {
        if ( !m_focus->RemoveEventHandler(m_handlerFocus) )
        {
            // see above
            m_handlerFocus = NULL;
        }
    }
    m_focus = NULL;
}

void wxPopupTransientWindow::Popup(wxWindow *winFocus)
{
    const wxWindowList& children = GetChildren();
    if ( children.GetCount() )
    {
        m_child = children.GetFirst()->GetData();
    }
    else
    {
        m_child = this;
    }

    Show();

    // There is is a problem if these are still in use
    wxASSERT(!m_handlerFocus || !m_handlerFocus->GetNextHandler());
    wxASSERT(!m_handlerPopup || !m_handlerPopup->GetNextHandler());

    if (!m_handlerPopup)
        m_handlerPopup = new wxPopupWindowHandler(this);

    m_child->PushEventHandler(m_handlerPopup);

    m_focus = winFocus ? winFocus : this;
    m_focus->SetFocus();

#ifdef __WXMSW__
    // MSW doesn't allow to set focus to the popup window, but we need to
    // subclass the window which has the focus, and not winFocus passed in or
    // otherwise everything else breaks down
    m_focus = FindFocus();
#elif __WXGTK__
    // GTK+ catches the activate events from the popup
    // window, not the focus events from the child window
    m_focus = this;
#endif

    if ( m_focus )
    {
        if (!m_handlerFocus)
            m_handlerFocus = new wxPopupFocusHandler(this);

        m_focus->PushEventHandler(m_handlerFocus);
    }

    // catch destroy events, if you close a program with a popup shown in MSW
    // you get a segfault if m_child or m_focus are deleted before this is
    m_child->Connect(wxEVT_DESTROY,
                     wxWindowDestroyEventHandler(wxPopupTransientWindow::OnDestroy),
                     NULL, this);
    if (m_focus)
        m_focus->Connect(wxEVT_DESTROY,
                         wxWindowDestroyEventHandler(wxPopupTransientWindow::OnDestroy),
                         NULL, this);
}

bool wxPopupTransientWindow::Show( bool show )
{
#ifdef __WXGTK__
    if (!show)
    {
        gdk_pointer_ungrab( (guint32)GDK_CURRENT_TIME );

        gtk_grab_remove( m_widget );
    }
#endif

#ifdef __WXX11__
    if (!show)
    {
        XUngrabPointer( wxGlobalDisplay(), CurrentTime );
    }
#endif

#ifdef __WXMSW__
    if (!show && m_child && m_child->HasCapture())
    {       
        m_child->ReleaseMouse();
    }
#endif
    
    bool ret = wxPopupWindow::Show( show );

#ifdef __WXGTK__
    if (show)
    {
        gtk_grab_add( m_widget );

        gdk_pointer_grab( m_widget->window, TRUE,
                          (GdkEventMask)
                            (GDK_BUTTON_PRESS_MASK |
                             GDK_BUTTON_RELEASE_MASK |
                             GDK_POINTER_MOTION_HINT_MASK |
                             GDK_POINTER_MOTION_MASK),
                          (GdkWindow *) NULL,
                          (GdkCursor *) NULL,
                          (guint32)GDK_CURRENT_TIME );
    }
#endif

#ifdef __WXX11__
    if (show)
    {
        Window xwindow = (Window) m_clientWindow;

        /* int res =*/ XGrabPointer(wxGlobalDisplay(), xwindow,
            True,
            ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | EnterWindowMask | LeaveWindowMask | PointerMotionMask,
            GrabModeAsync,
            GrabModeAsync,
            None,
            None,
            CurrentTime );
    }
#endif

#ifdef __WXMSW__
    if (show && m_child)
    {
        // Assume that the mouse is outside the popup to begin with
        m_child->CaptureMouse();
    }
#endif

    return ret;
}

void wxPopupTransientWindow::Dismiss()
{
    Hide();
    PopHandlers();
}

void wxPopupTransientWindow::DismissAndNotify()
{
    Dismiss();
    OnDismiss();
}

void wxPopupTransientWindow::OnDismiss()
{
    // nothing to do here - but it may be interesting for derived class
}

bool wxPopupTransientWindow::ProcessLeftDown(wxMouseEvent& WXUNUSED(event))
{
    // no special processing here
    return false;
}

void wxPopupTransientWindow::OnDestroy(wxWindowDestroyEvent& event)
{
    if (event.GetEventObject() == m_child)
        m_child = NULL;
    if (event.GetEventObject() == m_focus)
        m_focus = NULL;
}

#ifdef __WXMSW__
void wxPopupTransientWindow::OnIdle(wxIdleEvent& event)
{
    event.Skip();

    if (IsShown() && m_child)
    {
        wxPoint pos = ScreenToClient(wxGetMousePosition());
        wxRect rect(GetSize());

        if ( rect.Inside(pos) )
        {
            if ( m_child->HasCapture() )
            {
                m_child->ReleaseMouse();
            }
        }
        else
        {
            if ( !m_child->HasCapture() )
            {
                m_child->CaptureMouse();
            }
        }                
    }
}
#endif


#if wxUSE_COMBOBOX && defined(__WXUNIVERSAL__)

// ----------------------------------------------------------------------------
// wxPopupComboWindow
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxPopupComboWindow, wxPopupTransientWindow)
    EVT_KEY_DOWN(wxPopupComboWindow::OnKeyDown)
END_EVENT_TABLE()

wxPopupComboWindow::wxPopupComboWindow(wxComboControl *parent)
                  : wxPopupTransientWindow(parent)
{
    m_combo = parent;
}

bool wxPopupComboWindow::Create(wxComboControl *parent)
{
    m_combo = parent;

    return wxPopupWindow::Create(parent);
}

void wxPopupComboWindow::PositionNearCombo()
{
    // the origin point must be in screen coords
    wxPoint ptOrigin = m_combo->ClientToScreen(wxPoint(0,0));

#if 0 //def __WXUNIVERSAL__
    // account for the fact that (0, 0) is not the top left corner of the
    // window: there is also the border
    wxRect rectBorders = m_combo->GetRenderer()->
                            GetBorderDimensions(m_combo->GetBorder());
    ptOrigin.x -= rectBorders.x;
    ptOrigin.y -= rectBorders.y;
#endif // __WXUNIVERSAL__

    // position below or above the combobox: the width is 0 to put it exactly
    // below us, not to the left or to the right
    Position(ptOrigin, wxSize(0, m_combo->GetSize().y));
}

void wxPopupComboWindow::OnDismiss()
{
    m_combo->OnDismiss();
}

void wxPopupComboWindow::OnKeyDown(wxKeyEvent& event)
{
    m_combo->ProcessEvent(event);
}

#endif // wxUSE_COMBOBOX && defined(__WXUNIVERSAL__)

// ----------------------------------------------------------------------------
// wxPopupWindowHandler
// ----------------------------------------------------------------------------
wxPopupWindowHandler::~wxPopupWindowHandler()
{
    if (m_popup && (m_popup->m_handlerPopup == this))
        m_popup->m_handlerPopup = NULL;
}

void wxPopupWindowHandler::OnLeftDown(wxMouseEvent& event)
{
    // let the window have it first (we're the first event handler in the chain
    // of handlers for this window)
    if ( m_popup->ProcessLeftDown(event) )
    {
        return;
    }

    wxPoint pos = event.GetPosition();

    // scrollbar on which the click occurred
    wxWindow *sbar = NULL;

    wxWindow *win = (wxWindow *)event.GetEventObject();

    switch ( win->HitTest(pos.x, pos.y) )
    {
        case wxHT_WINDOW_OUTSIDE:
            {
                // do the coords translation now as after DismissAndNotify()
                // m_popup may be destroyed
                wxMouseEvent event2(event);

                m_popup->ClientToScreen(&event2.m_x, &event2.m_y);

                // clicking outside a popup dismisses it
                m_popup->DismissAndNotify();

                // dismissing a tooltip shouldn't waste a click, i.e. you
                // should be able to dismiss it and press the button with the
                // same click, so repost this event to the window beneath us
                wxWindow *win = wxFindWindowAtPoint(event2.GetPosition());
                if ( win )
                {
                    // translate the event coords to the ones of the window
                    // which is going to get the event
                    win->ScreenToClient(&event2.m_x, &event2.m_y);

                    event2.SetEventObject(win);
                    wxPostEvent(win, event2);
                }
            }
            break;

#ifdef __WXUNIVERSAL__
        case wxHT_WINDOW_HORZ_SCROLLBAR:
            sbar = win->GetScrollbar(wxHORIZONTAL);
            break;

        case wxHT_WINDOW_VERT_SCROLLBAR:
            sbar = win->GetScrollbar(wxVERTICAL);
            break;
#endif

        default:
            // forgot to update the switch after adding a new hit test code?
            wxFAIL_MSG( _T("unexpected HitTest() return value") );
            // fall through

        case wxHT_WINDOW_CORNER:
            // don't actually know if this one is good for anything, but let it
            // pass just in case

        case wxHT_WINDOW_INSIDE:
            // let the normal processing take place
            event.Skip();
            break;
    }

    if ( sbar )
    {
        // translate the event coordinates to the scrollbar ones
        pos = sbar->ScreenToClient(win->ClientToScreen(pos));

        // and give the event to it
        wxMouseEvent event2 = event;
        event2.m_x = pos.x;
        event2.m_y = pos.y;

        (void)sbar->GetEventHandler()->ProcessEvent(event2);
    }
}

// ----------------------------------------------------------------------------
// wxPopupFocusHandler
// ----------------------------------------------------------------------------
wxPopupFocusHandler::~wxPopupFocusHandler()
{
    if (m_popup && (m_popup->m_handlerFocus == this))
        m_popup->m_handlerFocus = NULL;
}

void wxPopupFocusHandler::OnKillFocus(wxFocusEvent& event)
{
    // when we lose focus we always disappear - unless it goes to the popup (in
    // which case we don't really lose it)
    wxWindow *win = event.GetWindow();
    while ( win )
    {
        if ( win == m_popup )
            return;
        win = win->GetParent();
    }

    m_popup->DismissAndNotify();
}

void wxPopupFocusHandler::OnKeyDown(wxKeyEvent& event)
{
    // let the window have it first, it might process the keys
    if ( !m_popup->ProcessEvent(event) )
    {
        // by default, dismiss the popup
        m_popup->DismissAndNotify();
    }
}

#endif // wxUSE_POPUPWIN

