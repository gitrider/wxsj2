/////////////////////////////////////////////////////////////////////////////
// Name:        minifram.cpp
// Purpose:     wxMiniFrame. Optional; identical to wxFrame if not supported.
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: minifram.cpp,v 1.3 2004/05/23 20:52:40 JS Exp $
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "minifram.h"
#endif

#include "wx/minifram.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxMiniFrame, wxFrame)
#endif


