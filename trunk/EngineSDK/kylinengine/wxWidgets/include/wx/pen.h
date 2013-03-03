/////////////////////////////////////////////////////////////////////////////
// Name:        wx/pen.h
// Purpose:     Base header for wxPen
// Author:      Julian Smart
// Modified by:
// Created:
// RCS-ID:      $Id: pen.h,v 1.15 2005/05/04 18:52:01 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PEN_H_BASE_
#define _WX_PEN_H_BASE_

#if defined(__WXPALMOS__)
#include "wx/palmos/pen.h"
#elif defined(__WXMSW__)
#include "wx/msw/pen.h"
#elif defined(__WXMOTIF__) || defined(__WXX11__)
#include "wx/x11/pen.h"
#elif defined(__WXGTK__)
#include "wx/gtk/pen.h"
#elif defined(__WXMGL__)
#include "wx/mgl/pen.h"
#elif defined(__WXMAC__)
#include "wx/mac/pen.h"
#elif defined(__WXCOCOA__)
#include "wx/cocoa/pen.h"
#elif defined(__WXPM__)
#include "wx/os2/pen.h"
#endif

#endif
    // _WX_PEN_H_BASE_
