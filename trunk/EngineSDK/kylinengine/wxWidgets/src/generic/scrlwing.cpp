/////////////////////////////////////////////////////////////////////////////
// Name:        generic/scrolwin.cpp
// Purpose:     wxGenericScrolledWindow implementation
// Author:      Julian Smart
// Modified by: Vadim Zeitlin on 31.08.00: wxScrollHelper allows to implement.
//              Ron Lee on 10.4.02:  virtual size / auto scrollbars et al.
// Created:     01/02/97
// RCS-ID:      $Id: scrlwing.cpp,v 1.69 2005/04/29 09:14:23 JS Exp $
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "genscrolwin.h"
#endif

#ifdef __VMS
#define XtDisplay XTDISPLAY
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if !defined(__WXGTK__) || defined(__WXUNIVERSAL__)

#include "wx/utils.h"
#include "wx/dcclient.h"

#include "wx/scrolwin.h"
#include "wx/panel.h"
#if wxUSE_TIMER
#include "wx/timer.h"
#endif
#include "wx/sizer.h"
#include "wx/recguard.h"

#ifdef __WXMSW__
    #include <windows.h> // for DLGC_WANTARROWS
    #include "wx/msw/winundef.h"
#endif

#ifdef __WXMOTIF__
// For wxRETAINED implementation
#ifdef __VMS__ //VMS's Xm.h is not (yet) compatible with C++
               //This code switches off the compiler warnings
# pragma message disable nosimpint
#endif
#include <Xm/Xm.h>
#ifdef __VMS__
# pragma message enable nosimpint
#endif
#endif

IMPLEMENT_CLASS(wxScrolledWindow, wxGenericScrolledWindow)

/*
    TODO PROPERTIES
        style wxHSCROLL | wxVSCROLL
*/

// ----------------------------------------------------------------------------
// wxScrollHelperEvtHandler: intercept the events from the window and forward
// them to wxScrollHelper
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxScrollHelperEvtHandler : public wxEvtHandler
{
public:
    wxScrollHelperEvtHandler(wxScrollHelper *scrollHelper)
    {
        m_scrollHelper = scrollHelper;
    }

    virtual bool ProcessEvent(wxEvent& event);

    void ResetDrawnFlag() { m_hasDrawnWindow = false; }

private:
    wxScrollHelper *m_scrollHelper;

    bool m_hasDrawnWindow;

    DECLARE_NO_COPY_CLASS(wxScrollHelperEvtHandler)
};

#if wxUSE_TIMER
// ----------------------------------------------------------------------------
// wxAutoScrollTimer: the timer used to generate a stream of scroll events when
// a captured mouse is held outside the window
// ----------------------------------------------------------------------------

class wxAutoScrollTimer : public wxTimer
{
public:
    wxAutoScrollTimer(wxWindow *winToScroll, wxScrollHelper *scroll,
                      wxEventType eventTypeToSend,
                      int pos, int orient);

    virtual void Notify();

private:
    wxWindow *m_win;
    wxScrollHelper *m_scrollHelper;
    wxEventType m_eventType;
    int m_pos,
        m_orient;

    DECLARE_NO_COPY_CLASS(wxAutoScrollTimer)
};

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxAutoScrollTimer
// ----------------------------------------------------------------------------

wxAutoScrollTimer::wxAutoScrollTimer(wxWindow *winToScroll,
                                     wxScrollHelper *scroll,
                                     wxEventType eventTypeToSend,
                                     int pos, int orient)
{
    m_win = winToScroll;
    m_scrollHelper = scroll;
    m_eventType = eventTypeToSend;
    m_pos = pos;
    m_orient = orient;
}

void wxAutoScrollTimer::Notify()
{
    // only do all this as long as the window is capturing the mouse
    if ( wxWindow::GetCapture() != m_win )
    {
        Stop();
    }
    else // we still capture the mouse, continue generating events
    {
        // first scroll the window if we are allowed to do it
        wxScrollWinEvent event1(m_eventType, m_pos, m_orient);
        event1.SetEventObject(m_win);
        if ( m_scrollHelper->SendAutoScrollEvents(event1) &&
                m_win->GetEventHandler()->ProcessEvent(event1) )
        {
            // and then send a pseudo mouse-move event to refresh the selection
            wxMouseEvent event2(wxEVT_MOTION);
            wxGetMousePosition(&event2.m_x, &event2.m_y);

            // the mouse event coordinates should be client, not screen as
            // returned by wxGetMousePosition
            wxWindow *parentTop = m_win;
            while ( parentTop->GetParent() )
                parentTop = parentTop->GetParent();
            wxPoint ptOrig = parentTop->GetPosition();
            event2.m_x -= ptOrig.x;
            event2.m_y -= ptOrig.y;

            event2.SetEventObject(m_win);

            // FIXME: we don't fill in the other members - ok?

            m_win->GetEventHandler()->ProcessEvent(event2);
        }
        else // can't scroll further, stop
        {
            Stop();
        }
    }
}
#endif

// ----------------------------------------------------------------------------
// wxScrollHelperEvtHandler
// ----------------------------------------------------------------------------

