/////////////////////////////////////////////////////////////////////////////
// Name:        utils.cpp
// Purpose:     Various utilities
// Author:      AUTHOR
// Modified by:
// Created:     2003/??/??
// RCS-ID:      $Id: utils.cpp,v 1.11 2005/05/23 12:26:15 ABX Exp $
// Copyright:   (c) AUTHOR
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/setup.h"
#include "wx/utils.h"
#include "wx/app.h"
#include "wx/apptrait.h"
#include "wx/display.h"

#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void wxDisplaySize(int *width, int *height)
{
    // TODO
    if(width)
        *width = 1024;
    if(height)
        *height = 768;
}

void wxDisplaySizeMM(int*,int*)
{
    // TODO
}

void wxClientDisplayRect(int *x,int *y,int *width,int *height)
{
    // TODO
    if(x)
        *x = 0;
    if(y)
        *y = 0;
    if(width)
        *width=1024;
    if(height)
        *height=768;
}

wxToolkitInfo& wxGUIAppTraits::GetToolkitInfo()
{
    static wxToolkitInfo info;
    info.shortName = _T("cocoa");
    info.name = _T("wxCocoa");
    // TODO: Finish this
    return info;
}

wxWindow* wxFindWindowAtPoint(const wxPoint& pt)
{
    return wxGenericFindWindowAtPoint(pt);
}

// Return true if we have a colour display
bool wxColourDisplay()
{
    // TODO
    return true;
}

void wxGetMousePosition( int* x, int* y )
{
    // TODO
};

// Returns depth of screen
int wxDisplayDepth()
{
    // TODO
    return 0;
}

// Emit a beeeeeep
void wxBell()
{
    // TODO
}

#if 0
// DFE: These aren't even implemented by wxGTK, and no wxWidgets code calls
// them.  If someone needs them, then they'll get a link error

// Consume all events until no more left
void wxFlushEvents()
{
}

// Check whether this window wants to process messages, e.g. Stop button
// in long calculations.
bool wxCheckForInterrupt(wxWindow *wnd)
{
    // TODO
    return false;
}

#endif

// Reading and writing resources (eg WIN.INI, .Xdefaults)
#if wxUSE_RESOURCES
bool wxWriteResource(const wxString& section, const wxString& entry, const wxString& value, const wxString& file)
{
    // TODO
    return false;
}

bool wxWriteResource(const wxString& section, const wxString& entry, float value, const wxString& file)
{
    char buf[50];
    sprintf(buf, "%.4f", value);
    return wxWriteResource(section, entry, buf, file);
}

bool wxWriteResource(const wxString& section, const wxString& entry, long value, const wxString& file)
{
    char buf[50];
    sprintf(buf, "%ld", value);
    return wxWriteResource(section, entry, buf, file);
}

bool wxWriteResource(const wxString& section, const wxString& entry, int value, const wxString& file)
{
    char buf[50];
    sprintf(buf, "%d", value);
    return wxWriteResource(section, entry, buf, file);
}

bool wxGetResource(const wxString& section, const wxString& entry, char **value, const wxString& file)
{
    // TODO
    return false;
}

bool wxGetResource(const wxString& section, const wxString& entry, float *value, const wxString& file)
{
    char *s = NULL;
    bool succ = wxGetResource(section, entry, (char **)&s, file);
    if (succ)
    {
        *value = (float)strtod(s, NULL);
        delete[] s;
        return true;
    }
    else return false;
}

bool wxGetResource(const wxString& section, const wxString& entry, long *value, const wxString& file)
{
    char *s = NULL;
    bool succ = wxGetResource(section, entry, (char **)&s, file);
    if (succ)
    {
        *value = strtol(s, NULL, 10);
        delete[] s;
        return true;
    }
    else return false;
}

bool wxGetResource(const wxString& section, const wxString& entry, int *value, const wxString& file)
{
    char *s = NULL;
    bool succ = wxGetResource(section, entry, (char **)&s, file);
    if (succ)
    {
        *value = (int)strtol(s, NULL, 10);
        delete[] s;
        return true;
    }
    else return false;
}
#endif // wxUSE_RESOURCES
