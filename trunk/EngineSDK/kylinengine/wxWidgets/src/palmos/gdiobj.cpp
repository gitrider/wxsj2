/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/gdiobj.cpp
// Purpose:     wxGDIObject class
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: gdiobj.cpp,v 1.2 2005/01/21 21:04:02 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "gdiobj.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <stdio.h>
#include "wx/setup.h"
#include "wx/list.h"
#include "wx/utils.h"
#include "wx/app.h"
#endif

#include "wx/gdiobj.h"

IMPLEMENT_DYNAMIC_CLASS(wxGDIObject, wxObject)

/*
void wxGDIObject::IncrementResourceUsage(void)
{
};

void wxGDIObject::DecrementResourceUsage(void)
{
};

*/