bool wxScrollHelperEvtHandler::ProcessEvent(wxEvent& event)
{
    wxEventType evType = event.GetEventType();

    // the explanation of wxEVT_PAINT processing hack: for historic reasons
    // there are 2 ways to process this event in classes deriving from
    // wxScrolledWindow. The user code may
    //
    //  1. override wxScrolledWindow::OnDraw(dc)
    //  2. define its own OnPaint() handler
    //
    // In addition, in wxUniversal wxWindow defines OnPaint() itself and
    // always processes the draw event, so we can't just try the window
    // OnPaint() first and call our HandleOnPaint() if it doesn't process it
    // (the latter would never be called in wxUniversal).
    //
    // So the solution is to have a flag telling us whether the user code drew
    // anything in the window. We set it to true here but reset it to false in
    // wxScrolledWindow::OnPaint() handler (which wouldn't be called if the
    // user code defined OnPaint() in the derived class)
    m_hasDrawnWindow = true;

    // pass it on to the real handler
    bool processed = wxEvtHandler::ProcessEvent(event);

    // always process the size events ourselves, even if the user code handles
    // them as well, as we need to AdjustScrollbars()
    //
    // NB: it is important to do it after processing the event in the normal
    //     way as HandleOnSize() may generate a wxEVT_SIZE itself if the
    //     scrollbar[s] (dis)appear and it should be seen by the user code
    //     after this one
    if ( evType == wxEVT_SIZE )
    {
        m_scrollHelper->HandleOnSize((wxSizeEvent &)event);

        return true;
    }

    if ( processed )
    {
        // normally, nothing more to do here - except if it was a paint event
        // which wasn't really processed, then we'll try to call our
        // OnDraw() below (from HandleOnPaint)
        if ( m_hasDrawnWindow )
        {
            return true;
        }
    }

    // reset the skipped flag to false as it might have been set to true in
    // ProcessEvent() above
    event.Skip(false);

    if ( evType == wxEVT_PAINT )
    {
        m_scrollHelper->HandleOnPaint((wxPaintEvent &)event);
        return true;
    }

    if ( evType == wxEVT_SCROLLWIN_TOP ||
         evType == wxEVT_SCROLLWIN_BOTTOM ||
         evType == wxEVT_SCROLLWIN_LINEUP ||
         evType == wxEVT_SCROLLWIN_LINEDOWN ||
         evType == wxEVT_SCROLLWIN_PAGEUP ||
         evType == wxEVT_SCROLLWIN_PAGEDOWN ||
         evType == wxEVT_SCROLLWIN_THUMBTRACK ||
         evType == wxEVT_SCROLLWIN_THUMBRELEASE )
    {
            m_scrollHelper->HandleOnScroll((wxScrollWinEvent &)event);
            return !event.GetSkipped();
    }

    if ( evType == wxEVT_ENTER_WINDOW )
    {
        m_scrollHelper->HandleOnMouseEnter((wxMouseEvent &)event);
    }
    else if ( evType == wxEVT_LEAVE_WINDOW )
    {
        m_scrollHelper->HandleOnMouseLeave((wxMouseEvent &)event);
    }
#if wxUSE_MOUSEWHEEL
    else if ( evType == wxEVT_MOUSEWHEEL )
    {
        m_scrollHelper->HandleOnMouseWheel((wxMouseEvent &)event);
    }
#endif // wxUSE_MOUSEWHEEL
    else if ( evType == wxEVT_CHAR )
    {
        m_scrollHelper->HandleOnChar((wxKeyEvent &)event);
        return !event.GetSkipped();
    }

    return false;
}

// ----------------------------------------------------------------------------
// wxScrollHelper construction
// ----------------------------------------------------------------------------

wxScrollHelper::wxScrollHelper(wxWindow *win)
{
    m_xScrollPixelsPerLine =
    m_yScrollPixelsPerLine =
    m_xScrollPosition =
    m_yScrollPosition =
    m_xScrollLines =
    m_yScrollLines =
    m_xScrollLinesPerPage =
    m_yScrollLinesPerPage = 0;

    m_xScrollingEnabled =
    m_yScrollingEnabled = true;

    m_scaleX =
    m_scaleY = 1.0;
#if wxUSE_MOUSEWHEEL
    m_wheelRotation = 0;
#endif

    m_win =
    m_targetWindow = (wxWindow *)NULL;

    m_timerAutoScroll = (wxTimer *)NULL;

    m_handler = NULL;

    if ( win )
        SetWindow(win);
}

wxScrollHelper::~wxScrollHelper()
{
    StopAutoScrolling();

    DeleteEvtHandler();
}

// ----------------------------------------------------------------------------
// setting scrolling parameters
// ----------------------------------------------------------------------------

