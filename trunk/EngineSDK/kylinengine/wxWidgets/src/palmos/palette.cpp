/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/palette.cpp
// Purpose:     wxPalette
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by:
// Created:     10/13/04
// RCS-ID:      $Id: palette.cpp,v 1.2 2005/01/21 21:04:03 ABX Exp $
// Copyright:   (c) William Osborne
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "palette.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_PALETTE

#ifndef WX_PRECOMP
    #include "wx/palette.h"
#endif

#include "wx/palmos/private.h"

IMPLEMENT_DYNAMIC_CLASS(wxPalette, wxGDIObject)

/*
 * Palette
 *
 */

wxPaletteRefData::wxPaletteRefData(void)
{
}

wxPaletteRefData::~wxPaletteRefData(void)
{
}

wxPalette::wxPalette(void)
{
}

wxPalette::wxPalette(int n, const unsigned char *red, const unsigned char *green, const unsigned char *blue)
{
}

wxPalette::~wxPalette(void)
{
}

bool wxPalette::FreeResource(bool WXUNUSED(force))
{
    return false;
}

bool wxPalette::Create(int n, const unsigned char *red, const unsigned char *green, const unsigned char *blue)
{
    return false;
}

int wxPalette::GetPixel(const unsigned char red, const unsigned char green, const unsigned char blue) const
{
    return 0;
}

bool wxPalette::GetRGB(int index, unsigned char *red, unsigned char *green, unsigned char *blue) const
{
    return false;
}

void wxPalette::SetHPALETTE(WXHPALETTE pal)
{
}

#endif // wxUSE_PALETTE

