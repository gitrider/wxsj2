/////////////////////////////////////////////////////////////////////////////
// Name:        gdiobj.cpp
// Purpose:     wxGDIObject class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: gdiobj.cpp,v 1.9 2005/01/27 21:01:31 SC Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "gdiobj.h"
#endif

#include "wx/wxprec.h"

#include "wx/gdiobj.h"

#if !USE_SHARED_LIBRARIES
IMPLEMENT_DYNAMIC_CLASS(wxGDIObject, wxObject)
#endif

// TODO: Nothing to do, unless you want to.