void wxScrollHelper::SetScrollbars(int pixelsPerUnitX,
                                   int pixelsPerUnitY,
                                   int noUnitsX,
                                   int noUnitsY,
                                   int xPos,
                                   int yPos,
                                   bool noRefresh)
{
    int xpos, ypos;

    CalcUnscrolledPosition(xPos, yPos, &xpos, &ypos);
    bool do_refresh =
    (
      (noUnitsX != 0 && m_xScrollLines == 0) ||
      (noUnitsX < m_xScrollLines && xpos > pixelsPerUnitX * noUnitsX) ||

      (noUnitsY != 0 && m_yScrollLines == 0) ||
      (noUnitsY < m_yScrollLines && ypos > pixelsPerUnitY * noUnitsY) ||
      (xPos != m_xScrollPosition) ||
      (yPos != m_yScrollPosition)
    );

    m_xScrollPixelsPerLine = pixelsPerUnitX;
    m_yScrollPixelsPerLine = pixelsPerUnitY;
    m_xScrollPosition = xPos;
    m_yScrollPosition = yPos;

    int w = noUnitsX * pixelsPerUnitX;
    int h = noUnitsY * pixelsPerUnitY;

    // For better backward compatibility we set persisting limits
    // here not just the size.  It makes SetScrollbars 'sticky'
    // emulating the old non-autoscroll behaviour.
    //   m_targetWindow->SetVirtualSizeHints( w, h );

    // The above should arguably be deprecated, this however we still need.

    // take care not to set 0 virtual size, 0 means that we don't have any
    // scrollbars and hence we should use the real size instead of the virtual
    // one which is indicated by using wxDefaultCoord
    m_targetWindow->SetVirtualSize( w ? w : wxDefaultCoord,
                                    h ? h : wxDefaultCoord);

    if (do_refresh && !noRefresh)
        m_targetWindow->Refresh(true, GetScrollRect());

#ifndef __WXUNIVERSAL__
    // If the target is not the same as the window with the scrollbars,
    // then we need to update the scrollbars here, since they won't have
    // been updated by SetVirtualSize().
    if ( m_targetWindow != m_win )
#endif // !__WXUNIVERSAL__
    {
        AdjustScrollbars();
    }
#ifndef __WXUNIVERSAL__
    else
    {
        // otherwise this has been done by AdjustScrollbars, above
    }
#endif // !__WXUNIVERSAL__
}

// ----------------------------------------------------------------------------
// [target] window handling
// ----------------------------------------------------------------------------

void wxScrollHelper::DeleteEvtHandler()
{
    // search for m_handler in the handler list
    if ( m_win && m_handler )
    {
        if ( m_win->RemoveEventHandler(m_handler) )
        {
            delete m_handler;
        }
        //else: something is very wrong, so better [maybe] leak memory than
        //      risk a crash because of double deletion

        m_handler = NULL;
    }
}

void wxScrollHelper::SetWindow(wxWindow *win)
{
    wxCHECK_RET( win, _T("wxScrollHelper needs a window to scroll") );

    m_win = win;

    // by default, the associated window is also the target window
    DoSetTargetWindow(win);
}

void wxScrollHelper::DoSetTargetWindow(wxWindow *target)
{
    m_targetWindow = target;
#ifdef __WXMAC__
    target->MacSetClipChildren( true ) ;
#endif

    // install the event handler which will intercept the events we're
    // interested in (but only do it for our real window, not the target window
    // which we scroll - we don't need to hijack its events)
    if ( m_targetWindow == m_win )
    {
        // if we already have a handler, delete it first
        DeleteEvtHandler();

        m_handler = new wxScrollHelperEvtHandler(this);
        m_targetWindow->PushEventHandler(m_handler);
    }
}

void wxScrollHelper::SetTargetWindow(wxWindow *target)
{
    wxCHECK_RET( target, wxT("target window must not be NULL") );

    if ( target == m_targetWindow )
        return;

    DoSetTargetWindow(target);
}

wxWindow *wxScrollHelper::GetTargetWindow() const
{
    return m_targetWindow;
}

// ----------------------------------------------------------------------------
// scrolling implementation itself
// ----------------------------------------------------------------------------

void wxScrollHelper::HandleOnScroll(wxScrollWinEvent& event)
{
    int nScrollInc = CalcScrollInc(event);
    if ( nScrollInc == 0 )
    {
        // can't scroll further
        event.Skip();

        return;
    }

    int orient = event.GetOrientation();
    if (orient == wxHORIZONTAL)
    {
        m_xScrollPosition += nScrollInc;
        m_win->SetScrollPos(wxHORIZONTAL, m_xScrollPosition);
    }
    else
    {
        m_yScrollPosition += nScrollInc;
        m_win->SetScrollPos(wxVERTICAL, m_yScrollPosition);
    }

    bool needsRefresh = false;
    int dx = 0,
        dy = 0;
    if (orient == wxHORIZONTAL)
    {
       if ( m_xScrollingEnabled )
       {
           dx = -m_xScrollPixelsPerLine * nScrollInc;
       }
       else
       {
           needsRefresh = true;
       }
    }
    else
    {
        if ( m_yScrollingEnabled )
        {
            dy = -m_yScrollPixelsPerLine * nScrollInc;
        }
        else
        {
            needsRefresh = true;
        }
    }

    if ( needsRefresh )
    {
        m_targetWindow->Refresh(true, GetScrollRect());
    }
    else
    {
        m_targetWindow->ScrollWindow(dx, dy, GetScrollRect());
    }
}

