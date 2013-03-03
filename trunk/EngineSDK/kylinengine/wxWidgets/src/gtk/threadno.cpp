/////////////////////////////////////////////////////////////////////////////
// Name:        thread.cpp
// Purpose:     Solaris thread support
// Author:      Guilhem Lavaux
// Modified by:
// Created:     04/22/98
// RCS-ID:      $Id: threadno.cpp,v 1.16 2004/11/03 21:13:19 RR Exp $
// Copyright:   (c) Wolfram Gloger (1996, 1997); Guilhem Lavaux (1998)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "thread.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/thread.h"
#include "wx/wx.h"
#include "wx/module.h"
#include "wx/log.h"

wxMutex::wxMutex()
{
    m_locked = 0;
}

wxMutex::~wxMutex()
{
    if (m_locked)
        wxLogDebug( "wxMutex warning: destroying a locked mutex (%d locks)", m_locked );
}

wxMutexError wxMutex::Lock()
{
    m_locked++;
    return wxMUTEX_NO_ERROR;
}

wxMutexError wxMutex::TryLock()
{
    if (m_locked > 0)
        return wxMUTEX_BUSY;
    m_locked++;
    return wxMUTEX_NO_ERROR;
}

wxMutexError wxMutex::Unlock()
{
    if (m_locked == 0)
        return wxMUTEX_UNLOCKED;
    m_locked--;
    return wxMUTEX_NO_ERROR;
}

wxCondition::wxCondition()
{
}

wxCondition::~wxCondition()
{
}

void wxCondition::Wait(wxMutex& WXUNUSED(mutex))
{
}

bool wxCondition::Wait(wxMutex& WXUNUSED(mutex), unsigned long WXUNUSED(sec),
        unsigned long WXUNUSED(nsec))
{
    return FALSE;
}

void wxCondition::Signal()
{
}

void wxCondition::Broadcast()
{
}

struct wxThreadInternal
{
    int thread_id;
    void* exit_status;
};

wxThreadError wxThread::Create()
{
    p_internal->exit_status = Entry();
    OnExit();
    return wxTHREAD_NO_ERROR;
}

wxThreadError wxThread::Destroy()
{
    return wxTHREAD_NOT_RUNNING;
}

wxThreadError wxThread::Pause()
{
    return wxTHREAD_NOT_RUNNING;
}

wxThreadError wxThread::Resume()
{
    return wxTHREAD_NOT_RUNNING;
}

void wxThread::DeferDestroy( bool WXUNUSED(on) )
{
}

void wxThread::TestDestroy()
{
}

void *wxThread::Join()
{
    return p_internal->exit_status;
}

unsigned long wxThread::GetID() const
{
    return 0;
}

bool wxThread::IsMain()
{
    return TRUE;
}

bool wxThread::IsRunning() const
{
    return FALSE;
}

bool wxThread::IsAlive() const
{
    return FALSE;
}

void wxThread::SetPriority(int WXUNUSED(prio)) { }
int wxThread::GetPriority() const { return 0; }

wxMutex *wxMainMutex; // controls access to all GUI functions

wxThread::wxThread()
{
    p_internal = new wxThreadInternal();
}

wxThread::~wxThread()
{
    Destroy();
    Join();
    delete p_internal;
}

// The default callback just joins the thread and throws away the result.
void wxThread::OnExit()
{
    Join();
}

IMPLEMENT_DYNAMIC_CLASS(wxThreadModule, wxModule)

bool wxThreadModule::OnInit()
{
    wxMainMutex = new wxMutex();
    wxMainMutex->Lock();
    return TRUE;
}

void wxThreadModule::OnExit()
{
    wxMainMutex->Unlock();
    delete wxMainMutex;
}



void wxMutexGuiEnter()
{
}

void wxMutexGuiLeave()
{
}
