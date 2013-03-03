/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gdiobj.h
// Purpose:     wxGDIObject base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id: gdiobj.h,v 1.13 2005/05/04 18:51:58 JS Exp $
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GDIOBJ_H_BASE_
#define _WX_GDIOBJ_H_BASE_

#if defined(__WXPALMOS__)
#include "wx/palmos/gdiobj.h"
#elif defined(__WXMSW__)
#include "wx/msw/gdiobj.h"
#elif defined(__WXMOTIF__)
#include "wx/motif/gdiobj.h"
#elif defined(__WXGTK__)
#include "wx/gtk/gdiobj.h"
#elif defined(__WXX11__)
#include "wx/x11/gdiobj.h"
#elif defined(__WXMGL__)
#include "wx/mgl/gdiobj.h"
#elif defined(__WXMAC__)
#include "wx/mac/gdiobj.h"
#elif defined(__WXCOCOA__)
#include "wx/cocoa/gdiobj.h"
#elif defined(__WXPM__)
#include "wx/os2/gdiobj.h"
#endif

#endif
    // _WX_GDIOBJ_H_BASE_