int wxScrollHelper::CalcScrollInc(wxScrollWinEvent& event)
{
    int pos = event.GetPosition();
    int orient = event.GetOrientation();

    int nScrollInc = 0;
    if (event.GetEventType() == wxEVT_SCROLLWIN_TOP)
    {
            if (orient == wxHORIZONTAL)
                nScrollInc = - m_xScrollPosition;
            else
                nScrollInc = - m_yScrollPosition;
    } else
    if (event.GetEventType() == wxEVT_SCROLLWIN_BOTTOM)
    {
            if (orient == wxHORIZONTAL)
                nScrollInc = m_xScrollLines - m_xScrollPosition;
            else
                nScrollInc = m_yScrollLines - m_yScrollPosition;
    } else
    if (event.GetEventType() == wxEVT_SCROLLWIN_LINEUP)
    {
            nScrollInc = -1;
    } else
    if (event.GetEventType() == wxEVT_SCROLLWIN_LINEDOWN)
    {
            nScrollInc = 1;
    } else
    if (event.GetEventType() == wxEVT_SCROLLWIN_PAGEUP)
    {
            if (orient == wxHORIZONTAL)
                nScrollInc = -GetScrollPageSize(wxHORIZONTAL);
            else
                nScrollInc = -GetScrollPageSize(wxVERTICAL);
    } else
    if (event.GetEventType() == wxEVT_SCROLLWIN_PAGEDOWN)
    {
            if (orient == wxHORIZONTAL)
                nScrollInc = GetScrollPageSize(wxHORIZONTAL);
            else
                nScrollInc = GetScrollPageSize(wxVERTICAL);
    } else
    if ((event.GetEventType() == wxEVT_SCROLLWIN_THUMBTRACK) ||
        (event.GetEventType() == wxEVT_SCROLLWIN_THUMBRELEASE))
    {
            if (orient == wxHORIZONTAL)
                nScrollInc = pos - m_xScrollPosition;
            else
                nScrollInc = pos - m_yScrollPosition;
    }

    if (orient == wxHORIZONTAL)
    {
        if (m_xScrollPixelsPerLine > 0)
        {
            if ( m_xScrollPosition + nScrollInc < 0 )
            {
                // As -ve as we can go
                nScrollInc = -m_xScrollPosition;
            }
            else // check for the other bound
            {
                const int posMax = m_xScrollLines - m_xScrollLinesPerPage;
                if ( m_xScrollPosition + nScrollInc > posMax )
                {
                    // As +ve as we can go
                    nScrollInc = posMax - m_xScrollPosition;
                }
            }
        }
        else
            m_targetWindow->Refresh(true, GetScrollRect());
    }
    else
    {
        if ( m_yScrollPixelsPerLine > 0 )
        {
            if ( m_yScrollPosition + nScrollInc < 0 )
            {
                // As -ve as we can go
                nScrollInc = -m_yScrollPosition;
            }
            else // check for the other bound
            {
                const int posMax = m_yScrollLines - m_yScrollLinesPerPage;
                if ( m_yScrollPosition + nScrollInc > posMax )
                {
                    // As +ve as we can go
                    nScrollInc = posMax - m_yScrollPosition;
                }
            }
        }
        else
        {
            // VZ: why do we do this? (FIXME)
            m_targetWindow->Refresh(true, GetScrollRect());
        }
    }

    return nScrollInc;
}

