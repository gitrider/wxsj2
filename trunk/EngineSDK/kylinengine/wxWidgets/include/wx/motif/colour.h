/////////////////////////////////////////////////////////////////////////////
// Name:        colour.h
// Purpose:     wxColour class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id: colour.h,v 1.17 2004/05/23 20:51:21 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COLOUR_H_
#define _WX_COLOUR_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "colour.h"
#endif

#include "wx/object.h"
#include "wx/string.h"

// Colour
class WXDLLEXPORT wxColour : public wxObject
{
    DECLARE_DYNAMIC_CLASS(wxColour)
public:
    // ctors
    // default
    wxColour();
    // from RGB
    wxColour( unsigned char red, unsigned char green, unsigned char blue )
        { Set(red, green, blue); }
    wxColour( unsigned long colRGB ) { Set(colRGB); }

    // implicit conversion from the colour name
    wxColour( const wxString &colourName ) { InitFromName(colourName); }
    wxColour( const char *colourName ) { InitFromName(colourName); }

    // copy ctors and assignment operators
    wxColour( const wxColour& col );
    wxColour& operator = ( const wxColour& col );

    // dtor
    ~wxColour();

    // Set() functions
    void Set( unsigned char red, unsigned char green, unsigned char blue );
    void Set( unsigned long colRGB )
    {
        // we don't need to know sizeof(long) here because we assume that the three
        // least significant bytes contain the R, G and B values
        Set((unsigned char)colRGB,
            (unsigned char)(colRGB >> 8),
            (unsigned char)(colRGB >> 16));
    }

    // accessors
    bool Ok() const {return m_isInit; }
    unsigned char Red() const { return m_red; }
    unsigned char Green() const { return m_green; }
    unsigned char Blue() const { return m_blue; }

    int GetPixel() const { return m_pixel; };
    void SetPixel(int pixel) { m_pixel = pixel; m_isInit = true; };

    inline bool operator == (const wxColour& colour) const { return (m_red == colour.m_red && m_green == colour.m_green && m_blue == colour.m_blue); }

    inline bool operator != (const wxColour& colour) const { return (!(m_red == colour.m_red && m_green == colour.m_green && m_blue == colour.m_blue)); }

    // Get colour from name or wxNullColour
    static wxColour CreateByName(const wxString& name);

    // Allocate a colour, or nearest colour, using the given display.
    // If realloc is true, ignore the existing pixel, otherwise just return
    // the existing one.
    // Returns the allocated pixel.

    // TODO: can this handle mono displays? If not, we should have an extra
    // flag to specify whether this should be black or white by default.

    int AllocColour(WXDisplay* display, bool realloc = false);

    void InitFromName(const wxString& col);

protected:
    // Helper function
    void Init();

private:
    bool          m_isInit;
    unsigned char m_red;
    unsigned char m_blue;
    unsigned char m_green;

public:
    int           m_pixel;
};

#endif
// _WX_COLOUR_H_
