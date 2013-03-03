/////////////////////////////////////////////////////////////////////////////
// Name:        utils.cpp
// Purpose:     Various utilities
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: utils.cpp,v 1.23 2004/05/23 20:53:31 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/setup.h"
#include "wx/utils.h"
#include "wx/app.h"
#include "wx/apptrait.h"
#include "wx/msgdlg.h"
#include "wx/cursor.h"
#include "wx/window.h" // for wxTopLevelWindows

#include <ctype.h>
#include <stdarg.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>
// #include <netdb.h>
#include <signal.h>

#if (defined(__SUNCC__) || defined(__CLCC__))
    #include <sysent.h>
#endif

#ifdef __VMS__
#pragma message disable nosimpint
#endif

#include "wx/unix/execute.h"

#include "wx/x11/private.h"

#include "X11/Xutil.h"

#ifdef __VMS__
#pragma message enable nosimpint
#endif

// ----------------------------------------------------------------------------
// async event processing
// ----------------------------------------------------------------------------

// Consume all events until no more left
void wxFlushEvents()
{
    Display *display = (Display*) wxGetDisplay();

    XSync (display, FALSE);

    // TODO for X11
    // ??
}

bool wxCheckForInterrupt(wxWindow *wnd)
{
    wxASSERT_MSG(FALSE, wxT("wxCheckForInterrupt not yet implemented."));
    return FALSE;
}

// ----------------------------------------------------------------------------
// wxExecute stuff
// ----------------------------------------------------------------------------

int wxAddProcessCallback(wxEndProcessData *proc_data, int fd)
{
    // TODO
    return 0;
}

// ----------------------------------------------------------------------------
// misc
// ----------------------------------------------------------------------------

// Emit a beeeeeep
#ifndef __EMX__
// on OS/2, we use the wxBell from wxBase library (src/os2/utils.cpp)
void wxBell()
{
    // Use current setting for the bell
    XBell ((Display*) wxGetDisplay(), 0);
}
#endif

wxToolkitInfo& wxGUIAppTraits::GetToolkitInfo()
{
    static wxToolkitInfo info;
    info.shortName = _T("x11univ");
    info.name = _T("wxX11");
    info.versionMajor = 0;
    info.versionMinor = 0;
    info.os = wxX11;
    return info;
}

// ----------------------------------------------------------------------------
// display info
// ----------------------------------------------------------------------------

void wxGetMousePosition( int* x, int* y )
{
#if wxUSE_NANOX
    // TODO
    *x = 0;
    *y = 0;
#else
    XMotionEvent xev;
    Window root, child;
    XQueryPointer((Display*) wxGetDisplay(),
                  DefaultRootWindow((Display*) wxGetDisplay()),
                  &root, &child,
                  &(xev.x_root), &(xev.y_root),
                  &(xev.x),      &(xev.y),
                  &(xev.state));
    *x = xev.x_root;
    *y = xev.y_root;
#endif
};

// Return TRUE if we have a colour display
bool wxColourDisplay()
{
    return wxDisplayDepth() > 1;
}

// Returns depth of screen
int wxDisplayDepth()
{
    Display *dpy = (Display*) wxGetDisplay();

    return DefaultDepth (dpy, DefaultScreen (dpy));
}

// Get size of display
void wxDisplaySize(int *width, int *height)
{
    Display *dpy = (Display*) wxGetDisplay();

    if ( width )
        *width = DisplayWidth (dpy, DefaultScreen (dpy));
    if ( height )
        *height = DisplayHeight (dpy, DefaultScreen (dpy));
}

void wxDisplaySizeMM(int *width, int *height)
{
    Display *dpy = (Display*) wxGetDisplay();

    if ( width )
        *width = DisplayWidthMM(dpy, DefaultScreen (dpy));
    if ( height )
        *height = DisplayHeightMM(dpy, DefaultScreen (dpy));
}

void wxClientDisplayRect(int *x, int *y, int *width, int *height)
{
    // This is supposed to return desktop dimensions minus any window
    // manager panels, menus, taskbars, etc.  If there is a way to do that
    // for this platform please fix this function, otherwise it defaults
    // to the entire desktop.
    if (x) *x = 0;
    if (y) *y = 0;
    wxDisplaySize(width, height);
}


// Configurable display in wxX11 and wxMotif
static WXDisplay *gs_currentDisplay = NULL;
static wxString gs_displayName;

WXDisplay *wxGetDisplay()
{
    if (gs_currentDisplay)
        return gs_currentDisplay;
    return wxApp::GetDisplay();
}

bool wxSetDisplay(const wxString& display_name)
{
    gs_displayName = display_name;

    if ( display_name.IsEmpty() )
    {
        gs_currentDisplay = NULL;

        return TRUE;
    }
    else
    {
        Display* display = XOpenDisplay((char*) display_name.c_str());

        if (display)
        {
            gs_currentDisplay = (WXDisplay*) display;
            return TRUE;
        }
        else
            return FALSE;
    }
}

wxString wxGetDisplayName()
{
    return gs_displayName;
}

wxWindow* wxFindWindowAtPoint(const wxPoint& pt)
{
    return wxGenericFindWindowAtPoint(pt);
}

// ----------------------------------------------------------------------------
// Some colour manipulation routines
// ----------------------------------------------------------------------------

