/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/nativdlg.cpp
// Purpose:     Native dialog loading code (part of wxWindow)
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: nativdlg.cpp,v 1.3 2005/02/14 10:49:01 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <stdio.h>

    #include "wx/wx.h"
#endif

#include "wx/spinbutt.h"

// ---------------------------------------------------------------------------
// global functions
// ---------------------------------------------------------------------------

// ===========================================================================
// implementation
// ===========================================================================

bool wxWindow::LoadNativeDialog(wxWindow* parent, wxWindowID& id)
{
    return false;
}

bool wxWindow::LoadNativeDialog(wxWindow* parent, const wxString& name)
{
    return false;
}

// ---------------------------------------------------------------------------
// look for child by id
// ---------------------------------------------------------------------------

wxWindow* wxWindow::GetWindowChild1(wxWindowID id)
{
    return NULL;
}

wxWindow* wxWindow::GetWindowChild(wxWindowID id)
{
    return NULL;
}

// ---------------------------------------------------------------------------
// create wxWin window from a native HWND
// ---------------------------------------------------------------------------

wxWindow* wxWindow::CreateWindowFromWinHandle(wxWindow* parent, WXWINHANDLE handle)
{
    return NULL;
}

// Make sure the window style (etc.) reflects the WinHandle style (roughly)
void wxWindow::AdoptAttributesFromWinHandle(void)
{
}

