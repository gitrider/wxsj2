/////////////////////////////////////////////////////////////////////////////
// Name:        gdiobj.cpp
// Purpose:     wxGDIObject class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: gdiobj.cpp,v 1.3 2004/05/23 20:52:40 JS Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "gdiobj.h"
#endif

#include "wx/gdiobj.h"

#if !USE_SHARED_LIBRARIES
IMPLEMENT_DYNAMIC_CLASS(wxGDIObject, wxObject)
#endif

// TODO: Nothing to do, unless you want to.
