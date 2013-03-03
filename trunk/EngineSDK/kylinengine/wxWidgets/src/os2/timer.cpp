/////////////////////////////////////////////////////////////////////////////
// Name:        timer.cpp
// Purpose:     wxTimer implementation
// Author:      David Webster
// Modified by:
// Created:     10/17/99
// RCS-ID:      $Id: timer.cpp,v 1.17 2004/11/28 18:39:26 SN Exp $
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "timer.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/window.h"
#include "wx/os2/private.h"

#ifndef WX_PRECOMP
    #include "wx/setup.h"
    #include "wx/list.h"
    #include "wx/event.h"
    #include "wx/app.h"
#endif

#include "wx/intl.h"
#include "wx/log.h"

#include "wx/timer.h"

#include <time.h>
#include <sys/types.h>

#include <sys/timeb.h>

// ----------------------------------------------------------------------------
// private globals
// ----------------------------------------------------------------------------

// define a hash containing all the timers: it is indexed by timer id and
// contains the corresponding timer
WX_DECLARE_HASH_MAP(unsigned long, wxTimer *, wxIntegerHash, wxIntegerEqual,
                    wxTimerMap);

// instead of using a global here, wrap it in a static function as otherwise it
// could have been used before being initialized if a timer object were created
// globally
static wxTimerMap& TimerMap()
{
    static wxTimerMap s_timerMap;

    return s_timerMap;
}

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

// timer callback used for all timers
ULONG wxTimerProc(HWND hwnd, ULONG, int nIdTimer, ULONG);

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxTimer, wxEvtHandler)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxTimer class
// ----------------------------------------------------------------------------

void wxTimer::Init()
{
    m_ulId = 0;
}

wxTimer::~wxTimer()
{
    wxTimer::Stop();
}

void wxTimer::Notify()
{
    //
    // The base class version generates an event if it has owner - which it
    // should because otherwise nobody can process timer events, but it does
    // not use the OS's ID, which OS/2 must have to figure out which timer fired
    //
    wxCHECK_RET( m_owner, _T("wxTimer::Notify() should be overridden.") );

    wxTimerEvent                    vEvent( m_idTimer
                                           ,m_milli
                                          );

    (void)m_owner->ProcessEvent(vEvent);
} // end of wxTimer::Notify

bool wxTimer::Start(
  int                               nMilliseconds
, bool                              bOneShot
)
{
    (void)wxTimerBase::Start( nMilliseconds
                             ,bOneShot
                            );

    wxCHECK_MSG( m_milli > 0L, FALSE, wxT("invalid value for timer") );

    wxWindow*                       pWin = NULL;

    if (m_owner)
    {
        pWin = (wxWindow*)m_owner;
        m_ulId = ::WinStartTimer( m_Hab
                                 ,pWin->GetHWND()
                                 ,m_idTimer
                                 ,(ULONG)nMilliseconds
                                );
    }
    else
        m_ulId = ::WinStartTimer( m_Hab
                                 ,NULLHANDLE
                                 ,0
                                 ,(ULONG)nMilliseconds
                                );
    if (m_ulId > 0L)
    {
        // check that SetTimer() didn't reuse an existing id: according to
        // the MSDN this can happen and this would be catastrophic to us as
        // we rely on ids uniquely identifying the timers because we use
        // them as keys in the hash
        if ( TimerMap().find(m_ulId) != TimerMap().end() )
        {
            wxLogError(_("Timer creation failed."));

            ::WinStopTimer(m_Hab, pWin?(pWin->GetHWND()):NULL, m_ulId);
            m_ulId = 0;

            return false;
        }

        TimerMap()[m_ulId] = this;

        return true;
    }
    else
    {
        wxLogSysError(_("Couldn't create a timer"));

        return(FALSE);
    }
}

void wxTimer::Stop()
{
    if ( m_ulId )
    {
        if (m_owner)
        {
            wxWindow*                   pWin = (wxWindow*)m_owner;

            ::WinStopTimer(m_Hab, pWin->GetHWND(), m_ulId);
        }
        else
            ::WinStopTimer(m_Hab, NULLHANDLE, m_ulId);

        TimerMap().erase(m_ulId);
    }
    m_ulId = 0L;
}

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

void wxProcessTimer(
  wxTimer&                          rTimer
)
{
    //
    // Avoid to process spurious timer events
    //
    if (rTimer.m_ulId == 0L)
        return;

    if (rTimer.IsOneShot())
        rTimer.Stop();

    rTimer.Notify();
}

ULONG wxTimerProc(
  HWND                              WXUNUSED(hwnd)
, ULONG
, int                               nIdTimer
, ULONG
)
{
    wxTimerMap::iterator node = TimerMap().find((ULONG)nIdTimer);

    wxCHECK_MSG(node != TimerMap().end(), 0,
                wxT("bogus timer id in wxTimerProc") );
    wxProcessTimer(*(node->second));
    return 0;
}

