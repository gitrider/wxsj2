///////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/caret.cpp
// Purpose:     Palm OS implementation of wxCaret
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10.13.04
// RCS-ID:      $Id: caret.cpp,v 1.2 2005/01/21 21:04:00 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "caret.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/log.h"
#endif // WX_PRECOMP

#include "wx/caret.h"

#if wxUSE_CARET

#include "wx/palmos/private.h"

// ---------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------

#define CALL_CARET_API(api, args)   \
        if ( !api args )                \
            wxLogLastError(_T(#api))

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// blink time
// ---------------------------------------------------------------------------

//static
int wxCaretBase::GetBlinkTime()
{
    return 0;
}

//static
void wxCaretBase::SetBlinkTime(int milliseconds)
{
}

// ---------------------------------------------------------------------------
// creating/destroying the caret
// ---------------------------------------------------------------------------

bool wxCaret::PalmOSCreateCaret()
{
    return false;
}

void wxCaret::OnSetFocus()
{
}

void wxCaret::OnKillFocus()
{
}

// ---------------------------------------------------------------------------
// showing/hiding the caret
// ---------------------------------------------------------------------------

void wxCaret::DoShow()
{
}

void wxCaret::DoHide()
{
}

// ---------------------------------------------------------------------------
// moving the caret
// ---------------------------------------------------------------------------

void wxCaret::DoMove()
{
}


// ---------------------------------------------------------------------------
// resizing the caret
// ---------------------------------------------------------------------------

void wxCaret::DoSize()
{
}

#endif
