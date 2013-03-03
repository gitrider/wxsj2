/////////////////////////////////////////////////////////////////////////////
// Name:        common/msgout.cpp
// Purpose:     wxMessageOutput implementation
// Author:      Mattia Barbon
// Modified by:
// Created:     17.07.02
// RCS-ID:      $Id: msgout.cpp,v 1.30 2004/12/22 07:22:25 ABX Exp $
// Copyright:   (c) the wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA) && !defined(__EMX__)
// Some older compilers (such as EMX) cannot handle
// #pragma interface/implementation correctly, iff
// #pragma implementation is used in _two_ translation
// units (as created by e.g. event.cpp compiled for
// libwx_base and event.cpp compiled for libwx_gui_core).
// So we must not use those pragmas for those compilers in
// such files.
    #pragma implementation "msgout.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/string.h"
    #include "wx/ffile.h"
    #include "wx/app.h"
    #include "wx/intl.h"
    #if wxUSE_GUI
        #include "wx/msgdlg.h"
    #endif // wxUSE_GUI
#endif

#include "wx/msgout.h"
#include "wx/apptrait.h"
#include "wx/log.h"

#include <stdarg.h>
#include <stdio.h>

#if defined(__WXMSW__)
    #include "wx/msw/private.h"
#endif
#ifdef __WXMAC__
    #include "wx/mac/private.h"
#endif

// ===========================================================================
// implementation
// ===========================================================================

#if wxUSE_BASE

// ----------------------------------------------------------------------------
// wxMessageOutput
// ----------------------------------------------------------------------------

wxMessageOutput* wxMessageOutput::ms_msgOut = 0;

wxMessageOutput* wxMessageOutput::Get()
{
    if ( !ms_msgOut && wxTheApp )
    {
        ms_msgOut = wxTheApp->GetTraits()->CreateMessageOutput();
    }

    return ms_msgOut;
}

wxMessageOutput* wxMessageOutput::Set(wxMessageOutput* msgout)
{
    wxMessageOutput* old = ms_msgOut;
    ms_msgOut = msgout;
    return old;
}

// ----------------------------------------------------------------------------
// wxMessageOutputStderr
// ----------------------------------------------------------------------------

void wxMessageOutputStderr::Printf(const wxChar* format, ...)
{
    va_list args;
    va_start(args, format);
    wxString out;

    out.PrintfV(format, args);
    va_end(args);

    fprintf(stderr, "%s", (const char*) out.mb_str());
}

// ----------------------------------------------------------------------------
// wxMessageOutputDebug
// ----------------------------------------------------------------------------

void wxMessageOutputDebug::Printf(const wxChar* format, ...)
{
    wxString out;

    va_list args;
    va_start(args, format);

    out.PrintfV(format, args);
    va_end(args);

#if defined(__WXMSW__) && !defined(__WXMICROWIN__)
    out.Replace(wxT("\t"), wxT("        "));
    out.Replace(wxT("\n"), wxT("\r\n"));
    ::OutputDebugString(out);
#elif defined(__WXMAC__) && !defined(__DARWIN__)
    if ( wxIsDebuggerRunning() )
    {
        Str255 pstr;
        wxString output = out + wxT(";g") ;
        wxMacStringToPascal(output.c_str(), pstr);

        #ifdef __powerc
            DebugStr(pstr);
        #else
            SysBreakStr(pstr);
        #endif
    }
#else
    wxFputs( out , stderr ) ;
    if ( out.Right(1) != wxT("\n") )
        wxFputs( wxT("\n") , stderr ) ;
    fflush( stderr ) ;
#endif // platform
}

// ----------------------------------------------------------------------------
// wxMessageOutputLog
// ----------------------------------------------------------------------------

void wxMessageOutputLog::Printf(const wxChar* format, ...)
{
    wxString out;

    va_list args;
    va_start(args, format);

    out.PrintfV(format, args);
    va_end(args);

    out.Replace(wxT("\t"), wxT("        "));

    ::wxLogMessage(wxT("%s"), out.c_str());
}

#endif // wxUSE_BASE

// ----------------------------------------------------------------------------
// wxMessageOutputMessageBox
// ----------------------------------------------------------------------------

#if wxUSE_GUI

void wxMessageOutputMessageBox::Printf(const wxChar* format, ...)
{
    va_list args;
    va_start(args, format);
    wxString out;

    out.PrintfV(format, args);
    va_end(args);

    // the native MSW msg box understands the TABs, others don't
#ifndef __WXMSW__
    out.Replace(wxT("\t"), wxT("        "));
#endif

    wxString title;
    if ( wxTheApp )
        title.Printf(_("%s message"), wxTheApp->GetAppName().c_str());

    ::wxMessageBox(out, title);
}

#endif // wxUSE_GUI

