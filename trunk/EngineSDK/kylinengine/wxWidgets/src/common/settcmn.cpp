/////////////////////////////////////////////////////////////////////////////
// Name:        common/settcmn.cpp
// Purpose:     common (to all ports) wxWindow functions
// Author:      Robert Roebling
// RCS-ID:      $Id: settcmn.cpp,v 1.10 2005/01/04 19:28:31 ABX Exp $
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
    #pragma implementation "settings.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/defs.h"
    #include "wx/utils.h"
    #include "wx/settings.h"
#endif //WX_PRECOMP

// ----------------------------------------------------------------------------
// static data
// ----------------------------------------------------------------------------

wxSystemScreenType wxSystemSettings::ms_screen = wxSYS_SCREEN_NONE;

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

wxSystemScreenType wxSystemSettings::GetScreenType()
{
    if (ms_screen == wxSYS_SCREEN_NONE)
    {
        // wxUniv will be used on small devices, too.
        int x = GetMetric( wxSYS_SCREEN_X );

        ms_screen = wxSYS_SCREEN_DESKTOP;

        if (x < 800)
            ms_screen = wxSYS_SCREEN_SMALL;

        if (x < 640)
            ms_screen = wxSYS_SCREEN_PDA;

        if (x < 200)
            ms_screen = wxSYS_SCREEN_TINY;

        // This is probably a bug, but VNC seems to report 0
        if (x < 10)
            ms_screen = wxSYS_SCREEN_DESKTOP;
    }

    return ms_screen;
}

void wxSystemSettings::SetScreenType( wxSystemScreenType screen )
{
    ms_screen = screen;
}

#if WXWIN_COMPATIBILITY_2_4

wxColour wxSystemSettings::GetSystemColour(int index)
{
    return GetColour((wxSystemColour)index);
}

wxFont wxSystemSettings::GetSystemFont(int index)
{
    return GetFont((wxSystemFont)index);
}

int wxSystemSettings::GetSystemMetric(int index)
{
    return GetMetric((wxSystemMetric)index);
}

#endif // WXWIN_COMPATIBILITY_2_4