// Adjust the scrollbars - new version.
void wxScrollHelper::AdjustScrollbars()
{
    static wxRecursionGuardFlag s_flagReentrancy;
    wxRecursionGuard guard(s_flagReentrancy);
    if ( guard.IsInside() )
    {
        // don't reenter AdjustScrollbars() while another call to
        // AdjustScrollbars() is in progress because this may lead to calling
        // ScrollWindow() twice and this can really happen under MSW if
        // SetScrollbar() call below adds or removes the scrollbar which
        // changes the window size and hence results in another
        // AdjustScrollbars() call
        return;
    }

    int w = 0, h = 0;
    int oldw, oldh;

    int oldXScroll = m_xScrollPosition;
    int oldYScroll = m_yScrollPosition;

    // VZ: at least under Windows this loop is useless because when scrollbars
    //     [dis]appear we get a WM_SIZE resulting in another call to
    //     AdjustScrollbars() anyhow. As it doesn't seem to do any harm I leave
    //     it here for now but it would be better to ensure that all ports
    //     generate EVT_SIZE when scrollbars [dis]appear, emulating it if
    //     necessary, and remove it later
    // JACS: Stop potential infinite loop by limiting number of iterations
    int iterationCount = 0;
    const int iterationMax = 5;
    do
    {
        iterationCount ++;
        
        GetTargetSize(&w, 0);

        // scroll lines per page: if 0, no scrolling is needed
        int linesPerPage;

        if ( m_xScrollPixelsPerLine == 0 )
        {
            // scrolling is disabled
            m_xScrollLines = 0;
            m_xScrollPosition = 0;
            linesPerPage = 0;
        }
        else // might need scrolling
        {
            // Round up integer division to catch any "leftover" client space.
            const int wVirt = m_targetWindow->GetVirtualSize().GetWidth();
            m_xScrollLines = (wVirt + m_xScrollPixelsPerLine - 1) / m_xScrollPixelsPerLine;

            // Calculate page size i.e. number of scroll units you get on the
            // current client window.
            linesPerPage = w / m_xScrollPixelsPerLine;

            // Special case. When client and virtual size are very close but
            // the client is big enough, kill scrollbar.
            if ((linesPerPage < m_xScrollLines) && (w >= wVirt)) ++linesPerPage;

            if (linesPerPage >= m_xScrollLines)
            {
                // we're big enough to not need scrolling
                linesPerPage =
                m_xScrollLines =
                m_xScrollPosition = 0;
            }
            else // we do need a scrollbar
            {
                if ( linesPerPage < 1 )
                    linesPerPage = 1;

                // Correct position if greater than extent of canvas minus
                // the visible portion of it or if below zero
                const int posMax = m_xScrollLines - linesPerPage;
                if ( m_xScrollPosition > posMax )
                    m_xScrollPosition = posMax;
                else if ( m_xScrollPosition < 0 )
                    m_xScrollPosition = 0;
            }
        }

        m_win->SetScrollbar(wxHORIZONTAL, m_xScrollPosition,
                            linesPerPage, m_xScrollLines);

        // The amount by which we scroll when paging
        SetScrollPageSize(wxHORIZONTAL, linesPerPage);

        GetTargetSize(0, &h);

        if ( m_yScrollPixelsPerLine == 0 )
        {
            // scrolling is disabled
            m_yScrollLines = 0;
            m_yScrollPosition = 0;
            linesPerPage = 0;
        }
        else // might need scrolling
        {
            // Round up integer division to catch any "leftover" client space.
            const int hVirt = m_targetWindow->GetVirtualSize().GetHeight();
            m_yScrollLines = ( hVirt + m_yScrollPixelsPerLine - 1 ) / m_yScrollPixelsPerLine;

            // Calculate page size i.e. number of scroll units you get on the
            // current client window.
            linesPerPage = h / m_yScrollPixelsPerLine;

            // Special case. When client and virtual size are very close but
            // the client is big enough, kill scrollbar.
            if ((linesPerPage < m_yScrollLines) && (h >= hVirt)) ++linesPerPage;

            if (linesPerPage >= m_yScrollLines)
            {
                // we're big enough to not need scrolling
                linesPerPage =
                m_yScrollLines =
                m_yScrollPosition = 0;
            }
            else // we do need a scrollbar
            {
                if ( linesPerPage < 1 )
                    linesPerPage = 1;

                // Correct position if greater than extent of canvas minus
                // the visible portion of it or if below zero
                const int posMax = m_yScrollLines - linesPerPage;
                if ( m_yScrollPosition > posMax )
                    m_yScrollPosition = posMax;
                else if ( m_yScrollPosition < 0 )
                    m_yScrollPosition = 0;
            }
        }

        m_win->SetScrollbar(wxVERTICAL, m_yScrollPosition,
                            linesPerPage, m_yScrollLines);

        // The amount by which we scroll when paging
        SetScrollPageSize(wxVERTICAL, linesPerPage);


        // If a scrollbar (dis)appeared as a result of this, adjust them again.
        oldw = w;
        oldh = h;

        GetTargetSize( &w, &h );
    } while ( (w != oldw || h != oldh) && (iterationCount < iterationMax) );

#ifdef __WXMOTIF__
    // Sorry, some Motif-specific code to implement a backing pixmap
    // for the wxRETAINED style. Implementing a backing store can't
    // be entirely generic because it relies on the wxWindowDC implementation
    // to duplicate X drawing calls for the backing pixmap.

    if ( m_targetWindow->GetWindowStyle() & wxRETAINED )
    {
        Display* dpy = XtDisplay((Widget)m_targetWindow->GetMainWidget());

        int totalPixelWidth = m_xScrollLines * m_xScrollPixelsPerLine;
        int totalPixelHeight = m_yScrollLines * m_yScrollPixelsPerLine;
        if (m_targetWindow->GetBackingPixmap() &&
           !((m_targetWindow->GetPixmapWidth() == totalPixelWidth) &&
             (m_targetWindow->GetPixmapHeight() == totalPixelHeight)))
        {
            XFreePixmap (dpy, (Pixmap) m_targetWindow->GetBackingPixmap());
            m_targetWindow->SetBackingPixmap((WXPixmap) 0);
        }

        if (!m_targetWindow->GetBackingPixmap() &&
           (m_xScrollLines != 0) && (m_yScrollLines != 0))
        {
            int depth = wxDisplayDepth();
            m_targetWindow->SetPixmapWidth(totalPixelWidth);
            m_targetWindow->SetPixmapHeight(totalPixelHeight);
            m_targetWindow->SetBackingPixmap((WXPixmap) XCreatePixmap (dpy, RootWindow (dpy, DefaultScreen (dpy)),
              m_targetWindow->GetPixmapWidth(), m_targetWindow->GetPixmapHeight(), depth));
        }

    }
#endif // Motif

    if (oldXScroll != m_xScrollPosition)
    {
       if (m_xScrollingEnabled)
            m_targetWindow->ScrollWindow( m_xScrollPixelsPerLine * (oldXScroll - m_xScrollPosition), 0,
                                          GetScrollRect() );
       else
            m_targetWindow->Refresh(true, GetScrollRect());
    }

    if (oldYScroll != m_yScrollPosition)
    {
        if (m_yScrollingEnabled)
            m_targetWindow->ScrollWindow( 0, m_yScrollPixelsPerLine * (oldYScroll-m_yScrollPosition),
                                          GetScrollRect() );
        else
            m_targetWindow->Refresh(true, GetScrollRect());
    }
}

void wxScrollHelper::DoPrepareDC(wxDC& dc)
{
    wxPoint pt = dc.GetDeviceOrigin();
    dc.SetDeviceOrigin( pt.x - m_xScrollPosition * m_xScrollPixelsPerLine,
                        pt.y - m_yScrollPosition * m_yScrollPixelsPerLine );
    dc.SetUserScale( m_scaleX, m_scaleY );
}

void wxScrollHelper::SetScrollRate( int xstep, int ystep )
{
    int old_x = m_xScrollPixelsPerLine * m_xScrollPosition;
    int old_y = m_yScrollPixelsPerLine * m_yScrollPosition;

    m_xScrollPixelsPerLine = xstep;
    m_yScrollPixelsPerLine = ystep;

    int new_x = m_xScrollPixelsPerLine * m_xScrollPosition;
    int new_y = m_yScrollPixelsPerLine * m_yScrollPosition;

    m_win->SetScrollPos( wxHORIZONTAL, m_xScrollPosition );
    m_win->SetScrollPos( wxVERTICAL, m_yScrollPosition );
    m_targetWindow->ScrollWindow( old_x - new_x, old_y - new_y );

    AdjustScrollbars();
}

