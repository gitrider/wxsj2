/////////////////////////////////////////////////////////////////////////////
// Name:        palette.h
// Purpose:
// Author:      Vaclav Slavik
// Created:     2001/03/11
// Id:          $Id: palette.h,v 1.8 2004/12/08 17:42:35 ABX Exp $
// Copyright:   (c) 2001-2002 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __WX_PALETTE_H__
#define __WX_PALETTE_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "palette.h"
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/gdiobj.h"
#include "wx/gdicmn.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxPalette;
struct palette_t;

//-----------------------------------------------------------------------------
// wxPalette
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxPalette: public wxPaletteBase
{
    DECLARE_DYNAMIC_CLASS(wxPalette)

public:
    wxPalette();
    wxPalette(int n, const unsigned char *red, const unsigned char *green, const unsigned char *blue);
    wxPalette(const wxPalette& palette);
    ~wxPalette();
    wxPalette& operator = (const wxPalette& palette);
    bool operator == (const wxPalette& palette) const;
    bool operator != (const wxPalette& palette) const;
    virtual bool Ok() const;

    bool Create(int n, const unsigned char *red, const unsigned char *green, const unsigned char *blue);
    int GetPixel(const unsigned char red, const unsigned char green, const unsigned char blue) const;
    bool GetRGB(int pixel, unsigned char *red, unsigned char *green, unsigned char *blue) const;

    // implementation
    virtual int GetColoursCount() const;

    palette_t *GetMGLpalette_t() const;
};

#endif // __WX_PALETTE_H__
