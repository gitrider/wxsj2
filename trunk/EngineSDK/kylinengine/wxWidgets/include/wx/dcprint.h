/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dcprint.h
// Purpose:     wxPrinterDC base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id: dcprint.h,v 1.8 2005/05/04 18:51:56 JS Exp $
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCPRINT_H_BASE_
#define _WX_DCPRINT_H_BASE_

#if defined(__WXPALMOS__)
#include "wx/palmos/dcprint.h"
#elif defined(__WXMSW__)
#include "wx/msw/dcprint.h"
#endif
#if defined(__WXPM__)
#include "wx/os2/dcprint.h"
#endif
#if defined(__WXMAC__)
#include "wx/mac/dcprint.h"
#endif

#endif
    // _WX_DCPRINT_H_BASE_