void wxScrollHelper::GetScrollPixelsPerUnit (int *x_unit, int *y_unit) const
{
    if ( x_unit )
        *x_unit = m_xScrollPixelsPerLine;
    if ( y_unit )
        *y_unit = m_yScrollPixelsPerLine;
}

int wxScrollHelper::GetScrollPageSize(int orient) const
{
    if ( orient == wxHORIZONTAL )
        return m_xScrollLinesPerPage;
    else
        return m_yScrollLinesPerPage;
}

void wxScrollHelper::SetScrollPageSize(int orient, int pageSize)
{
    if ( orient == wxHORIZONTAL )
        m_xScrollLinesPerPage = pageSize;
    else
        m_yScrollLinesPerPage = pageSize;
}

/*
 * Scroll to given position (scroll position, not pixel position)
 */
void wxScrollHelper::Scroll( int x_pos, int y_pos )
{
    if (!m_targetWindow)
        return;

    if (((x_pos == -1) || (x_pos == m_xScrollPosition)) &&
        ((y_pos == -1) || (y_pos == m_yScrollPosition))) return;

    int w, h;
    GetTargetSize(&w, &h);

    if ((x_pos != -1) && (m_xScrollPixelsPerLine))
    {
        int old_x = m_xScrollPosition;
        m_xScrollPosition = x_pos;

        // Calculate page size i.e. number of scroll units you get on the
        // current client window
        int noPagePositions = (int) ( (w/(double)m_xScrollPixelsPerLine) + 0.5 );
        if (noPagePositions < 1) noPagePositions = 1;

        // Correct position if greater than extent of canvas minus
        // the visible portion of it or if below zero
        m_xScrollPosition = wxMin( m_xScrollLines-noPagePositions, m_xScrollPosition );
        m_xScrollPosition = wxMax( 0, m_xScrollPosition );

        if (old_x != m_xScrollPosition) {
            m_win->SetScrollPos( wxHORIZONTAL, m_xScrollPosition );
            m_targetWindow->ScrollWindow( (old_x-m_xScrollPosition)*m_xScrollPixelsPerLine, 0,
                                          GetScrollRect() );
        }
    }
    if ((y_pos != -1) && (m_yScrollPixelsPerLine))
    {
        int old_y = m_yScrollPosition;
        m_yScrollPosition = y_pos;

        // Calculate page size i.e. number of scroll units you get on the
        // current client window
        int noPagePositions = (int) ( (h/(double)m_yScrollPixelsPerLine) + 0.5 );
        if (noPagePositions < 1) noPagePositions = 1;

        // Correct position if greater than extent of canvas minus
        // the visible portion of it or if below zero
        m_yScrollPosition = wxMin( m_yScrollLines-noPagePositions, m_yScrollPosition );
        m_yScrollPosition = wxMax( 0, m_yScrollPosition );

        if (old_y != m_yScrollPosition) {
            m_win->SetScrollPos( wxVERTICAL, m_yScrollPosition );
            m_targetWindow->ScrollWindow( 0, (old_y-m_yScrollPosition)*m_yScrollPixelsPerLine,
                                          GetScrollRect() );
        }
    }
}

void wxScrollHelper::EnableScrolling (bool x_scroll, bool y_scroll)
{
    m_xScrollingEnabled = x_scroll;
    m_yScrollingEnabled = y_scroll;
}

// Where the current view starts from
void wxScrollHelper::GetViewStart (int *x, int *y) const
{
    if ( x )
        *x = m_xScrollPosition;
    if ( y )
        *y = m_yScrollPosition;
}

#if WXWIN_COMPATIBILITY_2_2

void wxScrollHelper::ViewStart(int *x, int *y) const
{
    GetViewStart( x, y );
}

#endif // WXWIN_COMPATIBILITY_2_2

void wxScrollHelper::DoCalcScrolledPosition(int x, int y, int *xx, int *yy) const
{
    if ( xx )
        *xx = x - m_xScrollPosition * m_xScrollPixelsPerLine;
    if ( yy )
        *yy = y - m_yScrollPosition * m_yScrollPixelsPerLine;
}

