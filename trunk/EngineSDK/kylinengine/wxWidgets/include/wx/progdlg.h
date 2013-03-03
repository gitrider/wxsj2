/////////////////////////////////////////////////////////////////////////////
// Name:        wx/progdlg.h
// Purpose:     Base header for wxProgressDialog
// Author:      Julian Smart
// Modified by:
// Created:
// RCS-ID:      $Id: progdlg.h,v 1.3 2005/05/04 18:52:01 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PROGDLG_H_BASE_
#define _WX_PROGDLG_H_BASE_

#include "wx/defs.h"

#ifdef __WXPALMOS__
    #include "wx/palmos/progdlg.h"
#else
    #include "wx/generic/progdlgg.h"
#endif

#endif // _WX_PROGDLG_H_BASE_