void wxHSVToXColor(wxHSV *hsv,XColor *rgb)
{
    int h = hsv->h;
    int s = hsv->s;
    int v = hsv->v;
    int r = 0, g = 0, b = 0;
    int i, f;
    int p, q, t;
    s = (s * wxMAX_RGB) / wxMAX_SV;
    v = (v * wxMAX_RGB) / wxMAX_SV;
    if (h == 360) h = 0;
    if (s == 0) { h = 0; r = g = b = v; }
    i = h / 60;
    f = h % 60;
    p = v * (wxMAX_RGB - s) / wxMAX_RGB;
    q = v * (wxMAX_RGB - s * f / 60) / wxMAX_RGB;
    t = v * (wxMAX_RGB - s * (60 - f) / 60) / wxMAX_RGB;
    switch (i)
    {
    case 0: r = v, g = t, b = p; break;
    case 1: r = q, g = v, b = p; break;
    case 2: r = p, g = v, b = t; break;
    case 3: r = p, g = q, b = v; break;
    case 4: r = t, g = p, b = v; break;
    case 5: r = v, g = p, b = q; break;
    }
    rgb->red = r << 8;
    rgb->green = g << 8;
    rgb->blue = b << 8;
}

void wxXColorToHSV(wxHSV *hsv,XColor *rgb)
{
    int r = rgb->red >> 8;
    int g = rgb->green >> 8;
    int b = rgb->blue >> 8;
    int maxv = wxMax3(r, g, b);
    int minv = wxMin3(r, g, b);
    int h = 0, s, v;
    v = maxv;
    if (maxv) s = (maxv - minv) * wxMAX_RGB / maxv;
    else s = 0;
    if (s == 0) h = 0;
    else
    {
        int rc, gc, bc, hex = 0;
        rc = (maxv - r) * wxMAX_RGB / (maxv - minv);
        gc = (maxv - g) * wxMAX_RGB / (maxv - minv);
        bc = (maxv - b) * wxMAX_RGB / (maxv - minv);
        if (r == maxv) { h = bc - gc, hex = 0; }
        else if (g == maxv) { h = rc - bc, hex = 2; }
        else if (b == maxv) { h = gc - rc, hex = 4; }
        h = hex * 60 + (h * 60 / wxMAX_RGB);
        if (h < 0) h += 360;
    }
    hsv->h = h;
    hsv->s = (s * wxMAX_SV) / wxMAX_RGB;
    hsv->v = (v * wxMAX_SV) / wxMAX_RGB;
}

void wxAllocNearestColor(Display *d,Colormap cmp,XColor *xc)
{
#if !wxUSE_NANOX
    int llp;

    int screen = DefaultScreen(d);
    int num_colors = DisplayCells(d,screen);

    XColor *color_defs = new XColor[num_colors];
    for(llp = 0;llp < num_colors;llp++) color_defs[llp].pixel = llp;
    XQueryColors(d,cmp,color_defs,num_colors);

    wxHSV hsv_defs, hsv;
    wxXColorToHSV(&hsv,xc);

    int diff, min_diff = 0, pixel = 0;

    for(llp = 0;llp < num_colors;llp++)
    {
        wxXColorToHSV(&hsv_defs,&color_defs[llp]);
        diff = wxSIGN(wxH_WEIGHT * (hsv.h - hsv_defs.h)) +
            wxSIGN(wxS_WEIGHT * (hsv.s - hsv_defs.s)) +
            wxSIGN(wxV_WEIGHT * (hsv.v - hsv_defs.v));
        if (llp == 0) min_diff = diff;
        if (min_diff > diff) { min_diff = diff; pixel = llp; }
        if (min_diff == 0) break;
    }

    xc -> red = color_defs[pixel].red;
    xc -> green = color_defs[pixel].green;
    xc -> blue = color_defs[pixel].blue;
    xc -> flags = DoRed | DoGreen | DoBlue;

/*  FIXME, TODO
    if (!XAllocColor(d,cmp,xc))
        cout << "wxAllocNearestColor : Warning : Cannot find nearest color !\n";
*/

    delete[] color_defs;
#endif
}

void wxAllocColor(Display *d,Colormap cmp,XColor *xc)
{
    if (!XAllocColor(d,cmp,xc))
    {
        //          cout << "wxAllocColor : Warning : Can not allocate color, attempt find nearest !\n";
        wxAllocNearestColor(d,cmp,xc);
    }
}

#ifdef __WXDEBUG__
wxString wxGetXEventName(XEvent& event)
{
#if wxUSE_NANOX
    wxString str(wxT("(some event)"));
    return str;
#else
    int type = event.xany.type;
	    static char* event_name[] = {
		"", "unknown(-)",                                         // 0-1
		"KeyPress", "KeyRelease", "ButtonPress", "ButtonRelease", // 2-5
		"MotionNotify", "EnterNotify", "LeaveNotify", "FocusIn",  // 6-9
		"FocusOut", "KeymapNotify", "Expose", "GraphicsExpose",   // 10-13
		"NoExpose", "VisibilityNotify", "CreateNotify",           // 14-16
		"DestroyNotify", "UnmapNotify", "MapNotify", "MapRequest",// 17-20
		"ReparentNotify", "ConfigureNotify", "ConfigureRequest",  // 21-23
		"GravityNotify", "ResizeRequest", "CirculateNotify",      // 24-26
		"CirculateRequest", "PropertyNotify", "SelectionClear",   // 27-29
		"SelectionRequest", "SelectionNotify", "ColormapNotify",  // 30-32
		"ClientMessage", "MappingNotify",                         // 33-34
		"unknown(+)"};                                            // 35
	    type = wxMin(35, type); type = wxMax(1, type);
        return wxString::FromAscii(event_name[type]);
#endif
}
#endif

bool wxWindowIsVisible(Window win)
{
    XWindowAttributes wa;
    XGetWindowAttributes(wxGlobalDisplay(), win, &wa);

    return (wa.map_state == IsViewable);
}