void wxScrollHelper::DoCalcUnscrolledPosition(int x, int y, int *xx, int *yy) const
{
    if ( xx )
        *xx = x + m_xScrollPosition * m_xScrollPixelsPerLine;
    if ( yy )
        *yy = y + m_yScrollPosition * m_yScrollPixelsPerLine;
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

// Default OnSize resets scrollbars, if any
void wxScrollHelper::HandleOnSize(wxSizeEvent& WXUNUSED(event))
{
    if ( m_targetWindow->GetAutoLayout() )
    {
        wxSize size = m_targetWindow->GetBestVirtualSize();
        
        // This will call ::Layout() and ::AdjustScrollbars()
        m_win->SetVirtualSize( size );
    }
    else
    {
        AdjustScrollbars();
    }
}

// This calls OnDraw, having adjusted the origin according to the current
// scroll position
void wxScrollHelper::HandleOnPaint(wxPaintEvent& WXUNUSED(event))
{
    // don't use m_targetWindow here, this is always called for ourselves
    wxPaintDC dc(m_win);
    DoPrepareDC(dc);

    OnDraw(dc);
}

// kbd handling: notice that we use OnChar() and not OnKeyDown() for
// compatibility here - if we used OnKeyDown(), the programs which process
// arrows themselves in their OnChar() would never get the message and like
// this they always have the priority
void wxScrollHelper::HandleOnChar(wxKeyEvent& event)
{
    int stx, sty,       // view origin
        szx, szy,       // view size (total)
        clix, cliy;     // view size (on screen)

    GetViewStart(&stx, &sty);
    GetTargetSize(&clix, &cliy);
    m_targetWindow->GetVirtualSize(&szx, &szy);

    if( m_xScrollPixelsPerLine )
    {
        clix /= m_xScrollPixelsPerLine;
        szx /= m_xScrollPixelsPerLine;
    }
    else
    {
        clix = 0;
        szx = -1;
    }
    if( m_yScrollPixelsPerLine )
    {
        cliy /= m_yScrollPixelsPerLine;
        szy /= m_yScrollPixelsPerLine;
    }
    else
    {
        cliy = 0;
        szy = -1;
    }

    int xScrollOld = m_xScrollPosition,
        yScrollOld = m_yScrollPosition;

    int dsty;
    switch ( event.GetKeyCode() )
    {
        case WXK_PAGEUP:
        case WXK_PRIOR:
            dsty = sty - (5 * cliy / 6);
            Scroll(-1, (dsty == -1) ? 0 : dsty);
            break;

        case WXK_PAGEDOWN:
        case WXK_NEXT:
            Scroll(-1, sty + (5 * cliy / 6));
            break;

        case WXK_HOME:
            Scroll(0, event.ControlDown() ? 0 : -1);
            break;

        case WXK_END:
            Scroll(szx - clix, event.ControlDown() ? szy - cliy : -1);
            break;

        case WXK_UP:
            Scroll(-1, sty - 1);
            break;

        case WXK_DOWN:
            Scroll(-1, sty + 1);
            break;

        case WXK_LEFT:
            Scroll(stx - 1, -1);
            break;

        case WXK_RIGHT:
            Scroll(stx + 1, -1);
            break;

        default:
            // not for us
            event.Skip();
    }

    if ( m_xScrollPosition != xScrollOld )
    {
        wxScrollWinEvent event(wxEVT_SCROLLWIN_THUMBTRACK, m_xScrollPosition,
                               wxHORIZONTAL);
        event.SetEventObject(m_win);
        m_win->GetEventHandler()->ProcessEvent(event);
    }

    if ( m_yScrollPosition != yScrollOld )
    {
        wxScrollWinEvent event(wxEVT_SCROLLWIN_THUMBTRACK, m_yScrollPosition,
                               wxVERTICAL);
        event.SetEventObject(m_win);
        m_win->GetEventHandler()->ProcessEvent(event);
    }
}

// ----------------------------------------------------------------------------
// autoscroll stuff: these functions deal with sending fake scroll events when
// a captured mouse is being held outside the window
// ----------------------------------------------------------------------------

bool wxScrollHelper::SendAutoScrollEvents(wxScrollWinEvent& event) const
{
    // only send the event if the window is scrollable in this direction
    wxWindow *win = (wxWindow *)event.GetEventObject();
    return win->HasScrollbar(event.GetOrientation());
}

void wxScrollHelper::StopAutoScrolling()
{
#if wxUSE_TIMER
    if ( m_timerAutoScroll )
    {
        delete m_timerAutoScroll;
        m_timerAutoScroll = (wxTimer *)NULL;
    }
#endif
}

void wxScrollHelper::HandleOnMouseEnter(wxMouseEvent& event)
{
    StopAutoScrolling();

    event.Skip();
}

void wxScrollHelper::HandleOnMouseLeave(wxMouseEvent& event)
{
    // don't prevent the usual processing of the event from taking place
    event.Skip();

    // when a captured mouse leave a scrolled window we start generate
    // scrolling events to allow, for example, extending selection beyond the
    // visible area in some controls
    if ( wxWindow::GetCapture() == m_targetWindow )
    {
        // where is the mouse leaving?
        int pos, orient;
        wxPoint pt = event.GetPosition();
        if ( pt.x < 0 )
        {
            orient = wxHORIZONTAL;
            pos = 0;
        }
        else if ( pt.y < 0 )
        {
            orient = wxVERTICAL;
            pos = 0;
        }
        else // we're lower or to the right of the window
        {
            wxSize size = m_targetWindow->GetClientSize();
            if ( pt.x > size.x )
            {
                orient = wxHORIZONTAL;
                pos = m_xScrollLines;
            }
            else if ( pt.y > size.y )
            {
                orient = wxVERTICAL;
                pos = m_yScrollLines;
            }
            else // this should be impossible
            {
                // but seems to happen sometimes under wxMSW - maybe it's a bug
                // there but for now just ignore it

                //wxFAIL_MSG( _T("can't understand where has mouse gone") );

                return;
            }
        }

        // only start the auto scroll timer if the window can be scrolled in
        // this direction
        if ( !m_targetWindow->HasScrollbar(orient) )
            return;

#if wxUSE_TIMER
        delete m_timerAutoScroll;
        m_timerAutoScroll = new wxAutoScrollTimer
                                (
                                    m_targetWindow, this,
                                    pos == 0 ? wxEVT_SCROLLWIN_LINEUP
                                             : wxEVT_SCROLLWIN_LINEDOWN,
                                    pos,
                                    orient
                                );
        m_timerAutoScroll->Start(50); // FIXME: make configurable
#else
        wxUnusedVar(pos);
#endif
    }
}

#if wxUSE_MOUSEWHEEL

void wxScrollHelper::HandleOnMouseWheel(wxMouseEvent& event)
{
    m_wheelRotation += event.GetWheelRotation();
    int lines = m_wheelRotation / event.GetWheelDelta();
    m_wheelRotation -= lines * event.GetWheelDelta();

    if (lines != 0)
    {

        wxScrollWinEvent newEvent;

        newEvent.SetPosition(0);
        newEvent.SetOrientation(wxVERTICAL);
        newEvent.SetEventObject(m_win);

        if (event.IsPageScroll())
        {
            if (lines > 0)
                newEvent.SetEventType(wxEVT_SCROLLWIN_PAGEUP);
            else
                newEvent.SetEventType(wxEVT_SCROLLWIN_PAGEDOWN);

            m_win->GetEventHandler()->ProcessEvent(newEvent);
        }
        else
        {
            lines *= event.GetLinesPerAction();
            if (lines > 0)
                newEvent.SetEventType(wxEVT_SCROLLWIN_LINEUP);
            else
                newEvent.SetEventType(wxEVT_SCROLLWIN_LINEDOWN);

            int times = abs(lines);
            for (; times > 0; times--)
                m_win->GetEventHandler()->ProcessEvent(newEvent);
        }
    }
}

#endif // wxUSE_MOUSEWHEEL

// ----------------------------------------------------------------------------
// wxGenericScrolledWindow implementation
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxGenericScrolledWindow, wxPanel)

