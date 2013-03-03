/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dcclient.h
// Purpose:     wxClientDC base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id: dcclient.h,v 1.13 2005/05/04 18:51:56 JS Exp $
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCCLIENT_H_BASE_
#define _WX_DCCLIENT_H_BASE_

#if defined(__WXPALMOS__)
#include "wx/palmos/dcclient.h"
#elif defined(__WXMSW__)
#include "wx/msw/dcclient.h"
#elif defined(__WXMOTIF__)
#include "wx/motif/dcclient.h"
#elif defined(__WXGTK__)
#include "wx/gtk/dcclient.h"
#elif defined(__WXX11__)
#include "wx/x11/dcclient.h"
#elif defined(__WXMGL__)
#include "wx/mgl/dcclient.h"
#elif defined(__WXMAC__)
#include "wx/mac/dcclient.h"
#elif defined(__WXCOCOA__)
#include "wx/cocoa/dcclient.h"
#elif defined(__WXPM__)
#include "wx/os2/dcclient.h"
#endif

#endif
    // _WX_DCCLIENT_H_BASE_
