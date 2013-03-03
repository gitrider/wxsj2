///////////////////////////////////////////////////////////////////////////////
// Name:        unix/baseunix.cpp
// Purpose:     misc stuff only used in console applications under Unix
// Author:      Vadim Zeitlin
// Modified by:
// Created:     23.06.2003
// RCS-ID:      $Id: baseunix.cpp,v 1.13 2005/04/25 10:28:59 MW Exp $
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwindows.org>
// License:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include "wx/intl.h"
#endif //WX_PRECOMP

#include "wx/apptrait.h"
#include "wx/utils.h"
#include "wx/unix/execute.h"

// for waitpid()
#include <sys/types.h>
#include <sys/wait.h>

// ============================================================================
// wxConsoleAppTraits implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxExecute support
// ----------------------------------------------------------------------------

bool wxConsoleAppTraits::CreateEndProcessPipe(wxExecuteData& WXUNUSED(data))
{
    // nothing to do, so always ok
    return true;
}

bool
wxConsoleAppTraits::IsWriteFDOfEndProcessPipe(wxExecuteData& WXUNUSED(data),
                                              int WXUNUSED(fd))
{
    // we don't have any pipe
    return false;
}

void
wxConsoleAppTraits::DetachWriteFDOfEndProcessPipe(wxExecuteData& WXUNUSED(data))
{
    // nothing to do
}


int
wxConsoleAppTraits::WaitForChild(wxExecuteData& execData)
{
    wxASSERT_MSG( execData.flags & wxEXEC_SYNC,
                  wxT("async execution not supported yet") );

    int exitcode = 0;
    if ( waitpid(execData.pid, &exitcode, 0) == -1 || !WIFEXITED(exitcode) )
    {
        wxLogSysError(_("Waiting for subprocess termination failed"));
    }

    return exitcode;
}

// ----------------------------------------------------------------------------
// misc other stuff
// ----------------------------------------------------------------------------

// this is in mac/utils.cpp under Mac
#if !defined(__WXMAC__)

wxToolkitInfo& wxConsoleAppTraits::GetToolkitInfo()
{
    static wxToolkitInfo info;
    int major, minor;

    FILE *f = popen("uname -r", "r");
    if (f)
    {
        char buf[32];
        size_t c = fread(buf, 1, sizeof(buf) - 1, f);
        pclose(f);
        buf[c] = '\0';
        if ( sscanf(buf, "%d.%d", &major, &minor) != 2 )
        {
            // unrecognized uname string format
            major =
            minor = -1;
        }
    }
    else
    {
        // failed to run uname
        major =
        minor = -1;
    }

    info.versionMajor = major;
    info.versionMinor = minor;
    info.name = _T("wxBase");
    info.os = wxUNIX;

    return info;
}

#endif // __WXMAC__