BEGIN_EVENT_TABLE(wxGenericScrolledWindow, wxPanel)
    EVT_PAINT(wxGenericScrolledWindow::OnPaint)
END_EVENT_TABLE()

bool wxGenericScrolledWindow::Create(wxWindow *parent,
                              wxWindowID id,
                              const wxPoint& pos,
                              const wxSize& size,
                              long style,
                              const wxString& name)
{
    m_targetWindow = this;
#ifdef __WXMAC__
    MacSetClipChildren( true ) ;
#endif

    bool ok = wxPanel::Create(parent, id, pos, size, style|wxHSCROLL|wxVSCROLL, name);

    return ok;
}

wxGenericScrolledWindow::~wxGenericScrolledWindow()
{
}

bool wxGenericScrolledWindow::Layout()
{
    if (GetSizer() && m_targetWindow == this)
    {
        // If we're the scroll target, take into account the
        // virtual size and scrolled position of the window.

        int x, y, w, h;
        CalcScrolledPosition(0,0, &x,&y);
        GetVirtualSize(&w, &h);
        GetSizer()->SetDimension(x, y, w, h);
        return true;
    }

    // fall back to default for LayoutConstraints
    return wxPanel::Layout();
}

void wxGenericScrolledWindow::DoSetVirtualSize(int x, int y)
{
    wxPanel::DoSetVirtualSize( x, y );
    AdjustScrollbars();

    if (GetAutoLayout())
        Layout();
}

// wxWindow's GetBestVirtualSize returns the actual window size,
// whereas we want to return the virtual size
wxSize wxGenericScrolledWindow::GetBestVirtualSize() const
{
    wxSize  clientSize( GetClientSize() );
    if (GetSizer())
    {
        wxSize minSize( GetSizer()->CalcMin() );

        return wxSize( wxMax( clientSize.x, minSize.x ), wxMax( clientSize.y, minSize.y ) );
    }
    else
        return clientSize;
}

// return the size best suited for the current window
// (this isn't a virtual size, this is a sensible size for the window)
wxSize wxGenericScrolledWindow::DoGetBestSize() const
{
    wxSize best;

    if ( GetSizer() )
    {
        wxSize b = GetSizer()->GetMinSize();

        // Only use the content to set the window size in the direction
        // where there's no scrolling; otherwise we're going to get a huge
        // window in the direction in which scrolling is enabled
        int ppuX, ppuY;
        GetScrollPixelsPerUnit(& ppuX, & ppuY);

        wxSize minSize;
        if ( GetMinSize().IsFullySpecified() )
            minSize = GetMinSize();
        else
            minSize = GetSize();

        if (ppuX > 0)
            b.x = minSize.x;
        if (ppuY > 0)
            b.y = minSize.y;
        best = b;
    }
    else
        return wxWindow::DoGetBestSize();

    // Add any difference between size and client size
    wxSize diff = GetSize() - GetClientSize();
    best.x += wxMax(0, diff.x);
    best.y += wxMax(0, diff.y);

    return best;
}

void wxGenericScrolledWindow::OnPaint(wxPaintEvent& event)
{
    // the user code didn't really draw the window if we got here, so set this
    // flag to try to call OnDraw() later
    m_handler->ResetDrawnFlag();

    event.Skip();
}

#ifdef __WXMSW__
WXLRESULT
wxGenericScrolledWindow::MSWWindowProc(WXUINT nMsg,
                                       WXWPARAM wParam,
                                       WXLPARAM lParam)
{
    WXLRESULT rc = wxPanel::MSWWindowProc(nMsg, wParam, lParam);

#ifndef __WXWINCE__
    // we need to process arrows ourselves for scrolling
    if ( nMsg == WM_GETDLGCODE )
    {
        rc |= DLGC_WANTARROWS;
    }
#endif

    return rc;
}

#endif // __WXMSW__

#endif // !wxGTK

// vi:sts=4:sw=4:et
