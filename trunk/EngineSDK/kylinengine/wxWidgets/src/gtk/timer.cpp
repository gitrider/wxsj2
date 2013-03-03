/////////////////////////////////////////////////////////////////////////////
// Name:        gtk/timer.cpp
// Purpose:     wxTimer implementation
// Author:      Robert Roebling
// Id:          $Id: timer.cpp,v 1.22 2005/03/21 23:42:19 VZ Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "timer.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_TIMER

#include "wx/timer.h"

#include "gtk/gtk.h"

// ----------------------------------------------------------------------------
// wxTimer
// ----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxTimer, wxEvtHandler)

extern "C" {
static gint timeout_callback( gpointer data )
{
    wxTimer *timer = (wxTimer*)data;

    // Don't change the order of anything in this callback!
    
    if (timer->IsOneShot())
    {
        // This sets m_tag to -1
        timer->Stop();
    }
    
    // When getting called from GDK's timer handler we
    // are no longer within GDK's grab on the GUI
    // thread so we must lock it here ourselves.
    gdk_threads_enter();

    timer->Notify();

    // Release lock again.
    gdk_threads_leave();

    if (timer->IsOneShot())
        return FALSE;

    return TRUE;
}
}

void wxTimer::Init()
{
    m_tag = -1;
    m_milli = 1000;
}

wxTimer::~wxTimer()
{
    wxTimer::Stop();
}

bool wxTimer::Start( int millisecs, bool oneShot )
{
    (void)wxTimerBase::Start(millisecs, oneShot);

    if (m_tag != -1)
        gtk_timeout_remove( m_tag );

    m_tag = gtk_timeout_add( m_milli, timeout_callback, this );

    return TRUE;
}

void wxTimer::Stop()
{
    if (m_tag != -1)
    {
        gtk_timeout_remove( m_tag );
        m_tag = -1;
    }
}

#endif // wxUSE_TIMER

